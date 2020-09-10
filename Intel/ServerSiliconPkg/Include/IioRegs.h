/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020 Intel Corporation. <BR>

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

#ifndef _IIO_REGS_H_
#define _IIO_REGS_H_

#include <IioSetupDefinitions.h>

/**
==================================================================================================
==================================  General Definitions          ==================================
==================================================================================================
**/
//-----------------------------------------------------------------------------------
// PCIE port index for SKX
//------------------------------------------------------------------------------------
#define SOCKET_0_INDEX           0
#define SOCKET_1_INDEX           21
#define SOCKET_2_INDEX           42
#define SOCKET_3_INDEX           63
#define SOCKET_4_INDEX           84
#define SOCKET_5_INDEX           105
#define SOCKET_6_INDEX           126
#define SOCKET_7_INDEX           147

//-----------------------------------------------------------------------------------
// PCIE port index for SPR
//------------------------------------------------------------------------------------
#define SOCKET_0_INDEX_SPR           0
#define SOCKET_1_INDEX_SPR           41
#define SOCKET_2_INDEX_SPR           82
#define SOCKET_3_INDEX_SPR           123
#define SOCKET_4_INDEX_SPR           164
#define SOCKET_5_INDEX_SPR           205
#define SOCKET_6_INDEX_SPR           246
#define SOCKET_7_INDEX_SPR           287

//-----------------------------------------------------------------------------------
// Number's ports per stack definitions for 10nm
//------------------------------------------------------------------------------------

// STACK0 for: ICX-SP Redefined / SNR / ICX-D
#define NUMBER_PORTS_PER_STACK0_10NM       1

// NON-STACK0 for: ICX-SP Redefined / SNR / ICX-D
#define NUMBER_PORTS_PER_NON_STACK0_10NM   4

// SPR
#define NUMBER_PORTS_PER_STACK_SPR         8

//-----------------------------------------------------------------------------------
// Max IIO definitions for 10nm CPUs
//------------------------------------------------------------------------------------
#define MAX_IIO_STACK_ICX                  6
#define MAX_IIO_STACK_ICXDE                6
#define MAX_IIO_STACK_SNR                  5
#define MAX_IIO_STACK_SKX                  6
#define MAX_IIO_STACK_SPR                 12

#define MAX_HFI_STACK                      2  // KNH Only
#define MAX_DINO_STACK                     4  // SPR

#define MAX_MS2IOSF_CHANNEL_PER_STACK      8  // Maximun number of MS2IOSF channels per stack

#define MAX_UNCORE_STACK                   2  // MAX_LOGIC_IIO_STACK - MAX_IIO_STACK
typedef enum {
  UNCORE_STACK0 = 0,
  UNCORE_STACK1,
  MAX_UNCORE_STACK_10nm
} UNCORE_STACKS;  // 10nm only

#define IIO_UPLINK_PORT_INDEX         5    //Port 2A is the uplink port in Neon-City ///TODO Check if this is required for SKX/Purley SKX_TTEST
#define MaxIIO                        MAX_SOCKET

#define TOTAL_CB3_DEVICES             64   // IOAT_TOTAL_FUNCS * MAX_SOCKET. Note: this covers up to 8S.
#define MAX_TOTAL_PORTS               (MAX_SOCKET * NUMBER_PORTS_PER_SOCKET)   //NUMBER_PORTS_PER_SOCKET * MaxIIO. As now, treats setup S0-S3 = S4_S7 as optimal

#ifdef SPR_HOST
  #define NUM_IAX                       4    //number of IAX per Socket
  #define NUM_DSA                       4    //number of DSA per Socket
  #define NUM_CPM                       4    //number of CPM per Socket
  #define NUM_HQM                       4    //number of HQM per Socket
#else
  #define NUM_IAX                       1    //number of IAX per Socket
  #define NUM_DSA                       1    //number of DSA per Socket
  #define NUM_CPM                       1    //number of CPM per Socket
  #define NUM_HQM                       1    //number of HQM per Socket
#endif

#define TOTAL_IIO_STACKS              48   // MAX_SOCKET * MAX_IIO_STACK. Not reflect architecture but only sysHost structure!
#define TOTAL_SYSTEM_IIO_STACKS       32   // In term of system architecture support

#define NUMBER_NTB_PORTS_PER_SOCKET       5
#define NUMBER_NTB_PORTS_PER_SOCKET_SKX   3
#define NUMBER_NTB_PORTS_PER_SOCKET_ICXDE 2
#define NUMBER_NTB_PORTS_PER_SOCKET_SNR   0  // Doesn't support NTB

#ifndef MAX_STACKS_PER_SOCKET
  #ifdef SPR_HOST
    #define MAX_STACKS_PER_SOCKET     12
    #define MAX_IIO_PORTS_PER_STACK   NUMBER_PORTS_PER_STACK_SPR
  #else // SPR_HOST
    #define MAX_STACKS_PER_SOCKET     6
    #define MAX_IIO_PORTS_PER_STACK   NUMBER_PORTS_PER_NON_STACK0_10NM
  #endif // SPR_HOST
#endif

#define MAX_IOU_PER_SOCKET      5     // Max IOU number per socket for all silicon generation, SKX, ICX, ICXDE, SNR and SPR.

#define MAX_VMD_ROOTPORTS_PER_PCH   20    // Max number of rootports in PCH
#define MAX_VMD_STACKS_PER_SOCKET   6     // Max number of stacks per socket supported by VMD

#define VARIABLE_FUNC3_ELEMENTS       4

#define MAX_IOU_PORT_DEVICES          4

#define IIO_INVALID_INSTANCE     ((UINT8)-1)  // Higher than MaxIIO
#define IIO_INVALID_STACK        ((UINT8)-1)  // Higher than MAX_IIO_STACK

#ifndef NELEMENTS
#define NELEMENTS(Array) (sizeof(Array)/sizeof((Array)[0]))
#endif


/**
==================================================================================================
==================================  IIO Root Port Definitions              ====================
==================================================================================================
**/
typedef enum {
  IIO_GEN3 = 3,
  IIO_GEN4 = 4,
  IIO_GEN5 = 5,
  IIO_GEN_MAX
} IIO_PORT_GEN;

typedef enum {
  PI5_MODE_DMI = 0,
  PI5_MODE_IAL,
  PI5_MODE_GEN4_ONLY,
  PI5_MODE_GEN5_ONLY,
  PI5_MODE_GEN5_AND_GEN4,
  PI5_MODE_NTB_GEN4,
  PI5_MODE_NTB,
  PI5_MODE_MAX
} PI5_MODES;

typedef enum {
  IIO_HCX_NONE = 0,
  IIO_HCA,
  IIO_HCB
} IIO_HCX_TYPE;

typedef enum {
  IioPortA = 0,
  IioPortB = 1,
  IioPortC = 2,
  IioPortD = 3,
  IioPortE = 4,
  IioPortF = 5,
  IioPortG = 6,
  IioPortH = 7
} IIOPORTS;

#define VRP_SECBUS                0x19  // Virtual Root Port secondary bus number offset
#define VRP_FUNC                  0     // Virtual Root Port 1/2 function number

//------------------------------------------------------------------------------------
// IIO device numbers mapped in OOBMSM registers
//------------------------------------------------------------------------------------
typedef enum {
  IioHqmDevice = 0,
  IioCpkDevice = 4,
  IioCpmDevice = 5
} IIO_DEVICE_NUMS_IN_OOBMSM;

//------------------------------------------------------------------------------------
// IIO IPs located in extended stacks for SNR/ICXDE programs
//------------------------------------------------------------------------------------
#define CPM_VRP_DEV_ID_SNR          0x18DA
#define CPMPF_WAVE_1_2_DEV_ID_1     0x18EE
#define CPMPF_WAVE_1_2_DEV_ID_2     0x18EF
#define CPMPF_WAVE_1_2_DEV_ID_3     0x18A0
#define CPMPF_WAVE_1_2_DEV_ID(x)    ((x == CPMPF_WAVE_1_2_DEV_ID_1) || (x == CPMPF_WAVE_1_2_DEV_ID_2) || (x == CPMPF_WAVE_1_2_DEV_ID_3))
#define CPM_TOTAL_FUNCS             0x01
#define CPM_FUNC_START              0x00
#define CPK_VRP_DEV_ID_SNR          0x18D1
#define CPKPF_WAVE_1_2_DEV_ID_MIN   0x1888
#define CPKPF_WAVE_1_2_DEV_ID_MAX   0x189F
#define CPKPF_WAVE_1_2_DEV_ID(x)    ((x >= CPKPF_WAVE_1_2_DEV_ID_MIN) && (x <= CPKPF_WAVE_1_2_DEV_ID_MAX))
#define CPK_TOTAL_FUNCS             0x08
#define CPK_FUNC_START              0x00
#define HQM_EP_DEV_ID_SNR           0x270B
#define HQM_TOTAL_FUNCS             0x01
#define HQM_FUNC_START              0x00
//------------------------------------------------------------------------------------
// IIO IPs located in extended stacks for SPR program
//------------------------------------------------------------------------------------
#define HCX_BUS_RANGE             0x04
#define HCB_BUS_RANGE             0x08
#define CPM_BUS_OFFSET            0x01
#define HQM_BUS_OFFSET            0x03
#define CPM1_BUS_OFFSET           0x05
#define HQM1_BUS_OFFSET           0x07
#define CPM_MMIO_SIZE             0x100000000         // 4G MMIO resource for CPM
#define HQM_MMIO_SIZE             0x400000000         // 16G MMIO resource for HQM

//-----------------------------------------------------------------------------------
// IOU index definitions for SKX CPUs
//------------------------------------------------------------------------------------
typedef enum {
 IioIou0 = 0,
 IioIou1,
 IioIou2,
 IioMcp0,
 IioMcp1,
 IioIouMax
} IIOIOUS;

//-----------------------------------------------------------------------------------
// IOU index definitions for 10nm CPUs
//------------------------------------------------------------------------------------
typedef enum {
 IioIou010nm =0,
 IioIou110nm,
 IioIou210nm,
 IioIou310nm,
 IioIou410nm,
 IioIouMax10nm
} IIOIOUS_10NM;

#define DFX_IIO_RC_FLOW_AUTO      2
#define DFX_IIO_RC_FLOW_ENABLE    1
#define DFX_IIO_RC_FLOW_DISABLE   0

#define DFX_IIO_LNK_TRAIN_AUTO    2
#define DFX_IIO_LNK_TRAIN_ENABLE  1
#define DFX_IIO_LNK_TRAIN_DISABLE 0

// Port Links width
#define MAX_PORT_LINK_WIDTH_BIT_MAP  0xFFFF
#define MAX_PORT_LINK_WIDTH          16
#define PORT_LINK_WIDTH_x16          16
#define PORT_LINK_WIDTH_x8           8
#define PORT_LINK_WIDTH_x4           4
#define PORT_LINK_WIDTH_x2           2
#define PORT_LINK_WIDTH_x1           1

// Max BDFs definitions
#define MAX_FUNC_NUM            8
#define MAX_DEV_NUM             32
#define MAX_BUS_NUM             256

// Max lanes per PCIE controller
#define MAX_IIO_PCIE_LANES       16

#define PORT_0_INDEX             0
#define PORT_A_INDEX             1
#define PORT_B_INDEX             2
#define PORT_C_INDEX             3
#define PORT_D_INDEX             4
#define PORT_E_INDEX             5
#define PORT_F_INDEX             6
#define PORT_G_INDEX             7
#define PORT_H_INDEX             8

//-----------------------------------------------------------------------------------
// Port Index definition for SKX
//------------------------------------------------------------------------------------
#define PCIE_PORT_2_DEV          0x02
// IOU0
#define PORT_1A_INDEX            1
#define PORT_1B_INDEX            2
#define PORT_1C_INDEX            3
#define PORT_1D_INDEX            4
// IOU1
#define PORT_2A_INDEX            5
#define PORT_2B_INDEX            6
#define PORT_2C_INDEX            7
#define PORT_2D_INDEX            8
// IOU2
#define PORT_3A_INDEX            9
#define PORT_3B_INDEX            10
#define PORT_3C_INDEX            11
#define PORT_3D_INDEX            12
//MCP0
#define PORT_4A_INDEX            13
#define PORT_4B_INDEX            14
#define PORT_4C_INDEX            15
#define PORT_4D_INDEX            16
//MCP1
#define PORT_5A_INDEX            17
#define PORT_5B_INDEX            18
#define PORT_5C_INDEX            19
#define PORT_5D_INDEX            20

//-----------------------------------------------------------------------------------
// Port Index definition for ICX-SP Legacy / ICX-G / KNH
//------------------------------------------------------------------------------------

// IOU0
#define PORT_1A_INDEX_0            1
#define PORT_1B_INDEX_0            2
#define PORT_1C_INDEX_0            3
#define PORT_1D_INDEX_0            4
// IOU1
#define PORT_2A_INDEX_1            5
#define PORT_2B_INDEX_1            6
#define PORT_2C_INDEX_1            7
#define PORT_2D_INDEX_1            8
// IOU2
#define PORT_3A_INDEX_1            9
#define PORT_3B_INDEX_1            10
#define PORT_3C_INDEX_1            11
#define PORT_3D_INDEX_1            12
// IOU3
#define PORT_4A_INDEX_2            13
#define PORT_4B_INDEX_2            14
#define PORT_4C_INDEX_2            15
#define PORT_4D_INDEX_2            16
// IOU4
#define PORT_5A_INDEX_2            17
#define PORT_5B_INDEX_2            18
#define PORT_5C_INDEX_2            19
#define PORT_5D_INDEX_2            20

//-----------------------------------------------------------------------------------
// Port Index definition for ICX-SP Redefined
//------------------------------------------------------------------------------------

// IOU0
#define PORT_1A_INDEX_1           1
#define PORT_1B_INDEX_1           2
#define PORT_1C_INDEX_1           3
#define PORT_1D_INDEX_1           4
// IOU1
#define PORT_2A_INDEX_2           5
#define PORT_2B_INDEX_2           6
#define PORT_2C_INDEX_2           7
#define PORT_2D_INDEX_2           8
// IOU2
#define PORT_3A_INDEX_3           9
#define PORT_3B_INDEX_3           10
#define PORT_3C_INDEX_3           11
#define PORT_3D_INDEX_3           12
// IOU3
#define PORT_4A_INDEX_4           13
#define PORT_4B_INDEX_4           14
#define PORT_4C_INDEX_4           15
#define PORT_4D_INDEX_4           16
// IOU4
#define PORT_5A_INDEX_5           17
#define PORT_5B_INDEX_5           18
#define PORT_5C_INDEX_5           19
#define PORT_5D_INDEX_5           20

//-----------------------------------------------------------------------------------
// Port Index definition for ICX-DE / SNR
//------------------------------------------------------------------------------------
// IOU0
#define PORT_1A_INDEX_S1           1
#define PORT_1B_INDEX_S1           2
#define PORT_1C_INDEX_S1           3
#define PORT_1D_INDEX_S1           4

// IOU1
#define PORT_2A_INDEX_S1           5
#define PORT_2B_INDEX_S1           6
#define PORT_2C_INDEX_S1           7
#define PORT_2D_INDEX_S1           8

//-----------------------------------------------------------------------------------
// Port Index definition for SPR
//------------------------------------------------------------------------------------

// IOU0
#define PORT_1A_INDEX_SPR           1
#define PORT_1B_INDEX_SPR           2
#define PORT_1C_INDEX_SPR           3
#define PORT_1D_INDEX_SPR           4
#define PORT_1E_INDEX_SPR           5
#define PORT_1F_INDEX_SPR           6
#define PORT_1G_INDEX_SPR           7
#define PORT_1H_INDEX_SPR           8
// IOU1
#define PORT_2A_INDEX_SPR           9
#define PORT_2B_INDEX_SPR           10
#define PORT_2C_INDEX_SPR           11
#define PORT_2D_INDEX_SPR           12
#define PORT_2E_INDEX_SPR           13
#define PORT_2F_INDEX_SPR           14
#define PORT_2G_INDEX_SPR           15
#define PORT_2H_INDEX_SPR           16
// IOU2
#define PORT_3A_INDEX_SPR           17
#define PORT_3B_INDEX_SPR           18
#define PORT_3C_INDEX_SPR           19
#define PORT_3D_INDEX_SPR           20
#define PORT_3E_INDEX_SPR           21
#define PORT_3F_INDEX_SPR           22
#define PORT_3G_INDEX_SPR           23
#define PORT_3H_INDEX_SPR           24
// IOU3
#define PORT_4A_INDEX_SPR           25
#define PORT_4B_INDEX_SPR           26
#define PORT_4C_INDEX_SPR           27
#define PORT_4D_INDEX_SPR           28
#define PORT_4E_INDEX_SPR           29
#define PORT_4F_INDEX_SPR           30
#define PORT_4G_INDEX_SPR           31
#define PORT_4H_INDEX_SPR           32
// IOU4
#define PORT_5A_INDEX_SPR           33
#define PORT_5B_INDEX_SPR           34
#define PORT_5C_INDEX_SPR           35
#define PORT_5D_INDEX_SPR           36
#define PORT_5E_INDEX_SPR           37
#define PORT_5F_INDEX_SPR           38
#define PORT_5G_INDEX_SPR           39
#define PORT_5H_INDEX_SPR           40
// IOU5
#define PORT_6A_INDEX_SPR           41
#define PORT_6B_INDEX_SPR           42
#define PORT_6C_INDEX_SPR           43
#define PORT_6D_INDEX_SPR           44
#define PORT_6E_INDEX_SPR           45
#define PORT_6F_INDEX_SPR           46
#define PORT_6G_INDEX_SPR           47
#define PORT_6H_INDEX_SPR           48
// IOU6
#define PORT_7A_INDEX_SPR           49
#define PORT_7B_INDEX_SPR           50
#define PORT_7C_INDEX_SPR           51
#define PORT_7D_INDEX_SPR           52
#define PORT_7E_INDEX_SPR           53
#define PORT_7F_INDEX_SPR           54
#define PORT_7G_INDEX_SPR           55
#define PORT_7H_INDEX_SPR           56

//-----------------------------------------------------------------------------------
// Port Device/Function definition for SKX
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_DEV     0x00
#define PCIE_PORT_0_FUNC    0x00

#define PCIE_PORT_1A_DEV    0x00
#define PCIE_PORT_1B_DEV    0x01
#define PCIE_PORT_1C_DEV    0x02
#define PCIE_PORT_1D_DEV    0x03
#define PCIE_PORT_1A_FUNC   0x00
#define PCIE_PORT_1B_FUNC   0x00
#define PCIE_PORT_1C_FUNC   0x00
#define PCIE_PORT_1D_FUNC   0x00

#define PCIE_PORT_2A_DEV    0x00
#define PCIE_PORT_2B_DEV    0x01
#define PCIE_PORT_2C_DEV    0x02
#define PCIE_PORT_2D_DEV    0x03
#define PCIE_PORT_2A_FUNC   0x00
#define PCIE_PORT_2B_FUNC   0x00
#define PCIE_PORT_2C_FUNC   0x00
#define PCIE_PORT_2D_FUNC   0x00

#define PCIE_PORT_3A_DEV    0x00
#define PCIE_PORT_3B_DEV    0x01
#define PCIE_PORT_3C_DEV    0x02
#define PCIE_PORT_3D_DEV    0x03
#define PCIE_PORT_3A_FUNC   0x00
#define PCIE_PORT_3B_FUNC   0x00
#define PCIE_PORT_3C_FUNC   0x00
#define PCIE_PORT_3D_FUNC   0x00

#define PCIE_PORT_4A_DEV    0x00
#define PCIE_PORT_4B_DEV    0x01
#define PCIE_PORT_4C_DEV    0x02
#define PCIE_PORT_4D_DEV    0x03
#define PCIE_PORT_4A_FUNC   0x00
#define PCIE_PORT_4B_FUNC   0x00
#define PCIE_PORT_4C_FUNC   0x00
#define PCIE_PORT_4D_FUNC   0x00

#define PCIE_PORT_5A_DEV    0x00
#define PCIE_PORT_5B_DEV    0x01
#define PCIE_PORT_5C_DEV    0x02
#define PCIE_PORT_5D_DEV    0x03
#define PCIE_PORT_5A_FUNC   0x00
#define PCIE_PORT_5B_FUNC   0x00
#define PCIE_PORT_5C_FUNC   0x00
#define PCIE_PORT_5D_FUNC   0x00

//-----------------------------------------------------------------------------------
// Port Index definition for ICX-SP Legacy / ICX-G / KNH
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_DEV_0     0x03
#define PCIE_PORT_0_FUNC_0    0x00

#define PCIE_PORT_1A_DEV_0    0x04
#define PCIE_PORT_1B_DEV_0    0x05
#define PCIE_PORT_1C_DEV_0    0x06
#define PCIE_PORT_1D_DEV_0    0x07
#define PCIE_PORT_1A_FUNC_0   0x00
#define PCIE_PORT_1B_FUNC_0   0x00
#define PCIE_PORT_1C_FUNC_0   0x00
#define PCIE_PORT_1D_FUNC_0   0x00

#define PCIE_PORT_2A_DEV_1    0x04
#define PCIE_PORT_2B_DEV_1    0x05
#define PCIE_PORT_2C_DEV_1    0x06
#define PCIE_PORT_2D_DEV_1    0x07
#define PCIE_PORT_2A_FUNC_1   0x00
#define PCIE_PORT_2B_FUNC_1   0x00
#define PCIE_PORT_2C_FUNC_1   0x00
#define PCIE_PORT_2D_FUNC_1   0x00

#define PCIE_PORT_3A_DEV_1    0x08
#define PCIE_PORT_3B_DEV_1    0x09
#define PCIE_PORT_3C_DEV_1    0x0A
#define PCIE_PORT_3D_DEV_1    0x0B
#define PCIE_PORT_3A_FUNC_1   0x00
#define PCIE_PORT_3B_FUNC_1   0x00
#define PCIE_PORT_3C_FUNC_1   0x00
#define PCIE_PORT_3D_FUNC_1   0x00

#define PCIE_PORT_4A_DEV_2    0x04
#define PCIE_PORT_4B_DEV_2    0x05
#define PCIE_PORT_4C_DEV_2    0x06
#define PCIE_PORT_4D_DEV_2    0x07
#define PCIE_PORT_4A_FUNC_2   0x00
#define PCIE_PORT_4B_FUNC_2   0x00
#define PCIE_PORT_4C_FUNC_2   0x00
#define PCIE_PORT_4D_FUNC_2   0x00

#define PCIE_PORT_5A_DEV_2    0x08
#define PCIE_PORT_5B_DEV_2    0x09
#define PCIE_PORT_5C_DEV_2    0x0A
#define PCIE_PORT_5D_DEV_2    0x0B
#define PCIE_PORT_5A_FUNC_2   0x00
#define PCIE_PORT_5B_FUNC_2   0x00
#define PCIE_PORT_5C_FUNC_2   0x00
#define PCIE_PORT_5D_FUNC_2   0x00

//-----------------------------------------------------------------------------------
// Port Index definition for  SPR 0P8
//------------------------------------------------------------------------------------
#define PCIE_PORT_A_DEV_SPR    0x01
#define PCIE_PORT_B_DEV_SPR    0x02
#define PCIE_PORT_C_DEV_SPR    0x03
#define PCIE_PORT_D_DEV_SPR    0x04
#define PCIE_PORT_E_DEV_SPR    0x05
#define PCIE_PORT_F_DEV_SPR    0x06
#define PCIE_PORT_G_DEV_SPR    0x07
#define PCIE_PORT_H_DEV_SPR    0x08
#define IIO_NTB_DEVICE_NUM_SPR 0x09
#define PCIE_PORT_FUNC_SPR     0x00

//-----------------------------------------------------------------------------------
// Port Index definition for ICX-SP Redefined / SPR
//------------------------------------------------------------------------------------
#define PCIE_PORT_1A_DEV_1    0x02
#define PCIE_PORT_1B_DEV_1    0x03
#define PCIE_PORT_1C_DEV_1    0x04
#define PCIE_PORT_1D_DEV_1    0x05
#define PCIE_PORT_1A_FUNC_1   0x00
#define PCIE_PORT_1B_FUNC_1   0x00
#define PCIE_PORT_1C_FUNC_1   0x00
#define PCIE_PORT_1D_FUNC_1   0x00


#define PCIE_PORT_2A_DEV_2    0x02
#define PCIE_PORT_2B_DEV_2    0x03
#define PCIE_PORT_2C_DEV_2    0x04
#define PCIE_PORT_2D_DEV_2    0x05
#define PCIE_PORT_2A_FUNC_2   0x00
#define PCIE_PORT_2B_FUNC_2   0x00
#define PCIE_PORT_2C_FUNC_2   0x00
#define PCIE_PORT_2D_FUNC_2   0x00

#define PCIE_PORT_3A_DEV_3    0x02
#define PCIE_PORT_3B_DEV_3    0x03
#define PCIE_PORT_3C_DEV_3    0x04
#define PCIE_PORT_3D_DEV_3    0x05
#define PCIE_PORT_3A_FUNC_3   0x00
#define PCIE_PORT_3B_FUNC_3   0x00
#define PCIE_PORT_3C_FUNC_3   0x00
#define PCIE_PORT_3D_FUNC_3   0x00

#define PCIE_PORT_4A_DEV_4    0x02
#define PCIE_PORT_4B_DEV_4    0x03
#define PCIE_PORT_4C_DEV_4    0x04
#define PCIE_PORT_4D_DEV_4    0x05
#define PCIE_PORT_4A_FUNC_4   0x00
#define PCIE_PORT_4B_FUNC_4   0x00
#define PCIE_PORT_4C_FUNC_4   0x00
#define PCIE_PORT_4D_FUNC_4   0x00

#define PCIE_PORT_5A_DEV_5    0x02
#define PCIE_PORT_5B_DEV_5    0x03
#define PCIE_PORT_5C_DEV_5    0x04
#define PCIE_PORT_5D_DEV_5    0x05
#define PCIE_PORT_5A_FUNC_5   0x00
#define PCIE_PORT_5B_FUNC_5   0x00
#define PCIE_PORT_5C_FUNC_5   0x00
#define PCIE_PORT_5D_FUNC_5   0x00

#define NTB_PORT_1A_DEV_1    0x01
#define NTB_PORT_1A_FUNC_1   0x00
#define NTB_PORT_2A_DEV_2    0x01
#define NTB_PORT_2A_FUNC_2   0x00
#define NTB_PORT_3A_DEV_3    0x01
#define NTB_PORT_3A_FUNC_3   0x00
#define NTB_PORT_4A_DEV_4    0x01
#define NTB_PORT_4A_FUNC_4   0x00
#define NTB_PORT_5A_DEV_5    0x01
#define NTB_PORT_5A_FUNC_5   0x00

//-----------------------------------------------------------------------------------
// Port Index definition for ICX-DE / SNR
//------------------------------------------------------------------------------------
#define PCIE_PORT_1A_DEV_S1    0x04
#define PCIE_PORT_1B_DEV_S1    0x05
#define PCIE_PORT_1C_DEV_S1    0x06
#define PCIE_PORT_1D_DEV_S1    0x07
#define PCIE_PORT_1A_FUNC_S1   0x00
#define PCIE_PORT_1B_FUNC_S1   0x00
#define PCIE_PORT_1C_FUNC_S1   0x00
#define PCIE_PORT_1D_FUNC_S1   0x00

#define PCIE_PORT_2A_DEV_S1    0x08
#define PCIE_PORT_2B_DEV_S1    0x09
#define PCIE_PORT_2C_DEV_S1    0x0A
#define PCIE_PORT_2D_DEV_S1    0x0B
#define PCIE_PORT_2A_FUNC_S1   0x00
#define PCIE_PORT_2B_FUNC_S1   0x00
#define PCIE_PORT_2C_FUNC_S1   0x00
#define PCIE_PORT_2D_FUNC_S1   0x00

//-----------------------------------------------------------------------------------
// Port DFX Device/Function definition for SKX
//------------------------------------------------------------------------------------

#define PCIE_PORT_GLOBAL_DFX_DEV     0x07
#define PCIE_PORT_GLOBAL_DFX_FUNC    0x07

#define PCIE_PORT_0_DFX_DEV          0x07
#define PCIE_PORT_0_DFX_FUNC         0x00

#define PCIE_PORT_1A_DFX_DEV    0x07
#define PCIE_PORT_1A_DFX_FUNC   0x00
#define PCIE_PORT_1B_DFX_DEV    0x07
#define PCIE_PORT_1B_DFX_FUNC   0x01
#define PCIE_PORT_1C_DFX_DEV    0x07
#define PCIE_PORT_1C_DFX_FUNC   0x02
#define PCIE_PORT_1D_DFX_DEV    0x07
#define PCIE_PORT_1D_DFX_FUNC   0x03

#define PCIE_PORT_2A_DFX_DEV    0x07
#define PCIE_PORT_2A_DFX_FUNC   0x00
#define PCIE_PORT_2B_DFX_DEV    0x07
#define PCIE_PORT_2B_DFX_FUNC   0x01
#define PCIE_PORT_2C_DFX_DEV    0x07
#define PCIE_PORT_2C_DFX_FUNC   0x02
#define PCIE_PORT_2D_DFX_DEV    0x07
#define PCIE_PORT_2D_DFX_FUNC   0x03

#define PCIE_PORT_3A_DFX_DEV    0x07
#define PCIE_PORT_3A_DFX_FUNC   0x00
#define PCIE_PORT_3B_DFX_DEV    0x07
#define PCIE_PORT_3B_DFX_FUNC   0x01
#define PCIE_PORT_3C_DFX_DEV    0x07
#define PCIE_PORT_3C_DFX_FUNC   0x02
#define PCIE_PORT_3D_DFX_DEV    0x07
#define PCIE_PORT_3D_DFX_FUNC   0x03

#define PCIE_PORT_4A_DFX_DEV    0x07
#define PCIE_PORT_4A_DFX_FUNC   0x00
#define PCIE_PORT_4B_DFX_DEV    0x07
#define PCIE_PORT_4B_DFX_FUNC   0x01
#define PCIE_PORT_4C_DFX_DEV    0x07
#define PCIE_PORT_4C_DFX_FUNC   0x02
#define PCIE_PORT_4D_DFX_DEV    0x07
#define PCIE_PORT_4D_DFX_FUNC   0x03

#define PCIE_PORT_5A_DFX_DEV    0x07
#define PCIE_PORT_5A_DFX_FUNC   0x00
#define PCIE_PORT_5B_DFX_DEV    0x07
#define PCIE_PORT_5B_DFX_FUNC   0x01
#define PCIE_PORT_5C_DFX_DEV    0x07
#define PCIE_PORT_5C_DFX_FUNC   0x02
#define PCIE_PORT_5D_DFX_DEV    0x07
#define PCIE_PORT_5D_DFX_FUNC   0x03

//-----------------------------------------------------------------------------------
// GLobal DFX Device/Function definition for 10nm projects
//------------------------------------------------------------------------------------
#define PCIE_PORT_GLOBAL_DFX_DEV_U1   0x1F
#define PCIE_PORT_GLOBAL_DFX_FUNC_U1  0x00

//-----------------------------------------------------------------------------------
// Port DFX Device/Function definition for ICX-SP Legacy / ICX-G / KNH
//------------------------------------------------------------------------------------
#define PCIE_PORT_GLOBAL_DFX_DEV_0   0x1F
#define PCIE_PORT_GLOBAL_DFX_FUNC_0  0x00

#define PCIE_PORT_0_DFX_DEV_0     0x03
#define PCIE_PORT_0_DFX_FUNC_0    0x01

#define PCIE_PORT_1A_DFX_DEV_0    0x04
#define PCIE_PORT_1A_DFX_FUNC_0   0x01
#define PCIE_PORT_1B_DFX_DEV_0    0x05
#define PCIE_PORT_1B_DFX_FUNC_0   0x01
#define PCIE_PORT_1C_DFX_DEV_0    0x06
#define PCIE_PORT_1C_DFX_FUNC_0   0x01
#define PCIE_PORT_1D_DFX_DEV_0    0x07
#define PCIE_PORT_1D_DFX_FUNC_0   0x01

#define PCIE_PORT_2A_DFX_DEV_1    0x04
#define PCIE_PORT_2A_DFX_FUNC_1   0x01
#define PCIE_PORT_2B_DFX_DEV_1    0x05
#define PCIE_PORT_2B_DFX_FUNC_1   0x01
#define PCIE_PORT_2C_DFX_DEV_1    0x06
#define PCIE_PORT_2C_DFX_FUNC_1   0x01
#define PCIE_PORT_2D_DFX_DEV_1    0x07
#define PCIE_PORT_2D_DFX_FUNC_1   0x01

#define PCIE_PORT_3A_DFX_DEV_1    0x08
#define PCIE_PORT_3A_DFX_FUNC_1   0x01
#define PCIE_PORT_3B_DFX_DEV_1    0x09
#define PCIE_PORT_3B_DFX_FUNC_1   0x01
#define PCIE_PORT_3C_DFX_DEV_1    0x0A
#define PCIE_PORT_3C_DFX_FUNC_1   0x01
#define PCIE_PORT_3D_DFX_DEV_1    0x0B
#define PCIE_PORT_3D_DFX_FUNC_1   0x01

#define PCIE_PORT_4A_DFX_DEV_2    0x04
#define PCIE_PORT_4A_DFX_FUNC_2   0x01
#define PCIE_PORT_4B_DFX_DEV_2    0x05
#define PCIE_PORT_4B_DFX_FUNC_2   0x01
#define PCIE_PORT_4C_DFX_DEV_2    0x06
#define PCIE_PORT_4C_DFX_FUNC_2   0x01
#define PCIE_PORT_4D_DFX_DEV_2    0x07
#define PCIE_PORT_4D_DFX_FUNC_2   0x01

#define PCIE_PORT_5A_DFX_DEV_2    0x08
#define PCIE_PORT_5A_DFX_FUNC_2   0x01
#define PCIE_PORT_5B_DFX_DEV_2    0x09
#define PCIE_PORT_5B_DFX_FUNC_2   0x01
#define PCIE_PORT_5C_DFX_DEV_2    0x0A
#define PCIE_PORT_5C_DFX_FUNC_2   0x01
#define PCIE_PORT_5D_DFX_DEV_2    0x0B
#define PCIE_PORT_5D_DFX_FUNC_2   0x01

//-----------------------------------------------------------------------------------
// Port DFX Device/Function definition for ICX-DE / SNR
//------------------------------------------------------------------------------------

#define PCIE_PORT_GLOBAL_DFX_DEV_S1   0x1F
#define PCIE_PORT_GLOBAL_DFX_FUNC_S1  0x00

#define PCIE_PORT_1A_DFX_DEV_S1   0x04
#define PCIE_PORT_1A_DFX_FUNC_S1  0x01
#define PCIE_PORT_1B_DFX_DEV_S1   0x05
#define PCIE_PORT_1B_DFX_FUNC_S1  0x01
#define PCIE_PORT_1C_DFX_DEV_S1   0x06
#define PCIE_PORT_1C_DFX_FUNC_S1  0x01
#define PCIE_PORT_1D_DFX_DEV_S1   0x07
#define PCIE_PORT_1D_DFX_FUNC_S1  0x01

#define PCIE_PORT_2A_DFX_DEV_S1   0x08
#define PCIE_PORT_2A_DFX_FUNC_S1  0x01
#define PCIE_PORT_2B_DFX_DEV_S1   0x09
#define PCIE_PORT_2B_DFX_FUNC_S1  0x01
#define PCIE_PORT_2C_DFX_DEV_S1   0x0A
#define PCIE_PORT_2C_DFX_FUNC_S1  0x01
#define PCIE_PORT_2D_DFX_DEV_S1   0x0B
#define PCIE_PORT_2D_DFX_FUNC_S1  0x01

// Generic Device/Function definitions for Non-DFX Ports
#define NON_DFX_PCIE_PORT_DEV     0xFF
#define NON_DFX_PCIE_PORT_FUNC    0xFF

//-----------------------------------------------------------------------------------
// Port Link_Select definition for SKX
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_LINK_SEL  0x00

#define PCIE_PORT_1A_LINK_SEL  0x00
#define PCIE_PORT_1B_LINK_SEL  0x01
#define PCIE_PORT_1C_LINK_SEL  0x02
#define PCIE_PORT_1D_LINK_SEL  0x03

#define PCIE_PORT_2A_LINK_SEL  0x00
#define PCIE_PORT_2B_LINK_SEL  0x01
#define PCIE_PORT_2C_LINK_SEL  0x02
#define PCIE_PORT_2D_LINK_SEL  0x03

#define PCIE_PORT_3A_LINK_SEL  0x00
#define PCIE_PORT_3B_LINK_SEL  0x01
#define PCIE_PORT_3C_LINK_SEL  0x02
#define PCIE_PORT_3D_LINK_SEL  0x03

#define PCIE_PORT_4A_LINK_SEL  0x00
#define PCIE_PORT_4B_LINK_SEL  0x01
#define PCIE_PORT_4C_LINK_SEL  0x02
#define PCIE_PORT_4D_LINK_SEL  0x03

#define PCIE_PORT_5A_LINK_SEL  0x00
#define PCIE_PORT_5B_LINK_SEL  0x01
#define PCIE_PORT_5C_LINK_SEL  0x02
#define PCIE_PORT_5D_LINK_SEL  0x03

//-----------------------------------------------------------------------------------
// Port Link_Select definition for ICX-SP Legacy / ICX-G / KNH
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_LINK_SEL_0  0x00

#define PCIE_PORT_1A_LINK_SEL_0  0x00
#define PCIE_PORT_1B_LINK_SEL_0  0x01
#define PCIE_PORT_1C_LINK_SEL_0  0x02
#define PCIE_PORT_1D_LINK_SEL_0  0x03

#define PCIE_PORT_2A_LINK_SEL_1  0x00
#define PCIE_PORT_2B_LINK_SEL_1  0x01
#define PCIE_PORT_2C_LINK_SEL_1  0x02
#define PCIE_PORT_2D_LINK_SEL_1  0x03

#define PCIE_PORT_3A_LINK_SEL_1  0x00
#define PCIE_PORT_3B_LINK_SEL_1  0x01
#define PCIE_PORT_3C_LINK_SEL_1  0x02
#define PCIE_PORT_3D_LINK_SEL_1  0x03

#define PCIE_PORT_4A_LINK_SEL_2  0x00
#define PCIE_PORT_4B_LINK_SEL_2  0x01
#define PCIE_PORT_4C_LINK_SEL_2  0x02
#define PCIE_PORT_4D_LINK_SEL_2  0x03

#define PCIE_PORT_5A_LINK_SEL_2  0x00
#define PCIE_PORT_5B_LINK_SEL_2  0x01
#define PCIE_PORT_5C_LINK_SEL_2  0x02
#define PCIE_PORT_5D_LINK_SEL_2  0x03

//-----------------------------------------------------------------------------------
// Port Link_Select definition for ICX-SP Redefined / SPR
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_LINK_SEL_0  0x00

#define PCIE_PORT_1A_LINK_SEL_1  0x00
#define PCIE_PORT_1B_LINK_SEL_1  0x01
#define PCIE_PORT_1C_LINK_SEL_1  0x02
#define PCIE_PORT_1D_LINK_SEL_1  0x03
#define PCIE_PORT_1E_LINK_SEL_1  0x04
#define PCIE_PORT_1F_LINK_SEL_1  0x05
#define PCIE_PORT_1G_LINK_SEL_1  0x06
#define PCIE_PORT_1H_LINK_SEL_1  0x07

#define PCIE_PORT_2A_LINK_SEL_2  0x00
#define PCIE_PORT_2B_LINK_SEL_2  0x01
#define PCIE_PORT_2C_LINK_SEL_2  0x02
#define PCIE_PORT_2D_LINK_SEL_2  0x03
#define PCIE_PORT_2E_LINK_SEL_2  0x04
#define PCIE_PORT_2F_LINK_SEL_2  0x05
#define PCIE_PORT_2G_LINK_SEL_2  0x06
#define PCIE_PORT_2H_LINK_SEL_2  0x07

#define PCIE_PORT_3A_LINK_SEL_3  0x00
#define PCIE_PORT_3B_LINK_SEL_3  0x01
#define PCIE_PORT_3C_LINK_SEL_3  0x02
#define PCIE_PORT_3D_LINK_SEL_3  0x03
#define PCIE_PORT_3E_LINK_SEL_3  0x04
#define PCIE_PORT_3F_LINK_SEL_3  0x05
#define PCIE_PORT_3G_LINK_SEL_3  0x06
#define PCIE_PORT_3H_LINK_SEL_3  0x07

#define PCIE_PORT_4A_LINK_SEL_4  0x00
#define PCIE_PORT_4B_LINK_SEL_4  0x01
#define PCIE_PORT_4C_LINK_SEL_4  0x02
#define PCIE_PORT_4D_LINK_SEL_4  0x03
#define PCIE_PORT_4E_LINK_SEL_4  0x04
#define PCIE_PORT_4F_LINK_SEL_4  0x05
#define PCIE_PORT_4G_LINK_SEL_4  0x06
#define PCIE_PORT_4H_LINK_SEL_4  0x07

#define PCIE_PORT_5A_LINK_SEL_5  0x00
#define PCIE_PORT_5B_LINK_SEL_5  0x01
#define PCIE_PORT_5C_LINK_SEL_5  0x02
#define PCIE_PORT_5D_LINK_SEL_5  0x03
#define PCIE_PORT_5E_LINK_SEL_5  0x04
#define PCIE_PORT_5F_LINK_SEL_5  0x05
#define PCIE_PORT_5G_LINK_SEL_5  0x06
#define PCIE_PORT_5H_LINK_SEL_5  0x07

//-----------------------------------------------------------------------------------
// Port Link_Select definition for ICX-DE / SNR
//------------------------------------------------------------------------------------
#define PCIE_PORT_1A_LINK_SEL_S1 0x00
#define PCIE_PORT_1B_LINK_SEL_S1 0x01
#define PCIE_PORT_1C_LINK_SEL_S1 0x02
#define PCIE_PORT_1D_LINK_SEL_S1 0x03

#define PCIE_PORT_2A_LINK_SEL_S1 0x00
#define PCIE_PORT_2B_LINK_SEL_S1 0x01
#define PCIE_PORT_2C_LINK_SEL_S1 0x02
#define PCIE_PORT_2D_LINK_SEL_S1 0x03

//-----------------------------------------------------------------------------------
// Port SuperCluster Index definition for SKX
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_SUPER_CLUSTER_PORT  0x00

#define PCIE_PORT_1A_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_1B_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_1C_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_1D_SUPER_CLUSTER_PORT  0x01

#define PCIE_PORT_2A_SUPER_CLUSTER_PORT  0x05
#define PCIE_PORT_2B_SUPER_CLUSTER_PORT  0x05
#define PCIE_PORT_2C_SUPER_CLUSTER_PORT  0x05
#define PCIE_PORT_2D_SUPER_CLUSTER_PORT  0x05

#define PCIE_PORT_3A_SUPER_CLUSTER_PORT  0x09
#define PCIE_PORT_3B_SUPER_CLUSTER_PORT  0x09
#define PCIE_PORT_3C_SUPER_CLUSTER_PORT  0x09
#define PCIE_PORT_3D_SUPER_CLUSTER_PORT  0x09

#define PCIE_PORT_4A_SUPER_CLUSTER_PORT  0x0D
#define PCIE_PORT_4B_SUPER_CLUSTER_PORT  0x0D
#define PCIE_PORT_4C_SUPER_CLUSTER_PORT  0x0D
#define PCIE_PORT_4D_SUPER_CLUSTER_PORT  0x0D

#define PCIE_PORT_5A_SUPER_CLUSTER_PORT  0x11
#define PCIE_PORT_5B_SUPER_CLUSTER_PORT  0x11
#define PCIE_PORT_5C_SUPER_CLUSTER_PORT  0x11
#define PCIE_PORT_5D_SUPER_CLUSTER_PORT  0x11

//-----------------------------------------------------------------------------------
// Port SuperCluster Index definition for ICX-SP Legacy / ICX-G / KNH
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_SUPER_CLUSTER_PORT_0  0x00

#define PCIE_PORT_1A_SUPER_CLUSTER_PORT_0  0x01
#define PCIE_PORT_1B_SUPER_CLUSTER_PORT_0  0x01
#define PCIE_PORT_1C_SUPER_CLUSTER_PORT_0  0x01
#define PCIE_PORT_1D_SUPER_CLUSTER_PORT_0  0x01

#define PCIE_PORT_2A_SUPER_CLUSTER_PORT_1  0x05
#define PCIE_PORT_2B_SUPER_CLUSTER_PORT_1  0x05
#define PCIE_PORT_2C_SUPER_CLUSTER_PORT_1  0x05
#define PCIE_PORT_2D_SUPER_CLUSTER_PORT_1  0x05

#define PCIE_PORT_3A_SUPER_CLUSTER_PORT_1  0x09
#define PCIE_PORT_3B_SUPER_CLUSTER_PORT_1  0x09
#define PCIE_PORT_3C_SUPER_CLUSTER_PORT_1  0x09
#define PCIE_PORT_3D_SUPER_CLUSTER_PORT_1  0x09

#define PCIE_PORT_4A_SUPER_CLUSTER_PORT_2  0x0D
#define PCIE_PORT_4B_SUPER_CLUSTER_PORT_2  0x0D
#define PCIE_PORT_4C_SUPER_CLUSTER_PORT_2  0x0D
#define PCIE_PORT_4D_SUPER_CLUSTER_PORT_2  0x0D

#define PCIE_PORT_5A_SUPER_CLUSTER_PORT_2  0x11
#define PCIE_PORT_5B_SUPER_CLUSTER_PORT_2  0x11
#define PCIE_PORT_5C_SUPER_CLUSTER_PORT_2  0x11
#define PCIE_PORT_5D_SUPER_CLUSTER_PORT_2  0x11

//-----------------------------------------------------------------------------------
// Port SuperCluster Index definition for ICX-SP Redefined
//------------------------------------------------------------------------------------
#define PCIE_PORT_1A_SUPER_CLUSTER_PORT_1  0x01
#define PCIE_PORT_1B_SUPER_CLUSTER_PORT_1  0x01
#define PCIE_PORT_1C_SUPER_CLUSTER_PORT_1  0x01
#define PCIE_PORT_1D_SUPER_CLUSTER_PORT_1  0x01
#define PCIE_PORT_1E_SUPER_CLUSTER_PORT_1  0x01
#define PCIE_PORT_1F_SUPER_CLUSTER_PORT_1  0x01
#define PCIE_PORT_1G_SUPER_CLUSTER_PORT_1  0x01
#define PCIE_PORT_1H_SUPER_CLUSTER_PORT_1  0x01

#define PCIE_PORT_2A_SUPER_CLUSTER_PORT_2  0x05
#define PCIE_PORT_2B_SUPER_CLUSTER_PORT_2  0x05
#define PCIE_PORT_2C_SUPER_CLUSTER_PORT_2  0x05
#define PCIE_PORT_2D_SUPER_CLUSTER_PORT_2  0x05
#define PCIE_PORT_2E_SUPER_CLUSTER_PORT_2  0x05
#define PCIE_PORT_2F_SUPER_CLUSTER_PORT_2  0x05
#define PCIE_PORT_2G_SUPER_CLUSTER_PORT_2  0x05
#define PCIE_PORT_2H_SUPER_CLUSTER_PORT_2  0x05

#define PCIE_PORT_3A_SUPER_CLUSTER_PORT_3  0x09
#define PCIE_PORT_3B_SUPER_CLUSTER_PORT_3  0x09
#define PCIE_PORT_3C_SUPER_CLUSTER_PORT_3  0x09
#define PCIE_PORT_3D_SUPER_CLUSTER_PORT_3  0x09
#define PCIE_PORT_3E_SUPER_CLUSTER_PORT_3  0x09
#define PCIE_PORT_3F_SUPER_CLUSTER_PORT_3  0x09
#define PCIE_PORT_3G_SUPER_CLUSTER_PORT_3  0x09
#define PCIE_PORT_3H_SUPER_CLUSTER_PORT_3  0x09

#define PCIE_PORT_4A_SUPER_CLUSTER_PORT_4  0x0D
#define PCIE_PORT_4B_SUPER_CLUSTER_PORT_4  0x0D
#define PCIE_PORT_4C_SUPER_CLUSTER_PORT_4  0x0D
#define PCIE_PORT_4D_SUPER_CLUSTER_PORT_4  0x0D
#define PCIE_PORT_4E_SUPER_CLUSTER_PORT_4  0x0D
#define PCIE_PORT_4F_SUPER_CLUSTER_PORT_4  0x0D
#define PCIE_PORT_4G_SUPER_CLUSTER_PORT_4  0x0D
#define PCIE_PORT_4H_SUPER_CLUSTER_PORT_4  0x0D

#define PCIE_PORT_5A_SUPER_CLUSTER_PORT_5  0x11
#define PCIE_PORT_5B_SUPER_CLUSTER_PORT_5  0x11
#define PCIE_PORT_5C_SUPER_CLUSTER_PORT_5  0x11
#define PCIE_PORT_5D_SUPER_CLUSTER_PORT_5  0x11
#define PCIE_PORT_5E_SUPER_CLUSTER_PORT_5  0x11
#define PCIE_PORT_5F_SUPER_CLUSTER_PORT_5  0x11
#define PCIE_PORT_5G_SUPER_CLUSTER_PORT_5  0x11
#define PCIE_PORT_5H_SUPER_CLUSTER_PORT_5  0x11

//-----------------------------------------------------------------------------------
// Port SuperCluster Index definition for ICX-DE / SNR
//------------------------------------------------------------------------------------
#define PCIE_PORT_1A_SUPER_CLUSTER_PORT_S1 0x01
#define PCIE_PORT_1B_SUPER_CLUSTER_PORT_S1 0x01
#define PCIE_PORT_1C_SUPER_CLUSTER_PORT_S1 0x01
#define PCIE_PORT_1D_SUPER_CLUSTER_PORT_S1 0x01

#define PCIE_PORT_2A_SUPER_CLUSTER_PORT_S1 0x05
#define PCIE_PORT_2B_SUPER_CLUSTER_PORT_S1 0x05
#define PCIE_PORT_2C_SUPER_CLUSTER_PORT_S1 0x05
#define PCIE_PORT_2D_SUPER_CLUSTER_PORT_S1 0x05

//-----------------------------------------------------------------------------------
// Port SuperCluster Index definition for  SPR
//------------------------------------------------------------------------------------
#define PCIE_PORT_0_SUPER_CLUSTER_PORT_SPR   0
#define PCIE_PORT_1A_SUPER_CLUSTER_PORT_SPR  1
#define PCIE_PORT_1H_SUPER_CLUSTER_PORT_SPR  8
#define PCIE_PORT_2A_SUPER_CLUSTER_PORT_SPR  9
#define PCIE_PORT_2H_SUPER_CLUSTER_PORT_SPR  16
#define PCIE_PORT_3A_SUPER_CLUSTER_PORT_SPR  17
#define PCIE_PORT_3H_SUPER_CLUSTER_PORT_SPR  24
#define PCIE_PORT_4A_SUPER_CLUSTER_PORT_SPR  25
#define PCIE_PORT_4H_SUPER_CLUSTER_PORT_SPR  32
#define PCIE_PORT_5A_SUPER_CLUSTER_PORT_SPR  33
#define PCIE_PORT_5H_SUPER_CLUSTER_PORT_SPR  40

#define R_PCIE_PORT_PCICMD                        0x04
#define   B_PCIE_PORT_PCICMD_PERRE                  BIT6
#define   B_PCIE_PORT_PCICMD_SERRE                  BIT8
#define   B_PCIE_PORT_PCICMD_INTDIS                 BIT10
#define R_PCIE_MISCCTRLSTS                        0x188

//
// Port Config Mode
//
#define REGULAR_PCIE_OWNERSHIP        0
#define PCIE_PORT_REGULAR_MODE        1
#define PCIE_PORT_NTB_MODE            2
#define VMD_OWNERSHIP                 3
#define PCIEAIC_OCL_OWNERSHIP         4

/**
==================================================================================================
==================================  Multi-Cast Definitions      ====================
==================================================================================================
**/
#define MCAST_MEM_AUTO      0x0
#define MCAST_MEM_BELOW_4G  0x1
//#define MCAST_MEM_ABOVE_4G  0x2

#define MC_INDEX_POS_MASK   0x3F

#define MC_ENABLE           BIT15
#define MC_NUM_GROUP_MASK   0x3F

#define MC_RCV_ENABLE_BITS  BIT1

typedef enum {
  MCAST_RET_SUCCESS = 0,
  MCAST_ERR_NOT_SUPPORTED,
  MCAST_ERR_MEM_ALLOC_FAIL,
  MCAST_ERR_INVALID_CONFIG,
} MCAST_RETURN_STATUS;

/**
==================================================================================================
==============================  MS2IOSF Registers  Definitions for 10nm CPUs  ====================
==================================================================================================
**/
#define MS2IOSF_DEV_NUM                   0
#define MS2IOSF_FUNC_NUM                  0
#define MS2IOSF_DFX_DEV_NUM               0
#define MS2IOSF_DFX_FUNC_NUM              3

#define R_BANK_MCTP_BROADCAST_EN          0x1E4

#define R_BANK0_DEVCON                    0x804
#define R_BANK0_PCICMD                    0x808
#define R_BANK0_SECBUS                    0x80D
#define R_BANK0_SUBBUS                    0x80E
#define R_BANK0_MBAS                      0x814
#define R_BANK0_MLIM                      0x816
#define R_BANK0_PBAS                      0x818
#define R_BANK0_PLIM                      0x81A
#define R_BANK0_PBASU                     0x81C
#define R_BANK0_PLIMU                     0x820

/**
==================================================================================================
==================================  DMI Registers  Definitions      ====================
==================================================================================================
**/

#define DMI_PORT_INDEX                0                           // Iio instance wide PortIndex
#define DMI_BUS_NUM                   0
#define DMI_DEV_NUM                   0
#define DMI_FUNC_NUM                  0

#define DMI_DEV_NUM_SPR               0xff
#define DMI_FUNC_NUM_SPR              0xff

#define R_PCIE_PORT_DMIRCBAR                      0x50            // DMI Root Complex Block Base Address
#define   V_DMIRCBA_SIZE                            (4*1024)      // DMI RCBA size is 4KB with Natural Alignment

#define   B_PCIE_PORT_DMIRCBAR_EN_SET              BIT0           // Enable changing for DMI Bar, must unset before LT Lock
#define R_PCIE_PORT_LNKCON2                       0xFC            // PCI Express Link Control 2 Register

#define R_DMI_PORT_PXPLNKCTRL                    0x1B0
#define   B_DMI_PORT_PXPLNKCTRL_EXT_SYNCH          BIT7
#define   B_DMI_PORT_PXPLNKCTRL_CCCON              BIT6
#define   B_DMI_PORT_PXPLNKCTRL_RETRAIN_LINK       BIT5
#define   B_DMI_PORT_PXPLNKCTRL_LNKDIS             BIT4
#define R_DMI_PORT_PXPLNKSTS                     0x1B2
#define   B_DMI_PORT_PXPLNKSTS_LABS                BIT15
#define   B_DMI_PORT_PXPLNKSTS_LBMS                BIT14
#define   B_DMI_PORT_PXPLNKSTS_DLLA                BIT13
#define   B_DMI_PORT_PXPLNKSTS_SCCON               BIT12
#define   B_DMI_PORT_PXPLNKSTS_LINKTRAIN           BIT11
#define   B_DMI_PORT_PXPLNKSTS_NLNKWD              (BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9)
#define   B_DMI_PORT_PXPLNKSTS_LNKSPD              (BIT0 | BIT1 | BIT2 | BIT3)

#define R_DMI_PORT_PXPLNKCTRL2                   0x1C0
#define   B_DMI_PORT_PXPLNKCTRL2_DEEMPH            BIT6
#define   B_DMI_PORT_PXPLNKCTRL2_ENCmpl            BIT4
#define   B_DMI_PORT_PXPLNKCTRL2_LNKSPEED          (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_DMI_PORT_PXPLNKCTRL2_LS_GEN4           0x04
#define     V_DMI_PORT_PXPLNKCTRL2_LS_GEN3           0x03
#define     V_DMI_PORT_PXPLNKCTRL2_LS_GEN2           0x02
#define     V_DMI_PORT_PXPLNKCTRL2_LS_GEN1           0x01

#define R_XPUNCERRSTS                           0x208
#define RECEIVED_PCIE_COMPELTION_WITH_UR_STATUS BIT6

#define R_PCIEDMI_PORT_IPC_DFX_SPARE_REG0       0x614

/**
==================================================================================================
==================================  R-Link Registers  Definitions      ===========================
==================================================================================================
**/
#define   V_RLINKEXPPTMBAR_SIZE                            (32*1024)      // Rlink ExpPtmBar size is 32KB

/**
==================================================================================================
==================================  R-Link LogPhy Registers  Definitions      ====================
==================================================================================================
**/
#define R_SOUTH_BRIDGE_RLINK_MEM_LOGICAL_REMOTE_PHY_BAR   0x5000   ///< LogPHY registers Base Address (remote access from CDF side)
#define PORT_ID_SOUTH_BRIDGE_RLINK                        0x4D     //Port ID RLINK in CDF (South bridge)

#define R_NORTH_BRIDGE_RLINK_MEM_LCTL                     0x00          ///< Link Control
#define B_NORTH_BRIDGE_RLINK_MEM_LCTL_HAIE                BIT3         ///< Hardware-Autonomous IDLE_L1 Enable


#define R_NORTH_BRIDGE_RLINK_MEM_LPMC                     0x10          ///< Link Power Management Control
#define B_NORTH_BRIDGE_RLINK_MEM_LPMC_LCTGE               BIT27         ///< Link Clock Trunk Gate Enable
#define B_NORTH_BRIDGE_RLINK_MEM_LPMC_DSCGE               BIT28         ///< Dynamic Secondary Clock Gate Enable
#define B_NORTH_BRIDGE_RLINK_MEM_LPMC_DLCGE               BIT29         ///< Dynamic Link Clock Gate Enable
#define B_NORTH_BRIDGE_RLINK_MEM_LPMC_VSTE                BIT30         ///< Valid/Strobe Tristate Enable
#define B_NORTH_BRIDGE_RLINK_MEM_LPMC_DSTE                BIT31         ///< Data/StreamID Tristate Enable

#define R_NORTH_BRIDGE_RLINK_MEM_LCFG                     0x18          ///< Link Configuration
#define B_NORTH_BRIDGE_RLINK_MEM_LCFG_SRL                 BIT26         ///< Secure Register Lock
#define B_NORTH_BRIDGE_RLINK_MEM_LCFG_AME                 BIT27         ///< Link Clock Trunk Gate Enable

#define R_NORTH_BRIDGE_RLINK_MEM_IOSFSBCS                 0x50          ///< IOSF Sideband Control And Status
#define B_NORTH_BRIDGE_RLINK_MEM_IOSFSBCS_SCPTCGE         BIT6          ///< Side Clock Partition/Trunk Clock Gating Enable
#define B_NORTH_BRIDGE_RLINK_MEM_IOSFSBCS_SEOSCGE         BIT5          ///< Sideband Endpoint Oscillator/Side Dynamic Clock Gating Enable
#define B_NORTH_BRIDGE_RLINK_MEM_IOSFSBCS_SIID            (BIT3 | BIT2) ///< IOSF Sideband Interface Idle Counter

/**
==================================================================================================
==================================  R-Link Analog Registers  Definitions      ====================
==================================================================================================
**/
#define R_SOUTH_BRIDGE_RLINK_MEM_ANALOG_REMOTE_PHY_BAR      0x6000        ///< Analog (OPIO) PHY registers Base Address (remote access from CDF side)

#define R_NORTH_BRIDGE_RLINK_MEM_CMN                        0x34          ///< CMN Interface Clock Gating
#define B_NORTH_BRIDGE_RLINK_MEM_CMN_IOSFSBCLKGATE          BIT8          ///< IOSF SB clock-gating
#define B_NORTH_BRIDGE_RLINK_MEM_CMN_SECCLKTRUNKCLKGATEEN   BIT12         ///< Sec_clk trunk clock-gating enable.
#define B_NORTH_BRIDGE_RLINK_MEM_CMN_SECCLKDYNCLKGATEEN     BIT13         ///< Sec_clk dynamic clock-gating enable.


/**
==================================================================================================
==================================              VC/TC  Mapping            ========================
==================================================================================================
**/
#define R_IIO_RCRB_DMIPVCCAP1                              0x04          // DMI Port Capability Register 1
#define R_IIO_RCRB_DMIVC0RCAP                              0x10          // DMI VC0 Resource Capability
#define R_IIO_RCRB_DMIVC0RCTL                              0x14          // DMI VC0 Resource Control
#define   B_IIO_RCRB_DMIVCxRCTL_VCx_EN                       BIT31       // Virtual Channel x Enable (VCxE)
#define   B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK                  (7 << 24) // Bit[26:24]
#define   B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK               0xFE
#define R_IIO_RCRB_DMIVC0RSTS                              0x1A          // DMI VC0 Resource Status
#define   B_IIO_RCRB_DMIVCxRSTS_VCx_NP                       BIT1        // Virtual Channel x Negotiation Pending (VCxNP)
#define R_IIO_RCRB_DMIVC1RCAP                              0x1C          // DMI VC1 Resource Capability
#define R_IIO_RCRB_DMIVC1RCTL                              0x20          // DMI VC1 Resource Control
#define R_IIO_RCRB_DMIVC1RSTS                              0x26          // DMI VC1 Resource Status
#define R_IIO_RCRB_DMIVCMRCAP                              0x34          // DMI VCM Resource Capability
#define R_IIO_RCRB_DMIVCMRCTL                              0x38          // DMI VCP Resource Control
#define R_IIO_RCRB_DMIVCMRSTS                              0x3E          // DMI VCM Resource Status
#define R_IIO_RCRB_DMILCTRL                                0x88          // DMI Link control
#define R_IIO_RCRB_DMILSTS                                 0x8A          // DMI Link status
// Rlink specific definitions:
#define R_IIO_RCRB_RLINKVCMRCAP_HEADER_OFFSET              0x280
#define R_IIO_RCRB_RLINKVCMRCAP                            0x28          // RLINK-DMI VCM Resource Capability
#define R_IIO_RCRB_RLINKVCMRCTL                            0x2C          // RLINK-DMI VCM Resource Control
#define R_IIO_RCRB_RLINKVCMRSTS                            0x32          // RLINK-DMI VCM Resource Status
#define UNASSIGNED_VC                                      0xFF

/**
==================================================================================================
==================================  PCIE Registers Definitions     ====================
==================================================================================================
**/

// NOTE: Although not really applicaple for most UP/DP configuration the DMI port could be configured
//       as a PCIE port.  In that configuration.

#define PCI_EXPRESS_BASE_ADDRESS  ((VOID *) (UINTN) 0x80000000)

#define ONLY_REGISTER_OFFSET(x)  (x & 0xFFF)

#define PCI_PCIE_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

#define PCI_INVALID_VALUE        0xFFFFFFFF
#define PCI_INVALID_VALUE_8b     0xFF

// SKX PCIE Port Index per Stack
#define PCIESTACKPORTINDEX(port, stack, portIdx)  PCIESTACKPORTINDEX10NMR(port, stack, portIdx)
#define PORTINDEX_TO_STACK(portIdx)               PORTINDEX_TO_STACK_10NMR(portIdx)
#define PORTINDEX_TO_PORT(portIdx)                PORTINDEX_TO_PORT_10NMR(portIdx)

// ICX-SP / ICX-G / KNH PCIE Port Index per Stack
#define PCIESTACKPORTINDEX10NM(port,stack, portIdx) \
  if (stack == 0) {portIdx = port; \
  } else if (stack == 1) {portIdx = (port + 5); \
  } else if (stack == 2) {portIdx = (port + 13);\
  }
//
// ICX-SP Redefined Convert PortIndex to stack
//
#define PORTINDEX_TO_STACK_10NM(portIdx) \
  (((portIdx) >= 13) ? 2 : ((portIdx) >= 5) ? 1 : 0)
//
// ICX-SP Redefined Convert PortIndex to port in a stack
//
#define PORTINDEX_TO_PORT_10NM(portIdx) \
  ((portIdx) - (((portIdx) >= 13) ? 13 : ((portIdx) >= 5) ? 5 : 0))

// ICX-SP Redefined Port Index per Stack
#define PCIESTACKPORTINDEX10NMR(port, stack, portIdx) \
  if (stack == 0) {portIdx = port; \
  } else if (stack == 1) {portIdx = (port + 1); \
  } else if (stack == 2) {portIdx = (port + 5); \
  } else if (stack == 3) {portIdx = (port + 9); \
  } else if (stack == 4) {portIdx = (port + 13); \
  } else if (stack == 5) {portIdx = (port + 17); \
  }
//
// ICX-SP Redefined Convert PortIndex to stack
//
#define PORTINDEX_TO_STACK_10NMR(portIdx) \
  (((portIdx) >= 17) ? 5 : ((portIdx) >= 13) ? 4 : ((portIdx) >= 9) ? 3 : ((portIdx) >= 5) ? 2 : ((portIdx) >= 1) ? 1 : 0)
//
// ICX-SP Redefined Convert PortIndex to port in a stack
//
#define PORTINDEX_TO_PORT_10NMR(portIdx) \
  ((portIdx) - (((portIdx) >= 17) ? 17 : ((portIdx) >= 13) ? 13 : ((portIdx) >= 9) ? 9 : ((portIdx) >= 5) ? 5 : ((portIdx) >= 1) ? 1 : 0))

//
// SPR Port Index per Stack
//
#define PCIESTACKPORTINDEXSPR(port,stack, portIdx) \
  if (stack == 0) {portIdx = port; \
  } else if (stack == 1) {portIdx = (port + 1); \
  } else if (stack == 2) {portIdx = (port + 9); \
  } else if (stack == 3) {portIdx = (port + 17); \
  } else if (stack == 4) {portIdx = (port + 25); \
  } else if (stack == 5) {portIdx = (port + 33); \
  }
//
// SPR: Convert PortIndex to stack
//
#define PORTINDEX_TO_STACK_SPR(portIdx) \
  (((portIdx) >= 33) ? 5 : ((portIdx) >= 25) ? 4 : ((portIdx) >= 17) ? 3 : ((portIdx) >= 9) ? 2 : ((portIdx) >= 1) ? 1 : 0)
//
// SPR: Convert PortIndex to port in a stack
//
#define PORTINDEX_TO_PORT_SPR(portIdx) \
  ((portIdx) - (((portIdx) >= 33) ? 33 : ((portIdx) >= 25) ? 25 : ((portIdx) >= 17) ? 17 : ((portIdx) >= 9) ? 9 : ((portIdx) >= 1) ? 1 : 0))

// ICX-SP Redefined Cluster Index per Stack
#define PCIECLUSTERINDEX10NMR(stack, clusterIdx) \
  if (stack == 0) { clusterIdx = 0; \
  } else if (stack == 1) { clusterIdx = 1; \
  } else if (stack == 2) { clusterIdx = 5; \
  } else if (stack == 3) { clusterIdx = 9; \
  } else if (stack == 4) { clusterIdx = 13; \
  } else if (stack == 5) { clusterIdx = 17; \
  }

// SNR PCIE Port Index per Stack
#define PCIESTACKPORTINDEXSNR(port, stack, portIdx) \
  if (stack == 0) {portIdx = port; \
  } else if (stack == 1) {portIdx = (port + 1); \
  }
// ICX-DE PCIE Port Index per Stack
#define PCIESTACKPORTINDEXICXD(port, stack, portIdx) \
  if (stack == 0) {portIdx = port; \
  } else if (stack == 1) {portIdx = (port + 1); \
  } else if (stack == 5) {portIdx = (port + 5); \
  }

//
// ICX-DE / SNR Convert PortIndex to stack
//
#define PORTINDEX_TO_STACK_SNR(portIdx)  (((portIdx) >= 1) ? 1 : 0)
#define PORTINDEX_TO_STACK_ICXD(portIdx)  (((portIdx) >= 5) ? 5 : ((portIdx) >= 1) ? 1 : 0)
//
// ICX-DE / SNR Convert PortIndex to port in a stack
//
#define PORTINDEX_TO_PORT_SNR(portIdx)   ((portIdx) - (((portIdx) >= 1) ? 1 : 0))
#define PORTINDEX_TO_PORT_ICXD(portIdx)   ((portIdx) - (((portIdx) >= 5) ? 5 : ((portIdx) >= 1) ? 1 : 0))

// Device ID for the VMD dummy function
#define V_VMD_DUMMY_DID                           0x09AB

#define MAX_PAYLOAD_SIZE_512B                     2
#define MAX_PAYLOAD_SIZE_256B                     1
#define MAX_PAYLOAD_SIZE_128B                     0

#define TEMP_BUSBASE_OFFSET                       1

#define SECBUS_TEMP_DEV_NUM                       0
#define SECBUS_TEMP_FUNC_NUM                      0

#define R_PCIE_PORT_STATUS                        0x06
#define R_PCIE_PORT_CCR_N0                        0x09
#define R_PCIE_PORT_CCR_N1                        0x0A
#define R_PCIE_PORT_BAR_N0                        0x10
#define R_PCIE_PORT_BAR_N1                        0x14
#define R_PCIE_PORT_IOBAS                         0x1C
#define   B_PCIE_PORT_MOREIO                        (BIT2|BIT3)
#define R_PCIE_PORT_CAPPTR                        0x34
#define R_PCIE_PORT_BCTRL                         0x3E
#define   B_PCIE_PORT_BCTRL_SBR                   BIT6
#define R_PCIE_PORT_SCAPID                        0x40
#define R_PCIE_PORT_SNXTPTR                       0x41
#define R_PCIE_PORT_SVID_OTHER                    0x44
#define R_PCIE_PORT_SID_OTHER                     0x46

#define R_PCIE_PORT_MSINXTPTR                     0x61
#define R_PCIE_PORT_MSICTRL                       0x62
#define   B_PCIE_PORT_MSICTRL_MSIEN                 BIT0
#define R_PCIE_PORT_MSIAR                         0x64
#define R_PCIE_PORT_MSGDAT                        0x68
#define R_PCIE_PORT_MSIMSK                        0x6C
#define R_PCIE_PORT_MSIPENDING                    0x70

#define R_PCIE_PORT_PXPNXTPTR                     0x91
#define R_PCIE_PORT_PXPCAP                        0x92
#define   B_PCIE_PORT_PXPCAP_SLOTIMPL               BIT8
#define   B_PCIE_PORT_PXPCAP_DPT                    (BIT4|BIT5|BIT6|BIT7)
#define R_PCIE_PORT_PXPDEVCAP                     0x94
#define R_PCIE_PORT_PXPDEVCTRL                    0x98
#define   B_PCIE_PORT_PXPDEVCTRL_ENNOSNP            BIT11
#define   B_PCIE_PORT_PXPDEVCTRL_MPS                (BIT5|BIT6|BIT7)
#define     V_PCIE_PORT_PXPDEVCTRL_MPS_128            (0<<5)
#define     V_PCIE_PORT_PXPDEVCTRL_MPS_256            (1<<5)
#define   B_PCIE_PORT_PXPDEVCTRL_CERE               BIT0
#define   B_PCIE_PORT_PXPDEVCTRL_NERE               BIT1
#define   B_PCIE_PORT_PXPDEVCTRL_FERE               BIT2
#define   B_PCIE_PORT_PXPDEVCTRL_URRE               BIT3
#define R_PCIE_PORT_PXPDEVSTS                     0x9A
#define   B_PCIE_PORT_PXPDEVCTRL_CERS               BIT0
#define   B_PCIE_PORT_PXPDEVCTRL_NERS               BIT1
#define   B_PCIE_PORT_PXPDEVCTRL_FERS               BIT2
#define   B_PCIE_PORT_PXPDEVSTS_UNSUPREQT           BIT3
#define R_PCIE_PORT_PXPLNKCAP                     0x9C
#define   B_PCIE_PORT_PXPLNKCAP_PORTNUM             (BIT31 | BIT30 | BIT29 | BIT28 |BIT27 | BIT26 | BIT25 | BIT24)
#define   B_PCIE_PORT_PXPLNKCAP_L1EXITL             (BIT17 | BIT16 | BIT15)
#define     V_PCIE_PORT_PXPLNKCAP_L1EXITL_SHIFT       15
#define   B_PCIE_PORT_PXPLNKCAP_L0SEXITL            (BIT14 | BIT13 | BIT12)
#define     V_PCIE_PORT_PXPLNKCAP_L0SEXITL_SHIFT      12
#define   B_PCIE_PORT_PXPLNKCAP_ASPMCTRL            (BIT10 | BIT11)
#define     B_PCIE_PORT_PXPLNKCAP_ASPMCTRL_L0S        (BIT0<<10)
#define     B_PCIE_PORT_PXPLNKCAP_ASPMCTRL_L1         (BIT1<<10)
#define   B_PCIE_PORT_PXPLNKCAP_MAXLNKWH            (BIT9 | BIT8 | BIT7 | BIT6 | BIT5  | BIT4)
#define   B_PCIE_PORT_PXPLNKCAP_LNKSPEED            (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_PCIE_PORT_PXPLNKCAP_LS_GEN4             0x04
#define     V_PCIE_PORT_PXPLNKCAP_LS_GEN3             0x03
#define     V_PCIE_PORT_PXPLNKCAP_LS_GEN2             0x02
#define     V_PCIE_PORT_PXPLNKCAP_LS_GEN1             0x01
#define R_PCIE_PORT_PXPLNKCTRL                    0xA0
#define   B_PCIE_PORT_PXPLNKCTRL_EXT_SYNCH          BIT7
#define   B_PCIE_PORT_PXPLNKCTRL_CCCON              BIT6
#define   B_PCIE_PORT_PXPLNKCTRL_RETRAIN_LINK       BIT5
#define   B_PCIE_PORT_PXPLNKCTRL_LNKDIS             BIT4
#define R_PCIE_PORT_PXPLNKSTS                     0xA2
#define   B_PCIE_PORT_PXPLNKSTS_LABS                BIT15
#define   B_PCIE_PORT_PXPLNKSTS_LBMS                BIT14
#define   B_PCIE_PORT_PXPLNKSTS_DLLA                BIT13
#define   B_PCIE_PORT_PXPLNKSTS_SCCON               BIT12
#define   B_PCIE_PORT_PXPLNKSTS_NLNKWD              (BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9)
#define   B_PCIE_PORT_PXPLNKSTS_LNKSPD              (BIT0 | BIT1 | BIT2 | BIT3)


#define R_PCIE_PORT_PXPSLOTCAP                    0xA4
#define   B_PCIE_PORT_PXPSLOTCAP_PSN                (BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define   B_PCIE_PORT_PXPSLOTCAP_EIP                BIT17
#define   B_PCIE_PORT_PXPSLOTCAP_SPLS               (BIT16 | BIT15)
#define   B_PCIE_PORT_PXPSLOTCAP_SPLV               (BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 | BIT7)
#define   B_PCIE_PORT_PXPSLOTCAP_HPC                BIT6
#define   B_PCIE_PORT_PXPSLOTCAP_HPS                BIT5
#define   B_PCIE_PORT_PXPSLOTCAP_PIP                BIT4
#define   B_PCIE_PORT_PXPSLOTCAP_AIP                BIT3
#define   B_PCIE_PORT_PXPSLOTCAP_MRLSP              BIT2
#define   B_PCIE_PORT_PXPSLOTCAP_PCP                BIT1
#define   B_PCIE_PORT_PXPSLOTCAP_ABP                BIT0

#define R_PCIE_PORT_PXPSLOTCTRL                   0xA8
#define   B_PCIE_PORT_PXPSLOTCTRL_PWRLED            (BIT8 | BIT9)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRLED_OFF        (3<<8)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRLED_BLINK      (2<<8)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRLED_ON         (1<<8)
#define   B_PCIE_PORT_PXPSLOTCTRL_PWRCTRL           BIT10
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRCTRL_OFF       (1<<10)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRCTRL_ON        (0<<10)
#define   B_PCIE_PORT_PXPSLOTCTRL_ATNLED            (BIT6 | BIT7)
#define     V_PCIE_PORT_PXPSLOTCTRL_ATNLED_OFF        (3<<6)
#define     V_PCIE_PORT_PXPSLOTCTRL_ATNLED_BLINK      (2<<6)
#define     V_PCIE_PORT_PXPSLOTCTRL_ATNLED_ON         (1<<6)
#define   B_PCIE_PORT_PXPSLOTCTRL_HPINTEN           BIT5
#define   B_PCIE_PORT_PXPSLOTCTRL_CCIEN             BIT4
#define   B_PCIE_PORT_PXPSLOTCTRL_PRSINTEN          BIT3
#define   B_PCIE_PORT_PXPSLOTCTRL_MRLINTEN          BIT2
#define   B_PCIE_PORT_PXPSLOTCTRL_PWRINTEN          BIT1
#define   B_PCIE_PORT_PXPSLOTCTRL_ATNINTEN          BIT0

#define R_PCIE_PORT_PXPSLOTSTS                    0xAA
#define   B_PCIE_PORT_PXPSLOTSTS_DLLSCS             BIT8
#define   B_PCIE_PORT_PXPSLOTSTS_EMLS               BIT7
#define   B_PCIE_PORT_PXPSLOTSTS_PDS                BIT6
#define   B_PCIE_PORT_PXPSLOTSTS_MRLSS              BIT5
#define   B_PCIE_PORT_PXPSLOTSTS_CMDCOMP            BIT4
#define   B_PCIE_PORT_PXPSLOTSTS_PRSDC              BIT3
#define   B_PCIE_PORT_PXPSLOTSTS_MRLSC              BIT2
#define   B_PCIE_PORT_PXPSLOTSTS_PWRFD              BIT1
#define   B_PCIE_PORT_PXPSLOTSTS_ABP                BIT0

#define R_PCIE_PORT_ROOTCON                       0xAC
#define   B_PCIE_PORT_ROOTCON_CRSSWVISEN            BIT4
#define   B_PCIE_PORT_ROOTCON_SECE                  BIT0
#define   B_PCIE_PORT_ROOTCON_SENE                  BIT1
#define   B_PCIE_PORT_ROOTCON_SEFE                  BIT2

#define R_PCIE_PORT_PXPDEVCAP2                    0xB4
#define   B_PCIE_PORT_ARI_EN                        BIT5


#define R_PCIE_PORT_PXPDEVCTRL2                   0xB8
#define   B_PCIE_PORT_PXPDEVCTRL2_ARI               BIT5
#define   B_PCIE_PORT_PXPDEVCTRL2_CTPE              BIT4
#define     V_PCIE_PORT_PXPDEVCTRL2_CTPE_EN           (1<<4)
#define   B_PCIE_PORT_PXPDEVCTRL2_CTV               (BIT0 | BIT1 | BIT2 | BIT3)

#define R_PCIE_PORT_PXPLINKCAP2                   0xBC
#define   B_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC         (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7)
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_3G    BIT3
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_2G    BIT2
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_1G    BIT1
// LINK supports G4,G3, G2 and G1
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_4G_TOP    ( BIT4 | BIT3 | BIT2 | BIT1)
// LINK supports G3, G2 and G1
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_3G_TOP    ( BIT3 | BIT2 | BIT1)
// LINK supports G2 and G1
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_2G_TOP    ( BIT2 | BIT1 )
// LINK only supports G1
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_1G_TOP    BIT1

#define R_PCIE_PORT_PXPLNKCTRL2                   0xC0
#define   B_PCIE_PORT_PXPLNKCTRL2_DEEMPH            BIT6
#define   B_PCIE_PORT_PXPLNKCTRL2_ENCmpl            BIT4
#define   B_PCIE_PORT_PXPLNKCTRL2_LNKSPEED          (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_PCIE_PORT_PXPLNKCTRL2_LS_GEN4           0x04
#define     V_PCIE_PORT_PXPLNKCTRL2_LS_GEN3           0x03
#define     V_PCIE_PORT_PXPLNKCTRL2_LS_GEN2           0x02
#define     V_PCIE_PORT_PXPLNKCTRL2_LS_GEN1           0x01

#define R_PCIE_PORT_PXPPMCAP                      0xE0
#define R_PCIE_PORT_PXPPMCSR                      0xE4
#define   B_PCIE_PORT_PXPPMCSR_PWR_STATE              (BIT1 | BIT0)
#define     V_PCIE_PORT_PXPPMCSR_PWR_STATE_D0         0x00
#define     V_PCIE_PORT_PXPPMCSR_PWR_STATE_D3HOT      0x03

#define R_ACSCTRL                                 0x116
#define R_PCIE_PORT_ACSCTRL                       R_ACSCTRL
#define   B_PCIE_PORT_ACSCTRL_U                     BIT4
#define   B_PCIE_PORT_ACSCTRL_C                     BIT3
#define   B_PCIE_PORT_ACSCTRL_R                     BIT2
#define   B_PCIE_PORT_ACSCTRL_B                     BIT1
#define   B_PCIE_PORT_ACSCTRL_V                     BIT0

#define R_PCIE_PORT_APICBASE                      0x140
#define   B_PCIE_PORT_APICBASE_ENABLE               BIT0
#define R_PCIE_PORT_APICLIMIT                     0x142
#define R_PCIE_PORT_UNCERRSTS                     0x14C
#define   B_PCIE_PORT_UNSUPPORTED_REQUEST           BIT20

#define R_PCIE_PORT_CORERRMSK                     0x15C
#define   B_PCIE_CORERRMSK_ANFE_MASK               BIT13

#define R_PCIE_PORT_RPERRCMD                      0x174
#define   B_PCIE_CORRECTABLE_ERR_RPT_EN           BIT0
#define   B_PCIE_NONFATAL_ERR_RPT_EN              BIT1
#define   B_PCIE_FATAL_ERR_RPT_EN                 BIT2

#define R_PCIE_PORT_PCIEIPERFCTRLSTS              0x180
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_TPHDIS                             BIT41
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_DCA_ReqID_Override                 BIT40
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_max_read_completion_combine_size   BIT35
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_outstanding_requests_gen1          BIT16   //BIT 20:16 Outstanding Requests for Gen1
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_outstanding_requests_gen2          BIT8    //BIT 13:8 Outstanding Requests for Gen2
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_UseAllocatingFlowWr                BIT7
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_vcp_roen_nswr                      BIT6
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_vcp_nsen_rd                        BIT5
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_SIS          BIT4
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_NSOW         BIT3
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_NSOR         BIT2
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_SSP          BIT1
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_SSCTL        BIT0

#define R_PCIE_PORT_PCIEMISCCTRLSTS               0x188
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Locked_Rd_TO        BIT49
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Rcv_PME_TO_ACK      BIT48
#define   B_PROBLEMATIC_PORT_FOR_LOCK_FLOW                BIT38
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_FormFact            BIT36
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_MSIFATEN            BIT35
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_MSINFATEN           BIT34
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_MSICOREN            BIT33
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_ACPI_PMEEN          BIT32
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx           BIT31
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_InboundIOEN         BIT30
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_TO_DIS              BIT28
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_SIOLnkBWSts         BIT27
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI             BIT26
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PWrtDis   BIT25
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PRdDis    BIT24
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_PHold           BIT23
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_PME_Ack_TOCtl       (BIT8 | BIT7)
#define   V_PCIE_PORT_PCIEMISCCTRLSTS_PME_AckTOCtl        0x7
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_PME_TO_ACK_TIMEOUT  BIT6
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Send_PME_OFF        BIT5
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_SysERR_only4_AER    BIT4
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_HotPlug     BIT3
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_PM          BIT2
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Inbound_CFGEN       BIT1
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Set_HostBrdg_CC     BIT0

#define R_PCIE_PORT_PCIEIOUxBIFCTRL               0x190     // registe is only applicapble in device 1, 3, & 7
#define   B_PCIE_PORT_PCIEIOUxBIFCTRL_SBIF          BIT3    // This is the bit to initiate the bifurcuatin on the port
#define   B_PCIE_PORT_PCIEIOUxBIFCTRL_LNKCFG       (BIT0 + BIT1 + BIT2)
#define R_PCIE_PORT_XPCORERRSTS                   0x200
#define   B_PCIE_PORT_XPCORERRSTS_LBS               BIT1
#define R_PCIE_PORT_XPCORERRMSK                   0x204
#define   B_PCIE_PORT_XPCORERRMSK_LBM               BIT1
#define R_PCIE_PORT_XPUNCERRSTS                   0x208
#define   V_PCIE_PORT_XPUNCERRSTS                   (BIT1 + BIT3 + BIT4 + BIT5 + BIT6 + BIT8 + BIT9)
#define R_PCIE_PORT_XPUNCERRMSK                   0x20c
#define   V_PCIE_PORT_XPUNCERRMSK                   (BIT1 + BIT3 + BIT4 + BIT5 + BIT6 + BIT8 + BIT9)
#define R_PCIE_PORT_XPUNCERRSEV                   0x210
#define   V_PCIE_PORT_XPUNCERRSEV                   (BIT1 + BIT3 + BIT4 + BIT5 + BIT6 + BIT8 + BIT9)
#define R_PCIE_PORT_XPUNCERRPTR                   0x214
#define R_PCIE_PORT_UNCEDMASK                     0x218
#define R_PCIE_PORT_COREDMASK                     0x21c
#define   V_PCIE_PORT_COREDMASK                     (BIT1 + BIT6 + BIT7 + BIT8 + BIT12 + BIT13)
#define R_PCIE_PORT_RPEDMASK                      0x220
#define R_PCIE_PORT_XPUNCEDMASK                   0x224
#define R_PCIE_PORT_XPCOREDMASK                   0x228
#define R_PCIE_PORT_XPGLBERRSTS                   0x230
#define   B_PCIE_PORT_XPGLBERRSTS_FAER              BIT0
#define   B_PCIE_PORT_XPGLBERRSTS_NAER              BIT1
#define   B_PCIE_PORT_XPGLBERRSTS_CAER              BIT2
#define R_PCIE_PORT_XPGLBERRPTR                   0x232

#define R_PCIE_PORT_LN0LN3EQ                      0x25C

/**
==================================================================================================
================================== NTB Definitions  =========================================
==================================================================================================
**/

#define R_NTB_PPD                               0xD4
#define   B_NTB_PORT_DEF                          (BIT0 | BIT1)

#define R_NTB_MBAR_CFG_BASE                     0xB000
#define R_NTB_PORT_PXPLNKSTS                    0x1A2
#define   B_NTB_PORT_PXPLNKSTS_DLLA               BIT13
#define   B_NTB_PORT_PXPLNKSTS_SCCON              BIT12
#define   B_NTB_PORT_PXPLNKSTS_NLNKWD             (BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9)
#define   B_NTB_PORT_PXPLNKSTS_LNKSPD             (BIT0 | BIT1 | BIT2 | BIT3)

#define IIO_NTB_DEVICE_NUM                      0x1
#define IIO_NTB_FUNC_NUM                        0x0

#define NTB_IMBAR0_POW2_ALIGNMENT               16
#define NTB_IMBAR0_SIZE                         0x10000

#define R_EMBAR1_EMBAR2_OFF            0x500

#define R_EMBAR0                     0x510
#define   B_EMBAR0_PREFETCHABLE      BIT3
#define R_EMBAR1                     0x518
#define R_EMBAR2                     0x520

#define R_EMBAR1XBASE                0x4518
#define R_EMBAR2XBASE                0x4520

#define R_EMBAR1XBASE_IMBARPCIPCMD   0x504
#define R_EMBAR2XBASE_EMBARPCIPCMD   0x4504

#define B_IMBAR1BASET_Type_Mem64     2 << 1
#define IMBAR1XBASE_MBA_DEFAULT      0x4000000000
#define IMBAR2XBASE_MBA_DEFAULT      0x8000000000

// PCIE GEN4 NTB registers offset in MBAR
#define NTB_MBAR0_PCI_CFG_OFFSET     0xB040  // Cap offset
#define NTB_MBAR01_PPD0_OFFSET       0xB0D4  // PPD0 offset

#define NTB_MBAR01_LINKCAP_OFFSET    0xB04C  // LINKCAP_IIO_PCIEGen4_REG
#define NTB_MBAR01_LINKCAP2_OFFSET   0xB06C  // LINKCAP2_IIO_PCIEGen4_REG
#define NTB_MBAR01_SLOTCAP_OFFSET    0xB054  // SLOTCAP_IIO_PCIEGen4_REG
#define NTB_MBAR01_SLOTCTL_OFFSET    0xB058  // SLOTCTL_IIO_PCIEGen4_REG
#define NTB_MBAR01_LINKCTL2_OFFSET   0xB070  // LINKCTL2_IIO_PCIEGen4_REG
#define NTB_MBAR01_LTSSMTRNCTL_OFFSET   0xec28  // LINKCTL2_IIO_PCIEGen4_REG

#define NTB_IEP_LTSSMTRNCTL_OFFSET   0xF030  // LTSSMTRNCTL in NTB iEP offset
#define NTB_12K_OFFSET               0xC000  // NTB IEP OFFSET

/**
==================================================================================================
================================== IOAT Definitions     =========================================
==================================================================================================
**/
#define IOAT_DEVICE_NUM                           0x04
#define IOAT_DEVICE_NUM_10NM                      0x01
#define IOAT_FUNC_START                           0x00
#define IOAT_TOTAL_FUNCS                          0x08

#define R_IIO_CB_BAR_N0                           0x10
#define R_IIO_CB_BAR_N1                           0x14
#define R_IIO_CB_DEVCON                           0x98
#define   B_IIO_CB_DEVCON_RELAX_OR_EN             BIT4

#define R_CB_BAR_CSI_DMACAPABILITY                0x10
#define   B_CB_BAR_CSI_DMACAP_DCA_SUP             BIT4
#define R_CB_BAR_CSI_CAP_ENABLE                   0x10C
#define   B_CB_BAR_CSI_CAP_ENABLE_PREFETCH_HINT   BIT0
#define R_CB_BAR_PCIE_CAP_ENABLE                  0x10E
#define   B_CB_BAR_PCIE_CAP_ENABLE_MEM_WR         BIT0
#define R_CB_BAR_APICID_TAG_MAP                   0x110
#define R_CB_BAR_CBVER                            0x08
#define   V_CB32_VERSION                          0x32
#define   V_CB33_VERSION                          0x33
#define   V_CB34_VERSION                          0x34


#define IIO_CBDMA_MMIO_SIZE 0x10000 //64kB for one CBDMA function
#define IIO_CBDMA_MMIO_ALLIGNMENT 14 //2^14 - 16kB

#define V_NAC_IQAT_VRP_DEVICE                     5
#define V_NAC_IQAT_VRP_FUNCTION                   0

#define V_NAC_IQAT_DEVICE_ID                      0x18A0

#define V_NAC_IQAT_DEVICE                         0
#define V_NAC_IQAT_FUNCTION                       0

#define NAC_TOP_CR_PMISCLBAR_ADDR                 0x18
#define   B_NAC_TOP_CR_PMISCLBAR_ADDR_SHIFT       13
#define NAC_TOP_CR_PMISCUBAR_ADDR                 0x1C

#define R_NAC_TOP_CR_RAMBASEADDRLO                0x7101c
#define   B_NAC_TOP_CR_RAMBASEADDRLO_ADDR_MASK    0xFFFFFFFF
#define   B_NAC_TOP_CR_RAMBASEADDRLO_ADDR_SHIFT   32

#define R_NAC_TOP_CR_RAMBASEADDRHI                0x71020
#define   B_NAC_TOP_CR_RAMBASEADDRHI_ADDR_MASK    0xFFFF

#define V_PSF1_BUS                                0

#define V_CDF_IQAT_VRP_DEVICE                     6
#define V_CDF_IQAT_VRP_FUNCTION                   0

#define V_CDF_IQAT_DEVICE_ID                      0x18EE

#define V_CDF_IQAT_DEVICE                         0
#define V_CDF_IQAT_FUNCTION                       0

#define R_CDF_IQAT_CR_PMISCLBAR_ADDR              0x18
#define   B_CDF_IQAT_TOP_CR_PMISCLBAR_ADDR_SHIFT  18
#define R_CDF_IQAT_CR_PMISCUBAR_ADDR              0x1C

#define R_CDF_IQAT_CR_RAMBASEADDRLO               0x308D8
#define   B_CDF_IQAT_CR_RAMBASEADDRLO_ADDR_MASK   0xFFFFFFFF
#define   B_CDF_IQAT_CR_RAMBASEADDRLO_ADDR_SHIFT  32

#define R_CDF_IQAT_CR_RAMBASEADDRHI               0x308D4
#define   B_CDF_IQAT_CR_RAMBASEADDRHI_ADDR_MASK   0xFFFF

#define V_IOV_RESERVED_BUS_NUMBER                 3 // Reserved bus's  for TIP and HQM

/**
==================================================================================================
================================== VTd Definitions      =========================================
==================================================================================================
**/
//
// Dev 5, Func 0 - Intel VT, Address Map, System Management, Miscellaneous Registers
//
#define CORE05_DEV_NUM                            5
#define CORE05_FUNC_NUM                           0

#define R_IIO_CIPCTRL                          0x140             // Coherent Interface Protocol Control
#define   B_IIO_CIP_FLU_WRITE                    BIT31           // Flush Currently Pending Writes to dram from Write Cache
#define   B_IIO_CIP_WRI_UPDATE_DIS               BIT28           // Disable WriteUpdate Flow
#define   B_IIO_CIP_EXRTID_MODE                  BIT2            // Extended RTID Mode Enable
#define   B_IIO_CIP_WRI_COMBIN_DIS               BIT1            // Disable write combining
#define   V_IIO_CIP_PCIRDCURRENT                 0               // PCIRdCurrent/DRd.UC mode select
#define   V_IIO_CIP_DRDUC                        1

#define R_IIO_CIPSTS                           0x144             // Coherent Interface Protocol Status
#define R_IIO_CIPDCASAD                        0x148             // Coherent Interface Protocol DCA Source Address
#define   B_IIO_CIP_TPH_DCA_EN                   BIT0            // Enable TPH/DCA
#define R_IIO_CIPINTRC                         0x14C             // Coherent Interface Protocol Interrupt Control
#define   B_IIO_CIP_SMI_MASK                     BIT10           // SMI Mask
#define R_IIO_CIPINTRS                         0x150             // Coherent interface Protocol Interrupt Status


#define R_IIO_VTBAR                            0x180             // Base Address Register for Intel VT-d Registers
#define   B_IIO_VT_BAR_EN                        BIT0            // Intel VT-d Base Address Enable
#define   V_VT_D_IIO_BAR_SIZE                    (8*1024)        // VT-D IIO BAR is 8KB with Natural Alignment
#define R_IIO_VTGENCTRL                        0x184             // Intel VT-d General Control
#define   V_IIO_VTCTRL_LOCK_VTD_SET            BIT15             // VT-D Bar lock bit
#define   V_IIO_VTCTRL_LIMITS_CLEAR              0x0FF00         // Clear bits [7:0]
#define   V_IIO_VTCTRL_VTIGPAL_47BIT             0x07
#define   V_IIO_VTCTRL_VTHPAL_38BIT              0x03
#define   V_IIO_VTCTRL_VTHPAL_39BIT              0x04
#define   V_IIO_VTCTRL_VTHPAL_46BIT              0x0A
#define   V_IIO_VTCTRL_VTHPAL_48BIT              0x0C
#define   V_IIO_VTCTRL_VTHPAL_51BIT              0x0F
#define   V_IIO_VTCTRL_VTNGPAL_48BIT             0x08
#define R_IIO_VTISOCHCTRL                      0x188            // Intel VT-d Isoch Related Control
#define   B_IIO_VTISOCHCTRL_AZALIA_ON_VCP        BIT8           // Azalia on VCP bit
#define R_IIO_VTGENCTRL2                       0x18C            // Intel VT-d General Control 2
#define   B_IIO_VTGENCTRL2_PrefCtrl              (BIT6 | BIT5)  // Prefetch Control
#define R_IIO_IOTLBPARTITION                   0x194            // IOTLB Partitioning Control
#define   B_IIO_IOT_PARTITION_EN                 BIT0           // IOTLB Partitioning Enable
#define R_IIO_VTUNCERRSTS                      0x1A8            // Uncorrectable Error Status
#define R_IIO_VTUNCERRMSK                      0x1AC            // Intel VT Uncorrectable Error Mask
#define   B_IIO_VTUNCERRMSK_MASK                 BIT31          // Mask reporting Intel VT-d defined errors to IIO core logic
#define   B_MISCERRM                              BIT7
#define   B_VTDERR_MASK                           BIT31
#define R_IIO_VTUNCERRSEV                      0x1B0            // Intel VT Uncorrectable Error Severity
#define R_IIO_VTUNCERRPTR                      0x1B4            // Intel VT Uncorrectable Error Pointer

#define R_VTD_VERSION                             0x00
#define R_VTD_CAP                                 0x08
#define R_VTD_CAP_LOW                             0x08
#define R_VTD_CAP_HIGH                            0x0C
#define R_VTD_EXT_CAP_LOW                         0x10
#define R_VTD_EXT_CAP_HIGH                        0x14

#define V_VTD_REMAP_ENGINE_SIZE                   0x1000
#define VT_D_CHIPSET_BASE_ADDRESS                 0xFE710000

//
// Specific SKX definitions for VTd (Next set of registers are gone for 10nm's CPUs)
//
#define VTD1_BASE_OFFSET                          0x1000

#define R_VTD1_VERSION                            VTD1_BASE_OFFSET  + 0x00
#define R_VTD1_CAP                                VTD1_BASE_OFFSET + 0x08
#define R_VTD1_CAP_LOW                            VTD1_BASE_OFFSET + 0x08
#define R_VTD1_CAP_HIGH                           VTD1_BASE_OFFSET + 0x0C
#define R_VTD1_EXT_CAP_LOW                        VTD1_BASE_OFFSET + 0x10
#define R_VTD1_EXT_CAP_HIGH                       VTD1_BASE_OFFSET + 0x14
#define R_VTD0_INTR_REMAP_TABLE_BASE              0xB8          // VTD0 Interrupt Remapping Table Base Address
#define R_VTD1_INTR_REMAP_TABLE_BASE              0x10B8        // VTD1 Interrupt Remapping Table Base Address
#define   B_IA32_EXT_INT_ENABLE                    BIT11        // IA32_Extended_Interrupt_Enable

/**
==================================================================================================
================================== SLT2HIFI Definitions for KNH  =================================
==================================================================================================
**/
#define SLT2HFI_DEV                               1
#define SLT2HFI_FUNC                               0
#define SLT2HFI_VTD_BAR0                          0xF0
#define SLT2HFI_VTD_BAR1                          0xF4
#define SLT2HFI_VT_D_IIO_BAR_SIZE                 (4*1024)

/**
==================================================================================================
================================== PMON Definitions for SKX ====================================
==================================================================================================
**/
#define PMON_DEV_NUM                              5
#define PMON_FUNC_NUM                             6
/**
==================================================================================================
================================== IOAPIC Definitions for SKX ====================================
==================================================================================================
**/
#define CORE05_FUNC4_NUM                          4
#define CORE05_MAX_FUNC                           4

#define APIC_DEV_NUM                              CORE05_DEV_NUM
#define APIC_FUNC_NUM                             CORE05_FUNC4_NUM

#define R_APIC_MBAR                                    0x10          // IOxAPIC Base Address
#define R_APIC_ABAR                                    0x40          // I/OxAPIC Alternate BAR
#define   B_APIC_ABAR_ABAR_EN                            BIT15       // ABAR Enable
#define R_APIC_PMCAP                                   0xE0          // Power Management Capabilities
#define R_APIC_PMCSR                                   0xE4          // Power Management Control and Status
#define R_APIC_RDINDEX                                 0x80          // Alternate Index to read Indirect I/OxAPIC Registers
#define R_APIC_RDWINDOW                                0x90          // Alternate Window to read Indirect I/OxAPIC Registers
#define R_APIC_IOAPICTETPC                             0xA0          // IOxAPIC Table Entry Target Programmable Control
#define R_APIC_IOADSELS0                               0x288         // IOxAPIC DSELS Register 0
#define R_APIC_IOADSELS1                               0x28C         // IOxAPIC DSELS Register 1
#define R_APIC_IOINTSRC0                               0x2A0         // IO Interrupt Source Register 0
#define R_APIC_IOINTSRC1                               0x2A4         // IO Interrupt Source Register 1
#define R_APIC_IOREMINTCNT                             0x2A8         // Remote IO Interrupt Count
#define R_APIC_IOREMGPECNT                             0x2AC         // Remote IO GPE Count
#define R_APIC_IOXAPICPARERRINJCTL                     0x2C0         // IOxAPIC Parity Error Injection Control
#define R_APIC_FAUXGV                                  0x2C4         // FauxGV
#define   B_APIC_FAUX_GV_EN                              BIT0        // Faux GV Enable

/**
==================================================================================================
================================== NPK Definitions =========================================
==================================================================================================
**/
#define IIO_TRACE_HUB_PCI_CMD_REG                             0x4
#define IIO_TRACE_HUB_CSR_MTB_LBAR_REG                        0x10
#define IIO_TRACE_HUB_CSR_MTB_UBAR_REG                        0x14
#define IIO_TRACE_HUB_FW_LBAR_REG                             0x70
#define IIO_TRACE_HUB_FW_UBAR_REG                             0x74

#define MEMORY_SPACE_ENABLE_BIT                               BIT1
#define PMTCS_REG_OFFSET                                      0xFFD00
#define BLOCK_DRAIN_ENABLE_BIT                                BIT0

#ifdef SPR_HOST
#define IIO_TRACE_HUB_PCI_DEVICE_NUMBER                       4
#define IIO_TRACE_HUB_PCI_FUNCTION_NUMBER                     0
#define NUMBER_TRACE_HUB_PER_SOCKET                           4
#define FIRST_TRACE_HUB_STACK_NUMBER                          8
#else
#define IIO_TRACE_HUB_PCI_DEVICE_NUMBER                       2
#define IIO_TRACE_HUB_PCI_FUNCTION_NUMBER                     4
#define NUMBER_TRACE_HUB_PER_SOCKET                           1
#define FIRST_TRACE_HUB_STACK_NUMBER                          0
#endif

/**
==================================================================================================
================================== VMD Definitions =========================================
==================================================================================================
**/
#define IIO_VMD_DEV_14NM              5 // VMD function no for 14nm processors
#define IIO_VMD_DEV_10NM              0 // VMD function no for 10nm processors
#define IIO_VMD_FUNC                  5 // VMD function no common for 14 and 10nm

#define V_MASS_STORAGE_CCR_RAID_CTRL  0x010400

/**
==================================================================================================
================================== Devide Hide Definitions =======================================
==================================================================================================
**/
#if defined(ICX_HOST)
#define UBOX_MISC_DEV_NUM                           0x0
#define UBOX_MISC_FUNC_NUM                          0x0
#else
#define UBOX_MISC_DEV_NUM                           0x8
#define UBOX_MISC_FUNC_NUM                          0x0
#endif

#define PCU22_DEV_NUM                               30
#define PCU22_FUNC_NUM                              06


//
// 8 stacks per each socket:
//   - 6 IIO stacks (used only on 14nm systems - 10nm doesn't hide per-IP)
//   - 2 uncore stacks (used only for 10nm systems - 14nm doesn't have such stacks)
//
#define NUM_DEVHIDE_REGS_PER_STACK                  8  // devHide 32-bit register for each function on stack
#define NUM_DEVHIDE_STACKS                          8  // number of ALL stacks in setup structure
#define NUM_DEVHIDE_UNCORE_STACKS                   2  // number of uncore stacks in setup structure
#define NUM_DEVHIDE_IIO_STACKS                      6  // number of IIO stacks ins etup structure
#define MAX_DEVHIDE_REGS_PER_SOCKET                 (NUM_DEVHIDE_STACKS * NUM_DEVHIDE_REGS_PER_STACK)

#if MaxIIO > 4
#define MAX_DEVHIDE_REGS_PER_SYSTEM                  512 // MAX_DEVHIDE_REGS_PER_SOCKET * MaxIIO
#else
#define MAX_DEVHIDE_REGS_PER_SYSTEM                  256 // MAX_DEVHIDE_REGS_PER_SOCKET * MaxIIO
#endif


/**
==================================================================================================
================================== LT Definitions             ====================
==================================================================================================
**/

#ifdef LT_FLAG
#define LT_DPR_SIZE                               0x00300000
#define LT_DPR_SIZE_REG                           0x00030
#define LT_DEV_MEM_SIZE                           0x00200000
#define LT_DEV_MEM_ALIGNMENT                      0x00100000
#define SINIT_MAX_SIZE                            0x00050000 // 256KB + 64KB and up to 512KB per CBnT SAS and BWG
#define LT_HEAP_SIZE                              0x00100000

#define LT_PUB_BASE                               0xFED30000
#define LT_SPAD_HIGH                              0xFED300A4
#define LT_SPAD_EXIST                             0xFED30010
#define LPB_DPR_BAR                               0x0330

#define PLATFORM_TXT_FIT_FAILSAFE_TYPE            9
#define PLATFORM_TXT_FIT_BIOSACM_TYPE             2
#endif



/**
==================================================================================================
================================== PCH Definitions             ===================================
==================================================================================================
**/

//
// ME-HECI defines
//
#define HECI1_DEV_NUM                             0x12
#define HECI1_FUNC_NUM                            0x00
#define HECI2_DEV_NUM                             0x12
#define HECI2_FUNC_NUM                            0x01

#define V_HECI_VID                                V_INTEL_VID
#define V_HECI1_DID_CLIENT                        0x3414    // HECI-1 Client SKU
#define V_HECI2_DID_CLIENT                        0x3415    // HECI-2 Client SKU
#define V_HECI1_DID_SERVER                        0x3434    // HECI-1 Server SKU
#define V_HECI2_DID_SERVER                        0x3435    // HECI-2 Server SKU

#define R_HECIMBAR                                0x10
#define R_FWSTATE                                 0x40
#define   B_CUR_STATE                               (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_CUR_STATE_MEFW_HARDM_DISABLED           0x00
#define     V_CUR_STATE_MEFW_INITIALIZING             0x01
#define     V_CUR_STATE_MEFW_RECOVERY                 0x02
#define     V_CUR_STATE_MEFW_NPTM_ACTIVE              0x03
#define     V_CUR_STATE_MEFW_NPTM_INACTIVE            0x04
#define     V_CUR_STATE_MEFW_SOFTM_DISABLED           0x08
#define     V_CUR_STATE_MEFW_VALIDATION               0x0F

#define   B_ERR_CODE                                (BIT15 | BIT14 | BIT13 | BIT12)
#define     V_ERR_CORE_MEFW_NOERROR                   0x00
#define     V_ERR_CORE_MEFW_UNCATEGORIZED             0x01
#define     V_ERR_CORE_MEFW_DISABLED                  0x02
#define     V_ERR_CORE_MEFW_IMG_FAILURE               0x03

#define   B_EXT_STAT                                ( BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24 | \
                                                      BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)

#define R_HIDM                                    0xA0

//
// ME-HECI MBAR registers
//
#define R_HECI_HOST_CB_WW                         0x00
#define R_HECI_HOST_H_CSR                         0x04
#define R_HECI_ME_CB_RW                           0x08
#define R_HECI_ME_CSR_HA                          0x0C
#define R_HECI_GEN_STS                            0x4C

#define PCH_LBG_MROM0_PORT_DEVICE               17
#define PCH_LBG_MROM0_PORT_FUNCTION             0
#define PCH_LBG_MSUINT_MROM0_PLKCTL_REG         0xe8
#define PCH_LBG_MSUINT_MROM0_MSDEVFUNCHIDE_REG  0xD4

#define R_PCH_DMI_LSTS                         0x21AA
#define R_PCH_DMI_LCTL2                        0x21B0

#define PCH_PMC_ACPI_BASE                      0x40


/**
==================================================================================================
================================== LBC Command Definitions     ===================================
==================================================================================================
**/
#define R_OFFSET_LBC_CTRL_AFE_14NM                0x600
#define R_OFFSET_LBC_DATA_AFE_14NM                0x604
#define R_OFFSET_LBC_CTRL_COMMON_14NM             0x658
#define R_OFFSET_LBC_DATA_COMMON_14NM             0x65C

#define V_NUM_LANES_IN_BUNDLE                     0x2   // A bundle has 2 lanes
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE0_10NM        0x214
#define R_OFFSET_LBC_DATA_AFE_BUNDLE0_10NM        0x218
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE1_10NM        0x294
#define R_OFFSET_LBC_DATA_AFE_BUNDLE1_10NM        0x298
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE2_10NM        0x314
#define R_OFFSET_LBC_DATA_AFE_BUNDLE2_10NM        0x318
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE3_10NM        0x394
#define R_OFFSET_LBC_DATA_AFE_BUNDLE3_10NM        0x398
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE4_10NM        0x414
#define R_OFFSET_LBC_DATA_AFE_BUNDLE4_10NM        0x418
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE5_10NM        0x494
#define R_OFFSET_LBC_DATA_AFE_BUNDLE5_10NM        0x498
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE6_10NM        0x514
#define R_OFFSET_LBC_DATA_AFE_BUNDLE6_10NM        0x518
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE7_10NM        0x594
#define R_OFFSET_LBC_DATA_AFE_BUNDLE7_10NM        0x598

#define R_OFFSET_LBC_CTRL_AFE_BUNDLE0_ICX         0x914
#define R_OFFSET_LBC_DATA_AFE_BUNDLE0_ICX         0x918
#define R_OFFSET_LBC_CTRL_AFE_BUNDLE1_ICX         0x994
#define R_OFFSET_LBC_DATA_AFE_BUNDLE1_ICX         0x998

#define R_OFFSET_LBC_CTRL_COMMON_10NM             0x8D0
#define R_OFFSET_LBC_DATA_COMMON_10NM             0x8D4


#define LBC_CPL_COMMAND                           0

#define LBC_CTRL_START_COMMAND                    0x1
#define LBC_CTRL_START_MASK                       0x1    //  Width 1 bit
#define LBC_CTRL_START_BIT_POSITION_14NM          0      // Load bus controller start bit position, Bit[0] for Cpx, and Skx
#define LBC_CTRL_START_BIT_POSITION_10NM          16     // Load bus controller start bit position,  Bit[16] for Snr

#define LBC_CTRL_LOAD_SEL_MASK                    0x7F   //  Width 7 bits
#define LBC_CTRL_LOAD_SEL_BIT_POSITION_14NM       3      //  Bit position of the Load bus controller end point selection for Cpx and Skx, Bit[9:3].
#define LBC_CTRL_LOAD_SEL_BIT_POSITION_10NM       0      //  Bit position of the Load bus controller end point selection for Snr, Bit[6:0].

#define LBC_CTRL_READ_COMMAND                     0x1
#define LBC_CTRL_WRITE_COMMAND                    0x2
#define LBC_CTRL_REQ_MASK                         0x3    //  Load bus controller request type Width: 2 bits
#define LBC_CTRL_REQ_BIT_POSITION_14NM            1      //  Load bus controller request type Bit position, Bit[2:1] for Cpx, and Skx
#define LBC_CTRL_REQ_BIT_POSITION_10NM            8      //  Load bus controller request type Bit position, Bit[9:8] for Snr

#define LBC_CTRL_LN_SEL_MASK                      0x3    //  Width 2 bits (each bundle has 2 lanes)
#define LBC_CTRL_LN_SEL_BIT_POSITION_14NM         11     //  Load bus controller Lane select Bit position, Bits[12:11] for Cpx, and Skx (bundle 0)
#define LBC_CTRL_LN_SEL_BIT_POSITION_10NM         10     //  Load bus controller Lane select Bit position, Bit[11:10] for Snr

#define LBC_DATA_MASK                             0x3FFF // Width 14 bits
#define LBC_DATA_BIT_POSITION                     0x1


#define LBC_PROGRAM_DELAY                         1       //us
#define LBC_TIME_OUT                              5000   // 5 ms



/**
==================================================================================================
================================== UniPhy Definitions            ===================================
==================================================================================================
**/
//
// BIT 31 of PortBit field indicates IIO Global Dfx Register
//
#define GLOBAL_DFX_BIT                            0x80000000
#define IIO_UNKNOW_REG                            0xF            // An invalid case, a unknown register.
#define IIO_PORT_PCIE_REG                         0x0            // Register type PCIe
#define IIO_PORT_DFX_REG                          0x1            // DFX register per corresponding PCIe port
#define IIO_GLOBAL_DFX_REG                        0x7            // DFX register for all PCIe ports
#define IIO_REG_WO_BDF_ASSIGNED                   0xA            // An valid case, registers without BDF assigned (example SideBand)
#define R_PCIE_DFX_PORT_DFXPXPPRTGEN3EQPRIV       0x39C          // Register defined in Registers/Snr/IIO_DFX.h
#define R_PCIE_DFX_PORT_DFXGEN3PRIVTXEQ           0x3AC          // Register defined in Registers/Snr/IIO_DFX.h
#define R_PCIE_DFX_PORT_DFXTXPH3CURSOR            0x3CC          // Register defined in Registers/Snr/IIO_DFX.h
/**
==================================================================================================
================================== GEN3 Definitions            ===================================
==================================================================================================
**/

#define GEN3_LINK_POLL_GRANULARITY          100     //us
#define UNIPHY_RECIPE_DELAY                 180     //us
#define UNIPHY_DIS_DELAY                    100000  //us  (100ms)
#define VPP_WAIT_DELAY                      500     //us
#define GEN3_STALL_SBR                      2000    //us
#define IIO_LINK_TRAIN_TIME_NO_G3EQ         30000   //us  (30ms)
#define IIO_LINK_TRAIN_TIME_MARGIN_NO_G3EQ  100000  //us  (100ms)
#define WAIT_FOR_DOWNSTREAM_DEV             100000  //us  (100ms)
#define IIO_LINK_TRAIN_TIME_G3EQ            60000   //us  (60ms)
#define IIO_LINK_TRAIN_TIME_MARGIN_G3EQ     200000  //us  (200ms)
#define GEN3_LINK_POLL_TIMEOUT              10000   //us
#define LINK_INIT_POLL_TIMEOUT              10000   //us
#define SLTCAP_DELAY                        200000  //us
#define GEN3_LINK_POLL_TIMEOUT_MID          300000  //us
#define LINK_TRAINING_DELAY                 400000  //us
#define GEN3_LINK_POLL_TIMEOUT_RECOVERY     2000000 //us
#define SLOT_POWER_DELAY                    1000    //us  (1ms)
#define RX_RESET_DELAY                      1       //us

#define NONE_GEN3_PCIE_PORT                 0xFF

#define GEN3_SUCCESS                        0
#define GEN3_FAILURE                        1

/**
==================================================================================================
================================== IIO POST code - Major Definitions      ========================
==================================================================================================
**/

#define STS_IIO_EARLY_INIT_ENTRY            0xE0  // IIO early init entry
#define STS_EARLY_PRELINK_TRAINING          0xE1  // Early Pre-link training setting
#define STS_GEN3_EQ_PROGRAMMING             0xE2  // IIO Gen3 EQ programming
#define STS_LINK_TRAINING                   0xE3  // IIO Link training
#define STS_GEN3_OVERRIDE                   0xE4  // IIO Gen3 override
#define STS_IIO_EARLY_INIT_EXIT             0xE5  // IIO early init exit
#define STS_IIO_LATE_INIT_ENTRY             0xE6  // IIO late init entry
#define STS_IIO_PCIE_PORT_INIT              0xE7  // PCIE port init
#define STS_IIO_IOAPIC_INIT                 0xE8  // IOAPIC init
#define STS_IIO_VTD_INIT                    0xE9  // VTD init
#define STS_IIO_IOAT_INIT                   0xEA  // IOAT init
#define STS_IIO_DFX_INIT                    0xEB  // IIO DFX init
#define STS_IIO_NTB_INIT                    0xEC  // NTB init
#define STS_IIO_SECURITY_INIT               0xED  // Security init
#define STS_IIO_LATE_INIT_EXIT              0xEE  // IIO late init exit
#define STS_IIO_ON_READY_TO_BOOT            0xEF  // IIO On ready to boot

/**
==================================================================================================
================================== IIO POST code - Minor Definitions      ========================
==================================================================================================
**/
#define STS_IIO_INIT                        0x0   // IIO On ready to boot
#define STS_IIO_MS2IOSF_CREDITS_ENTRY       0x10  // IIO MS2IOSF Credit entry
#define STS_IIO_MS2IOSF_CREDITS_EXIT        0x1F  // IIO MS2IOSF Credit exit
#define STS_IIO_MS2IOSF_BANK_DECODE_ENTRY   0x20  // IIO MS2IOSF Bank decoder entry
#define STS_IIO_MS2IOSF_BANK_DECODE_EXIT    0x2F  // IIO MS2IOSF Bank decoder exit


#define DISABLE_ALL_PORTS           0xFFFF
#define DISABLE_A_PORT              0x000F
#define DISABLE_B_PORT              0x00F0
#define DISABLE_C_PORT              0x0F00
#define DISABLE_D_PORT              0xF000

/**
 * SAI bit definitions.
 **/
#define  HOSTIA_POSTBOOT_SAI        BIT0
#define  HOSTIA_SMM_SAI             BIT2
#define  HOSTIA_BOOT_SAI            BIT4
#define  DFX_UNTRUSTED_SAI          BIT25

#endif //_IIO_REGS_H_

