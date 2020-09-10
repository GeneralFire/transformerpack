/** @file
  Register names for ADL PCH LPC/eSPI device

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _PCH_REGS_LPC_ADL_H_
#define _PCH_REGS_LPC_ADL_H_

#define V_LPC_CFG_DID_ADL_S                       0x7A80
#define V_LPC_CFG_DID_ADL_P                       0x5180
#define V_LPC_CFG_DID_ADL_M                       0x5480


//
// ADL PCH-P LPC Device IDs
//
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_0         0x5180          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_1         0x5181          ///< LPC/eSPI Controller P SuperSKU
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_2         0x5182          ///< LPC/eSPI Controller P Premium
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_3         0x5183          ///< LPC/eSPI Controller Placeholder
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_4         0x5184          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_5         0x5185          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_6         0x5186          ///< LPC/eSPI Controller M Super SKU
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_7         0x5187          ///< LPC/eSPI Controller M Premium
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_8         0x5188          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_9         0x5189          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_10        0x518A          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_11        0x518B          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_12        0x518C          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_13        0x518D          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_14        0x518E          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_15        0x518F          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_16        0x5190          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_17        0x5191          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_18        0x5192          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_19        0x5193          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_20        0x5194          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_21        0x5195          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_22        0x5196          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_23        0x5197          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_24        0x5198          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_25        0x5199          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_26        0x519A          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_27        0x519B          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_28        0x519C          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_29        0x519D          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_30        0x519E          ///< LPC/eSPI Controller
#define V_ADL_PCH_P_LPC_CFG_DEVICE_ID_MB_31        0x519F          ///< LPC/eSPI Controller

//
// ADL PCH-M LPC Device IDs
//
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_0         0x5480          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_1         0x5481          ///< LPC/eSPI Controller U SuperSKU
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_2         0x5482          ///< LPC/eSPI Controller U Premiun
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_3         0x5483          ///< LPC/eSPI Controller U Base/Mainstream
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_4         0x5484          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_5         0x5485          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_6         0x5486          ///< LPC/eSPI Controller Y SuperSKU
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_7         0x5487          ///< LPC/eSPI Controller Y Premium
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_8         0x5488          ///< LPC/eSPI Controller IOT SKU
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_9         0x5489          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_10        0x548A          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_11        0x548B          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_12        0x548C          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_13        0x548D          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_14        0x548E          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_15        0x548F          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_16        0x5490          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_17        0x5491          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_18        0x5492          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_19        0x5493          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_20        0x5494          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_21        0x5495          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_22        0x5496          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_23        0x5497          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_24        0x5498          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_25        0x5499          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_26        0x549A          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_27        0x549B          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_28        0x549C          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_29        0x549D          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_30        0x549E          ///< LPC/eSPI Controller
#define V_ADL_PCH_M_LPC_CFG_DEVICE_ID_MB_31        0x549F          ///< LPC/eSPI Controller

//
// ADL PCH-S LPC Device IDs
//
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_0         0x7A80          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_1         0x7A81          ///< LPC/eSPI Controller Client SuperSKU (SSKU) unlocked
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_2         0x7A82          ///< LPC/eSPI Controller Server SuperSKU (SSKU) unlocked
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_3         0x7A83          ///< LPC/eSPI Controller Q670
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_4         0x7A84          ///< LPC/eSPI Controller Z690
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_5         0x7A85          ///< LPC/eSPI Controller H670
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_6         0x7A86          ///< LPC/eSPI Controller B660
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_7         0x7A87          ///< LPC/eSPI Controller H610
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_8         0x7A88          ///< LPC/eSPI Controller W680
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_9         0x7A89          ///< LPC/eSPI Controller X699
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_10        0x7A8A          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_11        0x7A8B          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_12        0x7A8C          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_13        0x7A8D          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_14        0x7A8E          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_15        0x7A8F          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_16        0x7A90          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_17        0x7A91          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_18        0x7A92          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_19        0x7A93          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_20        0x7A94          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_21        0x7A95          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_22        0x7A96          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_23        0x7A97          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_24        0x7A98          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_25        0x7A99          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_26        0x7A9A          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_27        0x7A9B          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_28        0x7A9C          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_29        0x7A9D          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_30        0x7A9E          ///< LPC/eSPI Controller
#define V_ADL_PCH_S_LPC_CFG_DEVICE_ID_MB_31        0x7A9F          ///< LPC/eSPI Controller
#endif
