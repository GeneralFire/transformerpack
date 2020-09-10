/** @file
  The PCH Initialization Dispatcher After Memory.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#include <Library/PostCodeLib.h>
#include <Library/SerialIoI2cMasterLib.h>
#include <Library/PchDmiLib.h>
#include <Library/PeiGbeInitLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Library/PeiRtcLib.h>
#include <Library/PeiRstPrivateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiSataLib.h>
#include <Library/PeiEspiInitLib.h>
#include <Library/PeiSerialIoInitLib.h>
#include <Library/PeiSmbusLib.h>
#include <Library/PeiIehInitLib.h>
#include <Library/PeiEnterprisePcieRpInitLib.h>
#include <Library/PeiPsfLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiHsioLib.h>
#include <Register/PchRegsHsio.h> // TODO: remove
#include <Ppi/SiPolicy.h>
#include <Library/PeiSmbusDmaLib.h>
#include <Library/PchAdrLib.h>
#include <Library/PeiSpiExtendedDecodeLib.h>
#include <Library/PeiHsioLib.h>
#include <Library/ChipsetInitLib.h>
#include <Library/PeiMcSmbLib.h>

#if FixedPcdGetBool(PcdFspBinaryEnable) == 0
#include <Library/PchSmmControlLib.h>
#endif

/**
  The function issues reset based on SI_SCHEDULE_RESET_HOB
**/
STATIC
VOID
PchPeiReset (
  VOID
  )
{
  EFI_STATUS                Status;
  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi;
  BOOLEAN                   ResetStatus;

  if (!SiScheduleResetIsRequired ()) {
    return;
  }

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );

  ResetStatus = SiScheduleResetPerformReset ();
  ASSERT (!ResetStatus);
}

/**
  Pch init after memory PEI module

  @param[in] SiPolicy     The Silicon Policy PPI instance

  @retval None
**/
VOID
PchInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  PostCode (0xB00); // PCH API Entry
  DEBUG ((DEBUG_INFO, "PchInit - Start\n"));

#if FixedPcdGetBool(PcdFspBinaryEnable) == 0
  //
  // Install PEI SMM Control PPI
  //
  PchSmmControlInit ();
#endif

  //
  // Configure PCH HSIO (ModPhy)
  //
  PchHsioConfigure (SiPolicy);

  //
  // SATA Controller initialization
  //
  PchSataInit (SiPolicy);

  //
  // ClkReq Mapping programming
  //
  PchConfigureClkreqMapping ();

  //
  // Configure GbE LAN
  //
  if (PchIsGbeSupported ()) {
    EbgPchGbeInit (SiPolicy);
  }

  //
  // Configure additional 32MB direct read access to BIOS region
  //
  PchSpiExtendedBiosDecodeRangeInit (SiPolicy);

  //
  // PchInfo HOB must be created before PCIe root port initialization, because
  // afterwards it cannot be determined which ports were fused off
  //
  BuildPchInfoHob ();

  //
  // Build and Update PCH configuration HOB
  //
  BuildPchConfigHob (SiPolicy);

  //
  // McSmb has to be configured before Root Port initialization, because
  // it is required to enable Hot Plug for Root Ports
  //
  PchMcSmbInit (SiPolicy);

  //
  // Configure PCH PCIe Root Ports
  //
  PcieRpInit (SiPolicy);

  DisableUnusedPcieClocks ();

  //
  // Handle PCH PSF Disable
  //
  PchPsfDisableP2pDecoding ();

  //
  // Configure PCH USB controllers
  //
  PchUsbConfigure (SiPolicy);

  //
  // Configure DMI
  //
  PchDmi15Init (SiPolicy);

  //
  // Configure P2SB
  //
  PostCode (0xB0B);
  P2sbConfigure (SiPolicy);

  //
  // Configure PSTH
  //
  PchPsthConfigure (SiPolicy);

  //
  // Configure IOAPIC
  //
  PostCode (0xB0C);
  ItssInitIoApic (SiPolicy);

  //
  // Configure interrupts.
  //
  PostCode (0xB0D);
  ItssConfigureInterrupts (SiPolicy);

  //
  // Configure GPIO PM and interrupt settings
  //
  GpioConfigurePm ();
  GpioSetIrq (ItssGetGpioDevIntConfig (SiPolicy));

  //
  // Configure PSF PM settings
  //
  PsfConfigurePowerManagement (SiPolicy);

  //
  // Configure RTC
  //
  PchRtcConfigure (SiPolicy);

  //
  // Configure SMBUS
  //
  PostCode (0xB15);
  SmbusConfigure (SiPolicy);
  //
  // Configure SMBUS DMA
  //
  SmbusDmaConfigure(SiPolicy);
  //
  // Configure ICLK
  //
  PchIClkConfigure ();

  //
  // Check if ME has the right HSIO Settings and sync with ME if required.
  // Has to be done before PMC Init to ensure the ModPHY Power Gating
  //
  ChipsetInitSync ();

  EbgPchPmcInit (SiPolicy);

  //
  // Configure LPC PM
  //
  LpcConfigurePm ();

  //
  // Configure Serial IO
  //
  SerialIoInit (SiPolicy);

  //
  // Configure eSPI after memory
  //
  EspiInit (SiPolicy);

  //
  // Configure IEH
  //
  IehInit (SiPolicy);

  //
  // Configure RST features and PCIe storage remapping
  // This must be done after PCIe root ports and SATA controller initialization and before function swapping
  //
  ConfigureRst (SiPolicy);

  //
  // SATA controller after RST configuration
  //
  PchSataAfterRstInit (SiPolicy);

  //
  // Configure PSF PCIe Grant Counts after PCIe Root Ports are initialized
  // and unused ports are disabled
  //
  PcieRpConfigureGrantCounts (SiPolicy);

  //
  // Configure Serial IO Function 0
  // This has to happen late here after all other PCH devices (non serial) are configured because non serial devices
  // may share device number with func0 serial Io
  //
  SerialIoFunction0Disable (SiPolicy);

  //
  // Issue Reset based on SiScheduleResetHob
  //
  PchPeiReset ();

  PostCode (0xB7F);  // PCH API Exit
  DEBUG ((DEBUG_INFO, "PchInit - End\n"));
}
