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

#ifndef _MS2ISOF_BANK_DECODER_COMMON_H_
#define _MS2ISOF_BANK_DECODER_COMMON_H_

//
// Bank definitions
//
#define BANK0_DEC                0
#define BANK1_DEC                1
#define BANK2_DEC                2
#define BANK3_DEC                3
#define BANK4_DEC                4
#define BANK5_DEC                5
#define BANK6_DEC                6
#define BANK7_DEC                7
#define BANK8_DEC                8
#define BANK9_DEC                9
#define BANK10_DEC               10
#define BANK11_DEC               11
#define BANK12_DEC               12
#define BANK13_DEC               13
#define BANK14_DEC               14
#define BANK15_DEC               15
#define BANK16_DEC               16
#define BANK17_DEC               17
#define BANK18_DEC               18
#define BANK19_DEC               19
#define BANK20_DEC               20
#define BANK21_DEC               21
#define BANK22_DEC               22
#define BANK23_DEC               23
#define BANK24_DEC               24
#define BANK25_DEC               25
#define BANK26_DEC               26
#define BANK27_DEC               27

#define BANK_NONE                ((UINT8)-1)

// Bank range
#define TYPE0_BANK_START         0xA00
#define TYPE0_BANK_STOP          0xC60
#define TYPE0_OFFSET_PER_BANK    0x20

#define TYPE1_BANK_START         0x800
#define TYPE1_BANK_STOP          0xA00
#define TYPE1_OFFSET_PER_BANK    0x40
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

#endif // _MS2ISOF_BANK_DECODER_COMMON_H_

