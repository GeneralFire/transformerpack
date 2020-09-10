/** @file
  Source code file for Silicon Init Post Memory module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#include "SiInit.h"
#include <Library/PerformanceLib.h>
#ifdef CDF_SC_FLAG
#include <Register/PchRegsPcie.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegsTraceHub.h>
#include <Register/PchRegsHsuart.h>
#include <Library/PchPcieRpInfo.h>
#include <Register/PchRegsScsCdf.h>
#include <Register/PchRegsIeh.h>
#endif
#if ME_SUPPORT_FLAG
#ifdef CDF_SC_FLAG
#include <Library/MeInitLib.h>
#include <MeChipset.h>
#else // CDF_SC_FLAG
#include <Library/MeLibPei.h>
#include "Library/PeiMeLib.h"
#endif // CDF_SC_FLAG
#endif // ME_SUPPORT_FLAG
#include <Library/PchMultiPch.h>


#ifdef EBG_SC_FLAG
STATIC SVID_SID_INIT_ENTRY mSsidTablePtr[] = {

  //
  // PCH Device(s)
  //
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_LPC,               PCI_DEVICE_NUMBER_PCH_LPC,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_P2SB,              PCI_DEVICE_NUMBER_PCH_P2SB,          DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_PMC,               PCI_DEVICE_NUMBER_PCH_PMC,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SATA_1,            PCI_DEVICE_NUMBER_PCH_SATA_1,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SATA_2,            PCI_DEVICE_NUMBER_PCH_SATA_2,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SATA_3,            PCI_DEVICE_NUMBER_PCH_SATA_3,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SMBUS,             PCI_DEVICE_NUMBER_PCH_SMBUS,         DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_SMBUS_HOST,            PCI_DEVICE_NUMBER_SMBUS_HOST,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SPI,               PCI_DEVICE_NUMBER_PCH_SPI,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,         PCI_DEVICE_NUMBER_PCH_TRACE_HUB,     DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_XHCI,              PCI_DEVICE_NUMBER_PCH_XHCI,          DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_4 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_5 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_6 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_7 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_8 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_9 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_10, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_11, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_12, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_13, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_13, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_14, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_14, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_15, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_15, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_16, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_16, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_HSUART0,       PCI_DEVICE_NUMBER_PCH_HSUART,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_HSUART1,       PCI_DEVICE_NUMBER_PCH_HSUART,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_IEH,               PCI_DEVICE_NUMBER_PCH_IEH,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},

  //
  // ME Device (s)
  //
#if ME_SUPPORT_FLAG
  {{{PCI_SVID_OFFSET,        HECI_FUNCTION_NUMBER,                      ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        HECI2_FUNCTION_NUMBER,                     ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        IDER_FUNCTION_NUMBER,                      ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        SOL_FUNCTION_NUMBER,                       ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        HECI3_FUNCTION_NUMBER,                     ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        HECI4_FUNCTION_NUMBER,                     ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
#endif // ME_SUPPORT_FLAG
};
#elif CDF_SC_FLAG
STATIC SVID_SID_INIT_ENTRY mSsidTablePtr[] = {

  //
  // PCH Device(s)
  //
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_LPC,               PCI_DEVICE_NUMBER_PCH_LPC,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_P2SB,              PCI_DEVICE_NUMBER_PCH_P2SB,          DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_PMC,               PCI_DEVICE_NUMBER_PCH_PMC,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SATA_1,            PCI_DEVICE_NUMBER_PCH_SATA_1,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SATA_2,            PCI_DEVICE_NUMBER_PCH_SATA_2,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SATA_3,            PCI_DEVICE_NUMBER_PCH_SATA_3,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SMBUS,             PCI_DEVICE_NUMBER_PCH_SMBUS,         DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_SMBUS_HOST,            PCI_DEVICE_NUMBER_SMBUS_HOST,        DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_SPI,               PCI_DEVICE_NUMBER_PCH_SPI,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_TRACE_HUB,         PCI_DEVICE_NUMBER_PCH_TRACE_HUB,     DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_XHCI,              PCI_DEVICE_NUMBER_PCH_XHCI,          DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_4 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_5 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_6 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_7 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_8 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9 , PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_9 , DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_10, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_11, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{R_PCH_PCIE_CFG_SSVID,    PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12, PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_12, DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_CDF_SCS_EMMC,      PCI_DEVICE_NUMBER_PCH_CDF_SCS_EMMC,  DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_HSUART0,       PCI_DEVICE_NUMBER_PCH_HSUART,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_HSUART1,       PCI_DEVICE_NUMBER_PCH_HSUART,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_HSUART2,       PCI_DEVICE_NUMBER_PCH_HSUART,    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        PCI_FUNCTION_NUMBER_PCH_IEH,               PCI_DEVICE_NUMBER_PCH_IEH,           DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},

  //
  // ME Device (s)
  //
#if ME_SUPPORT_FLAG
  {{{PCI_SVID_OFFSET,        HECI_FUNCTION_NUMBER,                      ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        HECI2_FUNCTION_NUMBER,                     ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        IDER_FUNCTION_NUMBER,                      ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        SOL_FUNCTION_NUMBER,                       ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        HECI3_FUNCTION_NUMBER,                     ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
  {{{PCI_SVID_OFFSET,        HECI4_FUNCTION_NUMBER,                     ME_DEVICE_NUMBER,                    DEFAULT_PCI_BUS_NUMBER_PCH, 0, DEFAULT_PCI_SEGMENT_NUMBER_PCH, 0}}, {0, 0},0},
#endif // ME_SUPPORT_FLAG
};
#endif //CDF_SC_FLAG

// APTIOV_SERVER_OVERRIDE_RC_START : Added AmiChipsetInitDoneLib to add depex to let AhciRecovery, NvmeRecovery and UsbPei run after we initiate chipset
EFI_PEI_PPI_DESCRIPTOR mEndOfSiInit = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI |EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmiEndOfSiInitPpiGuid,
  NULL
};

// APTIOV_SERVER_OVERRIDE_RC_END : Added AmiChipsetInitDoneLib to add depex to let AhciRecovery, NvmeRecovery and UsbPei run after we initiate chipset

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    SiInitOnEndOfPei
  }
};

static EFI_PEI_NOTIFY_DESCRIPTOR  mSiInitPostMemNotifyList[] = {
  {
#ifndef FSP_FLAG
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
#else
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
#endif
#ifdef CDF_SC_FLAG
    &gSiPolicyPpiGuid,
#else
    &gPchPlatformPolicyPpiGuid,
#endif //CDF_SC_FLAG
    SiInitPostMemOnPolicy
  }
};

/**
  Silicon Init End of PEI callback function. This is the last change before entering DXE and OS when S3 resume.

  @param[in] PeiServices   - Pointer to PEI Services Table.
  @param[in] NotifyDesc    - Pointer to the descriptor for the Notification event that
                             caused this function to execute.
  @param[in] Ppi           - Pointer to the PPI data associated with this function.

  @retval EFI_STATUS       - Always return EFI_SUCCESS
**/
EFI_STATUS
SiInitOnEndOfPei (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  )
{

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  //
  // Initializes PCH after End of Pei
  //

  PchOnEndOfPei ();

#ifdef ME_SUPPORT_FLAG
  MeOnEndOfPei ();
#endif // ME_SUPPORT_FLAG

  DEBUG ((DEBUG_INFO, "%a() - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

#ifdef CDF_SC_FLAG
/**
  Program SA devices Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

**/
VOID
ProgramSvidSid (
  VOID
  )
{
  UINT8                         Index;
  UINT16                        NumberOfSsidTableEntry;
  SVID_SID_INIT_ENTRY           *SsidTablePtr;
  EFI_STATUS                    Status;
  SI_POLICY_PPI                 *SiPolicy;
  SI_CONFIG                     *SiConfig;
  UINT32                        Data32;
  UINT16                        DataSvid;
  UINT16                        DataSsid;



  //
  // Set Table ptr and Table count to default value
  //

    SsidTablePtr = mSsidTablePtr;
    NumberOfSsidTableEntry = (sizeof (mSsidTablePtr) / sizeof (SVID_SID_INIT_ENTRY));
  //
  // Locate SiPolicyPpi
  //
  SiPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  if ((Status == EFI_SUCCESS) && (SiPolicy != NULL)) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
    if ((Status == EFI_SUCCESS) &&
       (SiConfig != NULL) &&
       (SiConfig->SsidTablePtr != 0) &&
       (SiConfig->NumberOfSsidTableEntry != 0)) {
      //
      // Use SiPolicy Table and table counts
      //
      SsidTablePtr = (SVID_SID_INIT_ENTRY*) SiConfig->SsidTablePtr;
      NumberOfSsidTableEntry = MAX_DEVICE_COUNT;
      //
      // Verify number of devices
      //
      if ((SiConfig->NumberOfSsidTableEntry < MAX_DEVICE_COUNT)) {
        NumberOfSsidTableEntry = SiConfig->NumberOfSsidTableEntry;
      }
    }
  }

  for (Index = 0; Index < NumberOfSsidTableEntry; Index++) {
    ///
    /// Skip if the device is disabled
    ///
    if (PciSegmentRead32 (SsidTablePtr[Index].Address.SegBusDevFuncRegister & (~0xFFF)) == 0xFFFFFFFF) {
      continue;
    }
    ///
    /// Program SA devices Subsystem Vendor Identifier (SVID)
    ///
    DataSvid = SsidTablePtr[Index].SvidSidValue.SubSystemVendorId;
    if (DataSvid == 0) {
      DataSvid = DEFAULT_SSVID;
    }
    //
    // GET SID
    //
    DataSsid = SsidTablePtr[Index].SvidSidValue.SubSystemId;
    if (DataSsid == 0) {
      DataSsid = DEFAULT_SSDID;
    }
    Data32 = (DataSsid << 16) | DataSvid;
    PciSegmentWrite32 (
      SsidTablePtr[Index].Address.SegBusDevFuncRegister,
      Data32
      );
  }

  return;
}
#endif //CDF_SC_FLAG
/**
  Slicon Initializes after PostMem phase Policy PPI produced,
  All required polices must be installed before the callback

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  SiPolicy PPI.

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_UNSUPPORTED         The function failed to locate SiPolicy
**/
EFI_STATUS
EFIAPI
SiInitPostMemOnPolicy (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  )
{
#ifdef CDF_SC_FLAG
  SI_POLICY_PPI             *SiPolicy;
#else
  PCH_POLICY_PPI            *PchPolicy;
#endif //CDF_SC_FLAG
  EFI_STATUS                Status;
  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));
#ifdef CDF_SC_FLAG
  SiPolicy = (SI_POLICY_PPI*) Ppi;

  ///
  /// SubsystemID programming on Internal Devices.
  ///
  DEBUG ((DEBUG_INFO, "SubsystemID programming on Internal Devices\n"));
  ProgramSvidSid ();

  //
  // Initializes ME after memory services initialized
  // Required before PCH to get HSIO information
  //
#if defined (ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  MePostMemInit ();
#endif // ME_SUPPORT_FLAG

  //
  // Initializes PCH after memory services initialized
  //
  PchInit (SiPolicy);
#else
  //
  // Locate installed PCH Policy PPI
  //
  Status = PeiServicesLocatePpi (
             &gPchPlatformPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PchPolicy
             );
  if (Status != EFI_SUCCESS) {
#if defined (ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
    NotifyMeReadyForSendingDid ();
#endif // ME_SUPPORT_FLAG
    //
    // PCH_POLICY_PPI must be installed at this point
    //
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Initializes Legacy PCH after memory services initialized
  //
  PchInit (PchPolicy);

  //
  // Initializes Non-Legacy PCH after memory services initialized
  //
  PchNonLegacyInit ();

  Status = PchInstallPchIpInfoHob ();
  ASSERT_EFI_ERROR (Status);

  //
  // Initializes ME after memory services initialized
  //
#if defined (ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  MePostMemInit ();
  NotifyMeReadyForSendingDid ();
#endif
#endif // CDF_SC_FLAG

  //
  // Install EndOfPei callback function.
  //
  Status = PeiServicesNotifyPpi (mSiInitNotifyList);
  ASSERT_EFI_ERROR (Status);

//APTIOV_SERVER_OVERRIDE_RC_START : Added AmiChipsetInitDoneLib to add depex to let AhciRecovery, NvmeRecovery and UsbPei run after we initiate chipset
  // End of SiInit notification event
  Status = PeiServicesInstallPpi (&mEndOfSiInit);
  ASSERT_EFI_ERROR (Status);
// APTIOV_SERVER_OVERRIDE_RC_END : Added AmiChipsetInitDoneLib to add depex to let AhciRecovery, NvmeRecovery and UsbPei run after we initiate chipset

  DEBUG ((DEBUG_INFO, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Silicon Initializes after memory services initialized

  @param[in] FileHandle           The file handle of the file, Not used.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completes successfully
**/
EFI_STATUS
EFIAPI
SiInit (
  IN  EFI_PEI_FILE_HANDLE               FileHandle,
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
{
  EFI_STATUS                Status;

  //
  // Install PostMem phase OnPolicyInstalled callback function.
  //
  Status = PeiServicesNotifyPpi (mSiInitPostMemNotifyList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
