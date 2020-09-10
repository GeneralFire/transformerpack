/** @file
  Cmi credit values. 
  This file has been generated Automatically using CmiCreditExtractorHBM.py, please avoid direct edit.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _MESH2MEM_CMI_PERFORMANCE_DATA_HBM_H_
#define _MESH2MEM_CMI_PERFORMANCE_DATA_HBM_H_

STATIC CMI_DEFAULT_REG_VALUE_HBM CmiRdCplCreditChnlConfig1ValueA0Hbm[] = {
  {  0,  32,  28, 0x03f}, // vc0_credits
  {  6,   0,   0, 0x03f}, // vc1_credits
  { 12,   0,   0, 0x03f}, // vc2_credits
  { 18,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0, 0x03f}, // shared_vc_credits
};


STATIC CMI_DEFAULT_REG_VALUE_HBM CmiRdCplCreditChnlConfig2ValueA0Hbm[] = {
  {  0,  32,  28, 0x03f}, // qdepth
  {  6,  10,  10, 0x00f}, // fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE_HBM Sysfeatures0A0Hbm[] = {
  { 17,   0,   4, 0x00f}, // NumEgrEntries4Cisgr
};


STATIC CMI_DEFAULT_REG_VALUE_HBM CmiRspCreditChnlConfigValueA0Hbm[] = {
  {  0, 127, 127, 0x07f}, // vc0_credits
  {  7,   0,   0, 0x07f}, // vc1_credits
  { 14,   0,   0, 0x07f}, // vc2_credits
  { 21,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4, 0x007}, // fab_credits
};


#endif // _MESH2MEM_CMI_PERFORMANCE_DATA_HBM_H
