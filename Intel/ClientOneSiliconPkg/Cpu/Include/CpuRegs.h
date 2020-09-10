/** @file
  Register names for CPU registers

  <b>Conventions</b>
  - Definitions beginning with "MSR_" are MSRs
  - Definitions beginning with "R_" are registers
  - Definitions beginning with "B_" are bits within registers
  - Definitions beginning with "V_" are meaningful values of bits within the registers
  - Definitions beginning with "S_" are register sizes
  - Definitions beginning with "N_" are the bit position

@copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation.

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
#ifndef _CPU_REGS_H_
#define _CPU_REGS_H_


/**
  Package. Maximum Ratio Limit of Turbo Mode by Core Groups (RW) Specifies
  Maximum Ratio Limit for each Core Group. Max ratio for groups with more
  cores must decrease monotonically. For groups with less than 4 cores, the
  max ratio must be 32 or less. For groups with 4-5 cores, the max ratio must
  be 22 or less. For groups with more than 5 cores, the max ratio must be 16
  or less..

  @param  ECX  MSR_TURBO_RATIO_LIMIT (0x000001AD)
  @param  EAX  Lower 32-bits of MSR value.
               Described by the type MSR_TURBO_RATIO_LIMIT_REGISTER.
  @param  EDX  Upper 32-bits of MSR value.
               Described by the type MSR_TURBO_RATIO_LIMIT_REGISTER.

  <b>Example usage</b>
  @code
  MSR_TURBO_RATIO_LIMIT_REGISTER  Msr;

  Msr.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
  AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT, Msr.Uint64);
  @endcode
**/
#define MSR_TURBO_RATIO_LIMIT           0x000001AD

/**
  MSR information returned for MSR index #MSR_TURBO_RATIO_LIMIT
**/
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    ///
    /// [Bits 7:0] Package. Maximum Ratio Limit for Active cores in MaxTurbo 1Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to MaxTurbo 1Core threshold.
    ///
    UINT32  MaxTurbo1Core:8;
    ///
    /// [Bits 15:8] Package. Maximum Ratio Limit for Active cores in MaxTurbo 2Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to MaxTurbo 2Core threshold and greater than MaxTurbo 1Core threshold.
    ///
    UINT32  MaxTurbo2Core:8;
    ///
    /// [Bits 23:16] Package. Maximum Ratio Limit for Active cores in MaxTurbo 3Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to MaxTurbo 3Core threshold and greater than MaxTurbo 2Core threshold.
    ///
    UINT32  MaxTurbo3Core:8;
    ///
    /// [Bits 31:24] Package. Maximum Ratio Limit for Active cores in MaxTurbo 4Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to MaxTurbo 4Core threshold and greater than MaxTurbo 3Core threshold.
    ///
    UINT32  MaxTurbo4Core:8;
    ///
    /// [Bits 39:32] Package. Maximum Ratio Limit for Active cores in MaxTurbo 5Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to MaxTurbo 5Core and greater than MaxTurbo 4Core threshold.
    ///
    UINT32  MaxTurbo5Core:8;
    ///
    /// [Bits 47:40] Package. Maximum Ratio Limit for Active cores in MaxTurbo 6Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to MaxTurbo 6Core threshold and greater than MaxTurbo 5Core threshold.
    ///
    UINT32  MaxTurbo6Core:8;
    ///
    /// [Bits 55:48] Package. Maximum Ratio Limit for Active cores in MaxTurbo 7Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to Group 7 threshold and greater than MaxTurbo 6Core threshold.
    ///
    UINT32  MaxTurbo7Core:8;
    ///
    /// [Bits 63:56] Package. Maximum Ratio Limit for Active cores in MaxTurbo 8Core
    /// Maximum turbo ratio limit when number of active cores is less or equal
    /// to MaxTurbo 8Core threshold and greater than MaxTurbo 7Core threshold.
    ///
    UINT32  MaxTurbo8Core:8;
  } Bits;
  ///
  /// All bit fields as a 64-bit value
  ///
  UINT64  Uint64;
} MSR_TURBO_RATIO_LIMIT_REGISTER;

#define PRMRR_MAX_SIZE                                                0x10000000

#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_19_MHZ    0x2FBA2E26
#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_24_MHZ    0x262E8B52
#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_38_MHZ    0x17DD1713
#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_DELAY_VALUE                 0x13

// @todo. Move the definitions to CoreMsrCommon.h
#define MSR_UNCORE_PRMRR_PHYS_BASE                                    0x000002F4
#define MSR_UNCORE_PRMRR_PHYS_MASK                                    0x000002F5
#define B_MSR_PRMRR_PHYS_MASK_LOCK                                    BIT10
#define B_MSR_PRMRR_VALID_BIT                                         BIT11

//. @todo. this alternate msr need to move under fru
#define MSR_PRMRR_PHYS_BASE_ALT                                       0x000001F4
#define MSR_PRMRR_PHYS_MASK_ALT                                       0x000001F5
#define B_MSR_PRMRR_BASE_0_CONFIGURED_MASK                            BIT3


#define B_CRASHLOG_DUMP_DISABLE                                       BIT0

#define V_TME_ACTIVATE_TME_POLICY_AES_XTS                             0x00
#define V_TME_ACTIVATE_TME_POLICY_AES_XTS_256                         0x02

#define MSR_BOOT_GUARD_SACM_INFO                                      0x0000013A
#define B_BOOT_GUARD_SACM_INFO_NEM_ENABLED                            BIT0
#define V_TPM_PRESENT_MASK                                            0x06
#define B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS                            BIT3
#define B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT                          BIT5
#define B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT                          BIT6
#define B_BOOT_GUARD_SACM_INFO_CAPABILITY                             BIT32

#define V_CSTATE_LIMIT_C1                                             0x01
#define V_CSTATE_LIMIT_C3                                             0x02
#define V_CSTATE_LIMIT_C6                                             0x03
#define V_CSTATE_LIMIT_C7                                             0x04
#define V_CSTATE_LIMIT_C7S                                            0x05
#define V_CSTATE_LIMIT_C8                                             0x06
#define V_CSTATE_LIMIT_C9                                             0x07
#define V_CSTATE_LIMIT_C10                                            0x08

#define V_IO_CAPT_LVL2                                                0x0   ///< C3
#define V_IO_CAPT_LVL3                                                0x1   ///< C6
#define V_IO_CAPT_LVL4                                                0x2   ///< C7
#define V_IO_CAPT_LVL5                                                0x3   ///< C8
#define V_IO_CAPT_LVL6                                                0x4   ///< C9
#define V_IO_CAPT_LVL7                                                0x5   ///< C10

#define B_CAP_MSR_HWP_REQ                                             BIT0

#define MSR_FAST_UNCORE_MSR_CTL_MASK                                  BIT0

// @todo. use XXX_MSR_FIT_BIOS_ERROR in each generation.
#define MSR_FIT_BIOS_ERROR                                            0x000000A5
#define B_MSR_FIT_BIOS_ERROR_ERROR_CODE_MASK                          0x000000FF
#define N_MSR_FIT_BIOS_ERROR_ENTRY_TYPE_OFFSET                        8
#define B_MSR_FIT_BIOS_ERROR_ENTRY_TYPE_MASK                          0x0000FF00

#define FIT_HEADER_ENTRY                                              0x0
#define FIT_SUCCESSFUL                                                0x0
#define FIT_SIZE_CHECK                                                0x1
#define FIT_RESERVED_FIELD_CHECK                                      0x2
#define FIT_VERSION_AND_TYPE_CHECK                                    0x3

#define FIT_MICROCODE_UPDATE_ENTRY                                    0x1
#define FIT_NO_MICROCODE_UPDATE                                       0x1
#define FIT_MICROCODE_UPDATE_FAIL                                     0x2

#define FIT_STARTUP_ACM_ENTRY                                         0x2
#define FIT_STARTUP_ACM_NOT_SUPPORTED                                 0x1
#define FIT_FATAL_ERROR_DURING_ACM                                    0x2
#define FIT_CPU_DOES_NOT_SUPPORT_LT                                   0x3
#define FIT_BIST_ERRORS                                               0x4
#define FIT_BEYOND_END_OF_FIT                                         0x5
#define FIT_NO_FIT_ACM_TYPE_MISMATCH                                  0x6
#define FIT_ACM_BASE_SIZE_AND_CHECKS                                  0x7

#define FIT_GENERAL_CHECKS                                            0x7F
#define FIT_DISABLED_BY_CPU                                           0x1
#define FIT_POINTER_ERROR                                             0x2
#define FIT_FIRST_FIT_ENTRY_MISMATCH                                  0x3


#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_OFFSET                 0
#define V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_MASK                   0x000000000000FFFF
#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_OFFSET                   16
#define V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_MASK                     0x000000000000FFFF
#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_OFFSET               32
#define V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_MASK                 0x000000000000FFFF
#define B_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE                            BIT62
#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE_OFFSET                     62

///
/// Other defines
///

#ifndef CPU_FEATURE_DISABLE
#define CPU_FEATURE_DISABLE  0
#endif
#ifndef CPU_FEATURE_ENABLE
#define CPU_FEATURE_ENABLE   1
#endif

#define CACHE_UNCACHEABLE               0
#define CACHE_WRITEBACK                 6

#define ATOM_MODULE_ID_1                0x8
#define ATOM_MODULE_ID_2                0x9
#define V_SMALL_CORE_TYPE               0x20
#define V_BIG_CORE_TYPE                 0x40

#define TXT_PUBLIC_BASE                                               0xFED30000
#define R_CPU_BOOT_GUARD_BOOTSTATUS                                   0xA0
#define R_CPU_BOOT_GUARD_ACM_STATUS                                   0x328
#define R_CPU_ACM_POLICY_STATUS                                       0x378
#define MMIO_ACM_POLICY_STATUS                                        (TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS)
#define V_CPU_BOOT_GUARD_LOAD_ACM_SUCCESS                             0x8000000000000000
#define B_BOOT_GUARD_ACM_ERRORCODE_MASK                               0x00007FF0
#define B_CPU_BOOT_GUARD_BOOTSTATUS_BLOCK_BOOT_ENABLED                BIT0
#define V_CPU_BOOT_GUARD_BOOTSTATUS_BLOCK_BOOT_ENABLED                0x01
#define B_CPU_BOOT_GUARD_BOOTSTATUS_S3_TPM_STARTUP_FAILED             BIT46
#define R_CPU_BOOT_GUARD_NEM_MAP_STRUCTURE_ADDRESS                    0x320

//
// Processor Definitions
//
#define CPUID_FULL_STEPPING                        0x0000000F
#define CPUID_FULL_FAMILY_MODEL                    0x0FFF0FF0
#define CPUID_FULL_FAMILY_MODEL_STEPPING           0x0FFF0FFF
#define CPUID_FULL_FAMILY_MODEL_COFFEELAKE_ULT_ULX 0x000806E0
#define CPUID_FULL_FAMILY_MODEL_ICELAKE_ULT_ULX    0x000706E0
#define CPUID_FULL_FAMILY_MODEL_ICELAKE_DT_HALO_R  0x000906F0
#define CPUID_FULL_FAMILY_MODEL_ICELAKE_DT_HALO    0x000706D0
#define CPUID_FULL_FAMILY_MODEL_ICELAKE_DT         0x000706D0
#define CPUID_FULL_FAMILY_MODEL_ICELAKE_AIPG       0x000906D0
#define CPUID_FULL_FAMILY_MODEL_ELKHARTLAKE        0x00090660
#define CPUID_FULL_FAMILY_MODEL_JASPERLAKE         0x000906C0
#define CPUID_FULL_FAMILY_MODEL_TIGERLAKE_ULT_ULX  0x000806C0
#define CPUID_FULL_FAMILY_MODEL_TIGERLAKE_DT_HALO  0x000806D0
#define CPUID_FULL_FAMILY_MODEL_ROCKETLAKE_ULT_ULX 0x000A0680
#define CPUID_FULL_FAMILY_MODEL_ROCKETLAKE_DT_HALO 0x000A0670
#define CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO  0x00090670
#define CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE   0x000906A0
#define CPUID_FULL_FAMILY_MODEL_COMETLAKE_DT_HALO  0x000A0650
#define CPUID_FULL_FAMILY_MODEL_METEORLAKE_MOBILE  0x000A06A0
#define CPUID_FULL_FAMILY_MODEL_METEORLAKE_DT_HALO 0x000A06C0

#ifndef SLE_FLAG
#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND 1
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND 1000
#endif
#else // SLE FLAG
#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND 0
#endif
#ifndef STALL_ONE_MILLI_SECOND
#define STALL_ONE_MILLI_SECOND 0
#endif
#endif // SLE_FLAG

#define BITS(x) (1 << (x))

/**
Notes :
  1.  Bit position always starts at 0.
  2.  Following macros are applicable only for Word aligned integers.
**/
#define BIT(Pos, Value)               (1 << (Pos) & (Value))

///
/// Enums for CPU Family IDs
///
typedef enum {
  EnumCpuCflUltUlx    = CPUID_FULL_FAMILY_MODEL_COFFEELAKE_ULT_ULX,
  EnumCpuIclUltUlx    = CPUID_FULL_FAMILY_MODEL_ICELAKE_ULT_ULX,
  EnumCpuIclDtHaloR   = CPUID_FULL_FAMILY_MODEL_ICELAKE_DT_HALO_R,
  EnumCpuIclDtHalo    = CPUID_FULL_FAMILY_MODEL_ICELAKE_DT_HALO,
  EnumCpuIclDt        = CPUID_FULL_FAMILY_MODEL_ICELAKE_DT,
  EnumCpuIclAipg      = CPUID_FULL_FAMILY_MODEL_ICELAKE_AIPG,
  EnumCpuJsl          = CPUID_FULL_FAMILY_MODEL_JASPERLAKE,
  EnumCpuEhl          = CPUID_FULL_FAMILY_MODEL_ELKHARTLAKE,
  EnumCpuTglUltUlx    = CPUID_FULL_FAMILY_MODEL_TIGERLAKE_ULT_ULX,
  EnumCpuTglDtHalo    = CPUID_FULL_FAMILY_MODEL_TIGERLAKE_DT_HALO,
  EnumCpuRklUltUlx    = CPUID_FULL_FAMILY_MODEL_ROCKETLAKE_ULT_ULX,
  EnumCpuRklDtHalo    = CPUID_FULL_FAMILY_MODEL_ROCKETLAKE_DT_HALO,
  EnumCpuAdlDtHalo    = CPUID_FULL_FAMILY_MODEL_ALDERLAKE_DT_HALO,
  EnumCpuAdlMobile    = CPUID_FULL_FAMILY_MODEL_ALDERLAKE_MOBILE,
  EnumCpuCmlDtHalo    = CPUID_FULL_FAMILY_MODEL_COMETLAKE_DT_HALO,
  EnumCpuMtlMobile    = CPUID_FULL_FAMILY_MODEL_METEORLAKE_MOBILE,
  EnumCpuMtlDtHalo    = CPUID_FULL_FAMILY_MODEL_METEORLAKE_DT_HALO,
  EnumCpuMax          = CPUID_FULL_FAMILY_MODEL
} CPU_FAMILY;

///
/// Enums for CPU Stepping IDs
///
typedef enum {
  /// Whiskey Lake ULT Steppings
  EnumCflW0         = 0xB,
  EnumCflV0         = 0xC,

  EnumCflMaxUltUlxStep = EnumCflW0,
  ///
  /// Icelake ULX/ULT Steppings
  ///
  EnumIclA0         = 0,
  EnumIclB0         = 1,
  EnumIclB4         = 2,
  EnumIclC0         = 3,
  EnumIclD0         = 4,
  EnumIclD1         = 5,


  ///
  /// Icelake DT/Halo Refresh Steppings
  ///
  EnumIclG0         = 0,
  EnumIclH0         = 1,

  ///
  /// Icelake DT/Halo Steppings
  ///
  EnumIclP0         = 1,
  EnumIclQ0         = 2,
  ///
  /// Icelake DT Steppings
  ///
  EnumIclS0         = 1,

  ///
  /// Icelake AIPG Steppings
  ///
  EnumIclI0         = 0,
  ///
  /// Elkhartlake Steppings
  ///
  EnumEhlA0 = 0,
  EnumEhlA1 = 1,
  ///
  ///
  /// Jasperlake Steppings
  ///
  EnumJslA0 = 0,
  ///
  /// Tigerlake ULX/ULT Steppings
  ///
  EnumTglA0 = 0,
  EnumTglB0 = 1,

  ///
  /// Tigerlake DT/Halo Steppings
  ///
  EnumTglP0 = 0,
  EnumTglQ0 = 1,

  ///
  /// RocketLake ULX/ULT Steppings
  ///
  EnumRklA0 = 0,

  ///
  /// RocketLake DT/Halo Steppings
  ///
  EnumRklP0 = 0,

  ///
  /// AlderLake DT Steppings
  ///
  EnumAdlA0 = 0,
  EnumAdlB0 = 1,

  ///
  /// AlderLake Halo Steppings
  ///
  EnumAdlG0 = 1,
  EnumAdlH0 = 2,

  ///
  /// AlderLake Mobile Steppings
  ///
  EnumAdlJ0 = 0,
  EnumAdlK0 = 1,
  EnumAdlR0 = 2,

  ///
  /// CometLake DT/Halo Steppings
  ///
  EnumCmlG0 = 0,
  EnumCmlR1 = 2,
  EnumCmlP0 = 1,
  EnumCmlG1 = 3,
  EnumCmlP1 = 4,
  EnumCmlQ0 = 5,
  EnumCmlMaxDtHaloStep = EnumCmlR1,

  ///
  /// MeteorLake DT Steppings
  ///
  EnumMtlSA0 = 0,


  ///
  /// MeteorLake Mobile Steppings
  ///
  EnumMtlPMA0 = 0,

  ///
  /// Max Stepping
  ///
  EnumCpuSteppingMax  = CPUID_FULL_STEPPING
} CPU_STEPPING;

///
/// Enums for CPU SKU IDs
///
typedef enum {
  EnumCpuUlt        = 0,
  EnumCpuTrad,
  EnumCpuUlx,
  EnumCpuHalo,
  EnumCpuUnknown
} CPU_SKU;

///
/// Enums for CPU Generation
///
typedef enum {
  EnumCflCpu  = 0,
  EnumIclCpu,
  EnumTglCpu,
  EnumJslCpu,
  EnumEhlCpu,
  EnumRklCpu,
  EnumAdlCpu,
  EnumCmlCpu,
  EnumMtlCpu,
  EnumCpuUnknownGeneration = 255
} CPU_GENERATION;
#endif
