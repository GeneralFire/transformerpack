/** @file
  Cmi credit values. 
  This file has been generated Automatically using CmiCreditExtractorV2.py, please avoid direct edit.

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

#ifndef _MEMMC_CMI_PERFORMANCE_DATA_H_
#define _MEMMC_CMI_PERFORMANCE_DATA_H_

#define MEMMC_CMI_PERFORMANCE_DATA_VERSION_A0 6
#define MEMMC_CMI_PERFORMANCE_DATA_VERSION_B0 4
#define MEMMC_CMI_PERFORMANCE_DATA_VERSION_C0 2

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValueA0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_TmeA0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_XtileA0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_Tme_XtileA0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_2LMA0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_Tme_2LMA0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValueA0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  58,  52,  58,  52,  52,  46, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_TmeA0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_XtileA0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  50,  44,  50,  44,  44,  38, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_Tme_XtileA0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_2LMA0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  58,  52,  58,  52,  52,  46, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_Tme_2LMA0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};


STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0A0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_TmeA0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_XtileA0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_Tme_XtileA0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_2LMA0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_Tme_2LMA0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};


STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1A0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_TmeA0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_XtileA0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_Tme_XtileA0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_2LMA0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_Tme_2LMA0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0ValueA0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_TmeA0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_XtileA0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_Tme_XtileA0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_2LMA0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_Tme_2LMA0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1ValueA0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_TmeA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_XtileA0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_Tme_XtileA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_2LMA0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_Tme_2LMA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfgA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_TmeA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_XtileA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_Tme_XtileA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_2LMA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_Tme_2LMA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0A0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_TmeA0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_XtileA0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_Tme_XtileA0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_2LMA0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_Tme_2LMA0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};


STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1A0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_TmeA0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_XtileA0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_Tme_XtileA0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_2LMA0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_Tme_2LMA0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};


STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValueB0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_TmeB0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_XtileB0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_Tme_XtileB0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_2LMB0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_Tme_2LMB0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValueB0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  58,  52,  58,  52,  52,  46, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_TmeB0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_XtileB0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  50,  44,  50,  44,  44,  38, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_Tme_XtileB0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_2LMB0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  58,  52,  58,  52,  52,  46, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_Tme_2LMB0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};


STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0B0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_TmeB0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_XtileB0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_Tme_XtileB0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_2LMB0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_Tme_2LMB0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};


STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1B0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_TmeB0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_XtileB0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_Tme_XtileB0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_2LMB0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_Tme_2LMB0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0ValueB0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_TmeB0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_XtileB0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_Tme_XtileB0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_2LMB0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_Tme_2LMB0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1ValueB0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_TmeB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_XtileB0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_Tme_XtileB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_2LMB0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_Tme_2LMB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfgB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_TmeB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_XtileB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_Tme_XtileB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_2LMB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_Tme_2LMB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0B0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_TmeB0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_XtileB0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_Tme_XtileB0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_2LMB0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_Tme_2LMB0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};


STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1B0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_TmeB0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_XtileB0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_Tme_XtileB0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_2LMB0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_Tme_2LMB0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};


STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValueC0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_TmeC0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_XtileC0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_Tme_XtileC0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_2LMC0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McMiscCmiCreditConfigValue_Tme_2LMC0[] = {
  {  0,   8,   8,   8,   8,   8,   8, 0x01f}, // req_fab_credits
  {  5,  12,  12,  12,  12,  12,  12, 0x01f}, // rsp_fab_credits
  { 10,  18,  18,  18,  18,  18,  18, 0x01f}, // rd_cpl_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValueC0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  58,  52,  58,  52,  52,  46, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_TmeC0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_XtileC0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  50,  44,  50,  44,  44,  38, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_Tme_XtileC0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_2LMC0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  58,  52,  58,  52,  52,  46, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};

STATIC CMI_DEFAULT_REG_VALUE McDpCmiQdepthCreditConfigValue_Tme_2LMC0[] = {
  {  0,   0,   6,   6,   6,   6,   6, 0x00f}, // vc0_rd_cpl_credits
  {  4,   0,   6,   0,   0,   6,   6, 0x00f}, // vc1_rd_cpl_credits
  {  8,   6,   0,   0,   6,   0,   6, 0x00f}, // vc2_rd_cpl_credits
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_credits
  { 16,  24,  18,  24,  18,  18,  12, 0x1ff}, // rd_cpl_shared_credits
  { 25,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_vec
};


STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0C0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_TmeC0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_XtileC0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_Tme_XtileC0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_2LMC0[] = {
  {  0,   0,  16,  16,  16,  16,  16, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   7,   7,   7,   7,   7, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   3,   0,   0,   3,   0,   3, 0x003}, // wr_vc2_floor
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue0_Tme_2LMC0[] = {
  {  0,   0,  18,  18,  18,  18,  18, 0x01f}, // rd_vc0_floor
  {  5,   0,   2,   0,   0,   2,   2, 0x01f}, // rd_vc1_floor
  { 10,  16,   0,   0,  16,   0,  16, 0x01f}, // rd_vc2_floor
  { 15,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_vc3_floor
  { 20,   0,   9,   9,   9,   9,   9, 0x01f}, // wr_vc0_floor
  { 25,   0,   2,   0,   0,   2,   2, 0x01f}, // wr_vc1_floor
  { 30,   1,   0,   0,   1,   0,   1, 0x003}, // wr_vc2_floor
};


STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1C0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_TmeC0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_XtileC0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_Tme_XtileC0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_2LMC0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE McCmiReqCpcfgConfigValue1_Tme_2LMC0[] = {
  {  0,   2,   0,   0,   2,   0,   2, 0x007}, // wr_vc2_floor
  {  3,   0,   0,   0,   0,   0,   0, 0x01f}, // wr_vc3_floor
  {  8,   6,   6,   6,   6,   6,   6, 0x01f}, // req_fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0ValueC0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  30,  32,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  30,  32,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_TmeC0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  28,  30,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  28,  30,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_XtileC0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_Tme_XtileC0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_2LMC0[] = {
  {  0,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  14,  16,  16,  14,  14, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  22,  24,  24,  22,  22, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN0Value_Tme_2LMC0[] = {
  {  0,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_0_shared_cnt
  {  5,   0,  12,  14,  14,  12,  12, 0x01f}, // rpq4_1_shared_cnt
  { 10,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_0_shared_cnt
  { 16,   0,  20,  22,  22,  20,  20, 0x03f}, // wpq4_1_shared_cnt
  { 22,  64,   0,   0,  64,   0,  64, 0x07f}, // rpqt_shared_cnt
  { 29,   7,   0,   0,   7,   0,   7, 0x007}, // wpqt_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1ValueC0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_TmeC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_XtileC0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_Tme_XtileC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_2LMC0[] = {
  {  0,   1,   0,   0,   1,   0,   1, 0x007}, // wpqt_shared_cnt
  {  9, 135, 133, 135, 112, 133, 110, 0x0ff}, // sb_shared_cnt
  { 17,   0,   7,   7,   7,   7,   7, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   2,   0,   0,   2,   0,   2, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE McCmiCreditCfgN1Value_Tme_2LMC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x007}, // wpqt_shared_cnt
  {  9, 133, 129, 131, 106, 129, 104, 0x0ff}, // sb_shared_cnt
  { 17,   0,   6,   6,   6,   6,   6, 0x007}, // early_rsp_fifo_0_shared_cnt
  { 20,   0,   1,   0,   0,   1,   1, 0x007}, // early_rsp_fifo_1_shared_cnt
  { 23,   0,   0,   0,   0,   0,   0, 0x007}, // early_rsp_fifo_2_shared_cnt
  { 26,   0,   5,   0,   5,   5,   5, 0x007}, // early_rsp_fifo_3_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfgC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_TmeC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_XtileC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_Tme_XtileC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_2LMC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5,   0,   0,   0,   0,   0,   0, 0x0ff}, // twk_buffer_shared_cnt
};

STATIC CMI_DEFAULT_REG_VALUE Mc_tme_cfg_Tme_2LMC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // ptl_wpq_shared_cnt
  {  5, 144, 140, 142, 126, 140, 124, 0x0ff}, // twk_buffer_shared_cnt
};


STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0C0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_TmeC0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_XtileC0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_Tme_XtileC0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_2LMC0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n0_Tme_2LMC0[] = {
  {  0, 588, 588, 588, 588, 588, 588, 0x1fff}, // vc0_struct_credit_map
  { 13, 1036, 1036, 1036, 1036, 1036, 1036, 0x1fff}, // vc1_struct_credit_map
  { 26,  32,  32,  32,  32,  32,  32, 0x03f}, // vc2_struct_credit_map
};


STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1C0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_TmeC0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_XtileC0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_Tme_XtileC0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_2LMC0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};

STATIC CMI_DEFAULT_REG_VALUE Mccmistructwrcreditcfg_n1_Tme_2LMC0[] = {
  {  0,  33,  33,  33,  33,  33,  33, 0x07f}, // vc2_struct_credit_map
  {  7, 4108, 4108, 4108, 4108, 4108, 4108, 0x1fff}, // vc3_struct_credit_map
};


#endif // _MEMMC_CMI_PERFORMANCE_DATA_H
