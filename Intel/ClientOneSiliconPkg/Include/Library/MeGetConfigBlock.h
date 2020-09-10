/** @file
  This is a library for ME ConfigBlock functionality.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _ME_GET_CONFIG_BLOCK_H_
#define _ME_GET_CONFIG_BLOCK_H_

#include <Base.h>
#include <PiPei.h>

#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <AmtConfig.h>
#include <MePeiConfig.h>
#include <SpsPeiConfig.h>


/**
  Get ME PEI Config Block

  @retval ME_PEI_CONFIG *
**/
ME_PEI_CONFIG *
GetMePeiConfigBlock (
  IN SI_POLICY_PPI  *SiPolicy
  )
;

/**
  Get AMT PEI Config Block

  @param[in] SiPolicy      The Silicon Policy PPI instance

  @retval AMT_PEI_CONFIG *
**/
AMT_PEI_CONFIG *
GetAmtPeiConfigBlock (
  IN SI_POLICY_PPI  *SiPolicy
  )
;

/**
  Get SPS PEI Config Block

  @param[in] SiPreMemPolicy The Silicon Policy PPI instance

  @retval SPS_PEI_CONFIG *
**/
SPS_PEI_CONFIG *
GetSpsPreMemPeiConfigBlock (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicy
  )
;

/**
  Get ME PreMem Config Block

  @param[in] SiPreMemPolicy The Silicon Policy PPI instance

  @retval ME_PEI_PREMEM_CONFIG *
**/
ME_PEI_PREMEM_CONFIG *
GetMePreMemConfigBlock (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicy
  )
;

#endif _ME_GET_CONFIG_BLOCK_H_
