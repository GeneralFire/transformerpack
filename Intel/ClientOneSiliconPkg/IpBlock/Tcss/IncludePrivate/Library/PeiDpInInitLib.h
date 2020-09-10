/** @file
  Header file for DPIN Init Lib in PEI.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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
#ifndef _DPIN_INIT_LIB_H_
#define _DPIN_INIT_LIB_H_

#include <DpInPreMemConfig.h>
#include <Ppi/SiPolicy.h>

/**
  This function performs basic initialization for DPIN in PEI phase after Policy produced at Pre-Mem phase.

  @param[in]  SiPreMemPolicyPpi              Pointer to Si PreMem Policy Ppi
**/
VOID
EFIAPI
DpInPreMemInit (
  IN  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

#endif
