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

#ifndef _memhostchipcommon_h
#define _memhostchipcommon_h


#include "SysHostChipCommon.h"
#include "NGNDimmPlatformCfgData.h"
#include <MemCommon.h>
#include <Library/MemTypeLib.h>


// Define the different SKX package Sku's
#define SKX_SKU_LCC 0x0
#define SKX_SKU_HCC 0x2
#define SKX_SKU_XCC 0x3

#define MREP_LOOPCOUNT  1 // core override, HSD 5331466 - cachelines = infinite, loopcount = 1
#define MREP_CACHELINES 0 //

#define NODE_TO_SKT(node)       (node / MAX_IMC)
#define SKT_TO_NODE(socket, mc) ((socket << 1) | (mc & BIT0))
#define NODE_TO_MC(node)        (node % MAX_IMC)
#define SKTMC_TO_NODE(socket, mc)  ((socket * MAX_IMC) | (mc % MAX_IMC))
#define NODECH_TO_SKTCH(node, ch)    (((node % MAX_IMC)*MAX_MC_CH) + ch)

//
// MemDisplay
//
#define DISPLAY_RX_VREF_SUPPORT 1  // leave undefined if unsupported

//
// MemTraining
//
#define EYE_HEIGHT_SIM_DEFAULT  20
#define RX_VREF_TUNING          0               // fixed on SKX
#define DESKEW_DELAY            RxDqsPBitDelay  // Set this to either RxDqsBitDelay or RxDqsPBitDelay
#define MIN_EARLYVREF_EYE       6

//
// DDR5 tMRD should be max(14ns, 16dclk)
//
#define TMRD_NS           14    // 14ns
#define TMRD_MIN_DCLK     16    // 16 dclk

#define tZQOPER_DDR4 512
#define tZQCS_DDR4 128

//
// tZQLAT should be max(30ns,8dclk)
//
#define DDR5_TZQLAT_NS          30    // 30ns
#define DDR5_TZQLAT_MIN_DCLK     8    // 8 dclk
#define DDRT2_TZQLAT_MIN_DCLK 0x80    // 128 dclk

#define DDRT2_TZQLAT_ANYCMD_MAX_DCLK 0x80    // 128 dclk

#define DDR5_TZQCAL_NS        1000  // 1us

#define DDR5_DQS_INTERVAL_TIMER_RUNTIME   0x40 // see MR45 in JEDEC DDR5 spec

//MemoryAttributes
#define MEMORY_REGION_DESC                 0
#define RESERVED_REGION_DESC               1
#define RESERVED_SYSTEM_MEMORY_REGION_DESC 2
#define MMIO_REGION_DESC                   4
#define PMEM_DESCRIPTOR_ATTRIBUTES  EFI_RESOURCE_ATTRIBUTE_PERSISTENT | EFI_RESOURCE_ATTRIBUTE_PERSISTABLE
#define CACHE_DESCRIPTOR_ATTRIBUTES EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE | EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE | EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE

//MemWriteLeveling
#define TX_DQ_DELAY_MAX 447;
#define TX_DQS_DELAY_MIN 64;
//
// Array sizes
//
#define IMC0 0
#define IMC1 1

#define MAX_MEM_SS          8                   // Max Memory Subsystems per socket
#define MAX_CLUSTERS        4                   // Maximum number of clusters supported

//
// MAX_IMC & MAX_MC_CH will be provided by the build process (typically in the platform DSC file)
// It is required to define them and pass as build flag to avoid compiler error here.
//

#define MEM_GRANULARITY                64
#define MC_MAX_NODE                    (MAX_SOCKET * MAX_IMC)    // Max number of memory nodes
#define MAX_SYS_CH                     (MAX_CH * MAX_SOCKET)     // Max channels in the system
#define MAX_DCPMM_CH                   1
#define MAX_DCPMMS                     (MAX_SYS_CH * MAX_DCPMM_CH) // Max DCPMMs in the system
#define MAX_DIMM                       2                         // Max DIMM per channel
#define CLOCKS_PER_DIMM                2
#define MAX_DDRT_DIMM_PER_CH           1                         // Max DDRT DIMM per channel
#define MAX_UNIQUE_NGN_DIMM_INTERLEAVE 2                         // Max number of unique interleaves for NGN DIMM
#define MAX_MEM_RISER                  2                         // Max memory risers per socket
//SKX hack
#define MAX_DIMM3           3                   // Max DIMM per channel
#ifdef DDR5_SUPPORT
#define  MAX_BITS    80 // Max number of data bits per rank in DDR5
#else
#define  MAX_BITS    72 // Max number of data bits per rank
#endif
#define DDR5_SUBCH_MAX_DQ_BITS   32
#define DDR5_SUBCH_MAX_ECC_BITS  8
#define DDR5_SUBCH_MAX_BITS      (DDR5_SUBCH_MAX_DQ_BITS + DDR5_SUBCH_MAX_ECC_BITS)
#define MAX_BITS_DDR5            (DDR5_SUBCH_MAX_BITS * SUB_CH)
#define MAX_TECH            19                  // Number of entries in DRAM technology table
#define MAX_TECH_DDR4       12                  // Number of entries in DRAM technology table
#define MAX_TECH_DDR5       15                  // Number of entries in DRAM technology table
#define MAX_TECH_DDRT        8
#define MAX_DENSITY         4
#define MAX_RM              2
#define MAX_RIR             4                   // Number of Rank Interleave Register rules for DDR
#define MAX_RIR_DDRT        4                   // Number of Rank Interleave Register rules for NVMDIMM
#define MAX_RIR_WAYS        8                   // Number of interleave ways for RIR for DDR
#define MAX_RIR_DDRT_WAYS   2                   // Number of interleave ways for RIR for NVMDIMM
#define RIR_UNIT            29                  // RIR limit unit is 512MB aka 2^29
#define TAD_RULES           8                   // Number of near memory TAD rule registers
#define FM_TAD_RULES        12                  // Number of far memory TAD rule registers
#define FM_TAD_RULES_10NM   4                   // Number of far memory only TAD rule registers in 10nm
#define MAX_TAD_RULES       (TAD_RULES + FM_TAD_RULES)      // Number of combined near and far TAD rules
#define MAX_TAD_RULES_10NM  (TAD_RULES + FM_TAD_RULES_10NM) // Number of combined near and far TAD rules in 10nm
#define MAX_TAD_WAYS        3                   // Number of interleave ways for TAD RULES
#define SAD_RULES_10NM     16                   // Number of SAD rule registers in 10NM
#define REMOTE_SAD_RULES_10NM_WAVE1      14     // Number of remote SAD rule registers in 10nm wave1
#define MAX_REMOTE_SAD_RULES             21     // Maximum number of remote SAD rule registers
#define MAX_SAD_WAYS        (MAX_IMC * 4)       // Number of interleave ways for SAD RULES
#define MAX_SAD_INDEX       (MAX_DRAM_CLUSTERS * MAX_SAD_RULES) // Max Sad Index for SAD table access
#define SAD_ATTR_DRAM       0                   // DRAM attribute for SAD rule
#define SAD_ATTR_MMCFG      1                   // MMCFG attribute for SAD rule
#define SAD_ATTR_DDRT       2                   // DDRT/Low Bandwidth memory attribute for SAD rule
#define SAD_ATTR_HBM        3                   // High Bandwidth memory attribute for SAD rule
#define SAD_NXM_ATTR        2                   // Attr value for an NXM SAD rule
#define SAD_CXL_ATTR        4                   // Attr value for a CXL SAD rule
#define SAD_FPGA_ATTR       5                   // Attr value for a FPGA Remote SAD rule(identification purpose only, not for CSR programming)
#define SAD_UNIT            26                  // SAD limit unit is 64MB aka 2^26
#define MAX_RT_WAYS         8                   // Max. interleave ways for DDR/DDRT RTs in 256B McChan granularity
#define MAX_4K_DDRT_RT_WAYS 4                   // Max. interleave ways for DDRT RT in 4K McChan granularity
#define MAX_RT              2                   // Number of RTs per route table type
#define MAX_TGT_LIST_ENTRY  8                   // Number of entries in a target/interleave list
#define MAX_SINGLE_TGT_IDX  4                   // Maximum index into RT for single target interleave
#define MAX_PAD             3                   // Number of PAD registers
#define MAX_RAW_CARD        5                   // Number of supported Raw Cards
#define MAX_MC_HALF         (MAX_IMC / MAX_RT)  // Number of MCs per half
#define RT_CHAN_WIDTH       2                   // shift size for route table channel
#define RT_TARG_WIDTH       4                   // shift size for route table target
#define MAX_FPGA_REMOTE_SAD_RULES         2     // Maximum FPGA sockets exists on ICX platform

//
// CXL Memory Definition
//
#define MAX_CXL_HDM_RANGES  2                   // Maximum number of CXL HDM ranges per CXL accelerator.

//
// DDR5_TODO
//
#define MAX_STROBE_DDR5     20                  // Number of strobe groups
#define MAX_STROBE_DDR4     18                  // Number of strobe groups
#define MAX_CLK_BS_SUBCH_DDR5  4                // Max Clks of backside sub channel.
#define DELAY_GROUP_BS_SUBCH_DDR5  2                // Max Clks of backside sub channel.
#ifdef DDR5_SUPPORT
#define MAX_STROBE          20                  // Number of strobe groups
#else
#define MAX_STROBE          18                  // Number of strobe groups
#endif
#define PARITY_INTERLEAVE   1                   // SAD interleave with merged HAs (pseudo-hemisphere mode)
#define MAX_SEEDS           10                  // Maximum
#define NMEM                0
#define FMEM                1
#define RIR_GRAN            512                 // RIR granularity in MB
#define SAD_GRAN            64                  // SAD granularity in MB
#define CRC_ALERT_DDR4      13000               // 13ns or 13k ps

#define MAX_RANK_DIMM_3DS   2                   // Max physical CS ranks per 3DS DIMM
#define MAX_SUBRANK_3DS     4                   // Max logical C[2:0] subranks per CS in 3DS DIMM
#define MAX_SPARE_RANK      2                   // Max number of spare ranks in a channel
#ifdef DDR5_SUPPORT
#define MAX_BANK_RANK       32                  // Max banks per rank
#else
#define MAX_BANK_RANK       16                  // Max banks per rank
#endif
#define MAX_SUBRANK         2                   // Max subranks per logical rank
#define SPD_MODULE_PART     18                  // Number of bytes of module part - DDR3
#define SPD_MODULE_PART_DDR4 20                 // Number of bytes of module part - DDR4
#define SPD_MODULE_PART_DDR5 30                 // Number of bytes of module part - DDR5
#define SPD_MODULE_PART_UNUSED  0x20            // Unused Module Part Number bytes, encoded as ASCII blanks
#define SPD_MODULE_SERIAL    4                  // Number of bytes of Module Serial Number
#define MAX_PB_DESKEW_GROUP 4                   // Max Per-Bit Deskew Groups
#define SAD_RULES_ADDR_RANGE  4                 // Max IOT rules = 4, Total address limits (lower(4) entries each)
#define BITS_PER_NIBBLE 4
#define MAX_PARTIAL_MIRROR      4               //Maximum number of partial mirror regions that can be created

#define CONVERT_64MB_TO_4KB_GRAN    14
#define CONVERT_4KB_TO_64MB_GRAN    14
#define CONVERT_64MB_TO_GB_GRAN     4
#define CONVERT_GB_TO_64MB_GRAN     4
#define CONVERT_64MB_TO_MB_GRAN     6
#define CONVERT_MB_TO_64MB_GRAN     6
#define CONVERT_64MB_TO_4GB_GRAN    6
#define CONVERT_4GB_TO_64MB_GRAN    6
#define CONVERT_64MB_TO_32GB_GRAN   9
#define CONVERT_64B_TO_64MB         20
#define CONVERT_B_TO_MB             20
#define CONVERT_MB_TO_B             20
#define CONVERT_B_TO_64MB           26
#define CONVERT_64MB_TO_B           26
#define CONVERT_64MB_TO_128MB_GRAN  1
#define CONVERT_256MB_TO_64MB_GRAN  2
#define CONVERT_64MB_TO_256MB_GRAN  2
#define CONVERT_B_TO_256MB_GRAN     28

#define MEM_1GB_AT_64MB_GRAN        0x10
#define MEM_1GB_AT_4KB_GRAN         0x40000

#define GB_TO_MB_CONVERSION 1024
#define MAX_2LM_NM 512                           //Maximum near memory size in GB
#define MAX_2LM_NM_IN_MB  MAX_2LM_NM * GB_TO_MB_CONVERSION  //Maximum near memory size in MB
#define MIN_2LM_NM 4                             //Minimum near memory size in GB
#define MIN_2LM_NM_IN_MB MIN_2LM_NM * GB_TO_MB_CONVERSION  // Minimum near memory size in MB
#define  TX_DQ_START_OFFSET       32
#define  TX_DQ_START_OFFSET_2TO2  96

// Ctl FUBs
#define CMD_CTL_B 10                            // Number of CMDCTL B signals
#define NUM_CTL_PLATFORM_GROUPS 4

// NVM DIMMs
#define MAX_MANUFACTURER_STRLEN 2
#define MAX_SERIALNUMBER_STRLEN 4
#define MAX_MODELNUMBER_STRLEN 20

//
// Socket/MC/Ch Interleave Granularity
#define INTERLEAVE_GRANULARITY_64B         0
#define INTERLEAVE_GRANULARITY_256B        1
#define INTERLEAVE_GRANULARITY_4KB         2
#define INTERLEAVE_GRANULARITY_8KB         3
#define INTERLEAVE_GRANULARITY_2KB         4

#define BITMAP_CH0_CH1_CH2  ( ( BIT0 ) | (BIT1 ) | (BIT2) )
#define BITMAP_CH0_CH1      ( ( BIT0 ) | (BIT1 ) )
#define BITMAP_CH1_CH2      ( ( BIT1 ) | (BIT2 ) )
#define BITMAP_CH0_CH2      ( ( BIT0 ) | (BIT2 ) )
#define BITMAP_CH0       BIT0
#define BITMAP_CH1       BIT1
#define BITMAP_CH2       BIT2

#define CONVERT_64MB_TO_BYTE              64 * 1024 * 1024


#define MEM_MAP_LVL_NORMAL              0
#define MEM_MAP_LVL_IGNORE_CFGIN        1
#define MEM_MAP_LVL_IGNORE_CFGIN_IGNORE_NEW_DIMMS   2
#define MEM_MAP_VOL_MEM_MODE_UNDEFINED    2


#define MEM_MAP_STATE_RESOURCE_CALCULATION             0
#define MEM_MAP_STATE_RESOURCE_CALCULATION_FAILURE     1
#define MEM_MAP_STATE_RESOURCE_CALCULATION_DONE        2
#define MEM_MAP_STATE_PARTITION_FAILURE                3


//These are the error code that are used to handle Resource unavailability.
//The error codes begin at 0x20 to make sure it doesnt overlap with error codes defined for NGN dimms.
#define ERROR_RESOURCE_CALCULATION_COMPLETED           0x020
#define ERROR_RESOURCE_CALCULATION_FAILURE             0x021
#define ERROR_ROUTETABLE_CALCULATION_FAILURE           0x022

//
// MC egress credit count values
//
#define MC_CREDIT_COUNT_1_CHANNEL_ENABLED    63
#define MC_CREDIT_COUNT_2_CHANNELS_ENABLED   31
#define MC_CREDIT_COUNT_3_CHANNELS_ENABLED   20

//
// SPD Defines
//-----------------------------------------------------------------------------

typedef struct {
  INT32 effBusDelaySR;
  INT32 effBusDelayDR;
  INT32 effBusDelayDD;
  UINT8 cas2RecEnDR;
  UINT8 cas2RecEnDD;
  UINT8 cas2DrvEnDR;
  UINT8 cas2DrvEnDD;
  UINT8 odtRdLeading;
  UINT8 odtRdTrailing;
  UINT8 odtWrLeading;
  UINT8 odtWrTrailing;
  UINT8 clkDiffDR;
  UINT8 clkDiffDD;
  UINT8 Gnt2Erid;
} BUS_TIMING_DATA;


#define CMD_GRP_DELAY     0
#define CMD_SIGNAL_DELAY  1
#define CMDCTL_SIGNAL_DELAY   2

//
// CpgcAdvCmdParity modes
//
typedef enum {
  RestoreCadbPattern,
  BacksideCadbSetup,
  NormalCadbSetup,
  ModeMax
} CPGC_ADV_CMD_PARITY_MODE;

#define UNMARGINED_CMD_EDGE   511

#define NUM_CS                    8
#define NUM_CKE                   4
#define NUM_ODT                   4
#define NUM_CA_SIGNALS            0
#define NUM_CTL_SIGNALS           (NUM_CS + NUM_CKE + NUM_ODT)
#define NUM_SIGNALS_TO_SWEEP_LB   (NUM_CTL_SIGNALS + NUM_CA_SIGNALS)

// For SKX: 0011 1111 (3F) when all channels are present on a socket ..as this is needed outside of BSSA also
// For ICX: 1111 1111 (FF) when all channels are present on a socket ..as this is needed outside of BSSA also
#define CH_BITMASK        ((1 << (MAX_IMC * MAX_MC_CH)) - 1)

//
// Per Bit De-Skew for IVT
//
#define MAX_PER_BIT_DESKEW_RANKS      4

#define CAVREF                    64    //Intialize CAVREF for B0
#define MAX_RD_VREF_OFFSET        48
#define MAX_RX_SAMPLER_OFFSET     63
#define MAX_WR_VREF_OFFSET_DDRT   64
#define MAX_WR_VREF_OFFSET        31
#define MAX_WR_VREF_OFFSET_DDR5   60
#define MAX_WR_VREF_OFFSET_BACKSIDE 50
#define MAX_WR_VREF_OFFSET_BACKSIDE_DDRT 63
#define MAX_VREF_OFFSET_RANGE     ((MAX_WR_VREF_OFFSET_DDRT * 2) + 1)
#define MAX_TIMING_OFFSET         31
#define MAX_FAN_STEPS             3
#define FAN_RIGHT                 0
#define FAN_LEFT                  1
#define FAN_CENTER                2
#define FAN_ENABLE                1
#define FAN_DISABLE               0

//
// Thermal equates
//
#define DRAM_TCASE_DEFAULT  (85 << 4)   // 85 degrees C
#define DRAM_TCASE_MAX      (95 << 4)   // 95 degrees C

#pragma pack(1)

//
// -----------------------------------------------------------------------------
//
// Host enumerated modes
//
#define CH_INDEPENDENT  0
#define FULL_MIRROR_1LM         BIT0
#define FULL_MIRROR_2LM         BIT1
#define CH_LOCKSTEP             BIT2
#define RK_SPARE                BIT3
#define PARTIAL_MIRROR_1LM      BIT5
#define PARTIAL_MIRROR_2LM      BIT6
#define STAT_VIRT_LOCKSTEP      BIT7
#define CH_ML                   (FULL_MIRROR_1LM | FULL_MIRROR_2LM | STAT_VIRT_LOCKSTEP)
#define CH_SL                   (RK_SPARE  | STAT_VIRT_LOCKSTEP)
#define CH_MS                   (RK_SPARE  | FULL_MIRROR_1LM | FULL_MIRROR_2LM)
#define CH_MLS                  (FULL_MIRROR_1LM | FULL_MIRROR_2LM | STAT_VIRT_LOCKSTEP | RK_SPARE)
#define CH_ALL_MIRROR           (FULL_MIRROR_1LM | FULL_MIRROR_2LM | PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM)
#define MAX_PCLS_REGION         16
//
// Host extended RAS Modes
//
#define SDDC_EN             BIT0
#define DMNDSCRB_EN         BIT1
#define PTRLSCRB_EN         BIT2
#define ADDDC_ERR_INJ_EN    BIT3
#define PCLS_EN             BIT4
#define MIRROR_SCRUB_EN     BIT5
#define ADDDC_EN            BIT6
#define PTRLSCRB_EOP_EN     BIT8

// ADDDC region sizes for Static Virtual LS, ADDC
#define ADDDC_REGION_SIZE_RANK    1
#define ADDDC_REGION_SIZE_BANK    0

#define MEM_RANGE_INVALID              0
#define MEM_RANGE_PERSISTENT           BIT0
#define MEM_RANGE_PART_PERSISTENT      BIT1
#define MEM_RANGE_VOLATILE             BIT2
#define MEM_RANGE_PART_VOLATILE        BIT3
#define MEM_RANGE_MIRRORED             BIT4
#define MEM_RANGE_PART_MIRRORED        BIT5
#define MEM_RANGE_RESERVED             BIT6
#define MEM_RANGE_PART_RESERVED        BIT7
#define MEM_RANGE_CTRL_REGION          BIT8
#define MEM_RANGE_PART_CTRL_REGION     BIT9
#define MEM_RANGE_VOLATILE_RESERVED    BIT12
//
// Host bit-mapped memSetup.options
//
//                   ; TEMPHIGH_EN enables support for 95 degree DIMMs
//                   ; PDWN_IBT_OFF_EN enables power down IBT off.
//                   ; PDWN_SR_CKE_MODE enables CKE to be tri-stated during register clock off power down self-refresh
//                   ; OPP_SELF_REF_EN enables the opportunistic self refresh mechanism
//                   ; MDLL_SHUT_DOWN_EN enables MDLL shutdown
//                   ; PAGE_POLICY: Clear for open page, set for closed page. Open page has better performance and
//                   ;  power usage in general. Close page may benefit some applications with poor locality
//                   ; ALLOW2XREF_EN enables 2X refresh if needed for extended operating temperature range (95degrees)
//                   ;  If TEMPHIGH_EN is also set, setting this bit will result in 2X refresh timing for the IMC
//                   ;  refresh control register
//                   ; MULTI_THREAD_MRC_EN enables multithreaded MRC. This reduces boot time for systems with multiple
//                   ;  processor sockets
//                   ; ADAPTIVE_PAGE_EN enables adaptive page mode. The memory controller will dynamically determine how long
//                   ;  to keep pages open to improve performance
//                   ; CMD_CLK_TRAINING_EN enables command to clock training step in BIOS
//                   ; SCRAMBLE_EN Set to enable data scrambling. This should always be enabled except for debug purposes.
//                   ; BANK_XOR_EN enables bank XOR memor mapping mode which is targeted at workloads with bank thrashing
//                   ;  caused by certain stride or page mappings
//                   ; RX_SENSE_AMP_EN < NOT CURRENTLY USED IN FUNCTIONAL CODE >
//                   ; DDR_RESET_LOOP enables infinite channel reset loop without retries for gathering of margin data
//                   ; NUMA_AWARE enables configuring memory interleaving appropriately for NUMA aware OS
//                   ; DISABLE_WMM_OPP_READ disables issuing read commands opportunistically during WMM
//                   ; ECC_CHECK_EN Enables ECC checking
//                   ; ECC_MIX_EN enables ECC in a system with mixed ECC and non-ECC memory in a channel by disabling ECC when
//                   ;  this configuration is detected.
//                   ; DISABLE_ECC_SUPPORT Disable ECC Check
//                   ; RAS_TO_INDP_EN switches from lockstep or mirror mode to independenct channel mode when memory is present
//                   ;  on channel 2 and this is enabled
//                   ; MARGIN_RANKS_EN Enables the rank margin tool
//                   ; MEM_OVERRIDE_EN enables use of inputMemTime inputs as hard overrides
//                   ; DRAMDLL_OFF_PD_EN < NOT CURRENTLY USED IN FUNCTIONAL CODE >
//                   ; MEMORY_TEST_EN enables execution of MemTest if on cold boot
//                   ; MEMORY_TEST_COLD_FAST_BOOT_EN enables the memory test when going through a cold fast boot path
//                   ; ATTEMPT_FAST_BOOT attempts to take a fast boot path if the NVRAM structure is good and the memory config
//                   ;  hasn't changed.  For example, on a warm boot, this will take the "fast warm" path through MRC which attempts
//                   ;  to make it as close as possible to the S3 path.
//                   ; SW_MEMORY_TEST_EN < NOT CURRENTLY USED IN FUNCTIONAL CODE >
//
#define TEMPHIGH_EN                   BIT0
#define ATTEMPT_FAST_BOOT_COLD        BIT1
#define PDWN_SR_CKE_MODE              BIT2
#define OPP_SELF_REF_EN               BIT3
#define MDLL_SHUT_DOWN_EN             BIT4
#define PAGE_POLICY                   BIT5
#define ALLOW2XREF_EN                 BIT6
#define MULTI_THREAD_MRC_EN           BIT7
#define ADAPTIVE_PAGE_EN              BIT8
#define CMD_CLK_TRAINING_EN           BIT9
#define SCRAMBLE_EN                   BIT10  // for ddr4
#define SCRAMBLE_EN_DDRT              BIT11
#define DISPLAY_EYE_EN                BIT12
#define DDR_RESET_LOOP                BIT13
#define NUMA_AWARE                    BIT14
#define DISABLE_WMM_OPP_READ          BIT15
#define RMT_COLD_FAST_BOOT            BIT16
#define ECC_CHECK_EN                  BIT17
#define ECC_MIX_EN                    BIT18
#define DISABLE_ECC_SUPPORT           BIT19
#define CA_PARITY_EN                  BIT20
#define PER_NIBBLE_EYE_EN             BIT22
#define RAS_TO_INDP_EN                BIT23
#define MARGIN_RANKS_EN               BIT25
#define MEM_OVERRIDE_EN               BIT26
#define DRAMDLL_OFF_PD_EN             BIT27
#define MEMORY_TEST_EN                BIT28
#define MEMORY_TEST_COLD_FAST_BOOT_EN BIT29
#define ATTEMPT_FAST_BOOT             BIT30
#define SW_MEMORY_TEST_EN             BIT31
#define FASTBOOT_MASK                 (BIT30 | BIT1)

//
// optionsExt
//

#define SPD_CRC_CHECK             BIT0
// BIT1 is available
// BIT2 is available
#define TURNAROUND_OPT_EN_DDRT    BIT3
#define TCO_COMP_EN               BIT4
#define PDA_EN                    BIT5
#define TURNAROUND_OPT_EN         BIT6
// BIT7 is available
#define ALLOW_CORRECTABLE_ERROR   BIT8
#define ALLOW_CORRECTABLE_MEM_TEST_ERROR   BIT9
// BIT10 is available
// BIT11 is available
// BIT12 is available
#define PER_BIT_MARGINS           BIT13
#define DUTY_CYCLE_EN             BIT14
#define LRDIMM_BACKSIDE_VREF_EN   BIT15
// BIT16 is available
#define DRAM_RX_EQ_EN             BIT17
// BIT18 is available
// BIT19 is available
// BIT19 is available
#define OPTIONS_EXT_RESERVED1     BIT21
#define EARLY_WR_CRC_DDRIO        BIT22
#define WR_CRC                    BIT23
#define OPTIONS_EXT_RESERVED2     BIT24
// BIT25 is available
// BIT26 is available
// BIT27 is available
// BIT28 is available
#define DIMM_ISOLATION_EN         BIT29
// BIT30 is available
#define TX_ECHO_CANCEL_EN         BIT31

//
// optionsNgn
//
#define NGN_CMD_TIME               BIT1
#define NGN_DEBUG_LOCK             BIT6
#define NGN_ARS_PUBLISH            BIT9
#define NGN_ECC_EXIT_CORR          BIT10
#define NGN_ECC_CORR               BIT11
#define NGN_ECC_WR_CHK             BIT12
#define NGN_ECC_RD_CHK             BIT13

//
// Host bit-mapped Memory Thermal Throttling Related options (mem.thermalThrottlingOptions)
//
#define MH_OUTPUT_EN          BIT0  // Enable for MEM_HOT output generation logic
#define MH_SENSE_EN           BIT1  // Enable for MEM_HOT sense logic
#define OLTT_EN               BIT2  // Enable open loop thermal throttling control
#define CLTT_EN               BIT3  // Enable closed loop thermal throttling control
#define CLTT_PECI_EN          BIT4  // Enable closed loop thermal throttling control w/PECI
#define OFFPKG_MEM_TO_THERMTRIP_EN   BIT5  // Enable offpkg MEMTRIP in ThermTrip tree
#define OFFPKG_MEM_TO_MEMTRIP_EN     BIT6  // Enable Offpkg Memtrip tree
#define MH_INPUT_EN           BIT7  // Enable for MEM-hot input enable
#define INPKG_MEM_TO_THERMTRIP_EN   BIT8  // Enable InPkg MEMTRIP in ThermTrip tree
#define INPKG_MEM_TO_MEMTRIP_EN     BIT9  // Enable InPkg Memtrip tree

//
// Flag for B2P ENABLE_DRAM_PM & ENABLE_DRAM_THERMALS
//
#define DISABLE_DRAM_MPM                0
#define ENABLE_DRAM_PM                  BIT0
#define ENABLE_DRAM_THERMAL             BIT1

//
// Define the WDB line. The WDB line is like the cache line.
//
#define MRC_WDB_LINES                 32
#define MRC_WDB_LINE_SIZE             64
#define MRC_WDB_PATTERN_P             0x55555555
#define MRC_WDB_PATTERN_N             0xAAAAAAAA
#define CADB_LINES                    16

#define NORMAL_MRS                    0           //NORMAL_MRS  - How writeCADBcmd usually works
#define LOAD_MRS                      BIT0        //LOAD_MRS - writeCADBcmd will load commands into cadb buffer but not execute
#define EXECUTE_MRS                   BIT1        //EXECUTE_MRS - writeCADBcmd will execute the stack of commands
#define CADB_CACHELINE_LIMIT          15

//
// Define in the Critical Section function on what to wait.
//
typedef enum {
  DoneAndRefDrained,
  Done,
  Immediate
} EWaitOn;

typedef enum {
  ssOne = 0,
  ssTwo,
  ssThree,
  ssFour,
} TSubSequencesNumber;

//
// Module definitions
//
#define RE_LOGIC_DELAY_MAX_VALUE             2   // define receive enable logic delay max value
#define RE_EDGE_SIZE_THRESHOLD               3   // define the min of valid one edge size
#define RE_MAX_LOGIC_DELAY                   7   // max logic delay value
#define RE_TRAINING_RESULT_HIGH_IN_BITS      32  // bit start of the training result
#define MAX_PHASE_IN_FINE_ADJUSTMENT         64
#define MAX_PHASE_IN_READ_ADJUSTMENT         72
#define MAX_PHASE_IN_READ_ADJUSTMENT_DQ      120 // larger range for added DQ 1/16 PI adjustments

#define RXDQS_SWEEP_START                      0
#define RXDQS_SWEEP_END                      176
#define READ_DQDQS_DDR5_LENGTH               (RXDQS_SWEEP_END - RXDQS_SWEEP_START)

#define MAX_PHASE_IN_READ_ADJ_DQ_RX_DFE      152 // larger range for added DQ 1/16 PI adjustments
#define RAS_ROW_BITS                         6   // (16 row bits) define the number of row bits from RAS command
#define RAS_LR_ROW_BITS                      1   // (18 row bits) define the number of row bits from RAS command
#define CAS_ROW_BITS                         0   // (10 column bits) for CAS command

#define TX_LOGIC_DELAY_MAX_VALUE        2   // define write max logic delay
#define ALL_CH                          0xFF
#define ALL_STROBES                     0xFF
#define ALL_BITS                        0xFF  // All bits modified at once
#define EACH_BIT                        0xFE  // Each bit modified independently
#define MRC_ROUND_TRIP_MAX_SCR_B0_VALUE 47  // MAX Round Trip value scrambler can handle - Fixed in C0 (with Throttling)

#define RE_LRDIMM_EDGE_SMALL_THRESHOLD_PHASE  32
#define RE_LRDIMM_EDGE_LARGE_THRESHOLD_PHASE  96
#define RE_LRDIMM_EDGE_SIZE_THRESHOLD         0

//SKX change
#define  TX_PER_BIT_SETTLE_TIME         0

#define   REC_EN_STEP_SIZE  1
#define   REC_EN_PI_START   0
#define   REC_EN_PI_RANGE   128

#define   LRDIMM_BACKSIDE_PI_START   0
#define   LRDIMM_BACKSIDE_PI_RANGE   128
#define   LRDIMM_MRD_BACKSIDE_PI_RANGE  32
#define   LRDIMM_BACKSIDE_STEP_SIZE  2
#define   LRDIMM_BACKSIDE_CYCLE_STEP_SIZE  1
#define   LRDIMM_BACKSIDE_READ_DELAY_START   -15
#define   LRDIMM_BACKSIDE_READ_DELAY_END      16
#define   LRDIMM_BACKSIDE_READ_DELAY_STEP_SIZE  1

#define   LRDIMM_BACKSIDE_WRITE_DELAY_START   -15
#define   LRDIMM_BACKSIDE_WRITE_DELAY_END      16
#define   LRDIMM_BACKSIDE_WRITE_DELAY_STEP_SIZE  1
#define   DDR5_LRDIMM_BACKSIDE_WRITE_DELAY_START  0
#define   DDR5_LRDIMM_BACKSIDE_WRITE_DELAY_END    256

#define   WR_LVL_STEP_SIZE  1
#define   WR_LVL_LOOP_COUNT 1
#define   WR_LVL_PI_START   128
#define   WR_LVL_PI_RANGE   128

#define   WR_DQ_STEP_SIZE  1
#define   WR_DQ_LOOP_COUNT 1
#define   WR_DQ_PI_START   0
#define   WR_DQ_PI_RANGE   128

#define MAX_PHASE_IN_FINE_ADJUSTMENT_WR_DDR5    256

#define  TX_DQ_START_OFFSET_DDR5  128 //256 BRS TODO: the sweep range needs to be optimized
#define  WR_DQ_PI_START_DDR5      0
#define  WR_DQ_PI_RANGE_DDR5      384 //512

//
// define the WDB line size.
//
#define MRC_WDB_BYTES_PER_TRANSFER    8
#define MRC_WDB_TRANSFERS_PER_LINE    8

#define SUB_SEQUENCES_NUMBER  4
#define MRC_WDB_NUM_MUX_SEEDS   3  // Number of WDB Mux
#define MRC_CADB_NUM_MUX_SEEDS  4  // Number of CADB Mux

#pragma pack()

#define CMD_STEP_SIZE         1
#define EARLY_CMD_STEP_SIZE   2
#define CTL_CLK_STEPSIZE      2

#define CTL_CLK_TEST_RANGE  256

#define MAX_CMD_MARGIN  255

#define BACKSIDE_SWEEP_START -31
#define BACKSIDE_SWEEP_END    31
#define BACKSIDE_SWEEP_RANGE  64
#define BACKSIDE_STEP_SIZE    1
#define BACKSIDE_SIGNAL_RANGE 26

#define PAR_TEST_START  0
#define PAR_TEST_RANGE  256
#define MAX_SAMPLES     128

// CMD Normalization
#define QCLKPITICKS  64 // QCLK PI Ticks
#define DCLKPITICKS 128 // DCLK PI Ticks
#define PITICKRANGE 511
#define EXTRAPIMARGIN 32
#define CMD_NORMALIZE_TXDQ_GUARDBAND        63
#define DENORMALIZE       -1
#define RENORMALIZE        1

//
// Timing mode Equates
//
#define TIMING_1N             0
#define TIMING_2N             2
#define TIMING_3N             3

//
// Definitions for when MrcDebugTurnarounds
// PCD is turned on
//

#define TA_POR_T_RRDR     0
#define TA_POR_T_RRDD     1
#define TA_POR_T_WWDR     2
#define TA_POR_T_WWDD     3
#define TA_POR_T_WRDR     4
#define TA_POR_T_WRDD     5
#define TA_POR_T_RWSR     6
#define TA_POR_T_RWDR     7
#define TA_POR_T_RWDD     8
#define TA_POR_T_CCD      9
#define TA_POR_T_EODT     10
#define TA_POR_TYPES      11     // Number of timing parameters

//
// The definition in DDR4DB02 Jedec, refer to F2BCEx, F3BCCx
//
#define DFE_ENABLE_GLOBAL     BIT7
#define DFE_WRITE_PER_DQ      BIT3
#define DFE_TAP_NEG           BIT7
#define DFE_TAP_ENABLE        BIT6

//
// defines for Receive enable
//
#define   RT_GUARDBAND                4
#define   DECREMENT_ROUNDTRIP_VALUE   2 // DCLK
#define   FINE_DELAY_FEEDBACK_MSK     0x1FF
#define   RESULT_STROBE_INDEX         MAX_STROBE

#define SKX_PCKG_TYPE 4 //CMD_CTL_DELAY_H

#define CHIP_IOGPDLY_PSECS SKX_PCKG_TYPE

// MemTiming.c


// MemTraining.c
#define RD_WR_CMD_INIT_MARG 5
#define CMD_VREF_INIT_MARG  5
#define CMD_INIT_MARG       15 // Initial CMD margining sweep starting setting
#define CMD_MARG_OFFSET     15 // CMD margining sweep setting from trained edges
#define RX_TX_DQ_INIT_MARG  5

#pragma pack(1)
typedef struct TADTable {
  UINT8   Enable;             // Rule enable
  UINT8   SADId;              // SAD Index
  UINT8   socketWays;         // Socket Interleave ways for TAD
  UINT8   NmTadIndex;         // Index of near memory TAD
  UINT8   FmTadIndex;         // Index of far memory TAD
  UINT32  Limit;              // Limit of the current TAD entry
  UINT8   TargetGran;         // MC granularity of 1LM forward and 2LM forward/reverse address decoding.
  UINT8   ChGran;             // Channel granularity of 1LM forward and 2LM forward/reverse address decoding.
} TAD_TABLE;

typedef struct SADTable {
  UINT8    Enable;         // Rule enable
  MEM_TYPE type;           // Bit map of memory region types, See defines 'MEM_TYPE_???' above for bit definitions of the ranges.
  UINT8    granularity;    // Interleave granularities for current SAD entry.  Possible interleave granularity options depend on the SAD entry type.  Note that SAD entry type BLK Window and CSR/Mailbox/Ctrl region do not support any granularity options
  UINT32   Base;           // Base of the current SAD entry
  UINT32   Limit;          // Limit of the current SAD entry
  UINT8    ways;           // Interleave ways for SAD
  UINT8    channelInterBitmap[MAX_IMC];   //Bit map to denote which DDR4/NM channels are interleaved per IMC eg: 111b - Ch 2,1 & 0 are interleaved; 011b denotes Ch1 & 0 are interleaved
  UINT8    FMchannelInterBitmap[MAX_IMC];   //Bit map to denote which FM channels are interleaved per IMC eg: 111b - Ch 2,1 & 0 are interleaved; 011b denotes Ch1 & 0 are interleaved
  UINT8    NmChWays;         // Channel Interleave ways for SAD. Represents channelInterBitmap ways for DDR4/NM.
  UINT8    FmChWays;         // Channel Interleave ways for SAD. Represents FMchannelInterBitmap ways for DDRT.
  UINT8    imcInterBitmap;   // Bit map to denote which IMCs are interleaved from this socket.
  UINT8    NmImcInterBitmap; // Bit map to denote which IMCs are interleaved from this socket as NM (10nm usage only).
  BOOLEAN  local;          //0 - Remote 1- Local
  UINT8    IotEnabled;     // To indicate if IOT is enabled
  UINT8    mirrored;       //To Indicate the SAD is mirrored while enabling partial mirroring
  UINT8    Attr;
  UINT8    tgtGranularity; // Interleave mode for target list
  UINT8    Cluster;        // SNC cluster, hemisphere, or quadrant index.
} SAD_TABLE;

//
// FPGA Mem Entries will present only in Remote DRAM rules
// These entries must be stored in memmap host to create HOB elements required for SLIT/SRAT
//
typedef struct _RSAD_TABLE_FPGA {
  UINT8   Enable;         // Rule enable
  UINT8   Attr;           // 05 - fpga
  UINT32  Base;           // Address of remote DRAM rule base
  UINT8   NodeId;         // Target Node for this DRAM rule
  UINT32  Limit;          // Address of remote DRAM rule limit
} RSAD_TABLE_FPGA;

typedef struct IMC {
  UINT8     imcEnabled[MAX_IMC];
  UINT8     imcNum;                                   // imc Number
  UINT32    memSize;                                  // DDR4 memory size for this imc (64MB granularity)
  UINT32    NVmemSize;                                // NV Memory size of this ha
  UINT32    volSize;                                  // Volatile size of the NVM dimms for this imc (64MB granularity)
  UINT32    NonVolSize;                               // Non-Volatile size of the NVM DIMMs for this iMC (64MB granularity)
  UINT32    perSize;                                  // Persistent size of the NVM dimms for this imc (64MB granularity)
  UINT32    WbCachePerSize;                           // Persistent WB cache (AD-WB) size of the NVM dimms for this imc (64MB granularity)
  UINT8     TADintList[MAX_TAD_RULES][MAX_TAD_WAYS];  // TAD interleave list for this socket
  UINT8     TADChnIndex[MAX_TAD_RULES][MAX_TAD_WAYS]; // Corresponding TAD channel indexes (per channel)
  INT32     TADOffset[MAX_TAD_RULES][MAX_TAD_WAYS];   // Corresponding TAD offsets (per channel)
  TAD_TABLE TAD[MAX_TAD_RULES];                       // TAD table
  UINT8     imcChannelListStartIndex;                 // Index in channel list of first channel on this imc
} IMC_INFO_STRUCT;

typedef struct firmwareRev {
  UINT8  majorVersion;
  UINT8  minorVersion;
  UINT8  hotfixVersion;
  UINT16 buildVersion;
} FIRMWARE_REV;

typedef struct dfxMemVars {
  UINT8                 DfxDdrtChInterleaveGran;
  UINT8                 dfxDimmManagement;
  UINT8                 dfxPartitionDDRTDimm;
  UINT8                 dfxPartitionRatio[MAX_SOCKET * MAX_IMC];
  UINT8                 dfxCfgMask2LM;
  UINT8                 dfxPerMemMode;                      // 0: Non Persistent Mode 1: Persistent Mode 2: Persistent mode Enable
  UINT8                 dfxHighAddrBitStart;                // valid range is 33-51
} MEM_DFXMEMVARS;

// HIGH_ADDR_EN enables extention of the MMIO hole to force memory to high address region
//#define HIGH_ADDR_EN        BIT0

#pragma pack()

//NGN error codes
#define NGN_CFG_CUR_REVISION_MISMATCH                 BIT0
#define NGN_CFG_CUR_CHECKSUM_FAILURE                  BIT1
#define NGN_CFG_CUR_INTERLEAVE_FAILURE                BIT2
#define NGN_CFG_REQ_REVISION_MISMATCH                 BIT4
#define NGN_CFG_REQ_CHECKSUM_FAILURE                  BIT5
#define NGN_CFG_REQ_INTERLEAVE_FAILURE                BIT6

#define AEP_MAJOR_VERSION_WPQ_ENABLED                 0x1
#define AEP_MINOR_VERSION_WPQ_ENABLED                 0x0
#define AEP_BUILD_NUMBER_WPQ_ENABLED                  0x4774

#define     MAX_SI_SOCKET        8   // Maximum silicon supported socket number

typedef struct {
  UINT32  BlockDecoderBase;   // 64MB unit
  UINT32  BlockDecoderLimit;
  UINT8   BlockSocketEnable;
  UINT8   BlockMcChEn[MAX_SI_SOCKET][MAX_IMC][MAX_MC_CH];
} MEMORY_MAP_BLOCK_DECODER_DATA;


#define MAX_PRMRR_COUNT          8

typedef struct {
  UINT64 ValidPrmrrBitMap;                      /// Indicate the supported PRMRR size based on current memory configure
  UINT8  PrmrrCount;                            /// Indicate the total number of PRMRR in a system
  UINT8  PrmrrCountPerPackage;                  /// Indicate the total number of PRMRR per package
  UINT64 PrmrrBase[MAX_PRMRR_COUNT];            /// Physical address for each specific PRMRR range
  UINT8  McBitMap[MAX_SOCKET][MAX_PRMRR_COUNT]; /// Memory Controller bitmap for each specific PRMRR range
} SGX_PRMRR_DATA;


//
// DDR XOR bitmap definitions
//
#define CLUSTER_XOR           BIT0
#define CHANNEL_XOR           BIT1
#define PSEUDO_CHANNEL_XOR    BIT2
#define DIMM_XOR              BIT3
#define RANK_XOR              BIT4
#define SID_XOR               BIT5
#define BANK_XOR              BIT6

//
// Chip specific section of struct Socket
//
#define SOCKET_CHIP     \
  struct SADTable   SAD[MAX_DRAM_CLUSTERS * MAX_SAD_RULES];                                                                                  \
  UINT8             DdrtChRouteTable[MAX_RT][MAX_RT_WAYS];  /* PMEM/BLK memory channel route table 2 for CR protocol */  \
  UINT8             DdrtTgtRouteTable[MAX_RT][MAX_RT_WAYS]; /* PMEM/BLK memory target route table 2 for CR protocol */   \
  struct IMC        imc[MAX_IMC];                                                                                        \
  UINT8             ddrClkData;                                                                                          \
  UINT8             ddrClkType;                                                                                          \
  UINT8             ddrFreqCheckFlag;                                                                                    \
  UINT8             SktSkuValid;                                                                                         \
  UINT32            SktSkuLimit;                                                                                         \
  UINT32            SktTotMemMapSPA;                                                                                     \
  UINT32            SktMemSize2LM;                                                                                       \
  UINT8             maxFreq;                                                                                             \
  UINT8             clkSwapFixDis;    /* HSD4929953 */                                                                   \
  UINT8             ioInitdone;

//
// Power training defines
//
#define NUM_DRAMRON_SETTINGS    3
#define NUM_MCODT_SETTINGS      5
#define NUM_NONTGTODT_SETTINGS  9
#define NUM_RTTWR_SETTINGS      4
#define NUM_MCRON_SETTINGS      3
#define NUM_TX_EQ_SETTINGS      16
#define NUM_TX_RFSR_SETTINGS    16
#define NUM_TX_FALLSR_SETTINGS  16
#define NUM_IMODE_SETTINGS      9
#define NUM_IMODE_SETTINGS_10NM 24
#define NUM_RX_EQ_SETTINGS      8
#define NUM_CTLE_SETTINGS       5
#define NUM_CTLE_SETTINGS_10NM  16
#define NUM_BIAS_SETTINGS       2
#define NUM_EXT_CTLE_SETTINGS   24
#define NUM_TCO_COMP_SETTINGS   32
#define MAX_TCO_COMP            127
#define MIN_TCO_COMP            64
#define MAX_TCO_COMP_RANGE2     95
#define MIN_TCO_COMP_RANGE2     64
#define MAX_TCO_COMP_RANGE1     31
#define MIN_TCO_COMP_RANGE1     0
#define NUM_TCO_COMP_STEP       2
#define NUM_TCO_COARSE_SETTINGS 8
#define NUM_TCO_FINE_SETTINGS   8
#define NUM_CMD_TX_EQ_SETTINGS  12
#define NUM_PXC_SETTINGS        4
#define NUM_DDJC_SETTINGS       4
#define NUM_RX_DFE_PARAMS       2
#define NUM_RX_DFE_DQ_SETTINGS  7
#define NUM_RX_DFE_COEFF_SETTINGS_PASS_1 2
#define NUM_RX_DFE_SETTINGS_PER_PARAM_PASS_1 (NUM_RX_DFE_DQ_SETTINGS * NUM_RX_DFE_COEFF_SETTINGS_PASS_1)
#define NUM_RX_DFE_COEFF_SETTINGS_PASS_2 15
#define NUM_RX_DFE_SETTINGS     MAX ((NUM_RX_DFE_PARAMS * NUM_RX_DFE_SETTINGS_PER_PARAM_PASS_1), NUM_RX_DFE_COEFF_SETTINGS_PASS_2)
#define NUM_DRAM_RQ_EQ_SETTINGS 8
#define NUM_DB_DFE_TAP1_SETTINGS     15
#define NUM_DB_DFE_TAP2_SETTINGS     7
#define NUM_DB_DFE_TAP3_SETTINGS     15
#define NUM_DB_DFE_TAP4_SETTINGS     15

typedef struct _CHIP_ELY_CMD_CK_STRUCT {
  INT16                               le[MAX_CH];
  INT16                               re[MAX_CH];
  DUMMY_REG                           tcdbp;
  DUMMY_REG                           trainingEn;
} CHIP_ERLY_CMD_CK_STRUCT, *PCHIP_ERLY_CMD_CK_STRUCT;

typedef struct _CHIP_GET_CMD_MGN_SWP_STRUCT {
  UINT16              eyeSize[MAX_CH];
  UINT8               chEyemask;
  UINT8               chDonemask;
} CHIP_GET_CMD_MGN_SWP_STRUCT, *PCHIP_GET_CMD_MGN_SWP_STRUCT;

typedef struct _CHIP_PARITY_CHECKING_STRUCT {
  UINT32                                    tCrapOrg;
  DUMMY_REG                                 tCrap;
  DUMMY_REG                                 ddrCRCtlControls;
  DUMMY_REG                                 ddr4cactl;
  DUMMY_REG                                 cmdParInjCtl;
  DUMMY_REG                                 mcSchedChknBit;
  UINT8                                     timingModeOrg;
} CHIP_PARITY_CHECKING_STRUCT, *PCHIP_PARITY_CHECKING_STRUCT;

typedef struct _CHIP_SET_NORMAL_CMD_TIMING_STRUCT {
  DUMMY_REG                           tCrap;
  DUMMY_REG                           ddrtMiscDelay;
  UINT8                               timingMode;
} CHIP_SET_NORMAL_CMD_TIMING_STRUCT, *PCHIP_SET_NORMAL_CMD_TIMING_STRUCT;

typedef struct {
  DUMMY_REG                           cpgcMiscODTCtl;
} EXECUTE_CTL_CLK_TEST_CHIP_STRUCT, *PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT;

typedef struct chipstruct {
  DUMMY_REG                            dataControl0;
  DUMMY_REG                            dataControl0_1; //KV3
  DUMMY_REG                            dataControl2;

} CHIP_DDRIO_STRUCT, *PCHIP_DDRIO_STRUCT;

#define FLY_BY_CACHELINES   4
#define FLY_BY_LC           1
#define WR_FLY_BY_PATTERN   0x3CC300FF
#define TX_DQS_DQ_OFFSET    32
#define TX_DQS_DQ_OFFSET_2TO2  -32

#define CHIP_OFFSET_ARRAY_SIZE 11
#define CHIP_OFFSET_ARRAY_VALUES {-3,-2,-1, 0, 1, 2, 3, 4, 5, 6, 7}

typedef struct _CHIP_WL_CLEANUP_STRUCT {
  DUMMY_REG                          ddrtDimm0BasicTiming;
  DUMMY_REG                          tCOTHP;
  DUMMY_REG                          tCOTHP2;
} CHIP_WL_CLEANUP_STRUCT, *PCHIP_WL_CLEANUP_STRUCT;

#define CHIP_GSM_CSN_SIGNALSTOSWEEP         {PAR, CAS_N, A13, RAS_N, WE_N, A10, BA1, A0, BA0, A1, A3, A2, A4, A5, A6, A7, A8, A9, A12, A11, A17, BG1, ACT_N, BG0, C0, C1, C2}
#define CHIP_GSM_CSN_SIGNALSTOSWEEPBACKSIDE {PAR, ACT_N, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, BA0, BA1, BG0, BG1, C0, C1, C2};
#define NUM_SIGNALS_TO_SWEEP 27
#define NUM_SIGNALS_TO_SWEEP_BACKSIDE 27

#define CHIP_CMD_CLK_TEST_START  128
#define CHIP_CMD_CLK_TEST_RANGE  160

#define CHIP_CMD_GRP_STOP CmdGrp2

#define CHIP_RNDTRP_DIFF_MPR_OFF_3N_TO_1N 4
#define CHIP_RNDTRP_DIFF_MPR_OFF_3N_TO_2N 2
#define CHIP_RNDTRP_DIFF_MPR_OFF_2N_TO_1N 2
#define CHIP_RNDTRP_DIFF_MPR_OFF_2N_TO_2N 0
#define CHIP_RNDTRP_DIFF_MPR_OFF_1N_TO_1N 0

#define CHIP_GET_CTL_CLK_RES_MAX_STROBE MAX_STROBE

//-----------------------------------------------------------------------------
// Test Modes (mode)
#define MODE_XTALK            BIT0  // Placeholder - This has not been implemented
#define MODE_VIC_AGG          BIT1
#define MODE_START_ZERO       BIT2
#define MODE_POS_ONLY         BIT3
#define MODE_NEG_ONLY         BIT4
#define MODE_DATA_MPR         BIT5
#define MODE_DATA_FIXED       BIT6
#define MODE_DATA_LFSR        BIT7
#define MODE_ADDR_LFSR        BIT8
#define MODE_ADDR_CMD0_SETUP  BIT9
#define MODE_CHECK_PARITY     BIT10
#define MODE_DESELECT         BIT11
#define MODE_VA_DESELECT      BIT12
#define MODE_TETHER_PN        BIT13

// Scope of the margin
#define SCOPE_SOCKET      0
#define SCOPE_CH        1
#define SCOPE_DIMM      2
#define SCOPE_RANK      3
#define SCOPE_STROBE    4
#define SCOPE_BIT       5
#define SCOPE_SUBCH     6

// Vref centering method
#define NON_PER_BIT     0
#define PER_BIT         1

// Type of training Low Margin (LM) check
#define LM_BASIC           0
#define LM_ADVANCED        BIT1
#define LM_LRDIMM          BIT2

struct baseMargin {
  INT16 n;
  INT16 p;
};


// Output structures based on scope

struct chMargin {
  struct baseMargin channel[MAX_CH];
};
struct dimmMargin {
  struct baseMargin dimm[MAX_CH][MAX_DIMM];
};
struct rankMargin {
  struct baseMargin rank[MAX_CH][MAX_RANK_CH];
};
struct strobeMargin {
  struct baseMargin strobe[MAX_CH][MAX_RANK_CH][MAX_STROBE];
};
struct bitMargin {
  struct baseMargin bits[MAX_CH][MAX_RANK_CH][MAX_BITS];
};
struct bitMarginCh {
  struct baseMargin bits[MAX_CH][MAX_BITS];
};

struct subRankMargin {
  struct rankMargin subRank[MAX_SUBRANK];
};

// Mask of bits to ignore
typedef struct bitMask {
  UINT8 bits[MAX_CH][MAX_RANK_CH][MAX_STROBE/2];
} BITMASK_STRUCT, *PBITMASK_STRUCT;

// Enum used to index into array elements of [MaxRtHalves] dimensions
typedef enum {
  RtHalf0,
  RtHalf1,
  MaxRtHalves = MAX_RT
  } ROUTE_TABLE_HALVES;

#endif // _memhostchipcommon_h
