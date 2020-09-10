/** @file
  CmiMcRegs.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _CMI_MC_REG_STRUCT_H_
#define _CMI_MC_REG_STRUCT_H_

//
// Maximum WPQ thershold to break CMI deadlock
//
#define MAX_DEPRD_WPQ_THERSHOLD   0x1F


STATIC const UINT32 McMiscCmiCreditConfigRegAddr[] = {
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_REG,
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_REG + 4,
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_REG + 8
};

#pragma pack(1)
typedef struct {
  MCMISC_CMI_CREDIT_CONFIG_0_MC_GLOBAL_STRUCT  McMiscCmiCreditConfig[MAX_MC_CH];
  MCCMIREQCPCFG_N1_MC_2LM_STRUCT McCmiReqCpcfg;
  MCSTRUCTCMICREDITCFG_N0_MC_2LM_STRUCT McCmiCreditCfgN0;
  MCSTRUCTCMICREDITCFG_N1_MC_2LM_STRUCT McCmiCreditCfgN1;
} CMI_MAIN_REG_INFO;
#pragma pack()

#endif //_CMI_MC_REG_STRUCT_H_
