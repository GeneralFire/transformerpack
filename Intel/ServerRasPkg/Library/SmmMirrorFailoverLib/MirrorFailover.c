/** @file
  Implementation of Mirror failover library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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
#include <IioUniversalData.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PlatMemErrorDataLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/S3MemDataLib.h>
#include <Library/RasSiliconLib.h>
#include <SiliconSetting.h>
#include <Library/MirrorFailoverLib.h>
#include <RcRegs.h>
#include <Library/ImcRoutingLib.h>
#include <MaxSocket.h>
#include "MirrorFailoverPrivate.h"
#include <Library/SiliconWorkaroundLib.h>
#include <Library/WorkaroundLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/MemFuncLib.h>
#include <Library/RasRuntimeStatusLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#include <Library/AmiOemRasLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

#define LINK_FAIL_STATE 0x09

/**
  Get VMSE error0 log bit.

  Note: corrected error threshold init is done by MRC

  @param[in]  socket    - socket number
  @param[in]  Ch        - Channel ID

  @retval None
**/
BOOLEAN
EFIAPI
GetVmseErr0Sts (
  IN        UINT8   Socket,
  IN        UINT8   Mc
  )
{

  UINT8                                     ChOnSkt;
  UINT8                                     ChOnMc;
  BOOLEAN                                   Flag = FALSE;

  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
    ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }

    if (IsVmseMcaCtlErr0Enabled (Socket, ChOnSkt)) {
      Flag =  TRUE;
      break;
    }
  }
  return Flag;
}

/**
  Is this channel fully mirrored or partially mirrored

  Note: corrected error threshold init is done by MRC

  @param[in]  socket    - Socket number
  @param[in]  Mc        - Mc number
  @param[in]  Ch        - Channel ID
  @param[out] MirrorStatus - Mirror Status FullyMirrored, PartiallyMirroed or No Mirror

  @retval None
**/
VOID
GetChannelMirrorStatus (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  IN  UINT8   ChOnMc,
  OUT CHANNEL_MIRROR_STATUS *MirrorStatus
  )
{
  UINT8 TadIndex;
  UINT8 EnabledTads;
  UINT8 MirroredTads;

  EnabledTads = 0;
  MirroredTads = 0;
  *MirrorStatus = NoMirror;

  for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
    if (IsTadValid (Socket, Mc, NODECHA_TO_SKTCHA (Mc, ChOnMc), TadIndex)) {
      EnabledTads++;
      if (IsTadMirrored (Socket, Mc, NODECHA_TO_SKTCHA (Mc, ChOnMc), TadIndex)) {
        MirroredTads++;
      }  //Tad Mirrored
    } //Tad Valid
  }//Tad loop

  if (EnabledTads > 0) {
    if (EnabledTads == MirroredTads) {
      *MirrorStatus = FullyMirrored;
    } else if ((EnabledTads > MirroredTads) && (MirroredTads > 0)) {
      *MirrorStatus = PartiallyMirrored;
    }
  }

  RAS_DEBUG((LEVEL_BASIC_FLOW, "\t[Mirror] GetChannelMirrorStatus - Ch %d on Socket %d is ", NODECHA_TO_SKTCHA (Mc, ChOnMc), Socket));
  RAS_DEBUG((LEVEL_BASIC_FLOW, (*MirrorStatus == FullyMirrored) ? "FullyMirrored \n" : "Not Fully Mirrored \n"));
  return;
}

/**
  Clears VMSE error0 log bit.

  Note: corrected error threshold init is done by MRC

  @param[in]  socket    - socket number
  @param[in]  Ch        - Channel ID

  @retval None
**/
VOID
EFIAPI
ClearVmseErr0Sts (
  IN        UINT8   Socket,
  IN        UINT8   Mc
  )
{

  UINT8                                     ChOnSkt;
  UINT8                                     ChOnMc;

  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
    ChOnSkt = NODECHA_TO_SKTCHA(Mc, ChOnMc);
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }
    ClearVmseMcaCtlErr0Log (Socket, ChOnSkt);
 }
}

/**
  This function enables the SMI signaling when link goes in link fail state.

  @retval EFI_SUCCESS if the call is succeed.

**/
VOID
EFIAPI
EnableVmseErr0Error (
  UINT8 Socket
  )
{
  UINT8                                     ChOnSkt;

  for (ChOnSkt = 0; ChOnSkt < IMC_MAX_CH; ChOnSkt++) {
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }

    EnableVmseMcaCtlErr0 (Socket, ChOnSkt);
  }
}

/**
  This function enables  SMI generation of for Mirror failover.

  @param[in]  Node      - Node to init

  @retval None

**/
VOID
EnableIntForMirrorFailover (
  IN        UINT8   Socket,
  IN        UINT8   EmcaCsmiEn
  )
{
  UINT8                             Mc;
  //
  //Enable BIOS Assisted Mirror Failover.
  //
  for (Mc = 0; Mc < MAX_IMC; Mc++) {
    if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
      continue;
    }
    if (!GetM2mSysFeatures0ImFailoverEn (Socket, Mc)) {
      SetM2mExRasConfigFailOverAct (Socket, Mc, EmcaCsmiEn, TRUE);
      SetM2mDefeatures1Cfgfailover (Socket, Mc, FALSE);
    }
  }
}


/**
  This function enables  SMI generation of for Mirror failover.

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number
  @param[in]  FailedChBitMap  - One hot encoded value for failed channel

  @retval EFI_SUCCESS  if successful
          EFI_UNSUPPORTED if unsuccessful

**/
EFI_STATUS
SetTadBaseForMirrorFailover (
    UINT8 Socket,
    UINT8 Mc,
    UINT8 FailedChBitMap
    )
{
  UINT8       TadIndex;
  UINT8       FailedCh;
  UINT8       Counter;

  FailedCh = 0;
  if (FailedChBitMap == 0) {
    RAS_DEBUG((LEVEL_REG, "[Mirror] Warning! Mirror failover without channel information \n"));
    return EFI_UNSUPPORTED;
  }

  Counter = FailedChBitMap >> 1;
  while (Counter) {
    FailedCh++;
    Counter = FailedChBitMap >> 1;
    if (FailedCh > 2) {
      return EFI_UNSUPPORTED;
    }
  }
  for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
    UpdateTadBaseForMirrorFailover (Socket, NODECHA_TO_SKTCHA(Mc, FailedCh), FailedCh, TadIndex);
  } // Tad loop

  return EFI_SUCCESS;
}

/**
 * DoHardwareAssistedMirrorFailover : In this method the silicon has already failed over a bad channel.
 * Bios is responsible to remove the ranks of this channel from Amap. Mark the mirrored TADs with
 *  Also log this information!
 * @param[in] UINT8  Socket
 * @param[in] UINT8  Mc
 * @param[out] McFailedCh : the failed channel if successful, 0xff indicates function failed
 * @retval : Return EFI_SUCCESS or EFI_FAILURE
 */
EFI_STATUS
DoHardwareAssistedMirrorFailover (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  OUT UINT8   *McFailedCh
  )
{
  UINT8         FailedChBitmap;
  EFI_STATUS    Status;
  MIRRORFAILOVER_M2MEM_MAIN_STRUCT  MirrorFailover;

  //Initialization
  Status = EFI_UNSUPPORTED;
  *McFailedCh = 0xff;

  MirrorFailover.Data = GetM2mMirrorFailOver (Socket, Mc);
  if (MirrorFailover.Bits.ddr4chnlfailed) {
    FailedChBitmap = (UINT8)MirrorFailover.Bits.ddr4chnlfailed;
    Status = SetTadBaseForMirrorFailover (Socket, Mc, FailedChBitmap);
    *McFailedCh = (UINT8)HighBitSet32 ((UINT32)FailedChBitmap);
  }
  return Status;
}

/**
 * DoBiosAssistedMirrorFailover : In this method the bios is resposible to determine the channel failed from the shadow registers. Bios
 * algorithm reads the value thrice to ensure itegrity and then programs this value into the mirrorfailover register under quiesce with the
 * assistance of pcode mailbox call
 * @param[in] UINT8  Socket
 * @param[in] UINT8  Mc
 * @param[out] McFailedCh : the failed channel if successful, 0xff indicates function failed
 *
 * @retval None
 */
VOID
DoBiosAssistedMirrorFailover (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  OUT UINT8   *McFailedCh
  )
{
  UINT8                               ChFailedBitmap;
  UINT8                               SecondaryCh;
  UINT8                               ChFailedRead[NUM_ATTEMPTS];
  UINT8                               McCmdRegionRead[NUM_ATTEMPTS];
  UINT8                               McCmdVldRead[NUM_ATTEMPTS];
  UINT8                               MirrorFailoverRead[NUM_ATTEMPTS];
  UINT8                               Counter;
  BOOLEAN                             Ddr4Failed;
  CHANNEL_MIRROR_STATUS               ChMirrorStatus;
  FAILED_DIMM_INFO                    FailedDimmInfo;
  DIMM_ERROR_INFO                     ErrorDataInfo;
  EFI_PLATFORM_MEMORY_ERROR_DATA      ErrorDataForReport;
  EFI_STATUS                          Status = EFI_UNSUPPORTED;

  //
  // Initializations
  //
  Ddr4Failed     = FALSE;
  *McFailedCh    = 0xff;

  //
  // Read the values of Ch failed, Mccmdregion,  mirror failover and mccmdvld three times and
  // ensure the value is same for three reads and then initiate a bios assisted failover
  //
  for (Counter=0; Counter < NUM_ATTEMPTS; Counter++) {
    ChFailedRead[Counter]        =  (GetM2mMciStatusShadowN0McaCod (Socket, Mc) & 0x3);
    McCmdRegionRead[Counter]     =  (UINT8) GetM2mMciMiscShadowN1McCmdMemRegion (Socket, Mc);
    MirrorFailoverRead[Counter]  =  (UINT8) GetM2mMciMiscShadowN1MirrFailover (Socket, Mc);
    McCmdVldRead[Counter]        =  (UINT8) GetM2mMciMiscShadowN1McCmdVld (Socket, Mc);
    RAS_DEBUG ((LEVEL_REG, "[Mirror] Read Status attempt:%d - Ch_Fail:%x  Region:%x  Failover:%x CmdVld:%x \n",Counter,
               ChFailedRead[Counter], McCmdRegionRead[Counter], MirrorFailoverRead[Counter], McCmdVldRead[Counter]));
  }

  if (((ChFailedRead[0] == ChFailedRead[1]) && (ChFailedRead[1] == ChFailedRead[2]))  &&
      ((McCmdRegionRead[0] == McCmdRegionRead[1]) && (McCmdRegionRead[1] == McCmdRegionRead[2])) &&
      ((MirrorFailoverRead[0] == MirrorFailoverRead[1]) && (MirrorFailoverRead[1] == MirrorFailoverRead[2])) &&
      ((McCmdVldRead[0] == 1) && (McCmdVldRead[0]  == McCmdVldRead[1]) && (McCmdVldRead[1] == McCmdVldRead[2]))) {

    Ddr4Failed = GetM2mMirrorFailOverDdr4Failed (Socket, Mc);
    ChFailedBitmap = 1 << ChFailedRead[0];

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
    {
        BOOLEAN   SkipEventHandlers = FALSE;
        UINT8     ErrorEvent = ERROR_EVENT_MIRR_FAILOVER;
        FAILED_DIMM_INFO    MirrorFailedDimmInfo;
        
        MirrorFailedDimmInfo.Node = SKTMC_TO_NODE(Socket, Mc);
        MirrorFailedDimmInfo.NodeCh = ChFailedRead[0];
        
		// Oem Hook To decide the Error action based on the Error data
        OemReadyToTakeAction(ErrorEvent, &MirrorFailedDimmInfo, &SkipEventHandlers);
        if (SkipEventHandlers == TRUE) {
        	DEBUG((DEBUG_INFO,"OEM request..Skipping Mirror Failover Event!\n"));
            return;
        }
    }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

    if (McCmdRegionRead[0] >> 2 == 0) { //Error on DDR4
      if (!Ddr4Failed) { // DDR4 channel not already failed over on this MC
        RAS_DEBUG((LEVEL_BASIC_FLOW, "\t[Mirror] Mirror Failover on DDR4 \n"));

        PatrolScrubInfoSaveOrRestore (SKTMC_TO_NODE(Socket, Mc), PATROL_SAVE);

        if (Ddr4FailoverMailbox (Socket, Mc, ChFailedBitmap) == 0) { //B2P Successful
          Ddr4Failed = GetM2mMirrorFailOverDdr4Failed (Socket, Mc);
          RAS_DEBUG((LEVEL_BASIC_FLOW, "\t[Mirror] Mirror Failover mailbox call successful\n"));
          RAS_DEBUG((LEVEL_REG,        "\t[Mirror] Mfo.Bits.ddr4chnlfailed:%d\n",Ddr4Failed));
          //
          // Disable link fail on secondary channel after failover
          //
          SecondaryCh = (UINT8)GetM2mMirrorChDdr4 (Socket, Mc, ChFailedRead[0]);
          if (SecondaryCh != 0xF) {
            DisableLinkFail (Socket, NODECHA_TO_SKTCHA(Mc, SecondaryCh));
          }
          *McFailedCh = ChFailedRead[0];
        } //B2P Successful
        GetChannelMirrorStatus (Socket, Mc, ChFailedRead[0], &ChMirrorStatus);
        if (ChMirrorStatus == FullyMirrored) {
          SetFullMirrorChFailed (SKTMC_TO_NODE(Socket, Mc), ChFailedRead[0]);
        }
        PatrolScrubInfoSaveOrRestore (SKTMC_TO_NODE(Socket, Mc), PATROL_RESTORE);
      }
    }

    ErrorDataInfo.Socket = Socket;
    ErrorDataInfo.SktCh = NODECHA_TO_SKTCHA(Mc, ChFailedRead[0]);

    Status = GetChannelErrorInfo (Mc, ChFailedRead[0], (UINT64)-1, &FailedDimmInfo);
    if (!EFI_ERROR(Status)) {
      ErrorDataInfo.Rank = FailedDimmInfo.FailedRank;
      ErrorDataInfo.Dimm = FailedDimmInfo.FailedDimm;

     } else {
       RAS_DEBUG ((LEVEL_REG, "[Mirror] Error! get retry failed! Status:%r \n", Status));
       ErrorDataInfo.Rank = 0;
       ErrorDataInfo.Dimm = 0;
     }

     FillCorrectablMemoryErrorData (MEM_MIRROR_FAILOVER, &ErrorDataInfo, &ErrorDataForReport);
     ReportCorrectedMemErrorLog (&ErrorDataForReport);
  }
}

/**
 * MirrorFailoverHandler : Handle a mirror scrub fail event on a channel
 * @param[in]   UINT8   Socket
 * @param[in]   UINT8   EmcaCsmiEn
 * @param[out]  EFI_STATUS MirrorFailoverStatus : Status EFI_SUCCESS if successful
 */
EFI_STATUS
MirrorFailoverHandler (
    IN        UINT8   Socket,
    IN        UINT8   EmcaCsmiEn
    )
{
  EFI_STATUS      Status;
  UINT8           Mc;
  UINT8           MirrorFailover;
  UINT8           McFailedCh;

  Status = EFI_UNSUPPORTED;

  for (Mc = 0; Mc < MAX_IMC; Mc++) {
    McFailedCh = 0xff;
    if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
      continue;
    }
    //
    // Setup registers for hardware based immediate failover event if the immediate failover bit was enabled
    // and this is a DDR4 failover event. DDRT failover event is always BIOS assisted
    //
    if ( (GetM2mSysFeatures0ImFailoverEn (Socket, Mc)) && (GetM2mMirrorFailOverDdr4Failed (Socket, Mc))){
      Status = DoHardwareAssistedMirrorFailover (Socket, Mc, &McFailedCh);
    } else {  // Hardware based immediate failover not enabled/supported
      if (GetM2mMciStatusShadowN1Valid(Socket, Mc) && GetM2mMciStatusShadowN1AddValid (Socket, Mc)) {
        MirrorFailover    =  (UINT8) GetM2mMciMiscShadowN1MirrFailover (Socket, Mc);
        if (MirrorFailover) {
          DoBiosAssistedMirrorFailover (Socket, Mc, &McFailedCh);
        } // mcistatusshadow valid
      }
    } // do bios assisted failover
    if (McFailedCh != 0xff) { // failover operation succesful
      if (EmcaCsmiEn == 0) {
        SignalCmciToOs (GetM2MemMcBankIndex(Mc) ,Socket, ALL_CORE, NULL);
      }
      SaveMirrorFailoverFailedChannel(Socket, Mc, McFailedCh);
    }

    //
    // Workaround for TADdowngrade
    //
    if (IsSiliconWorkaroundEnabled ("S1506332738")) {
      Status = TadRestorePsSeverity (Socket, Mc);
    }

  } // Mc loop
  return Status;
}

