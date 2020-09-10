/** @file
  Purley setting file define SKX, ICX, KNH CPU basic parameters.
  This file trend to be updated per silicon setting.
  This file purpose is to decomple.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef __SILICON_SETTING_H__
#define __SILICON_SETTING_H__


#define NODE_TO_SKT(node)       (node / MAX_IMC)
#define NODE_TO_MC(node)        (node % MAX_IMC)
#define NODECHA_TO_SKTCHA(node, ch)    (((node % MAX_IMC) * MAX_MC_CH) + ch)
#define SKTCH_TO_NODECH(ChOnSkt)     (ChOnSkt % MAX_MC_CH)
#define SKT_CH_TO_NODE(Skt,ChOnSkt)   (Skt * MAX_IMC + ChOnSkt / MAX_MC_CH)
#define SKTMC_TO_NODE(socket, mc)  ((socket * MAX_IMC) | (mc % MAX_IMC))
#define CHRANK_TO_DIMMRANK(Rank)   (Rank % 4)
#define CHRANK_TO_DIMM(Rank)       (Rank / 4)

#define MAX_SPARE_RANK      2
#define MAX_DIMM            2
#define TAD_RULES           8
#define IMC_MAX_CH              ((MAX_IMC) * (MAX_MC_CH))
#define MC_MAX_NODE         (MAX_SOCKET * MAX_IMC)


#define FULL_MIRROR_1LM         BIT0
#define FULL_MIRROR_2LM         BIT1
#define PARTIAL_MIRROR_1LM      BIT5
#define PARTIAL_MIRROR_2LM      BIT6
#define RK_SPARE                BIT3

#define SDDC_EN                 BIT0
#define ADDDC_EN                BIT6

//
// PPR Status, copy from MemHost.h
//
#define PPR_STS_SUCCESS         0x00
#define PPR_STS_ADDR_VALID      0x01
#define PPR_STS_FAILED          0x02

#define M2M0_BANK   7

#endif // __SILICON_SETTING_H__
