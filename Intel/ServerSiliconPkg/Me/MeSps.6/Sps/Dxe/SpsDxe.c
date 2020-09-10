/** @file
  This driver implements the DXE phase of SPS support as defined in
  SPS ME-BIOS Interface Specification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MeMeasurementsLib.h>
#include <ConfigBlock/SpsGetDxeConfigBlock.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Guid/GlobalVariable.h>
#include <Guid/HobList.h>
#include <Guid/EventLegacyBios.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Guid/EventGroup.h>
#include <Protocol/MpService.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/PciIo.h>
#include <Sps.h>
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include "SpsDxe.h"
#include "DualBiosWatchdog.h"
#include <Library/MeCosignLib.h>
#include <Library/SpsDxeLib.h>

#define DEFAULT_PCH_TEMP_REPORTING_INTERVAL     250
#define DEFAULT_PCH_TEMP_MAX_LOW_POWER_INTERVAL 1000

/******************************************************************************
 * Function prototypes
 */
EFI_STATUS SpsSendPchTempReportingCfg (VOID);


/******************************************************************************
 * Variables
 */
EFI_EVENT                   mSpsEndOfDxeEvent     = NULL;
EFI_EVENT                   mSpsReadyToBootEvent  = NULL;
SPS_DXE_CONTEXT             *mpSpsContext         = NULL;
EFI_EVENT                   mDfxReadyToBootEvent  = NULL;

/******************************************************************************
 * Functions
 */
/******************************************************************************
 * @brief Send PCH temperature reporting configuration to ME.
 *
 *   Note that this functionality is limited to BDX-DE only.
 *
 *  @retval EFI status
 **/
EFI_STATUS
SpsSendPchTempReportingCfg (VOID)
{
  EFI_STATUS         Status;
  UINT32             RspLen;
  union {
    HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_REQ Req;
    HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_RSP Rsp;
  } HeciMsg;
  HECI_CONTROL            *pHeci;

  if (MeSpsInRecovery ()) {
    DEBUG ((DEBUG_WARN, "[SPS] WARNING: Using PCH temperature from ME is enabled, but ME is not "
                       "operational!"));
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (%r)\n", Status));
    return Status;
  }

  SetMem (&HeciMsg, sizeof (HeciMsg), 0);
  HeciMsg.Req.Command = SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_REQ;
  HeciMsg.Req.PchTempReportingInterval = DEFAULT_PCH_TEMP_REPORTING_INTERVAL;
  HeciMsg.Req.PchTempMaximumLowPowerInterval = DEFAULT_PCH_TEMP_MAX_LOW_POWER_INTERVAL;

  DEBUG ((DEBUG_INFO, "[SPS] Sending PCH temperature reporting configuration to ME\n"));
  DEBUG ((DEBUG_INFO, "[SPS]   PCH Temperature Reporting Interval: 0x%04x\n", HeciMsg.Req.PchTempReportingInterval));
  DEBUG ((DEBUG_INFO, "[SPS]   PCH Temperature Maximum Low Power Interval: 0x%04x\n", HeciMsg.Req.PchTempMaximumLowPowerInterval));

  RspLen = sizeof (HeciMsg.Rsp);
  Status = pHeci->HeciSendAndReceive (
                    pHeci,
                    HECI_DEFAULT_PCH_ID,
                    NULL,
                    HECI_DEFAULT_RETRY_NUMBER,
                    SPS_CLIENTID_BIOS,
                    SPS_CLIENTID_ME_SPS,
                    (UINT32*) &HeciMsg.Req,
                    sizeof (HeciMsg.Req),
                    (UINT32*) &HeciMsg.Rsp,
                    &RspLen,
                    NULL,
                    NULL
                    );
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send PCH temperature reporting configuration to ME (%r)\n", Status));
  }
  else if (RspLen != sizeof (HeciMsg.Rsp) || HeciMsg.Rsp.Result != 0)
  {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: PCH temperature reporting configuration NACK (result: %d)\n", HeciMsg.Rsp.Result));
  }
  return Status;
}


/**
    @brief This function allows to retrieve SPS HOB.

    @return SPS_INFO_HOB is returned, or NULL if the HOB not found.
**/
SPS_INFO_HOB*
SpsGetHob (VOID)
{
  EFI_HOB_GUID_TYPE     *pGuidHob;

  pGuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
  if (pGuidHob == NULL)
  {
    return NULL;
  }
  return GET_GUID_HOB_DATA (pGuidHob);
} // SpsGetHob ()

/**
  SPS DXE actions execution function when all dependencies are met.

  @param Event      Event information
  @param Context    Called context information, pointer to EFI_STATUS
**/
VOID
EFIAPI
SpsDxeStartActionCallback (
  IN EFI_EVENT      Event,
  IN VOID           *Context
  )
{
  EFI_STATUS        Status;
  SPS_INFO_HOB      *pSpsHob;
  SPS_DXE_CONFIG    *pSpsPolicy = NULL;

  if (Context != NULL) {
    // Fill context Status
    *(EFI_STATUS*) Context = EFI_SUCCESS;
  }

  if (VerifyDependencyRules (NULL) == FALSE) {
    DEBUG ((DEBUG_ERROR, "[SPS] %a: Not all dependency are available - waiting\n"));
    return;
  }
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  DEBUG ((DEBUG_INFO, "[SPS] DXE PHASE - EXECUTION\n"));

  pSpsHob = SpsGetHob ();
  if (pSpsHob == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot find SPS info HOB\n"));
    return;
  }
  if (pSpsHob->WorkFlow != ME_FUNCTIONAL) {
    DEBUG ((DEBUG_WARN, "[SPS] WARNING: ME is not functional, driver will be unloaded\n"));
    return;
  }
  DEBUG ((DEBUG_INFO, "[SPS] HOB: flow %d, feature set 0x%08X, feature set 2 0x%08X, pwr opt boot %d, cores2disable %d, pwr msmt %d, hw change %d\n",
        pSpsHob->WorkFlow, pSpsHob->FeatureSet.Data.Set1, pSpsHob->FeatureSet.Data.Set2, pSpsHob->PwrOptBoot, pSpsHob->Cores2Disable,
        pSpsHob->NmPowerMsmtSupport, pSpsHob->NmHwChangeStatus));

  pSpsPolicy = GetSpsDxeConfigBlock ();
  if (pSpsPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot get SPS configuration policy\n"));
    return;
  }

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (*mpSpsContext), (VOID**)&mpSpsContext);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Memory allocation failed (%r). Driver will be unloaded.\n", Status));
    return;
  }
  SetMem (mpSpsContext, sizeof (*mpSpsContext), 0);
  mpSpsContext->pSpsHob = pSpsHob;
  mpSpsContext->pSpsPolicy = pSpsPolicy;

  if (mpSpsContext->pSpsHob->FeatureSet.Bits.SocThermalReporting)
  {
    SpsSendPchTempReportingCfg ();
  }

#if ME_TESTMENU_FLAG
  //
  // This is test option which can break RTC configuration so that ME can detect it and handle.
  //
#define PCAT_RTC_ADDRESS_REGISTER 0x70
#define PCAT_RTC_DATA_REGISTER    0x71
#define RTC_ADDRESS_REGISTER_A    10
#define RTC_BAD_VALUE             54
  if (pSpsPolicy->BreakRtcEnabled)
  {
    IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, (UINT8) (RTC_ADDRESS_REGISTER_A | (UINT8) (IoRead8 (PCAT_RTC_ADDRESS_REGISTER) & 0x80)));
    IoWrite8 (PCAT_RTC_DATA_REGISTER, RTC_BAD_VALUE);
  }
#endif

  if (GetBootModeHob () != BOOT_ON_S4_RESUME &&
      (pSpsPolicy->Common.NmPwrOptBoot != pSpsHob->PwrOptBoot ||
       pSpsPolicy->Common.NmCores2Disable != pSpsHob->Cores2Disable))
  {
#if ME_TESTMENU_FLAG
    if (!pSpsPolicy->Common.NmPwrOptBootOverride)
#endif
    {
      DEBUG ((DEBUG_INFO, "[SPS] Updating NM boot mode %d->%d\n", pSpsPolicy->Common.NmPwrOptBoot, pSpsHob->PwrOptBoot));
      pSpsPolicy->Common.NmPwrOptBoot = pSpsHob->PwrOptBoot;
      Status = !EFI_SUCCESS;
    }
#if ME_TESTMENU_FLAG
    if (!pSpsPolicy->Common.NmCores2DisableOverride)
#endif
    {
      DEBUG ((DEBUG_INFO, "[SPS] Updating NM cores to disable %d -> %d\n", pSpsPolicy->Common.NmCores2Disable, pSpsHob->Cores2Disable));
      pSpsPolicy->Common.NmCores2Disable = pSpsHob->Cores2Disable;
      Status = !EFI_SUCCESS;
    }
    if (Status != EFI_SUCCESS)
    {
      if (pSpsPolicy->SpsSavePolicy != NULL) {
        Status = pSpsPolicy->SpsSavePolicy (pSpsPolicy);
      } else {
        Status = EFI_INVALID_PARAMETER;
      }
      ASSERT_EFI_ERROR (Status);
    }
  }
  //
  // Send Host Configuration Info even for SiEn. This is recommended in SPS ME-BIOS Spec.
  //
  SpsNmSendHostCfgInfo (mpSpsContext);

  Status = RegisterMeMeasurementsEndOfDxeNotification ();
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, SpsEndOfDxeCallback,
                              mpSpsContext, &gEfiEndOfDxeEventGroupGuid, &mSpsEndOfDxeEvent);
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventReadyToBootEx (TPL_CALLBACK, SpsReadyToBootCallback,
                                       mpSpsContext, &mSpsReadyToBootEvent);
  ASSERT_EFI_ERROR (Status);

  // Init DualBios feature
  DualBiosWatchdogInit (mpSpsContext);


  //
  // Send ARB SVN commit command if CPU BTG capable and command is enabled in setup menu
  //
  if (pSpsHob->FeatureSet.Bits.BootGuard && pSpsPolicy->InvokeArbSvnCommit) {
    MeCoSignArbSvnCommit ();
  }
} // SpsDxeStartActionCallback ()

/**
  SPS DXE driver entry point.

  @param ImageHandle  Handle to SPS DXE image
  @param pSystemTable Pointer to system table

  @retval EFI_STATUS  Entry Point activities result
**/
EFI_STATUS
EFIAPI
SpsDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *pSystemTable
  )
{
  EFI_STATUS   Status;
  EFI_EVENT    Event;

  DEBUG ((DEBUG_INFO, "[SPS] DXE PHASE - LOAD\n"));

  if (MeTypeIsDfx ()) {
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               DfxReadyToBootCallback,
               NULL,
               &mDfxReadyToBootEvent
               );
    return EFI_SUCCESS;
  }

  if (VerifyDependencyRules (SpsDxeStartActionCallback)) {
    // No notification is required
    // Execute SPS Actions
    Event = NULL;
    SpsDxeStartActionCallback (Event, NULL);
  }

  return EFI_SUCCESS;
} // SpsDxeEntryPoint ()