/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_HOST_CHIP_10NM_H
#define _MEM_HOST_CHIP_10NM_H

#include "SysRegs.h"
#include "MemRegs.h"
#include <Memory/MemDefaults.h>
#include "NGNDimmPlatformCfgData.h"

//
// IMC Vendor and Device Ids
//
#define IMC_VID     0x8086
#define IMC_DID     0x3CA8
#define ALL_DVID    0xFFFFFFFF


//
// MemTraining
//
#define MAX_VREF_SETTINGS       20
#define EARLY_VREF_STEP_SIZE    1
#define RX_VREF_TRAINING_MODE   PER_BIT
#define TX_VREF_TRAINING_MODE   NON_PER_BIT

//
// InitMemChip
//
#define MIN_DDR4_VDD  900
#define MAX_DDR4_VDD  1850
#define AEP_VDD       1200
#define MIN_AEP_VDD   1128  // DCPMM supports +/-6% of VDD = 1.2V
#define MAX_AEP_VDD   1272

#define SET_CPL_1     1
#define SET_CPL_2     2
#define SET_CPL_3     3
#define SET_CPL_4     4

//SKX changes
#define MAX_CMD_GROUP0    6
#define MAX_CMD_GROUP1    3
#define MAX_CMD_GROUP2    4
#define MAX_CMD_GROUPALL  13

//SKX changes
#define MAX_CTL_GROUP0  4
#define MAX_CTL_GROUP1  2
#define MAX_CTL_GROUP2  3
#define MAX_CTL_GROUP3  2
#define MAX_CTL_GROUPALL  11

#define MAX_CMDCTL_GROUPALL  (MAX_CMD_GROUPALL + MAX_CTL_GROUPALL)
#define CAS2DRVEN_MAXGAP  39

#define MAX_CMD_TX_EQ_VAL         31

#define ODT_RD_CYCLE_LEADING  1
#define ODT_RD_CYCLE_TRAILING 1
#define ODT_RD_DELAY 0

#define ODT_WR_CYCLE_LEADING  1
#define ODT_WR_CYCLE_TRAILING 1
#define DDR4_LRDIMM_ODT_WR_CYCLE_LEADING  1
#define DDR4_LRDIMM_ODT_WR_CYCLE_TRAILING 2

//
// BIOS_RESET_CPL_PCU_FUN1_REG
//
#define SET_CPL_1     1
#define SET_CPL_2     2
#define SET_CPL_3     3
#define SET_CPL_4     4

#pragma pack(1)
struct ChannelTestGroup
{
  UINT8                       channelBitMask;
  UINT32                      maxMtr;
};
#pragma pack()

//
// Initial Round Trip Latency = (Initial Round Trip Latency Offset) + 2*CL
// Initial Round Trip Latency Offset = (Initial internal Delay) +
//                                     (IO_COMP) +
//                                     (Internal IO Latency Buffer) +
//                                     (Read Fly By Buffer) +
//                                     (Additional buffer)
//
#define INITIAL_INTERNAL_DELAY        2 // 2 DCLK     4  QCLK
#define IO_COMP_DELAY                 9 // 9 DCLK     18 QCLK
#define IO_LATENCY_BUFFER_DELAY       4 // 4 DCLK     8  QCLK
#define READ_FLY_BY_BUFFER_DELAY      4 // 4 DCLK     8  QCLK
#define ADDITIONAL_BUFFER_DELAY       FixedPcdGet8 (PcdRoundTripBufferDelayDclk)

//
// Round trip default starting value
//
#define MRC_ROUND_TRIP_DEFAULT_VALUE        ((INITIAL_INTERNAL_DELAY)   + \
                                             (IO_COMP_DELAY)            + \
                                             (IO_LATENCY_BUFFER_DELAY)  + \
                                             (READ_FLY_BY_BUFFER_DELAY) + \
                                             (ADDITIONAL_BUFFER_DELAY))
#define MRC_ROUND_TRIP_MAX_VALUE            126 // Round trip max value (Must be even for 10nm so this is 0x7F - 1)
#define MRC_IO_LATENCY_DEFAULT_VALUE        3   // IO latency default value
#define MRC_ROUND_TRIP_IO_COMPENSATION      18  // Roundtrip - IO compensation
#define MRC_ROUND_TRIP_IO_COMPENSATION_DDRT 20  // Roundtrip - IO compensation for DDRT
#define MRC_ROUND_TRIP_IO_COMPENSATION_EMU  17  // Roundtrip - IO compensation for Emulation
#define MRC_ROUND_TRIP_IO_COMP_START        18  // Roundtrip - IO compensation starting point
#define MRC_ROUND_TRIP_IO_COMP_START_DDRT   20  // Roundtrip - IO compensation starting point for DDRT

#define CTL_PI_GRP  511 // HW max is 255, but 511 is used to support "wrap-around" eyes that we later modulo to <= 255.

//
// defines for Receive enable
//
#define   MIN_IO_LATENCY              1


//
// Maximum value allowed for DdjcDelta + PxcDelta
//
#define MAX_DDJC_PLUS_PXC   4


// Mask for isolating the TxDqDelay logic delay
#define   TX_DQ_LOGIC_DELAY_MASK    0x3C0

//
// Platform Assigned CSRs for PIPE/SOCKET/MRC MILESTONES, Pipe.h
//
#define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_MMIO SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR
#define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR  (BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG)  // UBOX scratchpad CSR13
#define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_MMIO  SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR
#define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR   (BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG)  // UBOX scratchpad CSR14
#define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_MMIO     SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR
#define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR      (BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG)  // UBOX scratchpad CSR15

//
// 10NM specific value for the maximum pi Read Dq Dqs adjustment in the normal (DFE disabled) case
//
#define MAX_PHASE_IN_READ_ADJUSTMENT_DQ_10NM 128 // larger range for added DQ 1/16 PI adjustments

//
// MR2SHADOW register definiton in 10nm
//
typedef union {
struct {
    UINT32 AddrBitSwizzle:2;
    UINT32 TwoMrsEn:1;
    UINT32 Bl:2;
    UINT32 Asr:1;
    UINT32 Srt:1;
    UINT32 WrCmdLatency:2;
    UINT32 FineGranRefreshMode:3;
    UINT32 TempSensorReadout:1;
    UINT32 GeardownMode:1;
    UINT32 TempCtlRefreshMode:1;
    UINT32 TempCtlRefreshRange:1;
    UINT32 Rsvd:16;
}Bits;
UINT32 Data;
}ICXKNHMR2SHADOW_STRUCT;


#define MAX_RIR             4                   // Number of Rank Interleave Register rules for DDR
#define MAX_RIR_DDRT        4                   // Number of Rank Interleave Register rules for NVMDIMM
#define MAX_RIR_WAYS        8                   // Number of interleave ways for RIR for DDR
#define MAX_RIR_DDRT_WAYS   2                   // Number of interleave ways for RIR for NVMDIMM
#define TAD_RULES           8                   // Number of TAD rule registers
#define MAX_TAD_WAYS        3                   // Number of interleave ways for TAD RULES
#define MAX_SAD_WAYS        (MAX_IMC * 4)       // Number of interleave ways for SAD RULES
#define SAD_ATTR_DRAM       0                   // DRAM attribute for SAD rule
#define SAD_NXM_ATTR        2                   // Attr value for an NXM SAD rule
#define MAX_PAD             3                   // Number of PAD registers
#define MAX_RAW_CARD        5                   // Number of supported Raw Cards
#define PARITY_INTERLEAVE   1                   // SAD interleave with merged HAs (pseudo-hemisphere mode)
#define MAX_SEEDS           10                  // Maximum


//
// Panic Up/Down Multiplier
//
#define MIN_CAP 2 // 2 nF
#define CHARGED_VOLTAGE 24   // 24 mV
#define VSSHI 356   // Vsshi = 0.356 V (according to Vsshi target code = 0x25)
#define VCCP 1300   // Vccp = fixed value of 1.3 V
// Format 3.13 is a fixed point format in which the decimal point is assumed to be after 3 digits from the left. Dividing this whole number by 2^(13) gives the corresponding value in integer
// format.  Consider an example of converting 1100001000100011 in 3.13 format to integer format (unit is volts) (1100001000100011)2 = (49699)10; 49699 /2^(13){8192} = 6.0667724609375 volts
#define VCCP_CONVERSION_FACTOR 8192 // VCCP conversion factor is used when actual VCCP is read (and that it's 2^13 -- as it's written in the spec)
#define RDQ 90    // Rdq = 90 Ohm
#define PANICCOMPUPMULT 0x7F // max value for panic up multiplier
#define PANICCOMPDNMULT 0x7F // max value for panic down multiplier

//
// idle_cmd_to_cke_drops_timer value
//
#define IDLE_CMD_CKE_DROPS_TIMER 0xFF

//
// Page Policy and Timing parameters
//
#define IDLE_PAGE_RST_VAL_MAX 0xFFF
#define IDLE_PAGE_RST_VAL_MAX_HBM 0x200
#define WIN_SIZE              32  // For DDR and HBM2E
#define PPC_TH                6   // For DDR and HBM2E
#define OPC_TH                6   // For DDR and HBM2E

#endif // _MEM_HOST_CHIP_10NM_H
