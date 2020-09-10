/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#ifndef _MEM_PROJECT_SPECIFIC_H_
#define _MEM_PROJECT_SPECIFIC_H_

#include <Library/MemoryCoreLib.h>
#include <Memory/CpgcDefinitions.h>
#include <Library/CteNetLib.h>

//
// Place any project specific defines or structures here.
//
#define CHIP_TX_PI_SAMPLE_YAMDELAY 180*16

#define DDRT_GNT2ERID_MAX          0xFF
#define DDRT_GNT2ERID_MIN          0
#define MIN_CWL_ADJ   -3
#define MAX_CWL_ADJ    7

//SKX change
#define TX_RON_28 28
#define TX_RON_30 30
#define TX_RON_33 33

//
// TxEq tap1 and tap2 defines
//
#define MAX_RCOMP_CODE          64
#define NUM_TXEQTAP1_SETTINGS   10
#define NUM_TXEQTAP2_SETTINGS   7
#define TAP2_IDXOFFSET          (NUM_TXEQTAP2_SETTINGS - 1)
#define TXEQ_FACTOR1            128
#define TXEQ_FACTOR2            3

typedef struct _CHIP_CLEAR_PARITY_RESULTS_STRUCT {
  DUMMY_REG                             tCrap;
  DUMMY_REG                             errSignals;
} CHIP_CLEAR_PARITY_RESULTS_STRUCT, *PCHIP_CLEAR_PARITY_RESULTS_STRUCT;

#define CHIP_GET_REC_ENABLE_MAX_STROBE  MAX_STROBE

#endif // ifndef _MEM_PROJECT_SPECIFIC_H_
