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

#ifndef _MEMMC_CMI_PERFORMANCE_DATA_HBM_H_
#define _MEMMC_CMI_PERFORMANCE_DATA_HBM_H_

STATIC CMI_DEFAULT_REG_VALUE_HBM McMiscCmiCreditConfigValueA0Hbm[] = {
  {  0,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18, 0x01f}, // rd_cpl_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE_HBM McDpCmiQdepthCreditConfigValueA0Hbm[] = {
  {  0,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   0, 0x00f}, // vc1_rd_cpl_credits
  {  8,   0,   0, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  58,  50, 0x1ff}, // rd_cpl_shared_credits
  { 25,   1,   1, 0x00f}, // rd_cpl_qdepth_vc_vec
};


STATIC CMI_DEFAULT_REG_VALUE_HBM McCmiReqCpcfgConfigValue0A0Hbm[] = {
  {  0,  21,  21, 0x01f}, // rd_vc0_floor
  {  5,   0,   0, 0x01f}, // rd_vc1_floor
  { 10,   0,   0, 0x01f}, // rd_vc2_floor
  { 15,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,  12,  12, 0x01f}, // wr_vc0_floor
  { 25,   0,   0, 0x01f}, // wr_vc1_floor
  { 30,   0,   0, 0x003}, // wr_vc2_floor
};


STATIC CMI_DEFAULT_REG_VALUE_HBM McCmiReqCpcfgConfigValue1A0Hbm[] = {
  {  0,   0,   0, 0x007}, // wr_vc2_floor
  {  3,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,  12,  12, 0x01f}, // req_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE_HBM McCmiCreditCfgN0ValueA0Hbm[] = {
  {  0,  11,  11, 0x01f}, // rpq4_0_shared_cnt
  {  5,  11,  11, 0x01f}, // rpq4_1_shared_cnt
  { 10,  19,  19, 0x03f}, // wpq4_0_shared_cnt
  { 16,  19,  19, 0x03f}, // wpq4_1_shared_cnt
  { 22,   0,   0, 0x07f}, // rpqt_shared_cnt
  { 29,   0,   0, 0x007}, // wpqt_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE_HBM McCmiCreditCfgN1ValueA0Hbm[] = {
  {  0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 125, 125, 0x0ff}, // sb_shared_cnt
  { 17,   0,   0, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   0, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   0, 0x007}, // early_rsp_fifo_3_shared_cnt
};


#endif // _MEMMC_CMI_PERFORMANCE_DATA_HBM_H
