/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/MpExtensionLib.h>
#include <Library/RasDebugLib.h>

/**
  AP configure Shutdown Suppression feature routine.

  @param[in] ShutdownSuppression    The pointer to Shutdown Suppression setting.

  @retval None.
**/
VOID
EFIAPI
ApConfigureShutdownSuppression (
  IN UINT8  *ShutdownSuppression
  )
{
  ICX_MSR_SUPPRESS_SHUTDOWN_REGISTER  ShutdownSuppressionReg;
  ShutdownSuppressionReg.Uint64 = AsmReadMsr64 (ICX_MSR_SUPPRESS_SHUTDOWN);

  ShutdownSuppressionReg.Bits.SuppressShutdownAndLogmcaIerr = (*ShutdownSuppression) & BIT0;
  ShutdownSuppressionReg.Bits.ShutdownLogmcaIerr = ((*ShutdownSuppression) & BIT1) >> 1;

  AsmWriteMsr64 (ICX_MSR_SUPPRESS_SHUTDOWN, ShutdownSuppressionReg.Uint64);
}

/**
  Configure Shutdown Suppression feature.

  @param[in] ShutdownSuppression    Shutdown Suppression setting.

  @retval None

**/
VOID
EFIAPI
ConfigureShutdownSuppression (
  IN UINT8   ShutdownSuppression
)
{
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, FALSE, ApConfigureShutdownSuppression, &ShutdownSuppression);
}