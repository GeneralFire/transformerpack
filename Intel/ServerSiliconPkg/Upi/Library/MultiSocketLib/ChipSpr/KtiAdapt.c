/** @file
  This file includes the related functions for UPI TXEQ adaptation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <KtiMisc.h>
#include <Upi/KtiSi.h>
#include <KtiSetupDefinitions.h>
#include <Library/KtiApi.h>
#include <Guid/UboxIpInterface.h>
#include <Guid/KtiIpInterface.h>
#include <Library/UpiHostDataLib.h>
#include <Library/MultiSocketLib.h>
#include <UncoreCommonIncludes.h>
#include <Library/UncoreLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include "MultiSocketInternal.h"
#include "KtiAdapt.h"

//
// It is the draft version of coefficent for PH_TCL (as eparam) or PH_TCA (for txeq adapation).
// Open an issue (s1608833163) to track it.
// For the actual platform, hardware will provide the values accordingly.
//
UINT32    LANE_TX_COEFF_ARRAY[MAX_SET_OF_PRESET_COEFF] = {0xc900, 0x8a00, 0xa980, 0x6a80, 0xc00, 0xb04, 0xa86, 0xa884, 0x6906, 0xa08, 0x10800};
//
// A flag to indicate if the txeq adaptation has been started, if it is TRUE, skip the fixed initial programmings on the links for adaptation;
// otherwise, implement the initial programming for the links.
//
BOOLEAN   AdaptationStartedFlag = FALSE;

/**
  This routine is used to update the Adaptation Status to KTI_HOST_INTERNAL_GLOBAL.
  Then update the status of the current adapatation.

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval TRUE:  Current speed is adapted
          FALSE: Still in the speed adapting

**/
BOOLEAN
CheckIfCurrentSpeedAdapted (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  if (KtiInternalGlobal->KtiAdaptTcaIndex < MAX_SET_OF_PRESET_COEFF) {
    //
    // The current speed's adaptation doesn't finished, move to next tca coefficient
    //
    KtiInternalGlobal->KtiAdaptTcaIndex++;
    return FALSE;
  } else {
    //
    // The current speed's adaptation finished, update the adaptation status and move the tca coefficient
    // index to 0.
    //
    ZeroMem (KtiInternalGlobal->KtiAdaptFomArray, sizeof (KtiInternalGlobal->KtiAdaptFomArray));
    KtiInternalGlobal->KtiAdaptTcaIndex = 0;
    KTIVAR->KtiAdaptationTable[KtiInternalGlobal->CurrentSpeedAdapted].TableValid = TRUE;
    KTIVAR->KtiAdaptationTable[KtiInternalGlobal->CurrentSpeedAdapted].Speed = KtiInternalGlobal->CurrentSpeedAdapted;
    KtiInternalGlobal->AdaptSpeedsCompleted |= (1 << KtiInternalGlobal->CurrentSpeedAdapted);
    return TRUE;
  }
}

/**
  Get the Txeq adaptation parameters for each lane of the link from the previous adapted speed.
  If it is in the txeq adaptation process, try to get the latest TCL_LANE parameters based on the FOM,
  then update the output txeq adaptation table.

  Otherwise, don't touch the output txeq adaptation table, there may be existed valid txeq adaptation table
  stored in the SPI.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - SocId we are looking up
  @param LinkIndex         - Link# on socket

  @retval Status - return KTI_SUCCESS on successful programming

**/
KTI_STATUS
KtiGetTxeqAdaptationResult (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      LinkIndex
  )
{
  UINT8          CurrentFOM;
  UINT8          LaneIndex;;
  UINT8          FomArrayIndex;

  FomArrayIndex = Socket * SI_MAX_KTI_PORTS_SPR * MAX_LANES_PER_LINK_SPR + \
                  LinkIndex * MAX_LANES_PER_LINK_SPR;

  //
  // Check and get the FOMs only in the adapatation process && skip the first round tca, as the FOM is not ready
  // at that time.
  //
  if (KtiInternalGlobal->KtiAdaptTcaIndex > 0) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nSocket %d, Link %d\n", Socket, LinkIndex));
    //
    // Adaptation is in progress, try to get the latest TCL_LANE parameters based on the FOM,
    // then update the output txeq adaptation table.
    //
    for (LaneIndex = 0; LaneIndex < MAX_LANES_PER_LINK_SPR; LaneIndex++) {
      if (!GetKtiPhFomLane (Socket, LinkIndex, LaneIndex, &CurrentFOM)) {
        KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  The FOM of S%u Link%u Lane%u is not valid!", Socket, LinkIndex, LaneIndex));
        continue;
      }

      if (KtiInternalGlobal->KtiAdaptFomArray[FomArrayIndex + LaneIndex] < CurrentFOM) {
        //
        // Check if the LANE's FOM is greater than before
        // if yes, update the Txeq_FOM structure, and the adpatation table's tcl index.
        //
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Lane %d's Current FOM %x, previous largest FOM: %x\n",
                           LaneIndex, CurrentFOM, KtiInternalGlobal->KtiAdaptFomArray[FomArrayIndex + LaneIndex]));
        KtiInternalGlobal->KtiAdaptFomArray[FomArrayIndex + LaneIndex] = CurrentFOM;
        KTIVAR->KtiAdaptationTable[KtiInternalGlobal->CurrentSpeedAdapted].AdaptedEQs[Socket][LinkIndex].TcaCoeffIndex[LaneIndex] = (KtiInternalGlobal->KtiAdaptTcaIndex - 1);
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  This routine identifies the next speed, if any, to be adapted.
  When adaptation is already in progress. It also updates the Adaptation status scratch register with the outcome.

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Status - return KTI_SUCCESS on successful programming

**/
KTI_STATUS
IdentifyNextSpeedToAdapt (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8     AdaptSpeed;

  //
  // Check the adaptation status for the current adaptation speed.
  //
  if (!CheckIfCurrentSpeedAdapted (KtiInternalGlobal)) {
    return KTI_SUCCESS;
  }
  //
  // Check if it needs to adapt for all supported speed, identify the next speed.
  //
  if (KTISETUP->KtiAdaptationSpeed == MAX_KTI_LINK_SPEED) {
    KtiInternalGlobal->KtiRunAdaptation = FALSE;
    KtiInternalGlobal->CurrentSpeedAdapted = MAX_KTI_LINK_SPEED;
    for (AdaptSpeed = SPEED_REC_128GT; AdaptSpeed <= SPEED_REC_160GT; AdaptSpeed++) {
      if ((KtiInternalGlobal->AdaptSpeedsCompleted & (1 << AdaptSpeed)) == 0) {
        if (CheckLinkSpeedSupported (AdaptSpeed) == KTI_SUCCESS) {
          KtiInternalGlobal->KtiRunAdaptation = TRUE;
          KtiInternalGlobal->CurrentSpeedAdapted = AdaptSpeed;
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nJump to next speed %a adaptation\n", ((KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_128GT) ? "128GT" :
                                                                                         (KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_144GT) ? "144GT" :
                                                                                         (KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_160GT) ? "160GT" : "UNKNOWN")));
          break;
        }
      }
    }
  } else {
    if ((KtiInternalGlobal->AdaptSpeedsCompleted & (1 << KtiInternalGlobal->CurrentSpeedAdapted)) != 0) {
      KtiInternalGlobal->KtiRunAdaptation = FALSE;
      KtiInternalGlobal->CurrentSpeedAdapted = MAX_KTI_LINK_SPEED;
    } else {
      KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_ADAPTATION_FAILED);
      KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  Kti Adaptation is in progress, or Speed %a adaptation is failed!",
                                          ((KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_128GT) ? "128GT" :
                                           (KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_144GT) ? "144GT" :
                                           (KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_160GT) ? "160GT" : "UNKNOWN")));
    }
  }

  return KTI_SUCCESS;
}

/*++
  Routine Description:
  Checks if valid adaptation table is found for the platform. It also obtains the table from the OEM hook and stores
  it in QPI internal global structure to be used to override the EQ values during full speed switching.

  TRUE - Valid table found
  FALSE - Valid table not found

--*/
BOOLEAN
CheckForValidAdaptationTable (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN         Status;
  OEM_GET_ADAPTED_EQ_SETTINGS  OemGetAdaptedEqSettingsPtr;

  OemGetAdaptedEqSettingsPtr = (OEM_GET_ADAPTED_EQ_SETTINGS) (UINTN) (KTI_HOST_IN_PTR->OemGetAdaptedEqSettings);
  if (OemGetAdaptedEqSettingsPtr == NULL) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCan't get any valid TXEQ adapation table!\n"));
    return FALSE;
  }

  Status = OemGetAdaptedEqSettingsPtr (KTIVAR->KtiAdaptationTable);

  if (Status == TRUE) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Valid Adaptation Table is found!\n"));
  } else {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Valid Adaptation Table is missing!\n"));
  }

  return Status;
}

/**
  Programs txeq adaptation registers to enter the adaptatin flow

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - SocId we are looking up
  @param LinkIndex         - Link# on socket

**/
KTI_STATUS
ProgramAdaptationRegisters (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      LinkIndex
  )
{
  UINT8     ExpLoopCnt;
  //
  // Step.1 check SpUseBackChannel, if it is 1, clear it by writing 1 for adaptation
  //
  if (GetKtiSpUseBackChannel (Socket, LinkIndex) == KTI_BACK_CHANNEL_IN_USE) {
    SetKtiSpUseBackChannel (Socket, LinkIndex, 1);
  }
  //
  // Step.2 program txeq coeff to PH_TCA for txeq adaptation
  //
  SetKtiPhTca (Socket, LinkIndex, LANE_TX_COEFF_ARRAY[KtiInternalGlobal->KtiAdaptTcaIndex]);
  //
  // Step.3 Set TxAdaptationEn Bit
  //
  SetKtiTxAdaptEn (Socket, LinkIndex, KTI_TX_ADAPT_ENABLE);
  //
  // Step.4 Set Start Test for adaptation & exploopcnt (which is tuned to avoid the invalid FOM)
  //
  ExpLoopCnt = 0x12;
  SetKtiPhTtpgc (Socket, LinkIndex, &ExpLoopCnt);
  //
  // Step.5 set the c_init to CLocalLBM
  //
  SetKtiCInit (Socket, LinkIndex, C_INIT_TO_CLocalLBM);

  if (!AdaptationStartedFlag) {
    //
    // Belows are the fixed initial programmings for all links
    // Step.6, set the Initial Check Mask to 0xf (got from SV)
    //         extend the DET_TX secondary timer of slow mode to 0xf.
    //
    SetKtiPhTtpcc (Socket, LinkIndex, 0xf);
    SetKtiPhTsecSl (Socket, LinkIndex, 0xf);
    AdaptationStartedFlag = TRUE;
  }

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_FULL_SPEED_TRANSITION, MINOR_STS_TXEQ_ADAPTATION_START);
  return KTI_SUCCESS;
}

/**
  Programs txeq adaptation parameter for the link.
  The default value will be programmed to PH_TCC. If there are adaptation result for Lanes can be used.
  No matter the Lane adaptation result is come from the saved txeq table or the previous adaptation, program
  it to PH_TCL_LANEx.

  If there is no valid adaptation result, programs the txeq adaptation register to enter the adaptation
  if the adaptation is required.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - SocId we are looking up
  @param LinkIndex         - Link# on socket

  @retval Status - return KTI_SUCCESS on successful programming
                          KTI_IGNORE - Not supported

**/
KTI_STATUS
KtiProgramTxeqAdaptParams (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  UINT8                      LinkIndex
  )
{
  UINT8          LaneIndex, LinkSpeed;
  UINT32         TclCoeff;

  //
  // Get the Soc X Link Y's target speed
  //
  LinkSpeed = (UINT8) KtiInternalGlobal->PhyLinkPerPortSetting[Socket].Phy[LinkIndex].KtiLinkSpeed;

  if (KTIVAR->KtiAdaptationTable[LinkSpeed].TableValid == TRUE) {
    //
    // Clear TxAdaptEn Bit for each enabled link after the adaptation is finished.
    //
    SetKtiTxAdaptEn (Socket, LinkIndex, KTI_TX_ADAPT_DISABLE);
    //
    // Get the TXEQ adaptation table if the table is valid, and program the PH_TCL_LANE[x]
    // It has the higher priority than PH_TCC
    //
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nSocket %d, Link %d:", Socket, LinkIndex));
    for (LaneIndex = 0; LaneIndex < MAX_LANES_PER_LINK_SPR; LaneIndex++) {
      TclCoeff = LANE_TX_COEFF_ARRAY[KTIVAR->KtiAdaptationTable[LinkSpeed].AdaptedEQs[Socket][LinkIndex].TcaCoeffIndex[LaneIndex]];
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n       Lane %d, TclCoeff: %x, TcaIndex: %x",
                           LaneIndex, TclCoeff,
                           KTIVAR->KtiAdaptationTable[LinkSpeed].AdaptedEQs[Socket][LinkIndex].TcaCoeffIndex[LaneIndex]));
      SetKtiPhTclLane (Socket,
                       LinkIndex,
                       LaneIndex,
                       TclCoeff
                      );
    }
  } else {
    if (KtiInternalGlobal->KtiRunAdaptation == TRUE) {
      //
      // Get FOM result for the LINK and update the tcl index & txeq_fom structure.
      //
      KtiGetTxeqAdaptationResult (KtiInternalGlobal, Socket, LinkIndex);
      //
      // Programming the related register to enter the adaptation flow if there is any tca left
      //
      ProgramAdaptationRegisters (KtiInternalGlobal, Socket, LinkIndex);
    }
  }

  return KTI_SUCCESS;
}

/**
  The routine is used to select the target upi speed, and do the phy layer programming.
  Then trigger the phy/link layer reset and put the link to the GV flow as the new speed transition flow.
  It is used for adaptation flow, and link layer needn't to be programmed as it already be programmed in the
  normal flow.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Status - return KTI_SUCCESS on successful programming

**/
VOID
ProgramPhyLayerAndSwitchToTargetSpeed (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  //
  // select the target speed
  // If in the adaptation, it will use the current adaptation speed.
  // If the adaptation is finished, it will use the KTIVAR->OutKtiLinkSpeed as general
  //
  SelectSupportedKtiLinkSpeed (SocketData, KtiInternalGlobal);
  //
  // Normalize the phy layer options
  //
  NormalizeLinkOptions (SocketData, KtiInternalGlobal);
  //
  // do the phy layer programming for the new target speed
  //
  ProgramPhyLayer (KtiInternalGlobal, SocketData, KTI_GROUP);
  //
  // Implement the new flow for speed transition, file phy/link layer reset, trigger GV flow.
  //
  KtiSpeedTransitionMiscSteps (SocketData, KtiInternalGlobal, KTI_LINK_FULL_SPEED_MODE);

  return;
}

/**
  Check if there is next adaptation speed need to be adapted, or it is already finished.
  If it is finished, clears the TxAdaptEn bits for links.
  Otherwise, get the next adaptation speed and do the related steps (it is in without warm reset flow).

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Status - return KTI_SUCCESS on successful programming

**/
KTI_STATUS
CheckAndExecuteKtiAdaptation (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8      Socket, LinkIndex;

  while (TRUE) {
    //
    // Check if there is next adaptation speed need to be adapted, or it is already finished
    // If KtiInternalGlobal->CurrentSpeedAdapted = MAX_KTI_LINK_SPEED, means the adapatation is finished.
    //
    IdentifyNextSpeedToAdapt (KtiInternalGlobal);
    if (KtiInternalGlobal->CurrentSpeedAdapted == MAX_KTI_LINK_SPEED) {
      //
      // Set the target link speed with the setup one
      //
      KTIVAR->OutKtiLinkSpeed = KTISETUP->KtiLinkSpeed;
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nThe TXEQ adaptation is finished! Switch to the target speed with the adapted coefficient. \n"));
      ProgramPhyLayerAndSwitchToTargetSpeed (SocketData, KtiInternalGlobal);
      break;
    }

    if (KtiInternalGlobal->KtiAdaptTcaIndex == 0) {
      //
      // Adaptation will go on on the Next Speed, reprogram the Phy layer
      //
      ProgramPhyLayerAndSwitchToTargetSpeed (SocketData, KtiInternalGlobal);
    } else {
      //
      // Trigger the GV flow on each socket to slow mode from fast loop back mode.
      //
      KtiSpeedTransitionMiscSteps (SocketData, KtiInternalGlobal, KTI_LINK_SLOW_SPEED_MODE);
      //
      // if goes to the end of the PRESET_COEFF table, skip the fast loopback switch.
      //
      if (KtiInternalGlobal->KtiAdaptTcaIndex >= MAX_SET_OF_PRESET_COEFF) {
        continue;
      }
      //
      // Adapt for the next TCA coefficient
      //
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if (SocketPresent (Socket)) {
          for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
            if (KTIVAR->CpuInfo[Socket].LepInfo[LinkIndex].Valid == TRUE) {
              //
              // Only reprogram the Txeq adapation parameters (the coeff in the TCA array)
              // for all the valid links
              //
              KtiProgramTxeqAdaptParams (KtiInternalGlobal, Socket, LinkIndex);
            }
          }
        }
      }
      //
      // Trigger the GV flow on each socket to fast speed loop back mode.
      //
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nMove to next TCA coefficient, Index: %x\n", KtiInternalGlobal->KtiAdaptTcaIndex));
      KtiSpeedTransitionMiscSteps (SocketData, KtiInternalGlobal, KTI_LINK_FULL_SPEED_MODE);
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine checks if adaptation needs to be executed or not. It also updates the Adaptation status scratch register
  with the outcome. Executing the Adaptation engine is necessary only if all the populated sockets support
  adaptation and at least one of the following is TRUE:

      - CPU parts on the platform are changed/swapped
      - A valid adaptation table is not found
      - A valid adaptation table is found but the table entry for the requested adaptation speed is not found

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval BOOLEAN - TRUE  - if adaptation needed; FALSE - if not adaptation required

**/
BOOLEAN
CheckIfAdaptationNeeded (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN                                 TableValid, PartChanged;
  UINT8                                   AdaptationAction;
  UINT8                                   MaxSupportedLinkSpeed, Idx;
  OEM_CHECK_CPU_PARTS_CHANGE_SWAP         OemCheckCpuPartsChangeSwapPtr;

  AdaptationAction = ADAPTATION_NOT_REQUIRED;
  PartChanged = FALSE;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nKTIVAR->KtiAdaptationTable = %x, KtiAdaptFomArray = %x\n", KTIVAR->KtiAdaptationTable, KtiInternalGlobal->KtiAdaptFomArray));

  //
  // Check if the CPU part is changed or swapped.
  //
  OemCheckCpuPartsChangeSwapPtr = (OEM_CHECK_CPU_PARTS_CHANGE_SWAP) (UINTN) (KTI_HOST_IN_PTR->OemCheckCpuPartsChangeSwap);
  if (OemCheckCpuPartsChangeSwapPtr != NULL) {
    PartChanged = OemCheckCpuPartsChangeSwapPtr ();
  }
  if (PartChanged) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "CPU Parts have been Changed/Swapped\n"));
    TableValid = FALSE;
  } else {
    //
    // Part hasn't been changed/swapped, check if there is a valid txeq adaptation table.
    //
    TableValid = CheckForValidAdaptationTable (KtiInternalGlobal);
  }

  if (!TableValid) {
    //
    // No table is valid
    //
    ZeroMem (KTIVAR->KtiAdaptationTable, sizeof (KTIVAR->KtiAdaptationTable));
    if (KTISETUP->KtiAdaptationSpeed != MAX_KTI_LINK_SPEED) {
      //
      // Do the adaptation for the specified speed --- KTISETUP->KtiAdaptationSpeed
      //
      if (CheckLinkSpeedSupported (KTISETUP->KtiAdaptationSpeed) == KTI_SUCCESS) {
        KtiInternalGlobal->KtiRunAdaptation = TRUE;
        KtiInternalGlobal->CurrentSpeedAdapted = KTISETUP->KtiAdaptationSpeed;
        AdaptationAction = ADAPTATION_REQUIRED;
      } else {
        AdaptationAction = ADAPTATION_REQUIRED_SPEED_NOT_SUPPORTED;
      }
    } else {
      //
      // Do the adaptation for all the supported speeds
      //
      MaxSupportedLinkSpeed = GetMaxSupportedLinkSpeed ();
      KtiInternalGlobal->KtiRunAdaptation = TRUE;
      KtiInternalGlobal->CurrentSpeedAdapted = MaxSupportedLinkSpeed;
      AdaptationAction = ADAPTATION_REQUIRED;
    }
  } else {
    //
    // There is a txeq adaptation table valid, but still need to check if the target speed is adapted already.
    // First, get the speeds which the adaptation table supports.
    //
    for (Idx = 0; Idx < MAX_KTI_LINK_SPEED; Idx ++) {
      if (KTIVAR->KtiAdaptationTable[Idx].TableValid == TRUE) {
        KtiInternalGlobal->AdaptSpeedsCompleted |= KtiInternalGlobal->AdaptSpeedsCompleted | (1 << KTIVAR->KtiAdaptationTable[Idx].Speed);
      }
    }
    if (KTISETUP->KtiAdaptationSpeed != MAX_KTI_LINK_SPEED) {
      //
      // If we only do the adaptation for a specified speed, check the speed is valid in the table.
      //
      if ((KtiInternalGlobal->AdaptSpeedsCompleted & (1 << KTISETUP->KtiAdaptationSpeed)) == 0) {
        if (CheckLinkSpeedSupported (KTISETUP->KtiAdaptationSpeed) == KTI_SUCCESS) {
          KtiInternalGlobal->KtiRunAdaptation = TRUE;
          KtiInternalGlobal->CurrentSpeedAdapted = KTISETUP->KtiAdaptationSpeed;
          AdaptationAction = ADAPTATION_REQUIRED;
        } else {
          AdaptationAction = ADAPTATION_REQUIRED_SPEED_NOT_SUPPORTED;
        }
      }
    } else {
      //
      // If we are going to adapt for all speeds, check if the table covers all.
      // If not, do the adaptation for the missed speed.
      //
      for (Idx = SPEED_REC_128GT; Idx <= SPEED_REC_160GT; Idx++) {
        if ((KtiInternalGlobal->AdaptSpeedsCompleted & (1 << Idx)) == 0) {
          if (CheckLinkSpeedSupported (Idx) == KTI_SUCCESS) {
            KtiInternalGlobal->KtiRunAdaptation = TRUE;
            KtiInternalGlobal->CurrentSpeedAdapted = Idx;
            AdaptationAction = ADAPTATION_REQUIRED;
            break;
          } else {
            AdaptationAction = ADAPTATION_REQUIRED_SPEED_NOT_SUPPORTED;
          }
        }
      }
    }
  }

  if (AdaptationAction == ADAPTATION_NOT_REQUIRED) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nKTI Adaptation will not run, as the silicon doesn't support the feature or it is already adapted.\n"));
  } else if (AdaptationAction == ADAPTATION_REQUIRED) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nKTI Adaptation is set to run for speed %a\n", ((KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_128GT) ? "128GT" :
                                                                                              (KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_144GT) ? "144GT" :
                                                                                              (KtiInternalGlobal->CurrentSpeedAdapted == SPEED_REC_160GT) ? "160GT" : "UNKNOWN")));
  } else {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nKTI Adaptation will not run, as the speed is not supported.\n"));
  }

  return KtiInternalGlobal->KtiRunAdaptation;
}

/**
  This routine is to check the capability of the TXEQ adaptation

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval None

**/
BOOLEAN
CheckSocketAdaptationCapability (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  //
  // TBD, need to confirm if there is a capability bit for txeq adaptation
  //
  return TRUE;
}