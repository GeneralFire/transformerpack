/** @file

  Reference Code Fatal Error Header

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _REFERENCE_CODE_FATAL_ERRORS_H_
#define _REFERENCE_CODE_FATAL_ERRORS_H_

#include <Library/MemoryServicesLib.h>

//
// Conditional print/fatal error macro
//
#define RC_FATAL_ERROR(Expression, MajorCode, MinorCode) \
  { \
    if (!(Expression)) { \
      DEBUG ((DEBUG_ERROR, "\n\nRC_FATAL_ERROR! %a: %u", __FILE__, __LINE__)); \
      FatalError (MajorCode, MinorCode); \
    } \
  }

//
// Fatal error codes for use with RC_FATAL_ERROR. Need to
// clean this up, remove interleaving or major and minor,
// and align names.
//

  #define  ERR_SPD_DECODE                    0xE0
  #define  ERR_RC_DCA_DFE                    0xE6
  #define   ERR_INVALID_SIGNAL                          0x01
  #define  ERR_RC_SWEEP_LIB_INTERNAL         0xE7
  #define  ERR_NO_MEMORY                     0xE8
  #define  ERR_NO_MEMORY_MINOR_NO_MEMORY                0x01
  #define  ERR_NO_MEMORY_MINOR_ALL_CH_DISABLED          0x02
  #define  ERR_NO_MEMORY_MINOR_ALL_CH_DISABLED_MIXED    0x03
  #define  ERR_LT_LOCK                       0xE9
  #define  ERR_DDR_INIT                      0xEA
  #define  ERR_RD_DQ_DQS                                0x01
  #define  ERR_RC_EN                                    0x02
  #define  ERR_WR_LEVEL                                 0x03
  #define  ERR_WR_DQ_DQS                                0x04
  #define  ERR_TX_RETRAIN_CAP                           0x05
  #define  ERR_MEM_TEST                      0xEB
  #define  ERR_MEM_TEST_MINOR_SOFTWARE                  0x01
  #define  ERR_MEM_TEST_MINOR_HARDWARE                  0x02
  #define  ERR_MEM_TEST_MINOR_LOCKSTEP_MODE             0x03
  #define  ERR_VENDOR_SPECIFIC               0xEC
  #define ERR_DIMM_PLL_LOCK_ERROR                       0x01
  #define  ERR_DIMM_COMPAT                   0xED
  #define  ERR_MIXED_MEM_TYPE                           0x01
  #define  ERR_INVALID_POP                              0x02
  #define  ERR_INVALID_POP_MINOR_QR_AND_3RD_SLOT        0x03
  #define  ERR_INVALID_POP_MINOR_UDIMM_AND_3RD_SLOT     0x04
  #define  ERR_INVALID_POP_MINOR_UNSUPPORTED_VOLTAGE    0x05
  #define  ERR_DDR3_DDR4_MIXED                          0x06
  #define  ERR_MIXED_SPD_TYPE                           0x07
  #define  ERR_MISMATCH_DIMM_TYPE                       0x08
  #define  ERR_INVALID_DDR4_SPD_CONTENT                 0x09
  #define  ERR_SMBUS_READ_FAILURE                       0x0A
  #define  ERR_MIXED_MEM_AEP_AND_UDIMM                  0x0B
  #define  ERR_NVMDIMM_NOT_SUPPORTED                    0x0C
  #define  ERR_CPU_CAP_NVMDIMM_NOT_SUPPORTED            0x0D
  #define  ERR_MRC_COMPATIBILITY             0XEE
  #define  ERR_MRC_DIR_NONECC                           0X01
  #define  ERR_MRC_STRUCT                    0xEF
  #define   ERR_INVALID_BOOT_MODE                       0x01
  #define   ERR_INVALID_SUB_BOOT_MODE                   0x02
  #define   ERR_INVALID_HOST_ADDR                       0x03
  #define   ERR_ARRAY_OUT_OF_BOUNDS                     0x04
  #define   ERR_IMC_NUMBER_EXCEEDED                     0x05
  #define   ERR_ODT_STRUCT                              0x06
  #define ERR_SET_VDD                        0xF0
  #define ERR_UNKNOWN_VR_MODE                           0x01
  #define ERR_IOT_MEM_BUFFER                 0xF1
  #define ERR_RC_INTERNAL                    0xF2
  #define ERR_RC_INTERNAL_HBM                           0x01
  #define ERR_INVALID_REG_ACCESS             0xF3
  #define ERR_INVALID_WRITE_REG_BDF                     0x01
  #define ERR_INVALID_WRITE_REG_OFFSET                  0x02
  #define ERR_INVALID_READ_REG_BDF                      0x03
  #define ERR_INVALID_READ_REG_OFFSET                   0x04
  #define ERR_INVALID_WRITE_REG_SIZE                    0x05
  #define ERR_INVALID_READ_REG_SIZE                     0x06
  #define ERR_UNKNOWN_REG_TYPE                          0x07
  #define ERR_INVALID_ACCESS_METHOD                     0x08
  #define ERR_INVALID_BIT_ACCESS                        0x09
  #define ERR_SET_MC_FREQ                    0xF4
  #define ERR_UNSUPPORTED_MC_FREQ                       0x01
  #define ERR_UNSPECIFIED_MC_FREQ_SETTING_ERROR         0x02
  #define ERR_READ_MC_FREQ                   0xF5
  #define ERR_NOT_ABLE_READ_MC_FREQ                     0x01
  #define ERR_DIMM_CHANNEL                   0x70
  #define ERR_BIST_CHECK                     0x74
  #define ERR_SMBUS                          0xF6
  #define TSOD_POLLING_ENABLED_READ                     0x01
  #define TSOD_POLLING_ENABLED_WRITE                    0x02
  #define ERR_LRDIMM_SMBUS_READ_FAILURE                 0x03

 #define ERR_PCU                            0xF7
 #define PCU_NOT_RESPONDING                            0x01
 #define FUSE_ERROR                                    0x02
 #define ERR_PCU_COMMAND_NOT_SUPPORTED                 0x03
#define ERR_NGN                            0xF8
 #define NGN_DRIVER_NOT_RESPONSIBLE                    0x01
 #define NGN_ARRAY_OUT_OF_BOUNDS                       0x02
 #define NGN_PMEM_CONFIG_ERROR                         0x03
 #define INTERLEAVE_EXCEED                             0x04
 #define NGN_BYTES_MISMATCH                            0x05
 #define NGN_SKU_MISMATCH                              0x06
 #define ERR_INTERLEAVE_FAILURE             0xF9
 #define  ERR_RIR_RULES_EXCEEDED                       0x01

 #define ERR_SKU_LIMIT                      0xFA
 #define  ERR_SKU_LIMIT_VIOLATION                      0x01

 #define ERR_CAR_LIMIT                      0xFB
 #define  ERR_OUT_OF_CAR_RESOURCES                     0x01
 #define PRINTF_OUTOF_SYNC_ERR_MAJOR        0xCF
 #define PRINTF_OUTOF_SYNC_ERR_MINOR                   0x01
 #define ERR_CMI_FAILURE                    0xFC
 #define ERR_CMI_INIT_FAILED                           0x01
 #define ERR_VALUE_OUT_OF_RANGE             0xFD
 #define ERR_VALUE_BELOW_MIN                           0x01
 #define ERR_VALUE_ABOVE_MAX                           0x02
#define ERR_DDRIO_HWFSM                     0xFE
 #define ERR_XOVER_HWFSM_TIMEOUT                       0x01
 #define ERR_XOVER_HWFSM_FAILURE                       0x02
 #define ERR_SENSEAMP_HWFSM_TIMEOUT                    0x03
 #define ERR_SENSEAMP_HWFSM_FAILURE                    0x04
#define ERR_MRC_POINTER                     0xFF

//
// Minor Warning codes for use with RC_FATAL_ERROR
//

typedef enum {
  RC_FATAL_ERROR_MINOR_CODE_0 = 0,
  RC_FATAL_ERROR_MINOR_CODE_1 = 1,      // Failure in InitCpuNvram call
  RC_FATAL_ERROR_MINOR_CODE_2 = 2,      // Invalid Strobe parameter into GetSetSenseAmpRxOffsetDdrio
  RC_FATAL_ERROR_MINOR_CODE_3 = 3,      // Sim register read compare error
  RC_FATAL_ERROR_MINOR_CODE_4 = 4,      // Sim register read compare error
  RC_FATAL_ERROR_MINOR_CODE_5 = 5,      // USRA Invalid Register Size
  RC_FATAL_ERROR_MINOR_CODE_6 = 6,      // Refresh engine is not enabled on S3 exit
  RC_FATAL_ERROR_MINOR_CODE_7 = 7,      // USRA Invalid Register Size
  RC_FATAL_ERROR_MINOR_CODE_8 = 8,      // Unsupported function call in MemProjectCommon.c
  RC_FATAL_ERROR_MINOR_CODE_9 = 9,      // Sim register write compare error
  RC_FATAL_ERROR_MINOR_CODE_10 = 10,    // Sim register write compare error
  RC_FATAL_ERROR_MINOR_CODE_11 = 11,    // Invalid Bus type for DDRT Sim read
  RC_FATAL_ERROR_MINOR_CODE_12 = 12,    // Unable to find register struc for DDRT Sim read
  RC_FATAL_ERROR_MINOR_CODE_13 = 13,    // Unable to find register in BDF table for DDRT Sim read
  RC_FATAL_ERROR_MINOR_CODE_14 = 14,    // DDRT Sim register read compare error
  RC_FATAL_ERROR_MINOR_CODE_15 = 15,    // Invalid Bus type for DDRT Sim write
  RC_FATAL_ERROR_MINOR_CODE_16 = 16,    // Unable to find register struc for DDRT Sim write
  RC_FATAL_ERROR_MINOR_CODE_17 = 17,    // Unable to find register in BDF table for DDRT Sim write
  RC_FATAL_ERROR_MINOR_CODE_18 = 18,    // DDRT Sim register write compare error
  RC_FATAL_ERROR_MINOR_CODE_19 = 19,    // Unsupported function call in 10nm MemCpgc.c
  RC_FATAL_ERROR_MINOR_CODE_20 = 20,    // Unsupported function call in 14nm MemCpgc.c
  RC_FATAL_ERROR_MINOR_CODE_21 = 21,    // Unsupported function call in 14nm CpgcIpInterfaces.c
  RC_FATAL_ERROR_MINOR_CODE_22 = 22,    // Unsupported function call in 10nm MemChipDdrio.c
  RC_FATAL_ERROR_MINOR_CODE_23 = 23,    // Unsupported function call in 14nm MemChipDdrio.c
  RC_FATAL_ERROR_MINOR_CODE_24 = 24,    // Unsupported function call in 10nm MemConfigMc.c
  RC_FATAL_ERROR_MINOR_CODE_25 = 25,    // Unsupported function call in 14nm MemConfigMc.c
  RC_FATAL_ERROR_MINOR_CODE_26 = 26,    // Non-SKX CPU installed with SKX build
  RC_FATAL_ERROR_MINOR_CODE_27 = 27,    // Unsupported function call in 10nm MemMcChip.c
  RC_FATAL_ERROR_MINOR_CODE_28 = 28,    // Get SBSP id for legacy remote SBSP called before Host struc available
  RC_FATAL_ERROR_MINOR_CODE_29 = 29,    // AP Socket is trying to access another socket's SMBus
  RC_FATAL_ERROR_MINOR_CODE_30 = 30,    // Semaphore call depth rolled over to zero
  RC_FATAL_ERROR_MINOR_CODE_31 = 31,    // Invalid BoxType parameter into GetFnvCfgAddress
  RC_FATAL_ERROR_MINOR_CODE_32 = 32,    // Invalid interface parameter into GetFnvCfgAddress
  RC_FATAL_ERROR_MINOR_CODE_33 = 33,    // Invalid interface parameter into ReadFnvCfg
  RC_FATAL_ERROR_MINOR_CODE_34 = 34,    // Invalid interface parameter into WriteFnvCfg
  RC_FATAL_ERROR_MINOR_CODE_35 = 35,    // NULL pointer returned by GetValuesForCurrentConfigOdt()
  RC_FATAL_ERROR_MINOR_CODE_36 = 36,    // Invalid InterleaveWays value calculated
  RC_FATAL_ERROR_MINOR_CODE_37 = 37,    // Invalid control group specified
  RC_FATAL_ERROR_MINOR_CODE_38 = 38,    // Invalid side specified for IO group 6
  RC_FATAL_ERROR_MINOR_CODE_39 = 39,    // Invalid side specified for IO group 13
  RC_FATAL_ERROR_MINOR_CODE_40 = 40,    // Invalid IO group parameter into GetCmdGroupAddress
  RC_FATAL_ERROR_MINOR_CODE_41 = 41,    // Invalid IO group parameter into GetSetCmdDelay
  RC_FATAL_ERROR_MINOR_CODE_42 = 42,    // CTL Pi Group not found
  RC_FATAL_ERROR_MINOR_CODE_43 = 43,    // Unable to find CMD Pi group in MemCmdControl
  RC_FATAL_ERROR_MINOR_CODE_44 = 44,    // Unable to find CMD Pi group in MemCmdControl
  RC_FATAL_ERROR_MINOR_CODE_45 = 45,    // Attempting to clear TRNG_TARGET with DDRT present in MemCpgc
  RC_FATAL_ERROR_MINOR_CODE_46 = 46,    // Attempting to set TRNG_TARGET with no DDRT present in MemCpgc
  RC_FATAL_ERROR_MINOR_CODE_47 = 47,    // Invalid training pattern specified in MemCpgc
  RC_FATAL_ERROR_MINOR_CODE_48 = 48,    // Invalid PatternDepth parameter or NumCacheLines parameter into WriteWDBFixedPatternMATS
  RC_FATAL_ERROR_MINOR_CODE_49 = 49,    // A17 signal specified for SKX in CpgcCtlClkPattern
  RC_FATAL_ERROR_MINOR_CODE_50 = 50,    // Unsupported function call in 10nm MemTimingChip.c
  RC_FATAL_ERROR_MINOR_CODE_51 = 51,    // Invalid value for NumOfNibbles in ReadDqDqsCleanup
  RC_FATAL_ERROR_MINOR_CODE_52 = 52,    // DDR ODT table pointer not found
  RC_FATAL_ERROR_MINOR_CODE_53 = 53,    // Invalid group parameter into GetSetSenseAmpRxOffsetDdrio or GetSetDataGroup
  RC_FATAL_ERROR_MINOR_CODE_54 = 54,    // Invalid group parameter into GetSetTxDelayUnicast
  RC_FATAL_ERROR_MINOR_CODE_55 = 55,    // Invalid payload parameter into FnvDdrtIoInit
  RC_FATAL_ERROR_MINOR_CODE_56 = 56,    // Invalid bit parameter into GetSetTxDelayBit
  RC_FATAL_ERROR_MINOR_CODE_57 = 57,    // Insufficient range to handle wrap in MemIOControl
  RC_FATAL_ERROR_MINOR_CODE_58 = 58,    // Invalid group parameter into GetSetRxDelayUnicast for CSR read
  RC_FATAL_ERROR_MINOR_CODE_59 = 59,    // Invalid group parameter into GetSetRxDelayUnicast for cached read
  RC_FATAL_ERROR_MINOR_CODE_60 = 60,    // Invalid group parameter into GetSetRxDelayUnicast for LRDIMM
  RC_FATAL_ERROR_MINOR_CODE_61 = 61,    // Invalid bit parameter into GetSetRxDelayBit
  RC_FATAL_ERROR_MINOR_CODE_62 = 62,    // Invalid Invalid RTT_WRT ODT setting
  RC_FATAL_ERROR_MINOR_CODE_63 = 63,    // Invalid Rtt Wr value stored in Host
  RC_FATAL_ERROR_MINOR_CODE_64 = 64,    // Invalid ch parameter into ChipCtlClkFeedBack
  RC_FATAL_ERROR_MINOR_CODE_65 = 65,    // Write to DDR4 LRDIMM with no LRDIMM present
  RC_FATAL_ERROR_MINOR_CODE_66 = 66,    // Invalid testSize parameter into MemTestMATSN
  RC_FATAL_ERROR_MINOR_CODE_67 = 67,    // Invalid testSize parameter into MemTestMCH
  RC_FATAL_ERROR_MINOR_CODE_68 = 68,    // Invalid testSize parameter into MemTestGNDB
  RC_FATAL_ERROR_MINOR_CODE_69 = 69,    // Invalid testSize parameter into MemTestMARCHCM
  RC_FATAL_ERROR_MINOR_CODE_70 = 70,    // CTL_CLK_TEST_RANGE is evenly divisible by CTL_CLK_STEPSIZE
  RC_FATAL_ERROR_MINOR_CODE_71 = 71,    // CLK delay value out of range in GetSetCtlGroupDelayCore
  RC_FATAL_ERROR_MINOR_CODE_72 = 72,    // CLK delay value out of range in GetSetCmdGroupDelayCore
  RC_FATAL_ERROR_MINOR_CODE_73 = 73,    // CLK delay value out of range in GetSetClkDelayCore
  RC_FATAL_ERROR_MINOR_CODE_74 = 74,    // CLK delay value out of range in GetSetCmdVrefCore
  RC_FATAL_ERROR_MINOR_CODE_75 = 75,    // Invalid side parameter in WriteMrsDdr4 with DDRT present
  RC_FATAL_ERROR_MINOR_CODE_76 = 76,    // Invalid size parameter in WriteLrbufSmb
  RC_FATAL_ERROR_MINOR_CODE_77 = 77,    // Invalid size parameter in WriteLrbufSmb for CTLE
  RC_FATAL_ERROR_MINOR_CODE_78 = 78,    // Remote access to another socket's DIMM SPD in multi-threaded mode
  RC_FATAL_ERROR_MINOR_CODE_79 = 79,    // Invalid PostCodeMinor in PipeSync
  RC_FATAL_ERROR_MINOR_CODE_80 = 80,    // Invalid value in Host numRanks in GetCurrentDimmFrequencyAndPopulationOdt (from OdtTableLib)
  RC_FATAL_ERROR_MINOR_CODE_81 = 81,    // Invalid DIMM specified in GetCurrentDimmFrequencyAndPopulationOdt (from OdtTableLib)
  RC_FATAL_ERROR_MINOR_CODE_82 = 82,    // Invalid value in Host numRanks in GetActivationForCurrentConfigOdt
  RC_FATAL_ERROR_MINOR_CODE_83 = 83,    // Invalid DIMM specified in GetActivationForCurrentConfigOdt
  RC_FATAL_ERROR_MINOR_CODE_84 = 84,    // Invalid Bus Segement specified in ProcSmbChip
  RC_FATAL_ERROR_MINOR_CODE_85 = 85,    // Invalid SMBUS ClkFrequency Parameter in AdjustProcSmbClkPeriod
  RC_FATAL_ERROR_MINOR_CODE_86 = 86,    // CacheIoLatency - Invalid channel specified
  RC_FATAL_ERROR_MINOR_CODE_87 = 87,    // CacheRoundTrip - Invalid channel specified
  RC_FATAL_ERROR_MINOR_CODE_88 = 88,    // SetIoLatency - Invalid channel or rank specified
  RC_FATAL_ERROR_MINOR_CODE_89 = 89,    // GetIoLatency - Invalid channel or rank specified
  RC_FATAL_ERROR_MINOR_CODE_90 = 90,    // SetRoundTrip - Invalid channel or rank specified
  RC_FATAL_ERROR_MINOR_CODE_91 = 91,    // GetRoundTrip - Invalid channel or rank specified
  RC_FATAL_ERROR_MINOR_CODE_92 = 92,    // GetDataTimingRegOffset: Ch or SubCh out of bounds
  RC_FATAL_ERROR_MINOR_CODE_93 = 93,    // ProgramQcaDelay: Invalid SubCh
  RC_FATAL_ERROR_MINOR_CODE_94 = 94,    // GetChannelMapping - invalid parameter
  RC_FATAL_ERROR_MINOR_CODE_95 = 95,    // Invalid ioGroup parameter into GetCmdGroupAddress10nm
  RC_FATAL_ERROR_MINOR_CODE_96 = 96,    // GetMCID - invalid parameter
  RC_FATAL_ERROR_MINOR_CODE_97 = 97,    // SMBUS interface failure in PmicAccessLib
  RC_FATAL_ERROR_MINOR_CODE_98 = 98,    // New failure occured in previously masked range in UpdateRowFailures
  RC_FATAL_ERROR_MINOR_CODE_99 = 99,    // Failure occured outside of test range in CpgcMemTestMATS
  RC_FATAL_ERROR_MINOR_CODE_100 = 100,  // ConfigureDramForWriteLeveling - invalid Mode value
  RC_FATAL_ERROR_MINOR_CODE_101 = 101,  // GetDdrtWriteLatency - Value is less than valid min
  RC_FATAL_ERROR_MINOR_CODE_102 = 102,  // GetDdrtWriteLatency - Value is greater than valid max
  RC_FATAL_ERROR_MINOR_CODE_103 = 103,  // Error related to use of RC_SIM_FEEDBACK
  RC_FATAL_ERROR_MINOR_CODE_104 = 104,  // The CWL ADJ delta between the two sub channels is too large and out of range
  RC_FATAL_ERROR_MINOR_CODE_105 = 105,  // Cannot initialize the DDR5 mode register cache due to an invalid cache pointer
  RC_FATAL_ERROR_MINOR_CODE_106 = 106,  // Space allocated for DramNvdata < Space required
  RC_FATAL_ERROR_MINOR_CODE_107 = 107,  // Current strobe is out of bounds within subchannel in WriteDramModeRegCachePerDevice
  RC_FATAL_ERROR_MINOR_CODE_108 = 108,  // Current strobe is out of bounds within subchannel in ReadDramModeRegCachePerDevice
  RC_FATAL_ERROR_MINOR_CODE_109 = 109,  // The Mode Register cache "Valid" array index is out of bounds
  RC_FATAL_ERROR_MINOR_CODE_110 = 110,  // The DDR5 mode register is missing from the list of MRs to cache
  RC_FATAL_ERROR_MINOR_CODE_111 = 111,  // StrobeLinearToLogical invalid strobe
  RC_FATAL_ERROR_MINOR_CODE_112 = 112,  // Invalid parameter passed to SetDdrintfCtl0
  RC_FATAL_ERROR_MINOR_CODE_113 = 113,  // Invalid parameter passed to GetDdrintfCtl0
  RC_FATAL_ERROR_MINOR_CODE_114 = 114,  // Invalid paramter passed to GetDdrintfCtl0RegAddress
  RC_FATAL_ERROR_MINOR_CODE_115 = 115,  // Sendmrw target can not be DRAM when the DRAM CA is not trained
  RC_FATAL_ERROR_MINOR_CODE_116 = 116,  // CADB2 buffer index out of range
  RC_FATAL_ERROR_MINOR_CODE_117 = 117,  // Trace Hub ACPI base not 256KB aligned
  RC_FATAL_ERROR_MINOR_CODE_118 = 118,  // Unsupported processor detected in system
  RC_FATAL_ERROR_MINOR_CODE_119 = 119,  // MemWriteLrdimmBufferHostVrefDq input parameter out of range
  RC_FATAL_ERROR_MINOR_CODE_120 = 120,  // MemWriteDramVrefDdr5 - invalid input parameter
  RC_FATAL_ERROR_MINOR_CODE_121 = 121,  // Invalid ch parameter in ProgramJedecTimings
  RC_FATAL_ERROR_MINOR_CODE_122 = 122,  // Invalid ch parameter in SetCwlAdj
  RC_FATAL_ERROR_MINOR_CODE_123 = 123,  // Invalid Ch parameter in GetIoLatencyIoComp
  RC_FATAL_ERROR_MINOR_CODE_124 = 124,  // RestoreRoundTrip - invalid channel specified
  RC_FATAL_ERROR_MINOR_CODE_125 = 125,  // Invalid Ch parameter in SetIoLatencyIoComp
  RC_FATAL_ERROR_MINOR_CODE_126 = 126,  // ConfigureTxUi - Unable to find RegIndex for signal
  RC_FATAL_ERROR_MINOR_CODE_127 = 127,  // SimRegs -  Failed to locate register file list instance
  RC_FATAL_ERROR_MINOR_CODE_128 = 128,  // Memory timing limits not found
  RC_FATAL_ERROR_MINOR_CODE_129 = 129,  // ConfigureTxUi - Unable to find address for RegIndex
  RC_FATAL_ERROR_MINOR_CODE_130 = 130,  // Logging power training results failed
  RC_FATAL_ERROR_MINOR_CODE_131 = 131,  // GetPowerParamFromSignalName () called with unsupported SignalName
  RC_FATAL_ERROR_MINOR_CODE_132 = 132,  // Invalid SMBUS controller number
  RC_FATAL_ERROR_MINOR_CODE_133 = 133,  // SimRegs: Failed to locate socket for PCIE BDF address
  RC_FATAL_ERROR_MINOR_CODE_134 = 134,  // SimRegs: Failed to locate MemBar entry
  RC_FATAL_ERROR_MINOR_CODE_135 = 135,  // SimRegs: Memory subsystem type unsupported
  RC_FATAL_ERROR_MINOR_CODE_136 = 136,  // SimRegs: Failed to locate MMIO region entry
  RC_FATAL_ERROR_MINOR_CODE_137 = 137,  // SimRegs: Failed to locate register file
  RC_FATAL_ERROR_MINOR_CODE_138 = 138,  // SimRegs: Failed to find match for register address
  RC_FATAL_ERROR_MINOR_CODE_139 = 139,  // SimRegs: Failed to locate socket for PCIE BDF address
  RC_FATAL_ERROR_MINOR_CODE_140 = 140,  // GetSetCmdVrefHostSide called for unknown channel!
  RC_FATAL_ERROR_MINOR_CODE_141 = 141,  // Insufficient performance tracker storage
  RC_FATAL_ERROR_MINOR_CODE_142 = 142,  // RestoreIoLatency - invalid channel specified
  RC_FATAL_ERROR_MINOR_CODE_143 = 143,  // XoverCalib - ProgramOffsets() - Failed to find offset table
  RC_FATAL_ERROR_MINOR_CODE_144 = 144,  // RTL input is odd and should be even.
  RC_FATAL_ERROR_MINOR_CODE_145 = 145,  // IO Latency input is even and should be odd.
  RC_FATAL_ERROR_MINOR_CODE_146 = 146,  // First DIMM not found in an enabled memory channel.
  RC_FATAL_ERROR_MINOR_CODE_147 = 147,  // Failure to get SMBus address for DIMM slot
  RC_FATAL_ERROR_MINOR_CODE_148 = 148,  // No valid DDRIO Clock number for NVMDIMM
  RC_FATAL_ERROR_MINOR_CODE_149 = 149,  // lpmode_entry_latency is set to LPMODE_ENTRY_LATENCY_LARGE_SNR in SNR A0, which is a constraint
  RC_FATAL_ERROR_MINOR_CODE_150 = 150,  // t_ddrt_cke is set to lpmode_entry_latency-1 in SNR A0, which is a constraint
  RC_FATAL_ERROR_MINOR_CODE_151 = 151,  // INVALID cmpl_program_delay found
  RC_FATAL_ERROR_MINOR_CODE_152 = 152,  // InitDdrioInterfaceLate - Unreachable code (channel out of bounds)
  RC_FATAL_ERROR_MINOR_CODE_153 = 153,  // MemCpgc - Row range size is not 2^n
  RC_FATAL_ERROR_MINOR_CODE_154 = 154,  // GetSetSxpGnt2Erid - DIMM index is out of bounds
  RC_FATAL_ERROR_MINOR_CODE_155 = 155,  // InitializeDefaultData - Invalid number of sockets
  RC_FATAL_ERROR_MINOR_CODE_156 = 156,  // DimmInfoDisplayProc - Invalid socket type
  RC_FATAL_ERROR_MINOR_CODE_157 = 157,  // GetPclsCaptureDelayDddrio not implemented
  RC_FATAL_ERROR_MINOR_CODE_158 = 158,  // InitFnvAccessMode/RestoreFnvAccessMode - Invalid buffer
  RC_FATAL_ERROR_MINOR_CODE_159 = 159,  // GetSetTxTco - Invalid level
  RC_FATAL_ERROR_MINOR_CODE_160 = 160,  // ProgramMRSAEP - CWL exceeds MR2 bitfield range
  RC_FATAL_ERROR_MINOR_CODE_161 = 161,  // Invalid UI delta within the Register Nibble
  RC_FATAL_ERROR_MINOR_CODE_162 = 162,  // GetPmCmdPwrRegOffset - Index is out of bounds
  RC_FATAL_ERROR_MINOR_CODE_163 = 163,  // GetSetRxDelayBit - Writes must specify all bits in the nibble
  RC_FATAL_ERROR_MINOR_CODE_164 = 164,  // GetSetTxDelayBit - Writes must specify all bits in the nibble
  RC_FATAL_ERROR_MINOR_CODE_165 = 165,  // GetRoundTripRegOffset - Channel out of bounds for DIMM 0
  RC_FATAL_ERROR_MINOR_CODE_166 = 166,  // GetRoundTripRegOffset - Channel out of bounds for DIMM 1
  RC_FATAL_ERROR_MINOR_CODE_167 = 167,  // GetRoundTripRegOffset - DIMM out of bounds
  RC_FATAL_ERROR_MINOR_CODE_168 = 168,  // GetDimmMtrRegOffset - Index out of bounds
  RC_FATAL_ERROR_MINOR_CODE_169 = 169,  // RestoreChipRxEnableParams Invalid input parameter
  RC_FATAL_ERROR_MINOR_CODE_170 = 170,  // MemCpgc Address mode is not supported
  RC_FATAL_ERROR_MINOR_CODE_171 = 171,  // QxnCA[m] delay value out of range in GetSetCaBacksideDdr5
  RC_FATAL_ERROR_MINOR_CODE_172 = 172,  // QxCS[m] delay value out of range in GetSetCsBacksideDdr5
  RC_FATAL_ERROR_MINOR_CODE_173 = 173,  // Size exceeded for RCD  SMBUS Write
  RC_FATAL_ERROR_MINOR_CODE_174 = 174,  // Current strobe is out of bounds within subchannel in GetStrobeMapping
  RC_FATAL_ERROR_MINOR_CODE_175 = 175,  // Current strobe is out of bounds within channel in GetStrobeMapping
  RC_FATAL_ERROR_MINOR_CODE_176 = 176,  // Current strobe is out of bounds within channel in GetNibbleFromStrobe
  RC_FATAL_ERROR_MINOR_CODE_177 = 177,  // Clock number out of bounds in GetClkRegOffset
  RC_FATAL_ERROR_MINOR_CODE_178 = 178,  // ProgramIdelCntValidEarly : Invalid Channel value.
  RC_FATAL_ERROR_MINOR_CODE_179 = 179,  // In DCA DFE training, when the count window expires, the CADB is still running.
  RC_FATAL_ERROR_MINOR_CODE_180 = 180,  // Refresh mode for TREFI
  RC_FATAL_ERROR_MINOR_CODE_181 = 181,  // Invalid parameter into GetSetRxStrobeInvert
  RC_FATAL_ERROR_MINOR_CODE_182 = 182,  // DDRT Power cycle is required after frequency changed in CheckDdrtFreqChange
  RC_FATAL_ERROR_MINOR_CODE_183 = 183,  // Incorrect Context Signature in SetSafePowerTrainingMargins
  RC_FATAL_ERROR_MINOR_CODE_184 = 184,  // Incorrect Context Signature in RestorePowerTrainingMargins
  RC_FATAL_ERROR_MINOR_CODE_185 = 185,  // JEDECArm - Invalid NvDimmEnergyPolicy setup value
  RC_FATAL_ERROR_MINOR_CODE_186 = 186,  // EnableDfe - invalid GroupInput parameter
  RC_FATAL_ERROR_MINOR_CODE_187 = 187,  // DcaDfeInitDdr5 - invalid Mode parameter
  RC_FATAL_ERROR_MINOR_CODE_188 = 188,  // Cannot initialize the DDR5 RCD/DB control word cache due to an invalid cache pointer
  RC_FATAL_ERROR_MINOR_CODE_189 = 189,  // Space allocated for RcdNvdata/DbNvdata < Space required
  RC_FATAL_ERROR_MINOR_CODE_190 = 190,  // The RCD/DB Control Word cache "Valid" array index is out of bounds
  RC_FATAL_ERROR_MINOR_CODE_191 = 191,  // RCD/DB CW address not found in the cache list
  RC_FATAL_ERROR_MINOR_CODE_192 = 192,  // WARNING - WriteRcdCwCache/WriteDbCwCache called with an invalid parameter
  RC_FATAL_ERROR_MINOR_CODE_193 = 193,  // WARNING - ReadRcdCwCache/ReadDbCwCache called with an invalid parameter
  RC_FATAL_ERROR_MINOR_CODE_194 = 194,  // EncodedSpdToDramDensity - Invalid input parameter
  RC_FATAL_ERROR_MINOR_CODE_195 = 195,  // CompareDimmPartNumberDdr4 - Comparison against the template was incomplete
  RC_FATAL_ERROR_MINOR_CODE_196 = 196,  // Invalid group value into EnableTxDfeAllTaps
  RC_FATAL_ERROR_MINOR_CODE_197 = 197,  // DDRT Power cycle is needed after freq changed
  RC_FATAL_ERROR_MINOR_CODE_198 = 198,  // Invalid signal number
  RC_FATAL_ERROR_MINOR_CODE_199 = 199,  // Invalid Group parameter in GetSetRxSamplerEvenOd
  RC_FATAL_ERROR_MINOR_CODE_200 = 200,  // Invalid Strobe parameter in GetSetRxSamplerEvenOd
  RC_FATAL_ERROR_MINOR_CODE_201 = 201,  // Invaid access for CPGC register cache access
  RC_FATAL_ERROR_MINOR_CODE_202 = 202,  // Invalid tCL value got from SPD data for Minimum CAS Latency Time (tAAmin)
  RC_FATAL_ERROR_MINOR_CODE_203 = 203,  // Invalid Dimm in the InitAdrForDdrt
  RC_FATAL_ERROR_MINOR_CODE_204 = 204,  // No enabled socket found in the InitAdrTimer
  RC_FATAL_ERROR_MINOR_CODE_205 = 205,  // Invid Parameter in DbControlWordWrite/RestoreDbControlWordsFromCache
  RC_FATAL_ERROR_MINOR_CODE_206 = 206,  // Read Cache before it update by write in AccessDbCwCache
  RC_FATAL_ERROR_MINOR_CODE_207 = 207,  // GetDelay - DelayCounts is greater than 32 bits
// 208 available for future use
  RC_FATAL_ERROR_MINOR_CODE_209 = 209,  // RankListSize out of bounds in SSA CPGC test setup.
  RC_FATAL_ERROR_MINOR_CODE_210 = 210,  // DDRIO structure size mismatch
  RC_FATAL_ERROR_MINOR_CODE_211 = 211,  // ProgramRxAnalogEnEarly returns non-MRC_STATUS_SUCCESS.
  RC_FATAL_ERROR_MINOR_CODE_212 = 212,  // GetIntfDataTiming1RegOffset: Ch or SubCh out of bounds.
  RC_FATAL_ERROR_MINOR_CODE_213 = 213,  // Index is out of bounds within subchannel in RcSimCaClkResults or RcSimBacksideCsClkResults
  RC_FATAL_ERROR_MINOR_CODE_214 = 214,  // Attempted to release a semaphore that was not owned by the requester
// 215 available for future use
  RC_FATAL_ERROR_MINOR_CODE_216 = 216,  // Invalid GSM_CSN in GetRcdCaSignal
  RC_FATAL_ERROR_MINOR_CODE_217 = 217,  // Signal not found in SignalsToSweepDDR5
  RC_FATAL_ERROR_MINOR_CODE_218 = 218,  // NULL pointer of NumOfCacheLine in CheckHwSupportedForMATSAndFixupCacheLine for AMT
  RC_FATAL_ERROR_MINOR_CODE_219 = 219,  // CPGC PollUntilChannelTestingDone timeout
  RC_FATAL_ERROR_MINOR_CODE_220 = 220,  // Signal count more than RCD_CA_SIGNALS_TO_SWEEP
  RC_FATAL_ERROR_MINOR_CODE_221 = 221,  // Invalid Dimm, Rank or SubChannel in the GetBacksideCsSignal
  RC_FATAL_ERROR_MINOR_CODE_222 = 222,  // Invalid Refresh Mode policy value - DfxDdr5RefreshMode
  RC_FATAL_ERROR_MINOR_CODE_223 = 223,  // GetMaxPollingTime - ddrFreq is not in the valid range that can be supported in current
  RC_FATAL_ERROR_MINOR_CODE_224 = 224,  // SetCombinedCaGroupDdr5 - unmatched input parameter ValueSize
  RC_FATAL_ERROR_MINOR_CODE_225 = 225,  // SetCombinedCmdGroupDdr5 - unmatched input parameter ValueSize
  RC_FATAL_ERROR_MINOR_CODE_226 = 226,  // Invalid Dimm, Rank or SubChannel in the GetRcdCsSignal
  RC_FATAL_ERROR_MINOR_CODE_227 = 227,  // Illegal strobe in GetSetRxVref
  RC_FATAL_ERROR_MINOR_CODE_228 = 228,  // Illegal BitIndex in GetSetRxVref
  RC_FATAL_ERROR_MINOR_CODE_229 = 229,  // Invalid LogicalBit in GetDqSwizzle
  RC_FATAL_ERROR_MINOR_CODE_230 = 230,  // Invalid TargetStrobe in GetDqSwizzle
  RC_FATAL_ERROR_MINOR_CODE_231 = 231,  // Invalid LogicalBit & TargetStrobe combination in GetDqSwizzle
  RC_FATAL_ERROR_MINOR_CODE_232 = 232,  // Invalid test type ResultsProcessingDca
  RC_FATAL_ERROR_MINOR_CODE_233 = 233,  // Memory allocation failure for RMT core test data
  RC_FATAL_ERROR_MINOR_CODE_234 = 234,  // DramModeRegRead run into failure
  RC_FATAL_ERROR_MINOR_CODE_235 = 235,  // Incorrect format of MRR returned data
  RC_FATAL_ERROR_MINOR_CODE_236 = 236,  // GetDramDutyCycleAdjuster - invalid group parameter
  RC_FATAL_ERROR_MINOR_CODE_237 = 237,  // SetDramDutyCycleAdjuster - invalid group parameter
  RC_FATAL_ERROR_MINOR_CODE_238 = 238,  // GetSetDramDutyCycleAdjuster - invalid input
  RC_FATAL_ERROR_MINOR_CODE_239 = 239,  // GetSetDramDutyCycleAdjuster - invalid strobe or bit parameter
  RC_FATAL_ERROR_MINOR_CODE_240 = 240,  // ProcessDramDcaResults - invalid DRAM DCA group
  RC_FATAL_ERROR_MINOR_CODE_241 = 241,  // DRAM DCA CalculateEyeWidth - invalid UI/Chunk
  RC_FATAL_ERROR_MINOR_CODE_242 = 242,  // SwitchToNormalMode - Call to PcuMcTimeoutWaDisable had an error
  RC_FATAL_ERROR_MINOR_CODE_243 = 243,  // GetSetDcaTco - Invalid input parameter SubCh
  RC_FATAL_ERROR_MINOR_CODE_244 = 244,  // Invalid parameter Group for GetSetDelayUnicast
  RC_FATAL_ERROR_MINOR_CODE_245 = 245,  // Error when GetSetDelayMulticastOffset is executed in GetSetMulticastUnicast as Retry 1
  RC_FATAL_ERROR_MINOR_CODE_246 = 246,  // Error while executing unicast function in GetSetDelayUnicast
  RC_FATAL_ERROR_MINOR_CODE_247 = 247,  // Error when GetSetDelayMulticastOffset is executed in GetSetMulticastUnicast handling MRC_STATUS_OUT_OF_RESOURCES
  RC_FATAL_ERROR_MINOR_CODE_248 = 248,  // Error when GetSetDelayMulticastOffset is executed in GetSetRxDelay
  RC_FATAL_ERROR_MINOR_CODE_249 = 249,  // Error when GetSetDelayMulticastOffset is executed in GetSetRxSamplerEvenOdd
  RC_FATAL_ERROR_MINOR_CODE_250 = 250,  // Signal count more than BACKSIDE_CA_SIGNALS_TO_SWEEP
  RC_FATAL_ERROR_MINOR_CODE_251 = 251,  // Signal not found in SignalsToSweepBacksideDDR5
  RC_FATAL_ERROR_MINOR_CODE_252 = 252,  // SwitchToNormalMode - Call to MaximizeDcpmmReadDrainTimeout had an error
  RC_FATAL_ERROR_MINOR_CODE_253 = 253   // GetParityRecoveryDdr5 not supported for NULL library
} RC_FATAL_ERROR_MINOR_CODE;

typedef enum {
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_000,   // NULL pointer for ResultProcessing callback
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_001,   // NULL pointer for BitMargins in PrintCurrentStrobeResults1D
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_002,   // Channel out of bounds in SkipChannelExecution
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_003,   // Group delay has been declared as SingleToMultipleDelay and ResultAtRankLevel
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_004,   // Channel out of bounds in GetDelayPtr
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_005,   // Channel out of bounds in IsStrobeAtLimit
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_006,   // Available
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_007,   // Available
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_008,   // Available
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_009,   // Available
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_010,   // Illegal value for DeltaOffset in GetVerticalIndex
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_011,   // No first channel detected
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_012,   // NULL pointer for BitMargins in TrackingResults1D
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_013,   // NULL pointer for BitMargins in AdjustDitherResults1D
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_014,   // NULL pointer for BitMargins in AdjustRelativeResults1D
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_015,   // Illegal state in ExecuteStateAction
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_016,   // Illegal state in GetNextState
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_017,   // Illegal dither value in ResultsConditionByState
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_018,   // SubChannelIndex out of bounds in GetStrobeLogical
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_019,   // StrobeLogical out of bounds in GetStrobeToNibbleOffsetPositionForDdr4
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_020,   // StrobeLogical out of bounds in GetStrobeToNibbleOffsetPositionForDdr5
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_021,   // Per bit value read out during final adjustment not match to original value.
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_022,   // FLAGS_CATEGORY_RESULTS.ResultsEvenOdd is set for a multiknob group delay
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_023,   // NULL pointer for BitMargins in ConvertResults1D
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_024,   // NULL pointer for BitMargins in ConvertResults1D && AreResultsAtByteLevel
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_025,   // Invalid logical rank in ClearLegacyResults
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_026,   // Invalid ChunkSide in SetStrobeLegacyResult
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_027,   // Invalid ChunkSide in SetBitLegacyResult
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_028,   // Invalid ChunkSide in ConvertResults1D
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_029,   // Invalid ChunkSide in UpdateMaxLeftMinRighEvenOdd
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_030,   // Invalid ChunkSide in CleanUpLegacyResults
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_031,   // StrobeLogical out of bounds in GetStrobeLogical
  RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_MAX
} SWEEP_LIB_WARN_MINOR_CODE;

typedef enum {
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_000 = 0,   // CheckValueForPda - invalid BestTapBias pointer
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_001 = 1,   // ProgramDfeTapBias - invalid BestTapBias pointer instance 1
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_002 = 2,   // ProgramDfeTapBias - invalid BestTapBias pointer instance 2
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_003 = 3,   // GetSetProtectedDcpmmRegister - Value pointer invalid
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_004 = 4,   // ConvertValueToDramDutyCycleAdjusterRegFormat - bad pointer
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_005 = 5,   // WriteProtectedDcpmmRegister - bad pointer
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_006 = 6,   // ProgramReadDfeTapBias - bad BestTapBias pointer instance 1
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_007 = 7,   // ProgramReadDfeTapBias - bad BestTapBias pointer instance 2
 RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_MAX
} ERR_MRC_POINTER_MINOR_CODE;

typedef enum {
//
// 000 ~ 099: minor code is used for SPD read byte failed.
//
  SPD_FATAL_ERROR_READ_MINOR_CODE_000 = 0,             // DeviceType - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_001 = 1,             // DramDieCount - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_002 = 2,             // NumColBits - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_003 = 3,             // NumRowBits - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_004 = 4,             // BankGroupBits - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_005 = 5,             // BankAddressBits - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_006 = 6,             // RcdVendor - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_007 = 7,             // RcdRevision - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_008 = 8,             // DramCapacity - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_009 = 9,             // DramIOWidth - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_010 = 10,            // RankMixIndicator - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_011 = 11,            // PackageRanks - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_012 = 12,            // MiniCycleTime - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_013 = 13,            // BaseModuleType - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_014 = 14,            // OperableEndurant VDD - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_015 = 15,            // RawCardRefDesign - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_016 = 16,            // RawCardRefDesignRev - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_017 = 17,            // NominalHeight - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_018 = 18,            // PrimaryBusWidth - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_019 = 19,            // BusWidthExtension - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_020 = 20,            // ChannelNumPerDimm - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_021 = 21,            // RcdDeviceType - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_022 = 22,            // DramStepping - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_023 = 23,            // PartNum - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_024 = 24,            // SpecRevision - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_025 = 25,            // ManufacturerId - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_026 = 26,            // TrpVal - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_027 = 27,            // Module SerialNumber - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_028 = 28,            // ManufacturingDate - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_029 = 29,            // TrasVal - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_030 = 30,            // ManufacturingLocation - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_031 = 31,            // TrcVal - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_032 = 32,            // TrcdVal - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_033 = 33,            // TaaVal - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_034 = 34,            // CasSup - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_035 = 35,            // SpdIsThermalSensorInstalled - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_036 = 36,            // Ppr - SPD read Byte failed
  SPD_FATAL_ERROR_READ_MINOR_CODE_037 = 37,            // DramManufacturerId - SPD read Byte failed

//
// 100 ~ : minor code is used for value valid check, such as out of range.
//
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_100 = 100,    // Unsupported DeviceType read from SPD
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_101 = 101,    // Unsupported DramDieCount read from SPD
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_102 = 102,    // Unsupported NumColBits read from SPD
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_103 = 103,    // Unsupported NumRowBits read from SPD
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_104 = 104,    // Unsupported Trfc read from SPD
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_105 = 105,    // Unsupported SensorIndex
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_106 = 106,    // Unsupported Ppr read request
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_107 = 107,    // Unsupported RawCardRefDesign
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_108 = 108,    // Unsupported RawCardRefDesignRev
  SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_109 = 109,    // Unsupported NominalHeight
  SPD_FATAL_ERROR_MINOR_CODE_MAX
} SPD_FATAL_ERROR_MINOR_CODE;

#endif // #ifndef _REFERENCE_CODE_FATAL_ERRORS_H_
