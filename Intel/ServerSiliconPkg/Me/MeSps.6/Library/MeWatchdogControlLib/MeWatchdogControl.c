/** @file
  Implements ME Watchdog HECI Interface via SMM Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#include <Sps.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <MeAccess.h>

#include <Protocol/SmmCommunication.h>
#include <Protocol/SmmBase2.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeWatchdogControlLib.h>
#include <Protocol/HeciControlProtocol.h>

#include <ConfigBlock/SpsGetDxeConfigBlock.h>
#include <ConfigBlock/Sps/SpsDxeConfig.h>
#include <Guid/SpsInfoHobGuid.h>
#include <DualBiosMsgs.h>

static BOOLEAN mWatchdogControlEnabled = FALSE;

CHAR8 *mPrefix;
CHAR8 mPrefixSPS[]    = "SPS";
CHAR8 mPrefixSPSSMM[] = "SPS_SMM";

/**
  The constructor function initializes the ME Watchdog library

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
MeWatchdogLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HOB_GUID_TYPE    *SpsHob;
  SPS_INFO_HOB         *SpsHobData;
  SPS_DXE_CONFIG       *pSpsPolicy = NULL;

  if (IsInSmm (NULL)) {
    mPrefix = mPrefixSPSSMM;
  } else {
    mPrefix = mPrefixSPS;
  }

  if (MeTypeIsSps ()) {
    SpsHob = GetFirstGuidHob (&gSpsInfoHobGuid);
    pSpsPolicy = GetSpsDxeConfigBlock ();
    if (pSpsPolicy == NULL) {
      DEBUG ((DEBUG_ERROR, "[%a] ERROR: Cannot get SPS configuration policy\n", mPrefix));
      return EFI_SUCCESS;
    }

    if (SpsHob != NULL) {
      SpsHobData = GET_GUID_HOB_DATA (SpsHob);
      if ((SpsHobData->FeatureSet.Bits.DualBIOSSupport) &&
          (pSpsPolicy->Common.MeWatchdogControlEnabled)) {
        mWatchdogControlEnabled = TRUE;
      }
    }
    DEBUG ((DEBUG_INFO, "[%a] ME Watchdog Service %aavailable\n", mPrefix, (mWatchdogControlEnabled) ? "" : "NOT "));
  }

  return EFI_SUCCESS;
}

/**
  Checks whether ME Watchdog Control is enabled or not.

  @returns True is returned if ME is functional and supports Watchdog
           functionality, otherwise false.
 **/
BOOLEAN
EFIAPI
MeWatchdogControlIsEnabed (
  VOID
  )
{
  return mWatchdogControlEnabled;
}

/**
  Procedure to send BiosUpdateGetInventory HECI Request

  @param[in]      pHeci           pointer to HECI driver
  @param[in]      ImageId         identifier of region to check
  @param[in,out]  ImageInventory  buffer for Image Inventory from HECI response

  @return EFI_SUCCESS            Returned if success.
  @return EFI_INVALID_PARAMETER  Some parameter is wrong.
  @return EFI_UNSUPPORTED        If not SPS or unsuccessful response from SPS.
*/
EFI_STATUS
HeciReq_GetInventory (
  IN      HECI_CONTROL     *pHeci,
  IN      UINT8            ImageId,
  IN OUT  IMAGE_INVENTORY  *ImageInventory
  )
{
  UINT32      BufLen;
  EFI_STATUS  Status;

  union {
    HECI_MSG_BIOS_UPDATE_GET_IMAGE_INVENTORY_REQ Req;
    HECI_MSG_BIOS_UPDATE_GET_IMAGE_INVENTORY_RSP Rsp;
  } HeciMsg;

  if ((pHeci == NULL) || (ImageInventory == NULL)) {
    DEBUG ((DEBUG_ERROR, "[%a] Some parameter is wrong.\n", mPrefix));
    return EFI_INVALID_PARAMETER;
  }

  if (!MeTypeIsSps ()) {
    return EFI_UNSUPPORTED;
  }

  // Initializing the HeciMsg
  ZeroMem (&HeciMsg, sizeof (HeciMsg));

  DEBUG ((DEBUG_INFO, "[%a] Sending BIOS_UPDATE_GET_IMAGE_INVENTORY_REQ\n", mPrefix));

  //construct HECI request
  HeciMsg.Req.Command = SPS_CMD_BIOS_UPDATE_GET_IMAGE_INVENTORY_REQ;
  HeciMsg.Req.ImageId = ImageId;

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
    DEBUG ((
      DEBUG_ERROR,
      "[%a] ERROR: Cannot send BIOS_UPDATE_GET_IMAGE_INVENTORY_REQ (%r)\n",
      mPrefix,
      Status
      ));
    return Status;
  } else if (HeciMsg.Rsp.Command != SPS_CMD_BIOS_UPDATE_GET_IMAGE_INVENTORY_RSP) {
    DEBUG ((DEBUG_ERROR, "[%a] ERROR: BIOS_UPDATE_GET_IMAGE_INVENTORY response failed\n", mPrefix));
    return EFI_UNSUPPORTED;
  } else if (HeciMsg.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "[%a] ERROR: BIOS_UPDATE_GET_IMAGE_INVENTORY failed (status: %d)\n",
      mPrefix,
      HeciMsg.Rsp.Status
      ));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "[%a] BIOS_UPDATE_GET_IMAGE_INVENTORY reports success\n", mPrefix));

  //copy image inventory value to the result buffer
  ImageInventory->Byte = HeciMsg.Rsp.ImageInventory.Byte;

  return EFI_SUCCESS;
}

/**
  Function returns number of the current used bios region.

  @param[in]   pHeci             pointer to HECI driver
  @param[out]  Region            Buffer for the response - number of the current used region.
                                 Proper region number can be equal only to 0 or 1.
  @return EFI_SUCCESS            Returned if success.
  @return EFI_INVALID_PARAMETER  Some parameter is wrong.
  @return EFI_DEVICE_ERROR       Can't recognize which bios region is running and active.
*/
EFI_STATUS
GetActiveRegionNum (
  IN  HECI_CONTROL   *pHeci,
  OUT UINT8          *Region
  )
{
  EFI_STATUS        Status;
  IMAGE_INVENTORY   ImageInventory;
  UINT8             BiosRegions[] = {
                      BIOS_UPDATE_GET_IMAGE_INVENTORY_REGION1,
                      BIOS_UPDATE_GET_IMAGE_INVENTORY_REGION2
                      };
  UINT8             Index;
  UINT8             BiosRegionNumber;

  if ((pHeci == NULL) || (Region == NULL)) {
    DEBUG ((DEBUG_ERROR, "[%a] Some parameter is wrong.\n", mPrefix));
    return EFI_INVALID_PARAMETER;
  }

  BiosRegionNumber = sizeof (BiosRegions) / sizeof (UINT8);

  for (Index = 0; Index < BiosRegionNumber; Index++) {
    // Check if DualBios watchdog is running
    Status = HeciReq_GetInventory (pHeci, BiosRegions[Index], &ImageInventory);

    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "[%a] ERROR: Can't get first bios image inventory status. Status=%r\n",
        mPrefix,
       Status
        ));
      return EFI_DEVICE_ERROR;
    } else {
      if (ImageInventory.Bits.IsRunning == 1) {
        // Bios is booted from iterating region - check the status
        if (ImageInventory.Bits.ImageStatus == BIOS_UPDATE_GET_IMAGE_INVENTORY_IMG_STATUS_ACTIVE) {
          // Bios update watchdog is running - active
          DEBUG ((DEBUG_INFO, "[%a] Bios region %d is active.\n", mPrefix, Index));
          *Region = Index;
          return EFI_SUCCESS;
        } else {
          // Bios region in not active
          DEBUG ((DEBUG_INFO, "[%a] Bios region %d is not active.\n", mPrefix, Index));
        }
      }
    }
  }

  // Neither first nor second bios region is running - should never happen
  DEBUG ((
    DEBUG_ERROR,
    "[%a] ERROR: Can't recognize which bios region is running. ME reports both as not active.\n",
    mPrefix
    ));

  return EFI_DEVICE_ERROR;
}

