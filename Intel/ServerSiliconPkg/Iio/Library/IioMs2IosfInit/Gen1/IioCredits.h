/** @file

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

#ifndef _MS2IOSF_CREDITS_H_
#define _MS2IOSF_CREDITS_H_

#include "Include/IioCreditsCommon.h"

#define ITC_NUMBER_ENTRIES          28
#define OTC_NUMBER_ENTRIES          24

#define MAX_X4_BIFURCATION_TYPE     5

typedef struct {
  UINT8             Bifurcation;
  CREDITS_ENTRY    *Pcie4Table;
  CREDITS_ENTRY    *Pcie3Table;
} SINGLE_PCIE_CREDIT_TABLE;

CREDITS_ENTRY*
GetItcCreditSinglePcieTable (
  IN UINT8         Bifurcation,
  IN IIO_PORT_GEN  PcieGen
  );

CREDITS_ENTRY*
GetOtcCreditSinglePcieTable (
  IN UINT8         Bifurcation,
  IN IIO_PORT_GEN  PcieGen
  );

#endif // _MS2IOSF_CREDITS_H_
