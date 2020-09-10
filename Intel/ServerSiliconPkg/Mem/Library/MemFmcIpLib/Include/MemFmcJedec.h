/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#define DDRT_CAS_LATENCY_OFFSET       22
#define DDRT_CWL_LATENCY_OFFSET       20
#define DDRT_CAS_LATENCY_RL_STEP_SIZE  2
#define DDRT_CWL_LATENCY_RL_STEP_SIZE  2

//
// endqsodtparkmode
//
#define PARK_OFF                     0
#define PARK_DIFF_LOW                1
#define PARK_DIFF_LOW_TxAnalogEn     2
#define PARK_BOTH_LOW                3

#define MAX_DCS_VREF_PERCENTAGE      9750
#define VREF_STEP_OF_0P5PERCENTAGE   50
#define ONE_HUNDRED_PERCENTAGE_VREF  10000
#define CWV_DCS_VREF_MAX_ENCODING    192
#define CWV_DCA_VREF_MAX_ENCODING    192
#define CWV_DCA_VREF_MIN_VALUE       00
#define CWV_DCA_VREF_MAX_VALUE       191

typedef struct CwvMrwSideBand {
  UINT16 CrowValleyMraAddress;
  UINT32 CrowValleyCsr;
} MRW_SIDEBAND_TABLE;

typedef struct MraRemapTable {
  UINT16 JedecMraAddress;
  UINT8  Cw;
  UINT16 CrowValleyMraAddress;
} MRA_REMAP_TABLE;

#define MAX_MRA_REMAP 0xE

typedef struct SnoopMapTable {
  UINT8 DramAddress;
  UINT8 DbAddress;
} SNOOP_MAP_TABLE;

#define MAX_SNOOP_SETTING 8

/**
Decode RCD programming into percentage of VDDQ

@param[in] Data        - encoding of DCS Vref


@retval percentage of DCS VDDQ

**/
UINT32
DecodeRcdVref(
  IN UINT32 Data
);

/**
Eecode RCD programming into percentage of VDDQ

  @param[in] Data        - encoding of Vref

  @retval percentage of Vref VDDQ

**/
UINT32
EncodeRcdVref (
  IN UINT32 Data
);

