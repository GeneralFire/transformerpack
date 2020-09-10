/** @file
  This files contains PCH R-Link (DMI Mode) services for RCs usage

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
#include <Library/PeiPchRlinkLib.h>
#include <Register/PchRlinkRegs.h>
#include <Register/PchPcrRegs.h>
#include <Library/PchPcieRpLib.h>
#include <Library/TimerLib.h>


#define POLL_VC_STATUS_TIMEOUT 2000000

/**
  Get PCH default TC VC Mapping settings. This funciton returns the default PCH setting
  System Agent can update the settings according to polices.

  @param[in, out] PchDmiTcVcMap         Buffer for PCH_DMI_TC_VC_MAP instance.

**/
VOID
PchRlinkTcVcMapInit (
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

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchDmiPreMemConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);

  if (DmiConfig->MvcEnabled == TRUE) {
    //
    // TC to VC  mapping
    //
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].TcId = PchDmiTcTypeTc0;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].Vc = PchDmiVcTypeVc0;

    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc1].TcId = PchDmiTcTypeTc1;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc1].Vc = PchDmiVcTypeVc1;

    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc2].TcId = PchDmiTcTypeTc2;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc2].Vc = PchDmiVcTypeVc1;

    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc3].TcId = PchDmiTcTypeTc3;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc3].Vc = PchDmiVcTypeVc1;

    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc7].TcId = PchDmiTcTypeTc7;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc7].Vc = PchDmiVcTypeVcm;
  } else {
    //
    // TC to VC  mapping
    //
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].TcId = PchDmiTcTypeTc0;
    PchDmiTcVcMap->DmiTc[PchDmiTcTypeTc0].Vc = PchDmiVcTypeVc0;
  }
  //
  // VC Configuration
  //
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc0].Enable = TRUE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc0].VcId = 0;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc1].Enable = TRUE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVc1].VcId = 1;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVcm].Enable = TRUE;
  PchDmiTcVcMap->DmiVc[PchDmiVcTypeVcm].VcId = 7;

}

/**
  Get PCH RLINK SIP15 Virtual Channel Control and Status registers

  @param[in]  Vc                   The virtual channel number for programing
  @param[out] DmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] DmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
PchRlinkVcRegs (
  IN   PCH_DMI_VC_TYPE  Vc,
  OUT  UINT16           *DmiVcCtlAddress,
  OUT  UINT16           *DmiVcStsAddress
  )
{
  switch (Vc) {
    case PchDmiVcTypeVc0:
      *DmiVcCtlAddress = R_PCH_PCH_RLINK_PCR_VC0RCTL;
      *DmiVcStsAddress = R_PCH_PCH_RLINK_PCR_VC0RSTS;
      break;

    case PchDmiVcTypeVc1:
      *DmiVcCtlAddress = R_PCH_PCH_RLINK_PCR_VC1RCTL;
      *DmiVcStsAddress = R_PCH_PCH_RLINK_PCR_VC1RSTS;
      break;

    case PchDmiVcTypeVcm:
      *DmiVcCtlAddress = R_PCH_PCH_RLINK_PCR_VCMRCTL;
      *DmiVcStsAddress = R_PCH_PCH_RLINK_PCR_VCMRSTS;
      break;

    default:
      *DmiVcCtlAddress = 0;
      *DmiVcStsAddress = 0;
      ASSERT (FALSE);
      break;
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
PchSetRlinkTcVcMapping (
  IN   PCH_DMI_VC_TYPE  Vc,
  IN   UINT8            VcId,
  IN   UINT8            VcMap
  )
{
  UINT32         VxCtlAnd;
  UINT32         VxCtlOr;
  UINT16         VcCtlAddress;
  UINT16         VcStsAddress;

  PchRlinkVcRegs (Vc, &VcCtlAddress, &VcStsAddress);

  VxCtlAnd  = (UINT32) (~(B_PCH_PCH_RLINK_PCR_VC1RCTL_VCID | V_PCH_PCH_RLINK_PCR_VC1RCTL_TVM_MASK));
  VxCtlOr   = ((UINT32) VcId << N_PCH_PCH_RLINK_PCR_VC1RCTL_VCID) & B_PCH_PCH_RLINK_PCR_VC1RCTL_VCID;
  VxCtlOr |= (UINT32) VcMap;
  VxCtlOr |= B_PCH_PCH_RLINK_PCR_VC1RCTL_VCEN;

  PchPcrAndThenOr32 (
    PID_RLINK,
    VcCtlAddress,
    VxCtlAnd,
    VxCtlOr
    );
}

/**
  Polling negotiation status of the corresponding virtual channel

  @param[in] Vc                   The virtual channel number for programming
**/
STATIC
VOID
PchPollRlinkVcStatus (
  IN PCH_DMI_VC_TYPE   Vc
  )
{
  UINT16         VcCtlAddress;
  UINT16         VcStsAddress;
  UINT32         Timeout;

  PchRlinkVcRegs (Vc, &VcCtlAddress, &VcStsAddress);

  //
  // Wait for negotiation to complete
  //
  Timeout = 0;
  while ((PchPcrRead32 (PID_RLINK, VcStsAddress) & B_PCH_PCH_RLINK_PCR_VC1RSTS_VCNP) != 0) {
    // Assert if negotiation hangs for more than ~2 seconds.
    if (Timeout > POLL_VC_STATUS_TIMEOUT) {
      ASSERT (FALSE);
    }
    MicroSecondDelay (100);
    Timeout += 100;
  }
}

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion.

  @param[in] PchDmiTcVcMap              Buffer for PCH_DMI_TC_VC_MAP instance.

  @retval EFI_SUCCESS                   The function completed successfully
**/
EFI_STATUS
PchRlinkTcVcProgPoll (
  IN  PCH_DMI_TC_VC_MAP                 *PchDmiTcVcMap
  )
{
  UINT8                   Index;
  UINT8                   VcMap[PchDmiVcTypeMax];

  ZeroMem (VcMap, PchDmiVcTypeMax);

  ///
  /// Set the TC/VC mappings
  ///
  for (Index = 0; Index < PchDmiTcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "TC:%0x VC:%0x!\n", PchDmiTcVcMap->DmiTc[Index].TcId, PchDmiTcVcMap->DmiTc[Index].Vc));
    VcMap[PchDmiTcVcMap->DmiTc[Index].Vc] |= (BIT0 << PchDmiTcVcMap->DmiTc[Index].TcId);
  }

  for (Index = 0; Index < PchDmiVcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "VC:%0x VCID:%0x Enable:%0x!\n",Index, PchDmiTcVcMap->DmiVc[Index].VcId, PchDmiTcVcMap->DmiVc[Index].Enable));
    if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
      PchSetRlinkTcVcMapping (
        Index,
        PchDmiTcVcMap->DmiVc[Index].VcId,
        VcMap[Index]
        );
    }
  }

  ///
  /// After both above and upstream DMI TC/VC mapping are programmed,
  /// poll VC negotiation pending status until is zero:
  ///
  for (Index = 0; Index < PchDmiVcTypeMax; Index++) {
    if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
      PchPollRlinkVcStatus (Index);
    }
  }

  return EFI_SUCCESS;
}

/**
  The function set the Target Link Speed in PCH to Rlink.

  @param[in] TargetLinkSpeed        Target Link Speed
                                    2: GEN2
                                    3: GEN3
**/
VOID
PchRlinkSetTargetLinkSpeed (
  IN  UINT8                 TargetLinkSpeed
  )
{
  if ((TargetLinkSpeed < 2) || (TargetLinkSpeed > 3)) {
    ASSERT (FALSE);
    return;
  }

  DEBUG ((DEBUG_INFO, "PchRlinkSetTargetLinkSpeed(%d) Start\n", TargetLinkSpeed));

  if ((PchPcrRead32 (PID_RLINK, R_PCH_PCH_RLINK_PCR_LCAP) & B_PCH_PCH_RLINK_PCR_LCAP_MLS) >= TargetLinkSpeed) {
    ///
    /// Set Target Link Speed (TLS)
    ///
    PchPcrAndThenOr8 (PID_RLINK, R_PCH_PCH_RLINK_PCR_LCTL2, (UINT8)~B_PCH_PCH_RLINK_PCR_LCTL2_TLS, TargetLinkSpeed);
  }
  DEBUG ((DEBUG_INFO, "PchDmiRlinkSetTargetLinkSpeed(%d) End\n", TargetLinkSpeed));
}

/**
  This function performs basic Rlink initialization.
**/
STATIC
VOID
PchRlinkBasicInit (
  VOID
  )
{
  //
  // Configure Uncorrectable Error Mask
  // Clear Uncorrectable Internal Error Mask
  // Set the Completer Abort Mask, Poisoned TLP and Unsupported Request Error Mask
  //
  PchPcrAndThenOr32 (
    PID_RLINK, R_PCH_PCH_RLINK_PCR_UEM,
    (UINT32)~0,
    (B_PCH_PCH_RLINK_PCR_UEM_URE | B_PCH_PCH_RLINK_PCR_UEM_CM | B_PCH_PCH_RLINK_PCR_UEM_PT)
    );

  //
  // Configue Max Payload size Supported to 256
  //
  PchPcrAndThenOr8 (
    PID_RLINK, R_PCH_PCH_RLINK_PCR_DEVCTL,
    (UINT8) ~0,
    (UINT8)(V_PCH_PCH_RLINK_PCR_DEVCTL_MPS_256 << N_PCH_PCH_RLINK_PCR_DEVCTL_MPS_256)
    );
}

/**
  Configure Rlink clock gating.
  This function must be called before ASPM is enabled
**/
STATIC
VOID
PchConfigureRlinkClockGating (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  EFI_STATUS                    Status;
  PCH_DMI_CONFIG                *DmiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // 1. Enable Link clock dynamic clock gating
  // 2. Enable Secondary clock dynamic clock gating
  // 3. Enable Link clock trunk gating
  //
  PchPcrAndThenOr32 (
    PID_RLINK, R_PCH_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_PCH_RLINK_MEM_LPMC,
    (UINT8)~0,
    (B_PCH_PCH_RLINK_MEM_LPMC_DLCGE | B_PCH_PCH_RLINK_MEM_LPMC_DSCGE | B_PCH_PCH_RLINK_MEM_LPMC_LCTGE)
    );

  //
  // 1. Enable Side Clock Partition/Trunk Clock Gating
  // 2. Enable Sideband Endpoint Oscillator/Side Dynamic Clock Gating
  // 3. Set IOSF Sideband Interface Idle Counter to 00b - Wait for
  //    32 idle clocks before allowing trunk clock gating.
  //
  PchPcrAndThenOr32 (
    PID_RLINK, R_PCH_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_PCH_RLINK_MEM_IOSFSBCS,
    (UINT8)~B_PCH_PCH_RLINK_MEM_IOSFSBCS_SIID,
    (B_PCH_PCH_RLINK_MEM_IOSFSBCS_SCPTCGE | B_PCH_PCH_RLINK_MEM_IOSFSBCS_SEOSCGE)
    );

  if (DmiConfig->ClockGating) {
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_IOSFDEVCLKGCTL, ~0u, B_PCH_RLINK_CFG_IOSFDEVCLKGCTL_ICGE);
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_PT0TXNCLKGCTL , ~0u, B_PCH_RLINK_CFG_PT0TXNCLKGCTL_ICGE );
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_PT0LNKCLKGCTL , ~0u, B_PCH_RLINK_CFG_PT0LNKCLKGCTL_ICGE );
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_CFGPCIECLKGCTL, ~0u, B_PCH_RLINK_CFG_CFGPCIECLKGCTL_ICGE);
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_SBDEVCLKGCTL,   ~0u, B_PCH_RLINK_CFG_SBDEVCLKGCTL_ICGE  );
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_LDPHYCLKGCTL,   ~0u, B_PCH_RLINK_CFG_LDPHYCLKGCTL_ICGE  );
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_PDPHYCLKGCTL,   ~0u, B_PCH_RLINK_CFG_PDPHYCLKGCTL_ICGE  );
    PchPcrAndThenOr32 (PID_RLINK, R_PCH_RLINK_CFG_CMNREG, ~0u,
                                                  (UINT32) (B_PCH_RLINK_CFG_CMNREG_ICGE_CMNIOSFSBCLKGATE |
                                                            B_PCH_RLINK_CFG_CMNREG_ICGE_CMNSECCLKDYNCLKGATEEN |
                                                            B_PCH_RLINK_CFG_CMNREG_ICGE_CMNSECCLKTRUNKCLKGATEEN)
      );
  }
}

/**
  Configure Rlink link power management.
  This function must be called before ASPM is enabled
**/
STATIC
VOID
PchConfigureRinkPowerManagement (
  VOID
  )
{
  //
  // Hardware-Autonomous IDLE_L1 Enable
  //
  PchPcrAndThenOr32 (
    PID_RLINK, R_PCH_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_PCH_RLINK_MEM_LCTL,
    (UINT32) ~0,
    B_PCH_PCH_RLINK_MEM_LCTL_HAIE
    );

  //
  // 1. Data/StreamID Tristate Enable
  // 2. Valid/Strobe Tristate Enable
  //
  PchPcrAndThenOr32 (
    PID_RLINK, R_PCH_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_PCH_RLINK_MEM_LPMC,
    (UINT32) ~0,
    (B_PCH_PCH_RLINK_MEM_LPMC_DSTE | B_PCH_PCH_RLINK_MEM_LPMC_VSTE)
    );

  //
  // Aging Mitigation Enable
  //
  PchPcrAndThenOr32 (
    PID_RLINK, R_PCH_PCH_RLINK_MEM_LOGICAL_PHY_BAR + R_PCH_PCH_RLINK_MEM_LCFG,
    (UINT32) ~0,
    B_PCH_PCH_RLINK_MEM_LCFG_AME
    );

  //
  // Configure L1 Exit Latency to 0x4 (16us - 32us)
  //
  PchPcrAndThenOr32 (
    PID_RLINK, R_PCH_PCH_RLINK_PCR_LCAP,
    (UINT32) ~(B_PCH_PCH_RLINK_PCR_LCAP_EL1),
    (V_PCH_PCH_RLINK_PCR_LCAP_EL1_16US_32US << N_PCH_PCH_RLINK_PCR_LCAP_EL1)
    );
}

/**
  This function configures ASPM on Rlink

  @param[in] SiPolicyPpi The PCH Policy PPI instance
**/
STATIC
VOID
PchConfigureRlinkAspm (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  UINT16                        Data16And;
  UINT16                        Data16Or;
  PCH_PCIE_ASPM_CONTROL         DmiAspmCtrl;
  EFI_STATUS                    Status;
  PCH_DMI_CONFIG                *DmiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Enable DMI ASPM
  //
  if (DmiConfig->DmiAspmCtrl == TRUE) {
    //
    // DMI ASPM will be set to Intel recommended value - L1 will be enabled on PCH DMI side
    //
    DmiAspmCtrl = PchPcieAspmL1;
  } else {
    DmiAspmCtrl = PchPcieAspmDisabled;
  }

  Data16And = (UINT16)~B_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP;

  if (DmiAspmCtrl == PchPcieAspmL0sL1) {
    //
    // Enable L0s/L1 on Rlink
    //
    Data16Or = V_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP_L0SL1 << N_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP;
  } else if (DmiAspmCtrl == PchPcieAspmL0s) {
    //
    // Enable L0s Entry only
    //
    Data16Or = V_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP_L0S << N_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP;
  } else if (DmiAspmCtrl == PchPcieAspmL1) {
    //
    // Enable L1 Entry only
    //
    Data16Or = V_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP_L1 << N_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP;
  } else {
    //
    // ASPM Disabled
    //
    Data16Or  = V_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP_DIS << N_PCH_PCH_RLINK_PCR_LCAP_ASPMSUP;
  }

  //
  // Configue Rlink ASPM
  //
  PchPcrAndThenOr16 (
    PID_RLINK, R_PCH_PCH_RLINK_PCR_LCAP,
    Data16And,
    Data16Or
    );

  if (DmiAspmCtrl == PchPcieAspmL1) {

    PchPcrAndThenOr8 (
      PID_RLINK, R_PCH_PCH_RLINK_PCR_LCTL,
      (UINT8) ~0,
      V_PCH_PCH_RLINK_PCR_LCTL_ASPMCTL
      );

    PchPcrAndThenOr8 (
      PID_RLINK, R_PCH_PCH_RLINK_PCR_PMIDLTMR,
      (UINT8) ~B_PCH_PCH_RLINK_PCR_PMIDLTMR_L1_IDLE_TIMER,
      V_PCH_PCH_RLINK_PCR_PMIDLTMR_L1_IDLE_TIMER
      );

    PchPcrAndThenOr8 (
      PID_RLINK, R_PCH_PCH_RLINK_PCR_XPDDEF,
      (UINT8) ~B_PCH_PCH_RLINK_PCR_XPDDEF_REQ_L1_SS,
      V_PCH_PCH_RLINK_PCR_XPDDEF_REQ_L1_SS
      );

    PchPcrAndThenOr8 (
      PID_RLINK, R_PCH_PCH_RLINK_PCR_XPLDEF,
      (UINT8) ~B_PCH_PCH_RLINK_PCR_XPLDEF_DIS_ASPM_L1,
      0
      );
  }

}

/**
  This function configures R-Link (DMI Mode).

  @param[in] SiPolicyPpi The PCH Policy PPI instance
**/
VOID
PchRlinkInit (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{

  PchRlinkBasicInit ();
  PchConfigureRlinkClockGating (SiPolicyPpi);
  PchConfigureRinkPowerManagement ();
  PchConfigureRlinkAspm (SiPolicyPpi);
}