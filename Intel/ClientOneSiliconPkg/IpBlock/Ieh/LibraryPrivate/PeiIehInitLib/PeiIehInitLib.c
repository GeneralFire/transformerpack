/** @file
  Initializes IEH Device in PEI

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation.

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

#include <Library/PeiIehInitLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PchPcrLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/IehRegs.h>
#include <Register/ItssRegs.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <Register/PchPcrRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PchPciBdfLib.h>

/**
  The function performs IEH specific programming.

  @param[in] SiPolicyPpi        The SI Policy PPI instance

**/
VOID
IehInit (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  )
{
  IEH_CONFIG     *IehConfig;
  UINT64         IehPciBase;
  EFI_STATUS     Status;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIehConfigGuid, (VOID *) &IehConfig);
  ASSERT_EFI_ERROR (Status);

  IehPciBase = IehPciCfgBase ();

  if (PciSegmentRead32 (IehPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_INFO, "IEH device not enabled. Skipping Init\n"));
    return;
  }

  ASSERT (PciSegmentRead32 (IehPciBase + PCI_VENDOR_ID_OFFSET) != 0xFFFFFFFF);

  IehProgramSeverity (IehPciBase);

  if (IehConfig->Mode == IEH_MODE_ENABLE) {
    // IEH Mode is set to Enable
    PciSegmentAndThenOr32 (
      IehPciBase + R_IEH_CFG_MISCCTL1,
      (UINT32) ~(B_IEH_CFG_MISCCTL1_IFD),
      B_IEH_CFG_MISCCTL1_IEH);

    IehEnableMode (IehPciBase);
  } else {
    // IEH Mode is set to Bypass
    PciSegmentAndThenOr32 (
      IehPciBase + R_IEH_CFG_MISCCTL1,
      (UINT32) ~(B_IEH_CFG_MISCCTL1_IEH),
      B_IEH_CFG_MISCCTL1_IFD);
  }
}


