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

#ifndef __EV_AUTO_RECIPE_H__
#define __EV_AUTO_RECIPE_H__

#include <UncoreCommonIncludes.h>
#include <Library/EvAutoRecipeLib.h>

#define EV_AUTO_RECIPE_SIGNATURE  0x5F56455F

// Wild card definitions
//
#define WILDCARD_8     (0xFF)
#define WILDCARD_16    (0xFFFF)
#define WILDCARD_32    (0xFFFFFFFF)
#define WILDCARD_64    (0xFFFFFFFFFFFFFFFF)


#define PORT_ID_SHL      16

typedef enum {
  DMI_CFG = 0,
  PCIE_CFG,
  WFR_CFG,
  SRIS_CFG,
  FPGA_CFG,
  KTI_CFG,
  RCOMPOVRD_CFG,
  CPX4_CFG,
  NTB_CFG,
  LINK_CONFIG_TYPE_MAX
} LINK_CONFIG_TYPE;

typedef enum {
  TYPE_CSR = 0,
  TYPE_MSR,
  TYPE_IO,
  TYPE_KTI_IAR,
  TYPE_LBC_AFE,
  TYPE_LBC_COM,
  TYPE_LBC_COM_VCU,
  TYPE_CSR_VCU,
  TYPE_CSR_SB,
  TYPE_MEM_BAR,
  ACCESS_TYPE_MAX,
} ACCESS_TYPE;

typedef enum {
  WINDOW1_IAR_IOVB = 1,
  WINDOW1_IAR_IOVB_BCAST,
  WINDOW1_IAR_IOVC,
  WINDOW1_SUB_TYPE_MAX,
} WINDOW1_SUB_TYPE;

#define A0_STEP EV_REV_BIT_00
#define A1_STEP EV_REV_BIT_00
#define A2_STEP EV_REV_BIT_02
#define B0_STEP EV_REV_BIT_03
#define B1_STEP EV_REV_BIT_05
#define L0_STEP EV_REV_BIT_04
#define H0_STEP EV_REV_BIT_06
#define H1_STEP EV_REV_BIT_06
#define M0_STEP EV_REV_BIT_07
#define U0_STEP EV_REV_BIT_08


#define J0_STEP EV_REV_BIT_09
#define N0_STEP EV_REV_BIT_11
#define V0_STEP EV_REV_BIT_12



typedef enum {
  KTI_RATE_96GT = 0,
  KTI_RATE_104GT,
  MAX_QPI_RATE,
} QPI_RATE_TYPE;

//Place holder for future use in PCIe
typedef enum {
  PCIE_RATE_25GT    = 0,
  PCIE_RATE_50GT,
  PCIE_RATE_80GT,
  PCIE_RATE_160GT,
  PCIE_RATE_320GT,
  MAX_PCIE_RATE,
} PCIE_RATE_TYPE;

typedef enum {
  SKX_SOCKET = 0,
  SNR_SOCKET,
  ICX_SOCKET,
  CPX_SOCKET,
  SPR_SOCKET,
  PROCESSOR_TYPE_MAX,
} PROCESSOR_TYPE;

#define CLX_SOCKET SKX_SOCKET

//
//  Macros to get Window 1 access's sub-type and address
//
#define WINDOW1_ACCESS_SUB_TYPE(x)  ((x & 0xF0000000) >> 28)
#define WINDOW1_ACCESS_REG_ADDR(x)  (x & 0x3F)

//
//  Macros to form Window 1 based address from sub-type and reg addr
//
#define WINDOW1_ADDR(subtype, regaddr)  ((subtype << 28) | (regaddr & 0xFF))

#define REV_STR_CHARS                    10
#define HELP_STR_CHARS                   64

#pragma pack(1)
typedef struct {
  UINT32  Signature;                         // "_VE_"
  CHAR8   Revision[REV_STR_CHARS];           // 1
  UINT16  NumberEntries;                     //
  UINT8   DebugMode;                         // 0 or 1
  CHAR8   HelperString[HELP_STR_CHARS];      // fill in uniPhy, table rev and build date
} EV_RECIPE_HEADER;

typedef struct {
  UINT8           AccessType;            // One of ACCESS_TYPE
  UINT8           RegisterSize;          // size in bytes
  UINT16          Config;                 // 1 << one of LINK_CONFIG_TYPE, * = -1
  UINT32          RegisterAddress;       // 32 bit MMCFG encoded address | LBC load select
  UINT32          FieldMask;             // 32 bit field mask to AND field to zero
  UINT32          SocBitmap;             // * = -1, Else set bit for each valid socket to program
  UINT32          PortBitmap;            // * = -1, Else set bit for each port
  UINT32          LaneBitmap;            // * = -1, Else set bit for each lane
  UINT32          BitRates;              // Bitmask of rates. See Enum *_RATE_TYPE
  UINT64          Steppings;             // 1 << A0_REV_IVT, * = -1    //Will need to evaluate how we handle this dynamically
  UINT32          ProcessorType;         // SKX_SOCKET
  UINT16          Skus;                  // 1 << EP_SKU, * = -1
  UINT16          Chops;                 // Ignore for now
  UINT32          Data;                  // Value to write i.e data << start bit
} EV_RECIPE_ENTRY;

typedef union {
  struct {
    UINT32 LengthCode : 2;  // LengthCode - Bits[01:00]: 0 = 8-bits, 1 = 16-bits, 2 = 32-bits, 3 = 64-bits
    UINT32 Bar        : 2;  // BAR        - Bits[03:02]: Bar number (mmio only)
    UINT32 Bus        : 3;  // Bus        - Bits[06:04]: Bus Number (CSR), Instance number (mmio)
    UINT32 Reserved   : 1;  // RESERVED   - Bits[7]
    UINT32 Function   : 3;  // Function   - Bits[10:08]: Function Number
    UINT32 Device     : 5;  // Device     - Bits[15:11]: Device Number
    UINT32 Offset     :16;  // Offset     - Bits[31:16]: Register offset
  } Bits;
  UINT32 Data;
} VCU_API_DATA_FIELD;

#pragma pack()

#endif
