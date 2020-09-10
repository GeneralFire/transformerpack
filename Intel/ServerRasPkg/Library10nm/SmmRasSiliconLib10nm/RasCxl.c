/** @file
  Implementation of CXL ip

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

#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Upi/KtiSi.h>

/**
  Enable Posion for CXL.

  @param[in] Skt      - The socket to enable

  @retval None

**/
VOID
EFIAPI
EnableCxlPosionPerSkt (
  IN        UINT8   Skt
  )
{
  UINT8                                                         StackId;
  ERROR_CAPABILITIES_AND_CONTROL_REGISTER_IIO_CXL_CM_STRUCT     ErrorControlRegister;

  for (StackId = 0; StackId < MAX_CXL_PER_SOCKET; StackId++) {
    if (KTI_HOST_OUT_PTR->CxlInfo[Skt][StackId].CxlStatus != AlreadyInCxlMode) {
      continue;
    }
    ErrorControlRegister.Data = ReadCpuCsr (Skt, StackId, ERROR_CAPABILITIES_AND_CONTROL_REGISTER_IIO_CXL_CM_REG);
    ErrorControlRegister.Bits.poison_enabled = 1;
    WriteCpuCsr (Skt, StackId, ERROR_CAPABILITIES_AND_CONTROL_REGISTER_IIO_CXL_CM_REG, ErrorControlRegister.Data);
  }
}