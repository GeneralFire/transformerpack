/** @file
  MeUmaCommon.c

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

///
/// Statements that include other files
///
#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include "Library/MmPciBaseLib.h"
#include "MeAccess.h"
#include <Register/HeciRegs.h>
#include "IndustryStandard/Pci22.h"
#include "Ppi/MeUmaPpi.h"
#include "Library/DebugLib.h"
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <PchAccess.h>
#include <Register/PchRegsPmc.h>
#include <Library/PeiPchPcieLib.h>
#include "Library/PerformanceLib.h"
//
#include "Ppi/Wdt.h"
#include "Library/IoLib.h"
#include "Library/MeTypeLib.h"
#include <Library/PreSiliconEnvDetectLib.h>
#include <BupMsgs.h>
#include <Library/PeiMeLib.h>
#include <Library/PmcLib.h>
#include <PchResetPlatformSpecific.h>
#include <Protocol/MePlatformGetResetType.h>
#include <ConfigBlock.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <UncoreCommonIncludes.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/HobLib.h>
#include <MeUmaCommon.h>

/*****************************************************************************
 * Local definitions
 */
#define             DEBUG_ME_UMA          "ME UMA:"
extern CONST CHAR8  *DebugMeUmaPrefix;

/*****************************************************************************
 * Local prototypes
 */

/*****************************************************************************
 * Global variables
 */

/**
  This checks if ME UMA is enabled

  @param[in] MeUmaPrefix  Pointer to prefix

  @retval TRUE            ME UMA is enabled
  @retval FALSE           ME UMA is disabled
**/
BOOLEAN
EFIAPI
MeUmaInternalIsEnabled (
  IN CONST CHAR8  *MeUmaPrefix
  )
{
  BOOLEAN                         UmaEnabled = TRUE;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig = NULL;

  if (MeUmaPrefix == NULL) {
    ASSERT (FALSE);
    MeUmaPrefix = DEBUG_ME_UMA;
  }

  if (PmcIsDwrBootMode() == TRUE) {
    DEBUG ((DEBUG_INFO, "%a %a() for DWR UMA is disabled\n",MeUmaPrefix, __FUNCTION__));
    return FALSE;
  }

#if ME_TESTMENU_FLAG
  //
  // Configure UMA support
  // Disable UMA when END_OF_POST message is disabled
  //
  MePeiPreMemConfig = GetMePreMemConfigBlock (NULL);
  if (MePeiPreMemConfig != NULL) {
    UmaEnabled = (BOOLEAN)MePeiPreMemConfig->SendDidMsg;
  } else {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate ME Policy PPI, using default\n", MeUmaPrefix));
  }
#endif // ME_TESTMENU_FLAG

  if (MeTypeIsDisabled()) {
    UmaEnabled = FALSE;
  }

  DEBUG ((DEBUG_INFO, "%a UMA Enabled flag = %d\n",MeUmaPrefix, UmaEnabled));

  return UmaEnabled;
}

/**
  Updates the ME UMA Size Info in the ME UMA PPI

  @param[in] MeUmaPrefix    Pointer to prefix
  @param[in] MeUmaSize      ME UMA Size requested by the ME FW
  @param[in] MeUmaSizeError ME UMA Size request error (non 0 value indicates error)

  @retval EFI_SUCCESS       The ME UMA size info is updated.
  @retval EFI_DEVICE_ERROR  ME UMA Info data pointer invalid
**/
EFI_STATUS
EFIAPI
MeUmaUpdatePpiMeUmaSizeInfo (
  IN CONST CHAR8  *MeUmaPrefix,
  IN UINT32       MeUmaSize,
  IN UINT8        MeUmaSizeError
  )
{
  EFI_STATUS  Status;
  ME_UMA_INFO *MeUmaInfo;

  if (MeUmaPrefix == NULL) {
    ASSERT (FALSE);
    MeUmaPrefix = DEBUG_ME_UMA;
  }

  MeUmaInfo = MeUmaGetInfo ();
  if (MeUmaInfo != NULL) {
    MeUmaInfo->MeUmaSize = MeUmaSize;
    MeUmaInfo->MeUmaSizeError = MeUmaSizeError;
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a ME UMA Info update status = %r\n", MeUmaPrefix, Status));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  This procedure will clear the DISB.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
ClearDisb (
  VOID
  )
{
  PmcClearDramInitScratchpad();

  return EFI_SUCCESS;
}

/**
  This procedure will issue a Non-Power Cycle, Power Cycle, or Global Rest.

  @param[in] MeUmaPrefix          Pointer to prefix
  @param[in] ResetType            Type of reset to be issued.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
MeUmaPerformReset (
  IN CONST CHAR8         *MeUmaPrefix,
  IN UINT8               ResetType
  )
{
  EFI_RESET_TYPE      MeResetType;
  PCH_RESET_DATA      ResetData;
  UINTN               ResetDataSize;

  ResetDataSize = 0;
  if (MeUmaPrefix == NULL) {
    ASSERT (FALSE);
    MeUmaPrefix = DEBUG_ME_UMA;
  }

  if (MeTypeIsDisabled ()) {
    DEBUG ((DEBUG_ERROR, "%a %a(): Me disabled - no reset\n", MeUmaPrefix, __FUNCTION__));
    return EFI_SUCCESS;
  }

  switch (ResetType) {
  case CBM_DIR_NON_PCR_CLEARDISB:
  case CBM_DIR_NON_PCR:
    DEBUG ((DEBUG_ERROR, "%a ME FW DID ACK has requested a Non Power Cycle Reset.\n", MeUmaPrefix));
    MeResetType = EfiResetWarm;
    break;

  case CBM_DIR_PCR:
    ///
    /// Power Cycle Reset requested
    ///
    DEBUG ((DEBUG_INFO, "%a ME FW DID ACK has requested a Power Cycle Reset.\n", MeUmaPrefix));
    MeResetType = EfiResetCold;
    break;

  case CBM_DIR_GLOBAL_RESET:
    ///
    /// Global Reset
    ///
    DEBUG ((DEBUG_ERROR, "%a ME FW DID Ack requested a global reset.\n", MeUmaPrefix));
    MeResetType = EfiResetPlatformSpecific;
    CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
    StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
    ResetDataSize = sizeof (PCH_RESET_DATA);
    break;
  default:
    DEBUG ((DEBUG_ERROR, "%a ME FW No reset.\n", MeUmaPrefix));

    return EFI_SUCCESS;
  }
  (*GetPeiServicesTablePointer ())->ResetSystem2 (MeResetType, EFI_SUCCESS, ResetDataSize, &ResetData);

  //
  // We should never get here
  //
  DEBUG ((DEBUG_ERROR, "%a Waiting for reset execution.\n", MeUmaPrefix));
  CpuDeadLoop ();

  return EFI_SUCCESS;
}

/**
  This procedure will enforce the BIOS Action that was requested by ME FW
  as part of the DRAM Init Done message.

  @param[in] BiosAction           Me requests BIOS to act

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MeUmaHandleBiosAction  (
  UINT8               BiosAction
  )
{
  return MeUmaInternalHandleBiosAction (DebugMeUmaPrefix , BiosAction);
}

/**
  This procedure will enforce the BIOS Action that was requested by ME FW
  as part of the DRAM Init Done message.

  @param[in] MeUmaPrefix          Pointer to prefix
  @param[in] BiosAction           Me requests BIOS to act

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MeUmaInternalHandleBiosAction  (
  IN CONST CHAR8         *MeUmaPrefix,
  IN UINT8               BiosAction
  )
{
  EFI_STATUS            Status;
  HECI_GS_SHDW_REGISTER MeFwsts2;

  if (MeUmaPrefix == NULL) {
    ASSERT (FALSE);
    MeUmaPrefix = DEBUG_ME_UMA;
  }

  if (!MeUmaInternalIsEnabled (MeUmaPrefix)) {
    DEBUG ((DEBUG_INFO, "%a ME UMA feature disabled (%a()).\n", MeUmaPrefix, __FUNCTION__));
    return EFI_SUCCESS;
  }

  if (!MeTypeIsDfx ()) {
    if (MeTypeIsDisabled ()) {
      DEBUG ((DEBUG_INFO, "%a ME debug mode, do not %a. \n",MeUmaPrefix, __FUNCTION__));
      return EFI_SUCCESS;
    }
  }

#if ME_TESTMENU_FLAG
  if (HybridSystemLevelEmulationEnabled ()) {
    // Do not execute reset for emulation environment
    DEBUG ((DEBUG_ERROR, "%a Emulation environment detected. Continue post for BiosAction %d\n",MeUmaPrefix, BiosAction));
    return EFI_SUCCESS;
  }
#endif // ME_TESTMENU_FLAG

  ///
  /// Read ME FWS2
  ///
  MeFwsts2.ul = HeciPciRead32 (R_ME_HFS_2);
  DEBUG ((DEBUG_INFO, "%a MeFwsts2 = %x.\n",MeUmaPrefix, MeFwsts2.r));

  switch (BiosAction) {
  case CBM_DIR_DO_NOTHING:
    ///
    /// DID ACK was not received
    ///
    DEBUG ((DEBUG_ERROR, "%a DID Ack was not received, no BIOS Action to process.\n", MeUmaPrefix));
    break;
  case CBM_DIR_NON_PCR_CLEARDISB:
    DEBUG ((DEBUG_ERROR, "%a ME FW DID ACK has requested a Clear DISB.\n", MeUmaPrefix));
    ///
    /// Clear DISB and Issue a Non-Power Cycle Reset
    ///
    Status  = ClearDisb ();
    ASSERT_EFI_ERROR (Status);
    ///
    /// This type of reset is handled directly by MRC
    ///
    return Status;
  case CBM_DIR_NON_PCR:
    ///
    /// Perform Non-Power Cycle Reset
    ///
    DEBUG ((DEBUG_ERROR, "%a ME FW has requested a Non-PCR.\n", MeUmaPrefix));
    Status = MeUmaPerformReset (MeUmaPrefix, CBM_DIR_NON_PCR);
    break;

  case CBM_DIR_PCR:
    ///
    /// Perform Power Cycle Reset
    ///
    DEBUG ((DEBUG_ERROR, "%a ME FW has requested a PCR.\n", MeUmaPrefix));
    Status = MeUmaPerformReset (MeUmaPrefix, CBM_DIR_PCR);
    break;

  case CBM_DIR_GLOBAL_RESET:
    ///
    /// Perform Global Reset
    ///
    DEBUG ((DEBUG_ERROR, "%a ME FW has requested a Global Reset.\n", MeUmaPrefix));
    Status = MeUmaPerformReset (MeUmaPrefix, CBM_DIR_GLOBAL_RESET);
    break;

  default:
    DEBUG ((DEBUG_INFO, "%a ME FW DID ACK has requested action %d (default case)\n", MeUmaPrefix, BiosAction));

  case CBM_DIR_CONTINUE_POST:
    ///
    /// Continue to POST
    ///
    DEBUG ((DEBUG_INFO, "%a ME FW DID Ack requested to continue to POST.\n", MeUmaPrefix));
    break;
  }

  return EFI_SUCCESS;
}

/**
  Get pointer to ME_UMA_INFO stored in a HOB

  @retval ME_UMA_INFO*    Pointer to ME_UMA_INFO
  @retval NULL            Error, HOB cant be located
**/
ME_UMA_INFO*
EFIAPI
MeUmaGetInfo (
  VOID
  )
{
  ME_UMA_INFO        *MeUmaInfo;
  EFI_HOB_GUID_TYPE  *GuidHobPointer;

  MeUmaInfo = NULL;
  GuidHobPointer = GetFirstGuidHob (&gMeUmaInfoHobGuid);
  if (GuidHobPointer != NULL) {
    MeUmaInfo = GET_GUID_HOB_DATA (GuidHobPointer);
  }

  return MeUmaInfo;
}