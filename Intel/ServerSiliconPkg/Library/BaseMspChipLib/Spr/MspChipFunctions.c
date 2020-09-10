/** @file
  MspChipFunctions.c
  MSP chip specific functions.

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

#include "SysHost.h"
#include "MspLibraries.h"
#include <Library/MspCoreLib.h>
#include "Spr/Ddrio/Include/MMRCPROJECT_DEFINITIONS_GENERATED_DDR.h"
#include "Spr/Ddrio/Include/MMRCPROJECT_DATA_GENERATED_DDR.h"
#include "MspData.h"
#include <Library/SystemInfoLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/OdtTableLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>

//*****************************External Arrays for DDR***********************//
extern TASK_DESCRIPTOR InitTasks_DDR[];
extern CHAR8 *OutputStrings_DDR[];
#if CAPSULESTRINGS_DDR
  extern CHAR8 *CapsuleStrings_DDR[];
#endif // CAPSULESTRINGS
extern Register Registers_DDR[];
extern CONST SUBTASK_INDEX_OFFSET SubtaskOffsetIndex_DDR[MAX_SUBTASKS_DDR + 1];
extern CONST UINT8 InitData_DDR[];

extern INSTANCE_PORT_MAP InstancePortMapGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_BOXES_DDR];
extern CONST FLOORPLAN FloorPlanGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_CHANNELS_DDR][MAX_STROBES_DDR];
extern CONST INT16 StrobeToRegisterGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_STROBES_DDR][2];
extern CONST INT16 RankToRegisterGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_RANKS_DDR][2];
#if CAPSULESTRINGS_DDR
  extern CONST UINT8 UnitStringsGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_BOXES_DDR][MAX_BOXNAME_LENGTH_DDR];
#endif // CAPSULESTRINGS

extern INSTANCE_PORT_MAP InstancePortMapGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_BOXES_DDR];
extern INSTANCE_PORT_OFFSET InstancePortOffsetGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_INSTANCE_PORTS_DDR];
extern UINT8 ChannelToInstanceMapGenerated_DDR[MAX_BLUEPRINTS_DDR][MAX_BOXES_DDR][MAX_CHANNELS_DDR];
extern MSP_STATUS DynamicAssignmentGenerated_DDR(MSP_DATA *, UINT32 *, UINT8, UINT16, UINT32 *);

/**
  This routine converts a PFCT value from BITx to x.
  The Current* variables must not be in BITx format. For example
  if CurrentPlatform is set to BIT4, this routine sets
  CurrentPlatform to 4 instead.

  @param[in, out]  MspData         Host structure for all data related to Msp.
  @retval          NONE
**/
VOID
ConvertPfctFromBitXToX (
  IN        MSP_DATA         *MspData
  );

/**
  This routine is responsible for populating all dynamic variables in the spreadsheet with
  actual values to be programmed into the registers. Any translation required between values
  and register settings must happen here too. For example, if we have a DIMM with CAS latency
  of 11, but the memory controller's register needs to be programmed to "2" for TCL of 11, this
  routine would populate the dynamic variable for CAS latency to 2.

  @param[in]    Host      Pointer to the system Host (root) structure

  @retval       SUCCESS   Variables were populated

**/
UINT32
PopulateDynamicVariablesSprDdr (
  IN  OUT   PSYSHOST    Host,
  IN  OUT   MSP_DATA    *MspData
  )
{
  UINT8                       Socket;
  UINT8                       Channel;
  struct channelNvram         (*ChannelNvList)[MAX_CH];
  struct dimmNvram            (*DimmNvList)[MAX_DIMM];
  SYS_SETUP                   *Setup;
  UINT8                       MaxChDdr;
  UINT8                       AnalogTuningData;
  UINT16                      McVref;
  UINT16                      Vccd;
  UINT16                      VLow;
  UINT16                      VHigh;

  MaxChDdr = GetMaxChDdr ();
  Setup    = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Given values from design
  //
  Vccd  = 1100;
  VLow  = Vccd / 2;
  VHigh = Vccd;

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\n: Create Dynamic Variables for Populated Channels %d:\n", Channel);
    if (Channel >= MAX_CHANNELS_DDR) {
      break;
    }

    if (!GetMcVrefValueOdt (Socket, Channel, &McVref)) {
      continue;
    }

    RcDebugPrintWithDevice(
      SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "McVref = %d\n",
      McVref
    );

    McVref = ODT_TABLE_LIB_ROUND_VREF_PERCENT (McVref);
    RcDebugPrintWithDevice(
      SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ODT_TABLE_LIB_ROUND_VREF_PERCENT(McVref) = %d\n",
      McVref
    );

    DYNAMIC_VARS (Channel, RXVREF_DATA_DDR) = (((McVref * Vccd) / 100 - VLow) * 127) / (VHigh - VLow) + 1;

    RcDebugPrintWithDevice (
      SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RXVREF_DATA = %d\n",
      DYNAMIC_VARS (Channel, RXVREF_DATA_DDR)
      );

    if (FeaturePcdGet (PcdCteBuild) || FeaturePcdGet (PcdCosimBuild)) {
      DYNAMIC_VARS (Channel, IS_NOT_CTE_COSIM_DDR) = 0;
    } else {
      DYNAMIC_VARS (Channel, IS_NOT_CTE_COSIM_DDR) = 1;
    }

    RcDebugPrintWithDevice (
      SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_NOT_CTE_COSIM_DDR = %d\n",
      DYNAMIC_VARS (Channel, IS_NOT_CTE_COSIM_DDR)
      );

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    if ((*DimmNvList)[0].DcpmmPresent == 1) {
      DYNAMIC_VARS (Channel, IS_AEP_SLOT0_DDR) = 1;
    } else {
      DYNAMIC_VARS (Channel, IS_AEP_SLOT0_DDR) = 0;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM0, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_PMEM_SLOT0_DDR = %d\n", DYNAMIC_VARS (Channel, IS_AEP_SLOT0_DDR));

    if ((*DimmNvList)[1].DcpmmPresent == 1) {
      DYNAMIC_VARS (Channel, IS_AEP_SLOT1_DDR) = 1;
    } else {
      DYNAMIC_VARS (Channel, IS_AEP_SLOT1_DDR) = 0;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM1, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_PMem_SLOT1_DDR = %d\n", DYNAMIC_VARS (Channel, IS_AEP_SLOT1_DDR));

    if (((*DimmNvList)[0].DcpmmPresent == 1) || ((*DimmNvList)[1].DcpmmPresent == 1)) {
      DYNAMIC_VARS (Channel, IS_AEP_ANYSLOT_DDR) = 1;
    } else {
      DYNAMIC_VARS (Channel, IS_AEP_ANYSLOT_DDR) = 0;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_PMem_ANYSLOT_DDR = %d\n", DYNAMIC_VARS (Channel, IS_AEP_ANYSLOT_DDR));

    DYNAMIC_VARS (Channel, IS_HVM_MODE_DDR)     = 0;
    DYNAMIC_VARS (Channel, IS_NOT_HVM_MODE_DDR) = 1;

    if (IsHvmModeEnable ()) {
      DYNAMIC_VARS (Channel, IS_HVM_MODE_DDR)     = 1;
      DYNAMIC_VARS (Channel, IS_NOT_HVM_MODE_DDR) = 0;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_HVM_MODE_DDR = %d // IS_NOT_HVM_MODE_DDR = %d\n",
      DYNAMIC_VARS (Channel, IS_HVM_MODE_DDR),
      DYNAMIC_VARS (Channel, IS_NOT_HVM_MODE_DDR)
      );

    if (Host->nvram.mem.dimmTypePresent == UDIMM) {
      DYNAMIC_VARS (Channel, IS_NON_UDIMM_ANYSLOT_DDR) = 0;
      DYNAMIC_VARS (Channel, IS_UDIMM_ANYSLOT_DDR) = 1;
      if ((*DimmNvList)[0].dimmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_UDIMM_SLOT0_DDR) = 1;
      }
      if ((*DimmNvList)[1].dimmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_UDIMM_SLOT1_DDR) = 1;
      }
    } else {
      DYNAMIC_VARS (Channel, IS_NON_UDIMM_ANYSLOT_DDR) = 1;
      DYNAMIC_VARS (Channel, IS_UDIMM_ANYSLOT_DDR) = 0;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_NON_UDIMM_ANYSLOT_DDR = %d\n", DYNAMIC_VARS (Channel, IS_NON_UDIMM_ANYSLOT_DDR));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_UDIMM_ANYSLOT_DDR = %d\n", DYNAMIC_VARS (Channel, IS_UDIMM_ANYSLOT_DDR));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_UDIMM_SLOT0_DDR = %d\n", DYNAMIC_VARS (Channel, IS_UDIMM_SLOT0_DDR));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_UDIMM_SLOT1_DDR = %d\n", DYNAMIC_VARS (Channel, IS_UDIMM_SLOT1_DDR));

    if (Channel % 2) {
      //
      // Local Channel 1 in Memory Subsystem
      //

      if ((*DimmNvList)[0].DcpmmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_CH1_AEP_SLOT0_DDR) = 1;
      } else {
        DYNAMIC_VARS (Channel, IS_CH1_AEP_SLOT0_DDR) = 0;
      }

      if ((*DimmNvList)[1].DcpmmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_CH1_AEP_SLOT1_DDR) = 1;
      } else {
        DYNAMIC_VARS (Channel, IS_CH1_AEP_SLOT1_DDR) = 0;
      }

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM0, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_CH1_PMem_SLOT0_DDR = %d\n", DYNAMIC_VARS (Channel, IS_CH1_AEP_SLOT0_DDR));

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM1, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_CH1_PMem_SLOT1_DDR = %d\n", DYNAMIC_VARS (Channel, IS_CH1_AEP_SLOT1_DDR));

    } else {
      //
      // Local Channel 0 in Memory Subsystem
      //
      if ((*DimmNvList)[0].DcpmmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_CH0_AEP_SLOT0_DDR) = 1;
      } else {
        DYNAMIC_VARS (Channel, IS_CH0_AEP_SLOT0_DDR) = 0;
      }

      if ((*DimmNvList)[1].DcpmmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_CH0_AEP_SLOT1_DDR) = 1;
      } else {
        DYNAMIC_VARS (Channel, IS_CH0_AEP_SLOT1_DDR) = 0;
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM0, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_CH0_PMem_SLOT0_DDR = %d\n", DYNAMIC_VARS (Channel, IS_CH0_AEP_SLOT0_DDR));

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM1, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "IS_CH0_PMem_SLOT1_DDR = %d\n", DYNAMIC_VARS (Channel, IS_CH0_AEP_SLOT1_DDR));
    }

    DYNAMIC_VARS (Channel, IS_X8_OR_X16_DEVICE_SLOT0_DDR) = 0;
    DYNAMIC_VARS (Channel, IS_X8_OR_X16_DEVICE_SLOT1_DDR) = 0;
    if ((*DimmNvList)[0].dimmPresent == 1) {
      if ((*DimmNvList)[0].x4Present == 0) {
        DYNAMIC_VARS (Channel, IS_X8_OR_X16_DEVICE_SLOT0_DDR) = 1;
      }
    }
    if ((*DimmNvList)[1].dimmPresent == 1) {
      if ((*DimmNvList)[1].x4Present == 0) {
        DYNAMIC_VARS (Channel, IS_X8_OR_X16_DEVICE_SLOT1_DDR) = 1;
      }
    }

    DYNAMIC_VARS (Channel, IS_X16_DEVICE_SLOT0_DDR) = 0;
    DYNAMIC_VARS (Channel, IS_X16_DEVICE_SLOT1_DDR) = 0;
    if ((*ChannelNvList)[Channel].features & X16_PRESENT) {
      if ((*DimmNvList)[0].dimmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_X16_DEVICE_SLOT0_DDR) = 1;
      }
      if ((*DimmNvList)[1].dimmPresent == 1) {
        DYNAMIC_VARS (Channel, IS_X16_DEVICE_SLOT1_DDR) = 1;
      }
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM0, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "IS_X8_OR_X16_DEVICE_SLOT0_DDR = %d\n", DYNAMIC_VARS (Channel, IS_X8_OR_X16_DEVICE_SLOT0_DDR));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM1, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "IS_X8_OR_X16_DEVICE_SLOT1_DDR = %d\n", DYNAMIC_VARS (Channel, IS_X8_OR_X16_DEVICE_SLOT1_DDR));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM0, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "IS_X16_DEVICE_SLOT0_DDR = %d\n", DYNAMIC_VARS (Channel, IS_X16_DEVICE_SLOT0_DDR));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, DIMM1, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "IS_X16_DEVICE_SLOT1_DDR = %d\n", DYNAMIC_VARS (Channel, IS_X16_DEVICE_SLOT1_DDR));
    if ((GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_PXC_TRAINING)) || (Setup->mem.ForcePxcInit != 0)) {
      DYNAMIC_VARS (Channel, PXC_VALUE_DDR) = 1;
    } else {
      DYNAMIC_VARS (Channel, PXC_VALUE_DDR) = 0;
    }
    RcDebugPrint (SDBG_MAX, "PXC_VALUE = %d\n", DYNAMIC_VARS(Channel, PXC_VALUE_DDR));

    if (GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_DDJC_TRAINING)) {
      DYNAMIC_VARS (Channel, DDJC_VALUE_DDR) = 1;
    } else {
      DYNAMIC_VARS (Channel, DDJC_VALUE_DDR) = 0;
    }
    RcDebugPrint (SDBG_MAX, "DDJC_VALUE = %d\n", DYNAMIC_VARS(Channel, DDJC_VALUE_DDR));

    DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_0_DDR) = 0;
    DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_1_DDR) = 0;
    DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_2_DDR) = 0;
    DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_3_DDR) = 0;
    AnalogTuningData = GetAnalogTuningValue (Host, Socket, Channel);
    if (AnalogTuningData == 0) {
      DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_0_DDR) = 1;
    } else if (AnalogTuningData == 1) {
      DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_1_DDR) = 1;
    } else if (AnalogTuningData == 2) {
      DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_2_DDR) = 1;
    }else if (AnalogTuningData == 3) {
      DYNAMIC_VARS (Channel, IS_ANALOG_TUNING_3_DDR) = 1;
    }
    RcDebugPrint (SDBG_MAX, "IS_ANALOG_TUNING_0_DDR = %d\n", DYNAMIC_VARS(Channel, IS_ANALOG_TUNING_0_DDR));
    RcDebugPrint (SDBG_MAX, "IS_ANALOG_TUNING_1_DDR = %d\n", DYNAMIC_VARS(Channel, IS_ANALOG_TUNING_1_DDR));
    RcDebugPrint (SDBG_MAX, "IS_ANALOG_TUNING_2_DDR = %d\n", DYNAMIC_VARS(Channel, IS_ANALOG_TUNING_2_DDR));
    RcDebugPrint (SDBG_MAX, "IS_ANALOG_TUNING_3_DDR = %d\n", DYNAMIC_VARS(Channel, IS_ANALOG_TUNING_3_DDR));
  } // Channel loop

  return SUCCESS;
}

/**
  Function to setup MSP parameters for ICX-DDR

  @param[in]      Host                      Pointer to the system Host (root) structure
  @param[in,out]  ExecuteOnThisChannel      Structure to indicate which channels should run
  @param[in,out]  ChannelEnabled            Array that indicates which channels are enabled
  @param[in,out]  CurrentPlatform           Array that indicates current platform
  @param[in,out]  CurrentFrequency          Array that indicates current frequency
  @param[in,out]  CurrentConfiguration      Array that indicates current configuration
  @param[in,out]  CurrentTechnology         Array that indicates current technology
  @param[in,out]  DynamicVars               Array that indicates dynamic variables

**/
VOID
PopulateMspDdr(
  IN  OUT   PSYSHOST    Host,
  IN  OUT   MSP_DATA    *MspData,
  IN  OUT   BOOLEAN     ExecuteOnThisChannel[MAX_CHANNELS_DDR],
  IN  OUT   BOOLEAN     ChannelEnabled[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentPlatform[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentFrequency[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentConfiguration[MAX_CHANNELS_DDR],
  IN  OUT   UINT16      CurrentTechnology[MAX_CHANNELS_DDR],
  IN  OUT   UINT32      DynamicVars[MAX_CHANNELS_DDR][DYNVAR_MAX_DDR]
  )
{
  UINT8     Channel;
  UINT8     MemBarOffset;
  UINT32    RelativeBarOffset;
  BootMode  SysBootMode;

  SysBootMode = GetSysBootMode ();

  MspSet (ExecuteOnThisChannel, 0, MAX_CHANNELS_DDR * sizeof(BOOLEAN));
  MspSet (ChannelEnabled, 0, MAX_CHANNELS_DDR * sizeof(BOOLEAN));
  MspSet (CurrentPlatform, 0, MAX_CHANNELS_DDR * sizeof(UINT16));
  MspSet (CurrentFrequency, 0, MAX_CHANNELS_DDR * sizeof(UINT16));
  MspSet (CurrentConfiguration, 0, MAX_CHANNELS_DDR * sizeof(UINT16));
  MspSet (CurrentTechnology, 0, MAX_CHANNELS_DDR * sizeof(UINT16));
  MspSet (DynamicVars, 0, MAX_CHANNELS_DDR * DYNVAR_MAX_DDR * sizeof(UINT32));

  //
  // Detect the needed blueprint
  //
  MspData->CurrentBlueprint = BLUEPRINT_SPR_DDR;
  //
  // Translate the bootmode and subbootmode to MMRC-known bootmodes.
  //
  if (SysBootMode == S3Resume || Host->var.mem.subBootMode == WarmBootFast) {
    MspData->BootMode = S3_DDR;
  } else {
    if (Host->var.mem.subBootMode == ColdBootFast) {
      MspData->BootMode = FB_DDR;
    } else if (Host->var.mem.subBootMode == WarmBoot) {
      MspData->BootMode = S0_DDR;
    } else if (((SUB_BOOT_MODE)SysBootMode) == ColdBoot) {
      MspData->BootMode = S5_DDR;
    } else if (Host->var.mem.subBootMode == AdrResume) {
      MspData->BootMode = S3_ADR_DDR;
    } else {
      //
      // Default to cold boot.
      //
      MspData->BootMode = S5_DDR;
    }
  }

  //
  // Needs to be populated with actual values.
  //
  for (Channel = 0; Channel < MAX_TOTAL_CHANNEL; Channel++) {
    if (Channel >= MAX_CH) {
      ChannelEnabled[Channel] = FALSE;
      continue;
    }
    if (Channel >= MAX_CHANNELS_DDR) {
      break;
    }
    if (Host->nvram.mem.socket[Host->var.mem.currentSocket].channelList[Channel].enabled == 1) {
      ChannelEnabled[Channel] = TRUE;
    }
    //
    // Determine the MMIO BAR
    // Memory Sub-System MEMBAR_DDR[31:28]
    // Logical Bus DDR/MC MEMBAR_DDR[27:20]
    //
    MemBarOffset = TYPE_MEM_BAR0;

    //
    // Each MMIO BAR for DDRIO represent a unique Memory Subsystem (MemSs),
    // but current architecture only has define 2 DDRIO MemSs.
    // So the equation below will only target the 2 existing MMIO BAR for DDRIO
    //
    RelativeBarOffset = Channel / MAX_CH_PER_MEMSS;

    DynamicVars[Channel][MEMBAR_DDR]  = KTI_HOST_OUT_PTR->MmioBar[Host->var.mem.currentSocket][RelativeBarOffset + MemBarOffset];
    DynamicVars[Channel][MEMBAR_MCMISC_DDR]  = KTI_HOST_OUT_PTR->MmioBar[Host->var.mem.currentSocket][RelativeBarOffset + MemBarOffset];
    DynamicVars[Channel][MEMBAR_DDR] += 0x00010000;
    //
    // Platform Type
    //
    if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
      CurrentPlatform[Channel] = P_A0_DDR;
    } else if (IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
      CurrentPlatform[Channel] = P_B0_DDR;
    } else {
      CurrentPlatform[Channel] = P_SIM_DDR;
    }

    //
    // Force CTE and CoSim to assume SIM platform
    //
    if (FeaturePcdGet (PcdCteBuild) || FeaturePcdGet (PcdCosimBuild)) {
      CurrentPlatform[Channel] = P_SIM_DDR;
    }

    switch (Host->nvram.mem.socket[Host->var.mem.currentSocket].ddrFreq) {
      case DDR_1866:
        CurrentFrequency[Channel] = F_1866_DDR;
        break;
      case DDR_2133:
        CurrentFrequency[Channel] = F_2133_DDR;
        break;
      case DDR_2400:
        CurrentFrequency[Channel] = F_2400_DDR;
        break;
      case DDR_2666:
        CurrentFrequency[Channel] = F_2666_DDR;
        break;
      case DDR_2933:
        CurrentFrequency[Channel] = F_2933_DDR;
        break;
      case DDR_3200:
        CurrentFrequency[Channel] = F_3200_DDR;
        break;
      case DDR_3600:
        CurrentFrequency[Channel] = F_3600_DDR;
        break;
      case DDR_4000:
        CurrentFrequency[Channel] = F_4000_DDR;
        break;
      case DDR_4400:
        CurrentFrequency[Channel] = F_4400_DDR;
        break;
      case DDR_4800:
        CurrentFrequency[Channel] = F_4800_DDR;
        break;
      case DDR_5200:
        CurrentFrequency[Channel] = F_5200_DDR;
        break;
      case DDR_5600:
        CurrentFrequency[Channel] = F_5600_DDR;
        break;
      case DDR_6000:
        CurrentFrequency[Channel] = F_6000_DDR;
        break;
      case DDR_6400:
        CurrentFrequency[Channel] = F_6400_DDR;
        break;
      case DDR_8400:
        CurrentFrequency[Channel] = F_8400_DDR;
        break;
      default:
        CurrentFrequency[Channel] = F_1866_DDR;
        break;
    }
    //
    // Add support for other CPU types here (ICX, EX-CPUs if sharing the same MRC, etc).
    //
    CurrentConfiguration[Channel] = C_Spr_DDR;
    if (IsDdr5Present (Host, Host->var.mem.currentSocket)) {
      CurrentTechnology[Channel] = T_DDR5_DDR;
    } else {
      CurrentTechnology[Channel] = T_DDR4_DDR;
    }
  }
  InitializeMspDataPointers (
    MspData,

    MAX_BLUEPRINTS_DDR,
    MAX_CHANNELS_DDR,
    DYNVAR_MAX_DDR,
    MAX_BOXES_DDR,
    MAX_STROBES_DDR,
    MAX_RANKS_DDR,

    NUM_TYPE_DDR,
    NUM_CONF_DDR,
    NUM_FREQ_DDR,
    NUM_PLAT_DDR,

    MAX_INSTANCE_PORTS_DDR,
    MAX_BOXNAME_LENGTH_DDR,
    MAX_REGISTER_NAME_LENGTH_DDR,
    FALSE,

#if CAPSULESTRINGS_DDR
    (UINT8 **)CapsuleStrings_DDR,
    (UINT8 **)OutputStrings_DDR,
    (UINT8 *)UnitStringsGenerated_DDR,
    TRUE,
#else
    (UINT8 **)NULL,
    (UINT8 **)NULL,
    (UINT8 *)NULL,
    FALSE,
#endif
#if defined(DUMP_REGISTER_NAMES_DDR) && (DUMP_REGISTER_NAMES_DDR==1)
    TRUE,
#else
    FALSE,
#endif
    InitTasks_DDR,
    (UINT8 *)InitData_DDR,
    (SUBTASK_INDEX_OFFSET *)SubtaskOffsetIndex_DDR,
    MAX_SUBTASKS_DDR,

    (INSTANCE_PORT_MAP *)InstancePortMapGenerated_DDR,
    (INT16 *)StrobeToRegisterGenerated_DDR,
    (FLOORPLAN *)FloorPlanGenerated_DDR,
    Registers_DDR,
    (INT16 *)RankToRegisterGenerated_DDR,
    (UINT8 *)ChannelToInstanceMapGenerated_DDR,
    (INSTANCE_PORT_OFFSET *)InstancePortOffsetGenerated_DDR,
    DynamicAssignmentGenerated_DDR,

    &ChannelEnabled[0],
    &ExecuteOnThisChannel[0],

    &CurrentPlatform[0],
    &CurrentFrequency[0],
    &CurrentConfiguration[0],
    &CurrentTechnology[0],

    &DynamicVars[0][0],
    DYNVAR_MAX_DDR
    );

  PopulateDynamicVariablesSprDdr (Host, MspData);
}

/**

  This routine is responsible to run the MMRC for DDRIO.

  param[in]                 Host                Pointer to host structure

  @retval                   EFI_SUCCESS         HBMIO was initialized

**/
UINT32
RunMmrc (
  IN              PSYSHOST            Host
  )
{
  UINT8   Channel;
  //
  // All these variables are common which means the
  // maximum number of elements will be taken from the
  // max number across Platform Types
  //
  BOOLEAN     ExecuteOnThisChannel[MAX_CHANNELS_DDR];
  BOOLEAN     ChannelEnabled[MAX_CHANNELS_DDR];
  UINT16      CurrentPlatform[MAX_CHANNELS_DDR];
  UINT16      CurrentFrequency[MAX_CHANNELS_DDR];
  UINT16      CurrentConfiguration[MAX_CHANNELS_DDR];
  UINT16      CurrentTechnology[MAX_CHANNELS_DDR];

  UINT16      CurrentPlatformBackup[MAX_CHANNELS_DDR];
  UINT16      CurrentFrequencyBackup[MAX_CHANNELS_DDR];
  UINT16      CurrentConfigurationBackup[MAX_CHANNELS_DDR];
  UINT16      CurrentTechnologyBackup[MAX_CHANNELS_DDR];

  UINT32      DynamicVars[MAX_CHANNELS_DDR][DYNVAR_MAX_DDR];
  UINT32      BackupMessageLevel = GetDebugLevel ();
  MSP_DATA    MspData;

  //
  // Initialize all the required variables
  //
  MspSet (DynamicVars, 0, MAX_CHANNELS_DDR * DYNVAR_MAX_DDR * sizeof (UINT32));
  //
  // Populate DDR for SPR variables
  //
  PopulateMspDdr (
    Host,
    &MspData,
    ExecuteOnThisChannel,
    ChannelEnabled,
    CurrentPlatform,
    CurrentFrequency,
    CurrentConfiguration,
    CurrentTechnology,
    DynamicVars
    );
  //
  // Now that MMRC knows the current platform, frequency, configuration, and type,
  // invoke the entry point to process the spreadsheet data.
  //


  //
  // Save current Platform/Frequency/Configuration/Technology
  //
  for (Channel = 0; Channel < MAX_CHANNELS_DDR; Channel++) {
    CurrentPlatformBackup[Channel]      = MspData.CurrentPlatform[Channel];
    CurrentFrequencyBackup[Channel]     = MspData.CurrentFrequency[Channel];
    CurrentConfigurationBackup[Channel] = MspData.CurrentConfiguration[Channel];
    CurrentTechnologyBackup[Channel]    = MspData.CurrentTechnology[Channel];
  }

  ConvertPfctFromBitXToX (&MspData);

  for (Channel = 0; Channel < MAX_CH; Channel += MAX_MC_CH) {
    if (IsChannelEnabled (Host->var.mem.currentSocket, Channel) ||
        IsChannelEnabled (Host->var.mem.currentSocket, Channel + 1)) {
      RcDebugPrintWithDevice (
        SDBG_MAX, Host->var.mem.currentSocket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Calling MspExecuteTask - InitDdrioRegisters_All_DDR\n"
        );
      MspExecuteTask (&MspData, InitDdrioRegisters_All_DDR, 1, Channel);
      RcDebugPrintWithDevice (
        SDBG_MAX, Host->var.mem.currentSocket, Channel + 1, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Calling MspExecuteTask - InitDdrioRegisters_All_DDR\n"
        );
      MspExecuteTask (&MspData, InitDdrioRegisters_All_DDR, 1, Channel + 1);
    }
  }

  //
  // Restore current Platform/Frequency/Configuration/Technology
  //
  for (Channel = 0; Channel < MAX_CHANNELS_DDR; Channel++) {
    MspData.CurrentPlatform[Channel]       = CurrentPlatformBackup[Channel];
    MspData.CurrentFrequency[Channel]      = CurrentFrequencyBackup[Channel];
    MspData.CurrentConfiguration[Channel]  = CurrentConfigurationBackup[Channel];
    MspData.CurrentTechnology[Channel]     = CurrentTechnologyBackup[Channel];
  }


  RcDebugPrintWithDevice (SDBG_MAX, Host->var.mem.currentSocket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Calling MspEntry - SPR()\n");

  MspEntry (&MspData);
  SetDebugLevel (BackupMessageLevel);

  return EFI_SUCCESS;
}

