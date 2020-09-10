/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_BIOS_TO_PCODE_H_
#define _CPU_BIOS_TO_PCODE_H_

typedef union {
  struct {
    UINT32 command:8;           // Bits[7:0]
    UINT32 read_write:1;        // Bits[8], 0: read; 1: write
    UINT32 rsvd_28_9:20;        // Bits[28:9]
    UINT32 rsvd_30_29:2;        // Bits[30:29]
    UINT32 RunBusy:1;           // Bit[31]
  } Bits;
  UINT32 Data;

} B2P_STANDARD_INTERFACE_STRUCT;

typedef union {
  struct {
    UINT32 MCT_DISABLE:1;            // Bits[0], MCT_DISABLE Multi-core Turbo Disable
    UINT32 PL2_SAFETY_NET_ENABLE:1;  // Bits[1], default value is 1
    UINT32 DisPowerBalancing:1;      // Bits[2], Efficient Power Balancing Disable
    UINT32 DisFavouriteCore:1;       // Bits[3], Favourite Core Disable
    UINT32 rsvd_4_7:4;               // Bits[4:7], Reserved
    UINT32 DisAPSrocketing:1;        // Bits[8], APS rocketing disable
    UINT32 DisScalabilityCore:1;     // Bits[9], Scalability Core disable
    UINT32 DisPpoBudget:1;           // Bits[10], PPO-Budget disable (=RAPL Prioritization)
    UINT32 DisableEnhancedPmax:1;    // Bits[11], Enhanced PMAX detector is disabled.
    UINT32 rsvd_12:1;                // Bits[12], Reserved
    UINT32 AcpCtrl:1;                // Bits[13], Control ACP
    UINT32 rsvd_14_18:5;             // Bits[14:18], Reserved
    UINT32 DisPcodeRCOMP:2;          // Bits[19:20], Disable Pcode RCOMP
    UINT32 EnLegacyAVXTurbo:1;       // Bits[21], Legacy AVX turbo ratio limit mode enable (AVX limits used for all cores if any core is in AVX)
    UINT32 DisIioDynamicL1:1;        // Bits[22], IIO Dynamic L1 disable for Pkg C-states
    UINT32 DisPmaxCircuit:1;         // Bits[23], Disable Pmax circuit (once disabled cannot be re-enabled)
    UINT32 DisTSODPollingTimeout:1;  // Bits[24], TSOD Polling Timeout Disable
    UINT32 LimitPATo46:1;            // Bits[25], Limit PA to 46 bits
    UINT32 E2E_PARITY_ENABLE:1;      // Bits[26], E2E_PARITY_ENABLE
    UINT32 DisPcieDramHotPlug:1;     // Bits[27], PCIE/DRAM HOT PLUG DISABLE
    UINT32 DisUFS:1;                 // Bits[28], UFS DISABLE
    UINT32 rsvd_29_29:1;             // Bits[29], Reserved
    UINT32 EnSinglePowerDomain:1;    // Bits[30], Single Power Domain (SPD) enable
    UINT32 rsvd_31_31:1;             // Bits[31], Reserved
  } Bits;
  UINT32 Data;

} B2P_PCU_MISC_CONFIG_DATA_STRUCT;

typedef union {
  struct {
    UINT32 command:8;           // Bits[7:0]
    UINT32 instance_type:4;     // Bits[11:8]
    UINT32 logical_ip_type:8;   // Bits[19:12]
    UINT32 addrreserved:9;      // Bits[28:20]
    UINT32 rsvd_30_29:2;        // Bits[30:29]
    UINT32 RunBusy:1;           // Bit[31]
  } Bits;
  UINT32 Data;

} B2P_WRITE_PKGC_SA_PS_CRITERIA_INTERFACE_STRUCT;


typedef union {
  struct {
    UINT32 bios_pkgc_allowed_ps_mask:8;    // Bits[7:0]
    UINT32 bios_pkgc_criteria_ps_mask:8;   // Bits[15:8]
    UINT32 rsvd_30_16:16;                  // Bits[31:16]
  } Bits;
  UINT32 Data;

} B2P_WRITE_PKGC_SA_PS_CRITERIA_DATA_STRUCT;

typedef union {
  struct {
    UINT32 Mcp_Power_Limit:15;  // Bits[14:0], If write, MCP power limit in Watts in 12.3 format If read, reserved
    UINT32 Enable:1;            // Bits[15], 0: read; 1: write
    UINT32 Clamping:1;          // Bits[16], 0: Turbo range; 1: entire range. If read, reserved
    UINT32 time_contant:7;      // Bits[23:17], format in xxyyyyy; If read, reserved
    UINT32 rsvd_31_24:8;        // Bits[31:24], Reserved
  } Bits;
  UINT32 Data;

} B2P_XEON_POWER_LIMIT_DATA_STRUCT;

typedef union {
  struct {
    UINT32 Mcp_Power_Limit:15;  // Bits[14:0], If read, MCP power limit in Watts in 12.3 format If write, reserved
    UINT32 Enable:1;            // Bits[15], 0: read; 1: write
    UINT32 Clamping:1;          // Bits[16], 0: Turbo range; 1: entire range. If read, reserved
    UINT32 time_contant:7;      // Bits[23:17], format in xxyyyyy; If write, reserved
    UINT32 rsvd_31_24:8;        // Bits[31:24], Reserved
  }Bits;
  UINT32 Data;

} P2B_XEON_POWER_LIMIT_DATA_STRUCT;

typedef union {
  struct {
    UINT32 Core_Ratio:8;        // Bits[7:0]
    UINT32 Mesh_Ratio:8;        // Bits[15:8]
    UINT32 rsvd_31_16:16;       // Bits[31:16], Reserved
  } Bits;
  UINT32 Data;

} B2P_SET_CORE_MESH_RATIO_DATA_STRUCT;

typedef union {
  struct {
    UINT32 Default_Epp:8;       // Bits[7:0]
    UINT32 rsvd_31_8:24;        // Bits[31:8], Reserved
  } Bits;
  UINT32 Data;

} B2P_OOB_INIT_EPP_DATA_STRUCT;

typedef union {
  struct {
    UINT32 WA_Index:8;          // Bits[7:0]
    UINT32 WA_Param1:8;         // Bits[15:8]
    UINT32 rsvd_31_16:16;       // Bits[31:16], Reserved
  } Bits;
  UINT32 Data;

} B2P_MISC_WORKAROUND_ENABLE_DATA_STRUCT;

typedef union {
  struct {
    UINT32 command:8;           // Bits[7:0]
    UINT32 addrreserved:21;     // Bits[8:28]
    UINT32 rsvd_30_29:2;        // Bits[30:29]
    UINT32 RunBusy:1;           // Bit[31]
  } Bits;
  UINT32 Data;

} B2P_EPB_PECI_OVERRIDE_CONTROL_INTERFACE_STRUCT;

typedef union {
  struct {
    UINT32 EPB:1;               // Bits[0]
    UINT32 rsvd_31_1:31;        // Bits[31:1], Reserved
  } Bits;
  UINT32 Data;

} B2P_EPB_PECI_OVERRIDE_CONTROL_DATA_STRUCT;

typedef union {
  struct {
    UINT32 offset:7;            // Bits[6:0]
    UINT32 rsvd_7:1;            // Bits[7], Reserved
    UINT32 rsvd_30_8:23;        // Bits[30:8], Unused
    UINT32 lock:1;              // Bit[31]
  } Bits;
  UINT32 Data;

} B2P_PMAX_CONFIG_DATA_STRUCT;

typedef union {
  struct {
    UINT32 command    :8;          // Bits[7:0]
    UINT32 SubCommand :8;          // Bits[15:8]
    UINT32 rsvd_23_16 :8;          // Bits[23:16]
    UINT32 read_write :1;          // Bits[24], 0: read; 1: write
    UINT32 rsvd_28_25 :4;          // Bits[28:25]
    UINT32 rsvd_30_29 :2;          // Bits[30:29]
    UINT32 RunBusy    :1;          // Bit[31]
  } Bits;
  UINT32 Data;
} B2P_THERTMAL_REPORT_INTERFACE_STRUCT;

typedef union {
  struct {
    UINT32 WindowSize  :7;          // Bits[6:0], in TAU
    UINT32 Enable_EWMA :1;          // Bits[7:7]
    UINT32 rsvd_28_16  :24;         // Bits[31:8]
  } Bits;
  UINT32 Data;
} B2P_THERTMAL_REPORT_DATA_STRUCT;

//
// Structures that define input/output to/from config TDP
//  B2P commands and subcommands
//
typedef union {
  struct {
    UINT32    Command     : 8;
    UINT32    SubCommand  : 8;
    UINT32    Reserved    : 16;
  } Bits;
  UINT32 Data;
} B2P_SEND_CONFIG_TDP_SUBCOMMAND;

typedef union {
  struct {
    UINT32    ConfigIndex : 8;
    UINT32    MaskIndex   : 8;
    UINT32    Reserved    : 16;
  } Bits;
  UINT32 Data;
} B2P_SEND_SUBCOMMAND_DATA_GET_CORE_MASK;

typedef union {
  struct {
    UINT32    Version         : 8;
    UINT32    MaxTdpLevel     : 8;
    UINT32    CurrentTdpLevel : 8;
    UINT32    Reserved        : 7;
    UINT32    Enabled         : 1;
  } Bits;
  UINT32 Data;
} B2P_GET_LEVELS_INFO_OUTPUT_STRUCT;

typedef union {
  struct {
    UINT32    PkgTdpThisLevel : 15;
    UINT32    Reserved1       : 1;
    UINT32    RatioThisLevel  : 8;
    UINT32    Reserved2       : 8;
  } Bits;
  UINT32 Data;
} B2P_GET_TDP_INFO_OUTPUT_STRUCT;

typedef union {
  struct {
    UINT32    MaxPkgPwrThisLevel : 15;
    UINT32    Reserved15         : 1;
    UINT32    MinPkgPwrThisLevel : 15;
    UINT32    Reserved31         : 1;
  } Bits;
  UINT32 Data;
} B2P_GET_POWER_INFO_OUTPUT_STRUCT;

typedef union {
  struct {
    UINT32    MaxJunctionTempThisLevel  : 8;
    UINT32    MinLicenseThisLevel       : 5;
    UINT32    Reserved                  : 18;
    UINT32    FactConfigThisLevel       : 1;
  } Bits;
  UINT32 Data;
} B2P_GET_ICCP_TJMAX_OUTPUT_STRUCT;

typedef union {
  struct {
    UINT32 SstCpCapability :1;          // Bits[0] - 1:SST-CP is supported in the HW. 0: SST-CP is not supported in the HW
    UINT32 rsvd_15_1       :15;         // Bits[15:1]
    UINT32 SstCpState      :1;          // Bits[16] - 1:SST-CP is enabled. 0: SST-CP is disabled
    UINT32 rsvd_31_17      :15;         // Bits[31:17]
  } Bits;
  UINT32 Data;
} B2P_READ_PM_CONFIG_OUTPUT_STRUCT;

typedef union {
  struct {
    UINT32 command    :8;          // Bits[7:0]
    UINT32 SubCommand :8;          // Bits[15:8]
    UINT32 rsvd_28_16 :13;         // Bits[28:16]
    UINT32 rsvd_30_29 :2;          // Bits[30:29]
    UINT32 RunBusy    :1;          // Bit[31]
  } Bits;
  UINT32 Data;
} B2P_READ_WRITE_PM_CONFIG_INTERFACE_STRUCT;

typedef union {
  struct {
    UINT32 LimcaDisable:1;        // Bits[0]
    UINT32 PureCurrentEnable:1;   // Bits[1]
    UINT32 RememberCdyn:1;        // Bits[2]
    UINT32 SnapDown:1;            // Bits[3]
    UINT32 rsvd_31_4:28;          // Bits[31:4], Unused

  } Bits;
  UINT32 Data;

} B2P_WRITE_LIMCA_CONFIG_DATA_STRUCT;

typedef union {
  struct {
    UINT32    SseP1: 8;          // Bits[7:0] - Non AVX P1 ratio for this config level
    UINT32    Avx2P1: 8;         // Bits[15:8] - AVX2 P1 ratio for this config level
    UINT32    Avx3P1: 8;         // Bits[23:16] - AVX3 P1 ratio for this config level
    UINT32    Reserved: 8;       // Bits[31:24]
  } Bits;
  UINT32 Data;

} B2P_CONFIG_TDP_GET_P1_INFO_DATA_STRUCT;

typedef union {
  struct {
    UINT32    UncoreP0: 8;       // Bits[7:0] - Max ratio limit for Uncore
    UINT32    UncoreP1: 8;       // Bits[15:8] - Min ratio limit for Uncore
    UINT32    Reserved:16;       // Bits[31:16]
  } Bits;
  UINT32 Data;

} B2P_CONFIG_TDP_GET_UNCORE_P0_P1_INFO_DATA_STRUCT;

typedef union {
  struct {
    UINT32    ConfigIndex : 8;
    UINT32    WordIndex   : 8;
    UINT32    AvxIndex    : 8;
    UINT32    Reserved    : 8;
  } Bits;
  UINT32 Data;
} B2P_SEND_SUBCOMMAND_DATA_GET_TURBO_LIMIT;

typedef union {
  struct {
    UINT32 Command    :8;          // Bits[7:0]
    UINT32 SubCommand :8;          // Bits[15:8]
    UINT32 PhyVrAddr  :5;          // Bits[20:16]
    UINT32 Reserved   :10;         // Bits[30:21]
    UINT32 RunBusy    :1;          // Bit[31]
  } Bits;
  UINT32 Data;
} B2P_VR_HANDLER_SUBCOMMAND_STRUCT;

typedef union {
  struct {
    UINT32    FactSupport :1;       // Bits[0]
    UINT32    PbfSupport  :1;       // Bits[1]
    UINT32    Reserved_1  :14;      // Bits[15:2]
    UINT32    FactEnabled :1;       // Bits[16]
    UINT32    PbfEnabled  :1;       // Bits[17]
    UINT32    Reserved_2  :14;      // Bits[31:18]
  } Bits;
  UINT32 Data;
} B2P_CONFIG_TDP_GET_CONFIG_TDP_CONTROL_DATA_STRUCT;

typedef union {
  struct {
    UINT32    Reserved_1   :16;      // Bits[15:0]
    UINT32    FactEnable   :1;       // Bits[16]
    UINT32    PbfEnable    :1;       // Bits[17]
    UINT32    Reserved_2   :14;      // Bits[31:18]
  } Bits;
  UINT32 Data;
} B2P_CONFIG_TDP_SET_CONFIG_TDP_CONTROL_DATA_STRUCT;

typedef union {
  struct {
    UINT32    ConfigIndex : 8;
    UINT32    MaskIndex   : 8;
    UINT32    Reserved    : 16;
  } Bits;
  UINT32 Data;
} B2P_SEND_SUBCOMMAND_DATA_PBF_GET_CORE_MASK_INFO;

typedef union {
  struct {
    UINT32    P1Lo: 8;           // Bits[7:0]  - base frequency for low priority cores
    UINT32    P1Hi: 8;           // Bits[15:8] - base frequency for high priority cores
    UINT32    Reserved: 16;      // Bits[31:16]
  } Bits;
  UINT32 Data;

} B2P_CONFIG_TDP_PBF_GET_P1HI_P1LO_INFO_DATA_STRUCT;

typedef union {
  struct {
    UINT32 Command:8;                     // Bits[7:0]
    UINT32 ResetTscOnWarmReset:1;         // Bits[8:8]
    UINT32 Reserved:22;                   // Bits[30:9]
    UINT32 RunBusy:1;                     // Bit[31]
  } Bits;
  UINT32 Data;

} B2P_TSC_RESET_ENABLE_INTERFACE_STRUCT;

typedef union {
  struct {
    UINT32    NodeIdMask :8;          // Bits[7:0] - Node ID Mask of Sockets that are part of this Platform Psys Domain
    UINT32    Reserved :23;           // Bits[30:8]
    UINT32    Master :1;              // Bits[31] - MASTER - Indicates this socket is a psys master. 1 = Master, 0 = Not-Master
  } Bits;
  UINT32 Data;

} B2P_PSYS_CONFIG_DATA_STRUCT;

typedef union {
  struct {
    UINT32    Command :8;            // Bits[7:0]
    UINT32    VrId :5;               // Bits[12:8]
    UINT32    Address :16;           // Bits[28:13]
    UINT32    Reserved :2;           // Bits[30:29]
    UINT32    RunBusy :1;            // Bit[31]
  } Bits;
  UINT32 Data;
} B2P_VR_INTERFACE_CMD_STRUCT;

typedef union {
  struct {
    UINT32    VrCmd :5;              // Bits[4:0]
    UINT32    Reserved :3;           // Bits[7:5]
    UINT32    VidRegData :8;         // Bits[15:8]
    UINT32    RegAddress :8;         // Bits[23:16]
    UINT32    Reserved2 :7;          // Bits[30:24]
    UINT32    Lock :1;               // Bit[31]
  } Bits;
  UINT32 Data;
} B2P_VR_INTERFACE_DATA_STRUCT;

typedef union {
  struct {
    UINT32    ConfigLevel :8;            // Bits[7:0]
    UINT32    Reserved1   :8;            // Bits[15:8]
    UINT32    Uncore      :1;            // Bits[16] - 0: Core, 1: Uncore
    UINT32    Reserved2   :15;           // Bits[31:17]
  } Bits;
  UINT32 Data;
} B2P_CONFIG_TDP_GET_RATIO_INFO_CMD_DATA_STRUCT;

typedef union {
  struct {
    UINT32    P0: 8;       // Bits[7:0]   - P0 Maximum ratio limit
    UINT32    P1: 8;       // Bits[15:8]  - P1 Guaranteed ratio
    UINT32    Pn: 8;       // Bits[23:16] - Pn Maximum efficiency ratio
    UINT32    Pm: 8;       // Bits[31:24] - Pm Minimum ratio
  } Bits;
  UINT32 Data;
} B2P_CONFIG_TDP_GET_RATIO_INFO_RETURN_DATA_STRUCT;

//
// B2P commands used for config TDP / ISS for
//  SkyLake / CascadeLake SKUs
// *Not* defined in PcuMailBoxSkx.h, furthermore
//  these commands *replace* 3 commands in that file
//
#define MAILBOX_BIOS_CMD_CONFIG_TDP_GET_LEVELS_INFO     0xC0
#define  N_GET_LEVELS_INFO_CONFIG_TDP_LEVELS_SHIFT      8
#define  B_GET_LEVELS_INFO_CONFIG_TDP_LEVELS_MASK       0xFF
#define  N_GET_LEVELS_INFO_TDP_LEVEL_SHIFT              16
#define  B_GET_LEVELS_INFO_TDP_LEVEL_MASK               0xFF

#define MAILBOX_BIOS_CMD_CONFIG_TDP_GET_TDP_INFO        0xC1
#define  B_GET_TDP_INFO_PKG_TDP_MASK                    0x7FFF
#define  N_GET_TDP_INFO_TDP_RATIO_SHIFT                 16
#define  B_GET_TDP_INFO_TDP_RATIO_MASK                  0xFF

#define MAILBOX_BIOS_CMD_CONFIG_TDP_GET_POWER_INFO      0xC2
#define  B_GET_POWER_INFO_MIN_MAX_MASK                  0x7FFF7FFF

#define MAILBOX_BIOS_CMD_CONFIG_TDP_GET_CORE_COUNT      0xC3
#define  B_GET_CORE_COUNT_MASK                          0x1F

#define MAILBOX_BIOS_CMD_CONFIG_TDP_ACTIVATE_LEVEL      0xC4
#define  B_ACTIVATE_LEVEL_MASK                          0xFF

#define MAILBOX_BIOS_CMD_SET_MIN_ICCP_LICENSE           0xC5   // CLX/CPX

#define  B_GET_CORE_MASK_INDEX_SHIFT                    8
#define  B_GET_CORE_MASK_INDEX_MASK                     0xFF00

#define MAILBOX_BIOS_CMD_SET_GPSS_TIMER_SKX             0xC5
#define MAILBOX_BIOS_CMD_SET_GPSS_TIMER_CLX             0xCF
#define MAILBOX_BIOS_CMD_TSC_RESET_ENABLE               0x86

#define MAILBOX_BIOS_CMD_CONFIG_TDP_GET_P1_INFO         0xD0   // SKX/CLX

//MAILBOX_BIOS_CMD_CONFIG_TDP only for CPX
#define MAILBOX_BIOS_CMD_CONFIG_TDP                                                                     0x7f
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_LSB                                           0x8
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_MSB                                           0xf
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_RANGE                                         0x8
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_MASK                                          0xff00
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_LEVELS_INFO                               0x0
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CONFIG_TDP_CONTROL                        0x1
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_CONFIG_TDP_CONTROL                        0x2
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TDP_INFO                                  0x3
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_PWR_INFO                                  0x4
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_ICCP_TJMAX_INFO                           0x5
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_CORE_MASK                                 0x6
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_TURBO_LIMIT                               0x7
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_SET_LEVEL                                     0x8
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_UNCORE_P0_P1_INFO                         0x9
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_P1_INFO                                   0xa
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_MEM_FREQ                                  0xb
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_RATIO_INFO                                0xc
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_FACT_HP_TURBO_LIMIT_NUMCORES              0x10
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_FACT_HP_TURBO_LIMIT_RATIOS                0x11
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_GET_FACT_LP_CLIPPING_RATIO                    0x12
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_CORE_MASK_INFO                        0x20
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_P1HI_P1LO_INFO                        0x21
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_TJ_MAX_INFO                           0x22
#define MAILBOX_BIOS_CMD_CONFIG_TDP_INTERFACE_SUB_COMMAND_PBF_GET_TDP_INFO                              0x23
//

//MAILBOX_BIOS_CMD_READ_PM_CONFIG                                                                0x94
#define MAILBOX_BIOS_CMD_RW_PM_CONFIG_INTERFACE_SUB_COMMAND_PM_FEATURE                            0x3


//#define DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG 0x842200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti_apm_override_enable : 1;

                            /* Bits[0:0], Access Type=RW_V, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 rsvd_1 : 4;

                            /* Bits[4:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 iom_apm_override_enable : 1;

                            /* Bits[5:5], Access Type=RW_V, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 rsvd_6 : 4;

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 imc_apm_override_enable : 1;

                            /* Bits[10:10], Access Type=RW_V, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 rsvd_11 : 4;

                            /* Bits[14:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 io_bw_plimit_override_enable : 1;

                            /* Bits[15:15], Access Type=RW_V, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 allow_peci_pcode_error_rsp : 1;

                            /* Bits[16:16], Access Type=RWS_V, default=0x00000001*/

                            /*
                               allow pcode to attempt to respond to peci
                               requests after a pcode MCA
                            */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 eet_override_enable : 1;

                            /* Bits[18:18], Access Type=RW_V, default=0x00000000*/

                            /*  */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RW_V, default=0x00000000*/

                            /* Reserved - We want the Flops implemented. */

    UINT32 uncore_perf_plimit_override_enable : 1;

                            /* Bits[20:20], Access Type=RW_V, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 rsvd_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 turbo_demotion_override_enable : 1;

                            /* Bits[22:22], Access Type=RW_V, default=0x00000000*/

                            /* 0 - Disable override 1- Enable override */
    UINT32 cst_demotion_override_enable : 1;

                            /* Bits[23:23], Access Type=RW_V, default=0x00000000*/

                            /* 0 - Disable override 1- Enable override */
    UINT32 i_turbo_override_enable : 1;

                            /* Bits[24:24], Access Type=RW_V, default=0x00000000*/

                            /* 0 - Disable override 1- Enable override */
    UINT32 eep_l_override_enable : 1;

                            /* Bits[25:25], Access Type=RW_V, default=0x00000000*/

                            /* 0 disable over ride 1 enable over ride */
    UINT32 eep_l_override : 4;

                            /* Bits[29:26], Access Type=RW_V, default=0x00000000*/

                            /* This indicates a EEP L override. Value 0-15 */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RW_V, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DYNAMIC_PERF_POWER_CTL_PCU_FUN2_STRUCT_SKX;

#ifndef SAPMCTL_PCU_FUN1_REG_10NM
#define SAPMCTL_PCU_FUN1_REG_10NM 0x041200B0
#endif //#ifndef SAPMCTL_PCU_FUN1_REG

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iio0_pkgc_clock_gate_disable : 1;

                            /* Bits[0:0], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, disables clock gating of IIO during
                               package-c.
                            */
    UINT32 iio1_pkgc_clock_gate_disable : 1;

                            /* Bits[1:1], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, disables clock gating of IIO during
                               package-c.
                            */
    UINT32 iio2_pkgc_clock_gate_disable : 1;

                            /* Bits[2:2], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, disables clock gating of IIO during
                               package-c.
                            */
    UINT32 kti01_pkgc_clock_gate_disable : 1;

                            /* Bits[3:3], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, disables clock gating of Intel UPI
                               0/1 during package-c.
                            */
    UINT32 kti23_pkgc_clock_gate_disable : 1;

                            /* Bits[4:4], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, disables clock gating of Intel UPI
                               2/3 during package-c.
                            */

    UINT32 kti45_pkgc_clock_gate_disable : 1;

                            /* Bits[5:5], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, disables clock gating of Intel UPI
                               4/5 during package-c.
                            */

    UINT32 mc0_pkgc_clk_gate_disable : 1;

                            /* Bits[6:6], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, tells pcode to disallow clock
                               gating in MC0 when in the PKG-C6 state
                            */
    UINT32 mc1_pkgc_clk_gate_disable : 1;

                            /* Bits[7:7], Access Type=RW_L, default=0x00000000*/

                            /*
                               If set to 1, tells pcode to disallow clock
                               gating in MC1 when in the PKG-C6 state
                            */

    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RW_L, default=0x00000001*/

                            /* Reserved */
    UINT32 iio0_pll_off_enable : 1;

                            /* Bits[16:16], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               PCIe/DMI PLL. 0b PLL shutdown is not allowed 1b
                               PLL shutdown is allowed
                            */
    UINT32 iio1_pll_off_enable : 1;

                            /* Bits[17:17], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               PCIe/DMI PLL. 0b PLL shutdown is not allowed 1b
                               PLL shutdown is allowed
                            */
    UINT32 iio2_pll_off_enable : 1;

                            /* Bits[18:18], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               PCIe/DMI PLL. 0b PLL shutdown is not allowed 1b
                               PLL shutdown is allowed
                            */
    UINT32 kti01_pll_off_enable : 1;

                            /* Bits[19:19], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               Intel UPI 0/1 PLL. 0b PLL shutdown is not
                               allowed 1b PLL shutdown is allowed
                            */
    UINT32 kti23_pll_off_enable : 1;

                            /* Bits[20:20], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               Intel UPI 2/3 PLL. 0b PLL shutdown is not
                               allowed 1b PLL shutdown is allowed
                            */
    UINT32 kti45_pll_off_enable : 1;

                            /* Bits[21:21], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               Intel UPI 4/5 PLL. 0b PLL shutdown is not
                               allowed 1b PLL shutdown is allowed
                            */
    UINT32 mc0_pll_off_enable : 1;

                            /* Bits[22:22], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               Memory Controller PLLs (MCPLL). 0b PLL shutdown
                               is not allowed 1b PLL shutdown is allowed
                            */
    UINT32 mc1_pll_off_enable : 1;

                            /* Bits[23:23], Access Type=RW_L, default=0x00000001*/

                            /*
                               This bit is used to enable shutting down the
                               Memory Controller PLLs (MCPLL). 0b PLL shutdown
                               is not allowed 1b PLL shutdown is allowed
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RW_L, default=0x00000000*/

                            /*
                               Reserved for future use; this bit is lockable,
                               resets to 0
                            */
    UINT32 rsvd_25 : 1;

                            /* Bits[25:25], Access Type=RW_L, default=0x00000000*/

                            /*
                               Reserved for future use; this bit is lockable,
                               resets to 0
                            */
    UINT32 rsvd_26 : 1;

                            /* Bits[26:26], Access Type=RW_L, default=0x00000000*/

                            /*
                               Reserved for future use, this bit is lockable,
                               resets to 0
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RW_L, default=0x00000001*/

                            /*
                               Reserved for future use, this bit is lockable,
                               resets to 1
                            */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RW_L, default=0x00000001*/

                            /*
                               Reserved for future use, this bit is lockable,
                               resets to 1
                            */
    UINT32 rsvd_29 : 1;

                            /* Bits[29:29], Access Type=RW_L, default=0x00000001*/

                            /*
                               Reserved for future use, this bit is lockable,
                               resets to 1
                            */

    UINT32 setvid_decay_disable : 1;

                            /* Bits[30:30], Access Type=RW_L, default=0x00000000*/

                            /*
                               This bit is used by BIOS to disable SETVID Decay
                               to enable use of VR12 designs that do not
                               support decay function. 0 - Enable Decay
                               (Default) 1 - Disable Decay
                            */
    UINT32 lock : 1;

                            /* Bits[31:31], Access Type=RW_KL, default=0x00000000*/

                            /*
                               When set to 1b this bit locks various PM
                               registers.
                            */

  } Bits;
  UINT32 Data;

} SAPMCTL_PCU_FUN1_STRUCTURE;

#endif  //_CPU_BIOS_TO_PCODE_H_
