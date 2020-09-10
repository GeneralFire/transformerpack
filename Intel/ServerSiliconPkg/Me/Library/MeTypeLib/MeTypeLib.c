/** @file
  MeTypeLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation.

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include "MeAccess.h"
#include "Register/HeciRegs.h"
#include "Library/MeTypeLib.h"
#include "MeTypeHob.h"
#include <Library/HobLib.h>
#include <Library/PmcLib.h>
#include <Library/PcdLib.h>
#include <MeBiosPayloadHob.h>

/**
  Get detected ME FW type

  @retval ME_TYPE_UNDEF            Can't determine ME Type
  @retval ME_TYPE_SPS              Detected SPS
  @retval ME_TYPE_CLIENT           Detected Client ME
  @retval ME_TYPE_DFX              Detected DFX ME
  @retval ME_TYPE_DISABLED         Disable all ME flows
**/
ON_BOARD_ME_TYPE
GetOnBoardMeType (
  VOID
  )
{
  ON_BOARD_ME_TYPE  retVal = ME_TYPE_UNDEF;
#ifdef ME_SUPPORT_FLAG
  if (PmcIsDwrBootMode () == TRUE) {
    DEBUG ((DEBUG_INFO, "HECI: DWR flow return Disabled type\n"));
    return ME_TYPE_DISABLED;
  }

  if (MeTypeIsSpsIgnition ())  {
    DEBUG ((DEBUG_INFO, "HECI: SPS Ignition flow return Disabled type\n"));
    return ME_TYPE_DISABLED;
  }

  retVal = GetMeTypeFromHob ();

  if (retVal == ME_TYPE_UNDEF) {
    DEBUG ((
      DEBUG_ERROR,
      "HECI: ME type not recognized (MEFS1: 0x%08X, MEFS2: 0x%08X)\n",
      HeciPciRead32 (R_ME_HFS),
      HeciPciRead32 (R_ME_HFS_2)
      ));
  }
#endif // ME_SUPPORT_FLAG

  return retVal;
}

/**
  Get detected Me FW type

  @retval MeType
**/
ME_TYPE
MeTypeGet (
  VOID
  )
{
  return GetOnBoardMeType ();
}

/**
  Checks if Me FW is DISABLED type

  @retval FALSE           ME is not disabled
  @retval TRUE            ME is disabled
**/
BOOLEAN
MeTypeIsDisabled (
  VOID
  )
{
  return (BOOLEAN)(GetOnBoardMeType () == ME_TYPE_DISABLED);
}

/**
  Checks if Me FW is DFX type

  @retval FALSE           No Dfx ME on board
  @retval TRUE            Dfx ME detected
**/
BOOLEAN
MeTypeIsDfx (
  VOID
  )
{
  return (BOOLEAN)(GetOnBoardMeType () == ME_TYPE_DFX);
}


/**
  Checks if Me FW is Client type

  @retval FALSE           No Client ME on board
  @retval TRUE            Client ME detected
**/
BOOLEAN
MeTypeIsClient (
  VOID
  )
{
  return (BOOLEAN)(GetOnBoardMeType () == ME_TYPE_CLIENT);
}

/**
  Checks if Me FW is SPS type

  @retval FALSE           No SPS on board
  @retval TRUE            SPS detected
**/
BOOLEAN
MeTypeIsSps (
  VOID
  )
{
  return (BOOLEAN)(GetOnBoardMeType () == ME_TYPE_SPS);
}


/**
  Print the the ME FW Type and Subtype.

  @param[in]  DispString     Pointer to string decorator to mark source of debug output
**/
VOID
PrintMeType (
  IN CHAR16 *DispString
  )
{
  UINT8      FwType;
  UINT8      FwSubType;
  EFI_STATUS Status;

  Status = GetMeFwTypeSubTypeFromHob (&FwType, &FwSubType);

  if (DispString == NULL) {
    DispString = L"PrintMeType ()";
  }

  if (!EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_INFO,
      "[ME] %s: FwType = %d, FwSubType = %d\n",
      DispString,
      FwType,
      FwSubType
      ));
  }

  // Display detected ME only
  MeTypeShowDebug (DispString);
}

/**
  Displays debug information about detected Me FW type

  @param[in] Leading debug string
**/
VOID
MeTypeShowDebug (
  IN CHAR16 *DispString
  )
{
  ON_BOARD_ME_TYPE  MeType = GetOnBoardMeType();

  if (DispString == NULL) {
    DispString = L"MeTypeShowDebug()";
  }

  DEBUG ((DEBUG_ERROR, "[HECI] %s (MeType is ", DispString));
  switch (MeType) {
  case ME_TYPE_UNDEF:
    DEBUG ((DEBUG_ERROR, "ME_TYPE_UNDEF"));
    break;
  case ME_TYPE_SPS:
    DEBUG ((DEBUG_ERROR, "ME_TYPE_SPS"));
    break;
  case ME_TYPE_CLIENT:
    DEBUG ((DEBUG_ERROR, "ME_TYPE_CLIENT"));
    break;
  case ME_TYPE_DFX:
    DEBUG ((DEBUG_ERROR, "ME_TYPE_DFX"));
    break;
  case ME_TYPE_DISABLED:
    DEBUG ((DEBUG_ERROR, "ME_TYPE_DISABLED"));
    break;
  default:
    DEBUG ((DEBUG_ERROR, "UNKNOWN"));;
  }
  DEBUG ((DEBUG_ERROR, ")\n"));
}

/**
  Get ME FW type and subtype from HOB.

  @param[out] FwType     ME FW type
  @param[out] FwSubType  ME FW subtype

  @retval EFI_SUCCESS            Success
  @retval EFI_UNSUPPORTED        Not supported
  @retval EFI_INVALID_PARAMETER  Both parameters are NULL
  @retval EFI_NOT_FOUND          Cannot get MeType HOB
**/
EFI_STATUS
GetMeFwTypeSubTypeFromHob (
  OUT UINT8 *FwType,
  OUT UINT8 *FwSubType
)
{
  EFI_STATUS Status;

  Status = EFI_UNSUPPORTED;

#ifdef ME_SUPPORT_FLAG
  if (FwType != NULL || FwSubType != NULL) {
    ME_FW_TYPE_HOB *MeTypeHob;

    MeTypeHob = GetFirstGuidHob (&gMeTypeHobGuid);
    if (MeTypeHob != NULL) {
      if (FwType != NULL) {
        *FwType = MeTypeHob->FwType;
      }
      if (FwSubType != NULL) {
        *FwSubType = MeTypeHob->FwSubType;
      }
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_NOT_FOUND;
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  Checks if Me FW is SPS Ignition type

  @retval FALSE           No SPS Ignition on board
  @retval TRUE            SPS Ignition detected
**/
BOOLEAN
MeTypeIsSpsIgnition (
  VOID
  )
{
  HECI_FWS_REGISTER     MeFirmwareStatus;

  MeFirmwareStatus.ul = HeciPciRead32 (R_ME_HFS);
  if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "ERROR: HECI interface is disabled\n"));
    return TRUE;
  }

  if (MeFirmwareStatus.r.MeOperationMode == ME_OPERATION_MODE_SPS_IGNITION) {
    return TRUE;
  }

  return FALSE;
}