/** @file
  This file contains functions for PCH DMI configuration for SIP15

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
#include <Library/PchInfoLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Library/PchDmiLib.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchDmi15Regs.h>
#include <Register/PchDmi17Regs.h>
#include <ThermalConfig.h>

/**
  This function configures PCH DMI MultiVC support.
  Applicable from SIP17.

  @param[in]  Vc                   The virtual channel number for programing
**/
VOID
ConfigurePchDmi17MultiVC (
  IN   PCH_DMI_VC_TYPE  Vc
  )
{
  UINT32                        Data32Or;
  UINT32                        Data32And;

  Data32And = ~0u;
  Data32Or = 0u;

  switch (Vc) {
    case PchDmiVcTypeVc0:
      Data32And = (UINT32)~B_PCH_DMI17_PCR_PVCCR1_EVCC_MASK;
      Data32Or = (UINT32)(V_PCH_DMI17_PCR_PVCCR1_EVCC_3_VC << B_PCH_DMI17_PCR_PVCCR1_EVCC_OFFSET);
      PchPcrAndThenOr32 (
        PID_DMI, R_PCH_DMI17_PCR_PVCCR1,
        Data32And,
        Data32Or
        );
      Data32And = (UINT32)~(B_PCH_DMI17_PCR_VCCH_NCO_MASK | B_PCH_DMI17_PCR_VCCH_CV_MASK | B_PCH_DMI17_PCR_VCCH_CID_MASK);
      Data32Or = (UINT32)((V_PCH_DMI17_PCR_VCCH_NCO << B_PCH_DMI17_PCR_VCCH_NCO_OFFSET) |
                          (V_PCH_DMI17_PCR_VCCH_CV_3_VC << B_PCH_DMI17_PCR_VCCH_CV_OFFSET) |
                          (V_PCH_DMI17_PCR_VCCH_CID_3_VC << B_PCH_DMI17_PCR_VCCH_CID_OFFSET));
      PchPcrAndThenOr32 (
        PID_DMI, R_PCH_DMI17_PCR_VCCH,
        Data32And,
        Data32Or
        );
      Data32And = (PchPcrRead32 (PID_DMI, R_PCH_DMI17_PCR_V0VCRC) &
               (UINT32)~B_PCH_DMI17_PCR_V0VCRC_MTS_MASK);
      PchPcrWrite32 (
        PID_DMI, R_PCH_DMI17_PCR_V0VCRC,
        Data32And
        );
      break;

    case PchDmiVcTypeVc1:
      Data32And = (PchPcrRead32 (PID_DMI, R_PCH_DMI17_PCR_V1VCRC) &
               (UINT32)~B_PCH_DMI17_PCR_V1VCRC_MTS_MASK);
      PchPcrWrite32 (
        PID_DMI, R_PCH_DMI17_PCR_V1VCRC,
        Data32And
        );
      break;

    case PchDmiVcTypeVcm:
      break;

    case PchDmiVcTypeMax:
      ASSERT (FALSE);
  }
}

/**
  Get PCH DMI SIP17 Virtual Channel Control and Status registers

  @param[in]  Vc                      The virtual channel number for programing
  @param[out] PchDmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] PchDmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
PchDmi17VcRegs (
  IN   PCH_DMI_VC_TYPE  Vc,
  OUT  UINT16           *PchDmiVcCtlAddress,
  OUT  UINT16           *PchDmiVcStsAddress
  )
{
  switch (Vc) {
    case PchDmiVcTypeVc0:
      *PchDmiVcCtlAddress = R_PCH_DMI17_PCR_V0CTL;
      *PchDmiVcStsAddress = R_PCH_DMI17_PCR_V0STS;
      break;

    case PchDmiVcTypeVc1:
      *PchDmiVcCtlAddress = R_PCH_DMI17_PCR_V1CTL;
      *PchDmiVcStsAddress = R_PCH_DMI17_PCR_V1STS;
      break;

    case PchDmiVcTypeVcm:
      *PchDmiVcCtlAddress = R_PCH_DMI17_PCR_VMCTL;
      *PchDmiVcStsAddress = R_PCH_DMI17_PCR_VMSTS;
      break;

    case PchDmiVcTypeMax:
      *PchDmiVcCtlAddress = 0;
      *PchDmiVcStsAddress = 0;
      ASSERT (FALSE);
  }
}

/**
  Get PCH DMI SIP15 Virtual Channel Control and Status registers

  @param[in]  Vc                   The virtual channel number for programing
  @param[out] PchDmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] PchDmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
PchDmi15VcRegs (
  IN   PCH_DMI_VC_TYPE  Vc,
  OUT  UINT16           *PchDmiVcCtlAddress,
  OUT  UINT16           *PchDmiVcStsAddress
  )
{
  switch (Vc) {
    case PchDmiVcTypeVc0:
      *PchDmiVcCtlAddress = R_PCH_DMI15_PCR_V0CTL;
      *PchDmiVcStsAddress = R_PCH_DMI15_PCR_V0STS;
      break;

    case PchDmiVcTypeVc1:
      *PchDmiVcCtlAddress = R_PCH_DMI15_PCR_V1CTL;
      *PchDmiVcStsAddress = R_PCH_DMI15_PCR_V1STS;
      break;

    case PchDmiVcTypeVcm:
      *PchDmiVcCtlAddress = R_PCH_DMI15_PCR_VMCTL;
      *PchDmiVcStsAddress = R_PCH_DMI15_PCR_VMSTS;
      break;

    case PchDmiVcTypeMax:
      *PchDmiVcCtlAddress = 0;
      *PchDmiVcStsAddress = 0;
      ASSERT (FALSE);
  }
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
  // Set the Completer Abort Mask, Poisoned TLP and Unsupported Request Error Mask
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_UEM,
    ~0u,
    (B_PCH_DMI15_PCR_UEM_URE | B_PCH_DMI15_PCR_UEM_CM | B_PCH_DMI15_PCR_UEM_PT)
    );

  //
  // Configure Transmit Datapath Flush Timer
  //  00b - Wait for 4 clocks prior to initiating rate or powerstate change.
  // Configure Transmit Configuration Change Wait Time
  //  00b - 128ns for GEN1 and GEN2 mode. 256ns for GEN3 mode.
  //
  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_DMI15_PCR_PHYCTL2,
    (UINT8) ~(B_PCH_DMI15_PCR_PHYCTL2_TDFT | B_PCH_DMI15_PCR_PHYCTL2_TXCFGCHGWAIT),
    0
    );

  //
  // Configure Squelch Direction settings
  //
  if (IsAdlPch () && IsPchS ()) {
    PchPcrAndThenOr8 (
      PID_DMI, R_PCH_DMI15_PCR_PHYCTL3,
      (UINT8) ~(B_PCH_DMI15_PCR_PHYCTL3_SQDIROVREN | B_PCH_DMI15_PCR_PHYCTL3_SQDIRCTRL),
      (UINT8) 0
      );
  } else {
    PchPcrAndThenOr8 (
      PID_DMI, R_PCH_DMI15_PCR_PHYCTL3,
      (UINT8) ~0,
      (B_PCH_DMI15_PCR_PHYCTL3_SQDIROVREN | B_PCH_DMI15_PCR_PHYCTL3_SQDIRCTRL)
      );
  }

  //
  // Set Recovery Upconfiguration Disable
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_MPC2,
    ~0u,
    B_PCH_DMI15_PCR_MPC2_RUD
    );

  //
  // Set De-skew Buffer Unload Pointer Increment
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_PWRCTL,
    ~0u,
    B_PCH_DMI15_PCR_PWRCTL_DBUPI
    );

  //
  // Set FC to send update immediately after the CREDITS ALLOCATED register has been updated
  //
  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_DMI15_PCR_MPC,
    (UINT8) ~B_PCH_DMI15_PCR_MPC_FCP,
    0
    );
}

/**
  Configure DMI SIP17 aggresive clock gating.
  This function must be called before ASPM is enabled
**/
STATIC
VOID
ConfigurePchDmi17AggresiveClockGating (
  VOID
  )
{
  UINT32   Data32Or;

  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_IPCLKCTR, ~0u, B_PCH_DMI17_PCR_IPCLKCTR_PXDCGE);

  //
  // Configure Hardware Save and Restore
  //
  Data32Or = (UINT32)((V_PCH_DMI17_PCR_HWSNR_EEH << B_PCH_DMI17_PCR_HWSNR_EEH_OFFSET) |
                      (V_PCH_DMI17_PCR_HWSNR_REPW << B_PCH_DMI17_PCR_HWSNR_REPW_OFFSET) |
                      (V_PCH_DMI17_PCR_HWSNR_BEPW << B_PCH_DMI17_PCR_HWSNR_BEPW_OFFSET));
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_HWSNR,
    (UINT32)~(B_PCH_DMI17_PCR_HWSNR_EEH_MASK | B_PCH_DMI17_PCR_HWSNR_REPW_MASK | B_PCH_DMI17_PCR_HWSNR_BEPW_MASK),
    Data32Or
    );

  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PGCTRL,
    (UINT32)~B_PCH_DMI17_PCR_PGCTRL_PMREQBLKRSPT_MASK,
    (UINT32)(V_PCH_DMI17_PCR_PGCTRL_PMREQBLKRSPT << B_PCH_DMI17_PCR_PGCTRL_PMREQBLKRSPT_OFFSET)
    );

  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PGCBCTL1,
    (UINT32)~B_PCH_DMI17_PCR_PGCBCTL1_TACCRSTUP_MASK,
    (UINT32)(V_PCH_DMI17_PCR_PGCBCTL1_TACCRSTUP << B_PCH_DMI17_PCR_PGCBCTL1_TACCRSTUP_OFFSET)
    );

  //
  // Configure Replay Timer Policy
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PCIERTP1,
    (UINT32)~B_PCH_DMI17_PCR_PCIERTP1_G1X4_MASK,
    (UINT32)(V_PCH_DMI17_PCR_PCIERTP1_G1X4 << B_PCH_DMI17_PCR_PCIERTP1_G1X4_OFFSET)
    );

  //
  // Configure Aggresive Clock Gating
  //
  Data32Or = (UINT32)(B_PCH_DMI17_PCR_DCGM1_PXTTSULDCGM | B_PCH_DMI17_PCR_DCGM1_PXTTULDCGM |
                      B_PCH_DMI17_PCR_DCGM1_PXTRSULDCGM | B_PCH_DMI17_PCR_DCGM1_PXTRULDCGM |
                      B_PCH_DMI17_PCR_DCGM1_PXLSULDCGM | B_PCH_DMI17_PCR_DCGM1_PXLIULDCGM |
                      B_PCH_DMI17_PCR_DCGM1_PXLTULDCGM | B_PCH_DMI17_PCR_DCGM1_PXLRULDCGM |
                      B_PCH_DMI17_PCR_DCGM1_PXCULDCGM | B_PCH_DMI17_PCR_DCGM1_PXKGULDCGM);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCGM1, ~0u, Data32Or);

  Data32Or = (UINT32)(B_PCH_DMI17_PCR_DCGM2_PXSDRULDCGM | B_PCH_DMI17_PCR_DCGM2_PXSDTULDCGM |
                      B_PCH_DMI17_PCR_DCGM2_PXSDIULDCGM | B_PCH_DMI17_PCR_DCGM2_PXFRULDCGM |
                      B_PCH_DMI17_PCR_DCGM2_PXFTULDCGM | B_PCH_DMI17_PCR_DCGM2_PXPBULDCGM |
                      B_PCH_DMI17_PCR_DCGM2_PXPSULDCGM | B_PCH_DMI17_PCR_DCGM2_PXPIULDCGM);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCGM2, ~0u, Data32Or);

  Data32Or = (UINT32)(B_PCH_DMI17_PCR_DCGM3_PXTTSUPDCGM | B_PCH_DMI17_PCR_DCGM3_PXTTUPDCGM |
                      B_PCH_DMI17_PCR_DCGM3_PXTOUPDCGM | B_PCH_DMI17_PCR_DCGM3_PXTRSUPDCGM |
                      B_PCH_DMI17_PCR_DCGM3_PXTRUPDCGM | B_PCH_DMI17_PCR_DCGM3_PXLIUPDCGM |
                      B_PCH_DMI17_PCR_DCGM3_PXLTUPDCGM | B_PCH_DMI17_PCR_DCGM3_PXLRUPDCGM |
                      B_PCH_DMI17_PCR_DCGM3_PXSRUSSNRDCGM | B_PCH_DMI17_PCR_DCGM3_PXCUPSNRDCGM |
                      B_PCH_DMI17_PCR_DCGM3_PXCUPSRCDCGM | B_PCH_DMI17_PCR_DCGM3_PXBUPDCGM |
                      B_PCH_DMI17_PCR_DCGM3_PXEUPDCGM);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCGM3, ~0u, Data32Or);

  //
  // Enable Aggresive Clock Gating
  //
  Data32Or = (UINT32)(B_PCH_DMI17_PCR_DCGEN1_PXTTSULDCGEN | B_PCH_DMI17_PCR_DCGEN1_PXTTULDCGEN |
                      B_PCH_DMI17_PCR_DCGEN1_PXTRSULDCGEN | B_PCH_DMI17_PCR_DCGEN1_PXTRULDCGEN |
                      B_PCH_DMI17_PCR_DCGEN1_PXLSULDCGEN | B_PCH_DMI17_PCR_DCGEN1_PXLIULDCGEN |
                      B_PCH_DMI17_PCR_DCGEN1_PXLRULDCGEN | B_PCH_DMI17_PCR_DCGEN1_PXCULDCGEN |
                      B_PCH_DMI17_PCR_DCGEN1_PXKGULDCGEN);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCGEN1, ~0u, Data32Or);

  Data32Or = (UINT32)(B_PCH_DMI17_PCR_DCGEN2_PXDSRULDCGEN | B_PCH_DMI17_PCR_DCGEN2_PXSDTULDCGEN |
                      B_PCH_DMI17_PCR_DCGEN2_PXSDIULDCGEN | B_PCH_DMI17_PCR_DCGEN2_PXFRULDCGEN |
                      B_PCH_DMI17_PCR_DCGEN2_PXFTULDCGEN | B_PCH_DMI17_PCR_DCGEN2_PXPBULDCGEN |
                      B_PCH_DMI17_PCR_DCGEN2_PXPSULDCGEN |B_PCH_DMI17_PCR_DCGEN2_PXPIULDCGEN);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCGEN2, ~0u, Data32Or);

  Data32Or = (UINT32)(B_PCH_DMI17_PCR_DCGEN3_PXTTSUPDCGEN | B_PCH_DMI17_PCR_DCGEN3_PXTTUPDCGEN |
                      B_PCH_DMI17_PCR_DCGEN3_PXTOUPDCGEN | B_PCH_DMI17_PCR_DCGEN3_PXTRSUPDCGEN |
                      B_PCH_DMI17_PCR_DCGEN3_PXTRUPDCGEN | B_PCH_DMI17_PCR_DCGEN3_PXLIUPDCGEN |
                      B_PCH_DMI17_PCR_DCGEN3_PXLRUPDCGEN | B_PCH_DMI17_PCR_DCGEN3_PXSRUSSNRDCGEN |
                      B_PCH_DMI17_PCR_DCGEN3_PXBUPDCGEN | B_PCH_DMI17_PCR_DCGEN3_PXEUPDCGEN);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCGEN3, ~0u, Data32Or);
}

/**
  Configure DMI clock gating.
  This function must be called before ASPM is enabled
**/
STATIC
VOID
ConfigurePchDmiClockGating (
  VOID
  )
{
  //
  // Enable Dynamic Clock Gating on ISM Active
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_CCFG,
    ~0u,
    B_PCH_DMI15_PCR_CCFG_DCGEISMA
    );

  //
  // Enable below clock gating settings:
  // - Partition/Trunk Oscillator Clock Gate Enable
  // - Link CLKREQ Enable
  // - Backbone CLKREQ Enable
  // - Shared Resource Dynamic Backbone Clock Gate Enable
  // - Root Port Dynamic Link Clock Gate Enable
  // - Root Port Dynamic Backbone Clock Gate Enable
  //
  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_DMI15_PCR_RPDCGEN,
    (UINT8)~0,
    (B_PCH_DMI15_PCR_PTOCGE | B_PCH_DMI15_PCR_LCLKREQEN | B_PCH_DMI15_PCR_BBCLKREQEN |
    B_PCH_DMI15_PCR_SRDBCGEN | B_PCH_DMI15_PCR_RPDLCGEN | B_PCH_DMI15_PCR_RPDBCGEN)
    );

  //
  // Enable Side Clock Partition/Trunk Clock Gating
  // Set IOSF Sideband Interface Idle Counter to 00b - Wait for
  // 32 idle clocks before allowing trunk clock gating.
  //
  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_DMI15_PCR_IOSFSBCS,
    (UINT8)~B_PCH_DMI15_PCR_IOSFSBCS_SIID,
    B_PCH_DMI15_PCR_IOSFSBCS_SCPTCGE
    );

  //
  // Enable Sideband Endpoint Oscillator/Side Clock Gating
  //
  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_DMI15_PCR_RPPGEN,
    (UINT8)~0,
    B_PCH_DMI15_PCR_RPPGEN_SEOSCGE
    );

  if (IsAdlPch () && IsPchS ()) {
    //
    // Program OSC Clock Gate Hysteresis
    //
    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_PME_CTL3,
      (UINT32)~(B_PCH_DMI17_PCR_PME_CTL3_L1PGAUTOPGEN | B_PCH_DMI17_PCR_PME_CTL3_OSCCGH | B_PCH_DMI17_PCR_PME_CTL3_PMREQCPGEXH_MASK),
      (UINT32)((V_PCH_DMI17_PCR_PME_CTL3_OSCCGH_1US << B_PCH_DMI17_PCR_PME_CTL3_OSCCGH_OFFSET) |
               (V_PCH_DMI17_PCR_PME_CTL3_PMREQCPGEXH << B_PCH_DMI17_PCR_PME_CTL3_PMREQCPGEXH_OFFSET))
      );
    ConfigurePchDmi17AggresiveClockGating ();
  }
}

/**
  Configure DMI link power management.
  This function must be called before ASPM is enabled
**/
STATIC
VOID
ConfigurePchDmiLinkPowerManagement (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  EFI_STATUS            Status;
  BOOLEAN               IsSip17Dmi;
  PCH_DMI_CONFIG       *PchDmiConfig;
  UINT8                 Data8And;
  UINT8                 Data8Or;
  UINT32                Data32And;
  UINT32                Data32Or;

  Data32And = ~0u;
  Data32Or = 0u;
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchDmiConfigGuid, (VOID *) &PchDmiConfig);
  ASSERT_EFI_ERROR (Status);

  IsSip17Dmi = IsAdlPch () && IsPchS ();
  if (IsSip17Dmi) {
    Data8Or  = (V_PCH_DMI15_PCR_CTRL1_L1PL_2US << N_PCH_DMI15_PCR_CTRL1_L1PL) >> 8;
  } else {
    Data8Or  = (V_PCH_DMI15_PCR_CTRL1_L1PL_4US << N_PCH_DMI15_PCR_CTRL1_L1PL) >> 8;
  }
  Data8And = (UINT8)~(B_PCH_DMI15_PCR_CTRL1_L1PL >> 8);
  if (PchDmiConfig->L1RpCtl == FALSE) {
    Data8And &= (UINT8)~(B_PCH_DMI15_PCR_CTRL1_L1RC >> 8);
  }
  //
  // Configure DMI L1 Preparation Latency to 4us - this is the time link layer has to
  // indicate IDLE before the link initialization and control logic enters Active State L1.
  // Be careful not to touch first byte of this register as CTRL1.PSS is RWO.
  //

  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_DMI15_PCR_CTRL1 + 1,
    Data8And,
    Data8Or
    );

  //
  // Configure Gen1 and Gen2 Active State L0s Preparation Latency - time that link layer has to
  // indicate IDLE before the link initialization and control logic enters L0s
  // Set it to 0x14 clocks
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_PCIEL0SC,
    (UINT32)~(B_PCH_DMI15_PCR_PCIEL0SC_G2ASL0SPL | B_PCH_DMI15_PCR_PCIEL0SC_G1ASL0SPL),
    ((0x14 << N_PCH_DMI15_PCR_PCIEL0SC_G2ASL0SPL) |
    (0x14 << N_PCH_DMI15_PCR_PCIEL0SC_G1ASL0SPL))
    );

  //
  // Configure Gen1 and Gen2 Common Clock N_FTS
  //
  if (IsSip17Dmi) {
    Data32And = (UINT32)~(B_PCH_DMI17_PCR_PCIENFTS_G2UCNFTS_MASK | B_PCH_DMI17_PCR_PCIENFTS_G2CCNFTS_MASK |
                          B_PCH_DMI17_PCR_PCIENFTS_G1UCNFTS_MASK | B_PCH_DMI17_PCR_PCIENFTS_G1CCNFTS_MASK);
    Data32Or = (UINT32)((V_PCH_DMI17_PCR_PCIENFTS_G2UCNFTS << B_PCH_DMI17_PCR_PCIENFTS_G2UCNFTS_OFFSET) |
                        (V_PCH_DMI17_PCR_PCIENFTS_G2CCNFTS << B_PCH_DMI17_PCR_PCIENFTS_G2CCNFTS_OFFSET) |
                        (V_PCH_DMI17_PCR_PCIENFTS_G1UCNFTS << B_PCH_DMI17_PCR_PCIENFTS_G1UCNFTS_OFFSET) |
                        (V_PCH_DMI17_PCR_PCIENFTS_G1CCNFTS << B_PCH_DMI17_PCR_PCIENFTS_G1CCNFTS_OFFSET));
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PCIENFTS, Data32And, Data32Or);

    Data32And = (UINT32)~(B_PCH_DMI17_PCR_G3L0SCTL_G3ASL0SPL_MASK | B_PCH_DMI17_PCR_G3L0SCTL_G3UCNFTS_MASK | B_PCH_DMI17_PCR_G3L0SCTL_G3CCNFTS_MASK);
    Data32Or = (UINT32)((V_PCH_DMI17_PCR_G3L0SCTL_G3ASL0SPL << B_PCH_DMI17_PCR_G3L0SCTL_G3ASL0SPL_OFFSET) |
                        (V_PCH_DMI17_PCR_G3L0SCTL_G3UCNFTS << B_PCH_DMI17_PCR_G3L0SCTL_G3UCNFTS_OFFSET) |
                        (V_PCH_DMI17_PCR_G3L0SCTL_G3CCNFTS << B_PCH_DMI17_PCR_G3L0SCTL_G3CCNFTS_OFFSET));
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_G3L0SCTL, Data32And, Data32Or);

    //
    // Configure Gen3 Common Clock N_FTS to 0x70
    //
    Data32And = (UINT32)~(B_PCH_DMI17_PCR_G4L0SCTL_G4ASL0SPL_MASK | B_PCH_DMI17_PCR_G4L0SCTL_G4L0SIC_MASK | B_PCH_DMI17_PCR_G4L0SCTL_G4UCNFTS_MASK | B_PCH_DMI17_PCR_G4L0SCTL_G4CCNFTS_MASK);
    Data32Or = (UINT32) ((V_PCH_DMI17_PCR_G4L0SCTL_G4ASL0SPL << B_PCH_DMI17_PCR_G4L0SCTL_G4ASL0SPL_OFFSET) |
                         (V_PCH_DMI17_PCR_G4L0SCTL_G4L0SIC << B_PCH_DMI17_PCR_G4L0SCTL_G4L0SIC_OFFSET) |
                         (V_PCH_DMI17_PCR_G4L0SCTL_G4UCNFTS << B_PCH_DMI17_PCR_G4L0SCTL_G4UCNFTS_OFFSET) |
                         (V_PCH_DMI17_PCR_G4L0SCTL_G4CCNFTS << B_PCH_DMI17_PCR_G4L0SCTL_G4CCNFTS_OFFSET));
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_G4L0SCTL, Data32And, Data32Or);

    Data32And = (UINT32)~(B_PCH_DMI17_PCR_G5L0SCTL_G5UCNFTS_MASK | B_PCH_DMI17_PCR_G5L0SCTL_G5CCNFTS_MASK);
    Data32Or = (UINT32) ((V_PCH_DMI17_PCR_G5L0SCTL_G5UCNFTS << B_PCH_DMI17_PCR_G5L0SCTL_G5CCNFTS_OFFSET));
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_G5L0SCTL, Data32And, Data32Or);
  } else {
    //
    // Configure Gen3 Active State L0s Preparation Latency - time that link layer has to
    // indicate IDLE before the link initialization and control logic enters L0s
    // Set it to 0x28 clocks
    // Set Gen3 Common Clock N_FTS to 0x38
    //
    Data32And = (UINT32)~(B_PCH_DMI15_PCR_PCIENFTS_G2CCNFTS | B_PCH_DMI15_PCR_PCIENFTS_G1CCNFTS);
    Data32Or = (UINT32)((0x70 << N_PCH_DMI15_PCR_PCIENFTS_G2CCNFTS) | (0x38 << N_PCH_DMI15_PCR_PCIENFTS_G1CCNFTS));
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_PCIENFTS, Data32And, Data32Or);

    Data32And = (UINT32)~(B_PCH_DMI15_PCR_G3L0SCTL_G3ASL0SPL | B_PCH_DMI15_PCR_G3L0SCTL_G3CCNFTS);
    Data32Or = (UINT32)(((0x28 << N_PCH_DMI15_PCR_G3L0SCTL_G3ASL0SPL) | (0x2c << N_PCH_DMI15_PCR_G3L0SCTL_G3CCNFTS)));
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_G3L0SCTL, Data32And, Data32Or);
  }
}

/**
  Configure DMI Squelch Power Management.
**/
STATIC
VOID
ConfigurePchDmiSquelchPowerManagement (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  Data32And = ~0u;
  Data32Or = 0u;
  //
  // Enable Low Bandwidth Squelch Settling Timer
  // Set this before enabling any of the squelch power management
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_PCIECFG2,
    ~0u,
    B_PCH_DMI15_PCR_PCIECFG2_LBWSSTE
    );

  //
  // Enable Squelch propagation control
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_PCIEDBG,
    ~0u,
    B_PCH_DMI15_PCR_PCIEDBG_SPCE
    );

  //
  // Configure Root Port Squelch Exit Wait Latency to 120ns
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_PWRCTL,
    (UINT32) ~B_PCH_DMI15_PCR_PWRCTL_RPSEWL,
    V_PCH_DMI15_PCR_PWRCTL_RPSEWL_120NS << N_PCH_DMI15_PCR_PWRCTL_RPSEWL
    );

  if (IsAdlPch () && IsPchS ()) {
    Data32And = (B_PCH_DMI17_PCR_RXL0SESQCP4_G4LBWSST_MASK | B_PCH_DMI17_PCR_RXL0SESQCP4_G3LBWSST_MASK |
                 B_PCH_DMI17_PCR_RXL0SESQCP4_G2LBWSST_MASK | B_PCH_DMI17_PCR_RXL0SESQCP4_G1LBWSST_MASK);
    Data32Or = ((V_PCH_DMI17_PCR_RXL0SESQCP4_G4LBWSST << B_PCH_DMI17_PCR_RXL0SESQCP4_G4LBWSST_OFFSET) |
                (V_PCH_DMI17_PCR_RXL0SESQCP4_G3LBWSST << B_PCH_DMI17_PCR_RXL0SESQCP4_G3LBWSST_OFFSET) |
                (V_PCH_DMI17_PCR_RXL0SESQCP4_G2LBWSST << B_PCH_DMI17_PCR_RXL0SESQCP4_G2LBWSST_OFFSET) |
                (V_PCH_DMI17_PCR_RXL0SESQCP4_G1LBWSST << B_PCH_DMI17_PCR_RXL0SESQCP4_G1LBWSST_OFFSET));
    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_RXL0SESQCP4,
      Data32And,
      Data32Or
      );
  }
}

/**
  Configure DMI PLL Shutdown.
**/
STATIC
VOID
ConfigurePchDmiPllShutdown (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;
  UINT8    Data8And;
  UINT8    Data8Or;

  //
  // Configure PLL Wait to 5us.This is amount of time to wait from gating the link clock
  // before turning off the PLL.
  // Enable Gen2 PLL coupling (leave disabled for CNL-PCH-H-Ax)
  // Enable Gen3 PLL coupling
  // This needs to be done before enabling PLL shutdown
  //
  Data8And = (UINT8)~0;
  Data8Or = (UINT8)0;
  Data32And = (UINT32)~0;
  Data32Or = (UINT32)0;
  if (IsCnlPch () && (PchStepping () < PCH_B0)) {
    Data32And = (UINT32) ~(B_PCH_DMI15_PCR_MPC2_PLLWAIT | B_PCH_DMI15_PCR_MPC2_GEN2PLLC);
    Data32Or = (UINT32) ((V_PCH_DMI15_PCR_MPC2_PLLWAIT_5US << N_PCH_DMI15_PCR_MPC2_PLLWAIT) | B_PCH_DMI15_PCR_MPC2_GEN3PLLC);
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_MPC2, Data32And, Data32Or);
  } else {
    Data32And = (UINT32) ~B_PCH_DMI15_PCR_MPC2_PLLWAIT;
    Data32Or = (UINT32) ((V_PCH_DMI15_PCR_MPC2_PLLWAIT_5US << N_PCH_DMI15_PCR_MPC2_PLLWAIT) | B_PCH_DMI15_PCR_MPC2_GEN2PLLC | B_PCH_DMI15_PCR_MPC2_GEN3PLLC);
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_MPC2, Data32And, Data32Or);
  }

  //
  // Enable PCI Express GEN2 and GEN3 PLL Off
  // If enabled and no devices require the use of the PXP PLL clock outputs,
  // the PXP PLL can be shutdown.
  // This programming should be done before enabling ASPM
  //
  Data8And = (UINT8)~0;
  Data8Or = (UINT8)(B_PCH_DMI15_PCR_PHYCTL2_PXPG3PLLOFFEN | B_PCH_DMI15_PCR_PHYCTL2_PXPG2PLLOFFEN);
  PchPcrAndThenOr8 (PID_DMI, R_PCH_DMI15_PCR_PHYCTL2, Data8And, Data8Or);
}

/**
  Configure DMI Power Gating.
  This needs to done before enabling ASPM L1

  @param[in] SiPolicyPpi The PCH Policy PPI instance
**/
STATIC
VOID
ConfigurePchDmiPowerGating (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  BOOLEAN           IsSip17Dmi;
  PCH_DMI_CONFIG    *PchDmiConfig;
  UINT32            AndData;
  UINT32            OrData;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchDmiConfigGuid, (VOID *) &PchDmiConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // Disable the squelch circuitry for all lanes
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_PHYCTL4,
    ~0u,
    B_PCH_DMI15_PCR_PHYCTL4_SQDIS
    );

  //
  // Disable IP-Accessible Context Propagation
  // Enable PHY Lane Power Gating when link resides in LTSSM Disabled, Detect or L23_Rdy state
  // Enable disabling squelch when the link resides in LTSSM Disabled, L23_Rdy state or if the lane is unconfigured
  // Enable IP-Inaccessible Entry Policy
  //
  IsSip17Dmi = IsAdlPch () && IsPchS ();
  if (IsSip17Dmi) {
    AndData = (UINT32) ~B_PCH_DMI17_PCR_PWRCTL_DARECE;
    OrData = (UINT32) (B_PCH_DMI17_PCR_PWRCTL_WPDMPGEP | B_PCH_DMI17_PCR_PWRCTL_TXSWING);
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PWRCTL, AndData, OrData);
    AndData = (UINT32)~(B_PCH_DMI15_PCR_PCIEPMECTL_IPACPE | B_PCH_DMI15_PCR_PCIEPMECTL_DLSULDLSD);
    OrData = (UINT32) (B_PCH_DMI15_PCR_PCIEPMECTL_DLSULPPGE | B_PCH_DMI15_PCR_PCIEPMECTL_IPIEP);
  } else {
    AndData = (UINT32)~B_PCH_DMI15_PCR_PCIEPMECTL_IPACPE;
    OrData = (UINT32) (B_PCH_DMI15_PCR_PCIEPMECTL_DLSULPPGE | B_PCH_DMI15_PCR_PCIEPMECTL_DLSULDLSD | B_PCH_DMI15_PCR_PCIEPMECTL_IPIEP);
  }
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_PCIEPMECTL, AndData, OrData);

  if (PchDmiConfig->DmiPowerReduction) {
    //
    // Enable L1 State Controller Power Gating
    // Enable L1 State PHY Data Lane Power Gating
    // Enable PHY Common Lane Power Gating
    //
    if (IsSip17Dmi) {
      AndData = (UINT32)~(B_PCH_DMI17_PCR_PCIEPMECTL2_CPMCSRE | B_PCH_DMI17_PCR_PCIEPMECTL2_CPGEXH_MASK | B_PCH_DMI17_PCR_PCIEPMECTL2_CPGENH_MASK);
      OrData = (UINT32)(B_PCH_DMI15_PCR_PCIEPMECTL2_PHYCLPGE | B_PCH_DMI15_PCR_PCIEPMECTL2_L1SCPGE| B_PCH_DMI15_PCR_PCIEPMECTL2_L1SPHYDLPGE);
      PchPcrAndThenOr32 (
        PID_DMI, R_PCH_DMI17_PCR_PCIEPMECTL,
        (UINT32)~(B_PCH_DMI17_PCR_PCIEPMECTL_DLSULDLSD | B_PCH_DMI17_PCR_PCIEPMECTL_L1FSOE |
                  B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLSV | B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLV),
        (UINT32)(B_PCH_DMI17_PCR_PCIEPMECTL_DLSULPPGE | (V_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLSV << B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLSV_OFFSET) |
                 (V_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLV << B_PCH_DMI17_PCR_PCIEPMECTL_L1LTRTLV_OFFSET))
        );
      PchPcrAndThenOr32 (
        PID_DMI, R_PCH_DMI17_PCR_PGTHRES,
        (UINT32)~(B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLSV | B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLV),
        (UINT32)((V_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLSV << B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLSV_OFFSET) |
                 (V_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLV << B_PCH_DMI17_PCR_PGTHRES_L1PGLTRTLV_OFFSET))
        );
    } else {
      AndData = (UINT32)~0u;
      OrData = (UINT32)(B_PCH_DMI15_PCR_PCIEPMECTL2_PHYCLPGE | B_PCH_DMI15_PCR_PCIEPMECTL2_L1SPHYDLPGE | B_PCH_DMI15_PCR_PCIEPMECTL2_L1SCPGE);
    }
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI15_PCR_PCIEPMECTL2, AndData, OrData);
  }

  if (PchDmiConfig->DmiPowerReduction) {
    //
    // Set Hardware Autonomous Enable,
    // PMC Request Enable and Sleep Enable
    //
    AndData = (UINT32)~B_PCH_DMI15_PCR_PCE_PMCRE;
    OrData = B_PCH_DMI15_PCR_PCE_HAE | B_PCH_DMI15_PCR_PCE_SE;
  } else {
    //
    // Clear Hardware Autonomous Enable and Sleep Enable
    //
    AndData = (UINT32)~(B_PCH_DMI15_PCR_PCE_PMCRE | B_PCH_DMI15_PCR_PCE_SE);
    if (IsAdlPch () && IsPchS ()) {
      OrData = B_PCH_DMI15_PCR_PCE_SE;
    } else {
      OrData = 0;
    }
  }
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_PCE,
    AndData,
    OrData
    );

  //
  // CPG Exit Link Clock Wake Disable
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_CTRL2,
    ~0u,
    B_PCH_DMI15_PCR_CTRL2_CPGEXLCWDIS
    );
}

/**
  This function configures ASPM on DMI

  @param[in] SiPolicyPpi The PCH Policy PPI instance
**/
STATIC
VOID
ConfigurePchDmiAspm (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  UINT16                        Data16And;
  UINT16                        Data16Or;
  UINT32                        Data32And;
  UINT32                        Data32Or;
  PCH_PCIE_ASPM_CONTROL         DmiAspmCtrl;
  EFI_STATUS                    Status;
  PCH_DMI_CONFIG                *PchDmiConfig;
  PCH_GENERAL_CONFIG            *PchGeneralConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchDmiConfigGuid, (VOID *) &PchDmiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);

  Data32And = ~0u;
  Data32Or = 0u;
  //
  // Enable DMI ASPM
  //
  if (PchGeneralConfig->LegacyIoLowLatency) {
    DmiAspmCtrl = PchPcieAspmDisabled;
  } else {
    DmiAspmCtrl = PchDmiConfig->DmiAspmCtrl;
  }

  Data16And = (UINT16)~B_PCH_DMI15_PCR_LCTL_ASPM;

  if (IsAdlPch () && IsPchS()) {
    if (DmiAspmCtrl == PchPcieAspmL0sL1) {
      //
      // Enable L0s/L1 on DMI
      //
      Data16Or = V_PCH_DMI15_PCR_LCTL_ASPM_L0SL1;
      Data32Or = (UINT32) (V_PCH_DMI17_PCR_LCAP_APMS_L0SL1 << B_PCH_DMI17_PCR_LCAP_APMS_OFFSET);
    } else if (DmiAspmCtrl == PchPcieAspmL0s) {
      //
      // Enable L0s Entry only
      //
      Data16Or = V_PCH_DMI15_PCR_LCTL_ASPM_L0S;
      Data32Or = (UINT32) (V_PCH_DMI17_PCR_LCAP_APMS_L0S << B_PCH_DMI17_PCR_LCAP_APMS_OFFSET);
    } else if ((DmiAspmCtrl == PchPcieAspmAutoConfig) || (DmiAspmCtrl == PchPcieAspmL1)) {
      //
      // Enable L1 Entry only
      //
      Data16Or = V_PCH_DMI15_PCR_LCTL_ASPM_L1;
      Data32Or = (UINT32) (V_PCH_DMI17_PCR_LCAP_APMS_L1 << B_PCH_DMI17_PCR_LCAP_APMS_OFFSET);
    } else {
      //
      // ASPM Disabled
      //
      Data16Or = V_PCH_DMI15_PCR_LCTL_ASPM_DIS;
      Data32Or = (UINT32) (V_PCH_DMI17_PCR_LCAP_APMS_DIS << B_PCH_DMI17_PCR_LCAP_APMS_OFFSET);
    }
    Data32And = (UINT32)~(B_PCH_DMI17_PCR_LCAP_EL1_MASK | B_PCH_DMI17_PCR_LCAP_EL0_MASK | B_PCH_DMI17_PCR_LCAP_APMS_MASK);
    Data32Or |= (UINT32) ((V_PCH_DMI17_PCR_LCAP_EL1 << B_PCH_DMI17_PCR_LCAP_EL1_OFFSET) |
                          (V_PCH_DMI17_PCR_LCAP_EL0 << B_PCH_DMI17_PCR_LCAP_EL0_OFFSET) | B_PCH_DMI17_PCR_LCAP_ASPMOC);
  } else {
    if ((DmiAspmCtrl == PchPcieAspmAutoConfig) || (DmiAspmCtrl == PchPcieAspmL0sL1)) {
      //
      // Enable L0s/L1 on DMI
      //
      Data16Or = V_PCH_DMI15_PCR_LCTL_ASPM_L0SL1;
    } else if (DmiAspmCtrl == PchPcieAspmL0s) {
      //
      // Enable L0s Entry only
      //
      Data16Or = V_PCH_DMI15_PCR_LCTL_ASPM_L0S;
    } else if (DmiAspmCtrl == PchPcieAspmL1) {
      //
      // Enable L1 Entry only
      //
      Data16Or = V_PCH_DMI15_PCR_LCTL_ASPM_L1;
    } else {
      //
      // ASPM Disabled
      //
      Data16Or  = V_PCH_DMI15_PCR_LCTL_ASPM_DIS;
    }
    Data32And = (UINT32) ~(B_PCH_DMI_PCR_LCAP_EL1 | B_PCH_DMI_PCR_LCAP_EL0);
    Data32Or |= (UINT32)((V_PCH_DMI_PCR_LCAP_EL1_8US_16US << N_PCH_DMI_PCR_LCAP_EL1) |
                         (V_PCH_DMI_PCR_LCAP_EL0_256NS_512NS << N_PCH_DMI_PCR_LCAP_EL0));
  }

  //
  // Configue DMI ASPM
  //
  PchPcrAndThenOr16 (
    PID_DMI, R_PCH_DMI15_PCR_LCTL,
    Data16And,
    Data16Or
    );

  //
  // Configure L0s Exit Latency to 0x3 (128ns - 256ns)
  // Configure L1s Exit Latency to 0x4 (8us - 16us)
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI_PCR_LCAP,
    Data32And,
    Data32Or
    );
}

/**
  This function performs PCH-H DMI Thermal Throttling init.
**/
VOID
ConfigurePchDmiThermalThrottling (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  EFI_STATUS                    Status;
  THERMAL_CONFIG                *ThermalConfig;
  PCH_DMI_THERMAL_THROTTLING    DmiThermalThrottling;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);

  if (IsPchH ()) {
    if (ThermalConfig->DmiHaAWC.SuggestedSetting) {
      PchDmiSetRecommendedThermalThrottling ();
    } else if (ThermalConfig->DmiHaAWC.DmiTsawEn) {
      DmiThermalThrottling.ThermalSensor0TargetWidth = ThermalConfig->DmiHaAWC.TS0TW;
      DmiThermalThrottling.ThermalSensor1TargetWidth = ThermalConfig->DmiHaAWC.TS1TW;
      DmiThermalThrottling.ThermalSensor2TargetWidth = ThermalConfig->DmiHaAWC.TS2TW;
      DmiThermalThrottling.ThermalSensor3TargetWidth = ThermalConfig->DmiHaAWC.TS3TW;
      PchDmiSetCustomThermalThrottling (DmiThermalThrottling);
    }
  }
}

/**
  Configure PCH DMI 10 Bit Tag and Scaled Flow Control.
  Applicable from SIP 17.
**/
VOID
ConfigurePchDmi1710BitTagAndScaledFlowControl (
  VOID
  )
{
  UINT32         Data32And;
  UINT32         Data32Or;

  Data32And = (UINT32) ~(B_PCH_DMI17_PCR_DCAP2_PX10BTRS | B_PCH_DMI17_PCR_DCAP2_PX10BTCS);
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_DCAP2,
    Data32And,
    0
    );

  Data32And = (UINT32) ~B_PCH_DMI17_PCR_DCTL2_PX10BTRE;
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCTL2, Data32And, 0);
  //
  // Configure Power Gating Clock Wake Control
  //
  Data32And = (UINT32) ~(B_PCH_DMI17_PCR_ADVMCTRL_F10BTSE | B_PCH_DMI17_PCR_ADVMCTRL_CCBE | B_PCH_DMI17_PCR_ADVMCTRL_PMREQCWC | B_PCH_DMI17_PCR_ADVMCTRL_PMREQBLKPGRSPT);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_ADVMCTRL_PMREQCWC << B_PCH_DMI17_PCR_ADVMCTRL_PMREQCWC_OFFSET) |
                       (V_PCH_DMI17_PCR_ADVMCTRL_PMREQBLKPGRSPT << B_PCH_DMI17_PCR_ADVMCTRL_PMREQBLKPGRSPT_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_ADVMCTRL, Data32And, Data32Or);

  Data32Or = (UINT32) B_PCH_DMI17_PCR_DLFCAP_LSFCS;
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DLFCAP, ~0u, Data32Or);
}

/**
  This function performs disabling of PCH DMI TC aliasing.
  Applicable from SIP 17
**/
VOID
DisablePchDmi17TCAliasing (
  VOID
  )
{
  //
  // Affects only VC1
  // Cycles to IOSF remap TC to '0000'
  // Cycles from IOSF remap TC to '0001'
  // Configure Un-Squelch Sampling period
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PCIEDBG,
    (UINT32)~(B_PCH_DMI17_PCR_PCIEDBG_USSP_MASK | B_PCH_DMI17_PCR_PCIEDBG_LGCLKSQEXITDBTIMERS_MASK | B_PCH_DMI17_PCR_PCIEDBG_SPCE),
    (UINT32)((V_PCH_DMI17_PCR_PCIEDBG_USSP << B_PCH_DMI17_PCR_PCIEDBG_USSP_OFFSET) |
             (V_PCH_DMI17_PCR_PCIEDBG_LGCLKSQEXITDBTIMERS << B_PCH_DMI17_PCR_PCIEDBG_LGCLKSQEXITDBTIMERS_OFFSET) |
             B_PCH_DMI17_PCR_PCIEDBG_DTCA)
    );
}

/**
  This function performs programing of registers for Elastic Buffer.
  Applicable from SIP 17
**/
VOID
EFIAPI
ConfigurePchDmi17ElasticBuffer (
  VOID
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_ACGR3S2_G1EBM | B_PCH_DMI17_PCR_ACGR3S2_G2EBM |
                        B_PCH_DMI17_PCR_ACGR3S2_G3EBM | B_PCH_DMI17_PCR_ACGR3S2_LSTPTLS_MASK);
  Data32Or = (UINT32)(B_PCH_DMI17_PCR_ACGR3S2_UPL1EPC | B_PCH_DMI17_PCR_ACGR3S2_G1EBM | B_PCH_DMI17_PCR_ACGR3S2_G2EBM |
                      B_PCH_DMI17_PCR_ACGR3S2_G3EBM | B_PCH_DMI17_PCR_ACGR3S2_G4EBM | B_PCH_DMI17_PCR_ACGR3S2_SRT |
                      (V_PCH_DMI17_PCR_ACGR3S2_LSTPTLS << B_PCH_DMI17_PCR_ACGR3S2_LSTPTLS_OFFSET));

  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_ACGR3S2,
    Data32And,
    Data32Or
    );
}

/**
  This function configures DMI Server Error Reporting Mode
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17Serm (
  VOID
  )
{
  UINT32     Data32And;

  //
  // Configure SERM and PCIe Interrupt Pin
  //
  Data32And = (PchPcrRead32 (PID_DMI, R_PCH_DMI17_PCR_STRPFUSECFG) & (UINT32)~B_PCH_DMI17_PCR_STRPFUSECFG_SERM) | B_PCH_DMI17_PCR_STRPFUSECFG_PXIP_MASK;
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_STRPFUSECFG,
    Data32And,
    V_PCH_DMI17_PCR_STRPFUSECFG_PXIP << B_PCH_DMI17_PCR_STRPFUSECFG_PXIP_OFFSET
    );
}

/**
  This function sets PCH DMI Local and Remote Transmitter
  Link Equalization Preset/Coefficient Evaluation Bypass,
  and performs Precursor and Postcursor Preset/Coefficient
  mapping initialization.
**/
VOID
ConfigurePchDmiPresetCoefficientInitialization (
  BOOLEAN    TestEnable
  )
{
  UINT32     Data32And;
  UINT32     Data32Or;

  Data32And = (UINT32)~(0);
  Data32Or = (UINT32)(0);
  if (IsAdlPch () && IsPchS ()) {
    Data32And = (UINT32)~(B_PCH_DMI17_PCR_RTPCL1_PCM |
                B_PCH_DMI17_PCR_RTPCL1_RTPRECL2PL4 |
                B_PCH_DMI17_PCR_RTPCL1_RTPOSTCL1PL3 |
                B_PCH_DMI17_PCR_RTPCL1_RTPRECL1PL2 |
                B_PCH_DMI17_PCR_RTPCL1_RTPOSTCL0PL1 |
                B_PCH_DMI17_PCR_RTPCL1_RTPRECL0PL0
                );

    Data32Or = (UINT32) (( 6 << N_PCH_DMI17_PCR_RTPCL1_RTPRECL2PL4) |
                (5 << N_PCH_DMI17_PCR_RTPCL1_RTPOSTCL1PL3) |
                (4 << N_PCH_DMI17_PCR_RTPCL1_RTPRECL1PL2) |
                (1 << N_PCH_DMI17_PCR_RTPCL1_RTPOSTCL0PL1) |
                (0 << N_PCH_DMI17_PCR_RTPCL1_RTPRECL0PL0)
                );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_RTPCL1,
      Data32And,
      Data32Or
      );

    Data32And = (UINT32)~(B_PCH_DMI17_PCR_RTPCL2_RTPOSTCL2PL5 |
                          B_PCH_DMI17_PCR_RTPCL2_RTPRECL3PL6 |
                          B_PCH_DMI17_PCR_RTPCL2_RTPOSTCL3PL7
                          );

    Data32Or = (UINT32) (( 7 << N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL2PL5) |
                         ( 9 << N_PCH_DMI17_PCR_RTPCL2_RTPRECL3PL6 ) |
                         ( 8 << N_PCH_DMI17_PCR_RTPCL2_RTPOSTCL3PL7)
                         );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_RTPCL2,
      Data32And,
      Data32Or
      );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_HAEQ,
      (UINT32)~(B_PCH_DMI17_PCR_HAEQ_HAPCCPI_MASK | B_PCH_DMI17_PCR_HAEQ_MACFOMC | B_PCH_DMI17_PCR_HAEQ_DL_MASK),
      (UINT32)((V_PCH_DMI17_PCR_HAEQ_HAPCCPI << B_PCH_DMI17_PCR_HAEQ_HAPCCPI_OFFSET) |
       (V_PCH_DMI17_PCR_HAEQ_DL << B_PCH_DMI17_PCR_HAEQ_DL_OFFSET))
      );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_EQCFG2,
      (UINT32)~(B_PCH_DMI17_PCR_EQCFG2_PCET_MASK | B_PCH_DMI17_PCR_EQCFG2_HAPCSB_MASK | B_PCH_DMI17_PCR_EQCFG2_REWMET_MASK),
      (UINT32)((V_PCH_DMI17_PCR_EQCFG2_PCET << B_PCH_DMI17_PCR_EQCFG2_PCET_OFFSET) |
       (V_PCH_DMI17_PCR_EQCFG2_HAPCSB << B_PCH_DMI17_PCR_EQCFG2_HAPCSB_OFFSET) |
       (V_PCH_DMI17_PCR_EQCFG2_REWMET << B_PCH_DMI17_PCR_EQCFG2_REWMET_OFFSET))
      );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_EQCFG4,
      (UINT32)~(B_PCH_DMI17_PCR_EQCFG4_PX16GRWTNEVE_MASK | B_PCH_DMI17_PCR_EQCFG4_PX16GHAPCCPI_MASK),
      (UINT32)((V_PCH_DMI17_PCR_EQCFG4_PX16GRWTNEVE << B_PCH_DMI17_PCR_EQCFG4_PX16GRWTNEVE_OFFSET) |
       (V_PCH_DMI17_PCR_EQCFG4_PX16GHAPCCPI << B_PCH_DMI17_PCR_EQCFG4_PX16GHAPCCPI_OFFSET))
      );

    Data32And = (UINT32)~(N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL2PL4 |
                B_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL1PL3 |
                B_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL1PL2 |
                B_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL0PL1 |
                B_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL0PL0
                );

    Data32Or = (UINT32) (B_PCH_DMI17_PCR_PX16GRTPCL1_PCM |
                (7 << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL2PL4) |
                (5 << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL1PL3) |
                (4 << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL1PL2) |
                (1 << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPOSTCL0PL1) |
                (0 << N_PCH_DMI17_PCR_PX16GRTPCL1_RTPRECL0PL0)
                );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_PX16GRTPCL1,
      Data32And,
      Data32Or
      );

    Data32And = (UINT32)~(B_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL2PL5 |
                          B_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL3PL6 |
                          B_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL3PL7
                          );

    Data32Or = (UINT32) (( 8 << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL2PL5) |
                         ( 9 << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPRECL3PL6 ) |
                         ( 0 << N_PCH_DMI17_PCR_PX16GRTPCL2_RTPOSTCL3PL7)
                         );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_PX16GRTPCL2,
      Data32And,
      Data32Or
      );

    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_EQCFG5,
      (UINT32)~(B_PCH_DMI17_PCR_EQCFG5_PCET_MASK | B_PCH_DMI17_PCR_EQCFG5_HAPCSB_MASK),
      (UINT32)((V_PCH_DMI17_PCR_EQCFG5_PCET << B_PCH_DMI17_PCR_EQCFG5_PCET_OFFSET) |
       (V_PCH_DMI17_PCR_EQCFG5_HAPCSB << B_PCH_DMI17_PCR_EQCFG5_HAPCSB_OFFSET))
      );
    //
    // Set PCR[DMI] + Offset 2454h
    //
    Data32And = (UINT32) ~(B_PCH_DMI17_PCR_EQCFG1_TUPP | B_PCH_DMI17_PCR_EQCFG1_LERSMIE | B_PCH_DMI17_PCR_EQCFG1_RWTNEVE_MASK | B_PCH_DMI17_PCR_EQCFG1_HAPCCPIE);
    Data32Or = (UINT32) (B_PCH_DMI17_PCR_EQCFG1_RTPCOE | (V_PCH_DMI17_PCR_EQCFG1_RWTNEVE << B_PCH_DMI17_PCR_EQCFG1_RWTNEVE_OFFSET));
  }
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_EQCFG1,
    Data32And,
    Data32Or
    );
}

/**
  This function performs configuration of miscellaneous SIP 17 DMI registers
**/
VOID
ConfigurePchDmi17MiscConfiguration (
  VOID
  )
{
  UINT32    Data32And;
  UINT32    Data32Or;

  Data32And = (UINT32) ~(B_PCH_DMI17_PCR_PCIECFG2_RLLG3R | B_PCH_DMI17_PCR_PCIECFG2_PMET_MASK);
  Data32Or = (UINT32) (B_PCH_DMI17_PCR_PCIECFG2_CRSREN | (V_PCH_DMI17_PCR_PCIECFG2_PMET << B_PCH_DMI17_PCR_PCIECFG2_PMET_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PCIECFG2, Data32And, Data32Or);

  //
  // Configure Rx Delay
  //
  Data32And = (UINT32) ~B_PCH_DMI17_PCR_CCFG_UNRD_MASK;
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_CCFG, Data32And, 0u);

  //
  // Configure Completion Ordering Mode
  //
  Data32And = (UINT32) ~(B_PCH_DMI17_PCR_DC_DCT1C |B_PCH_DMI17_PCR_DC_DCT0C | B_PCH_DMI17_PCR_DC_COM);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DC, Data32And, 0u);

  Data32And = (UINT32) ~(B_PCH_DMI17_PCR_PCIEALC_ITLRCLD | B_PCH_DMI17_PCR_PCIEALC_ILLRCLD |
                         B_PCH_DMI17_PCR_PCIEALC_SSRLD | B_PCH_DMI17_PCR_PCIEALC_SSRRS |
                         B_PCH_DMI17_PCR_PCIEALC_PDSP);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PCIEALC, Data32And, 0u);

  //
  // Configure Peer Disable
  //
  Data32And = (UINT32) ~(B_PCH_DMI17_PCR_CCFG_UPSD);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_CCFG, Data32And, B_PCH_DMI17_PCR_CCFG_UPMWPD);

  Data32And = (UINT32) ~B_PCH_DMI17_PCR_PCIEDBG_LDSWQRP;
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PCIEDBG, Data32And, B_PCH_DMI17_PCR_PCIEDBG_CTONFAE);

  Data32And = (UINT32) ~(B_PCH_DMI17_PCR_ADVMCTRL_ACCRM | B_PCH_DMI17_PCR_ADVMCTRL_RXL0DC | B_PCH_DMI17_PCR_ADVMCTRL_RRLLCL | B_PCH_DMI17_PCR_ADVMCTRL_RLLG12R);
  Data32Or = (UINT32)(B_PCH_DMI17_PCR_ADVMCTRL_INRXL0CTRL | B_PCH_DMI17_PCR_ADVMCTRL_EIOSDISDS | B_PCH_DMI17_PCR_ADVMCTRL_EIOSMASKRX | B_PCH_DMI17_PCR_ADVMCTRL_G3STFER);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_ADVMCTRL, Data32And, Data32Or);

  //
  // Configure Error Injection
  //
  Data32Or = (UINT32) B_PCH_DMI17_PCR_EINJCTL_EINJDIS;
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_EINJCTL, ~0u, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_MPC_CCEL_MASK | B_PCH_DMI17_PCR_MPC_IRRCE);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_MPC_CCEL << B_PCH_DMI17_PCR_MPC_CCEL_OFFSET) | B_PCH_DMI17_PCR_MPC_BT);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_MPC, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_AECH_NCO_MASK | B_PCH_DMI17_PCR_AECH_CV_MASK | B_PCH_DMI17_PCR_AECH_CID_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_AECH_NCO << B_PCH_DMI17_PCR_AECH_NCO_OFFSET) |
                       (V_PCH_DMI17_PCR_AECH_CV << B_PCH_DMI17_PCR_AECH_CV_OFFSET) |
                       (V_PCH_DMI17_PCR_AECH_CID << B_PCH_DMI17_PCR_AECH_CID_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_AECH, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_SPEECH_NCO_MASK | B_PCH_DMI17_PCR_SPEECH_CV_MASK | B_PCH_DMI17_PCR_SPEECH_PCIEECID_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_SPEECH_NCO << B_PCH_DMI17_PCR_SPEECH_NCO_OFFSET) |
                       (V_PCH_DMI17_PCR_SPEECH_CV << B_PCH_DMI17_PCR_SPEECH_CV_OFFSET) |
                       (V_PCH_DMI17_PCR_SPEECH_PCIEECID << B_PCH_DMI17_PCR_SPEECH_PCIEECID_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_SPEECH, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_DLFECH_NCO_MASK | B_PCH_DMI17_PCR_DLFECH_CV_MASK | B_PCH_DMI17_PCR_DLFECH_PCIECID_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_DLFECH_NCO << B_PCH_DMI17_PCR_DLFECH_NCO_OFFSET) |
                       (V_PCH_DMI17_PCR_DLFECH_CV << B_PCH_DMI17_PCR_DLFECH_CV_OFFSET) |
                       (V_PCH_DMI17_PCR_DLFECH_PCIECID << B_PCH_DMI17_PCR_DLFECH_PCIECID_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DLFECH, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_PL16GECH_NCO_MASK | B_PCH_DMI17_PCR_PL16GECH_CV_MASK | B_PCH_DMI17_PCR_PL16GECH_PCIECID_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_PL16GECH_NCO << B_PCH_DMI17_PCR_PL16GECH_NCO_OFFSET) |
                       (V_PCH_DMI17_PCR_PL16GECH_CV << B_PCH_DMI17_PCR_PL16GECH_CV_OFFSET) |
                       (V_PCH_DMI17_PCR_PL16GECH_PCIECID << B_PCH_DMI17_PCR_PL16GECH_PCIECID_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PL16GECH, Data32And, Data32Or);

  Data32And = (UINT32)~B_PCH_DMI17_PCR_RXL0SESQCP1_SSMFRXL0S_MASK;
  Data32Or = (UINT32) (V_PCH_DMI17_PCR_RXL0SESQCP1_SSMFRXL0S << B_PCH_DMI17_PCR_RXL0SESQCP1_SSMFRXL0S_OFFSET);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_RXL0SESQCP1, Data32And, Data32Or);

  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PIDECCTL, ~0u, B_PCH_DMI17_PCR_PIDECCTL_CPLBNCHK);
}

/**
  This function performs configuration of DMI Reset Prep Settings.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17ResetPrepSettings (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_RPR_S5SMTO_MASK | B_PCH_DMI17_PCR_RPR_S4SMTO_MASK |
                        B_PCH_DMI17_PCR_RPR_S3SMTO_MASK | B_PCH_DMI17_PCR_RPR_CRMTO_MASK |
                        B_PCH_DMI17_PCR_RPR_WRMTO_MASK | B_PCH_DMI17_PCR_RPR_DMTO_MASK |
                        B_PCH_DMI17_PCR_RPR_S5SM_MASK | B_PCH_DMI17_PCR_RPR_S4SM_MASK |
                        B_PCH_DMI17_PCR_RPR_S3SM_MASK | B_PCH_DMI17_PCR_RPR_CRM_MASK |
                        B_PCH_DMI17_PCR_RPR_WRM_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_RPR_S5SMTO << B_PCH_DMI17_PCR_RPR_S5SMTO_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_S4SMTO << B_PCH_DMI17_PCR_RPR_S4SMTO_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_S3SMTO << B_PCH_DMI17_PCR_RPR_S3SMTO_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_CRMTO << B_PCH_DMI17_PCR_RPR_CRMTO_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_WRMTO << B_PCH_DMI17_PCR_RPR_WRMTO_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_DMTO << B_PCH_DMI17_PCR_RPR_DMTO_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_S5SM << B_PCH_DMI17_PCR_RPR_S5SM_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_S4SM << B_PCH_DMI17_PCR_RPR_S4SM_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_S3SM << B_PCH_DMI17_PCR_RPR_S3SM_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_CRM << B_PCH_DMI17_PCR_RPR_CRM_OFFSET) |
                       (V_PCH_DMI17_PCR_RPR_WRM << B_PCH_DMI17_PCR_RPR_WRM_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_RPR, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_RPDEC1_RPCREPT_MASK | B_PCH_DMI17_PCR_RPDEC1_RPCRERT_MASK |
                        B_PCH_DMI17_PCR_RPDEC1_RPWREPT_MASK | B_PCH_DMI17_PCR_RPDEC1_RPWRERT_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_RPDEC1_RPCREPT << B_PCH_DMI17_PCR_RPDEC1_RPCREPT_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC1_RPCRERT << B_PCH_DMI17_PCR_RPDEC1_RPCRERT_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC1_RPWREPT << B_PCH_DMI17_PCR_RPDEC1_RPWREPT_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC1_RPWRERT << B_PCH_DMI17_PCR_RPDEC1_RPWRERT_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_RPDEC1, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_RPDEC2_RPS4EPT_MASK | B_PCH_DMI17_PCR_RPDEC2_RPS4ERT_MASK |
                        B_PCH_DMI17_PCR_RPDEC2_RPS3EPT_MASK | B_PCH_DMI17_PCR_RPDEC2_RPS3ERT_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_RPDEC2_RPS4EPT << B_PCH_DMI17_PCR_RPDEC2_RPS4EPT_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC2_RPS4ERT << B_PCH_DMI17_PCR_RPDEC2_RPS4ERT_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC2_RPS3EPT << B_PCH_DMI17_PCR_RPDEC2_RPS3EPT_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC2_RPS3ERT << B_PCH_DMI17_PCR_RPDEC2_RPS3ERT_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_RPDEC2, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_RPDEC3_RPDH_MASK | B_PCH_DMI17_PCR_RPDEC3_RPS5EPT_MASK |
                        B_PCH_DMI17_PCR_RPDEC3_RPS5ERT_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_RPDEC3_RPDH << B_PCH_DMI17_PCR_RPDEC3_RPDH_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC3_RPS5EPT << B_PCH_DMI17_PCR_RPDEC3_RPS5EPT_OFFSET) |
                       (V_PCH_DMI17_PCR_RPDEC3_RPS5ERT << B_PCH_DMI17_PCR_RPDEC3_RPS5ERT_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_RPDEC3, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_FCTL2_HRTCTL_MASK | B_PCH_DMI17_PCR_FCTL2_RPTOT_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_FCTL2_HRTCTL << B_PCH_DMI17_PCR_FCTL2_HRTCTL_OFFSET) |
                       (V_PCH_DMI17_PCR_FCTL2_RPTOT << B_PCH_DMI17_PCR_FCTL2_RPTOT_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_FCTL2, Data32And, Data32Or);
}

/**
  This function performs configuration of DMI Rx Margining.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17RxMargining (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_RXMC1_MSRVS_MASK | B_PCH_DMI17_PCR_RXMC1_MSRTS_MASK |
                        B_PCH_DMI17_PCR_RXMC1_TMSLOP_MASK | B_PCH_DMI17_PCR_RXMC1_VMSLOP_MASK |
                        B_PCH_DMI17_PCR_RXMC1_MMNOLS_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_RXMC1_MSRVS << B_PCH_DMI17_PCR_RXMC1_MSRVS_OFFSET) |
                       (V_PCH_DMI17_PCR_RXMC1_MSRTS << B_PCH_DMI17_PCR_RXMC1_MSRTS_OFFSET) |
                       (V_PCH_DMI17_PCR_RXMC1_TMSLOP << B_PCH_DMI17_PCR_RXMC1_TMSLOP_OFFSET) |
                       (V_PCH_DMI17_PCR_RXMC1_VMSLOP << B_PCH_DMI17_PCR_RXMC1_VMSLOP_OFFSET) |
                       (V_PCH_DMI17_PCR_RXMC1_MMNOLS << B_PCH_DMI17_PCR_RXMC1_MMNOLS_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_RXMC1, Data32And, Data32Or);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_RXMC2_MNOTSS_MASK | B_PCH_DMI17_PCR_RXMC2_MMTOS_MASK |
                        B_PCH_DMI17_PCR_RXMC2_MNOVSS_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_RXMC2_MNOTSS << B_PCH_DMI17_PCR_RXMC2_MNOTSS_OFFSET) |
                       (V_PCH_DMI17_PCR_RXMC2_MMTOS << B_PCH_DMI17_PCR_RXMC2_MMTOS_OFFSET) |
                       (V_PCH_DMI17_PCR_RXMC2_MNOVSS << B_PCH_DMI17_PCR_RXMC2_MNOVSS_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_RXMC2, Data32And, Data32Or);
}

/**
  This function performs configuration of DMI Phy.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17Phy (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_CTRL2, ~0u, B_PCH_DMI17_PCR_CTRL2_HALEP);

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_LPHYCP4_LGUSSP_MASK | B_PCH_DMI17_PCR_LPHYCP4_OSCCLKSQEXITDBTIMERS_MASK |
                        B_PCH_DMI17_PCR_LPHYCP4_LGCLKSQEXITDBTIMERS_MASK);
  Data32Or = (UINT32) (B_PCH_DMI17_PCR_LPHYCP4_DPMDNDBFE | B_PCH_DMI17_PCR_LPHYCP4_PLLBUSDRC |
                       (V_PCH_DMI17_PCR_LPHYCP4_LGUSSP << B_PCH_DMI17_PCR_LPHYCP4_LGUSSP_OFFSET) |
                       (V_PCH_DMI17_PCR_LPHYCP4_OSCCLKSQEXITDBTIMERS << B_PCH_DMI17_PCR_LPHYCP4_OSCCLKSQEXITDBTIMERS_OFFSET) |
                       (V_PCH_DMI17_PCR_LPHYCP4_LGCLKSQEXITDBTIMERS << B_PCH_DMI17_PCR_LPHYCP4_LGCLKSQEXITDBTIMERS_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_LPHYCP4, Data32And, Data32Or);
}

/**
  This function performs configuration of DMI Phy Recalibration.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17PhyRecal (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_IORCCP1_G4ORRRXECC_MASK | B_PCH_DMI17_PCR_IORCCP1_G3ORRRXECC_MASK |
                        B_PCH_DMI17_PCR_IORCCP1_G2ORRRXECC_MASK | B_PCH_DMI17_PCR_IORCCP1_G1ORRRXECC_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_IORCCP1_G4ORRRXECC << B_PCH_DMI17_PCR_IORCCP1_G4ORRRXECC_OFFSET) |
                       (V_PCH_DMI17_PCR_IORCCP1_G3ORRRXECC << B_PCH_DMI17_PCR_IORCCP1_G3ORRRXECC_OFFSET) |
                       (V_PCH_DMI17_PCR_IORCCP1_G2ORRRXECC << B_PCH_DMI17_PCR_IORCCP1_G2ORRRXECC_OFFSET) |
                       (V_PCH_DMI17_PCR_IORCCP1_G1ORRRXECC << B_PCH_DMI17_PCR_IORCCP1_G1ORRRXECC_OFFSET) |
                        B_PCH_DMI17_PCR_IORCCP1_DISORCRODI | B_PCH_DMI17_PCR_IORCCP1_DRCORRP | B_PCH_DMI17_PCR_IORCCP1_DISORCL12REC);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_IORCCP1, Data32And, Data32Or);
}

/**
  This function performs configuration of DMI L1 Support.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17L1Support (
  VOID
  )
{
  UINT32   Data32Or;

  Data32Or = (UINT32) (B_PCH_DMI17_PCR_UPDLPHYCP_UPDLTCDC | B_PCH_DMI17_PCR_UPDLPHYCP_UPDLLSMFLD);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_UPDLPHYCP, ~0u, Data32Or);
}

/**
  This function performs configuration of DMI D3 Vnn removal.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17D3VnnRemCtl (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  Data32And = (UINT32)~(B_PCH_DMI17_PCR_VNNREMCTL_ISPLFVNNRE_MASK | B_PCH_DMI17_PCR_VNNREMCTL_LRSLFVNNRE_MASK);
  Data32Or = (UINT32) ((V_PCH_DMI17_PCR_VNNREMCTL_ISPLFVNNRE_8CLKS << B_PCH_DMI17_PCR_VNNREMCTL_ISPLFVNNRE_OFFSET) |
                       (V_PCH_DMI17_PCR_VNNREMCTL_LRSLFVNNRE_4CLKS << B_PCH_DMI17_PCR_VNNREMCTL_LRSLFVNNRE_OFFSET));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_VNNREMCTL, Data32And, Data32Or);
}

/**
  This function performs configuration of Master Cycle Decoding.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17RxMasterCycleDecoding (
  VOID
  )
{
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DECCTL, (UINT32)~(B_PCH_DMI17_PCR_DECCTL_URRXMCTPNTCO), 0u);

  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_PIDECCTL, ~0u, (UINT32)(B_PCH_DMI17_PCR_PIDECCTL_CPLBNCHK));
}

/**
  This function performs configuration of DMI Max Payload Size.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17MPS (
  VOID
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  Data32And = B_PCH_DMI17_PCR_DCAP_MPS_MASK;
  Data32Or = V_PCH_DMI17_PCR_DCAP_MPS_256 << B_PCH_DMI17_PCR_DCAP_MPS_OFFSET;
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCAP, Data32And, Data32Or);

  Data32And = B_PCH_DMI17_PCR_DCTL_MPS_MASK;
  Data32Or = (UINT32)((V_PCH_DMI17_PCR_DCTL_MPS_256 << B_PCH_DMI17_PCR_DCTL_MPS_OFFSET) |
                      (B_PCH_DMI17_PCR_DCTL_URE | B_PCH_DMI17_PCR_DCTL_NEE | B_PCH_DMI17_PCR_DCTL_NFE | B_PCH_DMI17_PCR_DCTL_CEE));
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_DCTL, Data32And, Data32Or);

  Data32And = (UINT32) ~B_PCH_DMI17_PCR_IPCS_IMPS_MASK;
  Data32Or = (V_PCH_DMI17_PCR_IPCS_IMPS << B_PCH_DMI17_PCR_IPCS_IMPS_OFFSET);
  PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_IPCS, Data32And, Data32Or);
}

/**
  This function performs configuration of DMI Coalescing.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17Coalescing (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_COCTL,
    (UINT32)~(B_PCH_DMI17_PCR_COCTL_NPCLM_MASK | B_PCH_DMI17_PCR_COCTL_PCLM_MASK | B_PCH_DMI17_PCR_COCTL_ROAOP),
    (UINT32)((V_PCH_DMI17_PCR_COCTL_NPCLM << B_PCH_DMI17_PCR_COCTL_NPCLM_OFFSET) | (V_PCH_DMI17_PCR_COCTL_PCLM << B_PCH_DMI17_PCR_COCTL_PCLM_OFFSET))
    );

  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_ACRG3,
    (UINT32) ~(B_PCH_DMI17_PCR_ACRG3_RRXDME | B_PCH_DMI17_PCR_ACRG3_CBGM | B_PCH_DMI17_PCR_ACRG3_ADESKEW_DIS),
    0u
    );
}

/**
  This function performs configuration of DMI SERR# Enable.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17SERREnable (
  VOID
  )
{
  PchPcrAndThenOr16 (
    PID_DMI, R_PCH_DMI17_PCR_CMD,
    (UINT16)~0,
    (UINT16)(B_PCH_DMI17_PCR_CMD_SEE)
    );

  PchPcrAndThenOr8 (
    PID_DMI, R_PCH_DMI17_PCR_BCTRL,
    (UINT8)~0,
    (UINT8)(B_PCH_DMI17_PCR_BCTRL_SE)
    );
}

/**
  This function performs configuration of DMI VC Rx flow control.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17VcRxFlowControl (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_RXQC,
    (UINT32)~(B_PCH_DMI17_PCR_RXQC_VMRQNPCOC_MASK | B_PCH_DMI17_PCR_RXQC_VMRQPCOC_MASK | B_PCH_DMI17_PCR_RXQC_V1RQNPCOC_MASK |
              B_PCH_DMI17_PCR_RXQC_V1RQPCOC_MASK | B_PCH_DMI17_PCR_RXQC_V0RQNPCOC_MASK | B_PCH_DMI17_PCR_RXQC_V0RQPCOC_MASK),
    (UINT32)((V_PCH_DMI17_PCR_RXQC_VMRQNPCOC << B_PCH_DMI17_PCR_RXQC_VMRQNPCOC_OFFSET) |
             (V_PCH_DMI17_PCR_RXQC_VMRQPCOC << B_PCH_DMI17_PCR_RXQC_VMRQPCOC_OFFSET) |
             (V_PCH_DMI17_PCR_RXQC_V1RQNPCOC << B_PCH_DMI17_PCR_RXQC_V1RQNPCOC_OFFSET) |
             (V_PCH_DMI17_PCR_RXQC_V1RQPCOC << B_PCH_DMI17_PCR_RXQC_V1RQPCOC_OFFSET) |
             (V_PCH_DMI17_PCR_RXQC_V0RQNPCOC << B_PCH_DMI17_PCR_RXQC_V0RQNPCOC_OFFSET) |
             (V_PCH_DMI17_PCR_RXQC_V0RQPCOC << B_PCH_DMI17_PCR_RXQC_V0RQPCOC_OFFSET))
    );
}


/**
  This function performs configuration of DMI Powerdown Mapping.
  Applicable from SIP 17
**/
VOID
ConfigurePchDmi17PowerdownMapping (
  VOID
  )
{
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PIPEPDCTL,
    (UINT32)~(B_PCH_DMI17_PCR_PIPEPDCTL_L1PGUPGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL_L1PGNOPGPDCTL_MASK |
              B_PCH_DMI17_PCR_PIPEPDCTL_DISPGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL_DISNOPGPDCTL_MASK |
              B_PCH_DMI17_PCR_PIPEPDCTL_L23PGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL_L23NOPGPDCTL_MASK |
              B_PCH_DMI17_PCR_PIPEPDCTL_DETPGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL_DETNOPGPDCTL_MASK),
    (UINT32)((V_PCH_DMI17_PCR_PIPEPDCTL_L1PGUPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_L1PGUPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL_L1PGNOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_L1PGNOPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL_DISPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_DISPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL_DISNOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_DISNOPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL_L23PGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_L23PGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL_L23NOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_L23NOPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL_DETPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_DETPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL_DETNOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL_DETNOPGPDCTL_OFFSET))
    );

  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PIPEPDCTL2,
    (UINT32)~(B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGPGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGNOPGPDCTL_MASK |
              B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2PGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2NOPGPDCTL_MASK |
              B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGPGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGNOPGPDCTL_MASK |
              B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1UPUPGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL2_L1UPNOPGPDCTL_MASK),
    (UINT32)((V_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGNOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_DUCFGNOPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL2_L1D2PGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2PGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL2_L1D2NOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_L1D2NOPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGNOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1PGNOPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL2_L1D1UPUPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_L1D1UPUPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL2_L1UPNOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL2_L1UPNOPGPDCTL_OFFSET))
    );

  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PIPEPDCTL3,
    (UINT32)~(B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWPGPDCTL_MASK | B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWNOPGPDCTL_MASK),
    (UINT32)((V_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWPGPDCTL_OFFSET) |
             (V_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWNOPGPDCTL << B_PCH_DMI17_PCR_PIPEPDCTL3_L1DLOWNOPGPDCTL_OFFSET))
    );

  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI17_PCR_PIPEPDCTLEXT,
    (UINT32)~(B_PCH_DMI17_PCR_PIPEPDCTLEXT_P2TP2TCD | B_PCH_DMI17_PCR_PIPEPDCTLEXT_P2TP2T),
    0u)
    ;
}

/**
  This function performs early PCH-H DMI SIP15 init.
  It must be called before any upstream bus master transactions are enabled
**/
VOID
PchDmi15EarlyInit (
  VOID
  )
{
  //
  // Enable Transaction Layer Packet Fast Transmit Mode and
  // DMI Credit Allocated Update Mode
  //
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_MPC2,
    ~0u,
    B_PCH_DMI15_PCR_MPC2_TLPF | B_PCH_DMI15_PCR_MPC2_CAM
    );
}


/**
  This function configures DMI SIP15.

  @param[in] SiPolicyPpi The PCH Policy PPI instance
**/
VOID
PchDmi15Init (
  IN  SI_POLICY_PPI  *SiPolicyPpi
  )
{
  PchDmiBasicInit ();
  ConfigurePchDmiClockGating ();
  ConfigurePchDmiLinkPowerManagement (SiPolicyPpi);
  ConfigurePchDmiThermalThrottling (SiPolicyPpi);
  ConfigurePchDmiSquelchPowerManagement ();
  ConfigurePchDmiPllShutdown ();
  ConfigurePchDmiPowerGating (SiPolicyPpi);
  ConfigurePchDmiAspm (SiPolicyPpi);
  if (IsAdlPch () && IsPchS ()) {
    ConfigurePchDmi17VcRxFlowControl ();
    ConfigurePchDmi17Serm ();
    ConfigurePchDmi17SERREnable ();
    ConfigurePchDmi17ElasticBuffer ();
    ConfigurePchDmi17L1Support ();
    ConfigurePchDmi17D3VnnRemCtl ();
    ConfigurePchDmi17MPS ();
    ConfigurePchDmi17Coalescing ();
    ConfigurePchDmi17RxMargining ();
    ConfigurePchDmi17ResetPrepSettings ();
    ConfigurePchDmi17PowerdownMapping ();
    ConfigurePchDmi17Phy ();
    ConfigurePchDmi17PhyRecal ();
    ConfigurePchDmi17MiscConfiguration ();
    ConfigurePchDmi1710BitTagAndScaledFlowControl ();
    DisablePchDmi17TCAliasing ();
    ConfigurePchDmi17RxMasterCycleDecoding ();
  }
}

/**
  This function will perform necessary programming before changing PCH DMI link speed to Gen2 or Gen3.

  @param[in] DmiSpeedGen              PCH DMI speed Gen (1-Gen1, 2-Gen2, 3-Gen3) to which link is going to be trained
**/
STATIC
VOID
PchDmiBeforeLinkSpeedChange (
  UINT8    DmiSpeedGen
  )
{
  UINT32            Data32Or;
  UINT32            Data32And;
  UINT16            Data16Or;
  UINT16            Data16And;
  UINTN             LaneIndex;
  BOOLEAN           TestEnable;

  static CONST UINT8  DmiUpPortTranPreset[] = {
    V_PCH_DMI15_PCR_UPL0TP,
    V_PCH_DMI15_PCR_UPL1TP,
    V_PCH_DMI15_PCR_UPL2TP,
    V_PCH_DMI15_PCR_UPL3TP,
    V_PCH_DMI15_PCR_UPL4TP,
    V_PCH_DMI15_PCR_UPL5TP,
    V_PCH_DMI15_PCR_UPL6TP,
    V_PCH_DMI15_PCR_UPL7TP
  };
  static CONST UINT16 DmiLaneEqualCntrReg[] = {
    R_PCH_DMI15_PCR_L01EC,
    R_PCH_DMI15_PCR_L23EC,
    R_PCH_DMI15_PCR_L45EC,
    R_PCH_DMI15_PCR_L67EC
  };
  static CONST UINT8  DmiGen4UpPortTranPreset[] = {
    V_PCH_DMI17_PCR_GEN4_UPL0TP,
    V_PCH_DMI17_PCR_GEN4_UPL1TP,
    V_PCH_DMI17_PCR_GEN4_UPL2TP,
    V_PCH_DMI17_PCR_GEN4_UPL3TP,
    V_PCH_DMI17_PCR_GEN4_UPL4TP,
    V_PCH_DMI17_PCR_GEN4_UPL5TP,
    V_PCH_DMI17_PCR_GEN4_UPL6TP,
    V_PCH_DMI17_PCR_GEN4_UPL7TP
  };
  static CONST UINT16 DmiGen4LaneEqualCntrReg[] = {
    R_PCH_DMI17_PCR_PL16L01EC,
    R_PCH_DMI17_PCR_PL16L23EC,
    R_PCH_DMI17_PCR_PL16L45EC,
    R_PCH_DMI17_PCR_PL16L67EC
  };
  if (IsAdlPch () && IsPchS ()) {
    //
    // Configure Phy.
    // This programming has to be executed before equalization.
    //
    Data32Or = (UINT32) (B_PCH_DMI17_PCR_LPHYCP1_RXEQFNEVC | B_PCH_DMI17_PCR_LPHYCP1_RXADPHM);
    PchPcrAndThenOr32 (PID_DMI, R_PCH_DMI17_PCR_LPHYCP1, ~0u, Data32Or);
  }

  TestEnable = FALSE;
  if (DmiSpeedGen > 2) {
    for (LaneIndex = 0; LaneIndex < PCH_MAX_DMI_LANES; LaneIndex += 2) {
      Data32And = ~(UINT32) (B_PCH_DMI15_PCR_UPL0246TP | B_PCH_DMI15_PCR_UPL1357TP);
      Data32Or = (((UINT32)DmiUpPortTranPreset[LaneIndex] << N_PCH_DMI15_PCR_UPL0246TP) |
                  ((UINT32)DmiUpPortTranPreset[LaneIndex + 1] << N_PCH_DMI15_PCR_UPL1357TP));

      PchPcrAndThenOr32 (PID_DMI, DmiLaneEqualCntrReg[LaneIndex/2], Data32And, Data32Or);

      if (IsAdlPch () && IsPchS ()) {
        Data16And = (UINT16) ~(B_PCH_DMI17_PCR_GEN4_UPL0246TP | B_PCH_DMI17_PCR_GEN4_UPL1357TP);
        Data16Or = (((UINT16)DmiGen4UpPortTranPreset[LaneIndex] << N_PCH_DMI17_PCR_GEN4_UPL0246TP) |
                    ((UINT16)DmiGen4UpPortTranPreset[LaneIndex + 1] << N_PCH_DMI17_PCR_GEN4_UPL1357TP));

        PchPcrAndThenOr16 (PID_DMI, DmiGen4LaneEqualCntrReg[LaneIndex/2], Data16And, Data16Or);
      }
    }

    ConfigurePchDmiPresetCoefficientInitialization (TestEnable);
    //
    // Before training to GEN3 Configure Lane0 P0-P10 Preset Coefficient mapping
    // based on recommendation for PCH DMI
    //
    if (IsEbgPch()) {
      //
      // EBG PCH
      //
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P0P1PCM,   0x00D1002F);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P1P2P3PCM, 0x37340C8B);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P3P4PCM,   0x0003F200);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P5P6PCM,   0x08DC01B9);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P6P7P8PCM, 0x2F346B00);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P8P9PCM,   0x002F4208);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P10PCM,    0x0001502A);
      //
      // Configure Lane0 Local PHY Full Swing and Low Frequency Value
      //
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0LFFS, 0x00003F15);
    } else if (IsAdlPch () && IsPchS ()) {
      //
      // ADP-S PCH
      //
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0P0P1PCM,   0x0084A01E);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0P1P2P3PCM, 0x23200807);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0P3P4PCM,   0x00028140);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0P5P6PCM,   0x058C0124);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0P6P7P8PCM, 0x1E204700);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0P8P9PCM,   0x001E1145);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0P10PCM,    0x0000D01B);
      //
      // Configure Lane0 Local PHY Full Swing and Low Frequency Value
      //
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_L0LFFS, 0x0000280E);

      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GP0P1PCM,   0x0084A01E);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GP1P2P3PCM, 0x23200807);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GP3P4PCM,   0x00028140);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GP5P6PCM,   0x058C0124);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GP6P7P8PCM, 0x1E204700);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GP8P9PCM,   0x001E1145);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GP10PCM,    0x0000D01B);
      //
      // Configure Lane0 Local PHY Full Swing and Low Frequency Value
      //
      PchPcrWrite32 (PID_DMI, R_PCH_DMI17_PCR_PX16GLFFS, 0x0000280E);
    } else {
      //
      // other PCH
      //
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P0P1PCM,   0x006C8018);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P1P2P3PCM, 0x1C180685);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P3P4PCM,   0x00020100);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P5P6PCM,   0x047000DD);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P6P7P8PCM, 0x181835C0);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P8P9PCM,   0x0015B104);
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0P10PCM,    0x0000A016);
      //
      // Configure Lane0 Local PHY Full Swing and Low Frequency Value
      //
      PchPcrWrite32 (PID_DMI, R_PCH_DMI15_PCR_L0LFFS, 0x0000200A);
    }
  }

  ///
  /// Set  DMI Link Data Rate Sustain Policy (DLDRSP)
  /// When set, DMI will always train to the highest supported link speed common to both sides of the link.
  /// DLDRSP should be set to 1 when the Target Link Speed is still indicating GEN1
  ///
  if (IsAdlPch () && IsPchS ()) {
    Data32Or = B_PCH_DMI15_PCR_CTRL2_DLDRSP | B_PCH_DMI15_PCR_CTRL2_PMETOFD;
    //
    // Program Link Speed Training Policy
    // and initial Link Speed requirement
    // it has to be done before programming of TLS
    //
    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_AECR1G3,
      ~0u,
      (UINT32)(B_PCH_DMI17_PCR_AECR1G3_DTCGCM | B_PCH_DMI17_PCR_AECR1G3_TPSE)
      );
    PchPcrAndThenOr32 (
      PID_DMI, R_PCH_DMI17_PCR_MPC2,
      (UINT32)~B_PCH_DMI17_PCR_MPC2_ORCE_MASK,
      (UINT32)((V_PCH_DMI17_PCR_MPC2_ORCE << B_PCH_DMI17_PCR_MPC2_ORCE_OFFSET) |
               B_PCH_DMI17_PCR_MPC2_PTNFAE | B_PCH_DMI17_PCR_MPC2_LSTP | B_PCH_DMI17_PCR_MPC2_EOIFD)
      );
  } else {
    Data32Or = B_PCH_DMI15_PCR_CTRL2_DLDRSP;
  }
  PchPcrAndThenOr32 (
    PID_DMI, R_PCH_DMI15_PCR_CTRL2,
    ~0u,
    Data32Or
    );
}

/**
  The function set the Target Link Speed in PCH-H to DMI GEN 3.

  @param[in] TargetLinkSpeed        Target Link Speed
                                    2: GEN2
                                    3: GEN3
**/
VOID
PchDmi15SetTargetLinkSpeed (
  IN  UINT8                 TargetLinkSpeed
  )
{

  if (IsAdlPch () && IsPchS ()) {
    if ((TargetLinkSpeed < 2) || (TargetLinkSpeed > 4)) {
      ASSERT (FALSE);
      return;
    }
  } else {
    if ((TargetLinkSpeed < 2) || (TargetLinkSpeed > 3)) {
      ASSERT (FALSE);
      return;
    }
    ASSERT (IsPchWithPdmi ());
  }

  DEBUG ((DEBUG_INFO, "PchDmi15SetTargetLinkSpeed(%d) Start\n", TargetLinkSpeed));

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
  if ((PchPcrRead32 (PID_DMI, R_PCH_DMI_PCR_LCAP) & B_PCH_DMI_PCR_LCAP_MLS) >= TargetLinkSpeed) {
    ///
    /// Set Target Link Speed (TLS)
    ///
    PchPcrAndThenOr8 (PID_DMI, R_PCH_DMI15_PCR_LCTL2, (UINT8)~B_PCH_DMI15_PCR_LCTL2_TLS, TargetLinkSpeed);
    ///
    /// Please refer to the System Agent BIOS Writer's Guide to perform DMI Link Retrain after
    /// configures new DMI Link Speed. (Done in SA code)
    ///
  }
  DEBUG ((DEBUG_INFO, "PchDmi15SetTargetLinkSpeed(%d) End\n", TargetLinkSpeed));
}
