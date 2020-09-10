/** @file
  The I/O Expander Initialization Dispatcher After Memory.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#include "PchInitPei.h"
#include <Ppi/SiPolicy.h>
#include <Library/PostCodeLib.h>
#include <Library/PchDmiLib.h>
#include <Library/PeiGbeInitLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Library/PeiRstPrivateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiSataLib.h>
#include <Library/PeiEnterprisePcieRpInitLib.h>
#include <Library/PeiPsfLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiHsioLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PeiHsioLib.h>
#include <Library/ChipsetInitLib.h>
#include <Library/PchIoExpander.h>

/**
  I/O Expander init after memory PEI module

  @param[in] SiPolicy     The Silicon Policy PPI instance

  @retval None
**/
VOID
EFIAPI
IoExpanderInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  UINT8    IoExpanderIndex;
  UINT8    NumIoExpanders;

  PostCode (0xB00); // IoExpanderInit API Entry
  DEBUG ((DEBUG_ERROR, "IoExpanderInit - Start\n"));

  NumIoExpanders = PchGetNumberDetectedIoExpanders ();

  DEBUG ((DEBUG_ERROR, "Detected I/O Expanders: %d\n", NumIoExpanders));

  for (IoExpanderIndex = 0; IoExpanderIndex < NumIoExpanders; IoExpanderIndex++) {
    EbgPchPmcProgramMmio (SiPolicy, IoExpanderIndex);

    //
    // Configure steps from pre mem phase for I/O Expander
    //
    //PchPreMemConfigure (SiPolicy);

    //
    // Configure PCH HSIO (ModPhy)
    //
    //PchHsioConfigure (SiPolicy);

    //
    // Program PSF grant counts for SATA
    // Call this before SATA ports are accessed for enumeration
    //
    //PsfConfigureSataGrantCounts ();

    //
    // SATA Controller initialization
    //
    IoExpanderSataInit (SiPolicy, IoExpanderIndex);

    //
    // ClkReq Mapping programming
    //
    //PchConfigureClkreqMapping ();

    //
    // Configure GbE LAN
    //
    if (PchIsGbeSupported ()) {
      EbgIoExpanderGbeInit (SiPolicy, IoExpanderIndex);
    }

    //
    // Configure PCH PCIe Root Ports
    //
    IoePcieRpInit (SiPolicy, IoExpanderIndex);

    //DisableUnusedPcieClocks ();

    //
    // Handle PCH PSF Disable
    //
    //PchPsfDisableP2pDecoding ();

    //
    // Configure PCH USB controllers
    //
    //PchUsbConfigure (SiPolicy);

    //
    // Configure DMI
    //
    //PchDmi15Init (SiPolicy);

    //
    // Configure P2SB
    //
    PostCode (0xB0B);
    //P2sbConfigure (SiPolicy);

    //
    //
    // Configure interrupts.
    //
    PostCode (0xB0D);
    //ItssConfigureInterrupts (SiPolicy);

    //
    // Configure GPIO PM and interrupt settings
    //
    //GpioConfigurePm ();
    //GpioSetIrq (ItssGetGpioDevIntConfig (SiPolicy));

    //
    // Configure PSF PM settings
    //
    //PsfConfigurePowerManagement (SiPolicy);

    //
    // Check if ME has the right HSIO Settings and sync with ME if required.
    // Has to be done before PMC Init to ensure the ModPHY Power Gating
    //
    //ChipsetInitSync ();

    //
    // Configure PMC settings
    //
    EbgPchIoExpanderPmcInit (SiPolicy, IoExpanderIndex);

    //
    // Configure RST features and PCIe storage remapping
    // This must be done after PCIe root ports and SATA controller initialization and before function swapping
    //
    //ConfigureRst (SiPolicy);

    //
    // Configure PSF PCIe Grant Counts after PCIe Root Ports are initialized
    // and unused ports are disabled
    //
    //PcieRpConfigureGrantCounts ();
  }

  PchInstallIoExpanderInfoHob ();

  PostCode (0xB7F);  // IoExpanderInit API Exit
  DEBUG ((DEBUG_ERROR, "IoExpanderInit - End\n"));
}
