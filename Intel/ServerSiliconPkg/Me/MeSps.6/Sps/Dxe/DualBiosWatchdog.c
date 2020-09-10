/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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
**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>

#include <Protocol/HeciControlProtocol.h>

#include <Guid/SpsInfoHobGuid.h>

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>

#include <Sps.h>
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include "SpsDxe.h"
#include <Library/MeWatchdogControlLib.h>
#include "DualBiosWatchdog.h"

/**
 Procedure to init control of the ME watchdog after bios update

 For the first boot of the new image ME starts the watchdog to discover boot error
 Bios needs to suspend, resume and stops this watchdog.

 **/
VOID DualBiosWatchdogInit ( IN OUT SPS_DXE_CONTEXT *mpSpsContext)
{
  EFI_STATUS            Status;
  IMAGE_INVENTORY       ImageInv;
  HECI_CONTROL          *pHeci;

  // watchdog is disabled by default
  mpSpsContext->IsDualBiosWatchdogRunning = FALSE;

  if (!MeTypeIsSps ()) {
    return;
  }

  if ( !mpSpsContext->pSpsHob->FeatureSet.Bits.DualBIOSSupport ) {
    DEBUG  ((DEBUG_INFO, "[SPS] ME doesn't support DualBios feature.\n"));
    return;
  }

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: HECI protocol not found (%r)\n", Status));
    return;
  }

#ifdef ME_TESTMENU_FLAG
  if (!mpSpsContext->pSpsPolicy->Common.MeWatchdogControlEnabled) {
    DEBUG (
        (DEBUG_ERROR, "[SPS] BiosUpdate_WatchdogControl messages disabled in setup.\n"));
    return;
  }
#endif //ME_TESTMENU_FLAG

  // Check if DualBios watchdog is running (this is first boot after bios image update)
  Status = HeciReq_GetInventory (pHeci, BIOS_UPDATE_GET_IMAGE_INVENTORY_REGION1,
      &ImageInv);

  if (EFI_ERROR (Status)) {
    DEBUG (
        (DEBUG_ERROR, "[SPS] ERROR: Can't get bios first image inventory status. Status=%r\n", Status));
    DEBUG (
        (DEBUG_ERROR, "[SPS] ERROR: ME bios update watchdog handling skipped.\n"));
    return;
  } else {

    if (ImageInv.Bits.IsRunning == 1) {
      //BIOS is booted from region1 - check the status
      if (ImageInv.Bits.ImageStatus
          == BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_BOOTNEXT) {

        //Bios update watchdog is running - bios needs to handle it
        DEBUG ((DEBUG_INFO, "[SPS] ME bios update watchdog handling init.\n"));

        //init callbacks - continue...

      } else {
        DEBUG (
            (DEBUG_INFO, "[SPS] ME bios update watchdog handling init skipped.\n"));
        return;
      }

    } else {

      //region1 is not running - try region2
      Status = HeciReq_GetInventory (pHeci,
          BIOS_UPDATE_GET_IMAGE_INVENTORY_REGION2, &ImageInv);
      if (EFI_ERROR (Status)) {
        DEBUG (
            (DEBUG_ERROR, "[SPS] ERROR: Can't get second bios image inventory status. Status=%r\n", Status));
        DEBUG (
            (DEBUG_ERROR, "[SPS] ERROR: ME bios update watchdog handling skipped.\n"));
        return;
      } else {

        if (ImageInv.Bits.IsRunning == 1) {
          //BIOS is booted from region2 - check the status

          if (ImageInv.Bits.ImageStatus
              == BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_BOOTNEXT) {

            //Bios update watchdog is running - bios needs to handle it
            DEBUG (
                (DEBUG_INFO, "[SPS] ME bios update watchdog handling init.\n"));
            //init callbacks - continue...

          } else {
            DEBUG (
                (DEBUG_INFO, "[SPS] ME bios update watchdog handling init skipped.\n"));
            return;
          }

        } else {
          //Neither region1 nor region2 is running - strange - should never happen
          DEBUG (
              (DEBUG_ERROR,"[SPS] ERROR: Can't recognize which bios region is running. ME reports both as inactive.\n"));
          return;
        }
      }
    }
  }

  mpSpsContext->IsDualBiosWatchdogRunning = TRUE;
  mpSpsContext->pSpsHob->DualBIOSWatchdogEnabled = TRUE;
}

/** Procedure to send BiosUpdateWatchdogControl HECI Request

 @param[in]    *pHeci  Pointer to HECI driver
 @param[in]    Action  Action to send in request

 @return EFI_STATUS is returned.
 */
EFI_STATUS HeciReq_WatchdogControl (
  IN HECI_CONTROL          *pHeci,
  IN UINT8                 Action
  )
{
  UINT32     BufLen;
  EFI_STATUS Status;

  union {
    HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_REQ Req;
    HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_RSP Rsp;
  } HeciMsg;

  if (!MeTypeIsSps ()) {
    return EFI_UNSUPPORTED;
  }

  // Initializing the HeciMsg
  ZeroMem (&HeciMsg, sizeof (HeciMsg));

  DEBUG (
      (DEBUG_INFO, "[SPS] Sending SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ\n"));

  //construct Heci request
  HeciMsg.Req.Command = SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ;
  HeciMsg.Req.ActionFlag = Action;

  BufLen = sizeof (HeciMsg.Rsp);

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
                    &BufLen,
                    NULL,
                    NULL
                    );
  if (EFI_ERROR (Status)) {
    DEBUG (
        (DEBUG_ERROR, "[SPS] ERROR: Cannot send SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ (%r)\n", Status));

  } else if (HeciMsg.Rsp.Command != SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_RSP) {
    DEBUG (
        (DEBUG_ERROR, "[SPS] ERROR: SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL response failed\n"));
    Status = EFI_UNSUPPORTED;

  } else if (HeciMsg.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG (
        (DEBUG_ERROR, "[SPS] ERROR: BIOS_UPDATE_WATCHDOG_CONTROL failed (status: %d)\n", HeciMsg.Rsp.Status));
    Status = EFI_UNSUPPORTED;

  } else {
    //no error, status OK
    DEBUG ((DEBUG_INFO, "[SPS] BIOS_UPDATE_WATCHDOG_CONTROL reports success\n"));
  }

  return Status;
}

/**
  Function returns number of the current used bios region.

  @param[out]  Region       Buffer for the response - number of the current used region.
                            Proper region number can be equal only to 0 or 1.
  @return EFI_SUCCESS       Returned if success.
  @return EFI_DEVICE_ERROR  Smm Heci Protocol not found or can't recognize which bios region
                            is running and active.
*/
EFI_STATUS
GetActiveRegion (
  OUT UINT8     *Region
  )
{
  EFI_STATUS      Status;
  HECI_CONTROL    *pHeci;

  // find HECI protocol
  pHeci = NULL;
  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Can't locate HeciProtocol.\n"));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }
  return GetActiveRegionNum (pHeci, Region);
}

