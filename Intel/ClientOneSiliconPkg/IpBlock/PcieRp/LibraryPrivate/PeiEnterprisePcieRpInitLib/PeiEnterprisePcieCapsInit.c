/** @file
  Implementation of the library for enterprise PCIe root ports initialization

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

#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchInfoLib.h>
#include <PchLimits.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PeiPcieRpLib.h>
#include <Library/DebugLib.h>
#include <Register/PchRegs.h>
#include <Library/PchPcieRpLib.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcrRegs.h>
#include <PcieRegs.h>
#include <Library/HobLib.h>
#include <Library/PsfLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/TimerLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/PcieHelperLib.h>
#include <Register/EnterprisePcieRpRegs.h>
#include <PchPcieRpConfig.h>
#include <EnterprisePcieConfig.h>

#include "PeiEnterprisePcieRpInitInternal.h"

#define DEBUG_SUPPORTED(x) DEBUG ((DEBUG_INFO, "%a: supported - %a\n", __FUNCTION__, (x) ? "true" : "false"))

typedef enum {
  L1_NONE,
  L1_P1S,
  L1_P2S
} L1PMSVersion;

/*
  Entry point for setting capabilities list.

  1.  Read and write back to capability registers 34h, 40h, 80h and 88h using byte access.
  2.  Program NSR, A4h[3] = 0b

  @param[in] RpBase  Root Port pci segment base address
*/
VOID
PchPcieCapabilitiesInitEntry (
  IN UINT64 RpBase
  )
{
  UINT32      Data32;
  UINT16      Data16;
  UINT8       Data8;

  ASSERT (RpBase != 0);

  Data8 = PciSegmentRead8 (RpBase + PCI_CAPBILITY_POINTER_OFFSET);
  PciSegmentWrite8 (RpBase + PCI_CAPBILITY_POINTER_OFFSET, Data8);

  Data16 = PciSegmentRead16 (RpBase + R_PCH_PCIE_CFG_EXPCAPLST);
  PciSegmentWrite16 (RpBase + R_PCH_PCIE_CFG_EXPCAPLST, Data16);

  Data16 = PciSegmentRead16 (RpBase + R_PCH_PCIE_CFG_MSICAPLST);
  PciSegmentWrite16 (RpBase + R_PCH_PCIE_CFG_MSICAPLST, Data16);

  Data16 = PciSegmentRead16 (RpBase + R_PCH_PCIE_CFG_SSCAPLST);
  PciSegmentWrite16 (RpBase + R_PCH_PCIE_CFG_SSCAPLST, Data16);

  Data32 = PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_PMCSR);
  Data32 &= (UINT32) ~(B_PCIE_PMCS_NSR);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_PMCSR, Data32);
}

/*
  Helper function to set DPC header.

  @param[in] NextCap    Next capability on list offset
  @param[in] RpBase     Root Port pci segment base address
  @param[in] Supported  DPC capability supported
  @retval UINT16        If capability supported returns this capability offset
*/
STATIC
UINT16
PchPcieCapabilitiesInitDpcHeader (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  )
{
  UINT32 Data32 = 0;
  if (Supported) {
    Data32  = (V_PCH_PCIE_CFG_DPCECH_CV << N_PCIE_EXCAP_CV) | V_PCIE_EX_DPC_CID;
    Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
    NextCap = R_PCH_PCIE_CFG_DPCECH;
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_DPCECH, Data32);

  return NextCap;
}

/*
  Sets Downstream Port Containment capability

  If Downstream Port Containment is enabled, then
    1. Set Next Capability Offset, Dxx:Fn +190h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +190h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +190h[15:0] = 001h
    4. NEXT_CAP = 190h
    ELSE, set Dxx:Fn +190h [31:0] = 0

  @param[in] NextCap            Next capability on list offset
  @param[in] RpBase             Root Port pci segment base address
  @param[in] Supported          Capability supported
  @param[in] DpcWithExtensions  Are DPC Extensions to be enabled
  @retval UINT16                If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitDpcExt (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported,
  IN BOOLEAN DpcWithExtensions
  )
{
  UINT32 Data32 = 0;

  ASSERT (RpBase != 0);

  DEBUG_SUPPORTED (Supported);
  DEBUG ((DEBUG_INFO, "\twith extensions %a\n", DpcWithExtensions ? "supported" : "not supported"));

  NextCap = PchPcieCapabilitiesInitDpcHeader (NextCap, RpBase, Supported);

  /* Enable DPC capability with extensions */
  if (Supported) {
    Data32 = PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_DPCECH + R_PCIE_EX_DPC_CAP_OFFSET);
    if (DpcWithExtensions) {
      Data32 |= BIT5;
    } else {
      Data32 &= ~BIT5;
    }
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_DPCECH + R_PCIE_EX_DPC_CAP_OFFSET, Data32);

  return NextCap;
}

/*
  Sets Secondary PCI Express Extended Capability

  If the RP is GEN3 capable (by fuse and BIOS policy), enable Secondary PCI Express Extended Capability
  1. Set Next Capability Offset, Dxx:Fn +200h[31:20] = NEXT_CAP
  2. Set Capability Version, Dxx:Fn +200h[19:16] = 1h
  3. Set Capability ID, Dxx:Fn +200h[15:0] = 0019h
  4. NEXT_CAP = 200h
  ELSE, set Dxx:Fn +200h [31:0] = 0

  @param[in] NextCap    Next capability on list offset
  @param[in] RpBase     Root Port pci segment base address
  @param[in] Supported  Capability supported
  @retval UINT16        If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitSecExp (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  )
{
  UINT32 Data32 = 0;

  ASSERT (RpBase != 0);

  DEBUG_SUPPORTED (Supported);

  if (Supported) {
    Data32  = (V_PCH_PCIE_CFG_SECEXPCAP_CV << N_PCIE_EXCAP_CV) | V_PCIE_EX_SPE_CID;
    Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
    NextCap = R_PCH_PCIE_CFG_SECEXPCAP;
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_SECEXPCAP, Data32);

  return NextCap;
}

/*
  Helper function to set L1PM Substates capabilitiy header.

  @param[in] NextCap    Next capability on list offset
  @param[in] RpBase     Root Port pci segment base address
  @param[in] Supported  Capability supported
  @retval UINT16        If capability supported returns this capability offset
*/
STATIC
UINT16
PchPcieCapabilitiesInitL1PMSHeader (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  )
{
  UINT32 Data32 = 0;
  if ( Supported ) {
    Data32  = (V_PCH_PCIE_CFG_L1PMSECH_CV << N_PCIE_EXCAP_CV) | V_PCIE_EX_L1S_CID;
    Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
    NextCap = R_PCH_PCIE_CFG_L1PMSECH;
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_L1PMSECH, Data32);

  return NextCap;
}

/*
  Helper function to set L1PM Substates version.

  @param[in] RpBase     Root Port pci segment base address
  @param[in] Version    L1PM Substates version
*/
STATIC
VOID
SetL1PMSVersion(
  IN UINT64 RpBase,
  IN L1PMSVersion Version
  )
{
  UINT32 Data32;

  Data32 = PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_L1PMSCAP);

  switch (Version) {
  case L1_NONE:
    Data32 &= (UINT32) ~(0x1F); // Disable all
    break;
  case L1_P1S:
    Data32 |= (UINT32) (BIT1 | BIT3 | BIT4);
    Data32 &= (UINT32) ~(BIT0 | BIT2); // Disable L1.2 support
    break;
  case L1_P2S:
    // do not disable anything
    // fallthrough
  default:
    Data32 |= (UINT32) (0x1F);
    break;
  }

  //
  // Set TpowerOn capability to 44us
  Data32 &= ~(B_PCIE_EX_L1SCAP_PTV | B_PCIE_EX_L1SCAP_PTPOS);
  Data32 |=  (22 << N_PCIE_EX_L1SCAP_PTV) | (V_PCIE_EX_L1SCAP_PTPOS_2us << N_PCIE_EX_L1SCAP_PTPOS);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_L1PMSCAP, Data32);
}

/*
  Set L1PM Substates capability

  If support L1 Sub-State
    1. Set Next Capability Offset, Dxx:Fn +1D0h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +1D0h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +1D0h[15:0] = 001Eh
    4. Read and write back to Dxx:Fn +1D4h
    5. Refer to section 8.3 for other requirements (Not implemented here)
    6. NEXT_CAP = 1D0h
  ELSE, set Dxx:Fn +1D0h [31:0] = 0, and read and write back to Dxx:Fn +1D4h

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @param[in] L12PMSSupported  L1.2 version supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitL1PMS (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported,
  IN BOOLEAN L12PMSSupported
  )
{
  ASSERT (RpBase != 0);

  DEBUG_SUPPORTED(Supported);
  DEBUG((DEBUG_INFO, "\twith L12PMSSupported %a\n", L12PMSSupported ? "supported" : "not supported"));

  NextCap = PchPcieCapabilitiesInitL1PMSHeader (NextCap, RpBase, Supported);
  if (L12PMSSupported) {
    SetL1PMSVersion (RpBase, Supported ? L1_P2S : L1_NONE);
  } else {
    SetL1PMSVersion (RpBase, Supported ? L1_P1S : L1_NONE);
  }

  return NextCap;
}

/*
  Set PTM capability

  If support PTM
    1. Set Next Capability Offset, Dxx:Fn +1E0h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +1E0h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +1E0h[15:0] = 001Fh
    4. Read and write back to Dxx:Fn +1E4h
    5. NEXT_CAP = 1E0h
    6. Set the PTME field in PTMCTL register
  ELSE, set Dxx:Fn +1E0h [31:0] = 0
  In both cases: read Dxx:Fn + 1E4h, set BIT1 and BIT2 then write it back

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitPtm (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  )
{
  UINT32 Data32 = 0;

  ASSERT (RpBase != 0);

  DEBUG_SUPPORTED (Supported);

  if (Supported) {
    Data32 = (V_PCH_PCIE_CFG_PTMECH_CV << N_PCIE_EXCAP_CV) | V_PCIE_EX_PTM_CID;
    Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
    NextCap = R_PCH_PCIE_CFG_PTMECH;
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_PTMECH, Data32);

  Data32 = PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_PTMCAP);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_PTMCAP, (Data32 | B_PCIE_EX_PTMCAP_PTMRC | B_PCIE_EX_PTMCAP_PTMRSPC));

  // PTM Enable
  if (Supported) {
    PciSegmentOr32 (
      RpBase + R_PCH_PCIE_CFG_PTMCTL,
      B_PCH_PCIE_CFG_PTMCTL_PTME
      );
  }

  return NextCap;
}

/*
  Set ACS Capability.

  If support ACS
    1. Set Next Capability Offset, Dxx:Fn +148h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +148h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +148h[15:0] = 000Dh
    4. Read and write back to Dxx:Fn +14Bh
    5. NEXT_CAP = 148h
    6. Set the VACSSVE field in ACSCTL register
  ELSE, set Dxx:Fn +148h [31:0] = 0, and read and write back to Dxx:Fn +14Bh

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitAcs (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  )
{
  UINT32 Data32 = 0;

  ASSERT (RpBase != 0);

  DEBUG_SUPPORTED (Supported);

  if (Supported) {
    Data32 = (V_PCH_PCIE_CFG_ACSCAPHDR_CV << N_PCIE_EXCAP_CV) | V_PCIE_EX_ACS_CID;
    Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
    NextCap = R_PCH_PCIE_CFG_ACSCAPHDR;
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_ACSCAPHDR, Data32);

  Data32 = PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_ACSCAP);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_ACSCAP, Data32);

  // ACS Source Validation Enable
  if (Supported) {
    PciSegmentOr16 (
      RpBase + R_PCH_PCIE_CFG_ACSCTL,
      B_PCH_PCIE_CFG_ACSCTL_VACSSVE
      );
  }

  return NextCap;
}

/*
  Set Advanced Error Reporting capability

  If support Advanced Error Reporting
    1. Set Next Capability Offset, Dxx:Fn +100h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +100h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +100h[15:0] = 0001h
    4. Enable ECRC
  ELSE
    1. Set Next Capability Offset, Dxx:Fn +100h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +100h[19:16]  = 0h
    3. Set Capability ID, Dxx:Fn +100h[15:10]  = 0000h

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitAer (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  )
{
  UINT32 Data32 = 0;

  ASSERT (RpBase != 0);

  DEBUG_SUPPORTED (Supported);

  if (Supported) {
    Data32 = (V_PCH_PCIE_CFG_AERCAPHDR_CV << N_PCIE_EXCAP_CV) | V_PCIE_EX_AEC_CID;
    Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
    NextCap = R_PCH_PCIE_CFG_AERCAPHDR;
    PciSegmentOr8 (RpBase + R_PCH_PCIE_CFG_AERCAPCTL, B_PCH_PCIE_CFG_AERCAPCTL_EGC);
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_AERCAPHDR, Data32);

  return NextCap;
}

/**
  This function creates Capability and Extended Capability List

  @param[in] RpDevice        Pointer to PCIE RP device
  @param[in] PcieRpConfig    Pointer to a PCH_PCIE_CONFIG that provides the platform settings

**/
VOID
InitCapabilityList (
  IN PCH_RP_DEVICE                       *RpDevice,
  IN CONST PCIE_ROOT_PORT_COMMON_CONFIG  *PcieRpCommonConfig
  )
{
  UINT64 RpBase;
  UINT16 NextCap = V_PCIE_EXCAP_NCO_LISTEND;

  RpBase = RpDevice->PciCfgBaseAddr;

  PchPcieCapabilitiesInitEntry (RpBase);

  NextCap = PchPcieCapabilitiesInitDpcExt (
              NextCap,
              RpBase,
              (BOOLEAN) PcieRpCommonConfig->DpcEnabled,
              (BOOLEAN) PcieRpCommonConfig->RpDpcExtensionsEnabled
              );

  NextCap = PchPcieCapabilitiesInitSecExp (
              NextCap,
              RpBase,
              (GetMaxLinkSpeed (RpBase) >= V_PCIE_LCAP_MLS_GEN3)
              );

  NextCap = PchPcieCapabilitiesInitL1PMS (
              NextCap,
              RpBase,
              IsClkReqAssigned (PchClockUsagePchPcie0 + RpDevice->RootPortIndex) &&
                (PcieRpCommonConfig->L1Substates != PchPcieL1SubstatesDisabled),
              PcieRpCommonConfig->L1Substates > PchPcieL1SubstatesL1_1
              );

  NextCap = PchPcieCapabilitiesInitPtm (NextCap, RpBase, (BOOLEAN) PcieRpCommonConfig->PtmEnabled);
  NextCap = PchPcieCapabilitiesInitAcs (NextCap, RpBase, (BOOLEAN) PcieRpCommonConfig->AcsEnabled);
  NextCap = PchPcieCapabilitiesInitAer (NextCap, RpBase, (BOOLEAN) PcieRpCommonConfig->AdvancedErrorReporting);

  DEBUG ((DEBUG_INFO, "%a: last capability offset: 0x%X\n", __FUNCTION__, NextCap));
}

