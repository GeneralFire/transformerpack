/** @file
  This driver manages the initial phase of SPS ME firmware support specified in
  SPS ME-BIOS Interface Specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Ppi/HeciControlPpi.h>
#include <Ppi/SpsHwChangePpi.h>

#include <Guid/SpsInfoHobGuid.h>

#include <ConfigBlock/MeGetConfigBlock.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PeiFiaMuxConfigInitLib.h>

#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>

#include <Library/MeEwlLib.h>

#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <Sps.h>
#include <Library/MeTypeLib.h>
#include <Library/SpsPeiLib.h>
#include <Ppi/MeUmaPpi.h>
#include <SpsPei.h>
#include <IndustryStandard/Pci22.h>
#include <DualBiosMsgs.h>
#include <SpsPeiCommon.h>
#include <CpuRegs.h>
#include <Register/Intel/Cpuid.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Register/PchRegsPsf.h>

/*****************************************************************************
 * Local definitions
 */
#define SPS_PREFIX    "[SPS]"

/**
  Finalize SPS Non-Functional flow

  @param[in] SpsPrefix     Pointer to SpsPrefix for debug info
*/
VOID
MeIsNonFunctional (
  IN CONST CHAR8            *SpsPrefix
  )
{
  if (MeTypeIsSpsIgnition ()) {
    DEBUG ((DEBUG_ERROR, "[SPS] SPS Ignition firmware is running in ME\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Non SPS firmware running in ME\n"));
  }
  DEBUG ((
    DEBUG_ERROR,
    "             (MEFS1: 0x%08X, MEFS2: 0x%08X)\n",
    HeciPciRead32 (R_ME_HFS),
    HeciPciRead32 (R_ME_HFS_2)
    ));

  MeEwlStateFailures (
    MeEwlSeverityFatal,
    ME_EWL_CHKP_MAJOR_PREMEM,
    ME_EWL_CHKP_PREMEM_MESTATE,
    ME_EWL_WARN_MAJOR_MEFS1,
    ME_EWL_WARN_MEFS1_OPMODE
    );
  SpsHobCreate (SpsPrefix, ME_NONFUNCTIONAL, 0, 0, FALSE, 0);
}

/**
  Create HOB with SPS info for DXE

  @param[in] SpsPrefix     Pointer to SpsPrefix for debug info
  @param[in] WorkFlow      The state of ME firmware observed at PEI
  @param[in] FeatureSet    ME features enabled reported in Get ME-BIOS Interface response
  @param[in] FeatureSet2   ME features enabled reported in Get ME-BIOS Interface response
  @param[in] PwrOptBoot    Whether NM in ME requests power optimized boot
  @param[in] Cores2Disable Whether NM in ME requests disabling cores

  @retval EFI_SUCCESS      The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES HOB creation failed
**/
EFI_STATUS
SpsHobCreate (
  IN CONST CHAR8            *SpsPrefix,
  IN UINT8                  WorkFlow,
  IN UINT32                 FeatureSet,
  IN UINT32                 FeatureSet2,
  IN BOOLEAN                PwrOptBoot,
  IN UINT8                  Cores2Disable
  )
{
  EFI_STATUS                Status;
  SPS_INFO_HOB              SpsHob;
  EFI_HOB_GUID_TYPE         *pGuidHob;
  CONST EFI_PEI_SERVICES    **PeiServices = GetPeiServicesTablePointer();
  ME_UMA_PPI                *MeUma = NULL;
  ME_UMA_INFO               *MeUmaInfo;

  if (SpsPrefix == NULL) {
    ASSERT (FALSE);
    SpsPrefix = SPS_PREFIX;
  }
  //
  // Zero the HOB buffer
  //
  (*PeiServices)->SetMem (&SpsHob, sizeof(SpsHob), 0);

  if (WorkFlow == ME_FUNCTIONAL) {
    //
    // Locate MeUma PPI.
    //
    Status = (*PeiServices)->LocatePpi (PeiServices, &gMeUmaPpiGuid, 0, NULL, &MeUma);
    if (!EFI_ERROR (Status)) {
      MeUmaInfo = MeUma->MeUmaGetInfo ();
    } else {
      MeUmaInfo = NULL;
    }
    if (MeUmaInfo != NULL) {
      DEBUG ((DEBUG_INFO, "%a HOB: requested ME UMA size (%d)\n", SpsPrefix, MeUmaInfo->MeUmaSize));
    } else {
      DEBUG ((
        DEBUG_ERROR,
        "%a ERROR: Can't locate ME UMA PPI (status %r) - SPS Info HOB will not be updated with ME UMA size\n",
         SpsPrefix,
         Status
         ));
    }
  }

  //
  // Initialize the HOB data
  //
  SpsHob.FeatureSet.Data.Set1 = FeatureSet;
  SpsHob.FeatureSet.Data.Set2 = FeatureSet2;
  SpsHob.WorkFlow = WorkFlow;
  SpsHob.PwrOptBoot = PwrOptBoot;
  SpsHob.Cores2Disable = Cores2Disable;

  pGuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
  if (pGuidHob == NULL) {
    BuildGuidDataHob (&gSpsInfoHobGuid, &SpsHob, sizeof (SpsHob));
  } else {
    SPS_INFO_HOB *ExistingSpsHob;

    ExistingSpsHob = GET_GUID_HOB_DATA (pGuidHob);
     if (ExistingSpsHob != NULL) {
       CopyMem (ExistingSpsHob, &SpsHob, sizeof (SpsHob));
     }
  }

  DEBUG ((DEBUG_INFO, "%a HOB: flow %d, feature set 0x%08X, features 2 0x%08x, pwr opt boot %d, cores2disable %d\n",
         SpsPrefix,
         SpsHob.WorkFlow, SpsHob.FeatureSet.Data.Set1, SpsHob.FeatureSet.Data.Set2, SpsHob.PwrOptBoot, SpsHob.Cores2Disable));

  if (WorkFlow != ME_FUNCTIONAL) {
    if (MeTypeIsSpsIgnition ()) {
      DEBUG ((DEBUG_ERROR, "%a SPS is in ignition mode (flow %d)\n", SpsPrefix, WorkFlow));
    } else {
      DEBUG ((DEBUG_ERROR, "%a ERROR: ME is non functional (flow %d)\n", SpsPrefix, WorkFlow));
    }
  }

  pGuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
  if (pGuidHob != NULL) {
     return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_ERROR, "%a ERROR: SPS info HOB has not been registered\n", SpsPrefix));
  return EFI_OUT_OF_RESOURCES;
} // SpsHobCreate ()

