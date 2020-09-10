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

#ifndef _MESH2MEM_CMI_PERFORMANCE_DATA_H_
#define _MESH2MEM_CMI_PERFORMANCE_DATA_H_

#define MESH2MEM_CMI_PERFORMANCE_DATA_VERSION_A0 6
#define MESH2MEM_CMI_PERFORMANCE_DATA_VERSION_B0 4
#define MESH2MEM_CMI_PERFORMANCE_DATA_VERSION_C0 2

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1ValueA0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_TmeA0[] = {
  {  0,   0,  30,  30,  30,  30,  30, 0x03f}, // vc0_credits
  {  6,   0,  30,   0,   0,  30,  30, 0x03f}, // vc1_credits
  { 12,  30,   0,   0,  30,   0,  30, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   2,   2,   2,   2,   2,   2, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_XtileA0[] = {
  {  0,   0,  28,  28,  28,  28,  28, 0x03f}, // vc0_credits
  {  6,   0,  28,   0,   0,  28,  28, 0x03f}, // vc1_credits
  { 12,  28,   0,   0,  28,   0,  28, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_Tme_XtileA0[] = {
  {  0,   0,  28,  28,  28,  28,  28, 0x03f}, // vc0_credits
  {  6,   0,  28,   0,   0,  28,  28, 0x03f}, // vc1_credits
  { 12,  28,   0,   0,  28,   0,  28, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_2LMA0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_Tme_2LMA0[] = {
  {  0,   0,  30,  30,  30,  30,  30, 0x03f}, // vc0_credits
  {  6,   0,  30,   0,   0,  30,  30, 0x03f}, // vc1_credits
  { 12,  30,   0,   0,  30,   0,  30, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   2,   2,   2,   2,   2,   2, 0x03f}, // shared_vc_credits
};


STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2ValueA0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_TmeA0[] = {
  {  0,  32,  30,  32,  30,  30,  30, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_XtileA0[] = {
  {  0,  28,  28,  28,  28,  28,  28, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_Tme_XtileA0[] = {
  {  0,  28,  28,  28,  28,  28,  28, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_2LMA0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_Tme_2LMA0[] = {
  {  0,  32,  30,  32,  30,  30,  30, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0A0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_TmeA0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_XtileA0[] = {
  { 17,   4,   4,   4,   4,   4,   4, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_Tme_XtileA0[] = {
  { 17,   4,   4,   4,   4,   4,   4, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_2LMA0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_Tme_2LMA0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};


STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValueA0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_TmeA0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_XtileA0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_Tme_XtileA0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_2LMA0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_Tme_2LMA0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1ValueB0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_TmeB0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_XtileB0[] = {
  {  0,   0,  28,  28,  28,  28,  28, 0x03f}, // vc0_credits
  {  6,   0,  28,   0,   0,  28,  28, 0x03f}, // vc1_credits
  { 12,  28,   0,   0,  28,   0,  28, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_Tme_XtileB0[] = {
  {  0,   0,  28,  28,  28,  28,  28, 0x03f}, // vc0_credits
  {  6,   0,  28,   0,   0,  28,  28, 0x03f}, // vc1_credits
  { 12,  28,   0,   0,  28,   0,  28, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_2LMB0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_Tme_2LMB0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};


STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2ValueB0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_TmeB0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_XtileB0[] = {
  {  0,  28,  28,  28,  28,  28,  28, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_Tme_XtileB0[] = {
  {  0,  28,  28,  28,  28,  28,  28, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_2LMB0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_Tme_2LMB0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0B0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_TmeB0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_XtileB0[] = {
  { 17,   4,   4,   4,   4,   4,   4, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_Tme_XtileB0[] = {
  { 17,   4,   4,   4,   4,   4,   4, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_2LMB0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_Tme_2LMB0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};


STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValueB0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_TmeB0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_XtileB0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_Tme_XtileB0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_2LMB0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_Tme_2LMB0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1ValueC0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_TmeC0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_XtileC0[] = {
  {  0,   0,  28,  28,  28,  28,  28, 0x03f}, // vc0_credits
  {  6,   0,  28,   0,   0,  28,  28, 0x03f}, // vc1_credits
  { 12,  28,   0,   0,  28,   0,  28, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_Tme_XtileC0[] = {
  {  0,   0,  28,  28,  28,  28,  28, 0x03f}, // vc0_credits
  {  6,   0,  28,   0,   0,  28,  28, 0x03f}, // vc1_credits
  { 12,  28,   0,   0,  28,   0,  28, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_2LMC0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig1Value_Tme_2LMC0[] = {
  {  0,   0,  32,  32,  32,  32,  32, 0x03f}, // vc0_credits
  {  6,   0,  32,   0,   0,  32,  32, 0x03f}, // vc1_credits
  { 12,  32,   0,   0,  32,   0,  32, 0x03f}, // vc2_credits
  { 18,   0,   0,   0,   0,   0,   0, 0x03f}, // vc3_credits
  { 24,   0,   0,   0,   0,   0,   0, 0x03f}, // shared_vc_credits
};


STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2ValueC0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_TmeC0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_XtileC0[] = {
  {  0,  28,  28,  28,  28,  28,  28, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_Tme_XtileC0[] = {
  {  0,  28,  28,  28,  28,  28,  28, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_2LMC0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRdCplCreditChnlConfig2Value_Tme_2LMC0[] = {
  {  0,  32,  32,  32,  32,  32,  32, 0x03f}, // qdepth
  {  6,  10,  10,  10,  10,  10,  10, 0x00f}, // fab_credits
};


STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0C0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_TmeC0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_XtileC0[] = {
  { 17,   4,   4,   4,   4,   4,   4, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_Tme_XtileC0[] = {
  { 17,   4,   4,   4,   4,   4,   4, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_2LMC0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};

STATIC CMI_DEFAULT_REG_VALUE Sysfeatures0_Tme_2LMC0[] = {
  { 17,   0,   0,   0,   0,   0,   0, 0x00f}, // NumEgrEntries4Cisgr
};


STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValueC0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_TmeC0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_XtileC0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_Tme_XtileC0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_2LMC0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};

STATIC CMI_DEFAULT_REG_VALUE CmiRspCreditChnlConfigValue_Tme_2LMC0[] = {
  {  0,   0, 127, 127, 127, 127, 127, 0x07f}, // vc0_credits
  {  7,   0, 127,   0,   0, 127, 127, 0x07f}, // vc1_credits
  { 14, 127,   0,   0, 127,   0, 127, 0x07f}, // vc2_credits
  { 21,   0,   0,   0,   0,   0,   0, 0x07f}, // vc3_credits
  { 28,   4,   4,   4,   4,   4,   4, 0x007}, // fab_credits
};


#endif // _MESH2MEM_CMI_PERFORMANCE_DATA_H
