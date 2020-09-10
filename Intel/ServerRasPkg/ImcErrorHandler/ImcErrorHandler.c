/** @file
  Implementation of RAS imc/m2m error handler driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#include <Library/PlatPolicyLib.h>
#include <Library/RasDebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/ImcRoutingLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/PclsLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/CrystalRidgeLib.h>
#include <Library/MirrorFailoverLib.h>
#include <SiliconSettingRAS.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/PerformanceLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/CpuAndRevisionLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#include <Library/AmiOemRasLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

#define  PATROL_SCRUB_FEATURE_EN    BIT0
#define  RANK_SPARE_FEATURE_EN      BIT1
#define  ADDDC_FEATURE_EN           BIT2
#define  SDDC_PLUS_FEATURE_EN       BIT3
#define  MIRROR_FAILOVER_FEATURE_EN BIT4
#define  FNV_FEATURE_EN             BIT5
#define  PCLS_FEATURE_EN            BIT6
#define  ADDDC_ERROR_INJ_FEATURE_EN BIT7

extern BOOLEAN                   mIs14nmCpu;

/**
  The callback function to handle subsequent SMIs.  This callback will be called by SmmCoreDispatcher.

  @param[in] DispatchHandle             Not used
  @param[in] DispatchContext            Not used
  @param[in, out] CommBuffer            Not used
  @param[in, out] CommBufferSize        Not used

  @retval EFI_SUCCESS                   Function successfully completed
**/
EFI_STATUS
EFIAPI
ImcErrorHandler (
  IN EFI_HANDLE             DispatchHandle,
  IN CONST VOID            *DispatchContext,
  IN OUT VOID              *CommBuffer,
  IN OUT UINTN             *CommBufferSize
  )
{
  UINT8                     Socket;
  RAS_POLICY               *PolicyData;
  UINT32                    NewErrorBitmap;
  UINT32                    PendingErrorBitmap;
  MEM_TOPOLOGY             *MemTopology;
  IMC_RAS_STRUCTURE        *ImcRas;

  PolicyData  = GetRasPolicy ();
  MemTopology = GetMemTopology ();
  ImcRas      = GetImcRasData ();

  ImcRas->PatrolErrorDowngradeEn = PolicyData->ErrorControlPolicy.PatrolErrorDowngradeEn;
  ImcRas->DdrtInternalAlertEn = PolicyData->ImcPolicy.DdrtInternalAlertEn;
  ImcRas->ReportAlertSPA = PolicyData->ImcPolicy.ReportAlertSPA;
  ImcRas->DcpmmUncPoison = PolicyData->ImcPolicy.DcpmmUncPoison;
  ImcRas->RankVlsAlways  = PolicyData->ImcPolicy.RankVlsAlways;
  ImcRas->TriggerSWErrThEn = PolicyData->ImcPolicy.TriggerSWErrThEn;
  ImcRas->SpareSwErrTh = PolicyData->ImcPolicy.SpareSwErrTh;
  ImcRas->TimeWindow = PolicyData->ImcPolicy.TimeWindow;
  ImcRas->CloakingEn = PolicyData->McaPolicy.CloakingEn;
  ImcRas->SpareErrTh = PolicyData->ImcPolicy.SpareErrTh;

  PERF_START (NULL, "ImcErrorHandler", "RAS", 0);

  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (!IsSocketPresent(Socket)) {
      continue;
    }

    if (IsUboxImcError (Socket) || IsUboxM2MemError (Socket)) {
      //
      //Patrol scrub.
      //
      if ((PcdGet32 (PcdImcFeatureList) & (PATROL_SCRUB_FEATURE_EN | RANK_SPARE_FEATURE_EN)) == (PATROL_SCRUB_FEATURE_EN | RANK_SPARE_FEATURE_EN)) {
        if (IsPatrolEnabled ()  && (MemTopology->RasModesEnabled & RK_SPARE)) {
          CheckAndHandlePatrolEvent (Socket);
        }
      }

      //
      //iMc new & pending correctable error
      //
      NewErrorBitmap = 0;
      PendingErrorBitmap = 0;
      if (GetNewAndPendingError (Socket, &NewErrorBitmap, &PendingErrorBitmap)) {
        CorrectableMemoryErrorHandler (Socket, &NewErrorBitmap, &PendingErrorBitmap, PolicyData->ImcPolicy.MemCeFloodPolicy);
      }

      //
      // MirrorFailover handler
      //
      if ((PcdGet32 (PcdImcFeatureList) & ( MIRROR_FAILOVER_FEATURE_EN)) != 0) {
        if (MemTopology->RasModesEnabled & (FULL_MIRROR_1LM | FULL_MIRROR_2LM | PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM)) {
          MirrorFailoverHandler (Socket, PolicyData->McaPolicy.EmcaCsmiEn);
        }
      }

      ClearImcInterruptInUbox (Socket);
    }
  }

  //
  //CR error
  //
  if ((PcdGet32 (PcdImcFeatureList) & ( FNV_FEATURE_EN)) != 0) {
    if (PolicyData->ImcPolicy.FnvErrorEn &&
        ((PolicyData->ImcPolicy.FnvErrorHighPrioritySignal == SMI_FLAG) ||
        (PolicyData->ImcPolicy.FnvErrorLowPrioritySignal == SMI_FLAG))) {
      DdrtErrorHandler ();
    }
  }

  PERF_END (NULL, "ImcErrorHandler", "RAS", 0);
  return EFI_SUCCESS;
}

/**
  FNV error log callback for Crystal Ridge ready.

  Enable FNV error log per policy for each socket.
**/
VOID
EFIAPI
FnvErrorLogCrystalRidgeCallback (
  VOID
  )
{
  IMC_RAS_STRUCTURE *ImcRasData;
  RAS_POLICY        *PolicyData;
  UINT8             Socket;
  UINT8             Channel;
  UINT8             Dimm;
  BOOLEAN           DdrtErrorsEnabled = FALSE;
  UINT32            DebugLevel;

  PolicyData = GetRasPolicy ();
  ImcRasData = GetImcRasData ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    if (!IsSocketPresent (Socket)) {
      continue;
    }

    DdrtEnableError (
      Socket,
      PolicyData->ImcPolicy.NgnAddressRangeScrub,
      PolicyData->ImcPolicy.FnvErrorHighPrioritySignal,
      PolicyData->ImcPolicy.FnvErrorLowPrioritySignal,
      PolicyData->ImcPolicy.NgnHostAlertDpa,
      PolicyData->ImcPolicy.NgnHostAlertPatrolScrubUNC,
      &DdrtErrorsEnabled
      );
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        ImcRasData->DimmInfoStatus[Socket][Channel][Dimm] = GetIdentifyDimm (Socket, Channel, Dimm, &ImcRasData->DimmInfo[Socket][Channel][Dimm]);
      }
    }
  }

  DebugLevel = GetDebugPrintErrorLevel ();
  ImcRasData->GetErrorLogTimeoutCounter = (DebugLevel & (DEBUG_INFO | LEVEL_REG)) ? TEN_SECOND_TIME_IN_NANOSEC : ONE_SECOND_TIME_IN_NANOSEC;

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  OemDdrtErrEnableDisableReporting (PolicyData, &DdrtErrorsEnabled);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

  if (DdrtErrorsEnabled) {
    RAS_DEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "Error reporting has been enabled\n"));
    FnvNotifyHostAlertConfiguration (PolicyData->ImcPolicy.FnvErrorHighPrioritySignal == SMI_FLAG,
                                     PolicyData->ImcPolicy.FnvErrorLowPrioritySignal  == SMI_FLAG);
  }
  else {
    RAS_DEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "Error reporting not enabled, no DDRT DIMMs\n"));
  }

} // FnvErrorLogCrystalRidgeCallback

/**
  This function will enable Patrol scrub on the Imc Chs

  @param[in] Protocol        Protocol unique ID.
  @param[in] Interface       Interface instance.
  @param[in] Handle          The handle on which the interface is installed..

  @retval    Status.
**/
EFI_STATUS
EFIAPI
EnablePatrolScrubatEndofPostCallback (
  IN      CONST EFI_GUID   *Protocol,
  IN      VOID             *Interface,
  IN      EFI_HANDLE        Handle
  )
{

  UINT8            Socket;
  UINT8            Mc;
  UINT8            ChIndex;
  UINT8            SktCh;
  UINT8            Dimm;
  MEM_TOPOLOGY     *MemTopology;

  SktCh = 0;
  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "EnablePatrolScrubatEndofPostCallback : GetMemTopology failed \n"));
    return EFI_NOT_FOUND;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "EnablePatrolScrubatEndofPostCallback Entry \n"));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++){
    for (Mc = 0; Mc < MAX_IMC; Mc++) {
      if (MemTopology->Socket[Socket].imcEnabled[Mc] && MemTopology->Socket[Socket].PatrolScrubInterval[Mc]) {
        if (mIs14nmCpu) {
          SetScrubEnable (Socket, Mc, SktCh, 1);
        } else {
          //
          //For 10nm CPU , enable it on each channel.
          //
          for (ChIndex = 0; ChIndex < MAX_MC_CH; ChIndex++) {
            SktCh = NODECH_TO_SKTCH(Mc, ChIndex);
            if (MemTopology->Socket[Socket].ChannelInfo[SktCh].Enabled) {
              for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
                if ((MemTopology->Socket[Socket].ChannelInfo[SktCh].DimmInfo[Dimm].Present == 0) ||
                    (MemTopology->Socket[Socket].ChannelInfo[SktCh].DimmInfo[Dimm].Enabled == 0) ||
                    (MemTopology->Socket[Socket].ChannelInfo[SktCh].DimmInfo[Dimm].AepDimmPresent)) {
                  continue;
                }
                SetScrubEnable (Socket, Mc, SktCh, 1);
                break;
              }//for Dimm
            }//if ch enabled
          }//for ChIndex
        }//if mIs14nmCpu
      }//if imc enabled
    } //for Mc
  } //for Socket
  return EFI_SUCCESS;
}

/**
  Entry point for the imc/m2m error handler driver.

  This function initializes install imc/m2m error handler.


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
InitializeImcErrorHandler (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                ImageHandle2;
  UINT8                     Socket;
  RAS_POLICY               *PolicyData;
  MEM_TOPOLOGY             *MemTopology;
  UINT8                     Mc;
  UINT8                     ChOnMc;
  UINT8                     ChOnSkt;
 VOID                       *Registration;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,"[imc] initialization start!\n"));

  MemTopology = GetMemTopology ();
  PolicyData = GetRasPolicy ();

  InitEnhancedDimmErrRecList (PolicyData->ImcPolicy.SpareSwErrTh);

  mIs14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  ImageHandle2 = NULL;
  Status = gSmst->SmiHandlerRegister (
                    ImcErrorHandler,
                    NULL,
                    &ImageHandle2);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG((LEVEL_FUNC_FLOW, "%a, Error Registering ImcErrorHandler, Status = %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (!IsSocketPresent(Socket)) {
      continue;
    }
    ConfigSystemRetryRegister (Socket);

    if ((PolicyData->SystemErrorEn) && (PolicyData->ImcPolicy.CorrMemErrEn)) {
      if (((PcdGet32 (PcdImcFeatureList)) & ADDDC_ERROR_INJ_FEATURE_EN) != 0) {
        RAS_DEBUG ((LEVEL_REG,"[imc] ExRasModesEnabled:%x   AdddcErrInJ:%d \n", MemTopology->ExRasModesEnabled, MemTopology->ExRasModesEnabled & ADDDC_ERR_INJ_EN));
        if (MemTopology->ExRasModesEnabled & ADDDC_ERR_INJ_EN) {
          InitializeAdddcErrInj (Socket);
        }
      }

      if ((PcdGet32 (PcdImcFeatureList) & ( ADDDC_FEATURE_EN)) != 0) {
        RAS_DEBUG ((LEVEL_REG,"[imc] ExRasModesEnabled:%x   Adddc:%d \n", MemTopology->ExRasModesEnabled, MemTopology->ExRasModesEnabled & ADDDC_EN));
        if (MemTopology->ExRasModesEnabled & ADDDC_EN) {
          InitAdddcSparing ();
        }
      }

      if ((PcdGet32 (PcdImcFeatureList) & ( PCLS_FEATURE_EN)) != 0) {
        RAS_DEBUG ((LEVEL_REG,"[imc] ExRasModesEnabled:%x   Pcls:%d \n", MemTopology->ExRasModesEnabled, MemTopology->ExRasModesEnabled & PCLS_EN));
        if (MemTopology->ExRasModesEnabled & PCLS_EN) {
          InitPclsSparing ();
        }
      }

      if ((PcdGet32 (PcdImcFeatureList) & ( RANK_SPARE_FEATURE_EN)) != 0) {
        RAS_DEBUG ((LEVEL_REG,"[imc] RasModesEnabled:%x   spare:%d \n", MemTopology->RasModesEnabled, MemTopology->RasModesEnabled & RK_SPARE));
        if (MemTopology->RasModesEnabled & RK_SPARE) {
          InitializeRankSparing ();
        }
      }

      if ((PcdGet32 (PcdImcFeatureList) & ( SDDC_PLUS_FEATURE_EN)) != 0) {
        RAS_DEBUG ((LEVEL_REG,"[imc] ExRasModesEnabled:%x   SDDC:%d \n", MemTopology->ExRasModesEnabled, MemTopology->ExRasModesEnabled & SDDC_EN));
        if (MemTopology->ExRasModesEnabled & SDDC_EN) {
          InitializeSDDCPlusOne ();
        }
      }

      InitializeIndependentMode ();




      if ((PcdGet32 (PcdImcFeatureList) & ( MIRROR_FAILOVER_FEATURE_EN)) != 0) {
        if ((MemTopology->RasModesEnabled & FULL_MIRROR_1LM) ||
            (MemTopology->RasModesEnabled & FULL_MIRROR_2LM) ||
            (MemTopology->RasModesEnabled & PARTIAL_MIRROR_1LM)  ||
            (MemTopology->RasModesEnabled & PARTIAL_MIRROR_2LM)) {
          //
          //initialize channel link, M2M part has been intialized at MRC side(ERR_CNTR_CTL_M2MEM_MAIN_REG).
          //

          //EnableVmseErr0Error (Socket);
          EnableIntForMirrorFailover (Socket, PolicyData->McaPolicy.EmcaCsmiEn);
        }
      }
      if (PolicyData->ImcPolicy.SaiPolicyGroupWaBiosW) {
        if (IsSiliconWorkaroundEnabled ("S2204603196")) {
          PcuSetSmmSaiPolicyWa (Socket);
        }
      }

      ImcCorrectableErrorEnable (Socket, PolicyData->ImcPolicy.SpareIntSelect);
    }

    //
    // Initialize the values required for DDRT ECC
    //
    DcpmmEccModeInit (Socket);
    if (PolicyData->ImcPolicy.DcpmmEccModeSwitch) {
      RAS_DEBUG ((LEVEL_REG,"[imc] PMemEccModeSwitch Enabled \n"));
      DcpmmEccModeSwitchSetup (Socket);
    }

    if (PolicyData->CloakDevHideRegistersOs) {
      for (Mc = 0; Mc < MAX_IMC; Mc++) {
        if (IsNodePresent (SKTMC_TO_NODE (Socket, Mc)) == FALSE) {
          continue;
        }
        for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
          ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
          if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
            continue;
          }
          ClearOsToCorCntPg (Socket, ChOnSkt);
        }
      }
    }
  }
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  OemImcErrEnableDisableReporting (PolicyData, MemTopology);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

  //
  // If the feature is supported and at least one error priority is enabled,
  // register to enable the FNV error log when the library is ready.
  //
  if ((PcdGet32 (PcdImcFeatureList) & FNV_FEATURE_EN) != 0) {
    if (PolicyData->ImcPolicy.FnvErrorEn &&
        (PolicyData->ImcPolicy.FnvErrorHighPrioritySignal ||
         PolicyData->ImcPolicy.FnvErrorLowPrioritySignal)) {
      RegisterCrystalRidgeReadyCallback (FnvErrorLogCrystalRidgeCallback);
    }
    else {
      RAS_DEBUG ((DEBUG_WARN, RAS_INFO_STR ("DDRT") "DDRT error logging is disabled in BIOS setup\n"));
    }
  }

  if ((MemTopology->ExRasModesEnabled & PTRLSCRB_EN) && (MemTopology->ExRasModesEnabled & PTRLSCRB_EOP_EN)) {
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEdkiiSmmExitBootServicesProtocolGuid,
                      EnablePatrolScrubatEndofPostCallback,
                      &Registration
                      );

    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEdkiiSmmLegacyBootProtocolGuid,
                      EnablePatrolScrubatEndofPostCallback,
                      &Registration
                      );
   }

  InitImcRasS3Save ();
  return EFI_SUCCESS;
}
