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

#ifndef  _CPU_PCI_ACCESS_H_
#define  _CPU_PCI_ACCESS_H_

#include "SysHost.h"
#include <UsraAccessType.h>


//
// CPU Types; this needs to be contiguous to assist in table look up
//
#define MAX_CPU_TYPES        6           // 0- SKX, 1 -ICX-SP, 2-KNH, 3-ICX-DE, 4-SNR, 5-SPR-SP


//
// Box Types; this needs to be contiguous to assist in table look up
//
#define  BOX_CHA_MISC        0
#define  BOX_CHA_PMA         1
#define  BOX_CHA_CMS         2        //for SKX
#define  BOX_CHABC           3
#define  BOX_PCU             4
#define  BOX_VCU             5
#define  BOX_M2MEM           6
#define  BOX_MC              7
#define  BOX_MCIO            8
#define  BOX_KTI             9
#define  BOX_M3KTI           10
#define  BOX_MCDDC           11
#define  BOX_M2UPCIE         12
#define  BOX_IIO_PCIE_DMI    13
#define  BOX_IIO_PCIE        14
#define  BOX_IIO_PCIE_NTB    15
#define  BOX_IIO_CB          16
#define  BOX_IIO_VTD         17
#define  BOX_IIO_DFX         18
#define  BOX_UBOX            19
#define  BOX_MS2IDI          20
#define  BOX_HBM             21        // For KNH HBM IO / SPR HBM IO
#define  BOX_SPD             22
#define  BOX_MSM             23        //for 10nm
#define  BOX_RLINK           24        //ICX_TODO check with Kyle to see which device
#define  BOX_SB              25        //for 10nm

#define  BOX_FPGA_SKX        20        // SKX FPGA BOXTYPE 20
#define  BOX_FPGA            26        //ICX_TODO

#define  BOX_FNV             27
#define  BOX_SMEE            28
#define  BOX_FXR             29        //for 10nm
#define  BOX_NPK             30        //for 10nm
#define  BOX_IEH             31        //for SPR
#define  BOX_SPK             32        //for SPR
#define  BOX_DINO            33        //for SPR
#define  BOX_IIO_M2IAL       34        //for SPR
#define  BOX_PI5             35        //for SPR
#define  BOX_SB_EX           36        //for SPR SB EX
#define  BOX_DRA             38        //for SPR
#define  BOX_MCCADB          39        //for SPR
#define  BOX_MCCPGC          40        //for SPR
#define  BOX_MDFIS           41        //for SPR
#define  BOX_SB_I3C          42        //for SPR I3C
#define  BOX_SB_FBLP         44        //for SPR SB FBLP
#define  BOX_S3M             45        //for SPR S3M
#define  BOX_HBM2E_MC        46        //for SPR HBM MC
#define  BOX_SBO             47
#define  BOX_CMS             48
#define  BOX_B2CXL           49
#define  BOX_B2HOT           50
#define  BOX_B2UBOX          51
#define  MAX_BOX_TYPES       52

//
// Maximum Number of Instances supported by each box type. Note that if the number of instances
// are same for all supported CPUs, then we will have only one #define here (i.e MAX_ALL_XXXXX)
//
#define  MAX_SKX_CHA            28
#define  MAX_ICX_CHA            40
#define  MAX_ICXD_CHA           12
#define  MAX_ICXG_CHA           20
#define  MAX_SNR_CHA            6
#define  MAX_SPR_CHA            60
#define  MAX_GNR_CHA            128
#define  MAX_SPRUCC_CHA         38

#define  MAX_SKX_M2PCIE         5

#define  MAX_ALL_CBOBC          1

#define  MAX_SKX_M3KTI          2

#define  MAX_CPX_M3KTI          3

#define  MAX_ICX_M3KTI          3

#define  MAX_SPR_M3KTI          4

#define  MAX_SKX_M2MEM          2               //SKX
#define  MAX_ICX_M2MEM          4               //ICX
#define  MAX_ICXG_M2MEM         8               //ICX-G
#define  MAX_SNR_M2MEM          1               //SNR

//
// Spr definitions with MAX_IMC > 4 is for GNR early development
// TODO: https://hsdes.intel.com/appstore/article/#/1507967521
//
#if MAX_IMC <= 4
#define  MAX_SPR_M2MEM          4               //SPR
#elif MAX_IMC <=8
#define  MAX_SPR_M2MEM          8
#elif MAX_IMC <= 12
#define  MAX_SPR_M2MEM          12
#endif
#define  MAX_SPR_HBM_M2MEM      20              //SPR+HBM
#define  MAX_SPR_UCC_M2MEM      2               // SPRUCC
#ifdef ICXD_LCC_SKU
  #define  MAX_ICXD_M2MEM       1               //ICX-D-LCC-SKU
#else
  #define  MAX_ICXD_M2MEM       2               //ICX-D-HCC
#endif

//
// GNR/SRF specific
//
#define  MAX_GNRSRF_M2MEM       24              //GNRSR

#define  MAX_GNRSRF_PCU         5
#define  MAX_GNRSRF_IEH_GLOBAL  1

#define  MAX_ALL_UBOX           1
#define  MAX_ALL_IIO            4
#define  MAX_ALL_PCU            1
#define  MAX_ALL_MSM            1
#define  MAX_ALL_NPK            1
#define  MAX_ALL_VCU            1

#define  MAX_ALL_IIO_DFX         21 // 4 instances per M/PSTACK + 1 Cstack
#define  MAX_ALL_IIO_PCIE        21 // 4 instances per M/PSTACK + 1 Cstack

//
// Functional Blocks supported by each box type; there is a 1:1 maping between these definitons and
// PCI function supported by a box. But the valude of the define itself doesn't have any relationship
// with the actual PCI Function number. That translation will be done by the helper routine for
// different supported CPU types.
//
#define  BOX_FUNC_CHA_MISC            0

#define  BOX_FUNC_CHA_PMA             0

#define  BOX_FUNC_CHABC_SAD           0
#define  BOX_FUNC_CHABC_SAD1          1

#define  BOX_FUNC_PCU_FUN1            1
#define  BOX_FUNC_PCU_FUN3            3
#define  BOX_FUNC_PCU_FUN5            5

#define  BOX_FUNC_M2MEM_MAIN          0

#define  BOX_FUNC_MC_MAIN             0
#define  BOX_FUNC_MC_2LM              1
#define  BOX_FUNC_MC_GLOBAL           2
#define  BOX_FUNC_MC_UNSPTD           4

#define  BOX_FUNC_KTI_LLPMON          0
#define  BOX_FUNC_KTI_REUT            1

#define  BOX_FUNC_MCDDC_CTL           0
#define  BOX_FUNC_MCDDC_DP            1

#define  BOX_FUNC_M2UPCIE_MAIN        0

#define  BOX_FUNC_IIO_PCIEDMI         0

#define  BOX_FUNC_IIO_PCIE            0

#define  BOX_FUNC_IIO_PCIENTB         0

#define  BOX_FUNC_MSM_PMON            1

#define  IIO_CB_FUNC0        0
#define  IIO_CB_FUNC1        1
#define  IIO_CB_FUNC2        2
#define  IIO_CB_FUNC3        3
#define  IIO_CB_FUNC4        4
#define  IIO_CB_FUNC5        5
#define  IIO_CB_FUNC6        6
#define  IIO_CB_FUNC7        7

#define  BOX_FUNC_IIO_VTD             0
#define  BOX_FUNC_IIO_RAS             2

#define  BOX_FUNC_IIO_DFX             0
#define  BOX_FUNC_IIO_DFX_DMI         1
#define  BOX_FUNC_IIO_DFX_GLOBAL      2
#define  BOX_FUNC_IIO_DFX_GLOBAL_DMI  3
#define  BOX_FUNC_IIO_DFX_VTD         4
#define  BOX_FUNC_IIO_DFX_VTD_DMI     5

//
// FOR SPR
//
#define  MAX_ALL_DINO                 6
#define  MAX_ALL_PI5                  3
#define  MAX_ALL_IEH                  9

#define  BOX_FUNC_UBOX_CFG            0
#define  BOX_FUNC_UBOX_DFX            1
#define  BOX_FUNC_UBOX_MISC           2
#define  BOX_FUNC_UBOX_M2U            5

//
// MCIO Function Blocks
//
#define  BOX_FUNC_MCIO_DDRIO          0
#define  BOX_FUNC_MCIO_DDRIOEXT       1
#define  BOX_FUNC_MCIO_DDRIOMC        2
#define  BOX_FUNC_MCIO_DDRIO_UNSPTD   4

#define  MSIDI_MAIN          0

// HBM Function Blocks
#define HBM_AWD              0
#define HBM_DWD              1
#define HBM_TRN              2
#define HBM_MC               3

#define PCU_INSTANCE_0 0

#define MAX_I3C_INSTANCE  2
#define I3C_INSTANCE_0    0   // I3C0
#define I3C_INSTANCE_1    1   // I3C1

//MSIDI intstance
#define MS2IDI_MULTICAST 0xFF
#define MS2IDI_IOCOH0  90
#define MS2IDI_IOCOH1  91

#define IIO_RLINK_DEVICE_NUMBER     3
#define IIO_RLINK_FUNCTION_NUMBER    0

#define IIO_EXPPTMBAR_OFFSET       10

// Gen3Phy PCIe BDF
#define IIO_PCIE_GEN3PHY_DEVICE_NUMBER     31
#define IIO_PCIE_GEN3PHY_FUNCTION_NUMBER   0

// Rlink Function blokcs
#define BOX_FUNC_SB_RLINK_LOGPHY         3
#define BOX_FUNC_SB_RLINK_ANALOG_OPIOPHY 4

//
// MDFIS instance ids assignment
//
// Normal MDFIS instance ID: 0~0x27
#define MDFIS_INST_OFFSET  0
// MDFIS reserved instance id for Normal MDFIS start: 0x28~0x3f
#define MDFIS_RESERVED_INST_OFFSET  0x28
// UPIMDF instance ID (it will cover both UPI IP and MDFIS IP in UPIMDF):       0x40~0x57
#define UPIMDF_INST_OFFSET  0x40
// The left MDFIS reserved instance id start: 0x58~0xFD
#define MDFIS_RESERVED1_INST_OFFSET  0x58
// MDFIS Multi-cast support --- MDFIS instance IDs:
#define SBO_MDFS_MULTICAST   0xFE
#define MDFIS_MDFS_MULTICAST 0xFF

//
// GNR/SRF MDFIS unicast
//
#define MAX_GNRSRF_MDFIS_ALL_IO_DIES       20      // Total MDFIS on all IO dies
#define MAX_GNRSRF_MDFIS_ALL_CMPT_DIES     60      // Total MDFIS on all compute dies
#define MAX_GNRSRF_MDFIS                   (MAX_GNRSRF_MDFIS_ALL_IO_DIES + MAX_GNRSRF_MDFIS_ALL_CMPT_DIES)

// I3C Global Addr based on SCF_BAR_MAP.csv
// BAR Name EP Name               Global Addr Post-PLA Addr SB_BAR  SB_FID
// SCF      SPDI3CBUS.SPDI3CBUS.0 0xd8000     0x0           0       0
// SCF      SPDI3CBUS.SPDI3CBUS.1 0xd9000     0x0           0       0
#define I3C_INSTANCE_0_SCF_BAR_GLOBAL_ADDR  0xD8000
#define I3C_INSTANCE_1_SCF_BAR_GLOBAL_ADDR  0xD9000

/**
This macro returns the remote Rlink offset value for a given local Rlink offset value.

@param[in]  CSR_REG_N     - local Rlink CSR offset.

@retval Returns value of remote Rlink offset.
**/
#define REMOTE_RLINK_CSR(CSR_REG_N)    (CSR_REG_N + 0x3000)

// SB Function Blocks
#define BOX_FUNC_SB_PCIE              0
#define BOX_FUNC_SB_M2PCIE            1
#define BOX_FUNC_SB_PCIEGEN4          2
#define BOX_FUNC_SB_DMI               3
#define BOX_FUNC_SB_PSF               4
#define BOX_FUNC_SB_MCSMBUS           5
#define BOX_FUNC_SB_PCIE_G5           6
#define BOX_FUNC_SB_MGPHY             7

// SB FBLP Function Blocks
#define BOX_FUNC_SB_FBLP              0
#define BOX_FUNC_SB_FBLP_PRVT         1
#define BOX_FUNC_SB_CXL_AM            2

//
// SB EX Function Blocks
//
#define BOX_FUNC_SB_PSF_DINO          0
#define BOX_FUNC_SB_PSF_HCX           1
#define BOX_FUNC_SB_PSF_HCB           2
#define BOX_FUNC_SB_PRVT_PCIE_G4      5
#define BOX_FUNC_SB_PRVT_PCIE_G5      6

// MSM Function Blocks
#define  BOX_FUNC_MSM_OOB            0
#define  BOX_FUNC_MSM_PMON           1
#define  BOX_FUNC_MSM_DFX            2

// CXL MEMBAR0 Function Blocks
#define  BOX_FUNC_CXL_DP_CM          1
#define  BOX_FUNC_CXL_DP_AM          3

//
// Sets CSR address function block to 0x4 so the CNVT_*_UNSPTD address translation function is invoked,
// allowing access to channel-specific registers for unsupported channels
//
#define FUNCBLOCK_START_BIT   20
#define FUNCBLOCK_MASK        0xFF0FFFFF
#define SET_UNSPTD_FUNCBLOCK_MC(Addr) ((Addr & FUNCBLOCK_MASK) | (BOX_FUNC_MC_UNSPTD << FUNCBLOCK_START_BIT))
#define SET_UNSPTD_FUNCBLOCK_DDRIO(Addr) ((Addr & FUNCBLOCK_MASK) | (BOX_FUNC_MCIO_DDRIO_UNSPTD << FUNCBLOCK_START_BIT))

//
// Legacy PCI address for CSRs
// Note:
//     These addresses are used when syshost structure is not yet initialized and CSR read/warite functions are not yet usable.
//     They need to be manually updated when register location is changed to match the register header file.
//     Keep such usage of CSRs at minimun and only when necessary.
//
#define SKX_LEGACY_CSR_MMCFG_RULE_N0_CHABC_SAD1_REG        PCI_REG_ADDR(1,29,1,0xc0)  // 1:29:1:C0   MMCFG_RULE in CHA
#define ICX_LEGACY_CSR_MMCFG_RULE_N0_CHABC_SAD1_REG        PCI_REG_ADDR(31,29,1,0xc0) // 31:29:1:C0   MMCFG_RULE in CHA
#define SPR_LEGACY_CSR_MMCFG_RULE_N0_CHABC_SAD1_REG        PCI_REG_ADDR(31,29,1,0xc0) // 31:29:1:C0   MMCFG_RULE in CHA


//
// Format of CSR register offset passed to helper functions.
// This must be kept in sync with the CSR XML parser tool that generates CSR offset definitions in the CSR header files.
//
typedef union {
  struct  {
    UINT32 offset : 16;         // bits <15:0>
    UINT32 size : 3;            // bits <18:16>
    UINT32 pseudo : 1;          // bit  <19>
    UINT32 funcblk : 4;         // bits <23:20>
    UINT32 boxtype : 6;         // bits <29:24>
    UINT32 TenNmSupport: 1;     // bit  <30>
    UINT32 SkxSupport: 1;       // bit  <31>
  } Bits;
  UINT32 Data;
} CSR_OFFSET;

#define PCI_REG_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xff) | (((Function) & 0x07) << 8) | (((Device) & 0x1f) << 11) | (((Bus) & 0xff) << 16))
#define PCIE_REG_ADDR(Bus,Device,Function,Offset) \
    (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

#define STATION_ID_OFFSET_DRA_CH1  0x08

// MMIO OFFSETS (bits 17:12)
// DRA is 4K aligned
#define MMIO_REGION_DRA         0x01
// MCMISC - Global MC CSRS
#define MMIO_REGION_MCGLOBAL    0x02

#define MMIO_REGION_MCCADB_CH0  0x07
#define MMIO_REGION_MCCPGC_CH0  0x09
#define MMIO_REGION_MCCADB_CH1  0x0A
#define MMIO_REGION_MCCPGC_CH1  0x0C

//
// SMEE is 4K aligned
// For SPR
//
#define MMIO_REGION_SMEE_CH0_SPR    0x05
#define MMIO_REGION_SMEE_CH1_SPR    0x06

#define MMIO_REGION_SMEE_CH0    0x08
#define MMIO_REGION_SMEE_CH1    0x0A
#define MMIO_REGION_SMEE_CH2    0x0C
// DDRIO is 64K aligned
#define MMIO_REGION_DDRIO       0x10
// HBMIO is the same as DDRIO
#define MMIO_REGION_HBMIO       MMIO_REGION_DDRIO
// MC is 16K aligned
#define MMIO_REGION_MC0         0x20

//
// For SPR HBM
//
#define SPR_GLOBAL_ADDR_HBMIO         0x100000
#define SPR_GLOBAL_ADDR_HBMIO_CH      0x1000     // 4K aligned
#define SPR_GLOBAL_ADDR_HBMIO_CH_MASK 0xFFF
#define SPR_GLOBAL_ADDR_HBMIO_ALL     0x10000    // 2 * (8 channel per HBM IO)
#define SPR_GLOBAL_ADDR_HBMIO_MCAST   0x8000     // Min address for mcast registers

#define SPR_GLOBAL_ADDR_HBMMC         0x140000
#define SPR_GLOBAL_ADDR_HBMMC_MISC    0x8000
#define SPR_GLOBAL_ADDR_HBMCH_RANGE   0x4000
#define SPR_GLOBAL_ADDR_HBMCH_MASK    0x3FFF
#define SPR_GLOBAL_ADDR_HBM_CPGC_OFFSET 0x3000
#define SPR_GLOBAL_ADDR_HBMMC_RANGE   0x9000

#define SPR_GLOBAL_ADDR_HBM_CPGC_START  0x3800
#define SPR_GLOBAL_ADDR_HBM_CPGC_END    0x3F80

// SPR BOX_HBM 21 Function Blocks
#define SPR_HBM_FUNC_AWORD           0
#define SPR_HBM_FUNC_DWORD           1
#define SPR_HBM_FUNC_MIDSTACK        2
#define SPR_HBM_FUNC_MIDSTACK_GLOBAL 3

// SPR BOX_HBM2E_MC 46 Function Blocks
#define SPR_HBM_FUNC_MAIN            0
#define SPR_HBM_FUNC_2LM             1
#define SPR_HBM_FUNC_MC_GLOBAL       2
#define SPR_HBM_FUNC_DDC_CTL         3
#define SPR_HBM_FUNC_DDC_DP          4
#define SPR_HBM_FUNC_CPGC            5

//
// FOR SPR
//
#define MMIO_REGION_MC1_SPR     0x28

#define MMIO_REGION_MC1         0x24
#define MMIO_REGION_MC2         0x28

// additional MC for HBM
#define MMIO_REGION_HBM_MC0     0x20
#define MMIO_REGION_HBM_MC1     0x24
#define MMIO_REGION_HBM_MC2     0x28
#define MMIO_REGION_HBM_MC3     0x2C
#define MMIO_REGION_HBM_MC4     0x30
#define MMIO_REGION_HBM_MC5     0x34
#define MMIO_REGION_HBM_MC6     0x38
#define MMIO_REGION_HBM_MC7     0x3C

#endif   // _CPU_PCI_ACCESS_H_
