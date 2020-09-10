/** @file
  This file initialize the IIO DFX

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <Library/CpuAndRevisionLib.h>

#include <IioPlatformData.h>
#include <RcRegs.h>

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>

#include <Library/IioDfxInit.h>
#include <LibraryPrivate/IioDfxInitPrivate.h>

/**
  This function does DFX specific work

  @param Phase         - Defines the phase of PCI initialization that the PCI driver is executing.
                             Either:
                               EfiPciEnumerationDeviceScanning
                               EfiPciEnumerationBusNumberAssigned
                               EfiPciEnumerationResourceAssigned
  @param IioIndex      - Index to Iio
**/
VOID
IioDfxInit (
  IIO_INIT_PHASE                                      Phase,
  UINT8                                               IioIndex
  )
{
  UINT8         Stack;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (!IioIsStackPresent (IioIndex, Stack)) {
      continue;
    }
     //
     // SA BSU 0.8.11, 7.5.7
     // #3612865: Legacy GPE (HPGPE, PMEGPE, GPE) messages not routed to IRP when DMI is non-legacy
     //
    if (((IioIndex != 0) && (Stack != 0)) && (Phase == IIOInitPhase3)) {
      IioDfxSetCsipsubsad (IioIndex, Stack);
    }
  } // for each stack
}

/**
  IioIsocConfiguration: Configures ISOC parameters in order to enable it.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO
**/
VOID
IioIsocConfiguration (
  IN  IIO_GLOBALS                         *IioGlobalData,
  IN  UINT8                               IioIndex
  )
{
  UINT8   Stack;
  BOOLEAN EnableIsoc;
  BOOLEAN EnableVc1;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (!IioIsStackPresent (IioIndex, Stack)) {
      continue;
    }

    //
    // Configuration for SOC platforms
    //
    if (((IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL))) && (Stack >= 2)) {
      continue;
    }

    if (IioIndex == 0) {
      //
      // For legacy sockets:
      //
      if (IioGlobalData->IioVar.IioVData.IsocEnable) {
        EnableIsoc = TRUE;
        EnableVc1 = TRUE;
      } else {
        EnableIsoc = FALSE;
        EnableVc1 = FALSE;
      }

      if (IioGlobalData->IioVar.IioVData.meRequestedSize > 0 ||
          IioGlobalData->IioVar.IioVData.ieRequestedSize > 0) {
        //
        // If UMA for IE or ME is enabled IrpMiscDfx2Global.Bits.enable_vc1 needs to be disabled
        //
        EnableVc1 = FALSE;
      }

      //
      // VC1 needs to be enabled only for legacy socket
      //
      IioDfxProgramIrpMiscDfx2 (IioIndex, Stack, EnableVc1);
    } else {
      //
      // For non-legacy sockets
      //
      if ((IioGlobalData->IioVar.IioVData.IsocEnable) && (IioGlobalData->IioVar.IioVData.MultiPch)) {
        EnableIsoc = TRUE;
      } else {
        EnableIsoc = FALSE;
      }
    }
    IioDfxProgramIsoc (IioIndex, Stack, EnableIsoc);
    IioDfxProgramEgCredits (IioIndex, Stack);
  } // foreach stack
}



