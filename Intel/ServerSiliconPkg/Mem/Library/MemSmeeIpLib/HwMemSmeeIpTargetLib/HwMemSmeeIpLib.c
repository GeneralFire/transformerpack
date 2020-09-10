/** @file
  HwMemSmeeIpLib.c

  Defines SMEE IP related functions for Hardware

  This file supports SMEE IP access through API functions for real Hardware environment.

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
#include <Uefi.h>
#include <Library/MemoryCoreLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiApi.h>
#include "Include/MemSmeeRegs.h"
#include <Library/UsraCsrLib.h>
#include <Library/SystemInfoLib.h>

/**
  Build Target function to wait for Tme Cmi Cpcfg to complete.

  @param[in]   Socket           Socket Number.
  @param[in]   Channel          Channel.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
WaitForTmeCmiCfgCompleteTarget (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  UINT64 StartCount;
  UINT8 Size;
  UINT8 *RegAddr;
  CSR_OFFSET RegOffset;
  TME_CMI_N0_SMEE_TME_STRUCT TmeCmiCfgRegValue;
  BOOLEAN PowerGood;

  if (GetEmulation () == SIMICS_FLAG) {

    if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE) {
      RegOffset.Data = TME_CMI_N0_SMEE_TME_REG;
      Size = (UINT8) RegOffset.Bits.size;
      RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, Channel, TME_CMI_N0_SMEE_TME_REG);
      PowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
      EmulationPolling (RegAddr, BIT21|BIT22, 1, PowerGood);
    }

    return EFI_SUCCESS;
  }

  StartCount = GetCount ();

  do {

    if (GetDelay (StartCount) > CATCHALL_TIMEOUT) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n InitCmiCreditProgramming: TmeCmiCfg initialize failed \n");
      RC_FATAL_ERROR (FALSE, ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);
      FatalError (ERR_CMI_FAILURE, ERR_CMI_INIT_FAILED);

      return EFI_TIMEOUT;
    }

    TmeCmiCfgRegValue.Data = MemReadPciCfgEp (Socket, Channel, TME_CMI_N0_SMEE_TME_REG);

  } while (!((TmeCmiCfgRegValue.Bits.rsp_ism_credits_initialized == 1) && 
             (TmeCmiCfgRegValue.Bits.req_ism_credits_initialized == 1)));

  return EFI_SUCCESS;
}
