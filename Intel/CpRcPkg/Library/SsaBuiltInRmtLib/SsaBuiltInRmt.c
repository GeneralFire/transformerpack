/** @file
  SsaBuiltInRmt.c

  Library implementation of API to support SSA BuiltIn Rmt function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <Base.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Ppi/SsaBiosServicesPpi.h>
#include <Library/KtiApi.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/MemFmcIpLib.h>

/**

Check boot parameters before executing SSA RMT

@param Host  - Pointer to sysHost

@retval TRUE/FALSE

**/
BOOLEAN
CheckSSARMTParameters (
  PSYSHOST Host
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast ||
      (Host->var.mem.subBootMode == ColdBootFast && !Setup->mem.enableBiosSsaRMTonFCB)) {
    return TRUE; //We will skip SSA RMT if TRUE
  } else {
    return FALSE;
  }
} //CheckSSARMTParameters
/**
  Initialize the function pointers, pass them to stitching EV
  test content

  @param[in] Host - Pointer to sysHost

  @retval N/A
**/
VOID
SsaStitchContent (
  IN PSYSHOST Host
  )
{
  SSA_BIOS_SERVICES_PPI    *SsaBiosServicesPpi = NULL;
  EFI_STATUS               Status = EFI_SUCCESS;
  EFI_PEI_SERVICES         **PeiServices;
  UINT8                    Socket = 0;
  SYS_SETUP               *Setup = NULL;
  BOOLEAN                 SsaRmtRecvEnableDenormalize = FALSE;
  BOOLEAN                 TrainingResultOffsetFunctionEnable = FALSE;

  Setup = GetSysSetupPointer ();
  ASSERT (Setup != NULL);

  if (Setup != NULL) {
    SsaRmtRecvEnableDenormalize = Setup->mem.dfxMemSetup.DfxSsaRmtRecvEnableDenormalize;

    if (Setup->mem.trainingResultOffsetFunctionEnable == 1) {
      TrainingResultOffsetFunctionEnable = TRUE;
    }
  }

  PeiServices = (EFI_PEI_SERVICES **) (UINTN) Host->var.common.oemVariable;

  ASSERT (PeiServices != NULL);
  if (PeiServices == NULL) {
    return;
  }

  // Locate the EFI read only variable PPI
  Status = (*PeiServices)->LocatePpi (PeiServices, &gSsaBiosServicesPpiGuid, 0, NULL, &SsaBiosServicesPpi);
  if (EFI_ERROR(Status)) {
    return;
  }

  if (SsaRmtRecvEnableDenormalize) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if (!SocketPresent (Socket)) {
        continue;
      }
      DenormalizeRecvEnable (Socket);
    }
  }

  // Disable RID feedback for NVMDIMM
  ConfigNvmdimmtRidFeedback (Host, 1);
  SsaStichedModeRMTChip (SsaBiosServicesPpi);

  if (SsaRmtRecvEnableDenormalize) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if (!SocketPresent (Socket)) {
        continue;
      }
      RenormalizeRecvEnable (Socket);
    }
  }
  if (TrainingResultOffsetFunctionEnable) {
    RcDebugPrint (SDBG_MAX, "\nReapply RecEnOffset after SSA RMT...\n");
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if (!SocketPresent (Socket)) {
        continue;
      }
      TrainingResultRecEnOffsetCfg (Host, Socket);
      DisplayResults (Host, Socket, RecEnDelay);
    }
    RcDebugPrint (SDBG_MAX, "\nShowing RecEnDelay settings after Applying TrainingOffset...\n");
  }
}


/**
  This function initializes built-in BSSA RMT and runs it.
  Can be called after each memory training for EV validatation

  @retval N/A
**/
VOID
EFIAPI
RunBuiltInBssaRmt (
  VOID
  )
{
  PSYSHOST                  Host;
  SYS_SETUP                 *Setup;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();
  if ((!IsMemFlowEnabled (SsaRankMarginTool)) || (Setup->mem.enableBiosSsaRMT == 0)
    || (CheckSSARMTParameters (Host) == TRUE)) {
    return;
  }
  if (PcdGetBool (PcdMrcSsaOemHook) == TRUE) {
    SsaStitchContent (Host);
  }
}