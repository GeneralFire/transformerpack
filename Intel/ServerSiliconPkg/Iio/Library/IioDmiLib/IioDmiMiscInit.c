/** @file
  This file initialize the DMI device

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>

#include <IndustryStandard/PciExpress21.h>

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>

#ifndef CDF_SC_FLAG
#include <Ppi/PchInit.h>
#include <Library/PchMultiPch.h>
#include <Private/Library/PchRcLib.h>
#endif // CDF_SC_FLAG

#include <IioPlatformData.h>
#include <IioSetupDefinitions.h>

#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioDmiLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioDmiConf.h>


/**
  Disables DMI port on non-legacy sockets

  @param[in] IioIndex           Socket Index
**/
VOID
IioDmiDisableDmiOnNonLegacySkt (
  IN  UINT8             IioIndex
  )
{
  if (IioIndex != 0 && IioDmiIsPortIdentity (IioIndex, PORT_0_INDEX, IioDmiPortActive)) {
    //
    // This is DMI on non-legacy socket
    //
    if (IsMultiPchRunningOnThisSocket (IioIndex)) {
      //
      // MultiPch is running on this setup and DMI shouldn't be disabled
      //
      return;
    }

    DisableDmiOnSocket (IioIndex);
  }
}

/**
  DMi/RLink post initialization settings.

  @param[in]  IioIndex  Socket/IIO complex index
**/
VOID
IioDmiPostInit (
  IN UINT8  IioIndex
  )
{
  //
  // This is implemented only for RLINK
  //
  if (HasRlink ()) {
    LockLogicalPhyNorthBridge ();
  }
}

/**
  DMI on ICX-SP needs spec. credits settings (LBG side only)
**/
VOID
ProgramCustomPchCredits (
  VOID
  )
{
  if (!IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    return;
  }

#ifdef IA32
#ifndef CDF_SC_FLAG
  //
  // This code is a WA for ICX-SP + LBG setup
  //
  EFI_STATUS             Status;
  PCH_INIT_PPI           *PchInitPpi;

  Status = PeiServicesLocatePpi (&gPchInitPpiGuid, 0, NULL, &PchInitPpi);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }
  PchInitPpi->PchDmiSetVcCredits (PchGetPchIdBySocketId (SOCKET_0_INDEX));
#endif // CDF_SC_FLAG
#endif // IA32
}
