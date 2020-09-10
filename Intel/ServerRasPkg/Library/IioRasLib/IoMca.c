/** @file
  Implementation IoMca RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include "IioRasLibInternal.h"


/**
  This function  Enable IoMca.

  @param[in] Buffer        The pointer to private data buffer.

  @retval    Status.
**/
VOID
EFIAPI
EnableIoMca (
  IN      VOID  *Buffer
  )
{
  SPR_MSR_UBOX_MC_CTL_REGISTER  Mc6CtlReg;
  Mc6CtlReg.Uint64 = AsmReadMsr64 (SPR_MSR_UBOX_MC_CTL);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioStackEnableIoMca!\n"));

  RAS_DEBUG ((LEVEL_REG, "IioStackEnableIoMca, SPR_MSR_UBOX_MC_CTL: %x  \n", Mc6CtlReg.Uint64));

  Mc6CtlReg.Bits.IosfFatalmcaen = 1;
  Mc6CtlReg.Bits.IosfNonfatalmcaen = 1;

  AsmWriteMsr64 (SPR_MSR_UBOX_MC_CTL, Mc6CtlReg.Uint64);
}

/**
  This function  Enable IoMca.

  @param[in] Buffer        The pointer to private data buffer.

  @retval    Status.
**/
VOID
EFIAPI
IioCpuEnableIoMca (
  IN      VOID
  )
{
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, EnableIoMca, NULL);
}

/**
  This function enable Disable IoMca.

  @param[in] Buffer        The pointer to private data buffer.

  @retval    Status.
**/
VOID
EFIAPI
DisableIoMca (
  IN      VOID    *Buffer
  )
{
  SPR_MSR_UBOX_MC_CTL_REGISTER  Mc6CtlReg;
  Mc6CtlReg.Uint64 = AsmReadMsr64 (SPR_MSR_UBOX_MC_CTL);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "DisableIoMca!\n"));

  RAS_DEBUG ((LEVEL_REG, "DisableIoMca, MSR_MC06_CTL: %x  \n", Mc6CtlReg.Uint64));

  Mc6CtlReg.Bits.IosfFatalmcaen = 0;
  Mc6CtlReg.Bits.IosfNonfatalmcaen = 0;

  AsmWriteMsr64 (SPR_MSR_UBOX_MC_CTL, Mc6CtlReg.Uint64);
}

/**
  This function Disable IoMca.

  @param[in] Buffer        The pointer to private data buffer.

  @retval    Status.
**/
VOID
EFIAPI
IioCpuDisableIoMca (
  VOID
  )
{
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, DisableIoMca, NULL);
}




