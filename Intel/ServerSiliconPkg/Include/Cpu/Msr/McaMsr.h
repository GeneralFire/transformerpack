/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef _MCA_MSR_H_
#define _MCA_MSR_H_

//
// Defines
//
#define MCI_REG_BASE    0x400

#define IFU_CTL      (MCI_REG_BASE + 0x00)
#define DCU_CTL      (MCI_REG_BASE + 0x04)
#define DTLB_CTL     (MCI_REG_BASE + 0x08)
#define MLC_CTL      (MCI_REG_BASE + 0x0C)
#define PCU_CTL      (MCI_REG_BASE + 0x10)
#define QPI0_CTL     (MCI_REG_BASE + 0x14)
#define IIO_CTL      (MCI_REG_BASE + 0x18)
#define HA0_CTL      (MCI_REG_BASE + 0x1C)
#define HA1_CTL      (MCI_REG_BASE + 0x20)
#define IMC0_CTL     (MCI_REG_BASE + 0x24)
#define IMC1_CTL     (MCI_REG_BASE + 0x28)
#define IMC2_CTL     (MCI_REG_BASE + 0x2C)
#define IMC3_CTL     (MCI_REG_BASE + 0x30)
#define IMC4_CTL     (MCI_REG_BASE + 0x34)
#define IMC5_CTL     (MCI_REG_BASE + 0x38)
#define IMC6_CTL     (MCI_REG_BASE + 0x3C)
#define IMC7_CTL     (MCI_REG_BASE + 0x40)
#define CBO00_CTL    (MCI_REG_BASE + 0x44)
#define CBO01_CTL    (MCI_REG_BASE + 0x48)
#define CBO02_CTL    (MCI_REG_BASE + 0x4C)
#define CBO03_CTL    (MCI_REG_BASE + 0x50)
#define CBO04_CTL    (MCI_REG_BASE + 0x54)
#define CBO05_CTL    (MCI_REG_BASE + 0x58)
#define CBO06_CTL    (MCI_REG_BASE + 0x5C)
#define CBO07_CTL    (MCI_REG_BASE + 0x60)
#define CBO08_CTL    (MCI_REG_BASE + 0x64)
#define CBO09_CTL    (MCI_REG_BASE + 0x68)
#define CBO10_CTL    (MCI_REG_BASE + 0x6C)
#define CBO11_CTL    (MCI_REG_BASE + 0x70)
#define CBO12_CTL    (MCI_REG_BASE + 0x74)
#define CBO13_CTL    (MCI_REG_BASE + 0x78)
#define CBO14_CTL    (MCI_REG_BASE + 0x7C)

#define MCBANK_MAX                0x20

#define IMC_PATROL_SCRUB_ERR_MCACOD                 0xC0
#define IMC_PATROL_SCRUB_ERR_MCACOD_MSK             0xFFF0
#define IMC_UNC_PATROL_SCRUB_ERR_MSCOD              0x10
#define IMC_COR_PATROL_SCRUB_ERR_MSCOD              0x08
#define IMC_PATROL_SCRUB_ERR_MSCOD_MSK              0xFFFF
#define IMC_GENERIC_ERR_MCACOD                      0x80
#define IMC_GENERIC_ERR_MCACOD_MSK                  0xFF80

#define IMC_SPARING_ERR_MCACOD                      0xC0
#define IMC_SPARING_ERR_MCACOD_MSK                  0xFFF0
#define IMC_UNC_SPARING_ERR_MSCOD                   0x40
#define IMC_COR_SPARING_ERR_MSCOD                   0x20
#define IMC_SPARING_ERR_MSCOD_MSK                   0xFFFF

// DDR4 C/A Parity Error.

#define IMC_DDR4_CAP_ERR_MCACOD             0x0B0
#define IMC_DDR4_CAP_ERR_MCACOD_MSK         0x0F0
#define IMC_DDR4_CAP_ERR_MSCOD              0x0200
#define IMC_DDR4_CAP_ERR_MSCOD_MSK          0x0FFF
#define IMC_DDRT_CAP_ERR_MCACOD             0x80
#define IMC_DDRT_CAP_ERR_MCACOD_MSK         0xF0
#define IMC_DDRT_CAP_ERR_MSCOD              0x80d
#define IMC_DDRT_CAP_ERR_MSCOD_MSK          0x0FFF

#define IMC_MCACOD_CHANNEL_MSK             0x000F
#define IMC_MCACOD_ERRMODE_MSK             0xFF80
#define IMC_MCACOD_ERRTYPE_MSK             0x0070
#define M2M0_BANK  7
#define M2M1_BANK  8

#define IOMCA_MCACOD_ERR_MSK               0x0E0B

typedef union {
   struct{
      UINT64 mca_code:16;
                        /* MCA architected error code */
      UINT64 ms_code:16;
                        /* Model specific error code */
      UINT64 other_info:6;
      UINT64 corrected_err_count:15;
      UINT64 threshold_err_status:2;
      UINT64 ar:1;
                        /* Recovery action required for UCR error */
      UINT64 s:1;
                        /* Signaling an uncorrected recoverable (UCR) error*/
      UINT64 pcc:1;
                        /* processor context corrupted */
      UINT64 addrv:1;
                        /* MCi_ADDR valid */
      UINT64 miscv:1;
                        /* MCi_MISC valid */
      UINT64 en:1;
                        /* error reporting enabled */
      UINT64 uc:1;
                        /* uncorrected error */
      UINT64 over:1;
                        /* error overflow */
      UINT64 val:1;
                        /* MCi_STATUS valid */
   } Bits;
   UINT64 Data;
} IA32_MCI_STATUS_MSR_STRUCT;

typedef union {
  struct {
    UINT32 ripv:1;
    UINT32 eipv:1;
    UINT32 mcip:1;
    UINT32 lmce:1;
    UINT32 rsvd:28;
  } Bits;
  UINT32 Data;
} IA32_MCG_STATUS_MSR_STRUCT;

typedef union {
   struct{
      UINT64 error_threshold:15;
                        /* Corrected error count threshold */
      UINT64 reserved0:15;
                        /* reserved */
      UINT64 CMCI_EN:1;
                        /*Enable/Disable CMCI*/
      UINT64 reserved1:33;
                        /* reserved */
   } Bits;
   UINT64 Data;
} IA32_MCI_CTL2_MSR_STRUCT;

//
// Functions prototype declarations
//

#endif  // _MCA_MSR_H_

