/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _MS2IOSF_BANK_DECODER_GEN2_H_
#define _MS2IOSF_BANK_DECODER_GEN2_H_

#include "Include/IioBankDecoderCommon.h"

// Bank range
#define TYPE0_BANK_STOP_GEN2         0xB60
#define SUBTRACTIVE_BANK_START_GEN2  0xCF0

#define PORT_x16_A_BANK_GEN2            BANK0_DEC
#define PORT_x16_B_BANK_GEN2            BANK1_DEC
#define PORT_x16_C_BANK_GEN2            BANK2_DEC
#define PORT_x16_D_BANK_GEN2            BANK3_DEC
#define PORT_x8_A_BANK_GEN2             BANK4_DEC
#define PORT_x8_B_BANK_GEN2             BANK5_DEC
#define PORT_x8_C_BANK_GEN2             BANK6_DEC
#define PORT_x8_D_BANK_GEN2             BANK7_DEC

#define PORT_x16_A_BAR_BANK_GEN2        BANK8_DEC
#define PORT_x16_B_BAR_BANK_GEN2        BANK9_DEC
#define PORT_x16_C_BAR_BANK_GEN2        BANK10_DEC
#define PORT_x16_D_BAR_BANK_GEN2        BANK11_DEC
#define PORT_x8_A_BAR_BANK_GEN2         BANK12_DEC
#define PORT_x8_B_BAR_BANK_GEN2         BANK13_DEC
#define PORT_x8_C_BAR_BANK_GEN2         BANK14_DEC
#define PORT_x8_D_BAR_BANK_GEN2         BANK15_DEC

#define NTB_BAR_0_BANK_GEN2             BANK16_DEC
#define NTB_BAR_1_BANK_GEN2             BANK17_DEC
#define NTB_BAR_2_BANK_GEN2             BANK18_DEC

#define CPM_SRIOV_BANK_GEN2             BANK0_DEC
#define HQM_SRIOV_BANK_GEN2             BANK1_DEC

#define MSM_BANK_GEN2                   BANK8_DEC
#define MSM_PMON_1_BANK_GEN2            BANK9_DEC
#define MSM_PMON_2_BANK_GEN2            BANK10_DEC

#define CXL_DECODE_BANK_GEN2            BANK19_DEC
#define DMI_DECODE_BANK_GEN2            BANK19_DEC
#define FABRIC_DECODE_BANK_GEN2         BANK19_DEC

//
// DINO
//
#define NPK_1_BANK_GEN2                 BANK11_DEC
#define NPK_2_BANK_GEN2                 BANK12_DEC
#define NPK_3_BANK_GEN2                 BANK13_DEC
#define NPK_FWBAR_BANK_GEN2             BANK14_DEC

#define DSA_BAR_0_BANK_GEN2             BANK15_DEC
#define DSA_BAR_1_BANK_GEN2             BANK16_DEC

#define IAX_BAR_0_BANK_GEN2             BANK17_DEC
#define IAX_BAR_1_BANK_GEN2             BANK18_DEC

//
// DDON
//
#define MSM_DFX_DDON_BANK_GEN2          BANK11_DEC

#define NPK_1_DDON_BANK_GEN2            BANK12_DEC
#define NPK_2_DDON_BANK_GEN2            BANK13_DEC
#define NPK_3_DDON_BANK_GEN2            BANK14_DEC
#define NPK_FWBAR_DDON_BANK_GEN2        BANK15_DEC

#define DSA_BAR_0_DDON_BANK_GEN2        BANK16_DEC
#define DSA_BAR_1_DDON_BANK_GEN2        BANK17_DEC

//
// MS2IOSF bank register offset
//
#define BANKCTRL2_OFFSET                0x28


typedef struct {
  UINT32  Pi5PcieMode;
  UINT32  Pi5CxlMode;
  UINT32  Pi5Dmi;
  UINT32  DinoHcx;
  UINT32  DinoHcxB0;
  UINT32  DdonDmiHcx;
  UINT32  DinoHcb;
  UINT32  DdonPcieHcx;
  UINT32  Disable;
} STACK_TYPE_DATA_GEN2;

typedef struct {
  UINT8                 BankNumber;
  UINT32                RegisterAddress;       // 32 bit MMCFG encoded address
  UINT8                 RegisterSize;
  UINT32                FieldMask;               // bit mask
  STACK_TYPE_DATA_GEN2  StackTypeData;
} BANK_DECODER_ENTRY_GEN2;

#define REV_STR_CHARS                    8
#define HELP_STR_CHARS                   64

typedef struct {
  UINT32  Signature;                         // "_BD_"
  CHAR8   Revision[REV_STR_CHARS];           // 1
  UINT16  NumberEntries;                     //
  UINT8   DebugMode;                         // 0 or 1
  CHAR8   HelperString[HELP_STR_CHARS];      // fill in bank decoder, table rev and build date
} BANK_DECODER_HEADER_GEN2;

#endif // _MS2IOSF_BANK_DECODER_GEN2_H_
