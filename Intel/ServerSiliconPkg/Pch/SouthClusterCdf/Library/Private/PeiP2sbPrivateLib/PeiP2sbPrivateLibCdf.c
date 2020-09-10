/** @file
  Initializes PCH Primary To Sideband Bridge (P2SB) Device in PEI

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

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PciSegmentLib.h>
#include <Ppi/SiPolicy.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsP2sb.h>
#include <Register/PchRegsPcr.h>
#if FixedPcdGetBool(PcdHSleEnable) == 1
#include <Library/TimerLib.h>
#endif
#include <Library/PchInfoLib.h>
#include "PeiP2sbPrivateLibHelper.h"
#define R_HPET_MEM_GEN_CFG            0x10
#define B_HPET_MEM_GEN_CFG_ENABLE_CNF BIT0

/**
  Get P2SB pci segment address.

  @retval P2SB pci segment base address
**/
UINT64
P2sbPciBase (
  VOID
  )
{
  UINT64 P2sbBase;
  P2sbBase = PCI_SEGMENT_LIB_ADDRESS (
               DEFAULT_PCI_SEGMENT_NUMBER_PCH,
               DEFAULT_PCI_BUS_NUMBER_PCH,
               PCI_DEVICE_NUMBER_PCH_P2SB,
               PCI_FUNCTION_NUMBER_PCH_P2SB,
               0
               );
  ASSERT (PciSegmentRead16 (P2sbBase + PCI_VENDOR_ID_OFFSET) != 0xFFFF);
  return P2sbBase;
}

/**
  Hide P2SB device.

  @param[in]  P2sbBase                  Pci segment base address of P2SB controller.
**/
STATIC
VOID
P2sbHideDevice (
  IN  UINT64                            P2sbBase,
  SI_CONFIG                             *SiConfig
  )
{
  PciSegmentWrite8 (P2sbBase + R_P2SB_CFG_E0 + 1, BIT0);
#if FixedPcdGetBool(PcdHSleEnable) == 1
  if (SiConfig->HsleWorkaround == 1) {
    DEBUG ((DEBUG_INFO, "HSLE Workaround to avoid p2sb clock gating\n"));
    if (PciSegmentRead32 (P2sbBase) != 0xFFFFFFFF) {
      DEBUG ((DEBUG_INFO, "100ms delay before P2SB Base read\n"));
      MicroSecondDelay (100 * 1000); //wait for 100 ms
    }
  }
#endif
}

/**
  Lock SAI access from P2SB before any 3rd code execution.
**/
VOID
P2sbSaiLock (
  VOID
  )
{
}

/**
  Check SBREG readiness.

  @retval TRUE                SBREG is ready
  @retval FALSE               SBREG is not ready
**/
BOOLEAN
P2sbIsSbregReady (
  VOID
  )
{
  if ((PciSegmentRead32 (P2sbPciBase () + R_P2SB_CFG_SBREG_BAR) & B_P2SB_CFG_SBREG_RBA) == 0) {
    return FALSE;
  }
  return TRUE;
}

/**
  Internal function performing HPET initin early PEI phase
**/
VOID
P2sbHpetInit (
  VOID
  )
{
  UINT64                                P2sbBase;
  UINT32                                HpetBase;

  P2sbBase = P2sbPciBase ();

  //
  // Initial and enable HPET High Precision Timer memory address for basic usage
  // If HPET base is not set, the default would be 0xFED00000.
  //
  HpetBase = PcdGet32 (PcdSiHpetBaseAddress);
  ASSERT ((HpetBase & 0xFFFFCFFF) == 0xFED00000);
  PciSegmentAndThenOr8 (
    P2sbBase + R_P2SB_CFG_HPTC,
    (UINT8) ~B_P2SB_CFG_HPTC_AS,
    (UINT8) (((HpetBase >> N_HPET_ADDR_ASEL) & B_P2SB_CFG_HPTC_AS) | B_P2SB_CFG_HPTC_AE)
    );
  //
  // Read back for posted write to take effect
  //
  PciSegmentRead8 (P2sbBase + R_P2SB_CFG_HPTC);
  //
  // Set HPET Timer enable to start counter spinning
  //
  MmioOr32 (HpetBase + R_HPET_MEM_GEN_CFG, B_HPET_MEM_GEN_CFG_ENABLE_CNF);
  //
  // Build the resource descriptor hob for HPET address resource.
  // HPET only claims 0x400 in size, but the minimal size to reserve memory
  // is one page 0x1000.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    HpetBase,
    0x1000
    );
  BuildMemoryAllocationHob (
    HpetBase,
    0x1000,
    EfiMemoryMappedIO
    );
}

/**
 Early init P2SB configuration
**/
VOID
P2sbEarlyConfig (
  VOID
  )
{
  UINT64                  P2sbBase;

  P2sbBase  = P2sbPciBase ();

  ///
  /// BIOS shall program the PCI Capability List to 0 for P2SB controller.
  ///
  PciSegmentWrite8 (P2sbBase + PCI_CAPBILITY_POINTER_OFFSET, 0);
  /// For GPIO and ITSS that support sideband posted write, they can support
  /// back to back write after their corresponding bits under P2SB PCI Config
  /// 200h-21Fh are set.
  /// For CDF, program the following at early PCH BIOS init
  /// 1. Set P2SB PCI offset 200h to 0
  /// 2. Set P2SB PCI offset 204h to 0
  /// 3. Set P2SB PCI offset 208h to 0
  /// 4. Set P2SB PCI offset 20Ch to 0
  /// 5. Set P2SB PCI offset 210h to 0
  /// 6. Set P2SB PCI offset 214h to 0x00000024h
  /// 7. Set P2SB PCI offset 218h to 0x00010000h
  /// 8. Set P2SB PCI offset 21Ch to 0
  ///
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_200, 0);
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_204, 0);
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_208, 0);
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_20C, 0);
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_210, 0);
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_214, 0);
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_218, 0x00010024);
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_21C, 0);
}

/**
  The function performs P2SB initialization.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
P2sbConfigure (
  IN  SI_POLICY_PPI           *SiPolicy
  )
{
  UINT64                      P2sbBase;
#if FixedPcdGetBool(PcdHSleEnable) == 1
  SI_CONFIG                   *SiConfig;
  EFI_STATUS                  Status;
#endif


  P2sbBase = P2sbPciBase ();

  //
  // HPET and APIC BDF programming
  // Assign 0:30:6 for HPET and 0:30:7 for APIC statically.
  //
  PciSegmentWrite16 (
    P2sbBase + R_P2SB_CFG_HBDF,
    (V_P2SB_CFG_HBDF_BUS << 8) | (V_P2SB_CFG_HBDF_DEV << 3) | V_P2SB_CFG_HBDF_FUNC
    );
  PciSegmentWrite16 (
    P2sbBase + R_P2SB_CFG_IBDF,
    (V_P2SB_CFG_IBDF_BUS << 8) | (V_P2SB_CFG_IBDF_DEV << 3) | V_P2SB_CFG_IBDF_FUNC
    );

  //
  // Set P2SB PCI offset 0xE0[4] = 1.
  //
  PciSegmentOr8 (P2sbBase + R_P2SB_CFG_E0, BIT4);

  //
  // P2SB power management settings.
  //

#if FixedPcdGetBool(PcdHSleEnable) == 1
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);
  if (!SiConfig->HsleWorkaround) {
  DEBUG ((DEBUG_ERROR, "\nHSLE Workaround to avoid p2sb clock gating\n"));
#endif
  if(!IsCdfPch()) {
  ///
  /// Set PGCB clock gating enable (PGCBCGE)
  /// P2SB PCI offset 0xE0[16] = 1
  ///
  PciSegmentOr8 (P2sbBase + R_P2SB_CFG_E0 + 2, BIT0);
  ///
  /// Set Hardware Autonomous Enable (HAE) and PMC Power Gating Enable (PMCPG_EN)
  /// P2SB PCI offset 0xE4[5] = 1 and 0xE4[2,1.0] = 0's
  ///
  PciSegmentAndThenOr32 (P2sbBase + R_P2SB_CFG_E4,  (UINT32) ~(BIT2 | BIT1 | BIT0), BIT5);
  } else {

    PciSegmentOr32 (P2sbBase + R_P2SB_CFG_E0, B_P2SB_CFG_E0_PGCBCGE);

    PciSegmentAnd32 (P2sbBase + R_P2SB_CFG_E4,  (UINT32)
                    ~(B_P2SB_CFG_E4_D3HE |
                      B_P2SB_CFG_E4_I3E |
                      B_P2SB_CFG_E4_PMCPG_EN |
                      B_P2SB_CFG_E4_HAE));
  }
#if FixedPcdGetBool(PcdHSleEnable) == 1
  }
#endif

  ///
  /// Set LFIORIEC to 0 for IEC not supported.
  /// P2SB PCI offset 0x74 = 0
  ///
  PciSegmentWrite8 (P2sbBase + R_P2SB_CFG_LFIORIEC, 0);
}

/**
  Bios will remove the host accessing right to PSF, SPI/eSPI and MCSMB
  register range prior to any 3rd party code execution.

  @param[in] P2sbBase         P2SB PCI base address
**/
STATIC
VOID
P2sbRemovePsfAccess (
  IN UINT64                   P2sbBase
  )
{
  UINT32                                RegisterOff;
  UINT32                                BitNo;
  UINTN                                 Index;

  UINT16 P2SBMaskedDevices[] = {PID_PSF0, PID_PSF1, PID_PSF2, PID_PSFS, PID_PSF_SATA0, PID_PSF_SATA1, PID_PSF_SATA2, PID_PSF3, PID_PSF_CSME, PID_PSF_IE, PID_MCSMB, PID_ESPISPI, (UINT16)-1};

  DEBUG ((DEBUG_INFO, "P2sbRemovePsfAccess start\n"));

  for (Index = 0; ((UINT16)-1) != P2SBMaskedDevices[Index]; ++Index) {
    ///
    /// Set the bit corresponding to IOSF-SB endpoint id in the P2SB.EPMASK[0-7] register to remove the access
    ///
    RegisterOff = (P2SBMaskedDevices[Index] / 32) * sizeof(UINT32); // Calculate which 32 bit mask register
    BitNo = P2SBMaskedDevices[Index] % 32;                          // Calculate the bit number to be set in the mask register
    PciSegmentOr32 ((UINTN) (P2sbBase + R_P2SB_CFG_EPMASK0 + RegisterOff), (UINT32) 1 << BitNo);
    DEBUG ((DEBUG_INFO, "Removing PSF access: EPMASK[%d] BIT%d endpoint: %d (%02x)\n", (RegisterOff / sizeof(UINT32)), BitNo, P2SBMaskedDevices[Index], P2SBMaskedDevices[Index]));
  }
  ///
  /// "Endpoint Mask Lock" is not locked at this point because access to PSF1 is needed up until after enumeration, and this is where it's being locked
  ///
}

/**
  The function performs P2SB lock programming.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
P2sbLock (
  IN  SI_POLICY_PPI           *SiPolicy
  )
{
  EFI_STATUS             Status;
  UINT64                 P2sbBase;
  PCH_P2SB_CONFIG        *P2sbConfig;
  SI_CONFIG              *SiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);

  P2sbBase = P2sbPciBase ();

  //
  // Do P2SB SBI lock, Sideband access removal, Hide P2SB, and set PostBootSai in EndOfPei.
  //
  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Program EPMASK.
  // @note, doing EPMASK in end of pei will block the HECI device disable in EndOfPost.
  //
  if (P2sbConfig->SbAccessUnlock == 0) {
    P2sbRemovePsfAccess (P2sbBase);
  }

  //@todo. W/A for ICL simics since it doesn't support POSTBOOT SAI to block P2SB access.
  if (P2sbConfig->P2SbReveal == 0) {
    //
    // Hide P2SB controller in the end of PEI.
    //
    P2sbHideDevice (P2sbBase, SiConfig);
  }
}
