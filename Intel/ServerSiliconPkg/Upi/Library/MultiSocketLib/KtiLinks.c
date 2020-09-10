/** @file
  This file includes functions which are related KTI links, like speed transition, PHY/link layer programming etc.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <Library/EightSocketLib.h>
#include <KtiMisc.h>
#include <Library/KtiApi.h>
#include <RcRegs.h>
#include <KtiSetupDefinitions.h>
#include <UncoreCommonIncludes.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <ScratchpadList.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/FpgaIpInterface.h>
#include <Library/SiliconWorkaroundLib.h>
#include "MultiSocketInternal.h"
#include "KtiAdapt.h"

STATIC
KTI_STATUS
IssuePhyResetOnCpuLink (
  UINT8               Cpu,
  UINT8               Link,
  UINT8               ResetType
  );

STATIC
KTI_STATUS
ProgramLateAction (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT32              Group,               //0: KTI
  UINT32              Value
  );

STATIC
KTI_STATUS
DisableLinkForceToLowPower (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                     Socket,
  UINT8                     Link
  );

STATIC
KTI_STATUS
DisableForceToLowPowerAllUnusedLinks (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

STATIC
KTI_STATUS
ProgramLinkLayer (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_SOCKET_DATA          *SocketData,
  UINT8                    Group
  );

KTI_STATUS
SendMailBoxCmdSetKtiFreq (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

KTI_STATUS
ProgramPhyLayerControlRegisters (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData,
  UINT8                     Index,
  UINT8                     LinkIndex,
  UINT8                     Group
  );

KTI_STATUS
ProgramEparams (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Index,
  UINT8                      LinkIndex,
  UINT8                      Group
  );

STATIC CHAR8 UNIPHY_RECIPE_MESSAGE[] = "\n  ****** S%dp%d Program UniPhy Recipe - %a ******\n";
STATIC CHAR8 PHYCONTROL_MESSAGE[] = "\n  ****** S%dp%d Program Phy Control Regs - %a ******\n";

UINT8 SpeedTable[MAX_KTI_LINK_SPEED] = {96, 104, 112}; // 0:9.6G, 1:10.4G, 2:11.2G
UINT8 SpeedTableSpr[MAX_KTI_LINK_SPEED] = {128, 144, 160}; // 0:12.8G, 1:14.4G, 2:16.0G

/**
  Transition the KTI links to full speed operation.

  To transition the link to full speed, for each link we need to:
    1. Validate the phy/link layer input parameters; force safe values if needed
    2. Ensure that no partial loading of parameters by setting LA Disable
    3. Program the Link & Phy layer parameters
    4. Clear LA Disable

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

**/
KTI_STATUS
KtiTransitionFullSpeed (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{

  STATIC CHAR8 FULLSPEED_STATUS_MESSAGE[] = "\n\n  ******       %a      - %a ******";
  STATIC CHAR8 LATE_ACTION_MESSAGE[] ="\n\n  %a Late Action Disable bit for each socket/link\n";

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_FULL_SPEED_TRANSITION, MINOR_STS_FULL_SPEED_START);

  //
  // Transition to FULL speed?
  //
  if ((KTISETUP->KtiLinkSpeedMode == KTI_LINK_FULL_SPEED_MODE) && ((KtiInternalGlobal->ProgramPhyLink == TRUE) || (KTISETUP->KtiLbEn))) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n  Clearing KTI DFX Locks\n"));
    ClearOrSetKtiDfxLckForFullSpeedTransition (KtiInternalGlobal, 0);

    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, LATE_ACTION_MESSAGE, "Setting"));
    ProgramLateAction (KtiInternalGlobal, KTI_GROUP, 1);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, FULLSPEED_STATUS_MESSAGE, "Phy Init", "START" ));
    KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_FULL_SPEED_TRANSITION, MINOR_STS_PHY_LAYER_SETTING);
    // s5370513: RxDCC override in slow mode
    if (KtiInternalGlobal->CpuType == CPU_SKX || KtiInternalGlobal->CpuType == CPU_CLX) {
      SlowModeRxDccOverride (SocketData, KtiInternalGlobal);  //CHECK_CPX
    }
    ProgramPhyLayer (KtiInternalGlobal, SocketData, KTI_GROUP);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, FULLSPEED_STATUS_MESSAGE, "Phy Init", "END  " ));

    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, FULLSPEED_STATUS_MESSAGE, "LL init ", "START" ));
    KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_FULL_SPEED_TRANSITION, MINOR_STS_LINK_LAYER_SETTING);
    ProgramLinkLayer (KtiInternalGlobal, SocketData, KTI_GROUP);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, FULLSPEED_STATUS_MESSAGE, "LL Init ", "END  " ));

    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, LATE_ACTION_MESSAGE, "Clearing"));
    ProgramLateAction (KtiInternalGlobal, KTI_GROUP, 0);

    //
    // The specific steps on upi speed transition
    // Only some steps for SPR now, Simics doesn't support the new flow now.
    //
    KtiSpeedTransitionMiscSteps (SocketData, KtiInternalGlobal, KTI_LINK_FULL_SPEED_MODE);

    //
    // SPR doesn't need to trigger warm reset for speed transition.
    //
    if (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
      SetNewResetRequired (POST_RESET_WARM);
    }

    //
    // Check if there is another speed need to be adapted in the adaptation flow.
    // For warm reset flow, the adaptation will only be taked for 1 speed.
    // For no warm reset flow, the routine will adapt for all speed if it is required.
    //
    if (KtiInternalGlobal->KtiRunAdaptation == TRUE) {
      CheckAndExecuteKtiAdaptation (SocketData, KtiInternalGlobal);
    }
   } else {

    //
    // Indicate not transitioning to FS mode and why.
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  %a",
       (KtiInternalGlobal->TotCpu == 1) ? "Single Socket, no KTI Links to transition" : "Full speed transition not requested"));

    //
    // Force unused ports to low power mode
    //
    DisableForceToLowPowerAllUnusedLinks (SocketData, KtiInternalGlobal);
  }

  //
  // Execute this for any topology: multi-socket or 1S-hotplug-enabled in warm reset flow.
  // SPR doesn't require to file the B2P command here, as each BSP of the socket will file their own B2P command.
  //
  if (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    SendMailBoxCmdSetKtiFreq(SocketData, KtiInternalGlobal);
  }

  return KTI_SUCCESS;
}

 /**
  Transition the KTI links to full speed operation.

  To transition the link to full speed, for each link we need to:
    1. Validate the phy/link layer input parameters; force safe values if needed
    2. Ensure that no partial loading of parameters by setting LA Disable
    3. Program the Link & Phy layer parameters
    4. Clear LA Disable

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

**/
KTI_STATUS
KtiTransitionFullSpeedOnWarmReset (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  KTI_STATUS                            Status;
  UINT8                                 Index, LinkIndex, sbspSktId, MaxKtiPort;
  UINT32                                PeerSocId, PeerPort;
  BOOLEAN                               ResetState[MAX_SOCKET][MAX_FW_KTI_PORTS];
  BIOS_NON_STICKY_SCRATCHPAD0_STRUCT    BiosScratchPad0;
  STATIC CHAR8 PHY_WA_MESSAGE[] = "\n\n  ******       Phy WA init      - %a ******\n";

  sbspSktId  = KtiInternalGlobal->SbspSoc;
  MaxKtiPort = GetKtiPortCnt ();

  if (IsCpuAndRevision (CPU_ICXSP, REV_R0)) {

    BiosScratchPad0.Data = GetNonStickyScratchpad(0, BIOS_NON_SCRATCHPAD_0);
    if (BiosScratchPad0.Bits.SlowModeLinkTrain == 0) {
      return KTI_SUCCESS;
    }

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, PHY_WA_MESSAGE, "START"));

    for (Index = 0; Index < MAX_SOCKET; Index++) {
      ZeroMem (&ResetState[Index], sizeof(UINT8)  * MAX_FW_KTI_PORTS);
    }

    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (SocketPresent (Index)) {
        for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
          if ((KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].Valid == TRUE) || (KTISETUP->KtiLbEn)) {
            //
            // Program the UniPhy recipe (independent of board layout)
            //
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, UNIPHY_RECIPE_MESSAGE, Index, LinkIndex, "START"));
            if (!(UbiosGenerationOrHsleEnabled ())) {
               KtiEvAutoRecipe (KtiInternalGlobal, Index, LinkIndex);
            }
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, UNIPHY_RECIPE_MESSAGE, Index, LinkIndex, "END"));
            //
            // Program CTR1 and LDC registers
            //
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, PHYCONTROL_MESSAGE, Index, LinkIndex, "START"));
            ProgramKtiReutPhWA (Index, LinkIndex);
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, PHYCONTROL_MESSAGE, Index, LinkIndex, "END"));
          }
        } // For LinkIndex (2nd instance - Valid links only)
      } // If Socket valid
    } // For Socket

    //
    // Do reset in a serial way, but non optima way as some resets could time out
    // and make some sockets unreachable. A tree would be better so that we can start from the children.
    //
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (SocketPresent (Index)) {
        for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
          if ((KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].Valid == TRUE) || (KTISETUP->KtiLbEn)) {
            PeerSocId = KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].PeerSocId;
            PeerPort  = KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].PeerPort;
            if (ResetState[PeerSocId][PeerPort] == FALSE && ResetState[Index][LinkIndex] == FALSE) {
              Status  = IssuePhyResetOnCpuLink (Index, LinkIndex, 0);
              if (Status == KTI_SUCCESS) {
                ResetState[PeerSocId][PeerPort] = TRUE;
                ResetState[Index][LinkIndex] = TRUE;
              } else {
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n[ERROR]  SocId=%d, Link=%d failed to train after PhyReset.", Index, LinkIndex));
                KTI_ASSERT ((Status != KTI_SUCCESS), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_1);
                return KTI_FAILURE;
              }
            }
          }
        }
      }
    }

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, PHY_WA_MESSAGE, "END  "));
  }

  KTIVAR->KtiCurrentLinkSpeedMode = GetKtiCurrentLinkSpeedMode (KtiInternalGlobal, sbspSktId, MaxKtiPort);

  if (KTIVAR->OutKtiLinkSpeed == MAX_KTI_LINK_SPEED) {
    KTIVAR->OutKtiLinkSpeed = GetMaxSupportedLinkSpeed();
  }
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Link Speed (after warm reset): %a",
                      (KTIVAR->KtiCurrentLinkSpeedMode == KTI_LINK_SLOW_SPEED_MODE) ? "Slow Speed" : "Fast Speed"));

  return KTI_SUCCESS;
}

/**
  Send a pCode mailbox command to set KTI link frequency.

  @param SocketData        - pointer to the socket data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
SendMailBoxCmdSetKtiFreq (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8    Index, LinkIndex;
  UINT32   LinkSpeed = 0;
  UINT8    LastM3KtiPort;
  UINT8    CurrentM3KtiPort;
  BOOLEAN  MailboxCmdSent;
  UINT8    CpuType;

  CpuType     = KtiInternalGlobal->CpuType;

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      CurrentM3KtiPort = LastM3KtiPort = 0;
      MailboxCmdSent = FALSE;

      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE || KTIVAR->OutKtiCpuSktHotPlugEn == TRUE){
          switch (KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed) {
            //
            // ICX & SKX cases - SPEED_REC_96GT;
            // SPR case - SPEED_REC_128GT
            //
            case 0:
              if (!IsSprFamilyCpu (CpuType)) {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_96GT;
              } else {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_128GT;
              }
              break;
            //
            // ICX & SKX cases - SPEED_REC_104GT;
            //  SPR case - SPEED_REC_144GT
            //
            case 1:
              if (!IsSprFamilyCpu (CpuType)) {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_104GT;
              } else {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_144GT;
              }
              break;
            //
            // ICX & SKX cases - SPEED_REC_112GT;
            // SPR case - SPEED_REC_160GT
            //
            case 2:
              if (!IsSprFamilyCpu (CpuType)) {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_112GT;
              } else {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_160GT;
              }
              break;
            default:
              if (!IsSprFamilyCpu (CpuType)) {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_96GT;
              } else {
                LinkSpeed = UPI_RATIO_FOR_SPEED_REC_128GT;
              }
              break;
          }
          if (KTIVAR->SharedKtiPort0And1 == TRUE) {
            CurrentM3KtiPort = PORT_TO_M3KTI (LinkIndex);

            if (CurrentM3KtiPort != LastM3KtiPort) {
              MailboxCmdSent = FALSE;
            }

            if (!MailboxCmdSent) {
              PcuSetUpiFrequency (Index, LinkIndex, LinkSpeed);
              MailboxCmdSent = TRUE;
            }

            LastM3KtiPort = CurrentM3KtiPort;
          } else {
            //
            // Send command/link speed for each port
            //
            PcuSetUpiFrequency (Index, LinkIndex, LinkSpeed);
          }
        } // valid link?
      } // for LinkIndex loop
    } else if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_FPGA)) { // valid socket?
      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
          FpgaIpSetLinkSpeed (Index, LinkIndex);
        }
      }
    }
  } // for socket

  return KTI_SUCCESS;
}

/**
  Program late action registers.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Group             - 0: KTI
  @param Value             - Value to program

  @retval KTI_SUCCESS - on successful completion

**/
STATIC
KTI_STATUS
ProgramLateAction (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT32                    Group,
  UINT32                    Value
  )
{
  UINT8 Index;
  UINT8 LinkIndex;

  if (Group == KTI_GROUP) {               //for KTI links
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (SocketPresent (Index)) {

        for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
          //
          // Program lateactionloaddisable bit
          //
          if ((KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].Valid == TRUE) || (KTISETUP->KtiLbEn)) {
            if (IsUpiPortUpimdf (Index, LinkIndex)) {
              //
              // The port is UPIMDF subsystem, skip the phy accessing
              //
              continue;
            }
            ProgramLateActionOnCpuLink (Index, LinkIndex, Value);
          }
        }
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Programs PHY layer registers

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocketData        - Socket specific data structure
  @param Group             - 0: KTI

**/
KTI_STATUS
ProgramPhyLayer (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData,
  UINT8                     Group
  )
{
  UINT8         Index;
  UINT8         LinkIndex;
  UINT8         CpuType;

  CpuType     = KtiInternalGlobal->CpuType;

  if (Group != KTI_GROUP) {
    return KTI_SUCCESS;
  }

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketPresent (Index)) {
      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if ((KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].Valid == TRUE) || (KTISETUP->KtiLbEn)) {
          if (IsUpiPortUpimdf (Index, LinkIndex)) {
            //
            // The port is UPIMDF subsystem, skip the phy accessing
            //
            continue;
          }
          //
          // UnLock Phy Registers
          //
          PhyCsrLockManager (Index, LinkIndex, UNLOCK);
          //
          // Program Electrical Parameters (board-layout-dependent) and get the redriver status (if SPR)
          //
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** S%dp%d Program Eparams - %a ******\n", Index, LinkIndex, "START"));
          if (!(UbiosGenerationOrHsleEnabled ())) {
            ProgramEparams (KtiInternalGlobal, Index, LinkIndex, KTI_GROUP);
          }
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** S%dp%d Program Eparams - %a ******\n", Index, LinkIndex, "END"));
          //
          // Program CTR1 and LDC registers
          //
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, PHYCONTROL_MESSAGE, Index, LinkIndex, "START"));
          ProgramPhyLayerControlRegisters (KtiInternalGlobal, SocketData, Index, LinkIndex, KTI_GROUP);
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, PHYCONTROL_MESSAGE, Index, LinkIndex, "END"));

          //
          // Program the UniPhy recipe (independent of board layout)
          //
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, UNIPHY_RECIPE_MESSAGE, Index, LinkIndex, "START"));
          if (!(UbiosGenerationOrHsleEnabled ())) {
            KtiEvAutoRecipe (KtiInternalGlobal, Index, LinkIndex);
          }
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, UNIPHY_RECIPE_MESSAGE, Index, LinkIndex, "END"));

          //
          // This comes after Uniphy recipe update to avoid register being overwritten by Uniphy recipe
          //
          ExtendKtiL0pExitTime (Index, LinkIndex);

          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** S%dp%d Program Txeq Adaptation params - START ******\n", Index, LinkIndex));
          //
          // Program the Txeq Adaptation params for speed transition, if there is an adaptation required,
          // it will program the registers to enter the adaptation flow.
          //
          KtiProgramTxeqAdaptParams (KtiInternalGlobal, Index, LinkIndex);
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** S%dp%d Program Txeq Adaptation params - END ******\n", Index, LinkIndex));

          //
          // Lock Phy Registers
          //
          PhyCsrLockManager (Index, LinkIndex, LOCK);
        } else {
          //
          // Put unused links into low power/disable state
          //
          if ((KtiInternalGlobal->KtiPortPresentBitmap[Index] & (1 << LinkIndex)) != 0) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** S%dp%d Disable and Force Link to Low Power - %a ******\n", Index, LinkIndex, "START"));
            DisableLinkForceToLowPower (KtiInternalGlobal, Index, LinkIndex);
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** S%dp%d Disable and Force Link to Low Power - %a ******\n", Index, LinkIndex, "END"));
          }
        }
      } // For LinkIndex (2nd instance - Valid links only)
    } else if (SocketData->Cpu[Index].Valid && (FpgaIpIsNodeFpga (Index) == TRUE)) {
      for (LinkIndex = 0; LinkIndex < FPGA_WHR_UPI_PORTS; LinkIndex++) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** F%dp%d Program Eparams - %a ******\n", Index, LinkIndex, "START"));
        if (!(UbiosGenerationOrHsleEnabled ())) {
          ProgramEparams (KtiInternalGlobal, Index, LinkIndex, KTI_GROUP);
        }
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  ****** F%dp%d Program Eparams - %a ******\n", Index, LinkIndex, "END"));
      }
    }
  } // For Socket

  return KTI_SUCCESS;
}



/**
  Disables the KTI port.
  Accounts for two types of disabled KTI ports: unconnected ports and
  FW disabled ports. To determine the type of the disabled port, we
  check the PcSts.ll_status value. For connected, but FW-disabled links,
  we will see the LINK_SM_INIT_DONE value there.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - SocId we are looking up
  @param Link              - Link# on socket (0 or 1)

**/
KTI_STATUS
DisableLinkForceToLowPower (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8               Socket,
  UINT8               Link
  )
{
  if (GetLlStatus (Socket, Link) == LINK_SM_INIT_DONE) {
    //
    // Firmware-disabled KTI link since it trained in slow mode
    //
    SetCInitBegin (Socket, Link, 0);
    SetCpReset (Socket, Link);
    //
    // Tell the link to report L1 state to the PCU
    //
    SetUpiLinkToL1 (Socket, Link);
  } else {
    //
    // Unconnected link
    //
    SetCInitBegin (Socket, Link, 0);

    //
    // If hot plug is enabled, mask FW_AGENT through pcode mailbox command
    //
    if (KTIVAR->OutKtiCpuSktHotPlugEn == TRUE) {
      PcuMaskFwAgent (Socket, Link);
    }
  }

  return KTI_SUCCESS;
}



/**
  Disables the link forces it to low power

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

**/
STATIC
KTI_STATUS
DisableForceToLowPowerAllUnusedLinks (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{

  UINT8 Index;
  UINT8 LinkIndex;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Force unused links to disabled/low power state.\n"));
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if ((KtiInternalGlobal->KtiPortPresentBitmap[Index] & (1 << LinkIndex)) != 0) {
          if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == FALSE)  && !(KTISETUP->KtiLbEn)) {
            DisableLinkForceToLowPower (KtiInternalGlobal, Index, LinkIndex);
          }
        }
      }
    }
  }

  return KTI_SUCCESS;

}

/**
  Misc Link layer configuration

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Index             - Socket ID
  @param LinkIndex         - Link number
  @param Group             - KTI

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramLinkLayerControlRegisters (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Index,
  UINT8                    LinkIndex,
  UINT8                    Group
  )
{
  UINT8 KtiLLVnaCredit;
  UINT8 KtiLLVnaCreditWithVn1En;
  //
  // Setup CRC mode based on the output variable and check the per-link DFX override.
  //
  if (!IsUpiPortUpimdf (Index, LinkIndex)) {
    SetKtiLinkCrcMode (Index, LinkIndex, KTIVAR->OutKtiCrcMode, KTISETUP->DfxCpuCfg[Index].Link[LinkIndex].DfxCrcMode);
  }

  //
  // Setup LinkCreditControl
  //
  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    KtiLLVnaCredit = KTI_LL_VNA_NORMAL_SPR;
    KtiLLVnaCreditWithVn1En = KTI_LL_VNA_NORMAL_SPR;
  } else {
    KtiLLVnaCredit = KTI_LL_VNA_NORMAL_SKX;
    KtiLLVnaCreditWithVn1En = KTI_LL_VNA_WITH_VN1_SKX;
  }

  ProgramLinkCreditControl (Index, LinkIndex, KtiInternalGlobal->PhyLinkPerPortSetting[Index].Link[LinkIndex].KtiLinkVnaOverride, KTIVAR->OutVn1En, KTIVAR->OutKtiFpgaEnable[Index], KtiLLVnaCredit, KtiLLVnaCreditWithVn1En);


  //
  // Mask the COR PHY RESET error on the Link for SPR, as it will trigger the cprset to do the speed transition
  //
  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    ConfiguCorPhyResetErrOnKtiErrDis0 (Index, LinkIndex, TRUE);
  }

  return KTI_SUCCESS;
}

/**
  Program Link layer registers

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocketData        - Socket specific data structure

**/
STATIC
KTI_STATUS
ProgramLinkLayer (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData,
  UINT8                     Group
  )
{
  UINT8 Index, LinkIndex, M3KtiCtr, ProgrammedM3Kti;
  UINT32 VnaCreditCpu;
  UINT32 VnaCreditWhr;
  UINT32 VnaCredit0;
  UINT32 VnaCredit1;
  UINT32 VnaCredit2;
  BOOLEAN UpimdfPortFlag;

  if (KTIVAR->OutVn1En == TRUE) {
    VnaCreditCpu = KTI_LL_VNA_WITH_VN1_SKX;
  } else {
    VnaCreditCpu = KTI_LL_VNA_NORMAL_SKX;
  }
  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    VnaCreditCpu = KTI_LL_VNA_NORMAL_SPR;
  }
  VnaCreditWhr = KTI_LL_VNA_NORMAL_WHR;

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketPresent (Index)) {
      ProgrammedM3Kti = M3KtiCtr = 0xff;
      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if (KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].Valid == TRUE) {
          UpimdfPortFlag = FALSE;
          ProgramLinkLayerControlRegisters (KtiInternalGlobal, Index, LinkIndex, Group);

          if (IsIcxFamilyCpu (KtiInternalGlobal->CpuType) || IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
            M3KtiCtr = LinkIndex;
            //
            // Get VNA Credit total from opposite side of link (different credits for FPGA and Xeon)
            //
            if (SocketData->Cpu[Index].LinkData[LinkIndex].PeerSocType == SOCKET_TYPE_FPGA) {
              VnaCredit0 = VnaCreditWhr;
            } else {
              VnaCredit0 = VnaCreditCpu;
            }

            if (IsUpiPortUpimdf (Index, M3KtiCtr)) {
              VnaCredit0 = KTI_LL_BL_VNA_SPRUCC;
              VnaCredit1 = KTI_LL_AD_VNA_SPRUCC;
              VnaCredit2 = KTI_LL_AK_VNA_SPRUCC;
              UpimdfPortFlag = TRUE;
            }

            if (UpimdfPortFlag) {
              //
              // Program  M3INGDBGREMCRE_0/1/2_KTI0_M3KTI_MAIN_REG, VnaCredit0 - bl vna; VnaCredit1 - ad vna; VnaCredit2 - ak vna
              //
              ProgramLinkLayerOnM3Kti (Index, M3KtiCtr, &VnaCredit0, &VnaCredit1, &VnaCredit2);
            } else {
              //
              // Program  M3INGDBGREMCRE_0/1_KTI0_M3KTI_MAIN_REG, the value need to match with LCCL setting on peer port of peer socket
              //
              ProgramLinkLayerOnM3Kti (Index, M3KtiCtr, &VnaCredit0, NULL, NULL);
            }
            //
            // ICX R0 WA
            //
            if (IsCpuAndRevision (CPU_ICXSP, REV_R0)) {
              if (KTIVAR->SysConfig == SYS_CONFIG_2S2L) {
                ProgramExtraStallOnM3Kti (Index, M3KtiCtr);
              }
              if (IsSiliconWorkaroundEnabled ("S1606774351")) {
                ProgramDisableAdByPassOnM3Kti (Index, M3KtiCtr);
              }
            }
          } else {
            //
            // SKX + CPX, KTI port 0 & 1 share 1 M3KTI0, KTI port 2 owns M3KTI1
            //
            M3KtiCtr = PORT_TO_M3KTI (LinkIndex);
            if (ProgrammedM3Kti != M3KtiCtr) {
              //
              // Get VNA Credit total from opposite side of link (different credits for FPGA and Xeon)
              // for BOTH links on this M3KTI.
              //
              if (SocketData->Cpu[Index].LinkData[LinkIndex].PeerSocType == SOCKET_TYPE_FPGA) {
                VnaCredit0 = VnaCreditWhr;
              } else {
                VnaCredit0 = VnaCreditCpu;
              }
              if ((LinkIndex + 1) < KtiInternalGlobal->KtiPortCnt) {
                if (SocketData->Cpu[Index].LinkData[LinkIndex + 1].PeerSocType == SOCKET_TYPE_FPGA) {
                  VnaCredit1 = VnaCreditWhr;
                } else {
                  VnaCredit1 = VnaCreditCpu;
                }
              } else {
                VnaCredit1 = VnaCreditCpu;
              }
              //
              // Program  M3INGDBGREMCRE_0/1_KTI0/1_M3KTI_MAIN_REG, the value need to match with LCCL setting on peer port of peer socket
              //
              if ((KTIVAR->OutVn1En) && (KTIVAR->SysConfig >= SYS_CONFIG_4S) &&
                  (IsSkxFamilyCpu (KtiInternalGlobal->CpuType))) {
                MaskRemoteVnaCreditOverflow (Index, M3KtiCtr);
              }
              ProgramLinkLayerOnM3Kti (Index, M3KtiCtr, &VnaCredit0, &VnaCredit1, NULL);
              ProgrammedM3Kti = M3KtiCtr;
            }
          }
        }  // link valid
      } // for link
    } // socket valid
  } // for socket

  return KTI_SUCCESS;
}

/**
  Prime the KTIRC data structure based on input option and system capabilities.
  Also normalize DFX per-link options.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructurePhyLinkLayer (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8    Index, LinkIndex, LinkSpeed;
  BOOLEAN  PerLinkSpeed;
  UINT8    CpuType;
  UINT8    KtiRate;
  UINT32   CrcMode;
  UINT32   Vn1Supported;
  UINT32   L0pSupport;
  UINT32   L1Support;
  UINT32   FailoverSupport;

  if (KtiInternalGlobal->ProgramPhyLink == TRUE) {
    //
    // Gather common link and phy layer capabilites of all valid sockets
    //
    GatherCommonKtiCapabilities (KtiInternalGlobal, &CrcMode, &Vn1Supported, &L0pSupport, &L1Support, &FailoverSupport);
    CpuType     = KtiInternalGlobal->CpuType;

    if (KTIVAR->SysConfig == SYS_CONFIG_8S) {
      if (Vn1Supported == 0) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n%a () - VN1 is needed, but KTILCP reports VN1 is not supported.\n", __FUNCTION__));
      }
      if (!IsSprFamilyCpu (CpuType)) {
        KTI_ASSERT ((Vn1Supported != 0), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_SKU_MISMATCH);
      }
    }

    //
    //  SPR has to do the speed transition in all kinds of reset
    //
    if (KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD || IsSprFamilyCpu (CpuType) || IsGnrSrfFamilyCpu (CpuType)) {
      //
      // Cold reset path: prime setup options against capabilities
      //
      if ((CrcMode == 0) || (KTIVAR->OutKtiCrcMode == CRC_MODE_16BIT) || (KTIVAR->OutKtiCrcMode == CRC_MODE_AUTO)) {
        //
        // Rolling 32bit is not supported, or user specifically asked for 16bit mode
        //
        KTIVAR->OutKtiCrcMode = CRC_MODE_16BIT;
      } else {
        //
        // Rolling 32bit is supported and user didn't specifically request 16bit mode
        //
        KTIVAR->OutKtiCrcMode = CRC_MODE_ROLLING_32BIT;
      }

      if ((L0pSupport == 0) || (KTIVAR->OutKtiLinkL0pEn == KTI_DISABLE)) {
        //
        // L0p is not supported or user specifically asked to disable L0p
        //
        KTIVAR->OutKtiLinkL0pEn = KTI_DISABLE;
      } else {
        //
        // L0p is supported and user didn't specifically request to disable L0p
        //
        KTIVAR->OutKtiLinkL0pEn = KTI_ENABLE;
      }

      if ((L1Support == 0) || (KTIVAR->OutKtiLinkL1En == KTI_DISABLE)) {
        //
        // L1 is not supported or user specifically asked to disable L1
        //
        KTIVAR->OutKtiLinkL1En = KTI_DISABLE;
      } else {
        //
        // L1 is supported and user didn't specifically request to disable L1
        //
        KTIVAR->OutKtiLinkL1En = KTI_ENABLE;
      }

      if ((FailoverSupport == 0) || (KTIVAR->OutKtiFailoverEn == KTI_DISABLE)) {
        //
        // Failover is not supported or user specifically asked to disable it
        //
        KTIVAR->OutKtiFailoverEn = KTI_DISABLE;
      } else {
        //
        // Failover is supported and user didn't specifically request to disable it
        //
        KTIVAR->OutKtiFailoverEn = KTI_ENABLE;
      }

      //
      // This updates the OutKtiLinkSpeed and per-link frequency variables
      //
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n  ****** Selecting KTI freq. - %a ******", "START"));
      SelectSupportedKtiLinkSpeed (SocketData, KtiInternalGlobal);
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n  ****** Selecting KTI freq. - %a ******", "END  "));

      //
      // Normalize per-port knobs
      //
      NormalizeLinkOptions (SocketData, KtiInternalGlobal);
    } else {
      //
      // After warm reset, check status bits and populate output variables
      //

      //
      // We are in warm reset path; Log a warning if the links are in slow mode && not the 1S-FPGA topology with FPGA enabled case.
      //
      if ((KTIVAR->KtiCurrentLinkSpeedMode == KTI_LINK_SLOW_SPEED_MODE) &&
          (!((KTIVAR->SysConfig == SYS_CONFIG_1S) && (KTIVAR->OutKtiFpgaPresent[KtiInternalGlobal->SbspSoc] == 1) &&
             (KTIVAR->OutKtiFpgaEnable[KtiInternalGlobal->SbspSoc] == 1)))) {
        KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_LINK_SLOW_SPEED_MODE);
        KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING: System is operating in KTI Link Slow Speed Mode. "));
      } else {
        //
        // Need to select supported speed for hot-adds later
        //
        if (KTIVAR->OutKtiCpuSktHotPlugEn == TRUE || IsSprFamilyCpu (CpuType) || IsGnrSrfFamilyCpu (CpuType)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n  ****** Selecting KTI freq. - %a ******", "START"));
          SelectSupportedKtiLinkSpeed (SocketData, KtiInternalGlobal);
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n  ****** Selecting KTI freq. - %a ******", "END  "));
        }

        //
        // Identify the per link speed
        //
        LinkSpeed = 0xFF;
        PerLinkSpeed = FALSE;
        for (Index = 0; Index < MAX_SOCKET; Index++) {
          if (SocketData->Cpu[Index].Valid == TRUE) {
            for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
              if ((SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
                //
                // Display the link's fast-mode speed.
                //
                if (KTIVAR->SharedKtiPort0And1 == TRUE) {
                  KtiRate = GetKtiRate (Index, ((LinkIndex / 2) * 2));
                } else {
                  KtiRate = GetKtiRate (Index, LinkIndex);
                }
                KTIVAR->OutPerLinkSpeed[Index][LinkIndex] = KtiRate - 4;

                //Link 0, 1, or 2 on B0 or later, print the speed based on KtiMiscSts
                KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Socket %u KTI Link %u Freq is currently %u.", Index, LinkIndex,
                                     (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) ? SpeedTable[KtiRate - 4] : SpeedTableSpr[KtiRate - 4]));

                //
                // Keep track of the flag to indicate if two links have different speed (which implies per link speed)
                //
                if (LinkSpeed == 0xFF) {
                  LinkSpeed = KTIVAR->OutPerLinkSpeed[Index][LinkIndex];
                } else if (LinkSpeed != KTIVAR->OutPerLinkSpeed[Index][LinkIndex]){
                  PerLinkSpeed = TRUE;
                }
              }
            }
          }
        }

        //
        // Identify the system wide speed
        //
        KTIVAR->OutKtiLinkSpeed = MAX_KTI_LINK_SPEED;
        if (PerLinkSpeed == TRUE) {
          KTIVAR->OutKtiLinkSpeed = FREQ_PER_LINK;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Per Link speed is used."));
        } else {
          KTI_ASSERT ((LinkSpeed != 0xFF), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_1);
          KTIVAR->OutKtiLinkSpeed = LinkSpeed;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  System wide KTI Freq is currently %u.",
                               (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) ? SpeedTable[KTIVAR->OutKtiLinkSpeed] : SpeedTableSpr[KTIVAR->OutKtiLinkSpeed]));
        }
      }

      //
      // Identify the current L0p/L1/CrcMode settings.These settings are system wide, so just read it for only one valid link.
      //
      for (Index = 0; Index < MAX_SOCKET; Index++) {
        if (SocketData->Cpu[Index].Valid == TRUE) {
          for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
            if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
              KTIVAR->OutKtiCrcMode = (UINT8) GetCurrentCrcMode (Index, LinkIndex);
              GetKtiReutCurrentSettings (Index, LinkIndex, &(KTIVAR->OutKtiLinkL0pEn), &(KTIVAR->OutKtiLinkL1En), &(KTIVAR->OutKtiFailoverEn));
              Index = MAX_SOCKET;
              break;
            }
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to get KTI current link speed

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program
  @param MaxKtiPort        - Max KTI port number

  @retval current KTI Link Speed Mode

**/
UINT8
GetKtiCurrentLinkSpeedMode (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8 SocId,
  IN UINT8 MaxKtiPort
  )
{
  UINT8 Port;
  UINT8 CpuType;
  UINT8 KtiCurrentLinkSpeedMode;
  UINT32 LlStatus;

  KtiCurrentLinkSpeedMode = KTI_LINK_SLOW_SPEED_MODE;
  CpuType = KtiInternalGlobal->CpuType;

  for (Port = 0; Port < MaxKtiPort; Port++) {
    if (((KtiInternalGlobal->KtiPortPresentBitmap[SocId] & (1 << Port)) == 0) || IsUpiPortUpimdf (SocId, Port)) {
      continue;
    }

    LlStatus = GetLlStatus (SocId, Port);
    if ((LlStatus == LINK_SM_INIT_DONE) || (LlStatus == LINK_SM_CRD_RETURN_STALL)) {
      KtiCurrentLinkSpeedMode = GetKtiSpeed (SocId, Port);
      break;
    }
  }

  return KtiCurrentLinkSpeedMode;
}

/**
  Routine to determine and select the maximum KTI link speed supported by CPUs. It is determined by
  Min (Max link speed of all populated CPU sockets).  A valid User Selected freq will be returned
  if enabled. If PER_LINK_SPEED is selected, then the links speeds will be normalized for both ends
  of the link. Note that the normalization will be done only for the sockets & links that are valid.
  So that the per link option of the unpopulated/unconnected links will not influence the normalization
  outcome. For O*L operation, this routine should be called after the socket/link valid information is
  collected so that the per link options can be normalized properly for the new topology scenario.

  If adaptation is scheduled to run, the speed selected will be the one decided by adaptation code.
  Also, the PER_LINK_SPEED request will not be fullfilled since the adaptation can support only
  system wide common speed, not per link speed adaptation.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Speed to transition to via KTIVAR->OutKtiLinkSpeed

**/
KTI_STATUS
SelectSupportedKtiLinkSpeed (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8                    LinkSpeed, MaxSpeed;
  UINT8                    Speed0, Speed1, Speed2;
  UINT8                    Index, LinkIndex;
  BOOLEAN                  LinkDependency;
  KTI_LINK_DATA            *pPeerInfo;


  // Get the Max KTI link speed supported by all populated sockets
  MaxSpeed = GetMaxSupportedLinkSpeed ();

  LinkSpeed = SPEED_REC_96GT;

  if (KtiInternalGlobal->KtiRunAdaptation == TRUE) {
    //
    // KTI adaptation is scheduled to run. Use the speed selected by the adaptation engine.
    //
    if (KtiInternalGlobal->CurrentSpeedAdapted < MAX_KTI_LINK_SPEED) {
      LinkSpeed = KtiInternalGlobal->CurrentSpeedAdapted;
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Adaptation is going to run, using adaptation speed %u", SpeedTableSpr[LinkSpeed]));
    }
  } else if (KTIVAR->OutKtiLinkSpeed == MAX_KTI_LINK_SPEED) {
    //
    // Max speed is requested; so set the speed to maximum speed supported by all populated sockets.
    //
    LinkSpeed = MaxSpeed;
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Max supported KTI Speed requested: %u",
                         (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) ? SpeedTable[LinkSpeed] : SpeedTableSpr[LinkSpeed]));
  } else if (KTIVAR->OutKtiLinkSpeed < MAX_KTI_LINK_SPEED) {
    //
    // See if requested freq is supported
    //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Request: KTI Speed of %u",
                         (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) ? SpeedTable[KTIVAR->OutKtiLinkSpeed] : SpeedTableSpr[KTIVAR->OutKtiLinkSpeed]));
    if (CheckLinkSpeedSupported ((UINT8) KTIVAR->OutKtiLinkSpeed) == KTI_SUCCESS) {
      LinkSpeed = KTIVAR->OutKtiLinkSpeed;
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Requested speed is valid"));
    } else {
      //
      // Requested speed unsupported, default to 9.6GT
      //
      KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_UNSUPPORTED_LINK_SPEED);
      KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  Requested Link Speed %u is not supported. Defaulting to 9.6GT \n",
                          (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) ? SpeedTable[KTIVAR->OutKtiLinkSpeed] : SpeedTableSpr[KTIVAR->OutKtiLinkSpeed]));
      LinkSpeed = SPEED_REC_96GT;
    }
  } else if (KTIVAR->OutKtiLinkSpeed == FREQ_PER_LINK) {
    //
    // Normalize per link speed option.  Link 0 & 1 share same PLL while Link 2 has separate PLL. As a result link 0 & 1 must
    // operate at same speed while link 2 can be at different speed than link 0/1. Also, both sides of a given link must operate
    // at same speed. So if  link 2 of each populated socket is connected to link 0 or 1 at the peer side, then all links in the
    // system will be forced to operate same speed. So to take advantage of the per link Si feature, the platform should have
    // link 2 connected to link 2 at the peer side.
    //

    //
    // First if any of the links request for MAX_KTI_LINK_SPEED, or speed > MaxSpeed then set the speed for those links
    // to max allowed speed. Note that max allowed speed is system wide, not per link or per socket. Because Si
    // doesn't support per link max allowed speed, and we don't allow mixing of SKUs.
    // Note: The assumption is MaxSpeed and all speeds below it are assumed to be supported.
    //
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
          if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
            if (KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed > MaxSpeed) {
              KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed = MaxSpeed;
            }
          }
        }
      }
    }

    //
    // Normalize speeds by:
    //       1. Making sure link 0 & 1 have same link speed, otherwise force the speed to minmum of the two.
    //       2. Making sure the peers have same link speed; otherwise select the minimum of the two.
    // Note that step 2 can cuase "chain reaction". For example, if there is a S0 link1- S1 link2 connection,
    // step will 2 can cause S0 link1's speed to change. That requires S0 link0 speed to be changed as well.
    // So we have to repeat step 1. That could cause further dependencies. So we need  to repeat step 1 & 2
    // until step they don't change any link's speed. Since during each occurance of mismatched speed the
    // minimum speed is selected, the "chain reaction" is guranteed to stop and the loop will break.
    //
    do {
      LinkDependency = FALSE;
      //
      // If UPI link 0 & 1 share the same M2UPI, make sure link 0 & 1 have the same link speed; otherwise, force the speed to a minimum of the two
      // If UPI link 0 & 1 do not share the same M2UPI then skip this operation.
      //
      if (KTIVAR->SharedKtiPort0And1) {
        for (Index = 0; Index < MAX_SOCKET; Index++) {
          if (SocketData->Cpu[Index].Valid == FALSE) {
            continue;
          }
          if (SocketData->Cpu[Index].LinkData[0].Valid == TRUE && SocketData->Cpu[Index].LinkData[1].Valid == TRUE) {
            Speed0 = (UINT8)KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[0].KtiLinkSpeed;
            Speed1 = (UINT8)KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[1].KtiLinkSpeed;
            if (Speed0 != Speed1) {
              (Speed0 < Speed1) ? (Speed2 = Speed0) : (Speed2 = Speed1);
              KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[0].KtiLinkSpeed = Speed2;
              KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[1].KtiLinkSpeed = Speed2;
              LinkDependency = TRUE;
            }
          }
        }
      }
      //
      // Make sure the peers have same link speed; otherwise select the minimum of the two.
      //
      for (Index = 0; Index < MAX_SOCKET; Index++) {
        if (SocketData->Cpu[Index].Valid == TRUE) {
          for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
            if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
              pPeerInfo = &(SocketData->Cpu[Index].LinkData[LinkIndex]);
              Speed0 = (UINT8)KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed;
              Speed1 = (UINT8)KtiInternalGlobal->PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].KtiLinkSpeed;
              if (Speed0 != Speed1) {
                (Speed0 < Speed1) ? (Speed2 = Speed0) : (Speed2 = Speed1);
                KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed = Speed2;
                KtiInternalGlobal->PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].KtiLinkSpeed = Speed2;
                LinkDependency = TRUE;
              }
            }
          }
        }
      }
    }while (LinkDependency == TRUE);

    //
    // Print the normalized per link speed.
    //
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
         for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
           if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
             KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  %a %u Link %u KTI Freq is %u GT.",
                                  (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU) ? "Socket" : "FPGA",
                                  Index, LinkIndex,
                                  (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) ? SpeedTable[KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed] :
                                                                            SpeedTableSpr[KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed]));
           }
         }
      }
    }
  }

  //
  // If the requested speed is not per link, copy the selected speed in to the output structure and replicate it into the per link setup structure.
  //
  if (KTIVAR->OutKtiLinkSpeed != FREQ_PER_LINK) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Selected KTI Freq is %u GT",
                         (!IsSprFamilyCpu (KtiInternalGlobal->CpuType)) ? SpeedTable[LinkSpeed] : SpeedTableSpr[LinkSpeed]));
    KTIVAR->OutKtiLinkSpeed = LinkSpeed;
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (SocketData->Cpu[Index].Valid == TRUE || KTIVAR->OutKtiCpuSktHotPlugEn == TRUE) {
        for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
          KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed = LinkSpeed;
        }
      }
    }
  }

  //
  // Update the Host output per link speed entry with the selected speed
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        KTIVAR->OutPerLinkSpeed[Index][LinkIndex] = (UINT8)KtiInternalGlobal->PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed;
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to normalize the phy/link layer options so that the values to be programmed at both ends of
  link is agreeable.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

**/
KTI_STATUS
NormalizeLinkOptions (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  BOOLEAN                  Override;
  UINT8                    Index, LinkIndex;
  KTI_LINK_DATA            *pPeerInfo;

  //
  // Normalize per link options. If the peers don't match their options, they will be normalized as follows:
  //   Disable - Force both the ends to disable.
  //   DfxCrcMode -   force the 16b CRC mode
  //   DfxL0pEnable - force L0p disabled
  //   DfxL1Enable -  force L1 disabled
  //   DfxKtiFailoverEn - force failover disabled
  //

  Override = FALSE;
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
        if (SocketData->Cpu[Index].LinkData[LinkIndex].Valid == TRUE) {
          pPeerInfo = &(SocketData->Cpu[Index].LinkData[LinkIndex]);
          if (KtiInternalGlobal->PhyLinkPerPortSetting[Index].Link[LinkIndex].KtiPortDisable !=
              KtiInternalGlobal->PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].KtiPortDisable) {
            KtiInternalGlobal->PhyLinkPerPortSetting[Index].Link[LinkIndex].KtiPortDisable = TRUE;
            KtiInternalGlobal->PhyLinkPerPortSetting[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].KtiPortDisable = TRUE;
            Override = TRUE;
          }
          if (KTISETUP->DfxCpuCfg[Index].Link[LinkIndex].DfxCrcMode != KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].DfxCrcMode) {
            KTISETUP->DfxCpuCfg[Index].Link[LinkIndex].DfxCrcMode = CRC_MODE_16BIT;
            KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].DfxCrcMode = CRC_MODE_16BIT;
            Override = TRUE;
          }
          if (KTISETUP->DfxCpuCfg[Index].Link[LinkIndex].DfxL0pEnable != KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].DfxL0pEnable) {
            KTISETUP->DfxCpuCfg[Index].Link[LinkIndex].DfxL0pEnable = KTI_DISABLE;
            KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].DfxL0pEnable = KTI_DISABLE;
            Override = TRUE;
          }
          if (KTISETUP->DfxCpuCfg[Index].Link[LinkIndex].DfxL1Enable != KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].DfxL1Enable) {
            KTISETUP->DfxCpuCfg[Index].Link[LinkIndex].DfxL1Enable = KTI_DISABLE;
            KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Link[pPeerInfo->PeerPort].DfxL1Enable = KTI_DISABLE;
            Override = TRUE;
          }
          if (KTISETUP->DfxCpuCfg[Index].Phy[LinkIndex].DfxKtiFailoverEn != KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].DfxKtiFailoverEn) {
            KTISETUP->DfxCpuCfg[Index].Phy[LinkIndex].DfxKtiFailoverEn = KTI_DISABLE;
            KTISETUP->DfxCpuCfg[pPeerInfo->PeerSocId].Phy[pPeerInfo->PeerPort].DfxKtiFailoverEn = KTI_DISABLE;
            Override = TRUE;
          }
        }
      }
    }
  }

  if (Override == TRUE) {
    KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_PER_LINK_OPTION_MISMATCH);
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:   One or more per Link option mismatch detected. Forcing to common setting.  \n"));
  }


  return KTI_SUCCESS;
}

/**
  Issue physical layer reset on KTI link.

  @param Cpu       - Socket ID
  @param Link      - Port number
  @param ResetType -

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
IssuePhyResetOnCpuLink (
  UINT8               Cpu,
  UINT8               Link,
  UINT8               ResetType
  )
{
  UINT8        WaitTryCount;
  KTI_STATUS   Status;
  BOOLEAN      Trained = FALSE;

  // Issue the phy layer reset with the reset modifier set to the given type
  SetCpReset (Cpu, Link);

  // Wait for the link to train
  for (WaitTryCount = 0; WaitTryCount <= 20; WaitTryCount++) {
    Trained = IsLinkTrained (Cpu, Link);
    if (Trained) {
      break;
    }
    KtiFixedDelay (100);
  }

  // Check if the link trained or not
  if (!Trained) {
    Status = KTI_FAILURE;
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Link failed to train after PhyReset."));
  } else {
    Status = KTI_SUCCESS;
  }

  return Status;
}

/**

  Locates the platform EPARAMS (redriver status for SPR) based on socket/link

  @param KtiInternalGlobal       - KTIRC internal variables.
  @param[in] SocketID            - SocId we are looking up
  @param[in] Link                - Link# on socket
  @param[in] Freq                - Link frequency
  @param[out] LinkSpeedParameter - return Ptr

  @retval Ptr to EPARAM table
  @retval KTI_SUCCESS if entry found
  @retval KTI_FAILURE if no entry found

**/
KTI_STATUS
LocateEparameter (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    SocketID,
  UINT8                    Link,
  UINT8                    Freq,
  VOID                     *LinkSpeedParameter
  )
{
  ALL_LANES_EPARAM_LINK_INFO           *AllLanesPtr;
  PER_LANE_EPARAM_LINK_INFO            *PerLanePtr;
  ALL_LANES_PRESET_COEFF_LINK_INFO     *AllLanesPresetCoeffInfoPtr;
  PER_LANE_PRESET_COEFF_LINK_INFO      *PerLanePresetCoeffInfoPtr;
  UINT32                               AllLanesSizeOfTable;
  UINT32                               PerLaneSizeOfTable;
  UINT32                               AllLanesPersetCoeffSizeOfTable;
  UINT32                               PerLanePersetCoeffSizeOfTable;
  PER_LANE_EPARAM_LINK_INFO            TempPerLaneData;
  PER_LANE_PRESET_COEFF_INFO           TempPerLanePresetCoeffData;
  BOOLEAN                              InternalPort;

  AllLanesPtr = NULL;
  AllLanesSizeOfTable = 0;
  PerLanePtr = NULL;
  PerLaneSizeOfTable = 0;
  AllLanesPresetCoeffInfoPtr = NULL;
  AllLanesPersetCoeffSizeOfTable = 0;
  PerLanePresetCoeffInfoPtr = NULL;
  PerLanePersetCoeffSizeOfTable = 0;

  ZeroMem (&TempPerLaneData, sizeof(PER_LANE_EPARAM_LINK_INFO));
  ZeroMem (&TempPerLanePresetCoeffData, sizeof(PER_LANE_PRESET_COEFF_INFO));

  InternalPort = IsInternalUpiPort(KtiInternalGlobal, SocketID, Link);

  if (InternalPort) {
    GetInternalEparameterTable (&AllLanesPtr, &AllLanesSizeOfTable, &PerLanePtr, &PerLaneSizeOfTable);
  } else {
    if (IsSprFamilyCpu (KtiInternalGlobal->CpuType) || IsGnrSrfFamilyCpu (KtiInternalGlobal->CpuType)) {
      //
      // Get the All Lane Preset coeff table for the UPI links
      //
      AllLanesPresetCoeffInfoPtr = (ALL_LANES_PRESET_COEFF_LINK_INFO *)(UINTN)KTISETUP->AllLanesPtr;
      AllLanesPersetCoeffSizeOfTable = KTISETUP->AllLanesSizeOfTable;
      //
      // Get the Per Lane Preset coeff table for the UPI links
      //
      PerLanePresetCoeffInfoPtr = (PER_LANE_PRESET_COEFF_LINK_INFO *)(UINTN)KTISETUP->PerLanePtr;
      PerLanePersetCoeffSizeOfTable = KTISETUP->PerLaneSizeOfTable;
    } else {
      //
      // Find per bundle Table first
      //
      AllLanesSizeOfTable = KTISETUP->AllLanesSizeOfTable;
      AllLanesPtr = (ALL_LANES_EPARAM_LINK_INFO *)(UINTN)KTISETUP->AllLanesPtr;
      //
      // Find per lane Table
      //
      PerLaneSizeOfTable = KTISETUP->PerLaneSizeOfTable;
      PerLanePtr = (PER_LANE_EPARAM_LINK_INFO *)(UINTN)KTISETUP->PerLanePtr;
    }

  }

  if ((AllLanesSizeOfTable != 0) || (PerLaneSizeOfTable != 0)) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  S%u P%u is %a port. All lane table size: 0x%0x; Per lane table size: 0x%0x\n",
      SocketID, Link,  (InternalPort == TRUE) ? "Internal" : "External", AllLanesSizeOfTable, PerLaneSizeOfTable));
  } else {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  S%u P%u's Preset Coeff table, all lane table size: 0x%0x; per lane table size: 0x%0x\n",
      SocketID, Link, AllLanesPersetCoeffSizeOfTable, PerLanePersetCoeffSizeOfTable));
  }

  while ((AllLanesSizeOfTable != 0) && (AllLanesPtr != NULL)) {
    if ((AllLanesPtr->SocketID == SocketID) && ((AllLanesPtr->Freq & (1 << Freq)) != 0) && ((AllLanesPtr->Link & (1 << Link)) != 0)) {
      //
      //Convert to per lane data structure format from all lanes data structure
      //
      TempPerLaneData.SocketID = AllLanesPtr->SocketID;
      TempPerLaneData.AllLanesUseSameTxeq = ALL_LANES_TXEQ_ENABLED;
      TempPerLaneData.Freq = AllLanesPtr->Freq;
      TempPerLaneData.Link = AllLanesPtr->Link;
      TempPerLaneData.TXEQL[0] = AllLanesPtr->AllLanesTXEQ;
      TempPerLaneData.CTLEPEAK[0] = AllLanesPtr->CTLEPEAK;
      CopyMem ((UINT8*)LinkSpeedParameter, (UINT8*) &TempPerLaneData, sizeof (PER_LANE_EPARAM_LINK_INFO));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Full Speed EPARAM entry located for Socket %u Link %u", SocketID, Link));
      return KTI_SUCCESS;
    }

    AllLanesPtr++;
    AllLanesSizeOfTable--;
  }

  while ((PerLaneSizeOfTable != 0) && (PerLanePtr != NULL)) {
    if ((PerLanePtr->SocketID == SocketID) && ((PerLanePtr->Freq & (1 << Freq)) != 0) && ((PerLanePtr->Link & (1 << Link)) != 0)) {
      CopyMem ((UINT8*)LinkSpeedParameter, (UINT8*)PerLanePtr, sizeof (PER_LANE_EPARAM_LINK_INFO));
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Per Lane Full Speed EPARAM entry located for Socket %u Link %u", SocketID, Link));
      return KTI_SUCCESS;
    }

    PerLanePtr++;
    PerLaneSizeOfTable--;
  }

  //
  // For the Per Lane Preset Coeff (PH_TCL[x]) has the higher priority than All lane preset coeff (PH_TCC), if there is a
  // set of Per lane preset coeff valid, use them and don't care the all lane preset coeff.
  //
  while ((PerLanePersetCoeffSizeOfTable != 0) && (PerLanePresetCoeffInfoPtr != NULL)) {
    if ((PerLanePresetCoeffInfoPtr->SocketID == SocketID) && ((PerLanePresetCoeffInfoPtr->Link & (1 << Link)) != 0)) {
      CopyMem ((UINT8*)LinkSpeedParameter, (UINT8*)&PerLanePresetCoeffInfoPtr->LinkPerLanePresetCoeff[Freq], sizeof (PER_LANE_PRESET_COEFF_INFO));
      KtiInternalGlobal->UpiRedriverStatus[SocketID] |= (PerLanePresetCoeffInfoPtr->RedriverStatus << Link);
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Per Lane Preset coeffcients Index table located for Socket %u Link %u, RedriverStatus: %x",
                           SocketID, Link, PerLanePresetCoeffInfoPtr->RedriverStatus));
      return KTI_SUCCESS;
    }

    PerLanePresetCoeffInfoPtr++;
    PerLanePersetCoeffSizeOfTable--;
  }

  while ((AllLanesPersetCoeffSizeOfTable != 0) && (AllLanesPresetCoeffInfoPtr != NULL)) {
    if ((AllLanesPresetCoeffInfoPtr->SocketID == SocketID) && ((AllLanesPresetCoeffInfoPtr->Link & (1 << Link)) != 0)) {
      TempPerLanePresetCoeffData.AllLanesSafeCoeffFlag = TRUE;
      TempPerLanePresetCoeffData.AllLanesPresetCoeff = AllLanesPresetCoeffInfoPtr->LinkAllLanesPresetCoeff[Freq].AllLanesPresetCoeff;
      CopyMem ((UINT8*)LinkSpeedParameter, (UINT8*)&TempPerLanePresetCoeffData, sizeof (PER_LANE_PRESET_COEFF_INFO));
      KtiInternalGlobal->UpiRedriverStatus[SocketID] |= (AllLanesPresetCoeffInfoPtr->RedriverStatus << Link);
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n All Lanes Preset coeffcients Index table located for Socket %u Link %u, RedriverStatus: %x",
                           SocketID, Link, AllLanesPresetCoeffInfoPtr->RedriverStatus));
      return KTI_SUCCESS;
    }

    AllLanesPresetCoeffInfoPtr++;
    AllLanesPersetCoeffSizeOfTable--;
  }

  return KTI_FAILURE;
}

/**
  Programs Phy layer Electrical Parameters and get the UPI redriver status (if SPR)

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Index             - socket Index
  @param LinkIndex         - qpi port number

**/
KTI_STATUS
ProgramEparams (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  UINT8                      Index,
  UINT8                      LinkIndex,
  UINT8                      Group
  )
{
  UINT8      LinkSpeedParameter[128];
  UINT8      LinkSpeed = 0;
  KTI_STATUS Status;

  if (Group == KTI_GROUP) {
    LinkSpeed = (UINT8)KTIVAR->OutPerLinkSpeed[Index][LinkIndex];
  }

  //
  // Search in policy eparameter table for the correct EPARAM based on Socket, Link, Freq, Platform
  //

  Status = LocateEparameter (KtiInternalGlobal, Index, LinkIndex, LinkSpeed, &LinkSpeedParameter);

  if (Status != KTI_SUCCESS) {
    //
    // No entry found at all!!
    //
    KtiCheckPoint ((UINT8)(1 << Index), SOCKET_TYPE_CPU, (UINT8)(1 << LinkIndex), ERR_FULL_SPEED_TRANSITION, MINOR_ERR_KTI_ELEC_PARAM_NOT_FOUND);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\n***** KTI Electrical Parameters for CPU%u Link %u is not found. Please check TxEq table.****\n", Index, LinkIndex));
    KTI_ASSERT (FALSE, ERR_FULL_SPEED_TRANSITION, MINOR_ERR_KTI_ELEC_PARAM_NOT_FOUND);
  }

  ConfigSocketLinkEparams (Index, LinkIndex, &LinkSpeedParameter);

  return KTI_SUCCESS;
}

/**
  Programs PHY layer registers

  @param KtiInternalGlobal - KTIRC internal variables.
  @param Socket            - socket Index
  @param LinkIndex         - qpi port number

**/
KTI_STATUS
ProgramPhyLayerControlRegisters (
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_SOCKET_DATA           *SocketData,
  UINT8                     Socket,
  UINT8                     LinkIndex,
  UINT8                     Group
  )
{
  KTI_DFX_CPU_LINK_SETTING                 *pCpuLinkSetting;
  UINT8                                    CpuType;
  BOOLEAN                                  ProgramCNumInit;
  BOOLEAN                                  ProgramCOpSpeed;
  UINT32                                   c_l0p_en;
  UINT32                                   c_l1_en;
  UINT32                                   c_failover_en;
  UINT32                                   c_numinit;
  BOOLEAN                                  RedriverStatus;

  pCpuLinkSetting = (KTI_DFX_CPU_LINK_SETTING *) &(KTISETUP->DfxCpuCfg[Socket].Link[LinkIndex]);

  //
  // Setup L0p, L1 and failover based on output variables and DFX per-link overrides
  //
  c_l0p_en = KTIVAR->OutKtiLinkL0pEn;
  if (pCpuLinkSetting->DfxL0pEnable < KTI_AUTO) {
    c_l0p_en = pCpuLinkSetting->DfxL0pEnable;
  }

  CpuType     = KtiInternalGlobal->CpuType;

  //
  // s5370781: phy resets occur with L0p enabled
  //
  SetKtiL0pExitWorkaround (Socket, LinkIndex, KTIVAR->KtiCurrentLinkSpeedMode);

  c_l1_en = KTIVAR->OutKtiLinkL1En;
  if (pCpuLinkSetting->DfxL1Enable < KTI_AUTO) {
    c_l1_en = pCpuLinkSetting->DfxL1Enable;
  }
  KTIVAR->OutKtiPerLinkL1En[Socket][LinkIndex] = (BOOLEAN)c_l1_en;

  c_failover_en = KTIVAR->OutKtiFailoverEn;
  if (KTISETUP->DfxCpuCfg[Socket].Phy[LinkIndex].DfxKtiFailoverEn < KTI_AUTO) {
    c_failover_en = KTISETUP->DfxCpuCfg[Socket].Phy[LinkIndex].DfxKtiFailoverEn;
  }

  ProgramCNumInit = FALSE;
  c_numinit = 0;
  if (KTISETUP->DfxParm.DfxKtiMaxInitAbort < KTI_AUTO) {
    ProgramCNumInit = TRUE;
    c_numinit = KTISETUP->DfxParm.DfxKtiMaxInitAbort;
  }

  ProgramCOpSpeed = TRUE;
  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    ProgramCOpSpeed = FALSE;
  }

  ProgramKtiReutPhCtr1 (Socket, LinkIndex, ProgramCNumInit, ProgramCOpSpeed, c_l0p_en, c_l1_en, c_failover_en, c_numinit);

  //
  // Get the input UPI redriver status from eparam table (SPR, the other projects don't need this setting),
  // Don't have the way to detect it on SPR.
  //
  RedriverStatus = FALSE;
  if ((KtiInternalGlobal->UpiRedriverStatus[Socket] & (1 << LinkIndex)) != 0) {
    RedriverStatus = TRUE;
  }
  ProgramKtiReutPhCtr2 (Socket, LinkIndex, RedriverStatus);

  //
  // Physical Determinism Control
  //
  SetPhysicalDeterminismControl (Socket, LinkIndex);

  //
  // Force Phy Layer readapt before full-speed transition
  //
  ForcePhyLayerReadapt (Socket, LinkIndex);

  return KTI_SUCCESS;
}

/**
  Gathers common Link and Phy Layer Capabilities Registers for all valid links.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param KtiLcp            - Pointer to the KTILCP struct
  @param KtiReutPhCpr      - Pointer to the KTIREUT struct
  @param CrcMode           - Pointer to CRC Mode capability variable (anded capability bits of all KTI ports on all sockets)
  @param Vn1Supported      - Pointer to VN1 Supported capability variable (anded capability bits of all KTI ports on all sockets)
  @param L0pSupport        - Pointer to L0p Supported capability variable (anded capability bits of all KTI ports on all sockets)
  @param L1Support         - Pointer to L1 Supported capability variable (anded capability bits of all KTI ports on all sockets)
  @param FailoverSupport   - Pointer to Failover Supported capability variable (anded capability bits of all KTI ports on all sockets)

  @retval KTI_SUCCESS
**/
KTI_STATUS
GatherCommonKtiCapabilities (
  KTI_HOST_INTERNAL_GLOBAL       *KtiInternalGlobal,
  UINT32                         *CrcMode,
  UINT32                         *Vn1Supported,
  UINT32                         *L0pSupport,
  UINT32                         *L1Support,
  UINT32                         *FailoverSupport
  )
{
  UINT8  Index;
  UINT8  LinkIndex;
  UINT32 ccl0p;
  UINT32 ccl1;
  UINT32 ccfailover;

  *CrcMode           = 0xffffffff;
  *Vn1Supported      = 0xffffffff;
  *L0pSupport        = 0xffffffff;
  *L1Support         = 0xffffffff;
  *FailoverSupport   = 0xffffffff;

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (!SocketPresent (Index)) {
      continue;
    }
    for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
      if ((KTIVAR->CpuInfo[Index].LepInfo[LinkIndex].Valid == TRUE) || (KTISETUP->KtiLbEn)) {
        *Vn1Supported    &= GetVn1Supported (Index, LinkIndex);
        if (IsUpiPortUpimdf (Index, LinkIndex)) {
          //
          // The port is UPIMDF subsystem, skip the phy accessing
          //
          continue;
        }
        *CrcMode         &= GetCrcMode (Index, LinkIndex);
        GetKtiReutCapabilities (Index, LinkIndex, &ccl0p, &ccl1, &ccfailover);
        *L0pSupport      &= ccl0p;
        *L1Support       &= ccl1;
        *FailoverSupport &= ccfailover;
      }
    }
  }

  return KTI_SUCCESS;
}
