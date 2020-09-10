/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#ifndef _SNR_MSRS_HEADER_
#define _SNR_MSRS_HEADER_

/**
  Enhanced MCA Core error injection control
**/
#define SNR_MSR_VIRT_CR_THREAD_SMI_ERR_INJ_CONTROL 0x0000002B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Data : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_THREAD_SMI_ERR_INJ_CONTROL_REGISTER;

/**
  Enhanced MCA Core error injection control
**/
#define SNR_MSR_VIRT_CR_CORE_SMI_ERR_INJ_CONTROL 0x0000002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Mc0 : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Mc1 : 1;
    //Bits[2:2] Access Type=WO
    UINT32 Mc2 : 1;
    //Bits[3:3] Access Type=WO
    UINT32 Mc3 : 1;
    //Bits[7:4] Access Type=RO
    UINT32 Rsvd4 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_CORE_SMI_ERR_INJ_CONTROL_REGISTER;

/**
  Enhanced MCA Uncore error injection control
**/
#define SNR_MSR_VIRT_CR_UNCORE_SMI_ERR_INJ_CONTROL 0x0000002D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RO
    UINT32 Rsvd0 : 4;
    //Bits[31:4] Access Type=RW
    UINT32 Allbanks : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_UNCORE_SMI_ERR_INJ_CONTROL_REGISTER;

/**
  MSR used to enable/disable TPR messages. See NHM-B bug 2425138.
**/
#define SNR_MSR_UCODE_CR_PIC_MSG_CONTROL 0x0000002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=RO
    UINT32 Rsvd0 : 10;
    //Bits[10:10] Access Type=RW
    UINT32 TprMsgOff : 1;
    //Bits[15:11] Access Type=RO
    UINT32 Rsvd11 : 5;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_PIC_MSG_CONTROL_REGISTER;

/**
  DCU_TEST_CTL MSR. Ucode mirrors the appropriate split lock disable bit and this entry is used only to generate the proper address defines
**/
#define SNR_MSR_UCODE_CR_MEMORY_CONTROL 0x00000033

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[28:0] Access Type=RO
    UINT32 Rsvd0 : 29;
    //Bits[29:29] Access Type=RW
    UINT32 SplitLockDisable : 1;
    //Bits[30:30] Access Type=RO
    UINT32 Rsvd30 : 1;
    //Bits[31:31] Access Type=RW
    UINT32 SuppressLockEnable : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_MEMORY_CONTROL_REGISTER;

/**
  SLM PerfMon:Number of times SMI was entered.
**/
#define SNR_MSR_SCP_CR_SMI_COUNT 0x00000034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 SmmEntryCount : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_SMI_COUNT_REGISTER;

/**
  determine the current count of enabled Cores and Threads. The Core Count and ThreadCount fields reflect the enabled cores and threads based on the factory-configured core
**/
#define SNR_MSR_UCODE_CR_CORE_THREAD_COUNT 0x00000035

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RO
    UINT32 ThreadCount : 16;
    //Bits[31:16] Access Type=RO
    UINT32 CoreCount : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_CORE_THREAD_COUNT_REGISTER;

/**
  GLP HSDES 1202197475: package-scoped MSR. Only bits 0/1 are actual storage. An entire byte is reserved for port_in/out granularity. Protected Processor Inventory Number Enable Control (R/W). The reset value is 0. It will not survive warm reset.
**/
#define SNR_MSR_FUNNYIO_CR_SCP_PPIN_CTL 0x0000004E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Lock : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Enable : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SCP_PPIN_CTL_REGISTER;

/**
  Enhanced MCA Control - SLM D0 e3430879 / GLM i1958673: Microserver MCA Enhancements. Mapped by ucode into MSR 0x052. No ERROR_CLOAKING fuse for TNT
**/
#define SNR_MSR_FUNNYIO_CR_SCP_SMM_MCA_CONTROL 0x00000052

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 CerrRdStatusInSmmOnly : 1;
    //Bits[1:1] Access Type=RW
    UINT32 UcnaRdStatusInSmmOnly : 1;
    //Bits[2:2] Access Type=RW
    UINT32 DualSignalSmi : 1;
    //Bits[7:3] Access Type=RO
    UINT32 Rsvd3 : 5;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SCP_SMM_MCA_CONTROL_REGISTER;

/**
  determine the current count of enabled Cores and Threads. The Core Count and ThreadCount fields reflect the enabled cores and threads based on the factory-configured core
**/
#define SNR_MSR_VIRT_CR_THD_LOGICAL_ID 0x00000053

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RO
    UINT32 LogicalId : 6;
    //Bits[7:6] Access Type=RO
    UINT32 Rsvd6 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_THD_LOGICAL_ID_REGISTER;

/**
  SMM Write only server MSR
**/
#define SNR_MSR_VIRT_CR_SMM_CLEAR_SMI 0x00000057

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 ClearSmi : 1;
    //Bits[1:1] Access Type=WO
    UINT32 MsmiToMceDisable : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_SMM_CLEAR_SMI_REGISTER;

/**
  Server only SMM MSR
**/
#define SNR_MSR_VIRT_CR_SMM_CFG_OPTIONS_SRVR 0x00000058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 SmmWfsCtl : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_SMM_CFG_OPTIONS_SRVR_REGISTER;

/**
  
**/
#define SNR_MSR_PCU_CR_THERM_EVENT_FFM 0x0000005E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 EnableFfm : 1;
    //Bits[1:1] Access Type=RWOC
    UINT32 FfmSmiSignaled : 1;
    //Bits[31:2] Access Type=RO
    UINT32 Rsvd2 : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_THERM_EVENT_FFM_REGISTER;
/**
  This is special creg for Probe Mode, used for issuing LT Special Cycles to the chipset in RED unlocked part
**/
#define SNR_MSR_UCODE_CR_PROBE_LT_SPC_CYC 0x00000076
/**
  writing to this write-only MSR the address of the Patch causes a patch loading (no CR).
**/
#define SNR_MSR_UCODE_CR_PATCH_MATCH_REG 0x00000079

/**
  implementation simply ignores the wrmsr (XuCode would intercept it
**/
#define SNR_MSR_UCODE_CR_FEATURE_ACTIVATION_REG 0x0000007A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Se : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_FEATURE_ACTIVATION_REG_REGISTER;

/**
  GLP e1404625920: Holds state for MSR 0x80 which is the core specific AET NPK MMIO address. This MSR could have been package scope since BIOS only writes 1 value and locks it acorss cores. But wasting thread scope storage for compatibility with Core.
**/
#define SNR_MSR_C6_SCP_CR_NPK_STH_ACPIBAR_BASE 0x00000080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Lock : 1;
    //Bits[17:1] Access Type=RO
    UINT32 Rsvd1 : 17;
    //Bits[41:18] Access Type=RW
    UINT32 BarAddress : 24;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_NPK_STH_ACPIBAR_BASE_REGISTER;

/**
  FZM Domain ConfigValue Read Only
**/
#define SNR_MSR_VIRT_CR_FZM_DOMAIN_CONFIG 0x00000083

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RO
    UINT32 DomainIndex : 8;
    //Bits[39:8] Access Type=RO
    UINT32 DomainHandleLow0 : 32;
    //Bits[63:40] Access Type=RO
    UINT32 DomainHandle : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_FZM_DOMAIN_CONFIG_REGISTER;
/**
  FZM Domain Start Address - Read Only
**/
#define SNR_MSR_VIRT_CR_FZM_RANGE_STARTADDR 0x00000084
/**
  FZM Domain End Address - Read Only
**/
#define SNR_MSR_VIRT_CR_FZM_RANGE_ENDADDR 0x00000085

/**
  FZM Domain Write Status - Read Only
**/
#define SNR_MSR_VIRT_CR_FZM_RANGE_WRITESTATUS 0x00000086

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 WriteStatus : 1;
    //Bits[1:1] Access Type=RO
    UINT32 ReadStatus : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_FZM_RANGE_WRITESTATUS_REGISTER;

/**
  Patch Sign ID MSR. Mapped to h32 of a SCP
**/
#define SNR_MSR_UCODE_CR_PATCH_SIGN_ID 0x0000008B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=Rsvd0
    UINT32 Rsvd0 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 PatchSignId : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_PATCH_SIGN_ID_REGISTER;

/**
  This MSR is writeable only in SMM mode, else uCode generates #GP.
**/
#define SNR_MSR_SCP_CR_LT_MSEG_BASE_ADDRESS 0x0000009B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Valid : 1;
    //Bits[1:1] Access Type=RW
    UINT32 CstateSmiOptin : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Vmxoffctrl : 1;
    //Bits[11:3] Access Type=RW
    UINT32 RsvdL32 : 9;
    //Bits[31:12] Access Type=RW
    UINT32 MsegBasePageFrameAdr : 20;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_LT_MSEG_BASE_ADDRESS_REGISTER;

/**
  SMM Protected Mode Base MSR: the C6 MSR leaf depends on this address being the first C6 address with MSR access
**/
#define SNR_MSR_C6_SCP_CR_SMM_PROT_MODE_BASE 0x0000009D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Enable : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;
    //Bits[31:8] Access Type=RW
    UINT32 SmmsegPa : 24;
    //Bits[63:32] Access Type=RO
    UINT32 Rsvd32 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_SMM_PROT_MODE_BASE_REGISTER;

/**
  SLM e3430273:Read only MSR. WRMSR will always #GP. RDMSR will GP if not in SMM mode else read the SCP_CR_SMBASE
**/
#define SNR_MSR_VIRT_CR_SMBASE 0x0000009E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Data : 32;
    //Bits[63:32] Access Type=RO
    UINT32 Rsvd32 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_SMBASE_REGISTER;

/**
  MCHECK Error Code
**/
#define SNR_MSR_FUNNYIO_CR_SCP_MCHECK_ERR_CODE 0x000000A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 Data : 16;
    //Bits[30:16] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[31:31] Access Type=RW
    UINT32 PatchloadPartialSuccessIndication : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SCP_MCHECK_ERR_CODE_REGISTER;
/**
  Package registration ID 0
**/
#define SNR_MSR_CORE_INTERFACE_CR_SGX_PRID0 0x000000A1
/**
  Package registration ID 1
**/
#define SNR_MSR_CORE_INTERFACE_CR_SGX_PRID1 0x000000A2

/**
  
**/
#define SNR_MSR_VIRT_CR_SGX_RAS 0x000000A3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Optin : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_SGX_RAS_REGISTER;

/**
  FIT BIOS and ACM error type and code
**/
#define SNR_MSR_UCODE_CR_FIT_BIOS_ERROR 0x000000A5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 ErrorCode : 8;
    //Bits[15:8] Access Type=RW
    UINT32 EntryType : 8;
    //Bits[16:16] Access Type=RW
    UINT32 FitPatchEntry : 1;
    //Bits[48:17] Access Type=RW
    UINT32 BiosUseLow0 : 32;
    //Bits[62:49] Access Type=RW
    UINT32 BiosUse : 14;
    //Bits[63:63] Access Type=RW
    UINT32 Lock : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_FIT_BIOS_ERROR_REGISTER;

/**
  There was a decision to fuse off PL (aka MPX) in CNL and ICL, but not to remove it.  There is a new request from ISAs to add an option for BIOS to enable PL even if it's fused off
**/
#define SNR_MSR_UCODE_CR_MPX_FUSE_OVERRIDE 0x000000A6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 MpxEnabled : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_MPX_FUSE_OVERRIDE_REGISTER;
/**
  vMSRs for power mgmt
**/
#define SNR_VIRT_CR_VMSR_U2P_0_SRVR 0x000000B0
/**
  vMSRs for power mgmt
**/
#define SNR_VIRT_CR_VMSR_U2P_1_SRVR 0x000000B1
/**
  vMSRs for power mgmt
**/
#define SNR_VIRT_CR_VMSR_U2P_2_SRVR 0x000000B2
/**
  vMSRs for power mgmt
**/
#define SNR_VIRT_CR_VMSR_U2P_3_SRVR 0x000000B3

/**
  Power Management - C-states configuration. Note the presence of the LOCK bit (15):When set, locks bits 15-0 of this register for further writes, until the next reset occurs.
**/
#define SNR_MSR_SCP_CR_CST_CONFIG_CONTROL 0x000000E2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RW
    UINT32 PackLim : 4;
    //Bits[7:4] Access Type=RW
    UINT32 MaxCoreCState : 4;
    //Bits[9:8] Access Type=RO
    UINT32 Rsvd8 : 2;
    //Bits[10:10] Access Type=RW
    UINT32 IoMwaitRedirection : 1;
    //Bits[14:11] Access Type=RO
    UINT32 Rsvd11 : 4;
    //Bits[15:15] Access Type=RW
    UINT32 CstCfgLock : 1;
    //Bits[24:16] Access Type=RO
    UINT32 Rsvd16 : 9;
    //Bits[25:25] Access Type=RW
    UINT32 C3StateAutoDemotionEnable : 1;
    //Bits[26:26] Access Type=RW
    UINT32 C1StateAutoDemotionEnable : 1;
    //Bits[27:27] Access Type=RW
    UINT32 Enc3undemotion : 1;
    //Bits[28:28] Access Type=RW
    UINT32 Enc1undemotion : 1;
    //Bits[29:29] Access Type=RW
    UINT32 Enpkgcautodemotion : 1;
    //Bits[30:30] Access Type=RW
    UINT32 Enpkgcundemotion : 1;
    //Bits[31:31] Access Type=RW
    UINT32 TimedMwaitEnable : 1;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_CST_CONFIG_CONTROL_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_SCP_CR_PMG_IO_CAPTURE_BASE 0x000000E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 Lvl2b : 16;
    //Bits[18:16] Access Type=RW
    UINT32 CstRange : 3;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;
    //Bits[47:32] Access Type=RW
    UINT32 Spare : 16;
    //Bits[63:48] Access Type=RW
    UINT32 FdpData : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_PMG_IO_CAPTURE_BASE_REGISTER;
/**
  fill description here
**/
#define SNR_MSR_PMG_CR_PST_MCNT 0x000000E7
/**
  fill description here
**/
#define SNR_MSR_PMG_CR_PST_ACNT 0x000000E8

/**
  Read-only register containing MTRR feature identification for the processor. Should be used in conjunction with the CPUID[EDX] bit 12 to determine MTRR capabilities. This MSR is read only; uCode generates #GP on any write.
**/
#define SNR_MSR_UCODE_CR_MTRRCAP 0x000000FE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RO
    UINT32 Vcnt : 8;
    //Bits[8:8] Access Type=Rsvd1
    UINT32 Fix : 1;
    //Bits[9:9] Access Type=Rsvd0
    UINT32 Reserved : 1;
    //Bits[10:10] Access Type=Rsvd1
    UINT32 Write : 1;
    //Bits[11:11] Access Type=Rsvd1
    UINT32 Smrr : 1;
    //Bits[12:12] Access Type=RO
    UINT32 Prmrr : 1;
    //Bits[13:13] Access Type=RO
    UINT32 Smrr2 : 1;
    //Bits[14:14] Access Type=Rsvd1
    UINT32 SmrrLock : 1;
    //Bits[15:15] Access Type=RO
    UINT32 Rsvd15 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_MTRRCAP_REGISTER;

/**
  Software override for each C-state in Power Aware Interrupt Routing feature.
**/
#define SNR_VIRTUAL_MSR_CR_PAIR_WEIGHT_OVERRIDE 0x00000102

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[2:0] Access Type=RW
    UINT32 Cc1Weight : 3;
    //Bits[5:3] Access Type=RW
    UINT32 C0xWeight : 3;
    //Bits[8:6] Access Type=RW
    UINT32 C00Weight : 3;
    //Bits[11:9] Access Type=RW
    UINT32 Cc3Weight : 3;
    //Bits[14:12] Access Type=RW
    UINT32 Cc6Weight : 3;
    //Bits[30:15] Access Type=RO
    UINT32 Rsvd15 : 16;
    //Bits[31:31] Access Type=RW
    UINT32 OverrideEnable : 1;
    //Bits[63:32] Access Type=RO
    UINT32 Rsvd32 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VIRTUAL_MSR_CR_PAIR_WEIGHT_OVERRIDE_REGISTER;

/**
  Pair package level thread participation. Per BXT i1012209455, PAIR MSRs will exist without storage or functionality on BXT/DNV. WRMSRs will be silently dropped, RDMSRs return 0s. This matches SKL behavior if PAIR is fuse disabled.
**/
#define SNR_VIRTUAL_MSR_CR_PAIR_PKG_THREAD_PARTICIPATION 0x00000103

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 C0t0 : 1;
    //Bits[1:1] Access Type=RW
    UINT32 C0t1 : 1;
    //Bits[2:2] Access Type=RW
    UINT32 C1t0 : 1;
    //Bits[3:3] Access Type=RW
    UINT32 C1t1 : 1;
    //Bits[4:4] Access Type=RW
    UINT32 C2t0 : 1;
    //Bits[5:5] Access Type=RW
    UINT32 C2t1 : 1;
    //Bits[6:6] Access Type=RW
    UINT32 C3t0 : 1;
    //Bits[7:7] Access Type=RW
    UINT32 C3t1 : 1;
    //Bits[31:8] Access Type=RO
    UINT32 Rsvd8 : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VIRTUAL_MSR_CR_PAIR_PKG_THREAD_PARTICIPATION_REGISTER;

/**
  Register to specify a thread's inclusion or exclusion in Power Aware Interrupt Routing algorithm at a package level. Per BXT i1012209455, PAIR MSRs will exist without storage or functionality on BXT/DNV. WRMSRs will be silently dropped, RDMSRs return 0s. This matches SKL behavior if PAIR is fuse disabled.
**/
#define SNR_VIRTUAL_MSR_CR_PAIR_THREAD_PARTICIPATION 0x00000104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 PairExcl : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VIRTUAL_MSR_CR_PAIR_THREAD_PARTICIPATION_REGISTER;

/**
  Duplicate MSR with DL1 flush functionality to match Core
**/
#define SNR_MSR_UCODE_CR_WBINVAL_DL1 0x0000010B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 WbinvalDl1 : 1;
    //Bits[15:1] Access Type=RO
    UINT32 Rsvd1 : 15;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_WBINVAL_DL1_REGISTER;

/**
  Misc power controls. TNT: Only bit left is WBINVAL_DL1. So write only MSR. Originally added for S0i2 on STW. used for eHALT enables on SLM i1214710.
**/
#define SNR_MSR_UCODE_CR_POWER_MISC 0x00000120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RO
    UINT32 Rsvd0 : 8;
    //Bits[8:8] Access Type=RW
    UINT32 WbinvalDl1 : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_POWER_MISC_REGISTER;

/**
  GLM i1958937: Emulate PM_TMR in ucode for BXT
**/
#define SNR_MSR_SCP_CR_EMULATE_PM_TMR 0x00000121

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 PmTmrBlkBase : 16;
    //Bits[16:16] Access Type=RW
    UINT32 PmTmrBlkValid : 1;
    //Bits[19:17] Access Type=RO
    UINT32 Rsvd17 : 3;
    //Bits[31:20] Access Type=RW
    UINT32 RspDelayLoopCount : 12;
    //Bits[63:32] Access Type=RW
    UINT32 Multiplier : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_EMULATE_PM_TMR_REGISTER;

/**
  
**/
#define SNR_MSR_CORE_INTERFACE_CR_MMCFG_RULE_CFG 0x00000129

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 RuleEnable : 1;
    //Bits[3:1] Access Type=RW
    UINT32 Length : 3;
    //Bits[5:4] Access Type=RO
    UINT32 Rsvd4 : 2;
    //Bits[13:6] Access Type=RW
    UINT32 ValidTargets : 8;
    //Bits[25:14] Access Type=RO
    UINT32 Rsvd14 : 12;
    //Bits[51:26] Access Type=RW
    UINT32 BaseAddress : 26;
    //Bits[63:52] Access Type=RO
    UINT32 Rsvd52 : 12;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_MMCFG_RULE_CFG_REGISTER;
/**
  Anchor Cove MSR to indicate that the initial boot block is complete, and to re-enable events. Write-only and accessible via the ACM.
**/
#define SNR_MSR_VIRT_CR_ANC_FORCE_BOOT_POLICY_ENFORCED 0x00000138

/**
  SLM e3430087:Write only MSR. On TNT, this is simply based on ANCHOR_COVE fuse (not SECURE_BOOT_CAPABILITY_ENABLE)
**/
#define SNR_MSR_VIRT_CR_ANC_INITIAL_BOOT_BLOCK_COMPLETE 0x00000139

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 BiosRdyForMiscEvents : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_ANC_INITIAL_BOOT_BLOCK_COMPLETE_REGISTER;

/**
  An MSR for communicating basic policy information from the Anchor Cove ACM to early BIOS. Things like executing in NEM, thread protection enabled etc., will be communicated.
**/
#define SNR_MSR_VIRT_CR_ANC_SACM_INFO 0x0000013A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 SacmData : 8;
    //Bits[31:8] Access Type=RO
    UINT32 Rsvd8 : 24;
    //Bits[32:32] Access Type=RO
    UINT32 AnchorCoveEnFuse : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Pbe : 1;
    //Bits[34:34] Access Type=RO
    UINT32 LtSxEnFuse : 1;
    //Bits[35:35] Access Type=RO
    UINT32 NoResetSecretsProt : 1;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_ANC_SACM_INFO_REGISTER;

/**
  Set WSM i387670. MSR only accessible if AES is fuse enabled.
**/
#define SNR_MSR_UCODE_CR_FEATURE_CONFIG 0x0000013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 AesniLock : 1;
    //Bits[1:1] Access Type=RW
    UINT32 AesniDisable : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_FEATURE_CONFIG_REGISTER;

/**
  Miscellaneous enables for thread specific features
**/
#define SNR_MSR_UCODE_CR_FEATURE_ENABLES 0x00000140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 CpuidGpOnCplGt0 : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_FEATURE_ENABLES_REGISTER;

/**
  GLP 1706546645: Black Point. SMM-only MSR which allows locking of various states while in SMM mode
**/
#define SNR_MSR_VIRT_CR_SMM_SUPOVR_STATE_LOCK 0x00000141

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 PagingStates : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Smbase : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Ia32SmmMonitorCtlMsr : 1;
    //Bits[7:3] Access Type=RO
    UINT32 Rsvd3 : 5;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_SMM_SUPOVR_STATE_LOCK_REGISTER;

/**
  BIOS Done MSR for CNL i1351547. This replaces MSR 0x120 POWER_MISC[ENABLE_IA_UNTRUSTED_MODE]
**/
#define SNR_MSR_UCODE_CR_BIOS_DONE 0x00000151

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 EnableIaUntrusted : 1;
    //Bits[1:1] Access Type=RO
    UINT32 SocBiosDone : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_BIOS_DONE_REGISTER;

/**
  SMM MSR that read-combines THREAD_SMI_ERR_SRC_SET_AND_STATUS for rdmsr and writes THREAD_SMI_ERR_SRCRESET for wrmsr
**/
#define SNR_MSR_VIRT_CR_THREAD_SMI_ERR_SRC 0x00000158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Data : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_THREAD_SMI_ERR_SRC_REGISTER;

/**
  Special uncore usage msr, work in PFAT or in SMM BIOS flash
**/
#define SNR_MSR_FUNNYIO_CR_SPCL_UNCORE_USAGE 0x0000015B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Data : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SPCL_UNCORE_USAGE_REGISTER;

/**
  Register which contains the current SMM base value for the processor. The RESET event handler should set it to 0x00030000, which is the default value. When RSM executes, it reads the SMM base slot value in SMM memory and loads it into this register. The INIT event handler should not change the value of this register.
**/
#define SNR_MSR_SCP_CR_SMBASE 0x00000171

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Base : 32;
    //Bits[63:32] Access Type=RW
    UINT32 PatchSignId : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_SMBASE_REGISTER;

/**
  No RSVD bit checks on upper bits. See NHM i354994. Register used by the ""Fast System Call"" facility (SYSENTER/SYSEXIT). Contains the OS copy of the Target ring 0 CS selector value.
**/
#define SNR_SCP_CR_SYSENTER_CS_MSR 0x00000174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Data : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_SCP_CR_SYSENTER_CS_MSR_REGISTER;
/**
  Register used by the ""Fast System Call"" facility (SYSENTER/SYSEXIT). Contains the OS copy of the Target ring 0 ESP value. This CREG is a normal read/write MSR/bit.
**/
#define SNR_SCP_CR_SYSENTER_ESP_MSR 0x00000175
/**
  "Register used by the ""Fast System Call"" facility (SYSENTER/SYSEXIT). Contains the OS copy of the Target ring 0 entry point EIP value." This CREG is a normal read/write MSR/bit.
**/
#define SNR_SCP_CR_SYSENTER_EIP_MSR 0x00000176

/**
  SMM MSR that read-combines [CORE|MODULE]_SMI_ERR_SRC_SET_AND_STATUS for rdmsr and writes [CORE|MODULE]_SMI_ERR_SRCRESET for wrmsr
**/
#define SNR_MSR_VIRT_CR_CORE_SMI_ERR_SRC 0x0000017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Mc0 : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Mc1 : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Mc2 : 1;
    //Bits[3:3] Access Type=RW
    UINT32 Mc3 : 1;
    //Bits[7:4] Access Type=RO
    UINT32 Rsvd4 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_CORE_SMI_ERR_SRC_REGISTER;

/**
  SMM MSR that reads UNCORE_SMI_ERR_SRC_STATUS for rdmsr and writes UNCORE_SMI_ERR_SRC_RESET for wrmsr
**/
#define SNR_MSR_VIRT_CR_UNCORE_SMI_ERR_SRC 0x0000017E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RO
    UINT32 Rsvd0 : 4;
    //Bits[31:4] Access Type=RW
    UINT32 Allbanks : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_UNCORE_SMI_ERR_SRC_REGISTER;

/**
  A non-architectural, package-scoped MSR intended for use by BIOS. The bit will be saved and restored in C7+ states. The reset value for the bit is 0. It will not survive warm reset.
**/
#define SNR_MSR_FUNNYIO_CR_SCP_ERROR_CONTROL 0x0000017F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RO
    UINT32 Rsvd0 : 4;
    //Bits[4:4] Access Type=RW
    UINT32 CmciDisable : 1;
    //Bits[6:5] Access Type=RO
    UINT32 Rsvd5 : 2;
    //Bits[7:7] Access Type=RW
    UINT32 LlcEwbDowngrade : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SCP_ERROR_CONTROL_REGISTER;

/**
  Performance Status MSR. On Core family, this msr register contains the FIVR DAC target and core pll ratio. There is not the same FIVR internal regulation present inside GLM like HSW/BDW. On GLM, this does reflect the Voltage target given to an external VR (SVID or PMIC) or Internal VR called DSLDO (digital synthesized LDO). For DNV, the internal DSLDO is not intended to be used as all cores share a common external Voltage rail coming from a PMIC or SVID controller.
**/
#define SNR_MSR_VIRT_CR_PERF_STATUS 0x00000198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RO
    UINT32 Rsvd0 : 8;
    //Bits[15:8] Access Type=RO
    UINT32 Pllratio : 8;
    //Bits[31:16] Access Type=RO
    UINT32 Rsvd16 : 16;
    //Bits[47:32] Access Type=RO
    UINT32 CoreVoltage : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_PERF_STATUS_REGISTER;

/**
  Performance Control MSR. SW makes request for a new Performance state (P-State) by writing this MSR.
**/
#define SNR_MSR_C6_SCP_CR_PERF_CTL 0x00000199

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 VReq : 8;
    //Bits[14:8] Access Type=RW
    UINT32 PReq : 7;
    //Bits[15:15] Access Type=Rsvd0
    UINT32 Reserved15 : 1;
    //Bits[21:16] Access Type=RW
    UINT32 Pqosofst : 6;
    //Bits[31:22] Access Type=RO
    UINT32 Rsvd22 : 10;
    //Bits[32:32] Access Type=RW
    UINT32 TurboDisable : 1;
    //Bits[63:33] Access Type=RO
    UINT32 Rsvd33 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_PERF_CTL_REGISTER;

/**
  [p]PUNIT_MSR: CPU Core thermal interrupt control register.  This register is used to configure core or package level notifications of important thermal events.  These interrupts may be routed directly to this local core's APIC Thermal LVT or optional modes may be configured that allow the thermal interrupt notification to be routed to a master core for processing (fore more details on locking thermal interrupts, see the MISC_PWR_MGMT MSR definition).[/p]
            [p][b]MSR_Name[/b]: THERM_INTERRUPT  [b]MSR_Addr[/b]: 0x19B[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_THERM_INTERRUPT 0x0000019B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 HighTempIntEnable : 1;
    //Bits[1:1] Access Type=RW
    UINT32 LowTempIntEnable : 1;
    //Bits[2:2] Access Type=RW
    UINT32 ProchotIntEnable : 1;
    //Bits[3:3] Access Type=RSV
    UINT32 Reserved0 : 1;
    //Bits[4:4] Access Type=RW
    UINT32 OutOfSpecIntEnable : 1;
    //Bits[7:5] Access Type=RSV
    UINT32 Reserved1 : 3;
    //Bits[14:8] Access Type=RW
    UINT32 Threshold1RelTemp : 7;
    //Bits[15:15] Access Type=RW
    UINT32 Threshold1IntEnable : 1;
    //Bits[22:16] Access Type=RW
    UINT32 Threshold2RelTemp : 7;
    //Bits[23:23] Access Type=RW
    UINT32 Threshold2IntEnable : 1;
    //Bits[24:24] Access Type=RW
    UINT32 SparePwrLimitEnable : 1;
    //Bits[31:25] Access Type=RSV
    UINT32 Reserved2 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_THERM_INTERRUPT_REGISTER;

/**
  [p]PUNIT_MSR: CPU Core thermal and power status register.  This register is typically used to inspect the thermal status of a specific core, as well as to discover the sources of core thermal interrupt events.[/p]
            [p][b]MSR_Name[/b]: THERM_STATUS  [b]MSR_Addr[/b]: 0x19C[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_THERM_STATUS 0x0000019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 ThermalMonitorStatus : 1;
    //Bits[1:1] Access Type=RW_0C
    UINT32 ThermalMonitorLog : 1;
    //Bits[2:2] Access Type=RO
    UINT32 ProchotStatus : 1;
    //Bits[3:3] Access Type=RW_0C
    UINT32 ProchotLog : 1;
    //Bits[4:4] Access Type=RO
    UINT32 OutOfSpecStatus : 1;
    //Bits[5:5] Access Type=RW_0C
    UINT32 OutOfSpecLog : 1;
    //Bits[6:6] Access Type=RO
    UINT32 Threshold1Status : 1;
    //Bits[7:7] Access Type=RW_0C
    UINT32 Threshold1Log : 1;
    //Bits[8:8] Access Type=RO
    UINT32 Threshold2Status : 1;
    //Bits[9:9] Access Type=RW_0C
    UINT32 Threshold2Log : 1;
    //Bits[10:10] Access Type=RO
    UINT32 PowerLimitationStatus : 1;
    //Bits[11:11] Access Type=RW_0C
    UINT32 PowerLimitationLog : 1;
    //Bits[12:12] Access Type=ROFW
    UINT32 CurrentLimitStatus : 1;
    //Bits[13:13] Access Type=RW0C_FW
    UINT32 CurrentLimitLog : 1;
    //Bits[14:14] Access Type=ROFW
    UINT32 CrossDomainLimitStatus : 1;
    //Bits[15:15] Access Type=RW0C_FW
    UINT32 CrossDomainLimitLog : 1;
    //Bits[22:16] Access Type=RO
    UINT32 Temperature : 7;
    //Bits[26:23] Access Type=RSV
    UINT32 Reserved1 : 4;
    //Bits[30:27] Access Type=RO
    UINT32 Resolution : 4;
    //Bits[31:31] Access Type=RO
    UINT32 Valid : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_THERM_STATUS_REGISTER;

/**
  return 0x0 on reads to MSR_THERM2_CTL MSR (called CLOCK_GEYSIII_THERM on Merom). Functionality associated with this MSR does not exist since NHM. Can't get rid of this MSR since this is an architectural MSR
**/
#define SNR_UCODE_CR_MSR_THERM2_CTL 0x0000019D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=Rsvd0
    UINT32 Data : 32;
    //Bits[63:32] Access Type=RO
    UINT32 Rsvd32 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_UCODE_CR_MSR_THERM2_CTL_REGISTER;

/**
  [p]This register contains information about the fan speed control target temperature as well as details on the reference temperature for IA core DTS relative temperature reading.[/p]
            [p][b]MSR_Name[/b]: TEMPERATURE_TARGET  [b]MSR_Addr[/b]: 0x1A2[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_TEMPERATURE_TARGET 0x000001A2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RO
    UINT32 TccOffsetTimeWindow : 7;
    //Bits[7:7] Access Type=RO
    UINT32 TccOffsetClamp : 1;
    //Bits[15:8] Access Type=RO
    UINT32 FanTempTargetOffset : 8;
    //Bits[23:16] Access Type=RO
    UINT32 RefTemp : 8;
    //Bits[29:24] Access Type=RW
    UINT32 TjMaxTccOffset : 6;
    //Bits[30:30] Access Type=RO
    UINT32 Rsvd30 : 1;
    //Bits[31:31] Access Type=RW_L
    UINT32 Locked : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_TEMPERATURE_TARGET_REGISTER;

/**
  Mixed scope MSR that holds prefetch disable bits. Bits in this MSR map to 2 different cregs. This was added on GLM for the cQOS issue. SLM i1214941: Added prefetcher disable MSR for Avoton B0. See NHM issue 386311 and ECO 466960 for details of when it was added to Core
**/
#define SNR_MSR_UCODE_CR_MISC_FEATURE_CONTROL 0x000001A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 MlcStreamerPrefetchDisable : 1;
    //Bits[1:1] Access Type=RW
    UINT32 MlcSpatialPrefetchDisable : 1;
    //Bits[2:2] Access Type=RW
    UINT32 DcuStreamerPrefetchDisable : 1;
    //Bits[3:3] Access Type=RW
    UINT32 DcuIpPrefetchDisable : 1;
    //Bits[4:4] Access Type=RW
    UINT32 DcuNextPagePrefetchDisable : 1;
    //Bits[10:5] Access Type=RO
    UINT32 Rsvd5 : 6;
    //Bits[11:11] Access Type=RW
    UINT32 DisableThreeStrikeCnt : 1;
    //Bits[15:12] Access Type=RO
    UINT32 Rsvd12 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_MISC_FEATURE_CONTROL_REGISTER;

/**
  Uncore PerfMon Register 0
**/
#define SNR_MSR_SLM_CR_OFFCORE_RESP0 0x000001A6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 TrueDemandCacheLoad : 1;
    //Bits[1:1] Access Type=RW
    UINT32 DemandRfo : 1;
    //Bits[2:2] Access Type=RW
    UINT32 DemandCodeRead : 1;
    //Bits[3:3] Access Type=RW
    UINT32 CoreModifiedWriteback : 1;
    //Bits[4:4] Access Type=RW
    UINT32 HwPrefetchMlcLoad : 1;
    //Bits[5:5] Access Type=RW
    UINT32 HwPrefetchMlcRfo : 1;
    //Bits[6:6] Access Type=RW
    UINT32 HwPrefetchMlcCode : 1;
    //Bits[7:7] Access Type=RW
    UINT32 HwPrefetchLlcLoad : 1;
    //Bits[8:8] Access Type=RW
    UINT32 HwPrefetchLlcRfo : 1;
    //Bits[9:9] Access Type=RW
    UINT32 HwPrefetchLlcCode : 1;
    //Bits[10:10] Access Type=RW
    UINT32 L1Prefetch : 1;
    //Bits[11:11] Access Type=RW
    UINT32 AllStreamingStore : 1;
    //Bits[12:12] Access Type=RW
    UINT32 CoreNonModifiedWb : 1;
    //Bits[14:13] Access Type=Rsvd0
    UINT32 Reserved : 2;
    //Bits[15:15] Access Type=RW
    UINT32 Other : 1;
    //Bits[16:16] Access Type=RW
    UINT32 AnyResp : 1;
    //Bits[17:17] Access Type=RW
    UINT32 SupplierNone : 1;
    //Bits[18:18] Access Type=RW
    UINT32 LlcHitMState : 1;
    //Bits[19:19] Access Type=RW
    UINT32 LlcHitEState : 1;
    //Bits[20:20] Access Type=RW
    UINT32 LlcHitSState : 1;
    //Bits[21:21] Access Type=RW
    UINT32 LlcHitFState : 1;
    //Bits[22:22] Access Type=RW
    UINT32 FarMemLocal : 1;
    //Bits[23:23] Access Type=RW
    UINT32 FarMemRemote0Hop : 1;
    //Bits[24:24] Access Type=RW
    UINT32 FarMemRemote1Hop : 1;
    //Bits[25:25] Access Type=RW
    UINT32 FarMemRemote2PlusHop : 1;
    //Bits[26:26] Access Type=RW
    UINT32 NearMemLocal : 1;
    //Bits[27:27] Access Type=RW
    UINT32 NearMemRemote0Hop : 1;
    //Bits[28:28] Access Type=RW
    UINT32 NearMemRemote1Hop : 1;
    //Bits[29:29] Access Type=RW
    UINT32 NearMemRemote2PlusHop : 1;
    //Bits[30:30] Access Type=RW
    UINT32 SplHit : 1;
    //Bits[31:31] Access Type=RW
    UINT32 SnoopNone : 1;
    //Bits[32:32] Access Type=RW
    UINT32 NotNeeded : 1;
    //Bits[33:33] Access Type=RW
    UINT32 Miss : 1;
    //Bits[34:34] Access Type=RW
    UINT32 HitNoFwd : 1;
    //Bits[35:35] Access Type=RW
    UINT32 HitEfWithFwd : 1;
    //Bits[36:36] Access Type=RW
    UINT32 Hitm : 1;
    //Bits[37:37] Access Type=RW
    UINT32 NonDram : 1;
    //Bits[38:38] Access Type=RW
    UINT32 GoErr : 1;
    //Bits[39:39] Access Type=RW
    UINT32 GoNoGo : 1;
    //Bits[43:40] Access Type=Rsvd0
    UINT32 Reserved4340 : 4;
    //Bits[44:44] Access Type=RW
    UINT32 UcLoad : 1;
    //Bits[45:45] Access Type=RW
    UINT32 UcStore : 1;
    //Bits[46:46] Access Type=RW
    UINT32 PartialStreamingStores : 1;
    //Bits[47:47] Access Type=RW
    UINT32 FullStreamingStores : 1;
    //Bits[48:48] Access Type=RW
    UINT32 L1ModifiedWb : 1;
    //Bits[49:49] Access Type=RW
    UINT32 L2ModifiedWb : 1;
    //Bits[50:50] Access Type=RW
    UINT32 Psmi : 1;
    //Bits[51:51] Access Type=RW
    UINT32 Itom : 1;
    //Bits[62:52] Access Type=Rsvd0
    UINT32 Reserved6252 : 11;
    //Bits[63:63] Access Type=RW
    UINT32 OutstandingReq : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SLM_CR_OFFCORE_RESP0_REGISTER;

/**
  Uncore PerfMon Register 1
**/
#define SNR_MSR_SLM_CR_OFFCORE_RESP1 0x000001A7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 TrueDemandCacheLoad : 1;
    //Bits[1:1] Access Type=RW
    UINT32 DemandRfo : 1;
    //Bits[2:2] Access Type=RW
    UINT32 DemandCodeRead : 1;
    //Bits[3:3] Access Type=RW
    UINT32 CoreModifiedWriteback : 1;
    //Bits[4:4] Access Type=RW
    UINT32 HwPrefetchMlcLoad : 1;
    //Bits[5:5] Access Type=RW
    UINT32 HwPrefetchMlcRfo : 1;
    //Bits[6:6] Access Type=RW
    UINT32 HwPrefetchMlcCode : 1;
    //Bits[7:7] Access Type=RW
    UINT32 HwPrefetchLlcLoad : 1;
    //Bits[8:8] Access Type=RW
    UINT32 HwPrefetchLlcRfo : 1;
    //Bits[9:9] Access Type=RW
    UINT32 HwPrefetchLlcCode : 1;
    //Bits[10:10] Access Type=RW
    UINT32 L1Prefetch : 1;
    //Bits[11:11] Access Type=RW
    UINT32 AllStreamingStore : 1;
    //Bits[12:12] Access Type=RW
    UINT32 CoreNonModifiedWb : 1;
    //Bits[14:13] Access Type=Rsvd0
    UINT32 Reserved : 2;
    //Bits[15:15] Access Type=RW
    UINT32 Other : 1;
    //Bits[16:16] Access Type=RW
    UINT32 AnyResp : 1;
    //Bits[17:17] Access Type=RW
    UINT32 SupplierNone : 1;
    //Bits[18:18] Access Type=RW
    UINT32 LlcHitMState : 1;
    //Bits[19:19] Access Type=RW
    UINT32 LlcHitEState : 1;
    //Bits[20:20] Access Type=RW
    UINT32 LlcHitSState : 1;
    //Bits[21:21] Access Type=RW
    UINT32 LlcHitFState : 1;
    //Bits[22:22] Access Type=RW
    UINT32 FarMemLocal : 1;
    //Bits[23:23] Access Type=RW
    UINT32 FarMemRemote0Hop : 1;
    //Bits[24:24] Access Type=RW
    UINT32 FarMemRemote1Hop : 1;
    //Bits[25:25] Access Type=RW
    UINT32 FarMemRemote2PlusHop : 1;
    //Bits[26:26] Access Type=RW
    UINT32 NearMemLocal : 1;
    //Bits[27:27] Access Type=RW
    UINT32 NearMemRemote0Hop : 1;
    //Bits[28:28] Access Type=RW
    UINT32 NearMemRemote1Hop : 1;
    //Bits[29:29] Access Type=RW
    UINT32 NearMemRemote2PlusHop : 1;
    //Bits[30:30] Access Type=RW
    UINT32 SplHit : 1;
    //Bits[31:31] Access Type=RW
    UINT32 SnoopNone : 1;
    //Bits[32:32] Access Type=RW
    UINT32 NotNeeded : 1;
    //Bits[33:33] Access Type=RW
    UINT32 Miss : 1;
    //Bits[34:34] Access Type=RW
    UINT32 HitNoFwd : 1;
    //Bits[35:35] Access Type=RW
    UINT32 HitEfWithFwd : 1;
    //Bits[36:36] Access Type=RW
    UINT32 Hitm : 1;
    //Bits[37:37] Access Type=RW
    UINT32 NonDram : 1;
    //Bits[38:38] Access Type=RW
    UINT32 GoErr : 1;
    //Bits[39:39] Access Type=RW
    UINT32 GoNoGo : 1;
    //Bits[43:40] Access Type=Rsvd0
    UINT32 Reserved4340 : 4;
    //Bits[44:44] Access Type=RW
    UINT32 UcLoad : 1;
    //Bits[45:45] Access Type=RW
    UINT32 UcStore : 1;
    //Bits[46:46] Access Type=RW
    UINT32 PartialStreamingStores : 1;
    //Bits[47:47] Access Type=RW
    UINT32 FullStreamingStores : 1;
    //Bits[48:48] Access Type=RW
    UINT32 L1ModifiedWb : 1;
    //Bits[49:49] Access Type=RW
    UINT32 L2ModifiedWb : 1;
    //Bits[50:50] Access Type=RW
    UINT32 Psmi : 1;
    //Bits[51:51] Access Type=RW
    UINT32 Itom : 1;
    //Bits[63:52] Access Type=Rsvd0
    UINT32 Reserved6352 : 12;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SLM_CR_OFFCORE_RESP1_REGISTER;

/**
  MSR for thread specific defeature. No functionality on GLM. Only added for compatiblity. Returns 0s on reads, drops writes.
**/
#define SNR_MSR_VIRT_CR_THREAD_SW_DEFEATURE 0x000001A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 XinitDisable : 1;
    //Bits[1:1] Access Type=RW
    UINT32 XmodifiedDisable : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_THREAD_SW_DEFEATURE_REGISTER;

/**
  Legacy Miscellaneous Power Management Features.
**/
#define SNR_MSR_VIRT_CR_MISC_PWR_MGMT 0x000001AA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 SinglePctlEn : 1;
    //Bits[5:1] Access Type=RO
    UINT32 Rsvd1 : 5;
    //Bits[6:6] Access Type=RW
    UINT32 EnableHwp : 1;
    //Bits[7:7] Access Type=RW
    UINT32 EnableHwpInterrupt : 1;
    //Bits[8:8] Access Type=RW
    UINT32 EnableOutOfBandAutonomous : 1;
    //Bits[11:9] Access Type=RO
    UINT32 Rsvd9 : 3;
    //Bits[12:12] Access Type=RW
    UINT32 EnableHwpEpp : 1;
    //Bits[13:13] Access Type=RW
    UINT32 Lock : 1;
    //Bits[17:14] Access Type=RO
    UINT32 Rsvd14 : 4;
    //Bits[18:18] Access Type=RW
    UINT32 PlatPstateControl : 1;
    //Bits[21:19] Access Type=RO
    UINT32 Rsvd19 : 3;
    //Bits[22:22] Access Type=RW
    UINT32 LockThermInt : 1;
    //Bits[31:23] Access Type=RO
    UINT32 Rsvd23 : 9;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_MISC_PWR_MGMT_REGISTER;

/**
  [p]PUNIT_MSR: In conjunction with FREQ_LIMIT_CORES, this MSR allows an end-user to limit the number of turbo bins
               for different numbers of active cores (cores not in C6).[/p]
            [p]Each field in FREQ_LIMIT_RATIOS specifies a frequency limit for the specified number of active cores 
               in the corresponding field in FREQ_LIMIT_CORES.[/p]
            [p]Example: RATIO_LIMIT_0 is 0x20, CORE_COUNT_0 is 0x3; RATIO_LIMIT_1 is 0x16 and CORE_COUNT_1 is 0x5;
                        RATIO_LIMIT_3 is 0x10, CORE_COUNT_3 is 0xFF;
                        The frequency limit of <=3 active cores is 32, the limit for 4-5 active cores is 22, 
                        and the limit for 6 or more active cores is 16[/p]
            [p]Ratio limits must monotonically decrease in ascending bin order, core count must monotonically increase in ascending bin order,
               and the last valid FREQ_LIMIT_CORE field must be >= number of potentially active cores (just set to 0xFF)[/p]
            [p]Values of '0' are ignored.[/p]
            [p][b]MSR_Name[/b]: FREQ_LIMIT_RATIOS  [b]MSR_Addr[/b]: 0x1AD[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_FREQ_LIMIT_RATIOS 0x000001AD

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 RatioLimit0 : 8;
    //Bits[15:8] Access Type=RW
    UINT32 RatioLimit1 : 8;
    //Bits[23:16] Access Type=RW
    UINT32 RatioLimit2 : 8;
    //Bits[31:24] Access Type=RW
    UINT32 RatioLimit3 : 8;
    //Bits[39:32] Access Type=RW
    UINT32 RatioLimit4 : 8;
    //Bits[47:40] Access Type=RW
    UINT32 RatioLimit5 : 8;
    //Bits[55:48] Access Type=RW
    UINT32 RatioLimit6 : 8;
    //Bits[63:56] Access Type=RW
    UINT32 RatioLimit7 : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_FREQ_LIMIT_RATIOS_REGISTER;

/**
  Not a real register. This define exists to create the MSR routine that will be used to control the energy-efficient P-State performance policy. The bits will be redirected to a corresponding field in THREAD_P_REQ. This MSR will #GP unless POWER_CTL[PWR_PERF_PLTFRM_OVR] is set.
**/
#define SNR_MSR_UCODE_CR_ENERGY_PERFORMANCE_BIAS 0x000001B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RW
    UINT32 EnergyPolicy : 4;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_ENERGY_PERFORMANCE_BIAS_REGISTER;

/**
  [p]PUNIT_MSR: Package level thermal and power status register.  This register is typically used to inspect the thermal status of the entire package, as well as to discover the sources of package level thermal interrupt events.[/p]
            [p][b]MSR_Name[/b]: THERM_STATUS  [b]MSR_Addr[/b]: 0x19C[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PACKAGE_THERM_STATUS 0x000001B1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 ThermalMonitorStatus : 1;
    //Bits[1:1] Access Type=RW_0C
    UINT32 ThermalMonitorLog : 1;
    //Bits[2:2] Access Type=RO
    UINT32 ProchotStatus : 1;
    //Bits[3:3] Access Type=RW_0C
    UINT32 ProchotLog : 1;
    //Bits[4:4] Access Type=RO
    UINT32 OutOfSpecStatus : 1;
    //Bits[5:5] Access Type=RW_0C
    UINT32 OutOfSpecLog : 1;
    //Bits[6:6] Access Type=RO
    UINT32 Threshold1Status : 1;
    //Bits[7:7] Access Type=RW_0C
    UINT32 Threshold1Log : 1;
    //Bits[8:8] Access Type=RO
    UINT32 Threshold2Status : 1;
    //Bits[9:9] Access Type=RW_0C
    UINT32 Threshold2Log : 1;
    //Bits[10:10] Access Type=RO
    UINT32 PowerLimitationStatus : 1;
    //Bits[11:11] Access Type=RW_0C
    UINT32 PowerLimitationLog : 1;
    //Bits[12:12] Access Type=RO
    UINT32 PmaxStatus : 1;
    //Bits[13:13] Access Type=RW_0C_FW
    UINT32 PmaxLog : 1;
    //Bits[15:14] Access Type=RO
    UINT32 Rsvd14 : 2;
    //Bits[22:16] Access Type=RO
    UINT32 Temperature : 7;
    //Bits[25:23] Access Type=RSV
    UINT32 Reserved1 : 3;
    //Bits[26:26] Access Type=RW0C_FW
    UINT32 HwFeedbackNotificationLog : 1;
    //Bits[30:27] Access Type=RO
    UINT32 Resolution : 4;
    //Bits[31:31] Access Type=RW
    UINT32 Valid : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PACKAGE_THERM_STATUS_REGISTER;

/**
  [p]PUNIT_MSR: Package thermal interrupt control register.  This register is used to configure package level notifications of important thermal events.  
               These interrupts are routed directly to core APIC Thermal LVT's and are handled locally by the core that has unmasked the LVT.[/p]
            [p][b]MSR_Name[/b]: THERM_INTERRUPT  [b]MSR_Addr[/b]: 0x19B[/p]
   [IntelRsvd]THIS REGISTER IS DUPLICATED IN THE PCU IO SPACE, XML CHANGES MUST BE MADE IN BOTH PLACES[/IntelRsvd]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PACKAGE_THERM_INTERRUPT 0x000001B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 HighTempIntEnable : 1;
    //Bits[1:1] Access Type=RW
    UINT32 LowTempIntEnable : 1;
    //Bits[2:2] Access Type=RW
    UINT32 ProchotIntEnable : 1;
    //Bits[3:3] Access Type=RW
    UINT32 PmaxIntEnable : 1;
    //Bits[4:4] Access Type=RW
    UINT32 OutOfSpecIntEnable : 1;
    //Bits[7:5] Access Type=RSV
    UINT32 Reserved1 : 3;
    //Bits[14:8] Access Type=RW
    UINT32 Threshold1RelTemp : 7;
    //Bits[15:15] Access Type=RW
    UINT32 Threshold1IntEnable : 1;
    //Bits[22:16] Access Type=RW
    UINT32 Threshold2RelTemp : 7;
    //Bits[23:23] Access Type=RW
    UINT32 Threshold2IntEnable : 1;
    //Bits[24:24] Access Type=RW
    UINT32 PowerIntEnable : 1;
    //Bits[25:25] Access Type=RW
    UINT32 HwFeedbackNotificationEnable : 1;
    //Bits[31:26] Access Type=RSV
    UINT32 Reserved2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PACKAGE_THERM_INTERRUPT_REGISTER;

/**
  "Contains a 5-bit pointer (0 ... 31) to the MSR in the LBR stack that contains the most recent branch, interrupt, or exception recorded.  Prior to placing a new branch record on the stack, the TOS is incremented by 1. When the TOS pointer reaches 31, it wraps around to 0."
**/
#define SNR_MSR_ARR_CR_LBR_TOS 0x000001C9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[4:0] Access Type=RW
    UINT32 TosPointer : 5;
    //Bits[31:5] Access Type=RO
    UINT32 Rsvd5 : 27;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_TOS_REGISTER;

/**
  Register used to enable various documented and undocumented debug features. See PPro Developer-s Manual, Vol. 3 for more information on the public features. .
**/
#define SNR_BNL_CR_DEBUGCTLMSR 0x000001D9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Lbr : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Btf : 1;
    //Bits[5:2] Access Type=Rsvd0
    UINT32 ReservedUsedToBePb03 : 4;
    //Bits[6:6] Access Type=RW
    UINT32 Tr : 1;
    //Bits[7:7] Access Type=RW
    UINT32 Bts : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Btint : 1;
    //Bits[9:9] Access Type=RW
    UINT32 BtsOffOs : 1;
    //Bits[10:10] Access Type=RW
    UINT32 BtsOffUsr : 1;
    //Bits[11:11] Access Type=RW
    UINT32 FreezeLbrsOnPmi : 1;
    //Bits[12:12] Access Type=RW
    UINT32 FreezePerfmonOnPmi : 1;
    //Bits[13:13] Access Type=RW
    UINT32 EnablePmiFromUncore : 1;
    //Bits[14:14] Access Type=RW
    UINT32 FreezeWhileSmmEn : 1;
    //Bits[31:15] Access Type=Rsvd0
    UINT32 Reserved : 17;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_BNL_CR_DEBUGCTLMSR_REGISTER;

/**
  Register which contains the source address of the last branch taken prior to an exception or interrupt being generated. The address saved is the offset in the code segment of the branch source instruction.
**/
#define SNR_MSR_SCP_CR_LER_FROM_LIP 0x000001DD

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[62:48] Access Type=RW
    UINT32 LipExt : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_LER_FROM_LIP_REGISTER;

/**
  Register which contains the destination address of the last branch taken prior to an exception or interrupt being generated. The address saved is the offset in the code segment of the branch source instruction.
**/
#define SNR_MSR_SCP_CR_LER_TO_LIP 0x000001DE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Time : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_LER_TO_LIP_REGISTER;

/**
  Register which contains control and status bits related to probe mode. This register is only accessible when the Processor is in probe mode. micro-code uses this register to communicate with the probe mode software.
**/
#define SNR_MSR_BNL_CR_ICECTLPMR 0x000001DF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Ir : 1;
    //Bits[1:1] Access Type=Rsvd0
    UINT32 Reserved1 : 1;
    //Bits[2:2] Access Type=RO
    UINT32 Pmen : 1;
    //Bits[3:3] Access Type=RW
    UINT32 Pir : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Pmis : 1;
    //Bits[5:5] Access Type=RW
    UINT32 Mce : 1;
    //Bits[6:6] Access Type=RW
    UINT32 PmRedirect : 1;
    //Bits[7:7] Access Type=Rsvd0
    UINT32 ReservedHess : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Init : 1;
    //Bits[9:9] Access Type=RW
    UINT32 Mcen : 1;
    //Bits[10:10] Access Type=RW
    UINT32 Smen : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Smex : 1;
    //Bits[12:12] Access Type=Rsvd0
    UINT32 Reserved2 : 1;
    //Bits[13:13] Access Type=RW
    UINT32 PreserveRtitResources : 1;
    //Bits[14:14] Access Type=RW
    UINT32 PreserveDebugResources : 1;
    //Bits[15:15] Access Type=RW
    UINT32 PreserveLbrResources : 1;
    //Bits[16:16] Access Type=Rsvd0
    UINT32 ReservedUsedToBeRemainProbemodeInBnl : 1;
    //Bits[20:17] Access Type=Rsvd0
    UINT32 Rsvd2017 : 4;
    //Bits[21:21] Access Type=RW
    UINT32 EnclaveInterruption : 1;
    //Bits[31:22] Access Type=Rsvd0
    UINT32 Rsvd3122 : 10;
    //Bits[32:32] Access Type=RW
    UINT32 Cc1EntryRedirectionControl : 1;
    //Bits[33:33] Access Type=RW
    UINT32 Cc2EntryRedirectionControl : 1;
    //Bits[34:34] Access Type=RW
    UINT32 Cc3EntryRedirectionControl : 1;
    //Bits[35:35] Access Type=RW
    UINT32 Cc4EntryRedirectionControl : 1;
    //Bits[36:36] Access Type=RW
    UINT32 Cc5EntryRedirectionControl : 1;
    //Bits[37:37] Access Type=RW
    UINT32 Cc6EntryRedirectionControl : 1;
    //Bits[39:38] Access Type=Rsvd0
    UINT32 Reserved3938 : 2;
    //Bits[40:40] Access Type=RW
    UINT32 Spare : 1;
    //Bits[41:41] Access Type=RW
    UINT32 Getsec : 1;
    //Bits[42:42] Access Type=RW
    UINT32 PreCram : 1;
    //Bits[43:43] Access Type=RW
    UINT32 AcmodeEnter : 1;
    //Bits[44:44] Access Type=RW
    UINT32 AcmodeExit : 1;
    //Bits[45:45] Access Type=RW
    UINT32 SexitEnd : 1;
    //Bits[46:46] Access Type=RW
    UINT32 EncluRedirectionControl : 1;
    //Bits[53:47] Access Type=Rsvd0
    UINT32 Reserved5347 : 7;
    //Bits[54:54] Access Type=RW
    UINT32 Int1PmRedirectionControl : 1;
    //Bits[55:55] Access Type=RW
    UINT32 StepIntoExcepInterRedirectionControl : 1;
    //Bits[56:56] Access Type=RW
    UINT32 ShutdownRedirectionControl : 1;
    //Bits[57:57] Access Type=RW
    UINT32 VmclearRedirectionControl : 1;
    //Bits[58:58] Access Type=RW
    UINT32 VmexitRedirectionControl : 1;
    //Bits[60:59] Access Type=Rsvd0
    UINT32 Rsvd6059 : 2;
    //Bits[61:61] Access Type=RW
    UINT32 StmServiceCallRedirectionControl : 1;
    //Bits[62:62] Access Type=RW
    UINT32 OsvVerticalVmentryRedirectionControl : 1;
    //Bits[63:63] Access Type=RW
    UINT32 VmlaunchRedirectionControl : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BNL_CR_ICECTLPMR_REGISTER;

/**
  GLM i1958673:Microserver MCA Enhancements. This register contains controls to enable/disable both the MB bank write capability and the MCA/CMCI signaling capability. Note that the scope and position of the bits is changed from GLP on TNT to match Core.
**/
#define SNR_MSR_FUNNYIO_CR_DEBUG_ERR_INJ_CTL 0x000001E3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 McbwE : 1;
    //Bits[1:1] Access Type=RW
    UINT32 McaCmciSe : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_DEBUG_ERR_INJ_CTL_REGISTER;

/**
  Will be changed to full module scope for SLT CMP. This was mixed scope on Penryn
**/
#define SNR_MSR_PIC_CR_DEBUG_MODES 0x000001E6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 EnableC2uLlb : 1;
    //Bits[1:1] Access Type=RW
    UINT32 U2cLlbAsSmcNuke : 1;
    //Bits[2:2] Access Type=RW
    UINT32 ThrottleU2cReq : 1;
    //Bits[3:3] Access Type=RW
    UINT32 PicReqOff : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Spare4 : 1;
    //Bits[5:5] Access Type=RW
    UINT32 IcrMsgOff : 1;
    //Bits[6:6] Access Type=RW
    UINT32 EoiSuppressCb : 1;
    //Bits[7:7] Access Type=RW
    UINT32 EoiNoBdcstEnable : 1;
    //Bits[8:8] Access Type=RW
    UINT32 DisableIntpriupdGeneration : 1;
    //Bits[9:9] Access Type=RW
    UINT32 SvGlobalEnable : 1;
    //Bits[10:10] Access Type=RW
    UINT32 SvSenterEn : 1;
    //Bits[11:11] Access Type=RW
    UINT32 TscDeadlineCb : 1;
    //Bits[12:12] Access Type=RW
    UINT32 IdIsNotReadonly : 1;
    //Bits[13:13] Access Type=RW
    UINT32 Available13 : 1;
    //Bits[14:14] Access Type=RW
    UINT32 L2CmciInternal : 1;
    //Bits[15:15] Access Type=RW
    UINT32 CmciFeatureDisable : 1;
    //Bits[16:16] Access Type=RW
    UINT32 PeriodicSmcEn : 1;
    //Bits[18:17] Access Type=RW
    UINT32 PeriodicSmcTscSel : 2;
    //Bits[19:19] Access Type=RW
    UINT32 WatchdogSmcOtherNow : 1;
    //Bits[20:20] Access Type=RW
    UINT32 WatchdogSmcOtherWait : 1;
    //Bits[21:21] Access Type=RW
    UINT32 WatchdogSmcSameNow : 1;
    //Bits[22:22] Access Type=RW
    UINT32 WatchdogSmcSameWait : 1;
    //Bits[23:23] Access Type=RW
    UINT32 WatchdogSmcOtherRepeated : 1;
    //Bits[24:24] Access Type=RW
    UINT32 PsmiAssist : 1;
    //Bits[25:25] Access Type=RW
    UINT32 DisableAsyncMcerr : 1;
    //Bits[26:26] Access Type=RW
    UINT32 DisableAsyncMckind : 1;
    //Bits[27:27] Access Type=RW
    UINT32 WatchdogQuick : 1;
    //Bits[28:28] Access Type=RW
    UINT32 WatchdogIgnoreBusqempty : 1;
    //Bits[29:29] Access Type=RW
    UINT32 Spare29 : 1;
    //Bits[30:30] Access Type=RW
    UINT32 DisableCtcDlSync : 1;
    //Bits[31:31] Access Type=RW
    UINT32 SelfIpiOff : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_DEBUG_MODES_REGISTER;

/**
  Read only VLW capability MSR that reports information about the VLW messages that are supported. Added on GLM to match Core. NHM added this as part of the CSI spec
**/
#define SNR_MSR_UCODE_CR_VLW_CAPABILITY 0x000001F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=Rsvd0
    UINT32 Ignne : 1;
    //Bits[1:1] Access Type=Rsvd0
    UINT32 A20m : 1;
    //Bits[2:2] Access Type=Rsvd1
    UINT32 Intr : 1;
    //Bits[3:3] Access Type=RO
    UINT32 Rsvd3 : 1;
    //Bits[4:4] Access Type=Rsvd1
    UINT32 Smi : 1;
    //Bits[5:5] Access Type=Rsvd1
    UINT32 Init : 1;
    //Bits[6:6] Access Type=Rsvd1
    UINT32 Nmi : 1;
    //Bits[7:7] Access Type=RO
    UINT32 Rsvd7 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_VLW_CAPABILITY_REGISTER;

/**
  Virtual MSR to will write data to an Crash Log configuration
**/
#define SNR_MSR_UCODE_CR_CRASHLOG_CONTROL 0x000001F1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Cddis : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_CRASHLOG_CONTROL_REGISTER;

/**
  PRotected Memory Range Register - Physical Mask
**/
#define SNR_MSR_PMH_CR_PRMRR_MASK 0x000001F5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=RO
    UINT32 Rsvd0 : 10;
    //Bits[10:10] Access Type=RW
    UINT32 L : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Vld : 1;
    //Bits[19:12] Access Type=Rsvd0
    UINT32 Reserved1912 : 8;
    //Bits[41:20] Access Type=RW
    UINT32 Mask : 22;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_PRMRR_MASK_REGISTER;

/**
  Valid PRMRR configurations
**/
#define SNR_MSR_UCODE_CR_PRMRR_VALID_CONFIG 0x000001FB

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Config1m : 1;
    //Bits[1:1] Access Type=RO
    UINT32 Config2m : 1;
    //Bits[2:2] Access Type=RO
    UINT32 Config4m : 1;
    //Bits[3:3] Access Type=RO
    UINT32 Config8m : 1;
    //Bits[4:4] Access Type=RO
    UINT32 Config16m : 1;
    //Bits[5:5] Access Type=RO
    UINT32 Config32m : 1;
    //Bits[6:6] Access Type=RO
    UINT32 Config64m : 1;
    //Bits[7:7] Access Type=RO
    UINT32 Config128m : 1;
    //Bits[8:8] Access Type=RO
    UINT32 Config256m : 1;
    //Bits[9:9] Access Type=RO
    UINT32 Config512m : 1;
    //Bits[10:10] Access Type=RO
    UINT32 Config1gb : 1;
    //Bits[11:11] Access Type=RO
    UINT32 Config2gb : 1;
    //Bits[12:12] Access Type=RO
    UINT32 Config4gb : 1;
    //Bits[13:13] Access Type=RO
    UINT32 Config8gb : 1;
    //Bits[14:14] Access Type=RO
    UINT32 Config16gb : 1;
    //Bits[15:15] Access Type=RO
    UINT32 Config32gb : 1;
    //Bits[16:16] Access Type=RO
    UINT32 Config64gb : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Config128gb : 1;
    //Bits[18:18] Access Type=RO
    UINT32 Config256gb : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Config512gb : 1;
    //Bits[20:20] Access Type=RO
    UINT32 Config1tb : 1;
    //Bits[31:21] Access Type=RO
    UINT32 Rsvd21 : 11;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_PRMRR_VALID_CONFIG_REGISTER;

/**
  Special Chipset Usage MSR
**/
#define SNR_MSR_FUNNYIO_CR_SPCL_CHIPSET_USAGE 0x000001FE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 En : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SPCL_CHIPSET_USAGE_REGISTER;

/**
  Bank0 MCA Control Register2.
**/
#define SNR_MSR_BUS_CR_MCI_CTL2 0x00000280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BUS_CR_MCI_CTL2_REGISTER;

/**
  L2 MCA Control Register2.
**/
#define SNR_MSR_L2_CR_MCI_CTL2 0x00000281

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_L2_CR_MCI_CTL2_REGISTER;

/**
  MCA Addres Control Register for CMCI. The creg is restored during C6 restore.
**/
#define SNR_MSR_IC_CR_MCI_CTL2 0x00000282

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_IC_CR_MCI_CTL2_REGISTER;

/**
  MCA Control Register 2
**/
#define SNR_MSR_AGU_CR_MCI_CTL2 0x00000283

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=Rsvd0
    UINT32 Ignored : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_AGU_CR_MCI_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC04_CTL2 0x00000284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC04_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC05_CTL2 0x00000285

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC05_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC06_CTL2 0x00000286

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC06_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC07_CTL2 0x00000287

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC07_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC08_CTL2 0x00000288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC08_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC09_CTL2 0x00000289

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC09_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC10_CTL2 0x0000028A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC10_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC11_CTL2 0x0000028B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC11_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC12_CTL2 0x0000028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC12_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC14_CTL2 0x0000028E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC14_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC15_CTL2 0x0000028F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC15_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC16_CTL2 0x00000290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC16_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC17_CTL2 0x00000291

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC17_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC18_CTL2 0x00000292

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC18_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC19_CTL2 0x00000293

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC19_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC20_CTL2 0x00000294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC20_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC21_CTL2 0x00000295

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC21_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC22_CTL2 0x00000296

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC22_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC23_CTL2 0x00000297

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC23_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC24_CTL2 0x00000298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC24_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC25_CTL2 0x00000299

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC25_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC26_CTL2 0x0000029A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC26_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC27_CTL2 0x0000029B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC27_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC28_CTL2 0x0000029C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC28_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC29_CTL2 0x0000029D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC29_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC30_CTL2 0x0000029E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC30_CTL2_REGISTER;

/**
  Virtual Creg describing the architectural format of the MCi_CTL2 register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC31_CTL2 0x0000029F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 CorrectedErrorCountThreshold : 15;
    //Bits[29:15] Access Type=RO
    UINT32 Rsvd15 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 CmciEn : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Rsvd31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 CmciCtl : 1;
    //Bits[33:33] Access Type=RO
    UINT32 Rsvd33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 MceCtl : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNCORE_CR_MC31_CTL2_REGISTER;

/**
  rPRotected Memory Range Register - Physical Base
**/
#define SNR_MSR_PMH_CR_PRMRR_BASE 0x000002A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[2:0] Access Type=RW
    UINT32 Memtype : 3;
    //Bits[3:3] Access Type=RW
    UINT32 Configured : 1;
    //Bits[11:4] Access Type=RO
    UINT32 Rsvd4 : 8;
    //Bits[19:12] Access Type=Rsvd0
    UINT32 Reserved1912 : 8;
    //Bits[41:20] Access Type=RW
    UINT32 Base : 22;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_PRMRR_BASE_REGISTER;

/**
  Just for Macro-address
**/
#define SNR_MSR_BNL_CR_BIOS_MISC 0x000002E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 NemSetup : 1;
    //Bits[1:1] Access Type=RW
    UINT32 NemRun : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BNL_CR_BIOS_MISC_REGISTER;

/**
  ACM Only MSR for EDRAM management
**/
#define SNR_MSR_UCODE_CR_EDRAM_ACM 0x000002E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 ForceEdramDisabled : 1;
    //Bits[1:1] Access Type=RO
    UINT32 EdramIsDisabled : 1;
    //Bits[2:2] Access Type=RW
    UINT32 EdramExists : 1;
    //Bits[31:3] Access Type=RO
    UINT32 Rsvd3 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_EDRAM_ACM_REGISTER;
/**
  uCode scratchpad register shared by all threads.
**/
#define SNR_MSR_FUNNYIO_CR_SCP_SE_OWNER_EPOCH_0 0x00000300
/**
  uCode scratchpad register shared by all threads.
**/
#define SNR_MSR_FUNNYIO_CR_SCP_SE_OWNER_EPOCH_1 0x00000301

/**
  uCode scratchpad register shared by all threads.
**/
#define SNR_MSR_FUNNYIO_CR_BIOS_SE_SVN 0x00000302

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 PrResetSvn : 8;
    //Bits[15:8] Access Type=RO
    UINT32 Rsvd8 : 8;
    //Bits[23:16] Access Type=RW
    UINT32 GetsecSenterAcmSvn : 8;
    //Bits[31:24] Access Type=RW
    UINT32 GetsecScleanAcmSvn : 8;
    //Bits[39:32] Access Type=RW
    UINT32 AncAcmSvn : 8;
    //Bits[63:40] Access Type=RO
    UINT32 Rsvd40 : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_BIOS_SE_SVN_REGISTER;
/**
  Used as a mailbox between BIOS and MCHECK.
**/
#define SNR_MSR_FUNNYIO_CR_SCP_MCHECK_BOOT_SCENARIO 0x00000304

/**
  Holds the status information and control the operation of the PMON Fixed Counter.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCtrClr] is set to 1b.
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONCTLFIX_CLIENT 0x00000394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[19:0] Access Type=RO
    UINT32 Rsvd0 : 20;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfen : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[31:23] Access Type=RO
    UINT32 Rsvd23 : 9;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONCTLFIX_CLIENT_REGISTER;

/**
  PMON Fixed Counter data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONCTRFX_CLIENT 0x00000395

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONCTRFX_CLIENT_REGISTER;

/**
  This is MSR that shows to user configuration of uncore PMON's. In this particular case it shows the number of Cbo PMON banks
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONCONFIG_CLIENT 0x00000396

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RW
    UINT32 Numofcbopmon : 6;
    //Bits[31:6] Access Type=RO
    UINT32 Rsvd6 : 26;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONCONFIG_CLIENT_REGISTER;

/**
  PMON Counter 0  data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFCTR0_CLIENT 0x000003B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNC_ARB_0_CR_PERFCTR0_CLIENT_REGISTER;

/**
  PMON Counter 0  data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFCTR1_CLIENT 0x000003B1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNC_ARB_0_CR_PERFCTR1_CLIENT_REGISTER;
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFEVTSEL0_CLIENT 0x000003B2
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFEVTSEL1_CLIENT 0x000003B3
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERF_STATUS_CLIENT 0x000003B4
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERF_CTRL_CLIENT 0x000003B5

/**
  PMON Counter 0  data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_UNC_ARB_1_CR_PERFCTR0_CLIENT 0x000003B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNC_ARB_1_CR_PERFCTR0_CLIENT_REGISTER;

/**
  PMON Counter 0  data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_UNC_ARB_1_CR_PERFCTR1_CLIENT 0x000003B9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNC_ARB_1_CR_PERFCTR1_CLIENT_REGISTER;
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_1_CR_PERFEVTSEL0_CLIENT 0x000003BA
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_1_CR_PERFEVTSEL1_CLIENT 0x000003BB
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_1_CR_PERF_STATUS_CLIENT 0x000003BC
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_1_CR_PERF_CTRL_CLIENT 0x000003BD

/**
  [p]PUNIT_MSR: Currently unused residency counter.  When accessed at the core level via RDMSR, time is measured in the same units as the Maximum Non-Turbo clock frequency.  When accessed at the P-unit level, the time units are based on the crystal clock frequency.[p]
            [p][b]MSR_Name[/b]: PC3_RCNTR  [b]MSR_Addr[/b]: 0x3F8[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PC3_RCNTR 0x000003F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Rsvd60 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PC3_RCNTR_REGISTER;

/**
  [p]PUNIT_MSR: Primary Compute System (PCS) idle residency counter.  This counter increments at any time that the SOC is in the S0i1 state, with cores, graphics and camera processing system all idle.  Display, Camera Input System or other south complex IPs may be active at this time.  When accessed at the core level via RDMSR, time is measured in the same units as the Maximum Non-Turbo clock frequency.  When accessed at the P-unit level, the time units are based on the crystal clock frequency.[p]
            [p][b]MSR_Name[/b]: PC6_RCNTR  [b]MSR_Addr[/b]: 0x3F9[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PC6_RCNTR 0x000003F9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Rsvd60 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PC6_RCNTR_REGISTER;

/**
  [p]PUNIT_MSR: Primary Compute System (PCS) S0i3 residency counter.  This counter increments at any time that the PCS is ready for S0i3, which includes all compute domains idle (IA cores, graphics and camera) as well as display and camera input system both off.  The SOC may remain in this state for extended periods if south complex IPs are not ready to enter S0i3.  When accessed at the core level via RDMSR, time is measured in the same units as the Maximum Non-Turbo clock frequency.  When accessed at the P-unit level, the time units are based on the crystal clock frequency.[/p]
            [p][b]MSR_Name[/b]: PC7_RCNTR  [b]MSR_Addr[/b]: 0x3FA[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PC7_RCNTR 0x000003FA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Rsvd60 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PC7_RCNTR_REGISTER;
/**
  Was the core C3 residency timer. Core C3 is decprecated on TNT to match SNC. MSR will return 0s
**/
#define SNR_MSR_VIRT_CR_CORE_C3_RESIDENCY 0x000003FC

/**
  Time spent in the Core CState. It is given in units compatible to P1 clock frequency Guaranteed / Maximum Core NonTurbo Frequency. This time will be updated by PCODE only after the CState exit the update of this register has lower priority than actually ensuring that the CState exit occurs.
**/
#define SNR_MSR_CORE_INTERFACE_CR_CC7_RCNTR 0x000003FE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Reserved0 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_CC7_RCNTR_REGISTER;

/**
  MCA Control Register. MRM b1598713:Dont #GP on writing ones to the upper part of MCi_CTL because the software model always write one to the whole 64bit
**/
#define SNR_MSR_BUS_CR_MCI_CTL 0x00000400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=Rsvd0
    UINT32 RobTimeoutReserved : 1;
    //Bits[1:1] Access Type=RW
    UINT32 IdiParityError : 1;
    //Bits[3:2] Access Type=RW
    UINT32 Reserved32 : 2;
    //Bits[4:4] Access Type=RW
    UINT32 BgfPointerError : 1;
    //Bits[5:5] Access Type=Rsvd0
    UINT32 Reserved5 : 1;
    //Bits[6:6] Access Type=RW
    UINT32 GoErr : 1;
    //Bits[38:7] Access Type=Rsvd0
    UINT32 ReservedLow0 : 32;
    //Bits[63:39] Access Type=Rsvd0
    UINT32 Reserved : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BUS_CR_MCI_CTL_REGISTER;

/**
  MCA Status Register. WRMSR uCode generates #GP on non-zero writes. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E
**/
#define SNR_MSR_BUS_CR_MCI_STATUS 0x00000401

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[1:0] Access Type=RW
    UINT32 Mcacod : 2;
    //Bits[3:2] Access Type=RW
    UINT32 ModelSpecific3to2 : 2;
    //Bits[5:4] Access Type=RW
    UINT32 McacodReq : 2;
    //Bits[9:6] Access Type=RW
    UINT32 ModelSpecific9to6 : 4;
    //Bits[10:10] Access Type=RW
    UINT32 McacodRob : 1;
    //Bits[11:11] Access Type=RW
    UINT32 McacodIdi : 1;
    //Bits[15:12] Access Type=RW
    UINT32 ModelSpecific15to12 : 4;
    //Bits[23:16] Access Type=RW
    UINT32 IdiOpcodeOrBgfErr : 8;
    //Bits[24:24] Access Type=RW
    UINT32 GoErrOrBgfErr : 1;
    //Bits[27:25] Access Type=RW
    UINT32 ErrorType : 3;
    //Bits[36:28] Access Type=RW
    UINT32 ModelSpecific36to28 : 9;
    //Bits[37:37] Access Type=RW
    UINT32 Fw : 1;
    //Bits[54:38] Access Type=RW
    UINT32 ModelSpecific54to38 : 17;
    //Bits[55:55] Access Type=RW
    UINT32 Ar : 1;
    //Bits[56:56] Access Type=RW
    UINT32 S : 1;
    //Bits[57:57] Access Type=RW
    UINT32 Pcc : 1;
    //Bits[58:58] Access Type=RW
    UINT32 Addrv : 1;
    //Bits[59:59] Access Type=RW
    UINT32 Miscv : 1;
    //Bits[60:60] Access Type=RW
    UINT32 En : 1;
    //Bits[61:61] Access Type=RW
    UINT32 Uc : 1;
    //Bits[62:62] Access Type=RW
    UINT32 Overflow : 1;
    //Bits[63:63] Access Type=RW
    UINT32 Valid : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BUS_CR_MCI_STATUS_REGISTER;

/**
  MCA Address Register. WRMSR uCode generates #GP on non-zero writes. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E
**/
#define SNR_MSR_BUS_CR_MCI_ADDR 0x00000402

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Bits0to35Low0 : 32;
    //Bits[41:32] Access Type=RW
    UINT32 Bits0to35 : 10;
    //Bits[63:42] Access Type=RW
    UINT32 ModelSpecific63to42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BUS_CR_MCI_ADDR_REGISTER;

/**
  Bus Bank MCA Misc.
**/
#define SNR_MSR_BUS_CR_MCI_MISC 0x00000403

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RW
    UINT32 RecoverableAddressLsb : 6;
    //Bits[8:6] Access Type=RW
    UINT32 AddressMode : 3;
    //Bits[40:9] Access Type=RW
    UINT32 ModelSpecific63to9Low0 : 32;
    //Bits[63:41] Access Type=RW
    UINT32 ModelSpecific63to9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BUS_CR_MCI_MISC_REGISTER;

/**
  L2 MCA Control Register. MRM b1598713:Dont #GP on writing ones to the upper part of MCi_CTL because the software model always write one to the whole 64bit
**/
#define SNR_MSR_L2_CR_MCI_CTL 0x00000404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Data : 1;
    //Bits[32:1] Access Type=Rsvd0
    UINT32 ReservedLow0 : 32;
    //Bits[63:33] Access Type=Rsvd0
    UINT32 Reserved : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_L2_CR_MCI_CTL_REGISTER;

/**
  L2 MCA Status Register. WRMSR uCode generates #GP on non-zero writes. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E
**/
#define SNR_MSR_L2_CR_MCI_STATUS 0x00000405

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 Mcacod : 16;
    //Bits[20:16] Access Type=RW
    UINT32 Mcaw : 5;
    //Bits[30:21] Access Type=RW
    UINT32 EccSyndrome : 10;
    //Bits[31:31] Access Type=RW
    UINT32 EccSyndromeChunkId : 1;
    //Bits[32:32] Access Type=RW
    UINT32 McacValid : 1;
    //Bits[34:33] Access Type=RW
    UINT32 Mcac : 2;
    //Bits[36:35] Access Type=RW
    UINT32 Mscod : 2;
    //Bits[37:37] Access Type=RW
    UINT32 Fw : 1;
    //Bits[51:38] Access Type=RW
    UINT32 McErrorCount : 14;
    //Bits[52:52] Access Type=RW
    UINT32 McErrorCountOverflowBit : 1;
    //Bits[53:53] Access Type=RW
    UINT32 McGreenBit : 1;
    //Bits[54:54] Access Type=RW
    UINT32 McYellowBit : 1;
    //Bits[55:55] Access Type=RW
    UINT32 Ar : 1;
    //Bits[56:56] Access Type=RW
    UINT32 S : 1;
    //Bits[57:57] Access Type=RW
    UINT32 McPccBit : 1;
    //Bits[58:58] Access Type=RW
    UINT32 McAddrvBit : 1;
    //Bits[59:59] Access Type=RW
    UINT32 Miscv : 1;
    //Bits[60:60] Access Type=RW
    UINT32 McEnBit : 1;
    //Bits[61:61] Access Type=RW
    UINT32 McUcBit : 1;
    //Bits[62:62] Access Type=RW
    UINT32 McOvwrBit : 1;
    //Bits[63:63] Access Type=RW
    UINT32 McValidBit : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_L2_CR_MCI_STATUS_REGISTER;

/**
  L2 MCA Address Register. WRMSR uCode generates #GP on non-zero writes. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E
**/
#define SNR_MSR_L2_CR_MCI_ADDR 0x00000406

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 McaAddrLow0 : 32;
    //Bits[41:32] Access Type=RW
    UINT32 McaAddr : 10;
    //Bits[63:42] Access Type=RW
    UINT32 ModelSpecific63to42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_L2_CR_MCI_ADDR_REGISTER;

/**
  L2 Bank MCA Misc.
**/
#define SNR_MSR_L2_CR_MCI_MISC 0x00000407

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RW
    UINT32 RecoverableAddressLsb : 6;
    //Bits[8:6] Access Type=RW
    UINT32 AddressMode : 3;
    //Bits[40:9] Access Type=RW
    UINT32 ModelSpecific63to9Low0 : 32;
    //Bits[63:41] Access Type=RW
    UINT32 ModelSpecific63to9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_L2_CR_MCI_MISC_REGISTER;
/**
  MCA Control Register. MRM b1598713:Dont #GP on writing ones to the upper part of MCi_CTL because the software model always write one to the whole 64bit
**/
#define SNR_MSR_IC_CR_MCI_CTL 0x00000408

/**
  MCA Status Register. Only a write of all 0s is allowed by WRMSR ucode. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E.
**/
#define SNR_MSR_IC_CR_MCI_STATUS 0x00000409

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 McaErrorCode : 16;
    //Bits[31:16] Access Type=RW
    UINT32 ModelSpecificErrorCode : 16;
    //Bits[36:32] Access Type=RW
    UINT32 OtherInfo : 5;
    //Bits[37:37] Access Type=RW
    UINT32 Fw : 1;
    //Bits[52:38] Access Type=RW
    UINT32 CorrectedErrorCount : 15;
    //Bits[54:53] Access Type=RW
    UINT32 ThresholdBasedErrorStatus : 2;
    //Bits[55:55] Access Type=RW
    UINT32 Ar : 1;
    //Bits[56:56] Access Type=RW
    UINT32 S : 1;
    //Bits[57:57] Access Type=RW
    UINT32 Pcc : 1;
    //Bits[58:58] Access Type=RW
    UINT32 Addrv : 1;
    //Bits[59:59] Access Type=RW
    UINT32 Miscv : 1;
    //Bits[60:60] Access Type=RW
    UINT32 En : 1;
    //Bits[61:61] Access Type=RW
    UINT32 Uc : 1;
    //Bits[62:62] Access Type=RW
    UINT32 Over : 1;
    //Bits[63:63] Access Type=RW
    UINT32 Val : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_IC_CR_MCI_STATUS_REGISTER;

/**
  MCA Address Register. Only a write of all 0s is allowed by WRMSR ucode. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E. The creg is restored during C6 restore.
**/
#define SNR_MSR_IC_CR_MCI_ADDR 0x0000040A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 AddressLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Address : 16;
    //Bits[63:48] Access Type=RW
    UINT32 Reserved0 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_IC_CR_MCI_ADDR_REGISTER;

/**
  MCA MISC Register for CMCI. The creg is restored during C6 restore.
**/
#define SNR_MSR_IC_CR_MCI_MISC 0x0000040B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RW
    UINT32 RecoverableAddressLsb : 6;
    //Bits[8:6] Access Type=RW
    UINT32 AddressMode : 3;
    //Bits[40:9] Access Type=RW
    UINT32 ModelSpecificInformationLow0 : 32;
    //Bits[63:41] Access Type=RW
    UINT32 ModelSpecificInformation : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_IC_CR_MCI_MISC_REGISTER;

/**
  MCA Control Register. MRM b1598713:Dont #GP on writing ones to the upper part of MCi_CTL because the software model always write one to the whole 64bit
**/
#define SNR_MSR_MEC_CR_MCI_CTL 0x0000040C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 L1DataPerrUncorr : 1;
    //Bits[1:1] Access Type=RW
    UINT32 L1TagPerrUncorr : 1;
    //Bits[33:2] Access Type=Rsvd0
    UINT32 ReservedLow0 : 32;
    //Bits[63:34] Access Type=Rsvd0
    UINT32 Reserved : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MEC_CR_MCI_CTL_REGISTER;

/**
  MCA Status Register. WRMSR uCode generates #GP on non-zero writes. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E.
**/
#define SNR_MSR_MEC_CR_MCI_STATUS 0x0000040D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 McaErrorCode : 16;
    //Bits[31:16] Access Type=RW
    UINT32 ModelSpecificErrorCode : 16;
    //Bits[36:32] Access Type=RW
    UINT32 OtherInfo : 5;
    //Bits[37:37] Access Type=RW
    UINT32 FmwrUpdatedErrStatus : 1;
    //Bits[52:38] Access Type=RW
    UINT32 CorrectedErrorCount : 15;
    //Bits[54:53] Access Type=RW
    UINT32 ThrshldBasedErrStatus : 2;
    //Bits[55:55] Access Type=RW
    UINT32 Ar : 1;
    //Bits[56:56] Access Type=RW
    UINT32 S : 1;
    //Bits[57:57] Access Type=RW
    UINT32 Pcc : 1;
    //Bits[58:58] Access Type=RW
    UINT32 Addrv : 1;
    //Bits[59:59] Access Type=RW
    UINT32 Miscv : 1;
    //Bits[60:60] Access Type=RW
    UINT32 En : 1;
    //Bits[61:61] Access Type=RW
    UINT32 Uc : 1;
    //Bits[62:62] Access Type=RW
    UINT32 Over : 1;
    //Bits[63:63] Access Type=RW
    UINT32 Val : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MEC_CR_MCI_STATUS_REGISTER;

/**
  MCA Address Register. WRMSR uCode generates #GP on non-zero writes. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E
**/
#define SNR_MSR_MEC_CR_MCI_ADDR 0x0000040E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 AddressLow0 : 32;
    //Bits[41:32] Access Type=RW
    UINT32 Address : 10;
    //Bits[63:42] Access Type=RW
    UINT32 ModelSpecific : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MEC_CR_MCI_ADDR_REGISTER;

/**
  MCA MISC Register. WRMSR uCode generates #GP on non-zero writes. SKL i1350467: Allow WRMSR to update all bits if PPPE_ACTIVE. GLM i1958673 (Microserver MCA Enhancements):Allow WRMSR to update all bits if DEBUG_ERR_INJ_CTL.MCBW_E
**/
#define SNR_MSR_MEC_CR_MCI_MISC 0x0000040F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RW
    UINT32 RecoverableAddressLsb : 6;
    //Bits[8:6] Access Type=RW
    UINT32 AddrMode : 3;
    //Bits[40:9] Access Type=RW
    UINT32 ModelSpecificInfoLow0 : 32;
    //Bits[63:41] Access Type=RW
    UINT32 ModelSpecificInfo : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MEC_CR_MCI_MISC_REGISTER;
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC04_CTL 0x00000410
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC04_STATUS 0x00000411
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC04_ADDR 0x00000412
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC04_MISC 0x00000413
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC05_CTL 0x00000414
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC05_STATUS 0x00000415
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC05_ADDR 0x00000416
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC05_MISC 0x00000417
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC06_CTL 0x00000418
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC06_STATUS 0x00000419
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC06_ADDR 0x0000041A
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC06_MISC 0x0000041B
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC07_CTL 0x0000041C
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC07_STATUS 0x0000041D
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC07_ADDR 0x0000041E
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC07_MISC 0x0000041F
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC08_CTL 0x00000420
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC08_STATUS 0x00000421
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC08_ADDR 0x00000422
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC08_MISC 0x00000423
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC09_CTL 0x00000424
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC09_STATUS 0x00000425
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC09_ADDR 0x00000426
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC09_MISC 0x00000427
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC10_CTL 0x00000428
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC10_STATUS 0x00000429
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC10_ADDR 0x0000042A
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC10_MISC 0x0000042B
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC11_CTL 0x0000042C
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC11_STATUS 0x0000042D
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC11_ADDR 0x0000042E
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC11_MISC 0x0000042F
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC12_CTL 0x00000430
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC12_STATUS 0x00000431
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC12_ADDR 0x00000432
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC12_MISC 0x00000433
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC13_CTL 0x00000434
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC13_STATUS 0x00000435
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC13_ADDR 0x00000436
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC13_MISC 0x00000437
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC14_CTL 0x00000438
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC14_STATUS 0x00000439
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC14_ADDR 0x0000043A
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC14_MISC 0x0000043B
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC15_CTL 0x0000043C
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC15_STATUS 0x0000043D
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC15_ADDR 0x0000043E
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC15_MISC 0x0000043F
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC16_CTL 0x00000440
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC16_STATUS 0x00000441
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC16_ADDR 0x00000442
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC16_MISC 0x00000443
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC17_CTL 0x00000444
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC17_STATUS 0x00000445
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC17_ADDR 0x00000446
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC17_MISC 0x00000447
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC18_CTL 0x00000448
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC18_STATUS 0x00000449
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC18_ADDR 0x0000044A
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC18_MISC 0x0000044B
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC19_CTL 0x0000044C
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC19_STATUS 0x0000044D
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC19_ADDR 0x0000044E
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC19_MISC 0x0000044F
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC20_CTL 0x00000450
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC20_STATUS 0x00000451
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC20_ADDR 0x00000452
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC20_MISC 0x00000453
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC21_CTL 0x00000454
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC21_STATUS 0x00000455
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC21_ADDR 0x00000456
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC21_MISC 0x00000457
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC22_CTL 0x00000458
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC22_STATUS 0x00000459
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC22_ADDR 0x0000045A
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC22_MISC 0x0000045B
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC23_CTL 0x0000045C
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC23_STATUS 0x0000045D
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC23_ADDR 0x0000045E
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC23_MISC 0x0000045F
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC24_CTL 0x00000460
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC24_STATUS 0x00000461
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC24_ADDR 0x00000462
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC24_MISC 0x00000463
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC25_CTL 0x00000464
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC25_STATUS 0x00000465
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC25_ADDR 0x00000466
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC25_MISC 0x00000467
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC26_CTL 0x00000468
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC26_STATUS 0x00000469
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC26_ADDR 0x0000046A
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC26_MISC 0x0000046B
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC27_CTL 0x0000046C
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC27_STATUS 0x0000046D
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC27_ADDR 0x0000046E
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC27_MISC 0x0000046F
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC28_CTL 0x00000470
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC28_STATUS 0x00000471
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC28_ADDR 0x00000472
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC28_MISC 0x00000473
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC29_CTL 0x00000474
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC29_STATUS 0x00000475
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC29_ADDR 0x00000476
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC29_MISC 0x00000477
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC30_CTL 0x00000478
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC30_STATUS 0x00000479
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC30_ADDR 0x0000047A
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC30_MISC 0x0000047B
/**
  Virtual Creg describing the architectural format of the MCi_CTL register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC31_CTL 0x0000047C
/**
  Virtual Creg describing the architectural format of the MCi_STATUS register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC31_STATUS 0x0000047D
/**
  Virtual Creg describing the architectural format of the MCi_ADDR register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC31_ADDR 0x0000047E
/**
  Virtual Creg describing the architectural format of the MCi_MISC register in each bank
**/
#define SNR_MSR_UNCORE_CR_MC31_MISC 0x0000047F

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_0 0x00000480

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=Rsvd0
    UINT32 VmcsRevisionIdentifier : 32;
    //Bits[47:32] Access Type=Rsvd0
    UINT32 LengthOfVmcsRegion : 16;
    //Bits[48:48] Access Type=Rsvd0
    UINT32 PhysicalAddressRange32 : 1;
    //Bits[49:49] Access Type=Rsvd0
    UINT32 ParallelMonitorSupport : 1;
    //Bits[53:50] Access Type=Rsvd0
    UINT32 VmcsMemoryType : 4;
    //Bits[54:54] Access Type=Rsvd0
    UINT32 ReportsVmexitOfInsOuts : 1;
    //Bits[55:55] Access Type=Rsvd0
    UINT32 ExtendedVmxControls : 1;
    //Bits[63:56] Access Type=Rsvd0
    UINT32 Reserved : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_0_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_1 0x00000481

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsPinControls : 32;
    //Bits[32:32] Access Type=RO
    UINT32 ExternalInterruptExiting : 1;
    //Bits[33:33] Access Type=RO
    UINT32 HostInterruptFlag : 1;
    //Bits[34:34] Access Type=RO
    UINT32 InitExiting : 1;
    //Bits[35:35] Access Type=RO
    UINT32 NmiExiting : 1;
    //Bits[36:36] Access Type=RO
    UINT32 SipiExiting : 1;
    //Bits[37:37] Access Type=RO
    UINT32 VirtualNmi : 1;
    //Bits[38:38] Access Type=RO
    UINT32 ActivateVmxPreemptionTimer : 1;
    //Bits[39:39] Access Type=RO
    UINT32 PostedInterruptsProcessing : 1;
    //Bits[63:40] Access Type=RO
    UINT32 Rsvd40 : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_1_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_2 0x00000482

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsProcessorControls : 32;
    //Bits[63:32] Access Type=RO
    UINT32 Allowed1SettingsProcessorControls : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_2_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_3 0x00000483

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsVmexitControls : 32;
    //Bits[32:32] Access Type=RO
    UINT32 SaveCr0AndCr4 : 1;
    //Bits[33:33] Access Type=RO
    UINT32 SaveCr3 : 1;
    //Bits[34:34] Access Type=RO
    UINT32 SaveDebugControls : 1;
    //Bits[35:35] Access Type=RO
    UINT32 SaveSegmentRegisters : 1;
    //Bits[36:36] Access Type=RO
    UINT32 SaveEspEipEflags : 1;
    //Bits[37:37] Access Type=RO
    UINT32 SavePendingDebugExceptions : 1;
    //Bits[38:38] Access Type=RO
    UINT32 SaveInterruptibilityInfo : 1;
    //Bits[39:39] Access Type=RO
    UINT32 SaveActivityState : 1;
    //Bits[40:40] Access Type=RO
    UINT32 SaveWorkingVmcsPointer : 1;
    //Bits[41:41] Access Type=RO
    UINT32 HostAddressSpaceSize : 1;
    //Bits[42:42] Access Type=RO
    UINT32 LoadCr0AndCr4 : 1;
    //Bits[43:43] Access Type=RO
    UINT32 LoadCr3 : 1;
    //Bits[44:44] Access Type=RO
    UINT32 LoadPerfGlobalControl : 1;
    //Bits[45:45] Access Type=RO
    UINT32 LoadSegmentRegisters : 1;
    //Bits[46:46] Access Type=RO
    UINT32 LoadEspEip : 1;
    //Bits[47:47] Access Type=RO
    UINT32 AckIntOnExit : 1;
    //Bits[48:48] Access Type=RO
    UINT32 SaveSysenterMsr : 1;
    //Bits[49:49] Access Type=RO
    UINT32 LoadSysenterMsr : 1;
    //Bits[50:50] Access Type=RO
    UINT32 SaveGuestPat : 1;
    //Bits[51:51] Access Type=RO
    UINT32 LoadHostPat : 1;
    //Bits[52:52] Access Type=RO
    UINT32 SaveGuestEfer : 1;
    //Bits[53:53] Access Type=RO
    UINT32 LoadGuestEfer : 1;
    //Bits[54:54] Access Type=RO
    UINT32 SaveVmxPreemptionTimer : 1;
    //Bits[55:55] Access Type=RO
    UINT32 ClearBndcfgs : 1;
    //Bits[56:56] Access Type=RO
    UINT32 RtitSuppressPip : 1;
    //Bits[57:57] Access Type=RO
    UINT32 ClearIa32RtitCtl : 1;
    //Bits[63:58] Access Type=RO
    UINT32 Rsvd58 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_3_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_4 0x00000484

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsVmentryControls : 32;
    //Bits[32:32] Access Type=RO
    UINT32 LoadCr0Cr4 : 1;
    //Bits[33:33] Access Type=RO
    UINT32 LoadCr3 : 1;
    //Bits[34:34] Access Type=RO
    UINT32 LoadDebugControls : 1;
    //Bits[35:35] Access Type=RO
    UINT32 LoadSegmentRegisters : 1;
    //Bits[36:36] Access Type=RO
    UINT32 LoadEspEipEflags : 1;
    //Bits[37:37] Access Type=RO
    UINT32 LoadPendingDebugRegisters : 1;
    //Bits[38:38] Access Type=RO
    UINT32 LoadInterruptibilityInfo : 1;
    //Bits[39:39] Access Type=RO
    UINT32 LoadActivityState : 1;
    //Bits[40:40] Access Type=RO
    UINT32 LoadWorkingVmcsPointer : 1;
    //Bits[41:41] Access Type=RO
    UINT32 LongModeGuest : 1;
    //Bits[42:42] Access Type=RO
    UINT32 EntryToSmm : 1;
    //Bits[43:43] Access Type=RO
    UINT32 DeactivateDualMonitor : 1;
    //Bits[44:44] Access Type=RO
    UINT32 LoadSysenterMsr : 1;
    //Bits[45:45] Access Type=RO
    UINT32 LoadPerfGlobalCtrl : 1;
    //Bits[46:46] Access Type=RO
    UINT32 LoadGuestPat : 1;
    //Bits[47:47] Access Type=RO
    UINT32 LoadGuestEfer : 1;
    //Bits[48:48] Access Type=RO
    UINT32 LoadBndcfgs : 1;
    //Bits[49:49] Access Type=RO
    UINT32 RtitSuppressPip : 1;
    //Bits[50:50] Access Type=RO
    UINT32 LoadIa32RtitCtl : 1;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_4_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_5 0x00000485

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[4:0] Access Type=RO
    UINT32 PreemptionTimerRate : 5;
    //Bits[5:5] Access Type=RO
    UINT32 ExitsStoreEferLma : 1;
    //Bits[13:6] Access Type=RO
    UINT32 ActivityStatesSupported : 8;
    //Bits[14:14] Access Type=RO
    UINT32 RtitPostVmxon : 1;
    //Bits[15:15] Access Type=RO
    UINT32 SmbaseRdmsr : 1;
    //Bits[24:16] Access Type=RO
    UINT32 Cr3TargetValues : 9;
    //Bits[27:25] Access Type=RO
    UINT32 RecommendedMaxMsrsInMsrList : 3;
    //Bits[28:28] Access Type=RO
    UINT32 VmxoffctrlFeatureEnable : 1;
    //Bits[29:29] Access Type=RO
    UINT32 VmwriteReadOnlyFields : 1;
    //Bits[30:30] Access Type=RO
    UINT32 VoeZeroInstrLength : 1;
    //Bits[31:31] Access Type=RO
    UINT32 Reserved1 : 1;
    //Bits[63:32] Access Type=RO
    UINT32 MsegRevisionId : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_5_REGISTER;
/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_6 0x00000486
/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_7 0x00000487
/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_8 0x00000488

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_9 0x00000489

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Vme : 1;
    //Bits[1:1] Access Type=RO
    UINT32 Pvi : 1;
    //Bits[2:2] Access Type=RO
    UINT32 Tsd : 1;
    //Bits[3:3] Access Type=RO
    UINT32 De : 1;
    //Bits[4:4] Access Type=RO
    UINT32 Pse : 1;
    //Bits[5:5] Access Type=RO
    UINT32 Pae : 1;
    //Bits[6:6] Access Type=RO
    UINT32 Mce : 1;
    //Bits[7:7] Access Type=RO
    UINT32 Pge : 1;
    //Bits[8:8] Access Type=RO
    UINT32 Pce : 1;
    //Bits[9:9] Access Type=RO
    UINT32 Osfxsr : 1;
    //Bits[10:10] Access Type=RO
    UINT32 Xmmexc : 1;
    //Bits[11:11] Access Type=RO
    UINT32 Umip : 1;
    //Bits[12:12] Access Type=RO
    UINT32 Rsvd12 : 1;
    //Bits[13:13] Access Type=RO
    UINT32 VmxEnable : 1;
    //Bits[14:14] Access Type=RO
    UINT32 SmxEnable : 1;
    //Bits[15:15] Access Type=RO
    UINT32 Rsvd15 : 1;
    //Bits[16:16] Access Type=RO
    UINT32 Fsgsbase : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Rsvd17 : 1;
    //Bits[18:18] Access Type=RO
    UINT32 Osxsave : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RO
    UINT32 Smep : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Smap : 1;
    //Bits[31:22] Access Type=RO
    UINT32 Rsvd22 : 10;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_9_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_10 0x0000048A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=RO
    UINT32 HighestIndexForVmcsEncoding : 10;
    //Bits[15:10] Access Type=RO
    UINT32 Rsvd10 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_10_REGISTER;

/**
  VAPIC:LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_11 0x0000048B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsSecondaryProcessorControls : 32;
    //Bits[32:32] Access Type=RO
    UINT32 VirtualizeApicAccess : 1;
    //Bits[33:33] Access Type=RO
    UINT32 EnableEpt : 1;
    //Bits[34:34] Access Type=RO
    UINT32 DescriptorTableExiting : 1;
    //Bits[35:35] Access Type=RO
    UINT32 EnableRdtscp : 1;
    //Bits[36:36] Access Type=RO
    UINT32 VirtualizeX2apic : 1;
    //Bits[37:37] Access Type=RO
    UINT32 EnableVpid : 1;
    //Bits[38:38] Access Type=RO
    UINT32 WbinvdExiting : 1;
    //Bits[39:39] Access Type=RO
    UINT32 UnrestrictedGuest : 1;
    //Bits[40:40] Access Type=RO
    UINT32 ApicRegisterVirtualization : 1;
    //Bits[41:41] Access Type=RO
    UINT32 VirtualInterruptDelivery : 1;
    //Bits[42:42] Access Type=RO
    UINT32 PauseLoopExiting : 1;
    //Bits[43:43] Access Type=RO
    UINT32 RdrandExiting : 1;
    //Bits[44:44] Access Type=RO
    UINT32 Rsvd44 : 1;
    //Bits[45:45] Access Type=RO
    UINT32 VmfuncEnable : 1;
    //Bits[46:46] Access Type=RO
    UINT32 VmcsShadowing : 1;
    //Bits[47:47] Access Type=RO
    UINT32 EnclEnable : 1;
    //Bits[48:48] Access Type=RO
    UINT32 RdseedExiting : 1;
    //Bits[49:49] Access Type=RO
    UINT32 Rsvd49 : 1;
    //Bits[50:50] Access Type=RO
    UINT32 Bp2 : 1;
    //Bits[51:51] Access Type=RO
    UINT32 ConcealRtitVmx : 1;
    //Bits[52:52] Access Type=RO
    UINT32 XsavesXrstors : 1;
    //Bits[53:53] Access Type=RO
    UINT32 Rsvd53 : 1;
    //Bits[54:54] Access Type=RO
    UINT32 MbeEnable : 1;
    //Bits[55:55] Access Type=RO
    UINT32 SppEnable : 1;
    //Bits[56:56] Access Type=RO
    UINT32 Pt2gpa : 1;
    //Bits[57:57] Access Type=RO
    UINT32 UseRdtscScaling : 1;
    //Bits[58:58] Access Type=RO
    UINT32 UserLevelMwait : 1;
    //Bits[59:59] Access Type=RO
    UINT32 Rsvd59 : 1;
    //Bits[60:60] Access Type=RO
    UINT32 EnableEnclv : 1;
    //Bits[61:61] Access Type=RO
    UINT32 EnableEpcVirtualizationExtensions : 1;
    //Bits[63:62] Access Type=RO
    UINT32 Rsvd62 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_11_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_12 0x0000048C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 SupportsEptExecuteOnly : 1;
    //Bits[5:1] Access Type=RO
    UINT32 Rsvd1 : 5;
    //Bits[6:6] Access Type=RO
    UINT32 Supports4levelEptWalk : 1;
    //Bits[7:7] Access Type=RO
    UINT32 Rsvd7 : 1;
    //Bits[8:8] Access Type=RO
    UINT32 SupportsUcEptMemtype : 1;
    //Bits[13:9] Access Type=RO
    UINT32 Rsvd9 : 5;
    //Bits[14:14] Access Type=RO
    UINT32 SupportsWbEptMemtype : 1;
    //Bits[15:15] Access Type=RO
    UINT32 Rsvd15 : 1;
    //Bits[16:16] Access Type=RO
    UINT32 Supports2mbEptPages : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Supports1gbEptPages : 1;
    //Bits[19:18] Access Type=RO
    UINT32 Rsvd18 : 2;
    //Bits[20:20] Access Type=RO
    UINT32 SupportsInvept : 1;
    //Bits[21:21] Access Type=RO
    UINT32 EnableEptAdBits : 1;
    //Bits[22:22] Access Type=RO
    UINT32 EptViolationExitSavesAdditionalState : 1;
    //Bits[24:23] Access Type=RO
    UINT32 Rsvd23 : 2;
    //Bits[25:25] Access Type=RO
    UINT32 SupportsSingleContextInvept : 1;
    //Bits[26:26] Access Type=RO
    UINT32 SupportsAllContextInvept : 1;
    //Bits[31:27] Access Type=RO
    UINT32 Rsvd27 : 5;
    //Bits[32:32] Access Type=RO
    UINT32 SupportsInvvpid : 1;
    //Bits[39:33] Access Type=RO
    UINT32 Rsvd33 : 7;
    //Bits[40:40] Access Type=RO
    UINT32 SupportsIndividualAddressInvvpid : 1;
    //Bits[41:41] Access Type=RO
    UINT32 SupportsSingleContextInvvpid : 1;
    //Bits[42:42] Access Type=RO
    UINT32 SupportsAllContextInvvpid : 1;
    //Bits[43:43] Access Type=RO
    UINT32 SupportsSingleContextRetainGlobalsInvvpid : 1;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_12_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_13 0x0000048D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsTruePinControls : 32;
    //Bits[32:32] Access Type=RO
    UINT32 ExternalInterruptExiting : 1;
    //Bits[33:33] Access Type=RO
    UINT32 HostInterruptFlag : 1;
    //Bits[34:34] Access Type=RO
    UINT32 InitExiting : 1;
    //Bits[35:35] Access Type=RO
    UINT32 NmiExiting : 1;
    //Bits[36:36] Access Type=RO
    UINT32 SipiExiting : 1;
    //Bits[37:37] Access Type=RO
    UINT32 VirtualNmi : 1;
    //Bits[38:38] Access Type=RO
    UINT32 ActivateVmxPreemptionTimer : 1;
    //Bits[39:39] Access Type=RO
    UINT32 PostedInterruptsProcessing : 1;
    //Bits[63:40] Access Type=RO
    UINT32 Rsvd40 : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_13_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_14 0x0000048E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsTrueProcessorControls : 32;
    //Bits[63:32] Access Type=RO
    UINT32 Allowed1SettingsTrueProcessorControls : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_14_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_15 0x0000048F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsTrueVmexitControls : 32;
    //Bits[32:32] Access Type=RO
    UINT32 SaveCr0AndCr4 : 1;
    //Bits[33:33] Access Type=RO
    UINT32 SaveCr3 : 1;
    //Bits[34:34] Access Type=RO
    UINT32 SaveDebugControls : 1;
    //Bits[35:35] Access Type=RO
    UINT32 SaveSegmentRegisters : 1;
    //Bits[36:36] Access Type=RO
    UINT32 SaveEspEipEflags : 1;
    //Bits[37:37] Access Type=RO
    UINT32 SavePendingDebugExceptions : 1;
    //Bits[38:38] Access Type=RO
    UINT32 SaveInterruptibilityInfo : 1;
    //Bits[39:39] Access Type=RO
    UINT32 SaveActivityState : 1;
    //Bits[40:40] Access Type=RO
    UINT32 SaveWorkingVmcsPointer : 1;
    //Bits[41:41] Access Type=RO
    UINT32 HostAddressSpaceSize : 1;
    //Bits[42:42] Access Type=RO
    UINT32 LoadCr0AndCr4 : 1;
    //Bits[43:43] Access Type=RO
    UINT32 LoadCr3 : 1;
    //Bits[44:44] Access Type=RO
    UINT32 LoadPerfGlobalControl : 1;
    //Bits[45:45] Access Type=RO
    UINT32 LoadSegmentRegisters : 1;
    //Bits[46:46] Access Type=RO
    UINT32 LoadEspEip : 1;
    //Bits[47:47] Access Type=RO
    UINT32 AckIntOnExit : 1;
    //Bits[48:48] Access Type=RO
    UINT32 SaveSysenterMsr : 1;
    //Bits[49:49] Access Type=RO
    UINT32 LoadSysenterMsr : 1;
    //Bits[50:50] Access Type=RO
    UINT32 SaveGuestPat : 1;
    //Bits[51:51] Access Type=RO
    UINT32 LoadHostPat : 1;
    //Bits[52:52] Access Type=RO
    UINT32 SaveGuestEfer : 1;
    //Bits[53:53] Access Type=RO
    UINT32 LoadGuestEfer : 1;
    //Bits[54:54] Access Type=RO
    UINT32 SaveVmxPreemptionTimer : 1;
    //Bits[55:55] Access Type=RO
    UINT32 ClearBndcfgs : 1;
    //Bits[56:56] Access Type=RO
    UINT32 RtitSuppressPip : 1;
    //Bits[57:57] Access Type=RO
    UINT32 ClearIa32RtitCtl : 1;
    //Bits[63:58] Access Type=RO
    UINT32 Rsvd58 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_15_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_16 0x00000490

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 Allowed0SettingsTrueVmentryControls : 32;
    //Bits[32:32] Access Type=RO
    UINT32 LoadCr0Cr4 : 1;
    //Bits[33:33] Access Type=RO
    UINT32 LoadCr3 : 1;
    //Bits[34:34] Access Type=RO
    UINT32 LoadDebugControls : 1;
    //Bits[35:35] Access Type=RO
    UINT32 LoadSegmentRegisters : 1;
    //Bits[36:36] Access Type=RO
    UINT32 LoadEspEipEflags : 1;
    //Bits[37:37] Access Type=RO
    UINT32 LoadPendingDebugRegisters : 1;
    //Bits[38:38] Access Type=RO
    UINT32 LoadInterruptibilityInfo : 1;
    //Bits[39:39] Access Type=RO
    UINT32 LoadActivityState : 1;
    //Bits[40:40] Access Type=RO
    UINT32 LoadWorkingVmcsPointer : 1;
    //Bits[41:41] Access Type=RO
    UINT32 LongModeGuest : 1;
    //Bits[42:42] Access Type=RO
    UINT32 EntryToSmm : 1;
    //Bits[43:43] Access Type=RO
    UINT32 DeactivateDualMonitor : 1;
    //Bits[44:44] Access Type=RO
    UINT32 LoadSysenterMsr : 1;
    //Bits[45:45] Access Type=RO
    UINT32 LoadPerfGlobalCtrl : 1;
    //Bits[46:46] Access Type=RO
    UINT32 LoadGuestPat : 1;
    //Bits[47:47] Access Type=RO
    UINT32 LoadGuestEfer : 1;
    //Bits[48:48] Access Type=RO
    UINT32 LoadBndcfgs : 1;
    //Bits[49:49] Access Type=RO
    UINT32 RtitSuppressPip : 1;
    //Bits[50:50] Access Type=RO
    UINT32 LoadIa32RtitCtl : 1;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_16_REGISTER;

/**
  LT-VMX Capability MSR - no CR address . This MSR is read only; uCode generates #GP on any write
**/
#define SNR_VMX_CR_MSR_BASE_17 0x00000491

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 VmfuncEptpSwitchSupport : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VMX_CR_MSR_BASE_17_REGISTER;

/**
  This is an alias for the general purpose counter 0. It allows the full 48-bit WRMSR
**/
#define SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR0 0x000004C1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RW
    UINT32 GpCounter0Lo : 6;
    //Bits[37:6] Access Type=RW
    UINT32 GpCounter0HiLow0 : 32;
    //Bits[47:38] Access Type=RW
    UINT32 GpCounter0Hi : 10;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR0_REGISTER;

/**
  This is an alias for the PerfMon General Purpose counter 1. It allows the full 48-bit WRMSR
**/
#define SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR1 0x000004C2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 GpCounter1ValueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 GpCounter1Value : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR1_REGISTER;

/**
  This is an alias for the general purpose counter 2. It allows the full 48-bit WRMSR
**/
#define SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR2 0x000004C3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 GpCounter2ValueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 GpCounter2Value : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR2_REGISTER;

/**
  This is an alias for the PerfMon General Purpose counter 3. It allows the full 48-bit WRMSR
**/
#define SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR3 0x000004C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 GpCounter3ValueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 GpCounter3Value : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_BNL_CR_PERFMON_GP_FULL_CTR3_REGISTER;

/**
  This CSR holds enable bits for on-chip storage of SMM and
	Code Access Check Violation features.
**/
#define SNR_MSR_CORE_INTERFACE_CR_SMM_FEATURE_CONTROL 0x000004E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Lock : 1;
    //Bits[1:1] Access Type=RW
    UINT32 SmmCpuSaveEn : 1;
    //Bits[2:2] Access Type=RW
    UINT32 SmmCodeChkEn : 1;
    //Bits[31:3] Access Type=RO
    UINT32 Rsvd3 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_SMM_FEATURE_CONTROL_REGISTER;

/**
  This CSR contains bit per logical processor indicating whether the logical processor is in the middle of long flow and hence will delay servicing of SMI. SMI will be serviced after the long flow completes execution.
**/
#define SNR_MSR_T_CR_SMM_DELAYED 0x000004E2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 LogProc : 16;
    //Bits[47:16] Access Type=RSV
    UINT32 Reserved0Low0 : 32;
    //Bits[63:48] Access Type=RSV
    UINT32 Reserved0 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_T_CR_SMM_DELAYED_REGISTER;

/**
  This CSR contains bit per logical processor indicating whether the logical processor is in state where SMIs are blocked and hence will not be able to service SMI. SMI can be serviced after the logical processor exits the state in which SMIs are blocked
**/
#define SNR_MSR_T_CR_SMM_BLOCKED 0x000004E3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 LogProc : 16;
    //Bits[47:16] Access Type=RSV
    UINT32 Reserved0Low0 : 32;
    //Bits[63:48] Access Type=RSV
    UINT32 Reserved0 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_T_CR_SMM_BLOCKED_REGISTER;

/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_CORE_INTERFACE_CR_SMM_ENABLE_MATCH 0x000004E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[11:0] Access Type=RW
    UINT32 ThreadIdMatch : 12;
    //Bits[31:12] Access Type=RO
    UINT32 Rsvd12 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_SMM_ENABLE_MATCH_REGISTER;

/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_CORE_INTERFACE_CR_SMM_ENABLE_MASK 0x000004EA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[11:0] Access Type=RW
    UINT32 ThreadIdMask : 12;
    //Bits[31:12] Access Type=RO
    UINT32 Rsvd12 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_SMM_ENABLE_MASK_REGISTER;

/**
  
**/
#define SNR_MSR_FUNNYIO_CR_IA32_SE_SVN 0x00000500

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Lock : 1;
    //Bits[15:1] Access Type=RO
    UINT32 Rsvd1 : 15;
    //Bits[23:16] Access Type=RO
    UINT32 SinitSeSvn : 8;
    //Bits[31:24] Access Type=RO
    UINT32 Rsvd24 : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_IA32_SE_SVN_REGISTER;

/**
  GLM i1958945: package-scoped MSR. Only bit 0 is actual storage. An entire byte is reserved to avoid the need for a semaphore when writing to it. The bit will be saved and restored in C7+ states. The reset value for the bit is 0. It will not survive warm reset.
**/
#define SNR_MSR_FUNNYIO_CR_SGX_DEBUG_MODE 0x00000503

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 NpkRequest : 1;
    //Bits[1:1] Access Type=RO
    UINT32 NpkActive : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SGX_DEBUG_MODE_REGISTER;

/**
  used for VMEXIT redirection control
**/
#define SNR_MSR_SCP_CR_PROBE_MODE_4 0x00000504

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Interrupt : 1;
    //Bits[1:1] Access Type=RW
    UINT32 ExtInterrupt : 1;
    //Bits[2:2] Access Type=RW
    UINT32 TripleFault : 1;
    //Bits[3:3] Access Type=RW
    UINT32 Init : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Sipi : 1;
    //Bits[5:5] Access Type=RW
    UINT32 IoSmi : 1;
    //Bits[6:6] Access Type=RW
    UINT32 Othersmi : 1;
    //Bits[7:7] Access Type=RW
    UINT32 PndVirtInterrupt : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Nmip : 1;
    //Bits[9:9] Access Type=RW
    UINT32 TaskSwitch : 1;
    //Bits[10:10] Access Type=RW
    UINT32 Cpuid : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Getsec : 1;
    //Bits[12:12] Access Type=RW
    UINT32 Hlt : 1;
    //Bits[13:13] Access Type=RW
    UINT32 Invd : 1;
    //Bits[14:14] Access Type=RW
    UINT32 Invlpg : 1;
    //Bits[15:15] Access Type=RW
    UINT32 Rdpmc : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Rdtsc : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Rsm : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Vmcall : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Vmclear : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Vmlaunch : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Vmptrld : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Vmptrst : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Vmread : 1;
    //Bits[24:24] Access Type=RW
    UINT32 Vmresume : 1;
    //Bits[25:25] Access Type=RW
    UINT32 Vmwrite : 1;
    //Bits[26:26] Access Type=RW
    UINT32 Vmxoff : 1;
    //Bits[27:27] Access Type=RW
    UINT32 Vmxon : 1;
    //Bits[28:28] Access Type=RW
    UINT32 CrAccess : 1;
    //Bits[29:29] Access Type=RW
    UINT32 DrAccess : 1;
    //Bits[30:30] Access Type=RW
    UINT32 Ioexit : 1;
    //Bits[31:31] Access Type=RW
    UINT32 Rdmsr : 1;
    //Bits[32:32] Access Type=RW
    UINT32 Wrmsr : 1;
    //Bits[33:33] Access Type=RW
    UINT32 EntryBadguest : 1;
    //Bits[34:34] Access Type=RW
    UINT32 EntryBadmsr : 1;
    //Bits[35:35] Access Type=RW
    UINT32 Exitfault : 1;
    //Bits[36:36] Access Type=RW
    UINT32 Mwait : 1;
    //Bits[37:37] Access Type=RW
    UINT32 Mtf : 1;
    //Bits[38:38] Access Type=RW
    UINT32 CorruptedVmcs : 1;
    //Bits[39:39] Access Type=RW
    UINT32 Monitor : 1;
    //Bits[40:40] Access Type=RW
    UINT32 Pause : 1;
    //Bits[41:41] Access Type=RW
    UINT32 EntryMca : 1;
    //Bits[42:42] Access Type=RW
    UINT32 CstateSmi : 1;
    //Bits[43:43] Access Type=RW
    UINT32 TprBelowThreshold : 1;
    //Bits[44:44] Access Type=RW
    UINT32 ApicAccess : 1;
    //Bits[63:45] Access Type=RW
    UINT32 NotUsed : 19;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_PROBE_MODE_4_REGISTER;

/**
  used for VMEXIT redirection reason status. Changed to 32 bit MSR on GLm to match SKL.
**/
#define SNR_MSR_SCP_CR_PROBE_MODE_5 0x00000505

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Data : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_PROBE_MODE_5_REGISTER;

/**
  Extended Page Table Pointer
**/
#define SNR_MSR_PMH_CR_EPTP 0x00000510

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[2:0] Access Type=RW
    UINT32 Memtype : 3;
    //Bits[5:3] Access Type=Rsvd0
    UINT32 Reserved0 : 3;
    //Bits[6:6] Access Type=RW
    UINT32 EptAdEn : 1;
    //Bits[10:7] Access Type=Rsvd0
    UINT32 Reserved1 : 4;
    //Bits[11:11] Access Type=RW
    UINT32 EptMbe : 1;
    //Bits[41:12] Access Type=RW
    UINT32 Pml4Addr : 30;
    //Bits[62:42] Access Type=Rsvd0
    UINT32 Reserved2 : 21;
    //Bits[63:63] Access Type=RW
    UINT32 EptEn : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_EPTP_REGISTER;

/**
  Controls for which PDM events LBR data will be sent along with PDM packets if PDM and corresponding event is enabled in PDM_CTRL
**/
#define SNR_MSR_SCP_CR_PDM_LBR 0x00000511

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Intr : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Iret : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Exception : 1;
    //Bits[3:3] Access Type=RW
    UINT32 Msr : 1;
    //Bits[4:4] Access Type=RW
    UINT32 PowerEvent : 1;
    //Bits[5:5] Access Type=RW
    UINT32 PortInOut : 1;
    //Bits[6:6] Access Type=RW
    UINT32 Se : 1;
    //Bits[7:7] Access Type=RW
    UINT32 Wbinvd : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Reserved8 : 1;
    //Bits[9:9] Access Type=RW
    UINT32 CodeBreakpoint : 1;
    //Bits[10:10] Access Type=RW
    UINT32 DataBreakpoint : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RW
    UINT32 Btm : 1;
    //Bits[13:13] Access Type=RW
    UINT32 SmiNmiRsmInitSipi : 1;
    //Bits[14:14] Access Type=RW
    UINT32 MonitorMwait : 1;
    //Bits[27:15] Access Type=RW
    UINT32 Reserved2715 : 13;
    //Bits[28:28] Access Type=RW
    UINT32 LipDisable : 1;
    //Bits[29:29] Access Type=RW
    UINT32 RicDisable : 1;
    //Bits[31:30] Access Type=RW
    UINT32 Reserved3130 : 2;
    //Bits[32:32] Access Type=RW
    UINT32 SendNlip : 1;
    //Bits[33:33] Access Type=RW
    UINT32 SuppressLip : 1;
    //Bits[63:34] Access Type=RW
    UINT32 Spare : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_PDM_LBR_REGISTER;

/**
  Controls which events are enabled for PDM (Probeless Debug Methodology).  When PDM is enabled, it is a debug hook which causes the processor to dump CPU or other state when certain events or instructions are executed.  This SCP specifies which events will trigger state dump when PDM is enabled.
**/
#define SNR_MSR_SCP_CR_PDM_CTRL 0x00000512

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Intr : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Iret : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Exception : 1;
    //Bits[3:3] Access Type=RW
    UINT32 Msr : 1;
    //Bits[4:4] Access Type=RW
    UINT32 PowerEvent : 1;
    //Bits[5:5] Access Type=RW
    UINT32 PortInOut : 1;
    //Bits[6:6] Access Type=RW
    UINT32 Se : 1;
    //Bits[7:7] Access Type=RW
    UINT32 Wbinvd : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Reserved8 : 1;
    //Bits[9:9] Access Type=RW
    UINT32 CodeBreakpoint : 1;
    //Bits[10:10] Access Type=RW
    UINT32 DataBreakpoint : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RW
    UINT32 Btm : 1;
    //Bits[13:13] Access Type=RW
    UINT32 SmiNmiRsmInitSipi : 1;
    //Bits[14:14] Access Type=RW
    UINT32 MonitorMwait : 1;
    //Bits[29:15] Access Type=RW
    UINT32 Reserved2915 : 15;
    //Bits[30:30] Access Type=RW
    UINT32 Dest : 1;
    //Bits[31:31] Access Type=RW
    UINT32 MasterEn : 1;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_PDM_CTRL_REGISTER;

/**
  PPPE Redirection info used for Probe Mode
**/
#define SNR_MSR_SCP_CR_PROBE_MODE_PPPE_REDIRECTION_INFO 0x00000513

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Data : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_PROBE_MODE_PPPE_REDIRECTION_INFO_REGISTER;

/**
  Probe Mode saves/restores the details of the last interruptible micro-code flow. Probe Mode user can cannot change it. Intel Labs can change the contents for debug.
**/
#define SNR_MSR_SCP_CR_ICECTLPMR_PRIOR_STATE 0x00000520

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 ReturnUip : 15;
    //Bits[15:15] Access Type=RW
    UINT32 Reserved : 1;
    //Bits[22:16] Access Type=RW
    UINT32 PriorStateCodesUrl : 7;
    //Bits[23:23] Access Type=RW
    UINT32 UseReturnUip : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Reserved1 : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Spare : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_ICECTLPMR_PRIOR_STATE_REGISTER;

/**
  AET base address. programmed by probe mode software. Ucode will use it as the address where it will send all PDM messages.
**/
#define SNR_MSR_C6_SCP_CR_AET_BASE 0x00000521

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Valid : 1;
    //Bits[5:1] Access Type=RO
    UINT32 Rsvd1 : 5;
    //Bits[37:6] Access Type=RW
    UINT32 AddressLow0 : 32;
    //Bits[41:38] Access Type=RW
    UINT32 Address : 4;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_AET_BASE_REGISTER;

/**
  This is the RTIT Control Register used to control RTIT features
**/
#define SNR_MSR_GLM_CR_RTIT_CTL 0x00000570

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 TraceEn : 1;
    //Bits[1:1] Access Type=RW
    UINT32 CycEn : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Os : 1;
    //Bits[3:3] Access Type=RW
    UINT32 User : 1;
    //Bits[4:4] Access Type=RW
    UINT32 PwrevtEn : 1;
    //Bits[5:5] Access Type=RW
    UINT32 FupOnPtw : 1;
    //Bits[6:6] Access Type=Rsvd0
    UINT32 Reserved0 : 1;
    //Bits[7:7] Access Type=RW
    UINT32 Cr3Filter : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Topa : 1;
    //Bits[9:9] Access Type=RW
    UINT32 MtcEn : 1;
    //Bits[10:10] Access Type=RW
    UINT32 TscEn : 1;
    //Bits[11:11] Access Type=RW
    UINT32 DisRetc : 1;
    //Bits[12:12] Access Type=RW
    UINT32 PtwrEn : 1;
    //Bits[13:13] Access Type=RW
    UINT32 BranchEn : 1;
    //Bits[17:14] Access Type=RW
    UINT32 MtcFreq : 4;
    //Bits[18:18] Access Type=Rsvd0
    UINT32 Reserved2 : 1;
    //Bits[22:19] Access Type=RW
    UINT32 CycThresh : 4;
    //Bits[23:23] Access Type=Rsvd0
    UINT32 Reserved3 : 1;
    //Bits[27:24] Access Type=RW
    UINT32 PsbFreq : 4;
    //Bits[31:28] Access Type=Rsvd0
    UINT32 Reserved4 : 4;
    //Bits[35:32] Access Type=RW
    UINT32 Addr0Cfg : 4;
    //Bits[39:36] Access Type=RW
    UINT32 Addr1Cfg : 4;
    //Bits[55:40] Access Type=Rsvd0
    UINT32 Reserved5 : 16;
    //Bits[56:56] Access Type=RW
    UINT32 InjectPsbPmiOnEnable : 1;
    //Bits[63:57] Access Type=Rsvd0
    UINT32 Reserved6 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_GLM_CR_RTIT_CTL_REGISTER;

/**
  RTIT Status Register
**/
#define SNR_MSR_ARR_CR_RTIT_STATUS 0x00000571

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Filteren : 1;
    //Bits[1:1] Access Type=RO
    UINT32 Contexten : 1;
    //Bits[2:2] Access Type=RO
    UINT32 Triggeren : 1;
    //Bits[3:3] Access Type=Rsvd0
    UINT32 Reserved : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Error : 1;
    //Bits[5:5] Access Type=RW
    UINT32 Stopped : 1;
    //Bits[6:6] Access Type=RW
    UINT32 PendPsb : 1;
    //Bits[7:7] Access Type=RW
    UINT32 PendTopaPmi : 1;
    //Bits[31:8] Access Type=Rsvd0
    UINT32 Reserved0 : 24;
    //Bits[48:32] Access Type=RW
    UINT32 PacketByteCount : 17;
    //Bits[63:49] Access Type=Rsvd0
    UINT32 Reserved1 : 15;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_RTIT_STATUS_REGISTER;

/**
  RTIT CR3 Match registers have the programmed CR3 value for trace filtering. The bits correspond to that defined in CR3.
**/
#define SNR_MSR_SCP_CR_RTIT_CR3_MATCH 0x00000572

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[4:0] Access Type=Rsvd0
    UINT32 Reserved0 : 5;
    //Bits[36:5] Access Type=RW
    UINT32 Cr3Low0 : 32;
    //Bits[63:37] Access Type=RW
    UINT32 Cr3 : 27;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_RTIT_CR3_MATCH_REGISTER;

/**
  Used to configure RTIT IP filtering and Tracestop regions.  This MSR defines the base LIP for the region defined as ADDR0.
**/
#define SNR_MSR_ARR_CR_RTIT_ADDR0_A 0x00000580

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Ip0AddrLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Ip0Addr : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Ip0SignExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_RTIT_ADDR0_A_REGISTER;

/**
  Used to configure IP filtering and Tracestop regions.  This MSR defines the limit LIP for region defined as ADDR0.
**/
#define SNR_MSR_ARR_CR_RTIT_ADDR0_B 0x00000581

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Ip0AddrLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Ip0Addr : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Ip0SignExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_RTIT_ADDR0_B_REGISTER;

/**
  Used to configure RTIT IP filtering and Tracestop regions.  This MSR defines the base LIP for the region defined as ADDR1.
**/
#define SNR_MSR_ARR_CR_RTIT_ADDR1_A 0x00000582

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Ip1AddrLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Ip1Addr : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Ip1SignExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_RTIT_ADDR1_A_REGISTER;

/**
  Used to configure IP filtering and Tracestop regions.  This MSR defines the limit LIP for region defined as ADDR1.
**/
#define SNR_MSR_ARR_CR_RTIT_ADDR1_B 0x00000583

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Ip1AddrLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Ip1Addr : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Ip1SignExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_RTIT_ADDR1_B_REGISTER;

/**
  [p]PUNIT_MSR: VCCGI rail configuration information, including:[/p]
            [list]
            [*]Limitation on the maximum current consumption of the primary compute power plane.[/p]
            [*]Configuration of VR light load management, such as PS0, PS1, etc support
            [/list]
            [p]Set power state amp thresholds to 0A in order to disable that power state.[/p]
            [p][b]MSR_Name / address[/b]: VR_CURRENT_CONFIG / 0x601[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_VR_CURRENT_CONFIG 0x00000601

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[12:0] Access Type=RW_L
    UINT32 CurrentLimit : 13;
    //Bits[30:13] Access Type=RSV
    UINT32 Reserved0 : 18;
    //Bits[31:31] Access Type=RW_L
    UINT32 Lock : 1;
    //Bits[41:32] Access Type=RW
    UINT32 Psi1Threshold : 10;
    //Bits[51:42] Access Type=RW
    UINT32 Psi2Threshold : 10;
    //Bits[61:52] Access Type=RW
    UINT32 Psi3Threshold : 10;
    //Bits[62:62] Access Type=RW
    UINT32 Ps4Enable : 1;
    //Bits[63:63] Access Type=RSV
    UINT32 Reserved1 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_VR_CURRENT_CONFIG_REGISTER;

/**
  [p]PUNIT_MSR: Input voltage regulator configuration parameters. PCODE will read this register during Reset Phase 4.[/p]
            [p][b]MSR_Name[/b]: VR_MISC_CONFIG  [b]MSR_Addr[/b]: 0x603[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_VR_MISC_CONFIG 0x00000603

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 DynamicLoadLineR : 8;
    //Bits[15:8] Access Type=RW
    UINT32 IdleLoadLineR : 8;
    //Bits[23:16] Access Type=RW
    UINT32 LeakLoadLineR : 8;
    //Bits[31:24] Access Type=RW
    UINT32 MinVid : 8;
    //Bits[39:32] Access Type=RW
    UINT32 IoutOffset : 8;
    //Bits[49:40] Access Type=RW
    UINT32 IoutSlope : 10;
    //Bits[50:50] Access Type=RW
    UINT32 IdleExitRampRate : 1;
    //Bits[51:51] Access Type=RW
    UINT32 IdleEntryRampRate : 1;
    //Bits[52:52] Access Type=RW
    UINT32 IdleEntryDecayEnable : 1;
    //Bits[54:53] Access Type=RW
    UINT32 SlowSlewRateConfig : 2;
    //Bits[63:55] Access Type=RO
    UINT32 Rsvd55 : 9;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_VR_MISC_CONFIG_REGISTER;

/**
  Defines units for calculating SKU power, current, energy, resistance and timing parameters.
**/
#define SNR_MSR_CORE_INTERFACE_CR_PACKAGE_POWER_SKU_UNIT 0x00000606

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RW
    UINT32 PwrUnit : 4;
    //Bits[7:4] Access Type=RSV
    UINT32 Reserved0 : 4;
    //Bits[12:8] Access Type=RW
    UINT32 EnergyUnit : 5;
    //Bits[15:13] Access Type=RSV
    UINT32 Reserved1 : 3;
    //Bits[19:16] Access Type=RW
    UINT32 TimeUnit : 4;
    //Bits[23:20] Access Type=RSV
    UINT32 Reserved2 : 4;
    //Bits[27:24] Access Type=RW
    UINT32 CurrentUnit : 4;
    //Bits[31:28] Access Type=RW
    UINT32 ResistanceUnit : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PACKAGE_POWER_SKU_UNIT_REGISTER;

/**
  TNT: Match HSW e4400429 behavior: read/write THREAD_P_REQ[P_STATE_OFFSET]. Note that per BXT HSDES 1015230753, this MSR was ignored on GLM
**/
#define SNR_MSR_VIRT_CR_PST_CONFIG_CONTROL 0x00000609

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 PStateOffset : 8;
    //Bits[31:8] Access Type=RO
    UINT32 Rsvd8 : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_PST_CONFIG_CONTROL_REGISTER;

/**
  [p]PUNIT_MSR: Time spent in the C1E state.  When accessed at the core level via RDMSR, time is measured in the same units as the Maximum Non-Turbo clock frequency.  When accessed at the P-unit level, the time units are based on the crystal clock frequency.[p]
            [p][b]MSR_Name[/b]: PC2_RCNTR  [b]MSR_Addr[/b]: 0x60D[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PC2_RCNTR 0x0000060D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Rsvd60 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PC2_RCNTR_REGISTER;

/**
  BDW-C0 ECO 4664533: New virtual MSR for PL3. Read/write data from/to pcode through internal mailbox.
**/
#define SNR_MSR_UCODE_CR_PL3_CONTROL 0x00000615

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 PowerLimit : 15;
    //Bits[15:15] Access Type=RW
    UINT32 Enable : 1;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[23:17] Access Type=RW
    UINT32 TimeWindow : 7;
    //Bits[30:24] Access Type=RW
    UINT32 DutyCycle : 7;
    //Bits[31:31] Access Type=RW
    UINT32 Lock : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_PL3_CONTROL_REGISTER;

/**
  RAPL power limit for DDR domain. This register is written by platform software and read by pcode once per 1ms. There are actually two instances of this register: one with MSR access and one with MMIO access. There is a separate PECI/PCS command which is also analogous.
**/
#define SNR_MSR_CORE_INTERFACE_CR_DDR_RAPL_LIMIT 0x00000618

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW_L
    UINT32 Limit1Power : 15;
    //Bits[15:15] Access Type=RW_L
    UINT32 Limit1Enable : 1;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[23:17] Access Type=RW_L
    UINT32 CtrlTimeWin : 7;
    //Bits[31:24] Access Type=RO
    UINT32 Rsvd24 : 8;
    //Bits[46:32] Access Type=RW_L
    UINT32 Limit2Power : 15;
    //Bits[47:47] Access Type=RW_L
    UINT32 Limit2Enable : 1;
    //Bits[48:48] Access Type=RO
    UINT32 Rsvd48 : 1;
    //Bits[53:49] Access Type=RW_L
    UINT32 Limit2TimeWindowY : 5;
    //Bits[55:54] Access Type=RW_L
    UINT32 Limit2TimeWindowX : 2;
    //Bits[62:56] Access Type=RO
    UINT32 Rsvd56 : 7;
    //Bits[63:63] Access Type=RW_L
    UINT32 Locked : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_DDR_RAPL_LIMIT_REGISTER;

/**
  Current Uncore Ratio
**/
#define SNR_MSR_CORE_INTERFACE_CR_UNCORE_PERF_STATUS 0x00000621

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RO
    UINT32 CurrentClrRatio : 7;
    //Bits[31:7] Access Type=RO
    UINT32 Reserved0 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_UNCORE_PERF_STATUS_REGISTER;

/**
  Time spent in the Package CState. It is given in units compatible to P1 clock frequency Guaranteed / Maximum Core NonTurbo Frequency. This time will be updated by PCODE only after the CState exit the update of this register has lower priority than actually ensuring that the CState exit occurs.
**/
#define SNR_MSR_CORE_INTERFACE_CR_PC8_RCNTR 0x00000630

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Rsvd60 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PC8_RCNTR_REGISTER;

/**
  [p]PUNIT_MSR: Currently unused residency counter.  When accessed at the core level via RDMSR, time is measured in the same units as the Maximum Non-Turbo clock frequency.  When accessed at the P-unit level, the time units are based on the crystal clock frequency.[/p]
            [p][b]MSR_Name[/b]: PC9_RCNTR  [b]MSR_Addr[/b]: 0x631[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PC9_RCNTR 0x00000631

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Rsvd60 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PC9_RCNTR_REGISTER;

/**
  [p]PUNIT_MSR: SOC S0i3 residency counter.  This counter increments at any time that the entire SOC is in an S0i3 state.  This register does not distinguish variations on S0i3, it increments for all variations.  This residency counter may count some time during entry into and exit from S0i3.  When accessed at the core level via RDMSR, time is measured in the same units as the Maximum Non-Turbo clock frequency.  When accessed at the P-unit level, the time units are based on the crystal clock frequency.[/p]
            [p][b]MSR_Name[/b]: PC10_RCNTR  [b]MSR_Addr[/b]: 0x632[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_PC10_RCNTR 0x00000632

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Rsvd60 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PC10_RCNTR_REGISTER;

/**
  [p]PUNIT_MSR: This register describes the OS tolerated wake latency for the C8 state. The minimum core C-state is calculated among all IA cores in the die, 
               and power management firmware maps that resolved C-state to the appropriate Interrupt Response Tolerance latency setting.
               This setting will then affect package level idle state entry decisions such that it is guaranteed that the cores will wake up in less than the tolerated latency.[/p]
            [p]Registers initialized by Pcode at reset. BIOS cannot update these MSRs prior to PCODE_INIT_DONE, and it is not recommended that BIOS ever overwrite.[/p]
            [p][b]MSR_Name[/b]:  C_STATE_LATENCY_CONTROL_3  [b]MSR_Addr[/b]: 0x633[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_C_STATE_LATENCY_CONTROL_3 0x00000633

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=RW
    UINT32 Value : 10;
    //Bits[12:10] Access Type=RW
    UINT32 Multiplier : 3;
    //Bits[14:13] Access Type=RSV
    UINT32 Reserved0 : 2;
    //Bits[15:15] Access Type=RW
    UINT32 Valid : 1;
    //Bits[31:16] Access Type=RSV
    UINT32 Reserved1 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_C_STATE_LATENCY_CONTROL_3_REGISTER;

/**
  [p]PUNIT_MSR: This register describes the OS tolerated wake latency for the C9 state. The minimum core C-state is calculated among all IA cores in the die, 
               and power management firmware maps that resolved C-state to the appropriate Interrupt Response Tolerance latency setting.
               This setting will then affect package level idle state entry decisions such that it is guaranteed that the cores will wake up in less than the tolerated latency.[/p]
            [p]Registers initialized by Pcode at reset. BIOS cannot update these MSRs prior to PCODE_INIT_DONE, and it is not recommended that BIOS ever overwrite.[/p]
            [p][b]MSR_Name[/b]: C_STATE_LATENCY_CONTROL_4  [b]MSR_Addr[/b]: 0x634[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_C_STATE_LATENCY_CONTROL_4 0x00000634

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=RW
    UINT32 Value : 10;
    //Bits[12:10] Access Type=RW
    UINT32 Multiplier : 3;
    //Bits[14:13] Access Type=RSV
    UINT32 Reserved0 : 2;
    //Bits[15:15] Access Type=RW
    UINT32 Valid : 1;
    //Bits[31:16] Access Type=RSV
    UINT32 Reserved1 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_C_STATE_LATENCY_CONTROL_4_REGISTER;

/**
  [p]PUNIT_MSR: This register describes the OS tolerated wake latency for the C10 state. The minimum core C-state is calculated among all IA cores in the die, 
               and power management firmware maps that resolved C-state to the appropriate Interrupt Response Tolerance latency setting.
               This setting will then affect package level idle state entry decisions such that it is guaranteed that the cores will wake up in less than the tolerated latency.[/p]
            [p]Registers initialized by Pcode at reset. BIOS cannot update these MSRs prior to PCODE_INIT_DONE, and it is not recommended that BIOS ever overwrite.[/p]
            [p][b]MSR_Name[/b]:  C_STATE_LATENCY_CONTROL_5  [b]MSR_Addr[/b]: 12'h635[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_C_STATE_LATENCY_CONTROL_5 0x00000635

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=RW
    UINT32 Value : 10;
    //Bits[12:10] Access Type=RW
    UINT32 Multiplier : 3;
    //Bits[14:13] Access Type=RSV
    UINT32 Reserved0 : 2;
    //Bits[15:15] Access Type=RW
    UINT32 Valid : 1;
    //Bits[31:16] Access Type=RSV
    UINT32 Reserved1 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_C_STATE_LATENCY_CONTROL_5_REGISTER;

/**
  Input voltage regulator configuration parameters.
**/
#define SNR_MSR_CORE_INTERFACE_CR_VR_MISC_CONFIG2 0x00000636

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 FastRampVoltage : 8;
    //Bits[15:8] Access Type=RW
    UINT32 MinC8Voltage : 8;
    //Bits[31:16] Access Type=RO
    UINT32 Rsvd16 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_VR_MISC_CONFIG2_REGISTER;

/**
  The PRIMARY_PLANE_TURBO_POWER_POLICY and SECONDARY_PLANE_TURBO_POWER_POLICY are used together as hints to balance the power budget between the primary (IA core) and secondary (Graphics) power planes.   This biasing is effectively a performance biasing, and it helps Punit firmware assess where software needs performance the most
**/
#define SNR_MSR_CORE_INTERFACE_CR_PRIMARY_PLANE_TURBO_POWER_POLICY 0x0000063A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[4:0] Access Type=RW
    UINT32 Priptp : 5;
    //Bits[31:5] Access Type=RO
    UINT32 Reserved0 : 27;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PRIMARY_PLANE_TURBO_POWER_POLICY_REGISTER;
/**
  Memory RAPL performance excursion counter.  This register can report the performance impact of power limiting.
**/
#define SNR_MSR_PCU_CR_PRIMARY_PLANE_OVERFLOW_STATUS 0x0000063B

/**
  his register is used by BIOS/OS/Integrated Graphics Driver/CPM Driver to limit the power budget of the Secondary Power Plane.  The overall package turbo power limitation is controlled by PACKAGE_RAPL_LIMIT.
**/
#define SNR_MSR_CORE_INTERFACE_CR_SECONDARY_PLANE_TURBO_POWER_LIMIT 0x00000640

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 NonIaPpPwrLim : 15;
    //Bits[15:15] Access Type=RW
    UINT32 PwrLimCtrlEn : 1;
    //Bits[16:16] Access Type=RW
    UINT32 PpClampLim : 1;
    //Bits[23:17] Access Type=RW
    UINT32 CtrlTimeWin : 7;
    //Bits[30:24] Access Type=RSV
    UINT32 Reserved0 : 7;
    //Bits[31:31] Access Type=RW
    UINT32 SpPwrLimLock : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_SECONDARY_PLANE_TURBO_POWER_LIMIT_REGISTER;
/**
  [p]Reports total energy consumed across all IA cores.  The energy status is reported in units which are defined by PACKAGE_POWER_SKU_UNIT_MSR.ENERGY_UNIT.  The counter will wrap around and continue counting from zero when it reaches its limit and therefore should be polled sufficiently frequently to avoid aliasing.  Typically, software will calculate delta energy and delta time and divide the two to estimate Watts consumed over a time window.  The value of this register is updated at approximately every 1ms.[/p]
           [p]To calculate Watts:[/p]
           [sp][sp][b]Watts = delta(energy) / delta(time) / 2^PACKAGE_POWER_SKU_UNIT_MSR.ENERGY_UNIT[/b]
   [IntelRsvd]THIS REGISTER IS DUPLICATED IN THE PCU IO SPACE, XML CHANGES MUST BE MADE IN BOTH PLACES[/IntelRsvd]
**/
#define SNR_MSR_CORE_INTERFACE_CR_SECONDARY_PLANE_ENERGY_STATUS 0x00000641

/**
  The PRIMARY_PLANE_TURBO_POWER_POLICY and SECONDARY_PLANE_TURBO_POWER_POLICY are used together as hints to balance the power budget between the primary (IA core) and secondary (Graphics) power planes.   This biasing is effectively a performance biasing, and it helps Punit firmware assess where software needs performance the most
**/
#define SNR_MSR_CORE_INTERFACE_CR_SECONDARY_PLANE_TURBO_POWER_POLICY 0x00000642

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[4:0] Access Type=RW
    UINT32 Secptp : 5;
    //Bits[31:5] Access Type=RO
    UINT32 Reserved0 : 27;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_SECONDARY_PLANE_TURBO_POWER_POLICY_REGISTER;

/**
  Level 1 configurable TDP settings.
**/
#define SNR_MSR_CORE_INTERFACE_CR_CONFIG_TDP_LEVEL1 0x00000649

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 PkgTdp : 15;
    //Bits[15:15] Access Type=RO
    UINT32 Rsvd15 : 1;
    //Bits[23:16] Access Type=RW
    UINT32 TdpRatio : 8;
    //Bits[31:24] Access Type=RO
    UINT32 Rsvd24 : 8;
    //Bits[46:32] Access Type=RW
    UINT32 PkgMaxPwr : 15;
    //Bits[47:47] Access Type=RO
    UINT32 Rsvd47 : 1;
    //Bits[62:48] Access Type=RW
    UINT32 PkgMinPwr : 15;
    //Bits[63:63] Access Type=RO
    UINT32 Rsvd63 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_CONFIG_TDP_LEVEL1_REGISTER;

/**
  Level 2 configurable TDP settings. On sku that does not support config tdp, these to registers will report out 0.
**/
#define SNR_MSR_CORE_INTERFACE_CR_CONFIG_TDP_LEVEL2 0x0000064A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW
    UINT32 PkgTdp : 15;
    //Bits[15:15] Access Type=RO
    UINT32 Rsvd15 : 1;
    //Bits[23:16] Access Type=RW
    UINT32 TdpRatio : 8;
    //Bits[31:24] Access Type=RO
    UINT32 Rsvd24 : 8;
    //Bits[46:32] Access Type=RW
    UINT32 PkgMaxPwr : 15;
    //Bits[47:47] Access Type=RO
    UINT32 Rsvd47 : 1;
    //Bits[62:48] Access Type=RW
    UINT32 PkgMinPwr : 15;
    //Bits[63:63] Access Type=RO
    UINT32 Rsvd63 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_CONFIG_TDP_LEVEL2_REGISTER;

/**
  Rd/Wr register to allow platform SW to select TDP point and set lock via all three interfaces (MSR, MMIO and PECI/PCS)
**/
#define SNR_MSR_CORE_INTERFACE_CR_CONFIG_TDP_CONTROL 0x0000064B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[2:0] Access Type=RW_L
    UINT32 TdpLevel : 3;
    //Bits[30:3] Access Type=RO
    UINT32 Rsvd3 : 28;
    //Bits[31:31] Access Type=RW_L
    UINT32 ConfigTdpLock : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_CONFIG_TDP_CONTROL_REGISTER;
/**
  Reports the actual energy used by the platform
**/
#define SNR_MSR_CORE_INTERFACE_CR_PLATFORM_ENERGY_STATUS 0x0000064D
/**
  productivity cycles counter (HWP)
**/
#define SNR_MSR_PMG_CR_PST_PCNT 0x0000064E

/**
  Interface to allow software to determine what is causing 
  resolved frequency to be clamped 
  below the requested frequency. Status bits are updated by Punit firmware
  every millisecond and log bits are set upon observing the corresponding 
  status bit being set.  Software may clear log bits to track performance limit
  reasons inbetween reads of this register.
**/
#define SNR_MSR_CORE_INTERFACE_CR_IA_PERF_LIMIT_REASONS 0x0000064F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 ProchotStatus : 1;
    //Bits[1:1] Access Type=RO
    UINT32 ThermalStatus : 1;
    //Bits[2:2] Access Type=RO
    UINT32 Pl1Status : 1;
    //Bits[3:3] Access Type=RO
    UINT32 Pl2Status : 1;
    //Bits[4:4] Access Type=ROFW
    UINT32 RsrLimit : 1;
    //Bits[5:5] Access Type=ROFW
    UINT32 Ratl : 1;
    //Bits[6:6] Access Type=ROFW
    UINT32 VrThermalert : 1;
    //Bits[7:7] Access Type=ROFW
    UINT32 VrTdc : 1;
    //Bits[8:8] Access Type=ROFW
    UINT32 Other : 1;
    //Bits[9:9] Access Type=ROFW
    UINT32 SpareIa9 : 1;
    //Bits[10:10] Access Type=ROFW
    UINT32 PbmPl1 : 1;
    //Bits[11:11] Access Type=ROFW
    UINT32 PbmPl2 : 1;
    //Bits[12:12] Access Type=ROFW
    UINT32 MaxTurboLimit : 1;
    //Bits[13:13] Access Type=ROFW
    UINT32 TurboAtten : 1;
    //Bits[14:14] Access Type=ROFW
    UINT32 SpareIa14 : 1;
    //Bits[15:15] Access Type=ROFW
    UINT32 SpareIa15 : 1;
    //Bits[16:16] Access Type=RW0C_FW
    UINT32 ProchotLog : 1;
    //Bits[17:17] Access Type=RW0C_FW
    UINT32 ThermalLog : 1;
    //Bits[18:18] Access Type=RW0C_FW
    UINT32 SpareIa2Log : 1;
    //Bits[19:19] Access Type=RW0C_FW
    UINT32 SpareIa3Log : 1;
    //Bits[20:20] Access Type=RW0C_FW
    UINT32 RsrLimitLog : 1;
    //Bits[21:21] Access Type=RW0C_FW
    UINT32 RatlLog : 1;
    //Bits[22:22] Access Type=RW0C_FW
    UINT32 VrThermalertLog : 1;
    //Bits[23:23] Access Type=RW0C_FW
    UINT32 VrTdcLog : 1;
    //Bits[24:24] Access Type=RW0C_FW
    UINT32 OtherLog : 1;
    //Bits[25:25] Access Type=RW0C_FW
    UINT32 SpareIa9Log : 1;
    //Bits[26:26] Access Type=RW0C_FW
    UINT32 PbmPl1Log : 1;
    //Bits[27:27] Access Type=RW0C_FW
    UINT32 PbmPl2Log : 1;
    //Bits[28:28] Access Type=RW0C_FW
    UINT32 MaxTurboLimitLog : 1;
    //Bits[29:29] Access Type=RW0C_FW
    UINT32 TurboAttenLog : 1;
    //Bits[30:30] Access Type=RW0C_FW
    UINT32 SpareIa14Log : 1;
    //Bits[31:31] Access Type=RW0C_FW
    UINT32 SpareIa15Log : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_IA_PERF_LIMIT_REASONS_REGISTER;

/**
  Fast wrmsr/rdmsr control MSR. Each bit refers to one MSR and indicate if this MSR is set to use the FAST_UNCORE_MSRS mode.
**/
#define SNR_VIRT_CR_FAST_UNCORE_MSRS_CTL 0x00000657

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 HwpRequest : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VIRT_CR_FAST_UNCORE_MSRS_CTL_REGISTER;
/**
  Sum the cycles per number of active cores
**/
#define SNR_MSR_CORE_INTERFACE_CR_PKG_IA_C0_ANY_SUM 0x00000658
/**
  Sum the cycles of active cores
**/
#define SNR_MSR_CORE_INTERFACE_CR_PKG_IA_C0_ANY 0x00000659
/**
  Sum the cycles of active GT
**/
#define SNR_MSR_CORE_INTERFACE_CR_PKG_GT_C0_ANY 0x0000065A
/**
  Sum the cycles of overlap time between any IA cores and GT
**/
#define SNR_MSR_CORE_INTERFACE_CR_PKG_GT_AND_IA_OVERLAP 0x0000065B

/**
  Platform power limit
**/
#define SNR_MSR_CORE_INTERFACE_CR_PLATFORM_POWER_LIMIT 0x0000065C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[14:0] Access Type=RW_L
    UINT32 PowerLimit1 : 15;
    //Bits[15:15] Access Type=RW_L
    UINT32 PowerLimit1En : 1;
    //Bits[16:16] Access Type=RW_L
    UINT32 CriticalPowerClamp1 : 1;
    //Bits[23:17] Access Type=RW_L
    UINT32 PowerLimit1Time : 7;
    //Bits[31:24] Access Type=RO
    UINT32 Rsvd24 : 8;
    //Bits[46:32] Access Type=RW_L
    UINT32 PowerLimit2 : 15;
    //Bits[47:47] Access Type=RW_L
    UINT32 PowerLimit2En : 1;
    //Bits[48:48] Access Type=RW_L
    UINT32 CriticalPowerClamp2 : 1;
    //Bits[62:49] Access Type=RO
    UINT32 Rsvd49 : 14;
    //Bits[63:63] Access Type=RW_KL
    UINT32 Lock : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PLATFORM_POWER_LIMIT_REGISTER;

/**
  Fast uncore MSR - dirty bit status register. Since TNT emulates Fast MSRs, the status will always be 0
**/
#define SNR_VIRT_CR_FAST_UNCORE_MSRS_STATUS 0x0000065E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Cr0 : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VIRT_CR_FAST_UNCORE_MSRS_STATUS_REGISTER;

/**
  Fast wrmsr/rdmsr enumeration MSR. This is read-only MSR. Each bit refers to one MSR and indicate if this MSR
is enabled to be use in FAST_UNCORE_MSRS.
**/
#define SNR_VIRT_CR_FAST_UNCORE_MSRS_CAPABILITY 0x0000065F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 HwpRequest : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_VIRT_CR_FAST_UNCORE_MSRS_CAPABILITY_REGISTER;

/**
  Time spent in the Module CState. It is given in units compatible to P1 clock frequency Guaranteed / Maximum Core NonTurbo Frequency. This time will be updated by PCODE only after the CState exit the update of this register has lower priority than actually ensuring that the CState exit occurs.
**/
#define SNR_MSR_CORE_INTERFACE_CR_MC6_RCNTR 0x00000664

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CounterLow0 : 32;
    //Bits[59:32] Access Type=RW
    UINT32 Counter : 28;
    //Bits[63:60] Access Type=RO
    UINT32 Reserved0 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_MC6_RCNTR_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP00 0x00000680

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP00_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP01 0x00000681

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP01_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP02 0x00000682

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP02_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP03 0x00000683

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP03_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP04 0x00000684

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP04_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP05 0x00000685

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP05_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP06 0x00000686

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP06_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP07 0x00000687

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP07_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP08 0x00000688

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP08_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP09 0x00000689

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP09_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP10 0x0000068A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP10_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP11 0x0000068B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP11_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP18 0x00000692

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP18_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP19 0x00000693

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP19_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP20 0x00000694

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP20_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP21 0x00000695

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP21_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP22 0x00000696

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP22_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP23 0x00000697

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP23_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP24 0x00000698

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP24_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP25 0x00000699

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP25_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP26 0x0000069A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP26_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP27 0x0000069B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP27_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP28 0x0000069C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP28_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP29 0x0000069D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP29_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP30 0x0000069E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP30_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT
**/
#define SNR_MSR_VIRT_CR_LBR_CLIP31 0x0000069F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_CLIP31_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP00 0x000006C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP00_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP01 0x000006C1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP01_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP02 0x000006C2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP02_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP03 0x000006C3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP03_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP04 0x000006C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP04_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP05 0x000006C5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP05_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP06 0x000006C6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP06_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP07 0x000006C7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP07_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP08 0x000006C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP08_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP09 0x000006C9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP09_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP10 0x000006CA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP10_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP11 0x000006CB

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP11_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP12 0x000006CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP12_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP13 0x000006CD

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP13_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP14 0x000006CE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP14_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP15 0x000006CF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP15_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP16 0x000006D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP16_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP17 0x000006D1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP17_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP18 0x000006D2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP18_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP19 0x000006D3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP19_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP20 0x000006D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP20_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP21 0x000006D5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP21_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP22 0x000006D6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP22_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP23 0x000006D7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP23_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP24 0x000006D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP24_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP25 0x000006D9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP25_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP26 0x000006DA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP26_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP27 0x000006DB

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP27_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP28 0x000006DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP28_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP29 0x000006DD

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP29_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP30 0x000006DE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP30_REGISTER;

/**
  Virtual MSR - not an actual creg on TNT.
**/
#define SNR_MSR_VIRT_CR_LBR_BLIP31 0x000006DF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LipLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Lip : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 LipExt : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LBR_BLIP31_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000700

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000701

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_0_CLIENT 0x00000702

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_1_CLIENT 0x00000703

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_1_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000708

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000709

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_0_CLIENT 0x0000070A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_1_CLIENT 0x0000070B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_1_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000710

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000711

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_0_CLIENT 0x00000712

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_1_CLIENT 0x00000713

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_1_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000718

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000719

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_0_CLIENT 0x0000071A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_1_CLIENT 0x0000071B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_1_CLIENT_REGISTER;
/**
  Server only residency counter select.
**/
#define SNR_MSR_C6_SCP_CR_RESIDENCY_CTR_SELECT_SRVR 0x0000071B
/**
  Server residency counter MSR
**/
#define SNR_MSR_VIRT_CR_RESIDENCY_CTR_SRVR 0x0000071D

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000720

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000721

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_0_CLIENT 0x00000722

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_1_CLIENT 0x00000723

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_1_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000728

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000729

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_0_CLIENT 0x0000072A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_1_CLIENT 0x0000072B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_1_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000730

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000731

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCT_0_CLIENT 0x00000732

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCT_1_CLIENT 0x00000733

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE6_CR_CBOPMONCT_1_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000738

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000739

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCT_0_CLIENT 0x0000073A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCT_0_CLIENT_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCT_1_CLIENT 0x0000073B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE7_CR_CBOPMONCT_1_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_CR_CBOPMONCTRCTRL_0_CLIENT 0x00000768

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_CR_CBOPMONCTRCTRL_0_CLIENT_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_CR_CBOPMONCTRCTRL_1_CLIENT 0x00000769

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[17:16] Access Type=RO
    UINT32 Rsvd16 : 2;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[31:30] Access Type=RO
    UINT32 Rsvd30 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_CR_CBOPMONCTRCTRL_1_CLIENT_REGISTER;

/**
  Part of the HWP interface fo OS usage.  This is an MSR to enable HWP and specific modes.
**/
#define SNR_MSR_CORE_INTERFACE_CR_PM_ENABLE 0x00000770

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 HwpEnable : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Reserved : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_PM_ENABLE_REGISTER;

/**
  static report of the enumeration values, BDX new: IA32_HWP_BASE_CAPABILITY is defined to be thread-scoped, read-only MSR, but implemented as a package-scoped MSR. Pcode initializes this register. Ucode only reads this
**/
#define SNR_MSR_CORE_INTERFACE_CR_HWP_CAPABILITIES 0x00000771

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 HighestPerformance : 8;
    //Bits[15:8] Access Type=RW
    UINT32 GuaranteedPerformance : 8;
    //Bits[23:16] Access Type=RW
    UINT32 MostEfficientPerformance : 8;
    //Bits[31:24] Access Type=RW
    UINT32 LowestLinearPerformance : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_HWP_CAPABILITIES_REGISTER;

/**
  Used to convey the OSs performance request {Min, Max. Desired, tc} to apply across threads if requested.
**/
#define SNR_MSR_CORE_INTERFACE_CR_HWP_REQUEST_PKG 0x00000772

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 MinimumPerformance : 8;
    //Bits[15:8] Access Type=RW
    UINT32 MaximumPerformance : 8;
    //Bits[23:16] Access Type=RW
    UINT32 DesiredPerformance : 8;
    //Bits[31:24] Access Type=RW
    UINT32 EnergyPerformancePreference : 8;
    //Bits[41:32] Access Type=RW
    UINT32 ActivityWindow : 10;
    //Bits[47:42] Access Type=RO
    UINT32 Rsvd42 : 6;
    //Bits[63:48] Access Type=RO
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_HWP_REQUEST_PKG_REGISTER;

/**
  Used to control native interrupt generation (changes, excursions).
**/
#define SNR_MSR_CORE_INTERFACE_CR_HWP_INTERRUPT 0x00000773

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 ChangeToGuaranteed : 1;
    //Bits[1:1] Access Type=RW
    UINT32 ExcursionToMinimum : 1;
    //Bits[2:2] Access Type=RW
    UINT32 ChangeToHighest : 1;
    //Bits[3:3] Access Type=RW
    UINT32 PeciOverride : 1;
    //Bits[35:4] Access Type=RO
    UINT32 Rsvd4Low0 : 32;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_HWP_INTERRUPT_REGISTER;

/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_PCU_CR_PECI_HWP_REQUEST_INFO 0x00000775

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RO
    UINT32 MinimumPerformance : 8;
    //Bits[15:8] Access Type=RO
    UINT32 MaximumPerformance : 8;
    //Bits[23:16] Access Type=RO
    UINT32 Rsvd16 : 8;
    //Bits[31:24] Access Type=RO
    UINT32 EnergyPerformancePreference : 8;
    //Bits[59:32] Access Type=RO
    UINT32 Rsvd32 : 28;
    //Bits[60:60] Access Type=RO
    UINT32 EppPeciOverride : 1;
    //Bits[61:61] Access Type=RO
    UINT32 Rsvd61 : 1;
    //Bits[62:62] Access Type=RO
    UINT32 MaxPeciOverride : 1;
    //Bits[63:63] Access Type=RO
    UINT32 MinPeciOverride : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PECI_HWP_REQUEST_INFO_REGISTER;

/**
  Indicates when a Change or Excursion has occurred :
    BDX new: Thread HWP Status register.  All values in this register are updated by Pcode and typically read or cleared by SW (ucode).
**/
#define SNR_MSR_CORE_INTERFACE_CR_HWP_STATUS 0x00000777

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW0C
    UINT32 ChangeToGuaranteed : 1;
    //Bits[1:1] Access Type=RO
    UINT32 ReservedForExcursionToDesired : 1;
    //Bits[2:2] Access Type=RW0C
    UINT32 ExcursionToMinimum : 1;
    //Bits[3:3] Access Type=RW0C
    UINT32 HighestChange : 1;
    //Bits[4:4] Access Type=RW0C
    UINT32 PeciOverrideEnter : 1;
    //Bits[5:5] Access Type=RW0C
    UINT32 PeciOverrideExit : 1;
    //Bits[37:6] Access Type=RO
    UINT32 ReservedLow0 : 32;
    //Bits[63:38] Access Type=RO
    UINT32 Reserved : 26;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_HWP_STATUS_REGISTER;

/**
  This MSR will write the ERRINJLCK bit of IIO, QPI, CBO, MC
**/
#define SNR_MSR_VIRT_CR_ERR_INJ_LCK_UNLOCK_CTL 0x00000790

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 ErrInjLckUnlock : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_ERR_INJ_LCK_UNLOCK_CTL_REGISTER;

/**
  PIC Logical APIC ID register
**/
#define SNR_MSR_PIC_CR_PIC_EXTENDED_LOCAL_APIC_ID 0x00000802

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 ApicId : 8;
    //Bits[19:8] Access Type=RW
    UINT32 ExtendedApicId : 12;
    //Bits[31:20] Access Type=Rsvd0
    UINT32 Reserved : 12;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_EXTENDED_LOCAL_APIC_ID_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_TASK_PRIORITY 0x00000808

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RW
    UINT32 PrioritySubClass : 4;
    //Bits[7:4] Access Type=RW
    UINT32 Foo : 4;
    //Bits[31:8] Access Type=Rsvd0
    UINT32 Reserved : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_TASK_PRIORITY_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_PROCESSOR_PRIORITY 0x0000080A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[3:0] Access Type=RO
    UINT32 PrioritySubClass : 4;
    //Bits[7:4] Access Type=RO
    UINT32 Foo : 4;
    //Bits[31:8] Access Type=Rsvd0
    UINT32 Reserved : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_PROCESSOR_PRIORITY_REGISTER;
/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_EXTENDED_LOGICAL_DESTINATION 0x0000080D

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_ISR01 0x00000810

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=Rsvd0
    UINT32 Isr0Reserved : 16;
    //Bits[31:16] Access Type=RW
    UINT32 Isr0 : 16;
    //Bits[63:32] Access Type=RW
    UINT32 Isr1 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_ISR01_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_ISR23 0x00000812

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Isr2 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Isr3 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_ISR23_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_ISR45 0x00000814

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Isr4 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Isr5 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_ISR45_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_ISR67 0x00000816

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Isr6 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Isr7 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_ISR67_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_TMR01 0x00000818

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=Rsvd0
    UINT32 Tmr0Reserved : 16;
    //Bits[31:16] Access Type=RW
    UINT32 Tmr0 : 16;
    //Bits[63:32] Access Type=RW
    UINT32 Tmr1 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_TMR01_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_TMR23 0x0000081A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Tmr2 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Tmr3 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_TMR23_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_TMR45 0x0000081C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Tmr4 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Tmr5 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_TMR45_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_TMR67 0x0000081E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Tmr6 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Tmr7 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_TMR67_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_IRR01 0x00000820

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=Rsvd0
    UINT32 Irr0Reserved : 16;
    //Bits[31:16] Access Type=RW
    UINT32 Irr0 : 16;
    //Bits[63:32] Access Type=RW
    UINT32 Irr1 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_IRR01_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_IRR23 0x00000822

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Irr2 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Irr3 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_IRR23_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_IRR45 0x00000824

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Irr4 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Irr5 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_IRR45_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_IRR67 0x00000826

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Irr6 : 32;
    //Bits[63:32] Access Type=RW
    UINT32 Irr7 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_IRR67_REGISTER;

/**
  SKL only defines bits 7:4, and GP faults on all non-zero writes.
**/
#define SNR_MSR_PIC_CR_PIC_ERROR_STATUS 0x00000828

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=Rsvd0
    UINT32 SendChecksumError : 1;
    //Bits[1:1] Access Type=Rsvd0
    UINT32 ReceiveChecksumError : 1;
    //Bits[2:2] Access Type=Rsvd0
    UINT32 SendAcceptError : 1;
    //Bits[3:3] Access Type=Rsvd0
    UINT32 ReceiveAcceptError : 1;
    //Bits[4:4] Access Type=Rsvd0
    UINT32 RedirectableIpi : 1;
    //Bits[5:5] Access Type=RW
    UINT32 SendIllegalVector : 1;
    //Bits[6:6] Access Type=RW
    UINT32 ReceiveIllegalVector : 1;
    //Bits[7:7] Access Type=RW
    UINT32 IllegalRegisterAddress : 1;
    //Bits[31:8] Access Type=Rsvd0
    UINT32 Reserved : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_ERROR_STATUS_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_LVT_CMCI 0x0000082F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=RW
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=Rsvd0
    UINT32 Polarity : 1;
    //Bits[14:14] Access Type=Rsvd0
    UINT32 RemoteIrr : 1;
    //Bits[15:15] Access Type=Rsvd0
    UINT32 TriggerMode : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Mask : 1;
    //Bits[18:17] Access Type=Rsvd0
    UINT32 TimerMode : 2;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_LVT_CMCI_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_INTERRUPT_COMMAND 0x00000830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=RW
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=RW
    UINT32 DestinationMode : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=Rsvd0
    UINT32 Reserved1 : 1;
    //Bits[14:14] Access Type=Rsvd0
    UINT32 Level : 1;
    //Bits[15:15] Access Type=Rsvd0
    UINT32 TriggerMode : 1;
    //Bits[17:16] Access Type=Rsvd0
    UINT32 Reserved2 : 2;
    //Bits[19:18] Access Type=RW
    UINT32 DestinationShorthand : 2;
    //Bits[31:20] Access Type=Rsvd0
    UINT32 Reserved3 : 12;
    //Bits[63:32] Access Type=RW
    UINT32 DestinationField : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_INTERRUPT_COMMAND_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_LVT_TIMER 0x00000832

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=Rsvd0
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=Rsvd0
    UINT32 Polarity : 1;
    //Bits[14:14] Access Type=Rsvd0
    UINT32 RemoteIrr : 1;
    //Bits[15:15] Access Type=Rsvd0
    UINT32 TriggerMode : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Mask : 1;
    //Bits[18:17] Access Type=RW
    UINT32 TimerMode : 2;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_LVT_TIMER_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_LVT_THERM 0x00000833

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=RW
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=Rsvd0
    UINT32 Polarity : 1;
    //Bits[14:14] Access Type=Rsvd0
    UINT32 RemoteIrr : 1;
    //Bits[15:15] Access Type=Rsvd0
    UINT32 TriggerMode : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Mask : 1;
    //Bits[18:17] Access Type=Rsvd0
    UINT32 TimerMode : 2;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_LVT_THERM_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_LVT_PERF 0x00000834

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=RW
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=Rsvd0
    UINT32 Polarity : 1;
    //Bits[14:14] Access Type=Rsvd0
    UINT32 RemoteIrr : 1;
    //Bits[15:15] Access Type=Rsvd0
    UINT32 TriggerMode : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Mask : 1;
    //Bits[18:17] Access Type=Rsvd0
    UINT32 TimerMode : 2;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_LVT_PERF_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_LVT_LINT0 0x00000835

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=RW
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=RW
    UINT32 Polarity : 1;
    //Bits[14:14] Access Type=RW
    UINT32 RemoteIrr : 1;
    //Bits[15:15] Access Type=RW
    UINT32 TriggerMode : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Mask : 1;
    //Bits[18:17] Access Type=Rsvd0
    UINT32 TimerMode : 2;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_LVT_LINT0_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_LVT_LINT1 0x00000836

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=RW
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=RW
    UINT32 Polarity : 1;
    //Bits[14:14] Access Type=RW
    UINT32 RemoteIrr : 1;
    //Bits[15:15] Access Type=RW
    UINT32 TriggerMode : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Mask : 1;
    //Bits[18:17] Access Type=Rsvd0
    UINT32 TimerMode : 2;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_LVT_LINT1_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_LVT_ERROR 0x00000837

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[10:8] Access Type=Rsvd0
    UINT32 DeliveryMode : 3;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 DeliveryStatus : 1;
    //Bits[13:13] Access Type=Rsvd0
    UINT32 Polarity : 1;
    //Bits[14:14] Access Type=Rsvd0
    UINT32 RemoteIrr : 1;
    //Bits[15:15] Access Type=Rsvd0
    UINT32 TriggerMode : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Mask : 1;
    //Bits[18:17] Access Type=Rsvd0
    UINT32 TimerMode : 2;
    //Bits[31:19] Access Type=Rsvd0
    UINT32 Reserved : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_LVT_ERROR_REGISTER;

/**
  This corresponds to the value in the Divide Config Register in the APIC.
**/
#define SNR_MSR_PIC_CR_PIC_TIMER_DIVIDE_CONFIG_REG 0x0000083E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[1:0] Access Type=RW
    UINT32 CfgcntLo : 2;
    //Bits[2:2] Access Type=Rsvd0
    UINT32 ReservedLo : 1;
    //Bits[3:3] Access Type=RW
    UINT32 CfgcntHi : 1;
    //Bits[31:4] Access Type=Rsvd0
    UINT32 ReservedHi : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_TIMER_DIVIDE_CONFIG_REG_REGISTER;

/**
  fill description here
**/
#define SNR_MSR_PIC_CR_PIC_SELF_IPI 0x0000083F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Vector : 8;
    //Bits[8:8] Access Type=RW
    UINT32 SelfIpi : 1;
    //Bits[9:9] Access Type=RW
    UINT32 Spare0 : 1;
    //Bits[10:10] Access Type=RW
    UINT32 Spare1 : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Spare2 : 1;
    //Bits[12:12] Access Type=RW
    UINT32 DeliveryStatus : 1;
    //Bits[31:13] Access Type=Rsvd0
    UINT32 Reserved : 19;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_PIC_SELF_IPI_REGISTER;
/**
  This is the template to virtualize RDMSR access to all x2apic MSRs not defined in BUS2
**/
#define SNR_MSR_UCODE_CR_VIRTUAL_APIC 0x000008FF

/**
  Holds 2 XSAVE values:The X_FEATURE_ENABLED_MASK (see XSAVE EAS for details) and XSAVE_AREA triplet for the XMODIFIED optimization.
**/
#define SNR_MSR_SCP_CR_XFEM_AND_XRSTOR_INFO 0x00000900

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[1:0] Access Type=RW
    UINT32 XsaveAreaCpl : 2;
    //Bits[2:2] Access Type=RW
    UINT32 XsaveAreaVmmHostGuest : 1;
    //Bits[4:3] Access Type=RW
    UINT32 Unused43 : 2;
    //Bits[5:5] Access Type=RW
    UINT32 XsaveAreaValid : 1;
    //Bits[37:6] Access Type=RW
    UINT32 XsaveAreaAddressLow0 : 32;
    //Bits[55:38] Access Type=RW
    UINT32 XsaveAreaAddress : 18;
    //Bits[63:56] Access Type=RW
    UINT32 XFeatureEnabledMask : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_XFEM_AND_XRSTOR_INFO_REGISTER;

/**
  Register which contains data breakpoint state for pending data breakpoints. It is cleared by the ROB when a ClearTMPDR6 flow marker is retired. It is writ- ten by the FPEH micro-code to correctly setup the match bits when an FLD m64/32real macroinstruction that enters it is also the target of a data breakpoint. The Exact Match micro-code copies this register to DR6. micro-code must ensure that this register is not written at the same time as a microinstruction which contains a ClearTMPDR6 or EvalTFBit flow marker is retiring. micro-code must also ensure that this register is not written at the same time as a microinstruction capable of hitting a data breakpoint is retiring
**/
#define SNR_MSR_ARR_CR_TMPDR6 0x00000902

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 B0 : 1;
    //Bits[1:1] Access Type=RW
    UINT32 B1 : 1;
    //Bits[2:2] Access Type=RW
    UINT32 B2 : 1;
    //Bits[3:3] Access Type=RW
    UINT32 B3 : 1;
    //Bits[12:4] Access Type=Rsvd0
    UINT32 Reserved : 9;
    //Bits[13:13] Access Type=RW
    UINT32 Bd : 1;
    //Bits[14:14] Access Type=RW
    UINT32 Bs : 1;
    //Bits[15:15] Access Type=RW
    UINT32 Bt : 1;
    //Bits[31:16] Access Type=Rsvd0
    UINT32 Reserved1 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_TMPDR6_REGISTER;

/**
  - see SCP_CR_XU_MACROINSTRUCTION_ALIAS for SCP format
**/
#define SNR_MSR_XU_CR_MACROINSTRUCTION_ALIAS 0x00000903

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[1:0] Access Type=RW
    UINT32 Scale : 2;
    //Bits[6:2] Access Type=RW
    UINT32 Reserved62 : 5;
    //Bits[8:7] Access Type=RW
    UINT32 Asize : 2;
    //Bits[10:9] Access Type=RW
    UINT32 Reserved109 : 2;
    //Bits[12:11] Access Type=RW
    UINT32 Osize : 2;
    //Bits[14:13] Access Type=RW
    UINT32 Reserved1413 : 2;
    //Bits[17:15] Access Type=RW
    UINT32 Segment : 3;
    //Bits[31:18] Access Type=RW
    UINT32 Reserved3118 : 14;
    //Bits[63:32] Access Type=RW
    UINT32 Reserved6332 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_XU_CR_MACROINSTRUCTION_ALIAS_REGISTER;

/**
  This MSR provides PPPE access to the POST_VMXON bit. No reserved bit checks currently to match SKL.
**/
#define SNR_MSR_VIRT_CR_VMX_CONTROL 0x00000904

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=Rsvd0
    UINT32 Reserved0 : 1;
    //Bits[1:1] Access Type=RW
    UINT32 PostVmxon : 1;
    //Bits[33:2] Access Type=Rsvd0
    UINT32 ReservedLow0 : 32;
    //Bits[63:34] Access Type=Rsvd0
    UINT32 Reserved : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_VMX_CONTROL_REGISTER;

/**
  XuCode MSR used to control SMM states. No reserved bit checks currently to match SKL.
**/
#define SNR_MSR_VIRT_CR_SMM_STATE 0x00000908

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 InSmm : 1;
    //Bits[7:1] Access Type=RO
    UINT32 Rsvd1 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_SMM_STATE_REGISTER;

/**
  register
**/
#define SNR_MSR_CORE_INTERFACE_CR_GT_RELOAD_FLUSH 0x0000090E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 BcldReq : 1;
    //Bits[31:1] Access Type=RW
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_GT_RELOAD_FLUSH_REGISTER;
/**
  register
**/
#define SNR_MSR_CORE_INTERFACE_CR_GT_FLUSH_BCLD_ACK 0x0000090F

/**
  GLP e1404572178: Read only XuMSR
**/
#define SNR_MSR_UCODE_CR_XU_STATUS 0x00000910

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 C6DramEnabled : 1;
    //Bits[24:1] Access Type=RO
    UINT32 Rsvd1 : 24;
    //Bits[25:25] Access Type=RO
    UINT32 EpcOvDis : 1;
    //Bits[30:26] Access Type=RO
    UINT32 Rsvd26 : 5;
    //Bits[31:31] Access Type=RO
    UINT32 EdmmDisabled : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_XU_STATUS_REGISTER;

/**
  The Core version of the UnCore WhoAmI register. GLM format currently matches the uncore register exactly, so h32 is spare. This is different from SKL. If this changes on due to multi-package support, we would need to add back the reset ucode change to modify core_id, and change the MSR to be mapped to the funnyIO register like SKL.
**/
#define SNR_MSR_SCP_CR_WHO_AM_I_INFO 0x00000920

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Tid : 1;
    //Bits[2:1] Access Type=RW
    UINT32 CoreId : 2;
    //Bits[6:3] Access Type=RW
    UINT32 ModuleId : 4;
    //Bits[15:7] Access Type=RW
    UINT32 PackageId : 9;
    //Bits[27:16] Access Type=RW
    UINT32 Reserved2716 : 12;
    //Bits[30:28] Access Type=RW
    UINT32 CoreNum : 3;
    //Bits[31:31] Access Type=RW
    UINT32 Bsp : 1;
    //Bits[38:32] Access Type=RW
    UINT32 ResolvedCoreCount : 7;
    //Bits[45:39] Access Type=RW
    UINT32 FusedCoreCount : 7;
    //Bits[63:46] Access Type=RW
    UINT32 Reserved6346 : 18;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_WHO_AM_I_INFO_REGISTER;

/**
  Write only MSR for Xucode to send the PDM header before sending out the actual message(via WRMSR to PDM_MSG)
**/
#define SNR_MSR_UCODE_CR_PDM_HEADER 0x00000922

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=WO
    UINT32 PacketType : 8;
    //Bits[23:8] Access Type=WO
    UINT32 PacketLength : 16;
    //Bits[31:24] Access Type=RO
    UINT32 Rsvd24 : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UCODE_CR_PDM_HEADER_REGISTER;

/**
  Doorbell bitmap
**/
#define SNR_MSR_XU_CR_PPPE_DOORBELL_EXIT_BITMAP 0x00000924

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW
    UINT32 ExitBitmap : 7;
    //Bits[31:7] Access Type=RW
    UINT32 Reserved : 25;
    //Bits[63:32] Access Type=RW
    UINT32 Reserved6332 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_XU_CR_PPPE_DOORBELL_EXIT_BITMAP_REGISTER;

/**
  This is a virtual register to combine the PPPE-accessible    events into a single XuMSR. The bits are sourced from    BUS_CR_PENDING_* and ARR_CR_VMCTL
**/
#define SNR_MSR_VIRT_CR_PPPE_EVENT_STATUS 0x00000925

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Intr : 1;
    //Bits[1:1] Access Type=RO
    UINT32 Rsvd1 : 1;
    //Bits[2:2] Access Type=RO
    UINT32 VmxTimer : 1;
    //Bits[3:3] Access Type=RO
    UINT32 Rsvd3 : 1;
    //Bits[4:4] Access Type=RO
    UINT32 Monitor : 1;
    //Bits[8:5] Access Type=RO
    UINT32 Rsvd5 : 4;
    //Bits[9:9] Access Type=RO
    UINT32 Smi : 1;
    //Bits[10:10] Access Type=RO
    UINT32 Nmi : 1;
    //Bits[11:11] Access Type=RO
    UINT32 Rsvd11 : 1;
    //Bits[12:12] Access Type=RO
    UINT32 Init : 1;
    //Bits[13:13] Access Type=RO
    UINT32 Sipi : 1;
    //Bits[14:14] Access Type=RO
    UINT32 Rsvd14 : 1;
    //Bits[15:15] Access Type=RO
    UINT32 Kindmc : 1;
    //Bits[33:16] Access Type=RO
    UINT32 Rsvd16 : 18;
    //Bits[34:34] Access Type=RO
    UINT32 Nmip : 1;
    //Bits[63:35] Access Type=RO
    UINT32 Rsvd35 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_PPPE_EVENT_STATUS_REGISTER;

/**
  This is a virtual register to combine the PPPE-accessible    events into a single XuMSR. The bits are sourced from    BUS_CR_PENDING_* and ARR_CR_VMCTL
**/
#define SNR_MSR_VIRT_CR_PPPE_EVENT_SET 0x00000926

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[1:0] Access Type=RO
    UINT32 Rsvd0 : 2;
    //Bits[2:2] Access Type=WO
    UINT32 VmxTimer : 1;
    //Bits[34:3] Access Type=RO
    UINT32 Rsvd3Low0 : 32;
    //Bits[39:35] Access Type=RO
    UINT32 Rsvd3 : 5;
    //Bits[40:40] Access Type=WO
    UINT32 Mtfatbom : 1;
    //Bits[63:41] Access Type=RO
    UINT32 Rsvd41 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_PPPE_EVENT_SET_REGISTER;

/**
  This is a virtual register to combine the PPPE-accessible events into a single XuMSR. The bits are sourced from BUS_CR_PENDING_
**/
#define SNR_MSR_VIRT_CR_PPPE_EVENT_RESET 0x00000927

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[1:0] Access Type=RO
    UINT32 Rsvd0 : 2;
    //Bits[2:2] Access Type=WO
    UINT32 VmxTimer : 1;
    //Bits[3:3] Access Type=RO
    UINT32 Rsvd3 : 1;
    //Bits[4:4] Access Type=WO
    UINT32 Monitor : 1;
    //Bits[9:5] Access Type=RO
    UINT32 Rsvd5 : 5;
    //Bits[10:10] Access Type=WO
    UINT32 Nmi : 1;
    //Bits[15:11] Access Type=RO
    UINT32 Rsvd11 : 5;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_PPPE_EVENT_RESET_REGISTER;

/**
  Register which contains the current PSMI base value for the processor. The RESET event handler should set it to 0x00030000, which is the default value. When RSM executes, it reads the PSMI base slot value in PSMI memory and loads it into this register. The INIT event handler should not change the value of this register.
**/
#define SNR_MSR_SCP_CR_PSMBASE 0x00000934

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Data : 32;
    //Bits[63:32] Access Type=RW
    UINT32 PatchId : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_PSMBASE_REGISTER;

/**
  Used for SMX state
**/
#define SNR_MSR_VIRT_CR_LT_SMX_STATE 0x00000936

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Rsvd0 : 1;
    //Bits[1:1] Access Type=RW
    UINT32 SenterFlag : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_LT_SMX_STATE_REGISTER;

/**
  Stores the VMX Timer Count.
**/
#define SNR_MSR_PIC_CR_VMX_TIMER_CTRL 0x00000938

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 VmxTimerEnable : 1;
    //Bits[1:1] Access Type=RW
    UINT32 VmxTimerFreeze : 1;
    //Bits[31:2] Access Type=Rsvd0
    UINT32 Reserved : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PIC_CR_VMX_TIMER_CTRL_REGISTER;

/**
  Used by micro-code to hold IO instr initial linear memory address to be saved to SMRAM in synchronous SMI. Macro address changed from 0x4F4 on SLT to match PPPE/HSW.
**/
#define SNR_MSR_SCP_CR_IO_MISC_INFO 0x00000939

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 IoSmi : 1;
    //Bits[3:1] Access Type=RW
    UINT32 Length : 3;
    //Bits[7:4] Access Type=RW
    UINT32 Type : 4;
    //Bits[15:8] Access Type=RW
    UINT32 Reserved : 8;
    //Bits[31:16] Access Type=RW
    UINT32 PortAddress : 16;
    //Bits[63:32] Access Type=RW
    UINT32 FlowDetails : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_IO_MISC_INFO_REGISTER;

/**
  Registers containing the page directory pointers 0 used by the PMH to support Mode C paging.
**/
#define SNR_MSR_AGU_CR_PDPTR0 0x00000940

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Present : 1;
    //Bits[2:1] Access Type=Rsvd0
    UINT32 Reserved2 : 2;
    //Bits[3:3] Access Type=RW
    UINT32 Pwt : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Pcd : 1;
    //Bits[11:5] Access Type=Rsvd0
    UINT32 Reserved1 : 7;
    //Bits[41:12] Access Type=RW
    UINT32 PdeBaseAddr : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserved : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_AGU_CR_PDPTR0_REGISTER;

/**
  Registers containing the page directory pointers 1 used by the PMH to support Mode C paging.
**/
#define SNR_MSR_AGU_CR_PDPTR1 0x00000941

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Present : 1;
    //Bits[2:1] Access Type=Rsvd0
    UINT32 Reserved2 : 2;
    //Bits[3:3] Access Type=RW
    UINT32 Pwt : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Pcd : 1;
    //Bits[11:5] Access Type=Rsvd0
    UINT32 Reserved1 : 7;
    //Bits[41:12] Access Type=RW
    UINT32 PdeBaseAddr : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserved : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_AGU_CR_PDPTR1_REGISTER;

/**
  Registers containing the page directory pointers 2used by the PMH to support Mode C paging.
**/
#define SNR_MSR_AGU_CR_PDPTR2 0x00000942

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Present : 1;
    //Bits[2:1] Access Type=Rsvd0
    UINT32 Reserved2 : 2;
    //Bits[3:3] Access Type=RW
    UINT32 Pwt : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Pcd : 1;
    //Bits[11:5] Access Type=Rsvd0
    UINT32 Reserved1 : 7;
    //Bits[41:12] Access Type=RW
    UINT32 PdeBaseAddr : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserved : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_AGU_CR_PDPTR2_REGISTER;

/**
  Registers containing the page directory pointers 3 used by the PMH to support Mode C paging.
**/
#define SNR_MSR_AGU_CR_PDPTR3 0x00000943

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Present : 1;
    //Bits[2:1] Access Type=Rsvd0
    UINT32 Reserved2 : 2;
    //Bits[3:3] Access Type=RW
    UINT32 Pwt : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Pcd : 1;
    //Bits[11:5] Access Type=Rsvd0
    UINT32 Reserved1 : 7;
    //Bits[41:12] Access Type=RW
    UINT32 PdeBaseAddr : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserved : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_AGU_CR_PDPTR3_REGISTER;

/**
  uCode scratchpad register shared by all threads.
**/
#define SNR_MSR_FUNNYIO_CR_SCP_XU_ALL_CORES_POISON 0x0000094E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 PoisonEn : 1;
    //Bits[1:1] Access Type=RW
    UINT32 ViralEn : 1;
    //Bits[2:2] Access Type=RW
    UINT32 RasSgxOptin : 1;
    //Bits[7:3] Access Type=RO
    UINT32 Rsvd3 : 5;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_SCP_XU_ALL_CORES_POISON_REGISTER;

/**
  Each bit is command from xucode for specific action. Not really a register.
**/
#define SNR_MSR_XU_CR_XUCODE_UCODE_COMMAND 0x00000950

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 EnableEmxrr : 1;
    //Bits[1:1] Access Type=RW
    UINT32 UpdateCurrentSeSvn : 1;
    //Bits[2:2] Access Type=RW
    UINT32 LockSeSvn : 1;
    //Bits[3:3] Access Type=WO
    UINT32 SignalMckindEvent : 1;
    //Bits[4:4] Access Type=WO
    UINT32 DisablePdrCopy : 1;
    //Bits[7:5] Access Type=RO
    UINT32 Rsvd5 : 3;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_XU_CR_XUCODE_UCODE_COMMAND_REGISTER;

/**
  SE Enable and Global Disable/Lock
**/
#define SNR_MSR_VIRT_CR_SE_ENABLE 0x00000951

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 SeEnable : 1;
    //Bits[1:1] Access Type=RW
    UINT32 SeGlobalDisableLock : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_SE_ENABLE_REGISTER;

/**
  uCode scratchpad register shared by all threads.
**/
#define SNR_MSR_FUNNYIO_CR_PKG_SE_SVN 0x00000952

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 PrResetSvn : 8;
    //Bits[15:8] Access Type=RW
    UINT32 RLastPatchSvn : 8;
    //Bits[23:16] Access Type=RW
    UINT32 GetsecSenterAcmSvn : 8;
    //Bits[31:24] Access Type=RW
    UINT32 GetsecScleanAcmSvn : 8;
    //Bits[39:32] Access Type=RW
    UINT32 AncAcmSvn : 8;
    //Bits[47:40] Access Type=RW
    UINT32 PfatSvn : 8;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_PKG_SE_SVN_REGISTER;

/**
  PPPE_MEMTYPE
**/
#define SNR_MSR_VIRT_CR_PPPE_MEMTYPE 0x00000953

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Enable : 1;
    //Bits[3:1] Access Type=RW
    UINT32 Memtype : 3;
    //Bits[7:4] Access Type=RO
    UINT32 Rsvd4 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_PPPE_MEMTYPE_REGISTER;

/**
  Enclave Linear Address Region - Linear Base
**/
#define SNR_MSR_PMH_CR_ELSRR_BASE 0x00000955

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[11:0] Access Type=Rsvd0
    UINT32 Reserved110 : 12;
    //Bits[43:12] Access Type=RW
    UINT32 BaseLow0 : 32;
    //Bits[47:44] Access Type=RW
    UINT32 Base : 4;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserve6348 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_ELSRR_BASE_REGISTER;

/**
  Enclave Linear Address Region - Linear Mask
**/
#define SNR_MSR_PMH_CR_ELSRR_MASK 0x00000956

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[11:0] Access Type=Rsvd0
    UINT32 Reserved110 : 12;
    //Bits[43:12] Access Type=RW
    UINT32 MaskLow0 : 32;
    //Bits[47:44] Access Type=RW
    UINT32 Mask : 4;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserve6348 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_ELSRR_MASK_REGISTER;

/**
  SE TCS
**/
#define SNR_MSR_SCP_CR_TCS_PA 0x00000958

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 TcsPhysAddrLow0 : 32;
    //Bits[41:32] Access Type=RW
    UINT32 TcsPhysAddr : 10;
    //Bits[63:42] Access Type=RW
    UINT32 Reserved6342Mbz : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_TCS_PA_REGISTER;

/**
  SE State bits
**/
#define SNR_MSR_GLM_CR_SE 0x00000959

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 EnclaveMode : 1;
    //Bits[11:1] Access Type=RO
    UINT32 Rsvd1 : 11;
    //Bits[41:12] Access Type=RW
    UINT32 SecsSid : 30;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_GLM_CR_SE_REGISTER;

/**
  Perfmon Controls Set / Status
**/
#define SNR_MSR_PM_CR_CONTROLS_SET_AND_STATUS 0x0000095E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 SeSuppressCtrs : 1;
    //Bits[1:1] Access Type=RW
    UINT32 SeSuppressRtit : 1;
    //Bits[31:2] Access Type=Rsvd0
    UINT32 Reserved312 : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PM_CR_CONTROLS_SET_AND_STATUS_REGISTER;

/**
  Perfmon Controls Reset
**/
#define SNR_MSR_PM_CR_CONTROLS_RESET 0x0000095F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 SeSuppressCtrs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 SeSuppressRtit : 1;
    //Bits[31:2] Access Type=Rsvd0
    UINT32 Reserved312 : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PM_CR_CONTROLS_RESET_REGISTER;

/**
  Extended Microcode Range Register - Physical Base
**/
#define SNR_MSR_PMH_CR_EMRR_BASE 0x00000962

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[2:0] Access Type=RW
    UINT32 Memtype : 3;
    //Bits[11:3] Access Type=Rsvd0
    UINT32 Reserved113 : 9;
    //Bits[41:12] Access Type=RW
    UINT32 Base : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserve6340 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_EMRR_BASE_REGISTER;

/**
  Extended Microcode Range Register - Physical Mask
**/
#define SNR_MSR_PMH_CR_EMRR_MASK 0x00000963

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=Rsvd0
    UINT32 Reserved90 : 10;
    //Bits[10:10] Access Type=RW
    UINT32 L : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Vld : 1;
    //Bits[41:12] Access Type=RW
    UINT32 Mask : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserve6340 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_EMRR_MASK_REGISTER;

/**
  Extended Microcode Executable Range Register - Physical Base
**/
#define SNR_MSR_PMH_CR_EMXRR_BASE 0x00000964

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[11:0] Access Type=Rsvd0
    UINT32 Reserved110 : 12;
    //Bits[41:12] Access Type=RW
    UINT32 Base : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserve6340 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_EMXRR_BASE_REGISTER;

/**
  Extended Microcode Executable Range Register - Physical Mask
**/
#define SNR_MSR_PMH_CR_EMXRR_MASK 0x00000965

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[9:0] Access Type=Rsvd0
    UINT32 Reserved90 : 10;
    //Bits[10:10] Access Type=RW
    UINT32 L : 1;
    //Bits[11:11] Access Type=Rsvd0
    UINT32 Reserved11 : 1;
    //Bits[41:12] Access Type=RW
    UINT32 Mask : 30;
    //Bits[63:42] Access Type=Rsvd0
    UINT32 Reserve6340 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PMH_CR_EMXRR_MASK_REGISTER;
/**
  MEE Setup (write only)
**/
#define SNR_MSR_XU_CR_MEE_SETUP 0x0000096A

/**
  Tap Status - XuMSR - read-only
**/
#define SNR_MSR_VIRT_CR_PPPE_TAP_STATUS 0x0000096B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 DebugUnlockMode : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_PPPE_TAP_STATUS_REGISTER;

/**
  MEE Init Status (read only)
**/
#define SNR_MSR_XU_CR_MEE_INIT 0x0000096C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Lock : 1;
    //Bits[1:1] Access Type=RW
    UINT32 PoisonEn : 1;
    //Bits[2:2] Access Type=RW
    UINT32 ViralEn : 1;
    //Bits[31:3] Access Type=RO
    UINT32 Rsvd3 : 29;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_XU_CR_MEE_INIT_REGISTER;

/**
  This register contains the current status of the DRBG. This can be read by the identified agents. The HW also updates this register as it progresses.[br]
  [br] Restrictions: SAI access restricted
**/
#define SNR_MSR_DRNG_CR_STATUS 0x0000096D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 DrngCompletedBist : 1;
    //Bits[1:1] Access Type=RO
    UINT32 DrngPassedBist : 1;
    //Bits[2:2] Access Type=RO
    UINT32 EsPassedBist : 1;
    //Bits[3:3] Access Type=RO
    UINT32 KatPassedBist : 1;
    //Bits[7:4] Access Type=RO
    UINT32 DrbgOperationModeOut : 4;
    //Bits[8:8] Access Type=RO
    UINT32 DrbgFipsOut : 1;
    //Bits[24:9] Access Type=RO
    UINT32 Rsvd9 : 16;
    //Bits[25:25] Access Type=RO
    UINT32 DrbgPowerdownRdy : 1;
    //Bits[26:26] Access Type=RO
    UINT32 AonValid : 1;
    //Bits[31:27] Access Type=RO
    UINT32 Rsvd27 : 5;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_DRNG_CR_STATUS_REGISTER;
/**
  PCH ID 0 used by MCHECK for server attestation platform binding
**/
#define SNR_MSR_LT_CR_PCHSGXID_0 0x00000970
/**
  PCH ID 1 used by MCHECK for server attestation platform binding
**/
#define SNR_MSR_LT_CR_PCHSGXID_1 0x00000971
/**
  NPP Whitelist CPU Register Address
**/
#define SNR_MSR_NPP_CR_CPU_REGISTER_ADDR 0x00000972
/**
  NPP Whitelist CPU Register Data
**/
#define SNR_MSR_NPP_CR_CPU_REGISTER_DATA 0x00000973

/**
  Read only register for encryption policies supported by TME
**/
#define SNR_MSR_VIRT_CR_TME_CAPABILITY 0x00000981

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 AesXts : 1;
    //Bits[32:1] Access Type=RO
    UINT32 Rsvd1Low0 : 32;
    //Bits[63:33] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_TME_CAPABILITY_REGISTER;

/**
  Rd/Wr register to generate or restore Keys for TME
**/
#define SNR_MSR_VIRT_CR_TME_ACTIVATE 0x00000982

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RO
    UINT32 Lock : 1;
    //Bits[1:1] Access Type=RW
    UINT32 TmeEnable : 1;
    //Bits[2:2] Access Type=RO
    UINT32 KeySelect : 1;
    //Bits[3:3] Access Type=RO
    UINT32 SaveKeyForStandby : 1;
    //Bits[7:4] Access Type=Rsvd0
    UINT32 TmePolicy : 4;
    //Bits[39:8] Access Type=RO
    UINT32 Rsvd8Low0 : 32;
    //Bits[63:40] Access Type=RO
    UINT32 Rsvd8 : 24;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_TME_ACTIVATE_REGISTER;

/**
  Register for debug purposes to exclude a memory range from encryption by TME
**/
#define SNR_MSR_VIRT_CR_TME_EXCLUDE_MASK 0x00000983

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[10:0] Access Type=RO
    UINT32 Rsvd0 : 11;
    //Bits[11:11] Access Type=RW
    UINT32 Enable : 1;
    //Bits[41:12] Access Type=RW
    UINT32 Tmemask : 30;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_TME_EXCLUDE_MASK_REGISTER;

/**
  Register for debug purposes to exclude a memory range from encryption by TME
**/
#define SNR_MSR_VIRT_CR_TME_EXCLUDE_BASE 0x00000984

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[11:0] Access Type=RO
    UINT32 Rsvd0 : 12;
    //Bits[41:12] Access Type=RW
    UINT32 Tmebase : 30;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_TME_EXCLUDE_BASE_REGISTER;
/**
  eSMM C6 save area location (C6 core-area byte offset 0x2C8)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__CR0 0x00000C00
/**
  eSMM C6 save area location (C6 core-area byte offset 0x2C0)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__CR3 0x00000C01
/**
  eSMM C6 save area location (C6 core-area byte offset 0x2A0)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__DR6 0x00000C05
/**
  eSMM C6 save area location (C6 core-area byte offset 0x298)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__DR7 0x00000C06

/**
  eSMM C6 save area location (C6 core-area byte offset 0x290)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_TR_LDTR_SEL 0x00000C07

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LdtrValue : 32;
    //Bits[63:32] Access Type=RW
    UINT32 TrValue : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_TR_LDTR_SEL_REGISTER;

/**
  eSMM C6 save area location (C6 core-area byte offset 0x288)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_GS_FS_SEL 0x00000C08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 FsValue : 32;
    //Bits[63:32] Access Type=RW
    UINT32 GsValue : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_GS_FS_SEL_REGISTER;

/**
  eSMM C6 save area location (C6 core-area byte offset 0x280)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_DS_SS_SEL 0x00000C09

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 SsValue : 32;
    //Bits[63:32] Access Type=RW
    UINT32 DsValue : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_DS_SS_SEL_REGISTER;

/**
  eSMM C6 save area location (C6 core-area byte offset 0x278)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_CS_ES_SEL 0x00000C0A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 EsValue : 32;
    //Bits[63:32] Access Type=RW
    UINT32 CsValue : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_CS_ES_SEL_REGISTER;

/**
  eSMM C6 save area location (alias to C6 core-area byte offset 0x1C8). This is a read-only msr: the uip0 rdmsr_init leaf is used for GP checks only,
                   and the constant leaf is used to return the actual revid value (see SMREVID msr legacy def which this mirrors).
**/
#define SNR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_SMBASE_SMREVID__SMMREVID_MSR 0x00000C21

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RO
    UINT32 SmmRevisionIdentifier : 16;
    //Bits[16:16] Access Type=RO
    UINT32 IoInstructionRestart : 1;
    //Bits[17:17] Access Type=RO
    UINT32 SmbaseRelocation : 1;
    //Bits[49:18] Access Type=RO
    UINT32 Rsvd18Low0 : 32;
    //Bits[63:50] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_SMBASE_SMREVID__SMMREVID_MSR_REGISTER;

/**
  eSMM C6 save area location (C6 core-area byte offset 0x1B0)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_MODE_BITS_CS_BASE 0x00000C23

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 ModeBitsValue : 32;
    //Bits[63:32] Access Type=RW
    UINT32 CsBaseValue : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__FUSED_MODE_BITS_CS_BASE_REGISTER;

/**
  eSMM C6 save area location (C6 core-area byte offset 0x110)
**/
#define SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__CR4 0x00000C37

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Cr4Value : 32;
    //Bits[63:32] Access Type=RO
    UINT32 Rsvd32 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_ESMM_C6_SAVE_AREA__CR4_REGISTER;

/**
  This register is virtual in nature for the Cunit meaning it doesnt exist in Cunit but need this in an XLS format to create the ucode collateral and defines
**/
#define SNR_MSR_DFXREGISTERS_MSG_MAP_CR_CONSENT_CSR_MAP 0x00000C80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 PrivacyOpt : 1;
    //Bits[29:1] Access Type=RO
    UINT32 Rsvd1 : 29;
    //Bits[30:30] Access Type=RW
    UINT32 LockPrivacyOpt : 1;
    //Bits[31:31] Access Type=RO
    UINT32 DebugNotification : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_DFXREGISTERS_MSG_MAP_CR_CONSENT_CSR_MAP_REGISTER;

/**
  IO QOS cfg
**/
#define SNR_MSR_VIRT_CR_IO_QOS_CFG 0x00000C83

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 IoQosEnf : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_IO_QOS_CFG_REGISTER;

/**
  MBA (mem bandwidth alloc) cfg
**/
#define SNR_MSR_VIRT_CR_MBA_CFG 0x00000C84

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 MinMaxDelay : 1;
    //Bits[1:1] Access Type=RW
    UINT32 MbaEn : 1;
    //Bits[31:2] Access Type=RO
    UINT32 Rsvd2 : 30;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_MBA_CFG_REGISTER;

/**
  way mask for locked LLC ways. If the hit way intersects this mask, LLC to SF migration is disabled
**/
#define SNR_MSR_CORE_INTERFACE_CR_L3_PROTECTED_WAYS 0x00000C85

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[19:0] Access Type=RW
    UINT32 Mask : 20;
    //Bits[51:20] Access Type=RO
    UINT32 Rsvd20Low0 : 32;
    //Bits[63:52] Access Type=RO
    UINT32 Rsvd20 : 12;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_L3_PROTECTED_WAYS_REGISTER;

/**
  Way Mask for IIO accesses
**/
#define SNR_MSR_VIRT_CR_IIO_LLC_WAYS 0x00000C8B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 WayMask : 32;
    //Bits[63:32] Access Type=RO
    UINT32 Rsvd32 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_IIO_LLC_WAYS_REGISTER;

/**
  uCR: LRU Update values for various opcodes that update the LRU bits.
**/
#define SNR_MSR_CBO_CR_QLRU_CONFIG 0x00000C8C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[1:0] Access Type=RW
    UINT32 Cr0 : 2;
    //Bits[3:2] Access Type=RW
    UINT32 Cr1 : 2;
    //Bits[5:4] Access Type=RW
    UINT32 Cr2 : 2;
    //Bits[7:6] Access Type=RW
    UINT32 Cr3 : 2;
    //Bits[9:8] Access Type=RW
    UINT32 Cr4 : 2;
    //Bits[11:10] Access Type=RW
    UINT32 Cr5 : 2;
    //Bits[13:12] Access Type=RW
    UINT32 Cr6 : 2;
    //Bits[15:14] Access Type=RW
    UINT32 Cr7 : 2;
    //Bits[17:16] Access Type=RW
    UINT32 Cr8 : 2;
    //Bits[19:18] Access Type=RW
    UINT32 Cr9 : 2;
    //Bits[21:20] Access Type=RW
    UINT32 CrB : 2;
    //Bits[23:22] Access Type=RW
    UINT32 CrD : 2;
    //Bits[25:24] Access Type=RW
    UINT32 CrF : 2;
    //Bits[27:26] Access Type=RW
    UINT32 Cr10 : 2;
    //Bits[29:28] Access Type=RW
    UINT32 Cr11 : 2;
    //Bits[31:30] Access Type=RW
    UINT32 Cr12 : 2;
    //Bits[33:32] Access Type=RW
    UINT32 Cr13 : 2;
    //Bits[43:34] Access Type=RO
    UINT32 Rsvd34 : 10;
    //Bits[45:44] Access Type=RW
    UINT32 Cr6Llc : 2;
    //Bits[47:46] Access Type=RW
    UINT32 Cr7Llc : 2;
    //Bits[49:48] Access Type=RW
    UINT32 Rsvd48 : 2;
    //Bits[51:50] Access Type=RW
    UINT32 Cr9Llc : 2;
    //Bits[53:52] Access Type=RW
    UINT32 CrBLlc : 2;
    //Bits[55:54] Access Type=RW
    UINT32 Cr14 : 2;
    //Bits[57:56] Access Type=RW
    UINT32 Cr15 : 2;
    //Bits[59:58] Access Type=RW
    UINT32 Cr16 : 2;
    //Bits[61:60] Access Type=RW
    UINT32 Rsvd60 : 2;
    //Bits[63:62] Access Type=RO
    UINT32 Rsvd62 : 2;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_CR_QLRU_CONFIG_REGISTER;

/**
  AiA PASID - upper32 is not part of the MSR and is used for the VMX exception biamp
**/
#define SNR_MSR_SCP_CR_IA32_PASID 0x00000D93

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[19:0] Access Type=RW
    UINT32 Pasid : 20;
    //Bits[30:20] Access Type=RW
    UINT32 Reserved : 11;
    //Bits[31:31] Access Type=RW
    UINT32 Valid : 1;
    //Bits[63:32] Access Type=RW
    UINT32 ReservedForVmxExceptionBitmap : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_SCP_CR_IA32_PASID_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_00 0x00000DC0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_00_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_01 0x00000DC1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_01_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_02 0x00000DC2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_02_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_03 0x00000DC3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_03_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_04 0x00000DC4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_04_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_05 0x00000DC5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_05_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_06 0x00000DC6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_06_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_07 0x00000DC7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_07_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_08 0x00000DC8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_08_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_09 0x00000DC9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_09_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_10 0x00000DCA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_10_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_11 0x00000DCB

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_11_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_12 0x00000DCC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_12_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_13 0x00000DCD

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_13_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_14 0x00000DCE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_14_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_15 0x00000DCF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_15_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_16 0x00000DD0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_16_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_17 0x00000DD1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_17_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_18 0x00000DD2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_18_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_19 0x00000DD3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_19_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_20 0x00000DD4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_20_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_21 0x00000DD5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_21_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_22 0x00000DD6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_22_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_23 0x00000DD7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_23_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_24 0x00000DD8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_24_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_25 0x00000DD9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_25_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_26 0x00000DDA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_26_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_27 0x00000DDB

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_27_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_28 0x00000DDC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_28_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_29 0x00000DDD

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_29_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_30 0x00000DDE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_30_REGISTER;

/**
  LBR info address for debug
**/
#define SNR_MSR_ARR_CR_LBR_INFO_31 0x00000DDF

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RW
    UINT32 CycleCount : 16;
    //Bits[47:16] Access Type=RO
    UINT32 Rsvd16Low0 : 32;
    //Bits[62:48] Access Type=RO
    UINT32 Rsvd16 : 15;
    //Bits[63:63] Access Type=RW
    UINT32 Mispredict : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_ARR_CR_LBR_INFO_31_REGISTER;

/**
  Global PMON
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCtrClr] is set to 1b.
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONGLCTRL_CLIENT 0x00000E01

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Pmiselc0 : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Pmiselc1 : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Pmiselc2 : 1;
    //Bits[3:3] Access Type=RW
    UINT32 Pmiselc3 : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Pmiselc4 : 1;
    //Bits[5:5] Access Type=RW
    UINT32 Pmiselc5 : 1;
    //Bits[6:6] Access Type=RW
    UINT32 Pmiselc6 : 1;
    //Bits[7:7] Access Type=RW
    UINT32 Pmiselc7 : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Pmiselc8 : 1;
    //Bits[27:9] Access Type=RO
    UINT32 Rsvd9 : 19;
    //Bits[28:28] Access Type=RW
    UINT32 Pmiovfenubp : 1;
    //Bits[29:29] Access Type=RW
    UINT32 Pmongen : 1;
    //Bits[30:30] Access Type=RW
    UINT32 Wkonpmi : 1;
    //Bits[31:31] Access Type=RW
    UINT32 Frzcountr : 1;
    //Bits[63:32] Access Type=RO
    UINT32 Rsvd32 : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONGLCTRL_CLIENT_REGISTER;

/**
  PMON Global Status register.
  All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONGLSTS_CLIENT 0x00000E02

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1C
    UINT32 OvfInPmonBlock0 : 1;
    //Bits[1:1] Access Type=RW_1C
    UINT32 OvfInPmonBlock1 : 1;
    //Bits[2:2] Access Type=RW_1C
    UINT32 OvfInPmonBlock2 : 1;
    //Bits[3:3] Access Type=RW_1C
    UINT32 OvfInPmonBlock3 : 1;
    //Bits[4:4] Access Type=RW_1C
    UINT32 OvfInPmonBlock4 : 1;
    //Bits[5:5] Access Type=RW_1C
    UINT32 OvfInPmonBlock5 : 1;
    //Bits[6:6] Access Type=RW_1C
    UINT32 OvfInPmonBlock6 : 1;
    //Bits[7:7] Access Type=RW_1C
    UINT32 OvfInPmonBlock7 : 1;
    //Bits[8:8] Access Type=RW_1C
    UINT32 OvfInPmonBlock8 : 1;
    //Bits[9:9] Access Type=RW_1C
    UINT32 OvfInPmonBlock9 : 1;
    //Bits[10:10] Access Type=RW_1C
    UINT32 OvfInPmonBlock10 : 1;
    //Bits[11:11] Access Type=RW_1C
    UINT32 OvfInPmonBlock11 : 1;
    //Bits[12:12] Access Type=RW_1C
    UINT32 OvfInPmonBlock12 : 1;
    //Bits[13:13] Access Type=RW_1C
    UINT32 OvfInPmonBlock13 : 1;
    //Bits[14:14] Access Type=RW_1C
    UINT32 OvfInPmonBlock14 : 1;
    //Bits[15:15] Access Type=RW_1C
    UINT32 OvfInPmonBlock15 : 1;
    //Bits[16:16] Access Type=RW_1C
    UINT32 OvfInPmonBlock16 : 1;
    //Bits[17:17] Access Type=RW_1C
    UINT32 OvfInPmonBlock17 : 1;
    //Bits[18:18] Access Type=RW_1C
    UINT32 OvfInPmonBlock18 : 1;
    //Bits[19:19] Access Type=RW_1C
    UINT32 OvfInPmonBlock19 : 1;
    //Bits[20:20] Access Type=RW_1C
    UINT32 OvfInPmonBlock20 : 1;
    //Bits[21:21] Access Type=RW_1C
    UINT32 OvfInPmonBlock21 : 1;
    //Bits[22:22] Access Type=RW_1C
    UINT32 OvfInPmonBlock22 : 1;
    //Bits[23:23] Access Type=RW_1C
    UINT32 OvfInPmonBlock23 : 1;
    //Bits[24:24] Access Type=RW_1C
    UINT32 OvfInPmonBlock24 : 1;
    //Bits[25:25] Access Type=RW_1C
    UINT32 OvfInPmonBlock25 : 1;
    //Bits[26:26] Access Type=RW_1C
    UINT32 OvfInPmonBlock26 : 1;
    //Bits[27:27] Access Type=RW_1C
    UINT32 OvfInPmonBlock27 : 1;
    //Bits[28:28] Access Type=RW_1C
    UINT32 OvfInPmonBlock28 : 1;
    //Bits[29:29] Access Type=RW_1C
    UINT32 OvfInPmonBlock29 : 1;
    //Bits[30:30] Access Type=RW_1C
    UINT32 OvfInPmonBlock30 : 1;
    //Bits[31:31] Access Type=RW_1C
    UINT32 OvfInPmonBlock31 : 1;
    //Bits[32:32] Access Type=RW_1C
    UINT32 OvfInPmonBlock32 : 1;
    //Bits[33:33] Access Type=RW_1C
    UINT32 OvfInPmonBlock33 : 1;
    //Bits[34:34] Access Type=RW_1C
    UINT32 OvfInPmonBlock34 : 1;
    //Bits[35:35] Access Type=RW_1C
    UINT32 OvfInPmonBlock35 : 1;
    //Bits[36:36] Access Type=RW_1C
    UINT32 OvfInPmonBlock36 : 1;
    //Bits[37:37] Access Type=RW_1C
    UINT32 OvfInPmonBlock37 : 1;
    //Bits[38:38] Access Type=RW_1C
    UINT32 OvfInPmonBlock38 : 1;
    //Bits[39:39] Access Type=RW_1C
    UINT32 OvfInPmonBlock39 : 1;
    //Bits[40:40] Access Type=RW_1C
    UINT32 OvfInPmonBlock40 : 1;
    //Bits[41:41] Access Type=RW_1C
    UINT32 OvfInPmonBlock41 : 1;
    //Bits[42:42] Access Type=RW_1C
    UINT32 OvfInPmonBlock42 : 1;
    //Bits[43:43] Access Type=RW_1C
    UINT32 OvfInPmonBlock43 : 1;
    //Bits[44:44] Access Type=RW_1C
    UINT32 OvfInPmonBlock44 : 1;
    //Bits[45:45] Access Type=RW_1C
    UINT32 OvfInPmonBlock45 : 1;
    //Bits[46:46] Access Type=RW_1C
    UINT32 OvfInPmonBlock46 : 1;
    //Bits[47:47] Access Type=RW_1C
    UINT32 OvfInPmonBlock47 : 1;
    //Bits[48:48] Access Type=RW_1C
    UINT32 OvfInPmonBlock48 : 1;
    //Bits[49:49] Access Type=RW_1C
    UINT32 OvfInPmonBlock49 : 1;
    //Bits[50:50] Access Type=RW_1C
    UINT32 OvfInPmonBlock50 : 1;
    //Bits[51:51] Access Type=RW_1C
    UINT32 OvfInPmonBlock51 : 1;
    //Bits[52:52] Access Type=RW_1C
    UINT32 OvfInPmonBlock52 : 1;
    //Bits[53:53] Access Type=RW_1C
    UINT32 OvfInPmonBlock53 : 1;
    //Bits[54:54] Access Type=RW_1C
    UINT32 OvfInPmonBlock54 : 1;
    //Bits[55:55] Access Type=RW_1C
    UINT32 OvfInPmonBlock55 : 1;
    //Bits[56:56] Access Type=RW_1C
    UINT32 OvfInPmonBlock56 : 1;
    //Bits[57:57] Access Type=RW_1C
    UINT32 OvfInPmonBlock57 : 1;
    //Bits[58:58] Access Type=RW_1C
    UINT32 OvfInPmonBlock58 : 1;
    //Bits[59:59] Access Type=RW_1C
    UINT32 OvfInPmonBlock59 : 1;
    //Bits[60:60] Access Type=RW_1C
    UINT32 OvfInPmonBlock60 : 1;
    //Bits[61:61] Access Type=RW_1C
    UINT32 OvfInPmonBlock61 : 1;
    //Bits[62:62] Access Type=RW_1C
    UINT32 OvfInPmonBlock62 : 1;
    //Bits[63:63] Access Type=RW_1C
    UINT32 OvfInPmonBlock63 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONGLSTS_CLIENT_REGISTER;

/**
  This MSR contains the value that PEBS trap ucode will load into the corresponding Fixed0 counter after writing out PEBS record into PT OUTPUT area. Applicable only when PEBS via PT is selected. The register complies with Full Width definition, hence no sign extension
**/
#define SNR_MSR_C6_SCP_CR_RELOAD_FIXED_CTR0 0x00001309

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 FixedCounter0Low0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 FixedCounter0 : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_RELOAD_FIXED_CTR0_REGISTER;

/**
  This MSR contains the value that PEBS trap ucode will load into the corresponding Fixed1 counter after writing out PEBS record into PT OUTPUT area. Applicable only when PEBS via PT is selected. The register complies with Full Width definition, hence no sign extension
**/
#define SNR_MSR_C6_SCP_CR_RELOAD_FIXED_CTR1 0x0000130A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 FixedCounter1Low0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 FixedCounter1 : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_RELOAD_FIXED_CTR1_REGISTER;

/**
  This MSR contains the value that PEBS trap ucode will load into the corresponding Fixed2 counter after writing out PEBS record into PT OUTPUT area. Applicable only when PEBS via PT is selected. The register complies with Full Width definition, hence no sign extension
**/
#define SNR_MSR_C6_SCP_CR_RELOAD_FIXED_CTR2 0x0000130B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 FixedCounter2Low0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 FixedCounter2 : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_RELOAD_FIXED_CTR2_REGISTER;

/**
  This MSR contains the value that PEBS trap ucode will load into the corresponding GP0 counter after writing out PEBS record into PT OUTPUT area. Applicable only when PEBS via PT is selected. The register complies with Full Width definition, hence no sign extension
**/
#define SNR_MSR_C6_SCP_CR_RELOAD_PMC0 0x000014C1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Gpcounter0Low0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Gpcounter0 : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_RELOAD_PMC0_REGISTER;

/**
  This MSR contains the value that PEBS trap ucode will load into the corresponding GP1 counter after writing out PEBS record into PT OUTPUT area. Applicable only when PEBS via PT is selected. The register complies with Full Width definition, hence no sign extension
**/
#define SNR_MSR_C6_SCP_CR_RELOAD_PMC1 0x000014C2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Gpcounter1Low0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Gpcounter1 : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_RELOAD_PMC1_REGISTER;

/**
  This MSR contains the value that PEBS trap ucode will load into the corresponding GP2 counter after writing out PEBS record into PT OUTPUT area. Applicable only when PEBS via PT is selected. The register complies with Full Width definition, hence no sign extension
**/
#define SNR_MSR_C6_SCP_CR_RELOAD_PMC2 0x000014C3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Gpcounter2Low0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Gpcounter2 : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_RELOAD_PMC2_REGISTER;

/**
  This MSR contains the value that PEBS trap ucode will load into the corresponding GP3 counter after writing out PEBS record into PT OUTPUT area. Applicable only when PEBS via PT is selected. The register complies with Full Width definition, hence no sign extension
**/
#define SNR_MSR_C6_SCP_CR_RELOAD_PMC3 0x000014C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 Gpcounter3Low0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Gpcounter3 : 16;
    //Bits[63:48] Access Type=Rsvd0
    UINT32 Reserved : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_RELOAD_PMC3_REGISTER;

/**
  uCode scratchpad register shared by all threads.
**/
#define SNR_MSR_FUNNYIO_CR_HW_FEEDBACK_PTR 0x000017D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Valid : 1;
    //Bits[11:1] Access Type=RO
    UINT32 Rsvd1 : 11;
    //Bits[41:12] Access Type=RW
    UINT32 Addr : 30;
    //Bits[63:42] Access Type=RO
    UINT32 Rsvd42 : 22;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_FUNNYIO_CR_HW_FEEDBACK_PTR_REGISTER;

/**
  [p]PUNIT_MSR: HW Guided Scheduling Feedback Configuration[/p]
**/
#define SNR_MSR_CORE_INTERFACE_CR_HW_FEEDBACK_CONFIG 0x000017D1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Enable : 1;
    //Bits[31:1] Access Type=RO
    UINT32 Rsvd1 : 31;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_HW_FEEDBACK_CONFIG_REGISTER;

/**
  model specific MSR for programming Snoop Filter way mask reservation
**/
#define SNR_MSR_VIRT_CR_IIO_QOS_SF_WAYS 0x0000188B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[27:0] Access Type=RW
    UINT32 SfWays : 28;
    //Bits[31:28] Access Type=RO
    UINT32 Rsvd28 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_VIRT_CR_IIO_QOS_SF_WAYS_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_PMONUNITCTRL_SERVER 0x00001C00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_PMONUNITCTRL_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_0_SERVER 0x00001C01

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_0_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_1_SERVER 0x00001C02

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_1_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_2_SERVER 0x00001C03

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_2_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_3_SERVER 0x00001C04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRCTRL_3_SERVER_REGISTER;

/**
  Global Pmon control
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRLGLCTR_SERVER 0x00001C05

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[8:0] Access Type=RW
    UINT32 Eventselect : 9;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRLGLCTR_SERVER_REGISTER;

/**
  Cbo Pmon counters status
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRSTATUS_SERVER 0x00001C07

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counter0ovf : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counter1ovf : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counter2ovf : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counter3ovf : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCTRSTATUS_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_0_SERVER 0x00001C08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_0_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_1_SERVER 0x00001C09

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_1_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_2_SERVER 0x00001C0A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_2_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_3_SERVER 0x00001C0B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE0_CR_CBOPMONCT_3_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_PMONUNITCTRL_SERVER 0x00001C10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_PMONUNITCTRL_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_0_SERVER 0x00001C11

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_0_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_1_SERVER 0x00001C12

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_1_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_2_SERVER 0x00001C13

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_2_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_3_SERVER 0x00001C14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRCTRL_3_SERVER_REGISTER;

/**
  Global Pmon control
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRLGLCTR_SERVER 0x00001C15

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[8:0] Access Type=RW
    UINT32 Eventselect : 9;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRLGLCTR_SERVER_REGISTER;

/**
  Cbo Pmon counters status
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRSTATUS_SERVER 0x00001C17

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counter0ovf : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counter1ovf : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counter2ovf : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counter3ovf : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCTRSTATUS_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_0_SERVER 0x00001C18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_0_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_1_SERVER 0x00001C19

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_1_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_2_SERVER 0x00001C1A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_2_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_3_SERVER 0x00001C1B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE1_CR_CBOPMONCT_3_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_PMONUNITCTRL_SERVER 0x00001C20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_PMONUNITCTRL_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_0_SERVER 0x00001C21

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_0_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_1_SERVER 0x00001C22

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_1_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_2_SERVER 0x00001C23

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_2_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_3_SERVER 0x00001C24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRCTRL_3_SERVER_REGISTER;

/**
  Global Pmon control
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRLGLCTR_SERVER 0x00001C25

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[8:0] Access Type=RW
    UINT32 Eventselect : 9;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRLGLCTR_SERVER_REGISTER;

/**
  Cbo Pmon counters status
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRSTATUS_SERVER 0x00001C27

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counter0ovf : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counter1ovf : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counter2ovf : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counter3ovf : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCTRSTATUS_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_0_SERVER 0x00001C28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_0_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_1_SERVER 0x00001C29

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_1_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_2_SERVER 0x00001C2A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_2_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_3_SERVER 0x00001C2B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE2_CR_CBOPMONCT_3_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_PMONUNITCTRL_SERVER 0x00001C30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_PMONUNITCTRL_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_0_SERVER 0x00001C31

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_0_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_1_SERVER 0x00001C32

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_1_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_2_SERVER 0x00001C33

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_2_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_3_SERVER 0x00001C34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRCTRL_3_SERVER_REGISTER;

/**
  Global Pmon control
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRLGLCTR_SERVER 0x00001C35

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[8:0] Access Type=RW
    UINT32 Eventselect : 9;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRLGLCTR_SERVER_REGISTER;

/**
  Cbo Pmon counters status
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRSTATUS_SERVER 0x00001C37

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counter0ovf : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counter1ovf : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counter2ovf : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counter3ovf : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCTRSTATUS_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_0_SERVER 0x00001C38

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_0_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_1_SERVER 0x00001C39

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_1_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_2_SERVER 0x00001C3A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_2_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_3_SERVER 0x00001C3B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE3_CR_CBOPMONCT_3_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_PMONUNITCTRL_SERVER 0x00001C40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_PMONUNITCTRL_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_0_SERVER 0x00001C41

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_0_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_1_SERVER 0x00001C42

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_1_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_2_SERVER 0x00001C43

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_2_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_3_SERVER 0x00001C44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRCTRL_3_SERVER_REGISTER;

/**
  Global Pmon control
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRLGLCTR_SERVER 0x00001C45

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[8:0] Access Type=RW
    UINT32 Eventselect : 9;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRLGLCTR_SERVER_REGISTER;

/**
  Cbo Pmon counters status
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRSTATUS_SERVER 0x00001C47

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counter0ovf : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counter1ovf : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counter2ovf : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counter3ovf : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCTRSTATUS_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_0_SERVER 0x00001C48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_0_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_1_SERVER 0x00001C49

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_1_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_2_SERVER 0x00001C4A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_2_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_3_SERVER 0x00001C4B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE4_CR_CBOPMONCT_3_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_PMONUNITCTRL_SERVER 0x00001C50

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_PMONUNITCTRL_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_0_SERVER 0x00001C51

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_0_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_1_SERVER 0x00001C52

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_1_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_2_SERVER 0x00001C53

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_2_SERVER_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_3_SERVER 0x00001C54

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Evslct : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=WO
    UINT32 Queueoccupancyreset : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedet : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;
    //Bits[63:32] Access Type=RW
    UINT32 Extunitmask : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRCTRL_3_SERVER_REGISTER;

/**
  Global Pmon control
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRLGLCTR_SERVER 0x00001C55

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[8:0] Access Type=RW
    UINT32 Eventselect : 9;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRLGLCTR_SERVER_REGISTER;

/**
  Cbo Pmon counters status
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRSTATUS_SERVER 0x00001C57

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counter0ovf : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counter1ovf : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counter2ovf : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counter3ovf : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCTRSTATUS_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_0_SERVER 0x00001C58

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_0_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_1_SERVER 0x00001C59

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_1_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_2_SERVER 0x00001C5A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_2_SERVER_REGISTER;

/**
  PMON Counter 0 data register. Holds the counter value.
**/
#define SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_3_SERVER 0x00001C5B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CBO_PMON_SLICE5_CR_CBOPMONCT_3_SERVER_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONUNITCTRL 0x00001E00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_0 0x00001E01

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_1 0x00001E02

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_2 0x00001E03

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_3 0x00001E04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONCNTR_3_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
     a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
    defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
    al, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
    ] w/ [4] = Counter4 and [0] = Counter 0
    0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
    I: [4:0] (same as HA)
    0]: [0] = Counter0 and [3] = Counter 3
     [0] = Counter0; [1] = Counter1
     [2] = Counter0; [3] = Counter1
    [3:0] w/ [3] = Counter 3 and [0] = Counter 0
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONUNITSTATUS 0x00001E07

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask : 7;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_0 0x00001E08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_1 0x00001E09

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_1_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_2 0x00001E0A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_2_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_3 0x00001E0B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IMI_PMONCNTRCFG_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONUNITCTRL 0x00001E10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_0 0x00001E11

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_1 0x00001E12

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_2 0x00001E13

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_3 0x00001E14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONCNTR_3_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
     a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
    defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
    al, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
    ] w/ [4] = Counter4 and [0] = Counter 0
    0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
    I: [4:0] (same as HA)
    0]: [0] = Counter0 and [3] = Counter 3
     [0] = Counter0; [1] = Counter1
     [2] = Counter0; [3] = Counter1
    [3:0] w/ [3] = Counter 3 and [0] = Counter 0
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONUNITSTATUS 0x00001E17

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask : 7;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_0 0x00001E18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_1 0x00001E19

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_1_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_2 0x00001E1A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_2_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_3 0x00001E1B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IMI_PMONCNTRCFG_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONUNITCTRL 0x00001E20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_0 0x00001E21

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_1 0x00001E22

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_2 0x00001E23

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_3 0x00001E24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONCNTR_3_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
     a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
    defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
    al, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
    ] w/ [4] = Counter4 and [0] = Counter 0
    0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
    I: [4:0] (same as HA)
    0]: [0] = Counter0 and [3] = Counter 3
     [0] = Counter0; [1] = Counter1
     [2] = Counter0; [3] = Counter1
    [3:0] w/ [3] = Counter 3 and [0] = Counter 0
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONUNITSTATUS 0x00001E27

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask : 7;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_0 0x00001E28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_1 0x00001E29

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_1_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_2 0x00001E2A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_2_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_3 0x00001E2B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IMI_PMONCNTRCFG_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONUNITCTRL 0x00001E30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_0 0x00001E31

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_1 0x00001E32

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_2 0x00001E33

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_3 0x00001E34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONCNTR_3_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
     a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
    defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
    al, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
    ] w/ [4] = Counter4 and [0] = Counter 0
    0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
    I: [4:0] (same as HA)
    0]: [0] = Counter0 and [3] = Counter 3
     [0] = Counter0; [1] = Counter1
     [2] = Counter0; [3] = Counter1
    [3:0] w/ [3] = Counter 3 and [0] = Counter 0
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONUNITSTATUS 0x00001E37

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask : 7;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_0 0x00001E38

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_1 0x00001E39

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_1_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_2 0x00001E3A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_2_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_3 0x00001E3B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IMI_PMONCNTRCFG_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONUNITCTRL 0x00001E40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_0 0x00001E41

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_1 0x00001E42

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_2 0x00001E43

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_3 0x00001E44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONCNTR_3_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
     a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
    defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
    al, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
    ] w/ [4] = Counter4 and [0] = Counter 0
    0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
    I: [4:0] (same as HA)
    0]: [0] = Counter0 and [3] = Counter 3
     [0] = Counter0; [1] = Counter1
     [2] = Counter0; [3] = Counter1
    [3:0] w/ [3] = Counter 3 and [0] = Counter 0
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONUNITSTATUS 0x00001E47

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask : 7;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_0 0x00001E48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_1 0x00001E49

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_1_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_2 0x00001E4A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_2_REGISTER;

/**
  Perfmon Counter Control Register.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_3 0x00001E4B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[35:24] Access Type=RW
    UINT32 Threshold : 12;
    //Bits[47:36] Access Type=RW
    UINT32 Chnlmask : 12;
    //Bits[50:48] Access Type=RW
    UINT32 Fcmask : 3;
    //Bits[63:51] Access Type=RO
    UINT32 Rsvd51 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IMI_PMONCNTRCFG_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONUNITCTRL0 0x00001E50

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONUNITCTRL0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_0 0x00001E51

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_1 0x00001E52

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_2 0x00001E53

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_3 0x00001E54

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTR0_3_REGISTER;

/**
  
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCTRSTATUS0 0x00001E57

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflow0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflow1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflow2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflow3 : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCTRSTATUS0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_0 0x00001E58

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_1 0x00001E59

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_1_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_2 0x00001E5A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_2_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_3 0x00001E5B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_M2P_PMONCNTRCFG0_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONUNITCTRL0 0x00001E60

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONUNITCTRL0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_0 0x00001E61

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_1 0x00001E62

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_2 0x00001E63

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_3 0x00001E64

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTR0_3_REGISTER;

/**
  
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCTRSTATUS0 0x00001E67

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflow0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflow1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflow2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflow3 : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCTRSTATUS0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_0 0x00001E68

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_1 0x00001E69

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_1_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_2 0x00001E6A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_2_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_3 0x00001E6B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_M2P_PMONCNTRCFG0_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONUNITCTRL0 0x00001E70

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONUNITCTRL0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_0 0x00001E71

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_1 0x00001E72

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_2 0x00001E73

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_3 0x00001E74

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTR0_3_REGISTER;

/**
  
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCTRSTATUS0 0x00001E77

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflow0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflow1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflow2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflow3 : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCTRSTATUS0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_0 0x00001E78

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_1 0x00001E79

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_1_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_2 0x00001E7A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_2_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_3 0x00001E7B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_M2P_PMONCNTRCFG0_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONUNITCTRL0 0x00001E80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONUNITCTRL0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_0 0x00001E81

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_1 0x00001E82

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_2 0x00001E83

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_3 0x00001E84

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTR0_3_REGISTER;

/**
  
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCTRSTATUS0 0x00001E87

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflow0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflow1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflow2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflow3 : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCTRSTATUS0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_0 0x00001E88

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_1 0x00001E89

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_1_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_2 0x00001E8A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_2_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_3 0x00001E8B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_M2P_PMONCNTRCFG0_3_REGISTER;

/**
  This register is a perfmon unit control.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONUNITCTRL0 0x00001E90

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1S
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW_1S
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[31:18] Access Type=RO
    UINT32 Rsvd18 : 14;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONUNITCTRL0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_0 0x00001E91

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_1 0x00001E92

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_2 0x00001E93

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_3 0x00001E94

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTR0_3_REGISTER;

/**
  
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCTRSTATUS0 0x00001E97

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflow0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflow1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflow2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflow3 : 1;
    //Bits[31:4] Access Type=RO
    UINT32 Rsvd4 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCTRSTATUS0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_0 0x00001E98

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_1 0x00001E99

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_1_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_2 0x00001E9A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_2_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_3 0x00001E9B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_M2P_PMONCNTRCFG0_3_REGISTER;

/**
  generated by critter 05_6_0x0f0
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IRP_PMONUNITCTRL 0x00001EA0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Ptosel : 1;
    //Bits[31:19] Access Type=RO
    UINT32 Rsvd19 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IRP_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTR_0 0x00001EA1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTR_1 0x00001EA2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTR_1_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
Whenever a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
We have defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
In general, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
HA: [4:0] w/ [4] = Counter4 and [0] = Counter 0
IMC: [5:0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
Intel QPI: [4:0] (same as HA)
PCU: [3:0]: [0] = Counter0 and [3] = Counter 3
IO IRP0: [0] = Counter0; [1] = Counter1
IO IRP1: [2] = Counter0; [3] = Counter1
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONUNITSTATUS 0x00001EA7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflowbitmask1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflowbitmask2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflowbitmask3 : 1;
    //Bits[4:4] Access Type=RW1C
    UINT32 Counteroverflowbitmask4 : 1;
    //Bits[5:5] Access Type=RW1C
    UINT32 Counteroverflowbitmask5 : 1;
    //Bits[6:6] Access Type=RW1C
    UINT32 Counteroverflowbitmask6 : 1;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTRCFG_0 0x00001EA8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTRCFG_1 0x00001EA9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_IRP0_PMONCNTRCFG_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTRCLK 0x00001EAC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTRCLK_REGISTER;

/**
  generated by critter 05_6_0x0f0
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IRP_PMONUNITCTRL 0x00001EB0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Ptosel : 1;
    //Bits[31:19] Access Type=RO
    UINT32 Rsvd19 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IRP_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTR_0 0x00001EB1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTR_1 0x00001EB2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTR_1_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
Whenever a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
We have defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
In general, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
HA: [4:0] w/ [4] = Counter4 and [0] = Counter 0
IMC: [5:0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
Intel QPI: [4:0] (same as HA)
PCU: [3:0]: [0] = Counter0 and [3] = Counter 3
IO IRP0: [0] = Counter0; [1] = Counter1
IO IRP1: [2] = Counter0; [3] = Counter1
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONUNITSTATUS 0x00001EB7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflowbitmask1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflowbitmask2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflowbitmask3 : 1;
    //Bits[4:4] Access Type=RW1C
    UINT32 Counteroverflowbitmask4 : 1;
    //Bits[5:5] Access Type=RW1C
    UINT32 Counteroverflowbitmask5 : 1;
    //Bits[6:6] Access Type=RW1C
    UINT32 Counteroverflowbitmask6 : 1;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTRCFG_0 0x00001EB8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTRCFG_1 0x00001EB9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_IRP0_PMONCNTRCFG_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTRCLK 0x00001EBC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTRCLK_REGISTER;

/**
  generated by critter 05_6_0x0f0
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IRP_PMONUNITCTRL 0x00001EC0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Ptosel : 1;
    //Bits[31:19] Access Type=RO
    UINT32 Rsvd19 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IRP_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTR_0 0x00001EC1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTR_1 0x00001EC2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTR_1_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
Whenever a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
We have defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
In general, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
HA: [4:0] w/ [4] = Counter4 and [0] = Counter 0
IMC: [5:0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
Intel QPI: [4:0] (same as HA)
PCU: [3:0]: [0] = Counter0 and [3] = Counter 3
IO IRP0: [0] = Counter0; [1] = Counter1
IO IRP1: [2] = Counter0; [3] = Counter1
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONUNITSTATUS 0x00001EC7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflowbitmask1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflowbitmask2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflowbitmask3 : 1;
    //Bits[4:4] Access Type=RW1C
    UINT32 Counteroverflowbitmask4 : 1;
    //Bits[5:5] Access Type=RW1C
    UINT32 Counteroverflowbitmask5 : 1;
    //Bits[6:6] Access Type=RW1C
    UINT32 Counteroverflowbitmask6 : 1;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTRCFG_0 0x00001EC8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTRCFG_1 0x00001EC9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_IRP0_PMONCNTRCFG_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTRCLK 0x00001ECC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTRCLK_REGISTER;

/**
  generated by critter 05_6_0x0f0
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IRP_PMONUNITCTRL 0x00001ED0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Ptosel : 1;
    //Bits[31:19] Access Type=RO
    UINT32 Rsvd19 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IRP_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTR_0 0x00001ED1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTR_1 0x00001ED2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTR_1_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
Whenever a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
We have defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
In general, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
HA: [4:0] w/ [4] = Counter4 and [0] = Counter 0
IMC: [5:0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
Intel QPI: [4:0] (same as HA)
PCU: [3:0]: [0] = Counter0 and [3] = Counter 3
IO IRP0: [0] = Counter0; [1] = Counter1
IO IRP1: [2] = Counter0; [3] = Counter1
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONUNITSTATUS 0x00001ED7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflowbitmask1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflowbitmask2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflowbitmask3 : 1;
    //Bits[4:4] Access Type=RW1C
    UINT32 Counteroverflowbitmask4 : 1;
    //Bits[5:5] Access Type=RW1C
    UINT32 Counteroverflowbitmask5 : 1;
    //Bits[6:6] Access Type=RW1C
    UINT32 Counteroverflowbitmask6 : 1;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTRCFG_0 0x00001ED8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTRCFG_1 0x00001ED9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_IRP0_PMONCNTRCFG_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTRCLK 0x00001EDC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTRCLK_REGISTER;

/**
  generated by critter 05_6_0x0f0
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IRP_PMONUNITCTRL 0x00001EE0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=WO
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[15:9] Access Type=RO
    UINT32 Rsvd9 : 7;
    //Bits[16:16] Access Type=RO
    UINT32 Freezeenable : 1;
    //Bits[17:17] Access Type=RO
    UINT32 Overflowenable : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Ptosel : 1;
    //Bits[31:19] Access Type=RO
    UINT32 Rsvd19 : 13;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IRP_PMONUNITCTRL_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTR_0 0x00001EE1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTR_1 0x00001EE2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTR_1_REGISTER;

/**
  This field shows which registers have overflowed in the unit.
Whenever a register overflows, it should set the relevant bit to 1.  An overflow should not effect the other status bits.  This status should only be cleared by software.
We have defined 7 bits for this status.  This is overkill for many units.  See below for the bits that are used in the different units.
In general, if the unit has a fixed counter, it will use bit 0.  Counter 0 would use the next LSB, and the largest counter would use the MSB.
HA: [4:0] w/ [4] = Counter4 and [0] = Counter 0
IMC: [5:0] w/ [0] = Fixed; [1] = Counter0 and [5] = Counter4
Intel QPI: [4:0] (same as HA)
PCU: [3:0]: [0] = Counter0 and [3] = Counter 3
IO IRP0: [0] = Counter0; [1] = Counter1
IO IRP1: [2] = Counter0; [3] = Counter1
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONUNITSTATUS 0x00001EE7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask0 : 1;
    //Bits[1:1] Access Type=RW1C
    UINT32 Counteroverflowbitmask1 : 1;
    //Bits[2:2] Access Type=RW1C
    UINT32 Counteroverflowbitmask2 : 1;
    //Bits[3:3] Access Type=RW1C
    UINT32 Counteroverflowbitmask3 : 1;
    //Bits[4:4] Access Type=RW1C
    UINT32 Counteroverflowbitmask4 : 1;
    //Bits[5:5] Access Type=RW1C
    UINT32 Counteroverflowbitmask5 : 1;
    //Bits[6:6] Access Type=RW1C
    UINT32 Counteroverflowbitmask6 : 1;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONUNITSTATUS_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTRCFG_0 0x00001EE8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTRCFG_0_REGISTER;

/**
  Perfmon Counter Control Register
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTRCFG_1 0x00001EE9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[7:0] Access Type=RW
    UINT32 Eventselect : 8;
    //Bits[15:8] Access Type=RW
    UINT32 Unitmask : 8;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=RW_1S
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RO
    UINT32 Rsvd19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[31:24] Access Type=RW
    UINT32 Threshold : 8;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_IRP0_PMONCNTRCFG_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTRCLK 0x00001EEC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTRCLK_REGISTER;

/**
  
**/
#define SNR_MSR_PCU_CR_PCUPMONUNITCTRL 0x00001EF0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=WO
    UINT32 Resetcounterconfigs : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Resetcounters : 1;
    //Bits[7:2] Access Type=RO
    UINT32 Rsvd2 : 6;
    //Bits[8:8] Access Type=RW
    UINT32 Freezecounters : 1;
    //Bits[31:9] Access Type=RO
    UINT32 Rsvd9 : 23;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONUNITCTRL_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTRCFG_0 0x00001EF1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW
    UINT32 Filtermask : 7;
    //Bits[7:7] Access Type=RW
    UINT32 Useoccupancy : 1;
    //Bits[13:8] Access Type=RO
    UINT32 Rsvd8 : 6;
    //Bits[15:14] Access Type=RW
    UINT32 Occselect : 2;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[30:30] Access Type=RW
    UINT32 Occinvert : 1;
    //Bits[31:31] Access Type=RW
    UINT32 Occedgedetect : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTRCFG_0_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTRCFG_1 0x00001EF2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW
    UINT32 Filtermask : 7;
    //Bits[7:7] Access Type=RW
    UINT32 Useoccupancy : 1;
    //Bits[13:8] Access Type=RO
    UINT32 Rsvd8 : 6;
    //Bits[15:14] Access Type=RW
    UINT32 Occselect : 2;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[30:30] Access Type=RW
    UINT32 Occinvert : 1;
    //Bits[31:31] Access Type=RW
    UINT32 Occedgedetect : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTRCFG_1_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTRCFG_2 0x00001EF3

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW
    UINT32 Filtermask : 7;
    //Bits[7:7] Access Type=RW
    UINT32 Useoccupancy : 1;
    //Bits[13:8] Access Type=RO
    UINT32 Rsvd8 : 6;
    //Bits[15:14] Access Type=RW
    UINT32 Occselect : 2;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[30:30] Access Type=RW
    UINT32 Occinvert : 1;
    //Bits[31:31] Access Type=RW
    UINT32 Occedgedetect : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTRCFG_2_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTRCFG_3 0x00001EF4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW
    UINT32 Filtermask : 7;
    //Bits[7:7] Access Type=RW
    UINT32 Useoccupancy : 1;
    //Bits[13:8] Access Type=RO
    UINT32 Rsvd8 : 6;
    //Bits[15:14] Access Type=RW
    UINT32 Occselect : 2;
    //Bits[16:16] Access Type=RO
    UINT32 Rsvd16 : 1;
    //Bits[17:17] Access Type=WO
    UINT32 Counterreset : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Edgedetect : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Tidfilterenable : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Overflowenable : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Internal : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counterenable : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Invert : 1;
    //Bits[29:24] Access Type=RW
    UINT32 Threshold : 6;
    //Bits[30:30] Access Type=RW
    UINT32 Occinvert : 1;
    //Bits[31:31] Access Type=RW
    UINT32 Occedgedetect : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTRCFG_3_REGISTER;

/**
  
**/
#define SNR_MSR_PCU_CR_PMONUNITSTATUS 0x00001EF7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[6:0] Access Type=RW1C
    UINT32 Counteroverflowbitmask : 7;
    //Bits[31:7] Access Type=RO
    UINT32 Rsvd7 : 25;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PMONUNITSTATUS_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTR_0 0x00001EF8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTR_0_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTR_1 0x00001EF9

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTR_1_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTR_2 0x00001EFA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTR_2_REGISTER;

/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PCUPMONCNTR_3 0x00001EFB

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 CountervalueLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Countervalue : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_PCU_CR_PCUPMONCNTR_3_REGISTER;
/**
  Holds the status information and control the operation of the the PMON Fixed Counter.
**/
#define SNR_MSR_PCU_CR_PMONPCUFILTER 0x00001EFC

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_0 0x00001F00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_1 0x00001F01

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_2 0x00001F02

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_3 0x00001F03

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_3_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_4 0x00001F04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_4_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_5 0x00001F05

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_5_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_6 0x00001F06

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_6_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_7 0x00001F07

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF0_IIO_CR_TC_PMONFRCNTR_7_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_0 0x00001F10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_1 0x00001F11

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_2 0x00001F12

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_3 0x00001F13

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_3_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_4 0x00001F14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_4_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_5 0x00001F15

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_5_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_6 0x00001F16

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_6_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_7 0x00001F17

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF1_IIO_CR_TC_PMONFRCNTR_7_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_0 0x00001F20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_1 0x00001F21

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_2 0x00001F22

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_3 0x00001F23

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_3_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_4 0x00001F24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_4_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_5 0x00001F25

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_5_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_6 0x00001F26

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_6_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_7 0x00001F27

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF2_IIO_CR_TC_PMONFRCNTR_7_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_0 0x00001F30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_1 0x00001F31

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_2 0x00001F32

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_3 0x00001F33

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_3_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_4 0x00001F34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_4_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_5 0x00001F35

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_5_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_6 0x00001F36

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_6_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_7 0x00001F37

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF3_IIO_CR_TC_PMONFRCNTR_7_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_0 0x00001F40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_0_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_1 0x00001F41

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_1_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_2 0x00001F42

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_2_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_3 0x00001F43

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_3_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_4 0x00001F44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_4_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_5 0x00001F45

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_5_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_6 0x00001F46

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_6_REGISTER;

/**
  This register is a perfmon counter.  Software can both read it and write it.
**/
#define SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_7 0x00001F47

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RO
    UINT32 CountervalueLow0 : 32;
    //Bits[35:32] Access Type=RO
    UINT32 Countervalue : 4;
    //Bits[63:36] Access Type=RO
    UINT32 Rsvd36 : 28;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_MS2IOSF4_IIO_CR_TC_PMONFRCNTR_7_REGISTER;

/**
  Global PMON
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCtrClr] is set to 1b.
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONGLCTRL_SERVER 0x00001F80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW
    UINT32 Pmiselc0 : 1;
    //Bits[1:1] Access Type=RW
    UINT32 Pmiselc1 : 1;
    //Bits[2:2] Access Type=RW
    UINT32 Pmiselc2 : 1;
    //Bits[3:3] Access Type=RW
    UINT32 Pmiselc3 : 1;
    //Bits[4:4] Access Type=RW
    UINT32 Pmiselc4 : 1;
    //Bits[5:5] Access Type=RW
    UINT32 Pmiselc5 : 1;
    //Bits[6:6] Access Type=RW
    UINT32 Pmiselc6 : 1;
    //Bits[7:7] Access Type=RW
    UINT32 Pmiselc7 : 1;
    //Bits[8:8] Access Type=RW
    UINT32 Pmiselc8 : 1;
    //Bits[9:9] Access Type=RW
    UINT32 Pmiselc9 : 1;
    //Bits[10:10] Access Type=RW
    UINT32 Pmiselc10 : 1;
    //Bits[11:11] Access Type=RW
    UINT32 Pmiselc11 : 1;
    //Bits[12:12] Access Type=RW
    UINT32 Pmiselc12 : 1;
    //Bits[13:13] Access Type=RW
    UINT32 Pmiselc13 : 1;
    //Bits[14:14] Access Type=RW
    UINT32 Pmiselc14 : 1;
    //Bits[15:15] Access Type=RW
    UINT32 Pmiselc15 : 1;
    //Bits[16:16] Access Type=RW
    UINT32 Pmiselc16 : 1;
    //Bits[17:17] Access Type=RW
    UINT32 Pmiselc17 : 1;
    //Bits[18:18] Access Type=RW
    UINT32 Pmiselc18 : 1;
    //Bits[19:19] Access Type=RW
    UINT32 Pmiselc19 : 1;
    //Bits[20:20] Access Type=RW
    UINT32 Pmiselc20 : 1;
    //Bits[21:21] Access Type=RW
    UINT32 Pmiselc21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Pmiselc22 : 1;
    //Bits[23:23] Access Type=RW
    UINT32 Pmiselc23 : 1;
    //Bits[24:24] Access Type=RW
    UINT32 Pmiselc24 : 1;
    //Bits[25:25] Access Type=RW
    UINT32 Pmiselc25 : 1;
    //Bits[26:26] Access Type=RW
    UINT32 Pmiselc26 : 1;
    //Bits[27:27] Access Type=RW
    UINT32 Pmiselc27 : 1;
    //Bits[28:28] Access Type=RW
    UINT32 Pmiselc28 : 1;
    //Bits[29:29] Access Type=RW
    UINT32 Pmiselc29 : 1;
    //Bits[30:30] Access Type=RW
    UINT32 Pmiselc30 : 1;
    //Bits[31:31] Access Type=RW
    UINT32 Pmiselc31 : 1;
    //Bits[32:32] Access Type=RW
    UINT32 Pmiselc32 : 1;
    //Bits[33:33] Access Type=RW
    UINT32 Pmiselc33 : 1;
    //Bits[34:34] Access Type=RW
    UINT32 Pmiselc34 : 1;
    //Bits[35:35] Access Type=RW
    UINT32 Pmiselc35 : 1;
    //Bits[36:36] Access Type=RW
    UINT32 Pmiselc36 : 1;
    //Bits[37:37] Access Type=RW
    UINT32 Pmiselc37 : 1;
    //Bits[38:38] Access Type=RW
    UINT32 Pmiselc38 : 1;
    //Bits[39:39] Access Type=RW
    UINT32 Pmiselc39 : 1;
    //Bits[40:40] Access Type=RW
    UINT32 Pmiselc40 : 1;
    //Bits[41:41] Access Type=RW
    UINT32 Pmiselc41 : 1;
    //Bits[42:42] Access Type=RW
    UINT32 Pmiselc42 : 1;
    //Bits[43:43] Access Type=RW
    UINT32 Pmiselc43 : 1;
    //Bits[44:44] Access Type=RW
    UINT32 Pmiselc44 : 1;
    //Bits[58:45] Access Type=RO
    UINT32 Rsvd45 : 14;
    //Bits[59:59] Access Type=WO
    UINT32 Resetcntrs : 1;
    //Bits[60:60] Access Type=RW
    UINT32 Pmiovfenubp : 1;
    //Bits[61:61] Access Type=WO
    UINT32 Unfrzcountr : 1;
    //Bits[62:62] Access Type=RW
    UINT32 Wkonpmi : 1;
    //Bits[63:63] Access Type=WO
    UINT32 Frzcountr : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONGLCTRL_SERVER_REGISTER;

/**
  This is MSR that shows to user configuration of uncore PMON's. In this particular case it shows the number of Cbo PMON banks
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONCONFIG_SERVER 0x00001F82

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[5:0] Access Type=RW
    UINT32 Numofcbopmon : 6;
    //Bits[31:6] Access Type=RO
    UINT32 Rsvd6 : 26;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONCONFIG_SERVER_REGISTER;
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERF_CTRL_SERVER 0x00001F87

/**
  Ubox local PMON mask for NCDEC events
**/
#define SNR_MSR_NCDECS_CR_UBOX_PMON_EVENT_NCDEC_MASK_CTL0 0x00001F8B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[24:0] Access Type=RWS
    UINT32 Mask : 25;
    //Bits[31:25] Access Type=RO
    UINT32 Rsvd25 : 7;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCDECS_CR_UBOX_PMON_EVENT_NCDEC_MASK_CTL0_REGISTER;

/**
  Ubox local PMON mask for NCDEC events for subunit target
**/
#define SNR_MSR_NCDECS_CR_UBOX_PMON_EVENT_NCDEC_MASK_CTL1 0x00001F8C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[11:0] Access Type=RWS
    UINT32 Mask : 12;
    //Bits[27:12] Access Type=RWS
    UINT32 AddrMask : 16;
    //Bits[31:28] Access Type=RO
    UINT32 Rsvd28 : 4;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCDECS_CR_UBOX_PMON_EVENT_NCDEC_MASK_CTL1_REGISTER;

/**
  Address offset match value for Ubox match/mask. Will compare bits that are not masked
**/
#define SNR_MSR_NCDECS_CR_UBOX_PMON_EVENT_NCDEC_MATCH_ADDR 0x00001F8D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[15:0] Access Type=RWS
    UINT32 AddrMatch : 16;
    //Bits[31:16] Access Type=RO
    UINT32 Rsvd16 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCDECS_CR_UBOX_PMON_EVENT_NCDEC_MATCH_ADDR_REGISTER;

/**
  PMON Global Status register.
  All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_CORE_INTERFACE_CR_NCUPMONGLSTS0 0x00001F8E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1C
    UINT32 OvflowPmonBlock0 : 1;
    //Bits[1:1] Access Type=RW_1C
    UINT32 OvflowPmonBlock1 : 1;
    //Bits[2:2] Access Type=RW_1C
    UINT32 OvflowPmonBlock2 : 1;
    //Bits[3:3] Access Type=RW_1C
    UINT32 OvflowPmonBlock3 : 1;
    //Bits[4:4] Access Type=RW_1C
    UINT32 OvflowPmonBlock4 : 1;
    //Bits[5:5] Access Type=RW_1C
    UINT32 OvflowPmonBlock5 : 1;
    //Bits[6:6] Access Type=RW_1C
    UINT32 OvflowPmonBlock6 : 1;
    //Bits[7:7] Access Type=RW_1C
    UINT32 OvflowPmonBlock7 : 1;
    //Bits[8:8] Access Type=RW_1C
    UINT32 OvflowPmonBlock8 : 1;
    //Bits[9:9] Access Type=RW_1C
    UINT32 OvflowPmonBlock9 : 1;
    //Bits[10:10] Access Type=RW_1C
    UINT32 OvflowPmonBlock10 : 1;
    //Bits[11:11] Access Type=RW_1C
    UINT32 OvflowPmonBlock11 : 1;
    //Bits[12:12] Access Type=RW_1C
    UINT32 OvflowPmonBlock12 : 1;
    //Bits[13:13] Access Type=RW_1C
    UINT32 OvflowPmonBlock13 : 1;
    //Bits[14:14] Access Type=RW_1C
    UINT32 OvflowPmonBlock14 : 1;
    //Bits[15:15] Access Type=RW_1C
    UINT32 OvflowPmonBlock15 : 1;
    //Bits[16:16] Access Type=RW_1C
    UINT32 OvflowPmonBlock16 : 1;
    //Bits[17:17] Access Type=RW_1C
    UINT32 OvflowPmonBlock17 : 1;
    //Bits[18:18] Access Type=RW_1C
    UINT32 OvflowPmonBlock18 : 1;
    //Bits[19:19] Access Type=RW_1C
    UINT32 OvflowPmonBlock19 : 1;
    //Bits[20:20] Access Type=RW_1C
    UINT32 OvflowPmonBlock20 : 1;
    //Bits[21:21] Access Type=RW_1C
    UINT32 OvflowPmonBlock21 : 1;
    //Bits[22:22] Access Type=RW_1C
    UINT32 OvflowPmonBlock22 : 1;
    //Bits[23:23] Access Type=RW_1C
    UINT32 OvflowPmonBlock23 : 1;
    //Bits[24:24] Access Type=RW_1C
    UINT32 OvflowPmonBlock24 : 1;
    //Bits[25:25] Access Type=RW_1C
    UINT32 OvflowPmonBlock25 : 1;
    //Bits[26:26] Access Type=RW_1C
    UINT32 OvflowPmonBlock26 : 1;
    //Bits[27:27] Access Type=RW_1C
    UINT32 OvflowPmonBlock27 : 1;
    //Bits[28:28] Access Type=RW_1C
    UINT32 OvflowPmonBlock28 : 1;
    //Bits[29:29] Access Type=RW_1C
    UINT32 OvflowPmonBlock29 : 1;
    //Bits[30:30] Access Type=RW_1C
    UINT32 OvflowPmonBlock30 : 1;
    //Bits[31:31] Access Type=RW_1C
    UINT32 OvflowPmonBlock31 : 1;
    //Bits[32:32] Access Type=RW_1C
    UINT32 OvflowPmonBlock32 : 1;
    //Bits[33:33] Access Type=RW_1C
    UINT32 OvflowPmonBlock33 : 1;
    //Bits[34:34] Access Type=RW_1C
    UINT32 OvflowPmonBlock34 : 1;
    //Bits[35:35] Access Type=RW_1C
    UINT32 OvflowPmonBlock35 : 1;
    //Bits[36:36] Access Type=RW_1C
    UINT32 OvflowPmonBlock36 : 1;
    //Bits[37:37] Access Type=RW_1C
    UINT32 OvflowPmonBlock37 : 1;
    //Bits[38:38] Access Type=RW_1C
    UINT32 OvflowPmonBlock38 : 1;
    //Bits[39:39] Access Type=RW_1C
    UINT32 OvflowPmonBlock39 : 1;
    //Bits[40:40] Access Type=RW_1C
    UINT32 OvflowPmonBlock40 : 1;
    //Bits[41:41] Access Type=RW_1C
    UINT32 OvflowPmonBlock41 : 1;
    //Bits[42:42] Access Type=RW_1C
    UINT32 OvflowPmonBlock42 : 1;
    //Bits[43:43] Access Type=RW_1C
    UINT32 OvflowPmonBlock43 : 1;
    //Bits[44:44] Access Type=RW_1C
    UINT32 OvflowPmonBlock44 : 1;
    //Bits[45:45] Access Type=RW_1C
    UINT32 OvflowPmonBlock45 : 1;
    //Bits[46:46] Access Type=RW_1C
    UINT32 OvflowPmonBlock46 : 1;
    //Bits[47:47] Access Type=RW_1C
    UINT32 OvflowPmonBlock47 : 1;
    //Bits[48:48] Access Type=RW_1C
    UINT32 OvflowPmonBlock48 : 1;
    //Bits[49:49] Access Type=RW_1C
    UINT32 OvflowPmonBlock49 : 1;
    //Bits[50:50] Access Type=RW_1C
    UINT32 OvflowPmonBlock50 : 1;
    //Bits[51:51] Access Type=RW_1C
    UINT32 OvflowPmonBlock51 : 1;
    //Bits[52:52] Access Type=RW_1C
    UINT32 OvflowPmonBlock52 : 1;
    //Bits[53:53] Access Type=RW_1C
    UINT32 OvflowPmonBlock53 : 1;
    //Bits[54:54] Access Type=RW_1C
    UINT32 OvflowPmonBlock54 : 1;
    //Bits[55:55] Access Type=RW_1C
    UINT32 OvflowPmonBlock55 : 1;
    //Bits[56:56] Access Type=RW_1C
    UINT32 OvflowPmonBlock56 : 1;
    //Bits[57:57] Access Type=RW_1C
    UINT32 OvflowPmonBlock57 : 1;
    //Bits[58:58] Access Type=RW_1C
    UINT32 OvflowPmonBlock58 : 1;
    //Bits[59:59] Access Type=RW_1C
    UINT32 OvflowPmonBlock59 : 1;
    //Bits[60:60] Access Type=RW_1C
    UINT32 OvflowPmonBlock60 : 1;
    //Bits[61:61] Access Type=RW_1C
    UINT32 OvflowPmonBlock61 : 1;
    //Bits[62:62] Access Type=RW_1C
    UINT32 OvflowPmonBlock62 : 1;
    //Bits[63:63] Access Type=RW_1C
    UINT32 OvflowPmonBlock63 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_NCUPMONGLSTS0_REGISTER;

/**
  PMON Global Status register.
  All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_CORE_INTERFACE_CR_NCUPMONGLSTS1 0x00001F8F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[0:0] Access Type=RW_1C
    UINT32 OvflowPmonBlock0 : 1;
    //Bits[1:1] Access Type=RW_1C
    UINT32 OvflowPmonBlock1 : 1;
    //Bits[2:2] Access Type=RW_1C
    UINT32 OvflowPmonBlock2 : 1;
    //Bits[3:3] Access Type=RW_1C
    UINT32 OvflowPmonBlock3 : 1;
    //Bits[4:4] Access Type=RW_1C
    UINT32 OvflowPmonBlock4 : 1;
    //Bits[5:5] Access Type=RW_1C
    UINT32 OvflowPmonBlock5 : 1;
    //Bits[6:6] Access Type=RW_1C
    UINT32 OvflowPmonBlock6 : 1;
    //Bits[7:7] Access Type=RW_1C
    UINT32 OvflowPmonBlock7 : 1;
    //Bits[8:8] Access Type=RW_1C
    UINT32 OvflowPmonBlock8 : 1;
    //Bits[9:9] Access Type=RW_1C
    UINT32 OvflowPmonBlock9 : 1;
    //Bits[10:10] Access Type=RW_1C
    UINT32 OvflowPmonBlock10 : 1;
    //Bits[11:11] Access Type=RW_1C
    UINT32 OvflowPmonBlock11 : 1;
    //Bits[12:12] Access Type=RW_1C
    UINT32 OvflowPmonBlock12 : 1;
    //Bits[13:13] Access Type=RW_1C
    UINT32 OvflowPmonBlock13 : 1;
    //Bits[14:14] Access Type=RW_1C
    UINT32 OvflowPmonBlock14 : 1;
    //Bits[15:15] Access Type=RW_1C
    UINT32 OvflowPmonBlock15 : 1;
    //Bits[16:16] Access Type=RW_1C
    UINT32 OvflowPmonBlock16 : 1;
    //Bits[17:17] Access Type=RW_1C
    UINT32 OvflowPmonBlock17 : 1;
    //Bits[18:18] Access Type=RW_1C
    UINT32 OvflowPmonBlock18 : 1;
    //Bits[19:19] Access Type=RW_1C
    UINT32 OvflowPmonBlock19 : 1;
    //Bits[20:20] Access Type=RW_1C
    UINT32 OvflowPmonBlock20 : 1;
    //Bits[21:21] Access Type=RW_1C
    UINT32 OvflowPmonBlock21 : 1;
    //Bits[22:22] Access Type=RW_1C
    UINT32 OvflowPmonBlock22 : 1;
    //Bits[23:23] Access Type=RW_1C
    UINT32 OvflowPmonBlock23 : 1;
    //Bits[24:24] Access Type=RW_1C
    UINT32 OvflowPmonBlock24 : 1;
    //Bits[25:25] Access Type=RW_1C
    UINT32 OvflowPmonBlock25 : 1;
    //Bits[26:26] Access Type=RW_1C
    UINT32 OvflowPmonBlock26 : 1;
    //Bits[27:27] Access Type=RW_1C
    UINT32 OvflowPmonBlock27 : 1;
    //Bits[28:28] Access Type=RW_1C
    UINT32 OvflowPmonBlock28 : 1;
    //Bits[29:29] Access Type=RW_1C
    UINT32 OvflowPmonBlock29 : 1;
    //Bits[30:30] Access Type=RW_1C
    UINT32 OvflowPmonBlock30 : 1;
    //Bits[31:31] Access Type=RW_1C
    UINT32 OvflowPmonBlock31 : 1;
    //Bits[32:32] Access Type=RW_1C
    UINT32 OvflowPmonBlock32 : 1;
    //Bits[33:33] Access Type=RW_1C
    UINT32 OvflowPmonBlock33 : 1;
    //Bits[34:34] Access Type=RW_1C
    UINT32 OvflowPmonBlock34 : 1;
    //Bits[35:35] Access Type=RW_1C
    UINT32 OvflowPmonBlock35 : 1;
    //Bits[36:36] Access Type=RW_1C
    UINT32 OvflowPmonBlock36 : 1;
    //Bits[37:37] Access Type=RW_1C
    UINT32 OvflowPmonBlock37 : 1;
    //Bits[38:38] Access Type=RW_1C
    UINT32 OvflowPmonBlock38 : 1;
    //Bits[39:39] Access Type=RW_1C
    UINT32 OvflowPmonBlock39 : 1;
    //Bits[40:40] Access Type=RW_1C
    UINT32 OvflowPmonBlock40 : 1;
    //Bits[41:41] Access Type=RW_1C
    UINT32 OvflowPmonBlock41 : 1;
    //Bits[42:42] Access Type=RW_1C
    UINT32 OvflowPmonBlock42 : 1;
    //Bits[43:43] Access Type=RW_1C
    UINT32 OvflowPmonBlock43 : 1;
    //Bits[44:44] Access Type=RW_1C
    UINT32 OvflowPmonBlock44 : 1;
    //Bits[45:45] Access Type=RW_1C
    UINT32 OvflowPmonBlock45 : 1;
    //Bits[46:46] Access Type=RW_1C
    UINT32 OvflowPmonBlock46 : 1;
    //Bits[47:47] Access Type=RW_1C
    UINT32 OvflowPmonBlock47 : 1;
    //Bits[48:48] Access Type=RW_1C
    UINT32 OvflowPmonBlock48 : 1;
    //Bits[49:49] Access Type=RW_1C
    UINT32 OvflowPmonBlock49 : 1;
    //Bits[50:50] Access Type=RW_1C
    UINT32 OvflowPmonBlock50 : 1;
    //Bits[51:51] Access Type=RW_1C
    UINT32 OvflowPmonBlock51 : 1;
    //Bits[52:52] Access Type=RW_1C
    UINT32 OvflowPmonBlock52 : 1;
    //Bits[53:53] Access Type=RW_1C
    UINT32 OvflowPmonBlock53 : 1;
    //Bits[54:54] Access Type=RW_1C
    UINT32 OvflowPmonBlock54 : 1;
    //Bits[55:55] Access Type=RW_1C
    UINT32 OvflowPmonBlock55 : 1;
    //Bits[56:56] Access Type=RW_1C
    UINT32 OvflowPmonBlock56 : 1;
    //Bits[57:57] Access Type=RW_1C
    UINT32 OvflowPmonBlock57 : 1;
    //Bits[58:58] Access Type=RW_1C
    UINT32 OvflowPmonBlock58 : 1;
    //Bits[59:59] Access Type=RW_1C
    UINT32 OvflowPmonBlock59 : 1;
    //Bits[60:60] Access Type=RW_1C
    UINT32 OvflowPmonBlock60 : 1;
    //Bits[61:61] Access Type=RW_1C
    UINT32 OvflowPmonBlock61 : 1;
    //Bits[62:62] Access Type=RW_1C
    UINT32 OvflowPmonBlock62 : 1;
    //Bits[63:63] Access Type=RW_1C
    UINT32 OvflowPmonBlock63 : 1;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_CORE_INTERFACE_CR_NCUPMONGLSTS1_REGISTER;
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFEVTSEL0_SERVER 0x00001F91
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFEVTSEL1_SERVER 0x00001F92

/**
  Holds the status information and control the operation of the PMON Fixed Counter.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCtrClr] is set to 1b.
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONCTLFIX_SERVER 0x00001F93

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[19:0] Access Type=RO
    UINT32 Rsvd0 : 20;
    //Bits[20:20] Access Type=RW
    UINT32 Ovfen : 1;
    //Bits[21:21] Access Type=RO
    UINT32 Rsvd21 : 1;
    //Bits[22:22] Access Type=RW
    UINT32 Counteren : 1;
    //Bits[31:23] Access Type=RO
    UINT32 Rsvd23 : 9;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONCTLFIX_SERVER_REGISTER;

/**
  PMON Fixed Counter data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_NCEVENTS_CR_NCUPMONCTRFX_SERVER 0x00001F94

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[47:32] Access Type=RW
    UINT32 Pmonctrdata : 16;
    //Bits[63:48] Access Type=RO
    UINT32 Rsvd48 : 16;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_NCEVENTS_CR_NCUPMONCTRFX_SERVER_REGISTER;
/**
  Temporary definition for usim enabling.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERF_STATUS_SERVER 0x00001F97

/**
  PMON Counter 0  data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFCTR0_SERVER 0x00001F98

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNC_ARB_0_CR_PERFCTR0_SERVER_REGISTER;

/**
  PMON Counter 0  data register
 The Fix counter is counting UCLK cycles.
 All fields in this reigster must be reset when LT_CONTROL_MSR[LTPmonCntClr] is set to 1b.
**/
#define SNR_MSR_UNC_ARB_0_CR_PERFCTR1_SERVER 0x00001F99

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 PmonctrdataLow0 : 32;
    //Bits[43:32] Access Type=RW
    UINT32 Pmonctrdata : 12;
    //Bits[63:44] Access Type=RO
    UINT32 Rsvd44 : 20;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_UNC_ARB_0_CR_PERFCTR1_SERVER_REGISTER;

/**
  Lower half of CSTAR register
**/
#define SNR_MSR_C6_SCP_CR_CSTAR 0xC0000083

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    //Bits[31:0] Access Type=RW
    UINT32 LowerHalfOfTargetRip : 32;
    //Bits[63:32] Access Type=RW
    UINT32 UpperHalfOfTargetRip : 32;

  } Bits;

  UINT32  Uint32;
  UINT64  Uint64;
} SNR_MSR_C6_SCP_CR_CSTAR_REGISTER;
/**
  This register holds the Kernel GS Base value
**/
#define SNR_MSR_SCP_CR_KERNEL_GSBASE 0xC0000102

#endif  //_SNR_MSRS_HEADER_
