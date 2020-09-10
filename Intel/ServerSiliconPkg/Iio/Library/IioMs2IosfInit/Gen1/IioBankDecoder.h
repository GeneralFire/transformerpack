/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _MS2IOSF_BANK_DECODER_H_
#define _MS2IOSF_BANK_DECODER_H_

#include "Include/IioBankDecoderCommon.h"

#define BANK_NONE                ((UINT8)-1)

// Bank range
#define TYPE0_BANK_START         0xA00
#define TYPE0_BANK_STOP          0xC60
#define TYPE0_OFFSET_PER_BANK    0x20

#define TYPE1_BANK_START         0x800
#define TYPE1_BANK_STOP          0xA00
#define TYPE1_OFFSET_PER_BANK    0x40

#define PORT_0A_BANK             BANK0_DEC
#define PORT_0B_BANK             BANK1_DEC
#define PORT_0C_BANK             BANK2_DEC
#define PORT_0D_BANK             BANK3_DEC
#define PORT_1A_BANK             BANK4_DEC
#define PORT_1B_BANK             BANK5_DEC
#define PORT_1C_BANK             BANK6_DEC
#define PORT_1D_BANK             BANK7_DEC

#define PORT_1A_FLIP_BANK        BANK0_DEC
#define PORT_1B_FLIP_BANK        BANK1_DEC
#define PORT_1C_FLIP_BANK        BANK2_DEC
#define PORT_1D_FLIP_BANK        BANK3_DEC
#define PORT_0A_FLIP_BANK        BANK4_DEC
#define PORT_0B_FLIP_BANK        BANK5_DEC
#define PORT_0C_FLIP_BANK        BANK6_DEC
#define PORT_0D_FLIP_BANK        BANK7_DEC

// Only applicable for ICX-SP Redefined
#define PORT_0A_BAR_BANK         BANK8_DEC
#define PORT_0B_BAR_BANK         BANK9_DEC
#define PORT_0C_BAR_BANK         BANK10_DEC
#define PORT_0D_BAR_BANK         BANK11_DEC

#define GUNIT0_SRIOV_BANK        BANK0_DEC
#define GUNIT1_SRIOV_BANK        BANK1_DEC

#define HQM_SRIOV_BANK           BANK0_DEC
#define TIP_SRIOV_BANK           BANK0_DEC

#define CPM_VRP_BANK             BANK0_DEC
#define CPK_VRP_BANK             BANK1_DEC

#define NTB_0A_BANK_1            BANK12_DEC
#define NTB_0A_BANK_2            BANK13_DEC
#define NTB_0A_BANK_3            BANK14_DEC
#define NTB_1A_BANK_1            BANK16_DEC
#define NTB_1A_BANK_2            BANK17_DEC
#define NTB_1A_BANK_3            BANK18_DEC

#define NTB_1A_FLIP_BANK_1       BANK12_DEC
#define NTB_1A_FLIP_BANK_2       BANK13_DEC
#define NTB_1A_FLIP_BANK_3       BANK14_DEC
#define NTB_0A_FLIP_BANK_1       BANK16_DEC
#define NTB_0A_FLIP_BANK_2       BANK17_DEC
#define NTB_0A_FLIP_BANK_3       BANK18_DEC

#define CBDMA_CH0_BANK           BANK8_DEC
#define CBDMA_CH1_BANK           BANK9_DEC
#define CBDMA_CH2_BANK           BANK10_DEC
#define CBDMA_CH3_BANK           BANK11_DEC
#define CBDMA_CH4_BANK           BANK12_DEC
#define CBDMA_CH5_BANK           BANK13_DEC
#define CBDMA_CH6_BANK           BANK14_DEC
#define CBDMA_CH7_BANK           BANK15_DEC

#define MSM_BANK                 BANK19_DEC
#define MSM_PMON_BANK_1          BANK20_DEC
#define MSM_PMON_BANK_2          BANK21_DEC
#define MSM_DFX_BANK             BANK22_DEC

#define NPK_ACPI_BANK            BANK2_DEC
#define NPK_1_BANK               BANK23_DEC
#define NPK_2_BANK               BANK24_DEC
#define NPK_3_BANK               BANK25_DEC
#define NPK_FWBAR_BANK           BANK26_DEC

#define DMI_BANK                 BANK27_DEC
#define RLINK_BANK               BANK27_DEC
#define HFI_BANK                 BANK27_DEC

#define SPMC_BANK                BANK8_DEC

//
// MS2IOSF bank register offset
//
#define OFFSET_MASK              0xF
#define BANKCTRL_OFFSET          0x0
#define DESTID_OFFSET            0x2
#define PCICMD_OFFSET            0x8
#define BAR_N0_OFFSET            0x10
#define BAR_N1_OFFSET            0x14
#define BCTRL_OFFSET             0x26

//
// MS2IOSF bank register field
//
#define BANK_DECODE_ENABLE       BIT3
#define MC_BANK_DECODE_ENABLE    BIT5
#define MC_SHADOW_ENABLE         BIT6
#define VGAEN                    BIT3

//
// MS2IOSF Signature
//
#define MS2ISOF_BANK_DECODER_SIGNATURE  0x5F42445F

typedef struct {
  UINT16  DualPcie;
  UINT16  FlippedDualPcie;
  UINT16  DualPcieMcp;
  UINT16  DmiCbSinglePcie;
  UINT16  CbRlink;
  UINT16  SinglePcieGen3;
  UINT16  SinglePcieGen4;
  UINT16  FlippedSinglePcieGen4;
  UINT16  DualGunit;
  UINT16  SingleHfi;
  UINT16  CbDmiGen4;
  UINT16  Rlink;
  UINT16  CpkNac;
  UINT16  FlippedCpkNac;
  UINT16  CpmNac;
  UINT16  Hqm;
  UINT16  TurboIp;
  UINT16  CbRlinkNac;
  UINT16  Disable; // Not covered by official spreadsheet, but added for non-valid cases
} STACK_TYPE_DATA;

typedef struct {
  UINT8            BankNumber;
  UINT32           RegisterAddress;       // 32 bit MMCFG encoded address
  UINT8            RegisterSize;
  UINT32           FieldMask;               // bit mask
  STACK_TYPE_DATA  StackTypeData;
} BANK_DECODER_ENTRY;

#define REV_STR_CHARS                    8
#define HELP_STR_CHARS                   64

typedef struct {
  UINT32  Signature;                         // "_BD_"
  CHAR8   Revision[REV_STR_CHARS];           // 1
  UINT16  NumberEntries;                     //
  UINT8   DebugMode;                         // 0 or 1
  CHAR8   HelperString[HELP_STR_CHARS];      // fill in bank decoder, table rev and build date
} BANK_DECODER_HEADER;

#endif // _MS2IOSF_BANK_DECODER_H_

