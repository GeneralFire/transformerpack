/** @file
  Implementation of PCLS routine library.

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

#include <Library/ImcRoutingLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PclsLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#include <Library/AmiOemRasLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

extern PCLS_STRUCT        *mPclsData;

/**
  Initialize PCLS sparing data

  @param  None

  @retval EFI_STATUS      - EFI_SUCCESS -  event was handled
  @retval                 - EFI_FAILURE - event not handled

**/
EFI_STATUS
EFIAPI
InitPclsSparing (
  VOID
  )
{
  IMC_RAS_STRUCTURE       *ImcRas;
  MEM_TOPOLOGY            *MemTopology;

  RAS_DEBUG ( (LEVEL_BASIC_FLOW, "[PCLS]: InitPclsSparing()... Start\n"));

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  //Register handler into IMC RAS table
  //
  ImcRas = GetImcRasData ();
  if (ImcRas == NULL) {
    return EFI_UNSUPPORTED;
  }
  RAS_DEBUG ( (LEVEL_BASIC_FLOW, "[PCLS]: Register CheckAndHandlePclsSparing()...\n"));
  ImcRas->RasEventHndlrTable [PCLS_INDEX] = CheckAndHandlePclsSparing;


  return EFI_SUCCESS;
}

/**
  Check and handle PCLS event. A PCLS event can only be a new event.
  PCLS operation does not span across multiple SMIs

  @param  NodeId          - Memory controller ID
  @param  EventType       - NEW_EVENTS or EVENTS_IN_PROGRESS

  @retval IsEventHandled  - TRUE -  event was handled
  @retval                 - FALSE - event not handled

**/

BOOLEAN
EFIAPI
CheckAndHandlePclsSparing (
  IN  UINT8         NodeId,
  IN  EVENT_TYPE    EventType
  )
{
  BOOLEAN           IsEventHandled;

  RAS_DEBUG ( (LEVEL_BASIC_FLOW,
    "[PCLS]: Main handler start...\n"));

  IsEventHandled = FALSE;
  switch (EventType) {
    case NEW_EVENTS:
      HandleNewPclsEvent (NodeId, &IsEventHandled);
      break;

    case EVENTS_IN_PROGRESS:
      break; //PCLS cannot be in progress

    default:
      break;
  }

  RAS_DEBUG ( (LEVEL_BASIC_FLOW,
    "[PCLS]: Main handler return\n"));

  return IsEventHandled;
}

/**
  Check if ADDDC region has been enabled on this channel previously.

  This is a helper function for silicon workaround S14010407697.  Once an ADDDC region
  has been enabled on a channel, all future PCLS events on that channel will be ignored.

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel on this memory controller

  @retval BOOLEAN         - TRUE if ADDDC region was previously detected
                          - FALSE if no ADDDC region was previously detected

**/
BOOLEAN
EFIAPI
CheckForAdddcOnCh (
  IN      UINT8         NodeId,
  IN      UINT8         ChOnNode
  )
{
  UINT8               RegionNum;
  UINT8               RegionEnable;

  if (mPclsData->AdddcRegion[NodeId][ChOnNode].Present == TRUE) {
    // ADDDC region was detected previously
    return TRUE;
  } else {
    for (RegionNum = 0; RegionNum < MAX_ADDDC_REGION; RegionNum++) {
      GetAdddcRegion (NODE_TO_SKT (NodeId), NODECHA_TO_SKTCHA (NodeId, ChOnNode), RegionNum, &RegionEnable, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      if (RegionEnable != 0) {
        // Flag channel for ADDDC region for future use
        mPclsData->AdddcRegion[NodeId][ChOnNode].Present = TRUE;
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Handle new PCLS event

  @param  NodeId          - Memory controller ID
  @param  IsEventHandled  - ptr to buffer to hold event handled status

  @retval Status          - EFI_SUCCESS if no failures
                          - non-zero error code for failures

  @retval IsEventHandled  - TRUE  -  event was handled
                          - FALSE - event not handled

**/
VOID
EFIAPI
HandleNewPclsEvent (
  IN  UINT8             NodeId,
  OUT BOOLEAN          *IsEventHandled
  )
{
  EFI_STATUS              Status;
  UINT8                   ChOnNode;
  SPARING_REGION_STRUCT   SparingRegion;
  UINT32                  ChipSelect;
  UINT32                  Cbit;
  UINT32                  Bg;
  UINT32                  Ba;
  UINT32                  Row;
  UINT32                  Col;
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  FAILED_DIMM_INFO        PclsFailedDimmInfo;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

  *IsEventHandled = FALSE;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[PCLS]: HandleNewPclsEvent() Start...\n"));

  for (ChOnNode = 0; ChOnNode < MAX_MC_CH; ChOnNode++) {
    if (!IsChannelPresent (NodeId, ChOnNode)) {
      continue;
    }
    RAS_DEBUG ((
      LEVEL_BASIC_FLOW,
      "[PCLS]: Check Node %d, CH %d...\n",
      NodeId,
      ChOnNode
      ));
    Status = GetFailedDeviceInfo (NodeId, ChOnNode, &SparingRegion);
    if (EFI_ERROR (Status)) {
      //
      // No failed device was found on this channel
      //
      continue;
    }
   
   
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
    {
        BOOLEAN   SkipEventHandlers = FALSE;
        UINT8     ErrorEvent = ERROR_EVENT_PCLS;
        Status = GetChannelErrorInfo (NodeId, ChOnNode, FALSE, &PclsFailedDimmInfo);
        if (!EFI_ERROR(Status)) {
		    // Oem Hook To decide the Error action based on the Error data
            OemReadyToTakeAction(ErrorEvent, &PclsFailedDimmInfo, &SkipEventHandlers);
            if (SkipEventHandlers == TRUE) {
                DEBUG((DEBUG_INFO,"OEM request..Skipping PCLS Sparing Event!\n"));
                return;
            }
        }
    }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

    SparingRegion.FailedCol = (SparingRegion.FailedCol >> 2); //Write col[9:2] (8 bits) to PCLS CFG    
    ChipSelect    = SparingRegion.FailedRank;
    Cbit          = SparingRegion.FailedSubRank;
    Bg            = (SparingRegion.FailedBank >> 2);
    Ba            = (SparingRegion.FailedBank & 0x3);
    Row           = SparingRegion.FailedRow;
    Col           = SparingRegion.FailedCol;
    //
    // Check if the failure is single bit and one of 16 PCLS resources is available to perform PCLS
    //
    if ((IsCurrentFailureSingleBit (&SparingRegion) || IsCurrentFailureSingleBitInterLocSyn (&SparingRegion)) && IsPclsResourceAvailable (&SparingRegion)) {
      // Check if there are any ADDDC region on this channel.  If so, do not take PCLS action.
      if (CheckForAdddcOnCh (NodeId, ChOnNode) == TRUE) {
        // ADDDC region was already detected previously.  Do not handle it.
        Status = EFI_SUCCESS;
        continue;
      }
      if (!IsDuplicatePcls (NodeId, ChOnNode, ChipSelect, Cbit, Bg, Ba, Row, Col)) {
        if (TakePclsAction (&SparingRegion)) {
          if (IsSiliconWorkaroundEnabled ("S1409872481")) {
            SetM2mDefeatures0DiscritChunk (NODE_TO_SKT (NodeId), NODE_TO_MC (NodeId), 0x1);
          }
          *IsEventHandled = TRUE;
          RAS_DEBUG ((
                LEVEL_BASIC_FLOW,
                "[PCLS]: PCLS action is successful on Node %d, CH %d.\n",
                NodeId,
                ChOnNode
                ));
        }
      } else {
        RAS_DEBUG ((LEVEL_BASIC_FLOW,"[PCLS]: Duplicate PCLS entry, skip it.\n"));
      }
    }
  }

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  OemReportStatusCode(PclsFailedDimmInfo, PCLS_EVENT);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

  RAS_DEBUG ((
    LEVEL_BASIC_FLOW,
    "[PCLS]: HandleNewPclsEvent() return\n"
    ));
  return;
}
