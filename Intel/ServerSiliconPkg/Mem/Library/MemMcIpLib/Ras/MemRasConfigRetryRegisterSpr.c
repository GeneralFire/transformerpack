/** @file
  Implementation of retry register configuration of SPR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include "Include/MemMcRegs.h"

/**
  Configure Retry Register for Error logging

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
ConfigRetryRegister (
  IN    UINT8      Socket,
  IN    UINT8      ChOnSkt
)
{
  RETRY_RD_ERR_LOG_MCDDC_DP_STRUCT        RetryRdErrLog;
  RETRY_RD_ERR_SET2_LOG_MCDDC_DP_STRUCT   RetryRdErrLogSet2;
  RETRY_RD_ERR_SET3_LOG_MCDDC_DP_STRUCT   RetryRdErrLogSet3;

  //
  //Enable sparing/patrol scrub errors in the RETRY_RD_ERROR_LOG CSR
  //
  RetryRdErrLog.Data = ReadCpuCsr (Socket, ChOnSkt, RETRY_RD_ERR_LOG_MCDDC_DP_REG);
  RetryRdErrLog.Bits.en = 1;
  RetryRdErrLog.Bits.en_patspr = 1;
  RetryRdErrLog.Bits.noover = 0;
  WriteCpuCsr (Socket, ChOnSkt, RETRY_RD_ERR_LOG_MCDDC_DP_REG, RetryRdErrLog.Data);


  //
  //Enable ECC errors in the RETRY_RD_ERR_SET2_LOG CSR
  //
  RetryRdErrLogSet2.Data = ReadCpuCsr (Socket, ChOnSkt, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG);
  RetryRdErrLogSet2.Bits.en = 1;
  RetryRdErrLogSet2.Bits.en_patspr = 0;
  RetryRdErrLogSet2.Bits.noover = 0;
  WriteCpuCsr (Socket, ChOnSkt, RETRY_RD_ERR_SET2_LOG_MCDDC_DP_REG, RetryRdErrLogSet2.Data);

  //
  //Disable the third set of retry_rd_err
  //
  RetryRdErrLogSet3.Data = ReadCpuCsr (Socket, ChOnSkt, RETRY_RD_ERR_SET3_LOG_MCDDC_DP_REG);
  RetryRdErrLogSet3.Bits.en = 0;
  WriteCpuCsr (Socket, ChOnSkt, RETRY_RD_ERR_SET3_LOG_MCDDC_DP_REG, RetryRdErrLogSet3.Data);
}
