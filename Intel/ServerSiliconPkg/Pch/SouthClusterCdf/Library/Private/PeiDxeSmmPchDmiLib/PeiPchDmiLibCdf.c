/** @file
  This files contains PCH R-Link (DMI Mode) services for RCs usage

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchPcrLib.h>
#include <Private/Library/PeiPchDmiLib.h>
#include <Register/PchRegsDmiRlink.h>
#include <Private/Library/PchSbiAccessLib.h>
#include <Register/PchRegsPcr.h>
#include <Library/PchPcieRpLib.h>

#define PCH_DMI_MPSS PchPcieMaxPayload256  // According to CDF spec 256B supported


/**
  Get PCH default TC VC Mapping settings. This funciton returns the default PCH setting
  System Agent can update the settings according to polices.

  @param[in, out] PchDmiTcVcMap         Buffer for PCH_DMI_TC_VC_MAP instance.

**/
VOID
PchDmiTcVcMapInit (
  IN OUT  PCH_DMI_TC_VC_MAP             *PchDmiTcVcMap
  )
{
  EFI_STATUS               Status;
  SI_POLICY_PPI            *SiPolicy;
  PCH_DMI_PREMEM_CONFIG    *DmiConfig;

  if (PchDmiTcVcMap == NULL) {
    return;
  }

    Status = PeiServicesLocatePpi (
               &gSiPreMemPolicyPpiGuid,
               0,
               NULL,
               (VOID **)&SiPolicy
               );
    ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gDmiPreMemConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);

  if (DmiConfig->MvcEnabled == TRUE) {
    //
    // TC to VC  mapping
    //
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc0].TcId = DmiTcTypeTc0;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc0].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc1].TcId = DmiTcTypeTc1;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc1].Vc = DmiVcTypeVc1;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc2].TcId = DmiTcTypeTc2;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc2].Vc = DmiVcTypeVc1;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc3].TcId = DmiTcTypeTc3;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc3].Vc = DmiVcTypeVc1;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc7].TcId = DmiTcTypeTc7;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc7].Vc = DmiVcTypeVcm;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc4].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc4].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc5].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc5].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc6].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc6].Vc = DmiVcTypeVc0;
  } else {
    //
    // TC to VC  mapping
    //
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc0].TcId = DmiTcTypeTc0;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc0].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc1].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc1].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc2].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc2].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc3].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc3].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc4].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc4].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc5].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc5].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc6].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc6].Vc = DmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[DmiTcTypeTc7].TcId = DmiTcTypeNotAssigned;
    PchDmiTcVcMap->DmiTc[DmiTcTypeTc7].Vc = DmiVcTypeVc0;
  }
  //
  // VC Configuration
  //
  PchDmiTcVcMap->DmiVc[DmiVcTypeVc0].Enable = TRUE;
  PchDmiTcVcMap->DmiVc[DmiVcTypeVc0].VcId = 0;
  PchDmiTcVcMap->DmiVc[DmiVcTypeVc1].Enable = TRUE;
  PchDmiTcVcMap->DmiVc[DmiVcTypeVc1].VcId = 1;
  PchDmiTcVcMap->DmiVc[DmiVcTypeVcm].Enable = TRUE;
  PchDmiTcVcMap->DmiVc[DmiVcTypeVcm].VcId = 7;

}

/**
  Get PCH DMI SIP15 Virtual Channel Control and Status registers

  @param[in]  Vc                   The virtual channel number for programing
  @param[out] DmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] DmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
PchDmiRlinkVcRegs (
  IN   PCH_DMI_VC_TYPE  Vc,
  OUT  UINT16           *DmiVcCtlAddress,
  OUT  UINT16           *DmiVcStsAddress
  )
{
  switch (Vc) {
    case DmiVcTypeVc0:
      *DmiVcCtlAddress = R_PCH_RLINK_PCR_V0CTL;
      *DmiVcStsAddress = R_PCH_RLINK_PCR_V0STS;
      break;

    case DmiVcTypeVc1:
      *DmiVcCtlAddress = R_PCH_RLINK_PCR_V1CTL;
      *DmiVcStsAddress = R_PCH_RLINK_PCR_V1STS;
      break;

    case DmiVcTypeVcm:
      *DmiVcCtlAddress = R_PCH_RLINK_PCR_VMCTL;
      *DmiVcStsAddress = R_PCH_RLINK_PCR_VMSTS;
      break;

    default:
      *DmiVcCtlAddress = 0;
      *DmiVcStsAddress = 0;
      ASSERT (FALSE);
  }
}

/**
  Programing transaction classes of the corresponding virtual channel and Enable it

  @param[in] Vc                   The virtual channel number for programming
  @param[in] VcId                 The Identifier to be used for this virtual channel
  @param[in] VcMap                The transaction classes are mapped to this virtual channel.
                                  When a bit is set, this transaction class is mapped to the virtual channel

  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
VOID
PchSetDmiTcVcMapping (
  IN   PCH_DMI_VC_TYPE  Vc,
  IN   UINT8            VcId,
  IN   UINT8            VcMap
  )
{
  UINT32         VxCtlAnd;
  UINT32         VxCtlOr;
  UINT16         VcCtlAddress;
  UINT16         VcStsAddress;

  PchDmiRlinkVcRegs (Vc, &VcCtlAddress, &VcStsAddress);

  VxCtlAnd  = (UINT32) (~(B_PCH_RLINK_PCR_V1CTL_ID | V_PCH_RLINK_PCR_V1CTL_TVM_MASK));
  VxCtlOr   = ((UINT32) VcId << N_PCH_RLINK_PCR_V1CTL_ID) & B_PCH_RLINK_PCR_V1CTL_ID;
  VxCtlOr |= (UINT32) VcMap;
  VxCtlOr |= B_PCH_RLINK_PCR_V1CTL_EN;

  PchSbiPrivateAndThenOr32 (
    PID_DMI,
    VcCtlAddress,
    VxCtlAnd,
    VxCtlOr
    );
  //
  // Reads back for posted write to take effect
  // Read back is done in PchPcr lib
  //
}

/**
  Polling negotiation status of the corresponding virtual channel

  @param[in] Vc                   The virtual channel number for programming

  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_SUCCESS             The function completed successfully
**/
STATIC
VOID
PchPollDmiVcStatus (
  IN PCH_DMI_VC_TYPE   Vc
  )
{
  UINT16         VcCtlAddress;
  UINT16         VcStsAddress;

  PchDmiRlinkVcRegs (Vc, &VcCtlAddress, &VcStsAddress);

  //
  // Wait for negotiation to complete
  //
  while ((PchSbiPrivateRead32 (PID_DMI, VcStsAddress) & B_PCH_RLINK_PCR_V1STS_NP) != 0) {
  }
}

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion.

  @param[in] PchDmiTcVcMap              Buffer for PCH_DMI_TC_VC_MAP instance.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval Others                        All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
PchDmiTcVcProgPoll (
  IN  PCH_DMI_TC_VC_MAP                 *PchDmiTcVcMap
  )
{
  UINT8                   Index;
  UINT8                   VcMap[DmiVcTypeMax];

  ZeroMem (VcMap, DmiVcTypeMax);

  ///
  /// Set the TC/VC mappings
  ///
  for (Index = 0; Index < DmiTcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "TC:%0x VC:%0x!\n", PchDmiTcVcMap->DmiTc[Index].TcId, PchDmiTcVcMap->DmiTc[Index].Vc));
    if (PchDmiTcVcMap->DmiTc[Index].TcId != DmiTcTypeNotAssigned) {
      VcMap[PchDmiTcVcMap->DmiTc[Index].Vc] |= (BIT0 << PchDmiTcVcMap->DmiTc[Index].TcId);
    }
  }

  for (Index = 0; Index < DmiVcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "VC:%0x VCID:%0x Enable:%0x!\n",Index, PchDmiTcVcMap->DmiVc[Index].VcId, PchDmiTcVcMap->DmiVc[Index].Enable));
    if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
      PchSetDmiTcVcMapping (
        Index,
        PchDmiTcVcMap->DmiVc[Index].VcId,
        VcMap[Index]
        );
    }
  }

  ///
  /// After both above and System Agent DMI TC/VC mapping are programmed,
  /// poll VC negotiation pending status until is zero:
  ///
  for (Index = 0; Index < DmiVcTypeMax; Index++) {
    if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
      PchPollDmiVcStatus (Index);
    }
  }

  return EFI_SUCCESS;
}


/**
  This function provides Max Payload Size Supported by PCH in DMI.

  @return A value encoded in PCIe Capabilities is returned, i.e. 0 - 128B, 1 - 256B, 2 - 512B, etc.
**/
UINT8
PchDmiGetMaxPayloadSizeSupported (
  VOID
  )
{
  return PCH_DMI_MPSS;
}


/**
  This function reads Supported Link Speeds Vector from Link Capabilities 2 register.

  @return A bitmap of supported link speeds (gens) is returned.
**/
UINT8
PchDmiGetSupportedLinkSpeedsVector (
  VOID
  )
{
  PCI_REG_PCIE_LINK_CAPABILITY2 LinkCap2;

  LinkCap2.Uint32 = PchPcrRead32 (PID_DMI, R_PCH_RLINK_PCR_LCAP2);
  return (UINT8)LinkCap2.Bits.LinkSpeedsVector;
}


/**
  Backward DMI library API compatibility
  Not applicable to CDF

  This function will perform necessary programming before changing PCH DMI link speed to Gen2 or Gen3.

  @param[in] DmiSpeedGen              PCH DMI speed Gen (1-Gen1, 2-Gen2, 3-Gen3) to which link is going to be trained
**/
STATIC
VOID
PchDmiBeforeLinkSpeedChange (
  UINT8    DmiSpeedGen
  )
{

}

/**
  The function set the Target Link Speed in PCH to DMI GEN 3.

  @param[in] TargetLinkSpeed        Target Link Speed
                                    2: GEN2
                                    3: GEN3
**/
VOID
PchDmiSetTargetLinkSpeed (
  IN  UINT8                 TargetLinkSpeed
  )
{
  if ((TargetLinkSpeed < 2) || (TargetLinkSpeed > 3)) {
    ASSERT (FALSE);
    return;
  }

  DEBUG ((DEBUG_INFO, "PchDmiRlinkSetTargetLinkSpeed(%d) Start\n", TargetLinkSpeed));

  PchDmiBeforeLinkSpeedChange (TargetLinkSpeed);

  ///
  /// PCH BIOS Spec, Section 7
  /// Configure DMI Link Speed (for PCH With DMI ONLY)
  /// Please refer to the System Agent BIOS Writer's Guide on Supported Link Speed
  /// field in Link Capabilities register in CPU complex. (Done in SA code)
  /// If target link speed is GEN2, it can be configured as early as possible.
  /// Else if the target link speed is GEN3, refer to PCH BIOS spec for further programming requirement
  ///
  /// Link speed can be set to GEN2/3 if both LCAP in CPU complex (Done in SA code)
  /// and on PCH side indicates such capability.
  /// DMI Target Link Speed (TLS) is configurable on PCH side in Link Control 2 register (LCTL2)
  ///
  if ((PchPcrRead32 (PID_DMI, R_PCH_RLINK_PCR_LCAP) & B_PCH_RLINK_PCR_LCAP_MLS) >= TargetLinkSpeed) {
    ///
    /// Set Target Link Speed (TLS)
    ///
    PchPcrAndThenOr8 (PID_DMI, R_PCH_RLINK_PCR_LCTL2, (UINT8)~B_PCH_RLINK_PCR_LCTL2_TLS, TargetLinkSpeed);
    ///
    /// Please refer to the System Agent BIOS Writer's Guide to perform DMI Link Retrain after
    /// configures new DMI Link Speed. (Done in SA code)
    ///
  }
  DEBUG ((DEBUG_INFO, "PchDmiRlinkSetTargetLinkSpeed(%d) End\n", TargetLinkSpeed));
}

/**
  This function performs basic DMI initialization.
**/
STATIC
VOID
PchDmiBasicInit (
  VOID
  )
{
  //
  // Configure Uncorrectable Error Mask
  // Clear Uncorrectable Internal Error Mask
  // Set the Completer Abort Mask, Poisoned TLP and Unsupported Request Error Mask
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_RLINK_PCR_UEM,
    (UINT32)~0,
    (B_PCH_RLINK_PCR_UEM_URE | B_PCH_RLINK_PCR_UEM_CM | B_PCH_RLINK_PCR_UEM_PT)
    );

  //
  // Configue Max Payload size Supported to 256
  //
  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_RLINK_PCR_DEVCTL,
    (UINT8) ~0,
    (UINT8)(V_PCH_RLINK_PCR_DEVCTL_MPS_256 << N_PCH_RLINK_PCR_DEVCTL_MPS_256)
    );

  //
  // Configure Parity Check
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_RLINK_CFG_XPTDEF2,
    (UINT32)~(B_RLINK_CFG_XPTDEF2_TCPAR_CHK_DIS | B_RLINK_CFG_XPTDEF2_TDPAR_CHK_DIS),
    0
    );
}

/**
  This function programs Max Payload Size
**/
VOID
PchConfigureDmiMps (
  VOID
  )
{
  UINT16 MaxPayloadSize;
  MaxPayloadSize = PchPcieGetMaxPayloadSizeForAllPorts ();

  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_RLINK_PCR_DEVCAP,
    (UINT8) ~B_PCH_RLINK_PCR_DEVCAP_MPSS,
    MaxPayloadSize & B_PCH_RLINK_PCR_DEVCAP_MPSS
  );
}

/**
  Configure DMI clock gating.
  This function must be called before ASPM is enabled
**/
STATIC
VOID
PchConfigureDmiClockGating (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  EFI_STATUS                    Status;
  PCH_DMI_CONFIG                *DmiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // 1. Enable Link clock dynamic clock gating
  // 2. Enable Secondary clock dynamic clock gating
  // 3. Enable Link clock trunk gating
  //
  PchSbiMemoryAndThenOr32 (
    PID_DMI, R_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_RLINK_MEM_LPMC,
    (UINT8)~0,
    (B_PCH_RLINK_MEM_LPMC_DLCGE | B_PCH_RLINK_MEM_LPMC_DSCGE | B_PCH_RLINK_MEM_LPMC_LCTGE)
    );

  //
  // 1. Enable Side Clock Partition/Trunk Clock Gating
  // 2. Enable Sideband Endpoint Oscillator/Side Dynamic Clock Gating
  // 3. Set IOSF Sideband Interface Idle Counter to 00b - Wait for
  //    32 idle clocks before allowing trunk clock gating.
  //
  PchSbiMemoryAndThenOr32 (
    PID_DMI, R_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_RLINK_MEM_IOSFSBCS,
    (UINT8)~B_PCH_RLINK_MEM_IOSFSBCS_SIID,
    (B_PCH_RLINK_MEM_IOSFSBCS_SCPTCGE | B_PCH_RLINK_MEM_IOSFSBCS_SEOSCGE)
    );

  if (DmiConfig->ClockGating) {
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_IOSFDEVCLKGCTL, ~0u, B_RLINK_CFG_IOSFDEVCLKGCTL_ICGE);
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_PT0TXNCLKGCTL , ~0u, B_RLINK_CFG_PT0TXNCLKGCTL_ICGE );
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_PT0LNKCLKGCTL , ~0u, B_RLINK_CFG_PT0LNKCLKGCTL_ICGE );
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_CFGPCIECLKGCTL, ~0u, B_RLINK_CFG_CFGPCIECLKGCTL_ICGE);
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_SBDEVCLKGCTL,   ~0u, B_RLINK_CFG_SBDEVCLKGCTL_ICGE  );
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_LDPHYCLKGCTL,   ~0u, B_RLINK_CFG_LDPHYCLKGCTL_ICGE  );
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_PDPHYCLKGCTL,   ~0u, B_RLINK_CFG_PDPHYCLKGCTL_ICGE  );
    PchSbiPrivateAndThenOr32 (PID_DMI, R_RLINK_CFG_CMNREG, ~0u,
                    (UINT32) (B_RLINK_CFG_CMNREG_ICGE_CMNIOSFSBCLKGATE |
                              B_RLINK_CFG_CMNREG_ICGE_CMNSECCLKDYNCLKGATEEN |
                              B_RLINK_CFG_CMNREG_ICGE_CMNSECCLKTRUNKCLKGATEEN)
      );
  }
}

/**
  Configure DMI link power management.
  This function must be called before ASPM is enabled
**/
STATIC
VOID
PchConfigureDmiLinkPowerManagement (
  VOID
  )
{
  //
  // 1. Data/StreamID Tristate Enable
  // 2. Valid/Strobe Tristate Enable
  //
  PchSbiMemoryAndThenOr32 (
    PID_DMI, R_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_RLINK_MEM_LPMC,
    (UINT32) ~0,
    (B_PCH_RLINK_MEM_LPMC_DSTE | B_PCH_RLINK_MEM_LPMC_VSTE)
    );

  //
  // Aging Mitigation Enable
  //
  PchSbiMemoryAndThenOr32 (
    PID_DMI, R_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_RLINK_MEM_LCFG,
    (UINT32) ~0,
    B_PCH_RLINK_MEM_LCFG_AME
    );

  //
  // Hardware-Autonomous IDLE_L1 Enable
  //
  PchSbiMemoryAndThenOr32 (
    PID_DMI, R_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_RLINK_MEM_LCTL,
    (UINT32) ~0,
    B_PCH_RLINK_MEM_LCTL_HAIE
    );

  //
  // Configure L1 Exit Latency to 0x4 (16us - 32us)
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_RLINK_PCR_LCAP,
    (UINT32) ~(B_PCH_RLINK_PCR_LCAP_EL1),
    (V_PCH_RLINK_PCR_LCAP_EL1_16US_32US << N_PCH_RLINK_PCR_LCAP_EL1)
    );
}

/**
  Backward DMI library API compatibility
  Not applicable to CDF

  Configure DMI Squelch Power Management.
**/
STATIC
VOID
PchConfigureDmiSquelchPowerManagement (
  VOID
  )
{

}

/**
  Backward DMI library API compatibility
  Not applicable to CDF

  Configure DMI PLL Shutdown.
**/
STATIC
VOID
PchConfigureDmiPllShutdown (
  VOID
  )
{

}

/**
  Backward DMI library API compatibility
  Not applicable to CDF

  Configure DMI Power Gating.
  This needs to done before enabling ASPM L1
**/
STATIC
VOID
PchConfigureDmiPowerGating (
  VOID
  )
{

}

/**
  This function configures ASPM on DMI

  @param[in] SiPolicyPpi The PCH Policy PPI instance
**/
STATIC
VOID
PchConfigureDmiAspm (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  UINT16                        Data16And;
  UINT16                        Data16Or;
  PCH_PCIE_ASPM_CONTROL         DmiAspmCtrl;
  EFI_STATUS                    Status;
  PCH_DMI_CONFIG                *DmiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Enable DMI ASPM
  //
  if (DmiConfig->DmiAspm == TRUE) {
    //
    // DMI ASPM will be set to Intel recommended value - L1 will be enabled on PCH DMI side
    //
    DmiAspmCtrl = PchPcieAspmL1;
  } else {
    DmiAspmCtrl = PchPcieAspmDisabled;
  }

  Data16And = (UINT16)~B_PCH_RLINK_PCR_LCAP_ASPMSUP;

  if (DmiAspmCtrl == PchPcieAspmL0sL1) {
    //
    // Enable L0s/L1 on DMI
    //
    Data16Or = V_PCH_RLINK_PCR_LCAP_ASPMSUP_L0SL1 << N_PCH_RLINK_PCR_LCAP_ASPMSUP;
  } else if (DmiAspmCtrl == PchPcieAspmL0s) {
    //
    // Enable L0s Entry only
    //
    Data16Or = V_PCH_RLINK_PCR_LCAP_ASPMSUP_L0S << N_PCH_RLINK_PCR_LCAP_ASPMSUP;
  } else if (DmiAspmCtrl == PchPcieAspmL1) {
    //
    // Enable L1 Entry only
    //
    Data16Or = V_PCH_RLINK_PCR_LCAP_ASPMSUP_L1 << N_PCH_RLINK_PCR_LCAP_ASPMSUP;
  } else {
    //
    // ASPM Disabled
    //
    Data16Or  = V_PCH_RLINK_PCR_LCAP_ASPMSUP_DIS << N_PCH_RLINK_PCR_LCAP_ASPMSUP;
  }

  //
  // Configue DMI ASPM
  //
  PchPcrAndThenOr16 (
    PID_DMI, R_PCH_RLINK_PCR_LCAP,
    Data16And,
    Data16Or
    );

  if (DmiAspmCtrl == PchPcieAspmL1) {

    PchPcrAndThenOr8 (
      PID_DMI, R_PCH_RLINK_PCR_LCTL,
      (UINT8) ~0,
      V_PCH_RLINK_PCR_LCTL_ASPMCTL
      );

    PchPcrAndThenOr8 (
      PID_DMI, R_PCH_RLINK_PCR_PMIDLTMR,
      (UINT8) ~B_PCH_RLINK_PCR_PMIDLTMR_L1_IDLE_TIMER,
      V_PCH_RLINK_PCR_PMIDLTMR_L1_IDLE_TIMER
      );

    PchPcrAndThenOr8 (
      PID_DMI, R_PCH_RLINK_PCR_XPDDEF,
      (UINT8) ~B_PCH_RLINK_PCR_XPDDEF_REQ_L1_SS,
      V_PCH_RLINK_PCR_XPDDEF_REQ_L1_SS
      );

    PchPcrAndThenOr8 (
      PID_DMI, R_PCH_RLINK_PCR_XPLDEF,
      (UINT8) ~B_PCH_RLINK_PCR_XPLDEF_DIS_ASPM_L1,
      0
      );
  }

}

/**
  This function performs early PCH R-Link (DMI Mode) init.
  It must be called before any upstream bus master transactions are enabled
**/
VOID
PchDmiRlinkEarlyInit (
  VOID
  )
{

}

/**
  This function configures R-Link (DMI Mode).

  @param[in] SiPolicyPpi The PCH Policy PPI instance
**/
VOID
PchDmiRlinkInit (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{

  PchDmiBasicInit ();
  PchConfigureDmiMps ();
  PchConfigureDmiClockGating (SiPolicyPpi);
  PchConfigureDmiLinkPowerManagement ();
  // DMI Thermal Throttling is configure in Thermal module (ThermalInit())
  PchConfigureDmiSquelchPowerManagement ();
  PchConfigureDmiPllShutdown ();
  PchConfigureDmiPowerGating ();
  PchConfigureDmiAspm (SiPolicyPpi);
}
