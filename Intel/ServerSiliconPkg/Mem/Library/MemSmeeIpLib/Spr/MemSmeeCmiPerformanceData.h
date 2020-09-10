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

#ifndef _MEMSMEE_CMI_PERFORMANCE_DATA_H_
#define _MEMSMEE_CMI_PERFORMANCE_DATA_H_

#define MEMSMEE_CMI_PERFORMANCE_DATA_VERSION_A0 6
#define MEMSMEE_CMI_PERFORMANCE_DATA_VERSION_B0 4
#define MEMSMEE_CMI_PERFORMANCE_DATA_VERSION_C0 2

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValueA0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  14,  14,  14,  14,  14,  14, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_TmeA0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  14,  14,  14,  14,  14,  14, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_XtileA0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  14,  14,  14,  14,  14,  14, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_Tme_XtileA0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  14,  14,  14,  14,  14,  14, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_2LMA0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  14,  14,  14,  14,  14,  14, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_Tme_2LMA0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  14,  14,  14,  14,  14,  14, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0A0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_TmeA0[] = {
  {  0,  60,  52,  60,  52,  52,  48, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_XtileA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_Tme_XtileA0[] = {
  {  0,  52,  48,  52,  48,  48,  44, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_2LMA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_Tme_2LMA0[] = {
  {  0,  60,  52,  60,  52,  52,  48, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1A0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_TmeA0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_XtileA0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_Tme_XtileA0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_2LMA0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_Tme_2LMA0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};


STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValueA0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_TmeA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_XtileA0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_Tme_XtileA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_2LMA0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_Tme_2LMA0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValueA0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_TmeA0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_XtileA0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_Tme_XtileA0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_2LMA0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_Tme_2LMA0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValueB0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_TmeB0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_XtileB0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_Tme_XtileB0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_2LMB0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_Tme_2LMB0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0B0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_TmeB0[] = {
  {  0,  60,  56,  60,  56,  56,  52, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_XtileB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_Tme_XtileB0[] = {
  {  0,  52,  48,  52,  48,  48,  44, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_2LMB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_Tme_2LMB0[] = {
  {  0,  60,  56,  60,  56,  56,  52, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1B0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_TmeB0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_XtileB0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_Tme_XtileB0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_2LMB0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_Tme_2LMB0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};


STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValueB0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_TmeB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_XtileB0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_Tme_XtileB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_2LMB0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_Tme_2LMB0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValueB0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_TmeB0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_XtileB0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_Tme_XtileB0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_2LMB0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_Tme_2LMB0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValueC0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_TmeC0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_XtileC0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_Tme_XtileC0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_2LMC0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiCfgValue_Tme_2LMC0[] = {
  {  0,   9,   9,   9,   9,   9,   9, 0x01f}, // req_fab_credits_in
  {  5,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_credits_in
  { 10,  15,  15,  15,  15,  15,  15, 0x03f}, // rd_cpl_fab_input_credits
  { 16,   8,   8,   8,   8,   8,   8, 0x01f}, // rsp_fab_input_credits
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0C0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_TmeC0[] = {
  {  0,  60,  56,  60,  56,  56,  52, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_XtileC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_Tme_XtileC0[] = {
  {  0,  52,  48,  52,  48,  48,  44, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_2LMC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   0,   0,   0,   0,   0,   0, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   0,   0,   0,   0,   0, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   0,   0,   0,   0,   0, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   0,   0,   0,   0,   0,   0, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,   2,   2,   2,   2,   2,   2, 0x00f}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue0_Tme_2LMC0[] = {
  {  0,  60,  56,  60,  56,  56,  52, 0x0ff}, // rd_cpl_qdepth_shared_credits
  {  8,   4,  11,   1,  13,  11,  15, 0x00f}, // rd_cpl_qdepth_vc_valid_vec
  { 12,   0,   4,   4,   4,   4,   4, 0x00f}, // vc0_rd_cpl_qdepth_credits
  { 16,   0,   4,   0,   0,   4,   4, 0x00f}, // vc1_rd_cpl_qdepth_credits
  { 20,   4,   0,   0,   4,   0,   4, 0x00f}, // vc2_rd_cpl_qdepth_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x00f}, // vc3_rd_cpl_qdepth_credits
  { 28,  14,  14,  14,  14,  14,  14, 0x00f}, // rd_cpl_qdepth_to_peer
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1C0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_TmeC0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_XtileC0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_Tme_XtileC0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_2LMC0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCmiQdepthValue1_Tme_2LMC0[] = {
  {  0,   1,   1,   1,   1,   1,   1, 0x003}, // rd_cpl_qdepth_to_peer
};


STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValueC0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_TmeC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_XtileC0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_Tme_XtileC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_2LMC0[] = {
  {  0,  12,  12,  12,  12,  12,  12, 0x01f}, // rdb_shared_cnt
  {  5,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc0_floor
  { 10,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc1_floor
  { 15,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc2_floor
  { 20,   5,   5,   5,   5,   5,   5, 0x01f}, // rd_cpl_vc3_floor
};

STATIC CMI_DEFAULT_REG_VALUE McTmeRdbCfgValue_Tme_2LMC0[] = {
  {  0,   0,   0,   0,   0,   0,   0, 0x01f}, // rdb_shared_cnt
  {  5,   0,  30,  30,  30,  30,  30, 0x01f}, // rd_cpl_vc0_floor
  { 10,   0,  30,   0,   0,  30,  30, 0x01f}, // rd_cpl_vc1_floor
  { 15,  30,   0,   0,  30,   0,  30, 0x01f}, // rd_cpl_vc2_floor
  { 20,   0,   0,   0,   0,   0,   0, 0x01f}, // rd_cpl_vc3_floor
};


STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValueC0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_TmeC0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_XtileC0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_Tme_XtileC0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_2LMC0[] = {
  { 10,   1,   1,   1,   1,   1,   1, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   1,   1,   1,   1,   1,   1, 0x001}, // credit_over_sub_error_enable
};

STATIC CMI_DEFAULT_REG_VALUE McTmeCfgValue_Tme_2LMC0[] = {
  { 10,   0,   0,   0,   0,   0,   0, 0x001}, // use_rd_cpl_fab_buffer
  { 13,   0,   0,   0,   0,   0,   0, 0x001}, // credit_over_sub_error_enable
};


#endif // _MEMSMEE_CMI_PERFORMANCE_DATA_H
