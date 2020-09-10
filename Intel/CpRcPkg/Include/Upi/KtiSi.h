/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#ifndef _KTI_SI_H_
#define _KTI_SI_H_

#include <Upi/RasTypes.h>

/*********************************************************
                KTI Topology Related
*********************************************************/
#define SI_MAX_CPU_SOCKETS     8    // Maximum CPU sockets  supported by Si
#define SI_MAX_NIDS            8    // Maximum number of NIDs supported by SI
#if (MAX_SOCKET == 1)
  #define MAX_FW_KTI_PORTS     3    // Maximum KTI PORTS to be used in structure definition.
#else
  #define MAX_FW_KTI_PORTS     6    // Maximum KTI PORTS to be used in structure definition (update to 8 when scratchpads are defined for GNR/SRF).
#endif //(MAX_SOCKET == 1)

#if defined(GNRSRF_HOST)
  #define MAX_2S_LINKS         6    // Maximum KTI PORTS to be used in structure definition (update to 8 when scratchpads are defined for GNR/SRF).
#else
  #define MAX_2S_LINKS         4    // Maximum KTI PORTS to be used in structure definition.
#endif //defined(GNRSRF_HOST)

#define MAX_LANES_PER_LINK_SPR        24    // Maximum number of Lanes per UPI link of SPR Si
#define MAX_LANES_PER_LINK_GNRSRF     24    // Maximum number of Lanes per UPI link of SPR Si
//
// MAX Link Speeds supported
// SPR supports 12.8GT, 14.4GT and 16.0GT; ICX supports 9.6GT, 10.4GT, 11.2GT
//
#define KTI_SPEEDS_SUPPORTED  3

#define SI_MAX_IO_IOAPIC_TARGET_IN_KTITAD     6    // Maximum IO/IOAPIC target in KTI TAD supports per socket by Si

#define SI_MAX_KTI_PORTS_SKX      3    // Maximum Silicon KTI ports for SKX
#define SI_MAX_KTI_PORTS_ICX      3    // Maximum Silicon KTI ports for ICX
#define SI_MAX_KTI_PORTS_CPX      6    // Maximum Silicon KTI ports for CPX
#define SI_MAX_KTI_PORTS_SPR      4    // Maximum Silicon KTI ports for SPR
#define SI_MAX_KTI_PORTS_GNRSRF   4    // Maximum Silicon KTI ports for GNR/SRF (update to 8 when scratchpads are defined for GNR/SRF)

//
// The silicon attributes of the SPRUCC sku
// The Maximum number of the UPIMDF instances per UPIMDF subsystem
// UPIMDF subsystem is the UPI PORT 1
//
#define  MAX_UPIMDF_INSTANCE_SPRUCC      14
#define  UPIMDF_PORT_NUMBER_SPRUCC       1

//
// Max number of the txeq FOM -- only supported in SPR
// MAX_SOCKET * SI_MAX_KTI_PORTS_SPR * MAX_LANES_PER_LINK_SPR
//
#if defined (SPR_HOST)
#define MAX_NUMBER_OF_FOM   (MAX_SOCKET * SI_MAX_KTI_PORTS_SPR * MAX_LANES_PER_LINK_SPR)
#else
#define MAX_NUMBER_OF_FOM   (MAX_SOCKET * SI_MAX_KTI_PORTS_GNRSRF * MAX_LANES_PER_LINK_GNRSRF)
#endif

#define SOCKET_TYPE_CPU        0    // Type Cpu
#define SOCKET_TYPE_FPGA       1    // Type Fpga
#define SOCKET_LOCAL_4BITS_NID_MSB  8    // Bit3 indicates local vs remote socket when programming NID
#define SOCKET_LOCAL_5BITS_NID_MSB  0x10 // Bit4 indicates local vs remote socket when programming NID on SPR
                                         // Note: Some 10nm registers also use 5bits NID

//
// Using the Most Significant 2 Bits to indicate local or remote socket
//
#define SOCKET_LOCAL_FLAG      BIT30
#define SOCKET_REMOTE_FLAG     BIT31

#define MAX_FPGA_NID        3    // Max possible NID for system FPGA

#define SEGMENT_TYPE_WRKST  2    // capid0.segment identifier for Workstation sku
#define SEGMENT_TYPE_FPGA   3    // capid0.segment identifier for FPGA sku

#define EXSEGMENT_TYPE_64L  3    // capid5.extended_segment identifier for 64L sku

#define XEON_FPGA_PORT      2    // Xeon port used for FPGA
#define FPGA_PORT           0    // Port used by FPGA
#define FPGA_NID_OFFSET     2    // NID of connected FPGA is SID+2
#define FPGA_WHR_UPI_PORTS  1    // Maximum number of BIOS-supported WHR UPI ports on FPGA

#define ICX_LP0_SKU_TYPE_XNC        0          // Peer is XNC
#define ICX_LP0_SKU_TYPE_FPGA       1          // Peer is FPGA
#define ICX_LP0_SKU_TYPE_XEON       0xf        // Peer is XEON

#define WHR_DID_VID                 0xBCC48086
#define WHR_SIP_MC_DID_VID          0xBCC78086
#define WHR_CFG_DEVICE              4

/*********************************************************
                      IIO Stacks
*********************************************************/
#define CCR_PCI_HOST_BRIDGE 0x060000

//
// For ICX, ICX-DE, SNR, CPX-RCD
//  The number of MMIOL_VTD_PER_STACK is same as the number of MMIOH_VTD_PER_STACK
//
#define  TOTAL_MMIOL_CHA_PER_SOCKET   14
#define  TOTAL_MMIO_VTD_PER_STACK     13

//
// For SPR
//  The number of MMIOL_VTD_PER_STACK is same as the number of MMIOH_VTD_PER_STACK
//
#define  TOTAL_MMIOL_CHA_PER_SOCKET_SPR   20
#define  TOTAL_MMIO_VTD_PER_STACK_SPR     20

//
// For SKX, CPX
//
#define TOTAL_MMIOL_VTD_PER_STACK_SKX     12

//for SKX
#define IIO_CSTACK          0
#define IIO_PSTACK0         1
#define IIO_PSTACK1         2
#define IIO_PSTACK2         3
#define IIO_PSTACK3         4
#define IIO_PSTACK4         5


//for 10nm
#define IIO_STACK0          0
#define IIO_STACK1          1
#define IIO_STACK2          2
#define IIO_STACK3          3
#define IIO_STACK4          4
#define IIO_STACK5          5

//SPR
#define IIO_STACK6          6
#define IIO_STACK7          7
#define IIO_STACK8          8
#define IIO_STACK9          9
#define IIO_STACK10         10
#define IIO_STACK11         11

//GNR-SRF
#define IIO_STACK12         12
#define IIO_STACK13         13
#define IIO_STACK14         14
#define IIO_STACK15         15

//WHR FPGA
#define WHR_STACK           0
#define SIP_STACK           1

//
// Reserve last 2 stacks (MAX_LOGIC_IIO_STACK) for rootbusC and ubox
//
#define RSVD_STACK          MAX_LOGIC_IIO_STACK - 2 //use stack 12 to reserve rootbusC
#define UBOX_STACK          MAX_LOGIC_IIO_STACK - 1 //use stack 13 for ubox

#define BUS_PER_MCP                 2
#define BUS_PER_FPGA                6
#define BUS_PER_UBOX                1               //for SKX
#define BUS_PER_UBOX_10nm           2               //for 10nm
#define BUS_PER_DINO_HCA_SPR        5
#define BUS_PER_DINO_HCB_SPR        9
#define BUS_PER_NAC                 8
#define BUS_PER_LEG_STK             20
#define BUS_PER_NONLEG_STK          2
#define BUS_RSVD_10nm               2
#define BUS_RSVD_OOBMSM             1
#define BUS_PER_NONLEG_STK_10nm     20
#define BUS_NUM_ROOTBUSC            0xFC

#define UBOX_MMIO_BAR_SIZE          0x800000   //8M
#define UBOX_MMIO_SIZE_PER_BAR      0x80000    //512K
#define UBOX_MMIO_PCU_BAR_SIZE      0x1000     //4K (Used in GNR)
#define UBOX_MMIO_SBREG_BAR_SIZE    0x10000    //64K (Used in GNR)
#define UBOX_MMIO_SCF_BAR_SIZE      (UBOX_MMIO_BAR_SIZE - UBOX_MMIO_PCU_BAR_SIZE - UBOX_MMIO_SBREG_BAR_SIZE)  // Used in GNR
#define MAX_UBOX_MMIO_BAR_GNR       3
#define UBOX_MMIO_LOCAL_TARGET_ID   6

//
// In SPR 4D configuration, the maximum supported IIO stack number per Die is 3
//
#define MAX_IIO_STACK_PER_DIE_4D              3     // The maximum number of stack per DIE
#define MAX_TYPE_UBOX_IIO_STACK_PER_DIE_4D    2     // The maximum number of TYPE_UBOX_IIO per DIE
#define TYPE_DINO_STACK_BEGIN                 8     // TYPE_DINO stack start at 8
#define DIE_0                                 0
#define DIE_1                                 1
#define DIE_2                                 2
#define DIE_3                                 3

//
// The max number of PI5 Subsystem per socket
//
#define MAX_PI5_SS_PER_SOCKET                 6

//
// Currently, the max supported CXL is 6 per socket
// For 1Die configuration, the maximum supported CXL is 2
//
#define MAX_CXL_PER_SOCKET                6
#define MAX_CXL_PER_SOCKET_1D             2

//
// IAL RCRB BAR Size --- 8KB
// DP has the lower 4KB, UP has the upper 4KB
//
#define CXL_RCRB_BAR_SIZE                       0x2000      //8KB
#define CXL_RCRB_BAR_SIZE_PER_PORT              0x1000      //4KB, DP and UP port have 4KB each

#define CXL_MEMBAR0_SIZE_PER_PORT               0x20000     //128KB, DP and UP port have 128KB each

//
// DMI RCRB BAR Size --- 8KB
//
#define DMI_RCRB_BAR_SIZE              0x2000      //8KB

//
// DDRT QoS Feature related
//
#define LOWBWMEM_BASE_LIMIT_N0_IIO_VTD_SIZE     6                    // The field contains the base address's BIT[0:5].

/*********************************************************
                      CHA
*********************************************************/
#define MAX_CHA                     28
#define MAX_CHA_MAP                 4
#define ICXSP_UNSUPPORTED_CHA_COUNT 38

#define TOTAL_KTI_AD_CREDIT         96
#define CHA_TO_KTI_VN0_AD_INGRESS   3
#define KTI_TO_KTI_VN0_AD_INGRESS   3
#define KTI_TO_KTI_VN1_AD_INGRESS   3
#define MAX_M2PCIE_TO_M3KTI_BL_VNA  8
#define TOTAL_K2K_AD_VNA            12
#define MIN_K2K_AD_VNA              9

#define TOTAL_KTI_BL_CREDIT         64
#define BL_VNA_SHARED_BY_IIO        4

#define CHA_TO_KTI_VN0_BL_INGRESS   4
#define KTI_TO_KTI_VN0_BL_INGRESS   4
#define KTI_TO_KTI_VN1_BL_INGRESS   4
#define MAX_BL_VNA_CREDIT_PER_CHA   2
#define TOTAL_K2K_BL_VNA            4
#define MIN_K2K_BL_VNA              7

#define MAX_P2P_MESH_AGENTS_SPR     16

//
// SPR
//
typedef enum {
  SPR_1S = 0,
  SPR_2S1L,
  SPR_2S2L,
  SPR_2S3L,
  SPR_2S4L,
  SPR_4S3L,
  SPR_4S2L,
  SPR_8S,
  SPR_TOPOLOGY_MAX
} TOPOLOGY4CREDIT;

//
// CPX
//
typedef enum {
  CPX_2S2L_CREDIT = 0,
  CPX_2S3L_CREDIT,
  CPX_4SFLLC_CREDIT,
  CPX_4SRING_CREDIT,
  CPX_8SAFLC_CREDIT,
  CPX_8SPINW_CREDIT,
  CPX_2SW2FPGA_CREDIT,
  CPX_2SW1FPGA_CREDIT,
  CPX_4SWFPGA_CREDIT,
  CPX_TOPOLOGY_MAX
} TOPOLOGY_FOR_CREDIT_CPX;

#define CPX_4SFLLC_CREDIT_MCP           CPX_TOPOLOGY_MAX//ONLY one different extra credit setting
#define MAX_FPGA_TOPOLOGY_CPX           3// This means 2S2FPGA, 2S1FPGA, and 4S2FPGA
#define CPX_MCP_TOPOLOGY_MAX            1//Used to define extra entries in credit arrays
#define PER_LINK_ADDITIONAL_TOPOLOGY    (MAX_FPGA_TOPOLOGY_CPX * 2)//Two additional configurations
                                                                   //per link due to FPGA topology types.
#define FPGA_TOPOLOGY_TYPE_TO_INDEX(X) ((X)-CPX_2SW2FPGA_CREDIT)

#define TOTAL_UPI_AD_CREDIT_SPR         256
#define TOTAL_UPI_BL_CREDIT_SPR         256

#define TOTAL_UPI2CHA_CREDIT_SPR        32
#define TOTAL_MS2IOSF2CHA_CREDIT_SPR    60

#define TOTAL_UPI2CHA_CREDIT_CPX        30
#define TOTAL_CHA_TO_UBOX_BL_VNA_SPR    60
#define TOTAL_M2IAL_BL_CREDIT_XCC_SPR   256
#define RESERVED_MS2IOSF2CHA_DDRT       1
#define RESERVED_M2UPCIE2CHA_DDRT       1

//ICX
#if defined (ICX_HOST)
#define M2M_PER_SOCKET_10nm                4
#define TOTAL_UPI_AD_CREDIT_10nm         192
#define TOTAL_UPI_BL_CREDIT_10nm         160
#define TOTAL_K2K_BL_VNA_10nm              15
#define MIN_K2K_BL_VNA_10nm                12
#define MAX_M2M_TO_M2UPI_BL_VNA            10
#else
#define M2M_PER_SOCKET_10nm                2
#define TOTAL_UPI_AD_CREDIT_10nm         120
#define TOTAL_UPI_BL_CREDIT_10nm          84
#define TOTAL_K2K_BL_VNA_10nm              4
#define MIN_K2K_BL_VNA_10nm                9
#define MAX_M2M_TO_M2UPI_BL_VNA            9
#endif   //#if defined (ICX_HOST)

#define M2M_TO_KTI_VNA_BL_INGRESS_10nm               10
#define M2M_TO_KTI_VNA_BL_INGRESS_10nm_8S            4
#define M2M_TO_KTI_VNA_BL_INGRESS_10nm_4S2L_XCC_LCC  8
#define M2M_TO_KTI_VNA_BL_INGRESS_10nm_4S2L_HCC      6

#define UBOX_TO_KTI_VN0_AD_INGRESS_10nm    1
#define UBOX_TO_KTI_VN0_BL_INGRESS_10nm    3
#define UBOX_TO_M2PCIE_VN0_BL_INGRESS_10nm 1
#define CHA_TO_M2PCIE_VN0_BL_INGRESS_10nm  3
#define TOTAL_K2K_AD_VNA_10nm             15
#define XCC_8S_K2K_AD_VNA_10nm            12
#define MIN_K2K_AD_VNA_10nm               10

// for CHA->M2M
#define TOTAL_M2M_AD_BL_CREDIT                   56

//
// MAX CHA Number in XCC & HCC & LCC
//
#define XCC_CHA_MAX_NUMBER     40
#define HCC_CHA_MAX_NUMBER     28
#define LCC_CHA_MAX_NUMBER     16

#define HCC_CHA_MAX_NUMBER_ICXD     12
#define LCC_CHA_MAX_NUMBER_ICXD     6

//
// Credit CHA-M2M v6.2 - ICX Start
//
#define TOTAL_M2M_AD_CREDIT_10nm                                 104
#define TOTAL_M2M_BL_CREDIT_10nm                                 112

//
// XCC
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4            12
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC      4
#define RESERVED_TRANSGRESS_FLAT_ICX                             0
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX                  14
#define RESERVED_XPTPREFETCH_2LM_ICX                             8
#define RESERVED_TRANSGRESS_2LM_ICX                              (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX - RESERVED_XPTPREFETCH_2LM_ICX)

#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SM2M             20
#define RESERVED_XPTPREFETCH_2LM_ICX_SM2M                        14
#define RESERVED_TRANSGRESS_2LM_ICX_SM2M                         (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SM2M - RESERVED_XPTPREFETCH_2LM_ICX_SM2M)

//
// HCC
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4_HCC            12
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_HCC            20
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_NOSNC_HCC           6
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_HCC             13
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC       20
#define RESERVED_XPTPREFETCH_2LM_ICX_SNC4_HCC                        (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_HCC - RESERVED_TRANSGRESS_2LM_ICX)
#define RESERVED_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC                  (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC - RESERVED_TRANSGRESS_2LM_ICX)

//
// LCC
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4_LCC            20
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC_LCC      16
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC       20
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_LCC             16
#define RESERVED_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC                  (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC - RESERVED_TRANSGRESS_2LM_ICX)
#define RESERVED_XPTPREFETCH_2LM_ICX_SNC2_LCC                        (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_LCC - RESERVED_TRANSGRESS_2LM_ICX)

#define RESERVED_TRANSGRESS_1LM_DDR_DDRT_ICX                    0

//
// BL Transgress Credit
//
#define RESERVED_TRANSGRESS_BL_ICX                              12
#define RESERVED_TRANSGRESS_BL_FLAT_ICX_SNC2_NOSNC_LCC          15
//
// Credit CHA-M2M v6.2 - ICX End
//

//
// Credit CHA->M2M v5.0 2019.11.22 - SPR Start
//
#define SPR_XCC_CHA_MAX_NUMBER     60
#define SPR_HCC_CHA_MAX_NUMBER     44
#define SPR_MCC_CHA_MAX_NUMBER     34
#define SPR_ONEDIE_CHA_MAX_NUMBER  15

#define TOTAL_M2M_AD_CREDIT_SPR                                 114
#define TOTAL_M2M_BL_CREDIT_SPR                                 112           // The number of M2M BL ingress entries is 112

//
// XCC - 4 way & HCC - 2 way
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MD              14
#define RESERVED_TRANSGRESS_FLAT_SPR_MD                          0
#define RESERVED_XPTPREFETCH_FLAT_SPR_MD                         (RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MD - RESERVED_TRANSGRESS_FLAT_SPR_MD)

#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_SNC4_MD_XCC     8
#define RESERVED_XPTPREFETCH_FLAT_SPR_SNC4_MD_XCC                (RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_SNC4_MD_XCC - RESERVED_TRANSGRESS_FLAT_SPR_MD)

//
// XCC - 1Die configuration AD
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_XCC_1D           14
#define RESERVED_XPTPREFETCH_2LM_SPR_XCC_1D                      8
#define RESERVED_TRANSGRESS_2LM_SPR_XCC_1D                       (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_XCC_1D - RESERVED_XPTPREFETCH_2LM_SPR_XCC_1D)

//
// XCC - 1Die configuration BL
//
#define RESERVED_XPTPREFETCH_FLAT_SPR_XCC_1D_AD                  14
#define RESERVED_TRANSGRESS_FLAT_SPR_XCC_1D_BL                   12

//
// HCC 2-way:  the 2LM without AD & SNC2 / SNC4 will also use this set of configuration
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD               24
#define RESERVED_XPTPREFETCH_2LM_SPR_MD                          8
#define RESERVED_TRANSGRESS_2LM_SPR_MD                           (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD - RESERVED_XPTPREFETCH_2LM_SPR_MD)

//
// SPR 1 DIE
//

#define RESERVED_TRANSGRESS_AD_2LM_SPR_1DIE                      4
#define RESERVED_TRANSGRESS_BL_2LM_SPR_1DIE                      6

//
// Special for HCC - 2 way
// HCC 2LM AD credit --- except the 2LM without AD & SNC2 / SNC4
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD_HCC           26
#define RESERVED_XPTPREFETCH_2LM_SPR_MD_HCC                      10
#define RESERVED_TRANSGRESS_2LM_SPR_MD_HCC                      (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD_HCC - RESERVED_XPTPREFETCH_2LM_SPR_MD_HCC)

//
// MCC
//
#define RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MCC              12
#define RESERVED_XPTPREFETCH_SPR_MCC                              12
#define RESERVED_TRANSGRESS_FLAT_SPR_MCC                          (RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MCC - RESERVED_XPTPREFETCH_SPR_MCC)
#define RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MCC               28
#define RESERVED_TRANSGRESS_2LM_SPR_MCC                           (RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MCC - RESERVED_XPTPREFETCH_SPR_MCC)

//
// No CHA->M2M description LCC in V5.0
//

//
// BL Transgress Credit
//
#define RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC4_MD                    7
#define RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC2_NOSNC                 12       // XCC 4Die, 1Die, HCC_2way
#define RESERVED_TRANSGRESS_BL_2LM_SPR_MD                          16       // number of MC * 4
#define RESERVED_TRANSGRESS_BL_2LM_SPR_MD_XCC                      20       // number of MC * 5

//
// MCC
//
#define RESERVED_TRANSGRESS_BL_FLAT_SPR_MCC                        10
#define RESERVED_TRANSGRESS_BL_2LM_SPR_MCC                         24       // number of MC * 6
//
// Credit CHA-M2M v5.0 2019.11.22 - SPR End
//

//
// Credit settings ICX-DE
//
#define RESERVED_AD_TRANSGRESS_XPTPREFETCH_ICXD                 8
#define RESERVED_AD_TRANSGRESS_XPTPREFETCH_SNC_ICXD             14
#define RESERVED_BL_TRANSGRESS_ICXD                             15
#define RESERVED_XPTPREFETCH_ICXD_2LM                           4
#define RESERVED_XPTPREFETCH_ICXD                               8
#define RESERVED_XPTPREFETCH_SNC_ICXD                           14
#define RESERVED_AD_TRANSGRESS_ICXD                             (RESERVED_AD_TRANSGRESS_XPTPREFETCH_ICXD - RESERVED_XPTPREFETCH_ICXD)
#define RESERVED_AD_TRANSGRESS_2LM_ICXD                         (RESERVED_AD_TRANSGRESS_XPTPREFETCH_ICXD - RESERVED_XPTPREFETCH_ICXD_2LM)

//
// ICXDE LCC 2LM
//
#define RESERVED_AD_TRANSGRESS_XPTPREFETCH_2LM_ICXD_LCC     9
#define RESERVED_XPTPREFETCH_2LM_ICXD_LCC                   5
#define RESERVED_AD_TRANSGRESS_2LM_ICXD_LCC                 (RESERVED_AD_TRANSGRESS_XPTPREFETCH_2LM_ICXD_LCC - RESERVED_XPTPREFETCH_2LM_ICXD_LCC)

//
// Credit settings SNR
//
#define RESERVED_AD_TRANSGRESS_XPTPREFETCH_SNR     8
#define RESERVED_BL_TRANSGRESS_SNR                 0
#define RESERVED_XPTPREFETCH_SNR                   8
#define RESERVED_AD_TRANSGRESS_SNR                 (RESERVED_AD_TRANSGRESS_XPTPREFETCH_SNR - RESERVED_XPTPREFETCH_SNR)

#define RESERVED_TRANSGRESS_XPTPREFETCH_ICXG     24

#define MAX_CREDIT                 15 //4 bits for one port
#define MAX_CHA_TO_M2UPI_AD_CREDIT 3

// for CHA-> M2PCIE
#define MAX_CHA_TO_M2PCIE_BL_VNA_SKX  28
#if defined (ICX_HOST) || defined (ICXDE_HOST)
#define MAX_CHA_TO_M2PCIE_BL_VNA_10nm  160
#else
#define MAX_CHA_TO_M2PCIE_BL_VNA_10nm  128
#endif  // #if defined (ICX_HOST)

#define MAX_M2IOSF_INGRESS_BUFFER_CHA_BL_PKTS 84

#define MAX_M2PCIE_DEST_CREDITS     64
#define MAX_M2PCIE_STACK_COUNT_CPX  3

#define MAX_KTI_TO_M2PCIE_BL_VNA         9
#define SHARED_P2P_M2PCIE_BL_CREDIT      23
#define SHARED_P2P_CREDIT_SAME_RING      26
#define SHARED_P2P_CREDIT_DIFF_RING      31
#define NORTH_CAP_M2IOSF_ID_MASK         0x1E
#define SOUTH_CAP_M2IOSF_ID_MASK         0x21
#define NORTH_CAP_M2IOSF_ID_MASK_ICXD    0x23
#define SOUTH_CAP_M2IOSF_ID_MASK_ICXD    0x1C
#define NORTH_CAP_M2IOSF_ID_MASK_SNR     0x07
#define SOUTH_CAP_M2IOSF_ID_MASK_SNR     0x18

//SPR - HBM CREDIT
#define M2M_CLOSE_MAXIMUM_INDEX_SPR             7

#define HBM_AD_CLUSTER_CHAS_CLOSE               7
#define HBM_AD_CLUSTER_CHAS_DISTANT             8
#define HBM_BL_CLUSTER_CHAS_CLOSE               7
#define HBM_BL_CLUSTER_CHAS_DISTANT             8

#define HBM_AD_CLUSTER_CHAS_CLOSE_2LM           7
#define HBM_AD_CLUSTER_CHAS_DISTANT_2LM         8
#define HBM_BL_CLUSTER_CHAS_CLOSE_2LM           7
#define HBM_BL_CLUSTER_CHAS_DISTANT_2LM         7

#define HBMM2M_TO_DDRM2M_AD_2LM_DDR_SNC4        28
#define HBMM2M_TO_DDRM2M_BL_2LM_DDR_SNC4        28
#define HBMM2M_TO_DDRM2M_AD_2LM_DDR_ALL         4
#define HBMM2M_TO_DDRM2M_BL_2LM_DDR_ALL         4
#define DDRM2M_TO_HBMM2M_BL_2LM                 7

#define DDR_AD_CLUSTER_CHAS_CLOSE               7
#define DDR_AD_CLUSTER_CHAS_DISTANT             8
#define DDR_BL_CLUSTER_CHAS_CLOSE_1LM_HBM_DDR   7
#define DDR_BL_CLUSTER_CHAS_DISTANT_1LM_HBM_DDR 7
#define DDR_SNC4_QUAD_BL_TRANSGRESS_1LM         7
#define DDR_HEMI_BL_TRANSGRESS_1LM              6
#define DDR_ALL2ALL_BL_TRANSGRESS_1LM           9

#define RESERVED_TRANSGRESS_BL_HBM_2LM_All_TO_ALL                  16       // M2M per socket-DDR4 * 4
#define RESERVED_TRANSGRESS_AD_HBM_2LM_All_TO_ALL                  16       // M2M per socket-DDR4 * 4
#define RESERVED_TRANSGRESS_BL_HBM_1LM_All_TO_ALL                  9
#define RESERVED_TRANSGRESS_AD_HBM_1LM_All_TO_ALL                  0
#define RESERVED_TRANSGRESS_BL_HBM_1LM_SNC4                        7
#define RESERVED_TRANSGRESS_AD_HBM_1LM_SNC4                        0
#define RESERVED_TRANSGRESS_BL_HBM_1LM_SNC2                        8
#define RESERVED_TRANSGRESS_AD_HBM_1LM_SNC2                        0

/*

According to Filip:
 South cap is always MS2IOSF 0 and 5
 North cap is always MS2iosf 1, 2, 3 (except LCC), 4

    UINT32 physical_chop : 2;

                            // Bits[7:6], Access Type=RO/V, default=0x0

                            // 11:XCC; 10:HCC; 01:MCC; 00:LCC;
*/

#define RPQ_VNA_PER_UPI_10nm 3

// for M2PCIE-> CHA
#define TOTAL_PRQ_SKX                       18
#define TOTAL_PRQ_10nm                      24
#define PRQ_KNH_PER_M2PCIE                  4
#if defined (ICX_HOST) || defined (SPR_HOST)
#define MS2IOSF_TO_UPI_BL_VNA_INGRESS_10nm    45
#define MS2IOSF_TO_UPI_BL_VNA_INGRESS_8S_10nm 38
#else
#define MS2IOSF_TO_UPI_BL_VNA_INGRESS_10nm    11
#define MS2IOSF_TO_UPI_BL_VNA_INGRESS_8S_10nm 11
#endif  // #if defined (ICX_HOST) || defined (SPR_HOST)
#define MS2IOSF_TO_UPI_AD_VN0_INGRESS_MAX_10nm  15
#define MS2IOSF_TO_UPI_BL_VN0_INGRESS_10nm  2
#define DDRT_RESERVED_PRQ_CREDITS           1

//
// SAD/System Address Map related
//
#define MAX_SAD_TARGETS              32              // 32 targets for IO, IOAPIC, MMIOH  //ICX_TODO extend to 64 later
#define SADTGT_SIZE                  4               // SAD target is 4 bits in size
#define SAD_BUS_GRANTY               32              // 32 if mmcfg <=256
#define SAD_BUS_TARGETS              8               // 8 mmcfg targets
#define SAD_LEG_IO_GRANTY            0x800           // 2KB
#define SAD_LEG_IO_PCH_SIZE          8               // Reserve 16KB for Legacy_Pch
#define SAD_IOAPIC_GRANTY            0x8000          // 32KB
#define SAD_MMIOL_GRANTY             0x400000        // 4MB
#define MAX_ADDR_LINE                46              // 46 Address bits are supported
#define IOAPIC_BASE                  0xFEC00000
#define IOAPIC_LIMIT                 0xFECFFFFF
#define MMIOL_LIMIT                  0xFBFFFFFF      // (4GB - 64MB - 1)
#define MMIOH_BASE                   0x2000          // address above 4GB, so the 46 bits of physical address [45:0] is 0x2000_0000_0000  (32TB)
#define MMIOH_BASE_SNR               0x200           // address above 4GB, so the 42 bits of physical address [41:0] is 0x200_0000_0000
#define MMIOH_MAX_GRANTY             0x5             // 2**(size*2) GB. Range is 1GB to 32TB
#define MAX_ADDR_LINE_ICX            52              // 52 Address bits are supported
#define MAX_ADDR_LINE_SNR            42              // 42 Address bits are supported
#define MMIOH_BASE_ICX               0xE0000         // address above 4GB, so the 52 bits of physical address [51:0] is 0xE0000_0000_0000
#if !defined (SPR_HOST)
#define SAD_TGT_NUM_PER_REG          8               // the number of targets in the IoSadTgtList or IoapicSadTgtList
#define SAD_TGT_DEFAULT_VALUE        0x88888888      // the default value for IoSadTgtList or IoapicSadTgtList
#else
#define SAD_TGT_NUM_PER_REG          4               // the number of targets in the IoSadTgtList or IoapicSadTgtList on SPR
#define SAD_TGT_DEFAULT_VALUE        0x10101010      // the default value for IoSadTgtList or IoapicSadTgtList on SPR
#endif  // #if !defined (SPR_HOST)

//
// DDRT QoS CFG related
//
#define DDRT_QOS_LOCTOREM_MAXBW_THRESHOLD     0x9

//
// CHA KTI Routing Mode
//
typedef enum {
  UpiRoutingModeA = 0,  ///< 8-entry table. One link between nodes. Indexing using Node[2:0]
  UpiRoutingModeB0,     ///< 8-entry table. Up to two links between nodes. SKX, CLX. Indexing using P6^P12.
  UpiRoutingModeB1,     ///< 8-entry table. Up to two links between nodes. ICX B-step. NodeID[2:1],p6^p12.
  UpiRoutingModeB2,     ///< 16-entry table. Up to two links between nodes. CPX, SPR. Indexing using NodeID[2:0],p6^p12
  UpiRoutingModeC0,     ///< 8-entry table. Two nodes; 3 links between nodes. SKX, CLX. Indexing using Mod3[1:0].
  UpiRoutingModeC1,     ///< 16-entry table. Up to 3 links between nodes. CPX, SPR. Indexing using NodeId[2:1],Mod3[1:0]
  UpiRoutingModeD0,     ///< 16-entry table. Up to 4 links between nodes. SPR. Indexing using NodeId[1:0],PA[13:12]^PA[7:6]
  UpiRoutingModeMax
} UPI_ROUTING_MODE;

//
// UPI Routing Mode; The order of string must match with UPI_ROUTING_MODE enum
//
STATIC CHAR8 *UpiRoutingModeStr[] = {
  "A",
  "B0",
  "B1",
  "B2",
  "C0",
  "C1",
  "D0",
  "Unknown"
};

/*********************************************************
                      MC
*********************************************************/
#define MAX_MINISAD_32_ENTRY    32          // Maximum number of minisad entry for 10nm
#define MINISAD_ENTRY_ENABLE    BIT4        // Each minisad entry has 5 bits and when the entry is enabled then bit4 will be set
#define NUM_OF_REGSTERS_MINISAD_32_ENTRY    6  //number of registers for minisad 32 entry (each register cover 6 entries, each entry use 5 bits)
#define NUM_OF_REGSTERS_32_ENTRY_PREFETCH_BASE  4

/*********************************************************
                      M3KTI
*********************************************************/
#if defined(ICX_HOST)
#define MAX_M3KTI             3
#define MAX_PORT_IN_M3KTI     1
#elif defined (SPR_HOST)
#define MAX_M3KTI             4
#define MAX_PORT_IN_M3KTI     1
#elif defined (GNRSRF_HOST)
#define MAX_M3KTI             8
#define MAX_PORT_IN_M3KTI     1
#else // defined(ICX_HOST) || defined(SPR_HOST)
#if defined(CPX_HOST)
#define MAX_M3KTI             3
#define MAX_PORT_IN_M3KTI     2
#else
#define MAX_M3KTI             2
#define MAX_PORT_IN_M3KTI     2
#endif
#endif // defined(ICX_HOST) || defined(SPR_HOST)
#define PORT_TO_M3KTI(PORT)   (PORT >> 1) //for SoCs with 2 ports per M3KTI agent
#define M3KTI_TO_PORT_BITMAP(M3KTI)   ((1 << (2 * M3KTI)) | (1 << (2 * M3KTI + 1))) //for SoCs with 2 ports per M3KTI agent
#define D2C_THLD_DFLT_SETTING 2

/*********************************************************
                      KTI Agent Link Layer
*********************************************************/

#define KTI_INVALID_PORT      0xFF

//
// Link Layer Init State Machine
//
typedef enum {
  LINK_SM_NOT_RDY_FOR_INIT           = 0,
  LINK_SM_PARAM_EX,
  LINK_SM_CRD_RETURN_STALL,
  LINK_SM_INIT_DONE,
  LINK_SM_MAX
} LINK_SM;

//
// KTI Agent Type Definition
//
typedef enum {
  KTI_CONFIG_AGENT                  = 0,
  KTI_FIRMWARE_AGENT,
  KTI_ROUTER_AGENT,
  KTI_AGENT_RSVD,
  KTI_LT_AGENT,
  KTI_IO_PROXY_AGENT,
  KTI_HOME_AGENT,
  KTI_CACHING_AGENT,
  KTI_MAX_AGENT
} KTI_AGENT_TYPE;

typedef struct {
  UINT32 Address_Mask_51_46;   //Address mask for 51-46 bit
  UINT32 Address_Mask_45_40;   //Address mask for 45-40 bit
  UINT32 Address_Mask_39_28;   //Address mask for 39-28 bit
} KTI_AGENT_2LM_ADDRESS_MASK_10nm;

typedef struct {
  UINT32 Address_Mask_45_40;   //  Address mask for 45-40 bit
  UINT32 Address_Mask_39_31;   //  Address mask for 39-31 bit
} KTI_AGENT_2LM_ADDRESS_MASK_Skx;

typedef enum {
  KTI0_NCS_INDEX = 0,
  KTI1_NCS_INDEX,
  KTI2_NCS_INDEX,
  KTI0_NCB_INDEX,
  KTI1_NCB_INDEX,
  KTI2_NCB_INDEX,
  MAX_KTI_M2PCIE_ENTRY
} KTI_INDEX; //Used in SKX M3KTI->M2PCIE Mesh credit Programming

typedef enum {
  KTI_LINK_NO_TYPE = 0, // Link is has not being configured
  KTI_LINK_IS_RT,       // Link is route through
  KTI_LINK_NO_RT,       // Link is not route through
  KTI_LINK_IS_FPGA,     // Link is connected to an FPGA
  KTI_LINK_MAX_TYPE
} KTI_LINK_TYPES;

#define MAX_LINK_TYPES (KTI_LINK_MAX_TYPE - 1)
#define LINK_TYPE_TO_INDEX(X) (X-KTI_LINK_IS_RT)

//
// cor phy reset in cor_mask & cor_cnt_mask of KTIERRDIS0_KTI_LLPMON_REG
// These two bits have to set to 1 before cpreset in SPR, then restore after cpreset
//
#define COR_PHY_RESET_BIT_IN_KTIERRDIS0    BIT1

/*********************************************************
                      KTI Agent Phy Layer
*********************************************************/
#define UPIPHYPORT_STATE_RESET_CALIB        0x1
#define UPIPHYPORT_STATE_DETECT_CLKTERM     0x2
#define UPIPHYPORT_STATE_L0                 0xf
#define UPIPHYPORT_STATE_LOOPBACK_MASTER    0x12

#define PHY_STATE_RESETP                    0x0
#define PHY_STATE_RESETC                    0x2
#define PHY_STATE_RXDETECT                  0x3

#define PHY_STATE_L0C                       0xe
#define PHY_STATE_L0                        0xf

#define UPI_LINK_DEFAULT_RESET_MODE         0x1
#define UPI_LINK_SOFT_RESET_MODE            0x0

#define MAX_KTI_BUNDLES      10
#define MAX_KTI_LANES        (2*MAX_KTI_BUNDLES)

//
// encoding of the cri_freq_selection on SPR
//     3’b000  --- slow mode
//     3’b001 --- 12.8GT
//     3’b010 --- 14.4GT
//     3’b011 --- 16.0GT.
//
typedef enum {
  PIPE_FREQ_SELECT_SLOW = 0,
  PIPE_FREQ_SELECT_128GT,
  PIPE_FREQ_SELECT_144GT,
  PIPE_FREQ_SELECT_160GT,
  PIPE_FREQ_SELECT_UNKNOWN = 0xff
} PIPE_FREQ_SELECT;

//
// encoding of the c_init
//     2’b00 --- Init to L0
//     2’b01 --- CLocalLBM
//     2’b10 --- CLocalComplianceSlave
//     2’b11 --- CLocalComplianceMaster
//
typedef enum {
  C_INIT_TO_L0 = 0,
  C_INIT_TO_CLocalLBM,
  C_INIT_TO_CLocalComplianceSlave,
  C_INIT_TO_CLocalComplianceMaster
} C_INIT_TO_STATUS;

/*********************************************************
                      UBOX
*********************************************************/
//
// Supported System configuration. This clasification is based on Credit setting, coherency setting
// and not based on topology alone. For example, a 3S topology will be treated as 4S config so that it
// can have same Credit and other coherency setting as 4S topology.
//
typedef enum {
  SYS_CONFIG_1S = 0,  ///< Single socket
  SYS_CONFIG_2S1L,
  SYS_CONFIG_2S2L,
  SYS_CONFIG_2S3L,    ///< 2S 3 LINKs  (TwoSkt3Link)
  SYS_CONFIG_2S4L,    ///< 2S 4 LINKs  (TwoSkt4Link)
  SYS_CONFIG_4S,
  SYS_CONFIG_8S,
  SYS_CONFIG_MAX
} SYS_CONFIG;

//
// Supported 8-socket topology
// This field is not meaningful if SYS_CONFIG is not SYS_CONFIG_8S
//
// Example of 8S 4-link twisted cubes fully connected cubes:
//  +-----------------------+
//  |                       |       VN Assignment:
//  S0 ---- S1     S6 ---- S7       ===============
//  | \   / | \   / | \   / |       VN0: S0, S2, S4, S6
//  |  \ /  |  \ /  |  \ /  |       VN1: S1, S3, S5, S7
//  |   \   |   \   |   \   |
//  |  / \  |  / \  |  / \  |
//  | /   \ | /   \ | /   \ |
//  S2 ---- S3     S4 ---- S5
//  |                       |
//  +-----------------------+
//
typedef enum {
  Topo8SSingleLink = 0,           ///< Default value. This can be 8-socket single-link pinwheel, 8-socket degraded topology, or non 8-socket config.
  Topo8SDoublePinwheel,           ///< 8-socket double up pinwheel (each node has duallink to its neighbor node)
  Topo8SAlmostFullyConnect,       ///< 8-socket almost fully connect (each node has 6 UPI links, each link is connected to a different neighbors)
  Topo8STwoClumps,                ///< 8-socket 2 Clumps (each node has 4/5 actived UPI links). Clump means 4S Fully Connect
  Topo8STwistedFullyConnectCubes, ///< 8-socket 4-link twisted cubes fully connected cubes. CROSSLINK_PORT_ID is the crosslink within each fully connected 4S cube.
  Topo8SMax
} TOPOLOGY_8S;

#define SERVER_10NM_UBOX_BUS  30
#define SKX_UBOX_BUS          0

//
// Macro to create mask. Ex. FLD_MASK(3, 5) --> 0000 0000 1110 0000
//
#define FLD_MASK(Size, Pos) ((~((~0) << (Size))) << (Pos))

typedef union {
  struct {
    UINT32  Rsvd1 : 32;     // [31:0]   - Reserved but allow RW for late changes
  } Fields;
  UINT32 RawData;
} KTI_LEP2;
#pragma pack()

/*********************************************************
                           PCU
*********************************************************/

//
// Wayness types
//
typedef enum {
  ONE_WAY_SOCKET,
  TWO_WAY_SOCKET,
  FOUR_WAY_SOCKET,
  EIGHT_WAY_SOCKET,
  UNDEFINED_WAYS
}
WAYS_TYPE;

// Bits [15:8] should be 0x2 (LLC BIST complete) before the socket can be accessed.
// Bits [7:0] are 0xF8 indicating in SSP loop
//
#define BOOTGO_REQUIRED   0x02F8

typedef enum {
  CP_STACK1_CPX = 0,
  P_STACK2_CPX,
  P_STACK3_CPX,
  MAX_CP_STACK_STACK
} CPX_M2PCIE_VALID_STACKS;

#define CF8_REG_ADDR(Bus,Device,Function,Offset) \
  ( 0x80000000 | ((Offset) & 0xff) | (((Function) & 0x07) << 8) | (((Device) & 0x1f) << 11) | (((Bus) & 0xff) << 16))

#define BGF_POINTER_OVERRIDE_MEM_NON_GV   0  // Uncore to Memory  SKX.U2D
#define BGF_POINTER_OVERRIDE_MEM_GV       1  // Memory to Uncore  SKX.D2U
#define BGF_POINTER_OVERRIDE_KTI_NON_GV   0  // Uncore to KTI     SKX.U2K
#define BGF_POINTER_OVERRIDE_KTI_GV       1  // KTI to Uncore     SKX.K2U
#define BGF_POINTER_OVERRIDE_IIO_NON_GV   0  // Uncore to IIO     SKX.U2I
#define BGF_POINTER_OVERRIDE_IIO_GV       1  // IIO to Uncore     SKX.I2U

//
// legacy PCI address for CSRs
//

//
// Sync the SBSP BUSNO to LEGACY_BUS_NO in ProcessorStartupUncore.inc
//
#define LEGACY_BUS0  0
#define LEGACY_BUS1  0x11

#if defined(SKX_HOST)

#define MAX_MMCFG_LOCAL_RULES_ADDR_CHA             2
#define MAX_MMCFG_LOCAL_RULES_TARGET_IIO           1
#define MAX_MMCFG_LOCAL_RULES_TARGET_CHA           MAX_MMCFG_LOCAL_RULES_TARGET_IIO
#define MMCFG_LOCAL_RULE_IDX_ADDR_CHA              0
#define MMCFG_LOCAL_RULES_IDX_TARGET_IIO           2

#define LEGACY_CSR_MMCFG_RULE_N0                  (CF8_REG_ADDR(LEGACY_BUS1, 29, 1, 0xc0)) // CSR 11:29:1:0xc0
#define LEGACY_CSR_MMCFG_RULE_N1                  (CF8_REG_ADDR(LEGACY_BUS1, 29, 1, 0xc4)) // CSR 11:29:1:0xC4
#define LEGACY_CSR_MMCFG_TARGET_LIST              (CF8_REG_ADDR(LEGACY_BUS1, 29, 1, 0xec)) // CSR 11:29:1:0xec
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0 (CF8_REG_ADDR(LEGACY_BUS1, 29, 1, 0xc8)) // CSR 31:29:1:0xc8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1 (CF8_REG_ADDR(LEGACY_BUS1, 29, 1, 0xcc)) // CSR 31:29:1:0xcc
#define LEGACY_CSR_MMCFG_LOCAL_RULE               (CF8_REG_ADDR(LEGACY_BUS1, 29, 1, 0xe4)) // CSR 31:29:1:0xE4

#define LEGACY_CSR_CPUBUSNO_UBOX                  (CF8_REG_ADDR(LEGACY_BUS0, 8, 2, 0xcc)) // CSR 30:0:2:0xD0 on 10nm & SPR (offset 0xCC on SKX)
#define LEGACY_CSR_CPUBUSNO1_UBOX                 (CF8_REG_ADDR(LEGACY_BUS0, 8, 2, 0xd0)) // CSR 30:0:2:0xD0
#define LEGACY_CSR_CPUBUSNO_UBOX_10NMVERSION0     (CF8_REG_ADDR(LEGACY_BUS0, 8, 2, 0xcc)) // CSR 30:0:2:0xCC
#define LEGACY_CSR_CPUBUSNO_VALID_UBOX            (CF8_REG_ADDR(LEGACY_BUS0, 8, 2, 0xd4)) // CSR 30:0:2:0xD4

#define LEGACY_CSR_SAD_TARGET(BusNo)              (CF8_REG_ADDR(BusNo, 29, 1, 0xf0)) // CSR SKX: BUS1:29:1:0xF0
#define LEGACY_CSR_SAD_CONTROL(BusNo)             (CF8_REG_ADDR(BusNo, 29, 1, 0xf4)) // CSR SKX: BUS1:29:1:0xF4

#elif defined(GNRSRF_HOST)

#define LEGACY_CSR_MMCFG_RULE_N0                  (CF8_REG_ADDR(30,13,1,0xc0))    // CSR 30:13:1:0xc0
#define LEGACY_CSR_MMCFG_RULE_N1                  (CF8_REG_ADDR(30,13,1,0xc4))    // CSR 30:13:1:0xC4
#define LEGACY_CSR_MMCFG_TARGET_LIST              (CF8_REG_ADDR(30,13,1,0xec))    // CSR 30:13:1:0xec

#define LEGACY_CSR_SAD_TARGET(BusNo)              (CF8_REG_ADDR(BusNo, 13, 1, 0xf8)) // CSR UBOX: BUS1:13:1:0xF8
#define LEGACY_CSR_SAD_CONTROL(BusNo)             (CF8_REG_ADDR(BusNo, 13, 1, 0xf4)) // CSR UBOX_ BUS1:13:1:0xF4

//
// Default definition for GNR/SRF
//
#define MAX_MMCFG_LOCAL_RULES_ADDR_CHA                   4
#define MAX_MMCFG_LOCAL_RULES_TARGET_IIO                 2
#define MAX_MMCFG_LOCAL_RULES_TARGET_CHA                 MAX_MMCFG_LOCAL_RULES_TARGET_IIO
#define MMCFG_LOCAL_RULE_IDX_ADDR_CHA                    0
#define MMCFG_LOCAL_RULES_IDX_TARGET_IIO                 4

#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG(RuleIdx) (CF8_REG_ADDR(30,13,1,0xb0 + 0x4*(RuleIdx))) // CSR 30:13:1:0xb0
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0        (CF8_REG_ADDR(30,13,1,0xb0))                 // CSR 30:13:1:0xb0
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1        (CF8_REG_ADDR(30,13,1,0xb4))                 // CSR 30:13:1:0xb4
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_2        (CF8_REG_ADDR(30,13,1,0xb8))                 // CSR 30:13:1:0xb8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_3        (CF8_REG_ADDR(30,13,1,0xbc))                 // CSR 30:13:1:0xbc
#define LEGACY_CSR_MMCFG_LOCAL_RULE(RegIdx)              (CF8_REG_ADDR(30,13,1,0xc8 + 0x4*(RegIdx)))  // CSR 30:13:1:0xc8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_CFG                  (CF8_REG_ADDR(30,13,1,0xc8))                 // CSR 30:13:1:0xc8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_CFG2                 (CF8_REG_ADDR(30,13,1,0xcc))                 // CSR 30:13:1:0xcc

//
// For GNR CPU, 30:0:2 (UBOX CPUBUSNO) can only be used before the BUSNO reprogramed which is in KTIRC
//
#define LEGACY_CSR_CPUBUSNO_UBOX                  (CF8_REG_ADDR(30, 0, 2, 0xd0)) // CSR 30:0:2:0xD0
#define LEGACY_CSR_CPUBUSNO1_UBOX                 (CF8_REG_ADDR(30, 0, 2, 0xd0)) // CSR 30:0:2:0xD0
#define LEGACY_CSR_CPUBUSNO_UBOX_10NMVERSION0     (CF8_REG_ADDR(30, 0, 2, 0xcc)) // CSR 30:0:2:0xCC
#define LEGACY_CSR_CPUBUSNO_VALID_UBOX            (CF8_REG_ADDR(30, 0, 2, 0xd4)) // CSR 30:0:2:0xD4

#elif defined(SNR_HOST) || defined(TNR_HOST) || defined(ICXDE_HOST) || defined(ICX_HOST) || defined(SPR_HOST)

#define LEGACY_CSR_MMCFG_RULE_N0                  (CF8_REG_ADDR(31,29,1,0xc0)) // CSR 31:29:1:0xc0
#define LEGACY_CSR_MMCFG_RULE_N1                  (CF8_REG_ADDR(31,29,1,0xc4)) // CSR 31:29:1:0xC4
#define LEGACY_CSR_MMCFG_TARGET_LIST              (CF8_REG_ADDR(31,29,1,0xec)) // CSR 31:29:1:0xec

#define LEGACY_CSR_SAD_TARGET(BusNo)              (CF8_REG_ADDR(BusNo, 29, 1, 0xf8)) // CSR UBOX_BUS1:29:1:0xF8
#define LEGACY_CSR_SAD_CONTROL(BusNo)             (CF8_REG_ADDR(BusNo, 29, 1, 0xf4)) // CSR UBOX BUS1 29:1:0xF4

#if defined(SNR_HOST) || defined(TNR_HOST) || defined(ICXDE_HOST) || defined(ICX_HOST)
#define MAX_MMCFG_LOCAL_RULES_ADDR_CHA             2
#define MAX_MMCFG_LOCAL_RULES_TARGET_IIO           1
#define MAX_MMCFG_LOCAL_RULES_TARGET_CHA           MAX_MMCFG_LOCAL_RULES_TARGET_IIO
#define MMCFG_LOCAL_RULE_IDX_ADDR_CHA              0
#define MMCFG_LOCAL_RULES_IDX_TARGET_IIO           2
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0 (CF8_REG_ADDR(31,29,1,0xc8)) // CSR 31:29:1:0xc8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1 (CF8_REG_ADDR(31,29,1,0xcc)) // CSR 31:29:1:0xcc
#define LEGACY_CSR_MMCFG_LOCAL_RULE               (CF8_REG_ADDR(31,29,1,0xe4)) // CSR 31:29:1:0xE4
#elif defined(SPR_HOST)
#define MAX_MMCFG_LOCAL_RULES_ADDR_CHA                   4
#define MAX_MMCFG_LOCAL_RULES_TARGET_IIO                 2
#define MAX_MMCFG_LOCAL_RULES_TARGET_CHA                 MAX_MMCFG_LOCAL_RULES_TARGET_IIO
#define MMCFG_LOCAL_RULE_IDX_ADDR_CHA                    0
#define MMCFG_LOCAL_RULES_IDX_TARGET_IIO                 4
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG(RuleIdx) (CF8_REG_ADDR(31,29,1,0xb0 + 0x4*(RuleIdx))) // CSR 31:29:1:0xb0
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0        (CF8_REG_ADDR(31,29,1,0xb0)) // CSR 31:29:1:0xb0
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1        (CF8_REG_ADDR(31,29,1,0xb4)) // CSR 31:29:1:0xb4
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_2        (CF8_REG_ADDR(31,29,1,0xb8)) // CSR 31:29:1:0xb8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_ADDRESS_CFG_3        (CF8_REG_ADDR(31,29,1,0xbc)) // CSR 31:29:1:0xbc
#define LEGACY_CSR_MMCFG_LOCAL_RULE(RegIdx)              (CF8_REG_ADDR(31,29,1,0xc8 + 0x4*(RegIdx))) // CSR 31:29:1:0xc8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_CFG                  (CF8_REG_ADDR(31,29,1,0xc8)) // CSR 31:29:1:0xc8
#define LEGACY_CSR_MMCFG_LOCAL_RULE_CFG2                 (CF8_REG_ADDR(31,29,1,0xcc)) // CSR 31:29:1:0xcc
#endif //defined(SPR_HOST)

//
// For ICX CPU, 30:0:2 (UBOX CPUBUSNO) can only be used before the BUSNO reprogramed which is in KTIRC
//
#define LEGACY_CSR_CPUBUSNO_UBOX                  (CF8_REG_ADDR(30, 0, 2, 0xd0)) // CSR 30:0:2:0xD0 on 10nm & SPR (offset 0xCC on SKX)
#define LEGACY_CSR_CPUBUSNO1_UBOX                 (CF8_REG_ADDR(30, 0, 2, 0xd0)) // CSR 30:0:2:0xD0
#define LEGACY_CSR_CPUBUSNO_UBOX_10NMVERSION0     (CF8_REG_ADDR(30, 0, 2, 0xcc)) // CSR 30:0:2:0xCC
#define LEGACY_CSR_CPUBUSNO_VALID_UBOX            (CF8_REG_ADDR(30, 0, 2, 0xd4)) // CSR 30:0:2:0xD4

#else // #elif defined(SNR_HOST) || defined(TNR_HOST) || defined(ICXDE_HOST) || defined(SPR_HOST)

#error UNKNOWN COMPILE TARGET

#endif // defined(SKX_HOST)

#define MMCFG_STORE_SIZE (MAX_MMCFG_LOCAL_RULES_ADDR_CHA + MAX_MMCFG_LOCAL_RULES_TARGET_IIO)

#define LEGACY_CSR_CPUBUSNO_PCU                            CF8_REG_ADDR(LEGACY_BUS1, 30, 4, 0x94)               // CSR 1:30:4:0x94
#define LEGACY_CSR_CPUBUSNO1_PCU                           CF8_REG_ADDR(LEGACY_BUS1, 30, 4, 0x98)

#define LEGACY_CSR_CPUBUSNO_VCU                            CF8_REG_ADDR(LEGACY_BUS1, 31, 0, 0xA0)               // CSR 1:31:0:0xA0
#define LEGACY_CSR_CPUBUSNO1_VCU                           CF8_REG_ADDR(LEGACY_BUS1, 31, 0, 0xA4)               // CSR 1:31:0:0xA4
#define LEGACY_CSR_CPUBUSNO_VALID_VCU                      CF8_REG_ADDR(LEGACY_BUS1, 31, 0, 0xA8)               // CSR 1:31:0:0xA8

#define PCI_MMCFG_ADDRESS(base, bus, dev, func, reg) \
  (UINT64) ( base | \
  (((UINTN) bus) << 20) | \
  (((UINTN) dev) << 15) | \
  (((UINTN) func) << 12) | \
  ((UINTN) reg))

#define PACK_BITFIELD_4BW(VarReg32, BitIndex) (((VarReg32) & 0xF) << (BitIndex))
#define UNPACK_BITFIELD_4BW(VarReg32, BitIndex) ((VarReg32) >> (BitIndex) & 0xF)

#define PACK_BITFIELD_3BW(VarReg32, BitIndex) (((VarReg32) & 0x7) << (BitIndex))
#define UNPACK_BITFIELD_3BW(VarReg32, BitIndex) ((VarReg32) >> (BitIndex) & 0x7)

#define MAX_UPI_NCB_CRED_CPX 7
#define MAX_UPI_NCS_CRED_CPX (MAX_UPI_NCB_CRED_CPX-1)

#define CORE_RATIO_P1_ENCODING    0x3F
#define MESH_RATIO_P1_ENCODING    0x3F

/*********************************************************
                      S3M Related
*********************************************************/
//
// S3M Config and Softstrap data, maximum lenght is 0x40 DWs.
//
#define S3M_CPLD_DATA_BLOCK_SIZE      0x40

#endif // _KTI_SI_H_
