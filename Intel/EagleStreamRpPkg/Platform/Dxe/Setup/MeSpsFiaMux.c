/** @file

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

#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT

#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/HiiLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeMeLib.h>
#include <Guid/MdeModuleHii.h>
#include "Library/MeTypeLib.h"

#include <Library/SetupLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeFiaMuxLib.h>
#include <MeSpsSetup.h>
#include <MeSpsFiaMux.h>
#include <FiaMuxConfig.h>

/**
  Update FiaMux information in setup based on data received from SPS

  @param[out] HiiAdvancedHandle   BIOS setup database handle
  @param[out] PchSetup            PchSetup pointer to update suppress information
  @param[in]  FiaMuxConfigPtr     Pointer to FiaMux configuration

  @retval EFI_INVALID_PARAMETER   Bad input pointer
  @retval EFI_OUT_OF_RESOURCES    Can't allocate memory buffer
  @retval EFI_DEVICE_ERROR        Unexpected device in configuration
  @retval EFI_SUCCESS             Function execution success
**/
EFI_STATUS
FiaMuxUpdateStatusStrings (
  OUT    EFI_HII_HANDLE      HiiAdvancedHandle,
  OUT    PCH_SETUP           *PchSetup,
      IN ME_FIA_CONFIG       *FiaMuxConfigPtr
  )
{
  EFI_STATUS     Status;
  CHAR16         *StrBufPtr = NULL;
  EFI_STRING_ID  LaneTable[] = {
                   STR_FIA_LANE_0_STATUS_VALUE,
                   STR_FIA_LANE_1_STATUS_VALUE,
                   STR_FIA_LANE_2_STATUS_VALUE,
                   STR_FIA_LANE_3_STATUS_VALUE,
                   STR_FIA_LANE_4_STATUS_VALUE,
                   STR_FIA_LANE_5_STATUS_VALUE,
                   STR_FIA_LANE_6_STATUS_VALUE,
                   STR_FIA_LANE_7_STATUS_VALUE,
                   STR_FIA_LANE_8_STATUS_VALUE,
                   STR_FIA_LANE_9_STATUS_VALUE,
                   STR_FIA_LANE_10_STATUS_VALUE,
                   STR_FIA_LANE_11_STATUS_VALUE,
                   STR_FIA_LANE_12_STATUS_VALUE,
                   STR_FIA_LANE_13_STATUS_VALUE,
                   STR_FIA_LANE_14_STATUS_VALUE,
                   STR_FIA_LANE_15_STATUS_VALUE,
                   STR_FIA_LANE_16_STATUS_VALUE,
                   STR_FIA_LANE_17_STATUS_VALUE,
                   STR_FIA_LANE_18_STATUS_VALUE,
                   STR_FIA_LANE_19_STATUS_VALUE,
                   STR_FIA_LANE_20_STATUS_VALUE,
                   STR_FIA_LANE_21_STATUS_VALUE,
                   STR_FIA_LANE_22_STATUS_VALUE,
                   STR_FIA_LANE_23_STATUS_VALUE
                   };
  UINT32         FiaLaneIndex;
  UINT32         FiaRecordIndex;
  UINT32         LanesToSet;
  UINT8          HardwareIndex;
  CHAR8          *FiaMuxSelectionString;
  CHAR8          *SataString;
  CHAR8          *RootPortString;
  UINT8          HsioLanes;
  UINT8          LowLane;
  UINT8          HiLane;

  DEBUG ((DEBUG_INFO, "[ME] %a ()\n", __FUNCTION__));

  if (FiaMuxConfigPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_OUT_OF_RESOURCES;
  StrBufPtr = AllocateZeroPool (ME_SPS_INFO_MAX_STR_SIZE * sizeof (CHAR16));
  ASSERT (NULL != StrBufPtr);
  if (StrBufPtr != NULL) {
    LanesToSet = sizeof (LaneTable) / sizeof (EFI_STRING_ID);
    Status = EFI_SUCCESS;
    HsioLanes = GetHsioLanes (&LowLane, &HiLane);
    FiaRecordIndex = 0;

    if (LanesToSet < HsioLanes) {
      ASSERT_EFI_ERROR (LanesToSet < HsioLanes);
      HsioLanes = (UINT8) LanesToSet;
    }

    for (FiaLaneIndex = 0; ((FiaLaneIndex < HsioLanes) && (FiaRecordIndex < FiaMuxConfigPtr->FiaMuxRecordsCount)); FiaLaneIndex++) {
      if (FiaRecordIndex >= LanesToSet) {
        Status = EFI_DEVICE_ERROR;
        ASSERT_EFI_ERROR (Status);
        break;
      }
      HardwareIndex = FiaMuxConfigPtr->FiaMux[FiaLaneIndex].FiaMuxHardwareIndex;
      if (HardwareIndex >= LanesToSet) {
        Status = EFI_DEVICE_ERROR;
        ASSERT_EFI_ERROR (Status);
        break;
      }

      SataString = "N/A";
      RootPortString = "N/A";
      switch (FiaMuxConfigPtr->FiaMux[FiaLaneIndex].FiaMuxSelection) {
        case FIA_MUX_LANE_DISABLED:
          FiaMuxSelectionString = "disabled";
          break;
        case FIA_MUX_LANE_PCIE_OWNER:
          FiaMuxSelectionString = "PCIE owner";
          if (FiaMuxConfigPtr->PcieRootPort[FiaLaneIndex].PcieRootPortEnable == PCIE_ROOT_PORT_IS_ENABLED) {
            if (FiaMuxConfigPtr->PcieRootPort[FiaLaneIndex].PcieRootPortLinkWidth == PCIE_ROOT_PORT_LINK_WIDTH_SET_BY_BICTRL) {
              RootPortString = "enabled, with BITCTRL";
            } else {
              RootPortString = "enabled, with 1x";
            }
          } else {
            RootPortString = "not enabled";
          }
          break;
        case FIA_MUX_LANE_SATA_OWNER:
          FiaMuxSelectionString = "SATA owner";
          if (FiaMuxConfigPtr->Sata[FiaLaneIndex].SataSelection == SATA_CONTROLLER_ASSIGNED) {
            SataString = "assigned";
          } else {
            SataString = "not assigned";
          }
          break;
        case FIA_MUX_LANE_USB_OWNER:
          FiaMuxSelectionString = "USB owner";
          break;
        default:
          FiaMuxSelectionString = "unknown";
          break;
      }
      if (((FiaLaneIndex > LowLane) && (FiaLaneIndex < HiLane)) ||
           (FiaLaneIndex > HsioLanes)) {
        if (PchSetup != NULL) {
          PchSetup->HideFiaLane[FiaLaneIndex] = 1;
        }
        continue;
      }
      if (PchSetup != NULL) {
        PchSetup->HideFiaLane[FiaLaneIndex] = 0;
      }
      UnicodeSPrint (
        StrBufPtr,
        ME_SPS_INFO_MAX_STR_SIZE,
        L"Lane %d %a [SATA: %a][RP: %a])",
        HardwareIndex,
        FiaMuxSelectionString,
        SataString,
        RootPortString
        );
      HiiSetString (HiiAdvancedHandle, STRING_TOKEN(LaneTable[HardwareIndex]), StrBufPtr, NULL);
      FiaRecordIndex++;
    }
    FreePool (StrBufPtr);
    StrBufPtr = NULL;
  }

  return Status;
}

/**
  This function updates ME related strings for FiaMux in BIOS setup database.

  Some of the strings contain dummy values to be update with run-time data.
  These strings are updated by this function before any setup screen is
  printed. For some reason the strings from various screens,
  although identified with unique ids, are stored in separate databases.
  Class identifies the database. To avoid multiple requests to ME
  and multiple traces parts of this function are executed for particular
  forms.

  @param[out] HiiAdvancedHandle BIOS setup database handle
  @param[in]  SpsInfo           SpsInfo Hob pointer

  @retval EFI_SUCCESS         Always return EFI_SUCCESS
**/
EFI_STATUS
FiaMuxUpdateSetupInformation (
  OUT    EFI_HII_HANDLE HiiAdvancedHandle,
      IN SPS_INFO_HOB   *SpsInfo
  )
{
  PCH_SETUP           PchSetup;
  PCH_SETUP           PchSetupBack;
  UINT32              LanesAllowed;
  ME_FIA_CONFIG       *FiaMuxConfigPtr;
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "[ME] %a ()\n", __FUNCTION__));

  if (SpsInfo != NULL) {
    if (SpsInfo->FeatureSet.Bits.FiaMuxConfiguration) {
      Status = GetSpecificConfigGuid (&gPchSetupVariableGuid, (VOID*) &PchSetup);
      ASSERT_EFI_ERROR (Status);
      if (!EFI_ERROR (Status)) {
        CopyMem (&PchSetupBack, &PchSetup, sizeof (PCH_SETUP));
        FiaMuxConfigPtr = AllocateZeroPool (sizeof (ME_FIA_CONFIG));
        ASSERT (FiaMuxConfigPtr != NULL);
        if (FiaMuxConfigPtr != NULL) {
          Status = MeFiaMuxConfigGet (
                     FiaMuxConfigPtr,
                     NULL,
                     &LanesAllowed,
                     NULL
                     );
          if (!EFI_ERROR (Status)) {
            Status = FiaMuxUpdateStatusStrings (HiiAdvancedHandle, &PchSetup, FiaMuxConfigPtr);
            ASSERT_EFI_ERROR (Status);
            PchSetup.FiaMuxSupported = 1;
          } else {
            PchSetup.FiaMuxSupported = 0;
          }
          if (CompareMem (&PchSetupBack, &PchSetup, sizeof (PCH_SETUP)) != 0) {
            // Save changes to setup
            Status = SetSpecificConfigGuid (&gPchSetupVariableGuid, &PchSetup);
            ASSERT_EFI_ERROR (Status);
          }
          FreePool (FiaMuxConfigPtr);
        }
      }
    }
  }

  return EFI_SUCCESS;
}
#endif //ME_SPS_SUPPORT
