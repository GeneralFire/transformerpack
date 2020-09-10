/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Memory/JedecDefinitions.h>
#include <Library/MemoryCoreLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/SiliconWorkaroundLib.h>

#define POWER_TRAINING_DEFAULT_SETTING_UINT8  0xFF
#define POWER_TRAINING_SIGNATURE              0x50777254 // ASCII chars 'P' 'w' 'r' 'T' shifted and or'ed together


#pragma pack(push, 1)

// Structure definition needed to temporarily set safe margins in a power training context
typedef struct {
  UINT32            Signature; // This must be the first member.
  UINT8             CurrentSettingIndex;
  UINT8             NumParams;
  POWER_TRAIN_PARAM *Param;
  INT16             (*SafeValues)[MAX_PARAMS][MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS];
  UINT8             Socket;
} SAFE_MARGINS_POWER_TRAINING_CONTEXT;

#pragma pack(pop)

UINT8 InValidSettingCheck (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 param, INT16 setting);
UINT8 SkipRankCheck (PSYSHOST  Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 Rank, UINT8 param);
MRC_STATUS
GetParamPerChPerStrobeOrByte (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN OUT  INT16             Values[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  );

EFI_STATUS
GetMrcLevel (
  IN  MRC_GT    ParamGsm,
  OUT MRC_LT    *Level
  );

/**
  Populates a POWER_TRAIN_PARAM structure based on a MRC_GT signal name

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  SignalName  Signal group name
  @param[out] Param       Pointer to the power training parameter structure to populate

  @retval     MRC_STATUS_SUCCESS  The Param structure was successfully populated
  @retval     MRC_STATUS_FAILURE  Unknown error occurred
  @retval     MRC_STATUS_SIGNAL_NOT_SUPPORTED   Invalid parameter for SignalName provided
**/
MRC_STATUS
GetPowerParamFromSignalName (
  PSYSHOST            Host,
  UINT8               Socket,
  MRC_GT              SignalName,
  POWER_TRAIN_PARAM   *Param
  )
{

  INT16   DramDrvStrDefaultSettingList[] = {
            DDR_RON_34,
            DDR_RON_48
            };
  UINT8   SettingsSize;

  //
  // All parameters use the same values for these fields, so we set them here. These can be overridden in
  // a specific signal's case below if necessary in the future.
  //
  Param->Mode           = MODE_VIC_AGG;
  Param->PatternLength  = 64;
  Param->MarginType     = GetMargin;
  Param->ParamGsm       = SignalName;

  //
  // Get string for the group name - may be overwritten below
  //
  if (EFI_ERROR (GetGroupString (SignalName, &Param->Name))) {
    Param->Name = "Unknown";
  }

  switch (SignalName) {

  case RxOdt:
  case TxRon:
  case TxEq:
  case TxEqTap2:
  case CmdTxEq:
  case PxcDelta:
  case TxImode:
  case TxRiseFallSlewRate:
  case TxFallSlewRate:
  case TxTco:
  case DdjcDelta:
  case RxDfe:
  case RxDfeCoefficient:
  case RxEq:
  case RxBias:
  case RxCtleC:
  case RxCtleR:
  case DramRxEq:
  case TxEchoCoef:
  case TxEchoUiSel:
  case RxDfeCode:
  case RxDfeDelaySel:
  case DbDfeTap1:
  case DbDfeTap2:
  case DbDfeTap3:
  case DbDfeTap4:

    if (GetPowerParameterSettings (Host, Socket, Param) != MRC_STATUS_SUCCESS) {
      return FAILURE;
    }

    break;

  case DramDrvStr:

    //
    // Common settings for all SOCs
    //
    Param->NumSettings = sizeof (DramDrvStrDefaultSettingList) / sizeof (DramDrvStrDefaultSettingList[0]);

    SettingsSize = MIN (sizeof (Param->SettingList), sizeof (DramDrvStrDefaultSettingList));
    CopyMem ((UINT8 *) Param->SettingList, (UINT8 *) DramDrvStrDefaultSettingList, SettingsSize);

    if (GetPowerParameterSettings (Host, Socket, Param) != MRC_STATUS_SUCCESS) {
      return FAILURE;
    }

    break;

  case RxDqDelay:

    //
    // Special case to override the string
    //
    Param->Name = "DFE RxDq Delay";

    if (GetPowerParameterSettings (Host, Socket, Param) != MRC_STATUS_SUCCESS) {
      return FAILURE;
    }

    break;

  case ParkOdt:
  case WrOdt:
  case NomOdt:

    Param->ParamType = PerRankPerCh;

    break;

  default:
    RcDebugPrintWithDevice (
                            SDBG_MAX,
                            Socket,
                            NO_CH,
                            NO_DIMM,
                            NO_SUBCH,
                            NO_RANK,
                            NO_STROBE,
                            NO_BIT,
                            "GetPowerParamFromSignalName called with unsupported SignalName\n"
                            );

    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_131);
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    break;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Displays power training results per channel

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  Param       Pointer to the power training parameter to display results for

  @retval     MRC_STATUS_SUCCESS  The Param structure was successfully displayed
**/
MRC_STATUS
DisplayPowerResultsPerCh (
  PSYSHOST          Host,
  UINT8             Socket,
  POWER_TRAIN_PARAM *Param
  )
{
  UINT8   Channel;
  INT16   OptimalValue;
  MRC_GT  ParamGsm = Param->ParamGsm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    OptimalValue = 0;
    GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm, GSM_READ_ONLY, &OptimalValue);
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "%3d\n", OptimalValue);
  } // Channel loop

  return MRC_STATUS_SUCCESS;
}

/**
  Displays power training results per rank

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  Param       Pointer to the power training parameter to display results for

  @retval     MRC_STATUS_SUCCESS  The Param structure was successfully displayed
**/
MRC_STATUS
DisplayPowerResultsPerRank (
  PSYSHOST          Host,
  UINT8             Socket,
  POWER_TRAIN_PARAM *Param
  )
{

  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  INT16   OptimalValue;
  MRC_GT  ParamGsm = Param->ParamGsm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Channel, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Channel, Dimm) && Rank > 0) {
          continue;
        }

        OptimalValue = 0;
        GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, 0, ALL_BITS, DdrLevel, ParamGsm, GSM_READ_ONLY, &OptimalValue);
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                "%2d\n", OptimalValue);
      } // Rank loop
    } // Dimm loop
  } // Channel loop

  return MRC_STATUS_SUCCESS;
}

/**
  Displays power training results per channel per strobe or byte

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  Param       Pointer to the power training parameter to display results for

  @retval     MRC_STATUS_SUCCESS  The Param structure was successfully displayed
**/
MRC_STATUS
DisplayPowerResultsPerChPerStrobeOrByte (
  PSYSHOST          Host,
  UINT8             Socket,
  POWER_TRAIN_PARAM *Param
  )
{

  UINT8   Channel;
  UINT8   Strobe;
  INT16   OptimalValue;
  MRC_GT  ParamGsm = Param->ParamGsm;
  MPT_PT  ParamType = Param->ParamType;
  UINT8   MSVx4 = GetMaxStrobeValid (Host);
  UINT8   MSVx8 = MSVx4 / 2;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                          "        ");

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    if ((Strobe >= MSVx8) && (ParamType == PerBytePerCh)) {
      break;
    }
    RcDebugPrint (SDBG_MAX,
                            "  %3d", Strobe);
  }

  RcDebugPrint (SDBG_MEM_TRAIN,
                          "\n");

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "");

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }

      if ((Strobe >= MSVx8) && (ParamType == PerBytePerCh)) {
        break;
      }

      OptimalValue = 0;
      GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, 0, 0, Strobe, ALL_BITS, DdrLevel, ParamGsm, GSM_READ_ONLY, &OptimalValue);
      RcDebugPrint (SDBG_MEM_TRAIN,
                              "  %3d", OptimalValue);

    } // Strobe loop

    RcDebugPrint (SDBG_MEM_TRAIN, "\n");

  } // Channel loop

  return MRC_STATUS_SUCCESS;
}

/**
  Displays power training results per strobe or byte

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  Param       Pointer to the power training parameter to display results for

  @retval     MRC_STATUS_SUCCESS  The results were successfully displayed
**/
MRC_STATUS
DisplayPowerResultsPerStrobeOrByte (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param
  )
{
  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   Strobe;
  MPT_PT  ParamType;
  MRC_GT  ParamGsm;
  INT16   OptimalValue;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  RcDebugPrint (SDBG_MAX,
                          "              ");
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    if ((ParamType == PerBytePerRankPerCh) && (Strobe >= MSVx8)) {
      break;
    }

    RcDebugPrint (SDBG_MAX, "  %3d", Strobe);
  }

  RcDebugPrint (SDBG_MEM_TRAIN,
                          "\n");

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Channel, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Channel, Dimm) && Rank > 0) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "");

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          if ((ParamType == PerBytePerRankPerCh) && (Strobe >= MSVx8)) {
            break;
          }

          OptimalValue = 0;
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, ParamGsm, GSM_READ_ONLY, &OptimalValue);
          RcDebugPrint (SDBG_MEM_TRAIN,
                                  "  %3d", OptimalValue);

        } // Strobe loop

        RcDebugPrint (SDBG_MEM_TRAIN, "\n");

      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Displays power training results per rank per bit

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  Param       Pointer to the power training parameter to display results for

  @retval     MRC_STATUS_SUCCESS  The results were successfully displayed
**/
MRC_STATUS
DisplayPowerResultsPerRankPerBit (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param
  )
{
  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   Bit;
  UINT8   MaxBits;
  UINT8   EncodedNibble;
  MRC_GT  ParamGsm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  INT16 OptimalValue;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ParamGsm = Param->ParamGsm;

  MaxBits = GetMaxBitsValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "\n");
        for (Bit = 0; Bit < MaxBits; Bit++) {
          if ((Bit / 4) % 2) {
            EncodedNibble = (Bit / 8) + MSVx8;
          } else {
            EncodedNibble = Bit / 8;
          }
          if ((Bit == 0) || (Bit == (MaxBits / 2))) {
            PrintPerBitHeading (Host, Socket, Bit, MaxBits);
          }
          OptimalValue = 0;
          // Read value from specified bit
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, EncodedNibble, (Bit%4), DdrLevel,
            ParamGsm, GSM_READ_ONLY, &OptimalValue);
          RcDebugPrint (SDBG_MAX,
                                  " %3d", OptimalValue);
        } // Bit loop

        RcDebugPrint (SDBG_MAX, "\n");

      } // Rank loop
    } // Dimm loop
  } // Channel loop

  return MRC_STATUS_SUCCESS;
}


/**
  Displays the Power/Margin Training results

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  SignalName  Signal group name

  @retval     MRC_STATUS_SUCCESS  The results were successfully displayed
  @retval     MRC_STATUS_FAILURE  An unknown failure occurred
  @retval     MRC_STATUS_SIGNAL_NOT_SUPPORTED The provided SignalName is not valid
**/
MRC_STATUS
DisplayPowerTrainingResults (
  PSYSHOST          Host,
  UINT8             Socket,
  MRC_GT            SignalName
  )
{
  POWER_TRAIN_PARAM Param;
  MRC_STATUS        Status;

  if (((SignalName == TxRiseFallSlewRate) || (SignalName == TxFallSlewRate)) &&
      (!GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_T_TX_RISE_FALL_SLEW_RATE))) {
    return MRC_STATUS_SUCCESS;
  }

  Status = GetPowerParamFromSignalName (Host, Socket, SignalName, &Param);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  RcDebugPrint (SDBG_MEM_TRAIN,
                          "START_DATA_%a\n", Param.Name);

  switch (Param.ParamType) {
  case PerCh:
  case PerMc:
    Status = DisplayPowerResultsPerCh (Host, Socket, &Param);
    break;
  case PerRankPerCh:
    Status = DisplayPowerResultsPerRank (Host, Socket, &Param);
    break;
  case PerNibblePerCh:
  case PerBytePerCh:
    Status = DisplayPowerResultsPerChPerStrobeOrByte (Host, Socket, &Param);
    break;
  case PerNibblePerRankPerCh:
  case PerBytePerRankPerCh:
    Status = DisplayPowerResultsPerStrobeOrByte (Host, Socket, &Param);
    break;
  case PerBitPerRankPerCh:
    Status = DisplayPowerResultsPerRankPerBit (Host, Socket, &Param);
    break;
  default:
      RcDebugPrint (SDBG_MAX,
                              "\nUnrecognized parameter type: 0x%x\n", Param.ParamType);
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    break;
  }

  RcDebugPrint (SDBG_MEM_TRAIN,
                          "\n");

  return Status;

} // DisplayPowerTrainingResults

/**
  Optimizes DRAM drive strength by margining Rx eye width and height at various settings for DramDrvStr

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainDramRon (
  IN      PSYSHOST Host
  )
{
  UINT8             Socket;
  MRC_GT            Group[MAX_GROUPS];
  UINT8             NumGroups;
  POWER_TRAIN_PARAM Param;
  UINT8             NumParams;
  SYS_SETUP         *Setup;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (DramROnTraining)) {
    return SUCCESS;
  }

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, DramDrvStr, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Signal groups to be margined for this parameter
  Group[0] = RxVref;
  Group[1] = RxDqsDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;
} // TrainDramRon

/**
  Searches for programmable CTLE DIMMs available in the current socked

  @retval TRUE is there is at least one DIMM supporting programmable CLTE in the current socket,
          FALSE if there is not
**/
BOOLEAN
IsProgrammableCtleDimmAvailable (
  VOID
  )
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               MaxChDdr;
  UINT8               Ch;
  UINT8               Dimm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if (DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        return TRUE;
      }
    } // Dimm loop
  } // Ch loop

  return FALSE;
}

/**

  Train DRAM Rx Eq

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EFIAPI
TrainDramRxEq (
  IN PSYSHOST Host
  )
{
  UINT8             Socket;
  MRC_GT            Group[MAX_GROUPS];
  UINT8             NumGroups;
  POWER_TRAIN_PARAM Param;
  UINT8             NumParams;
  SYS_SETUP         *Setup;
  UINT8             DdrFrequency;


  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  GetEncodedDdrFreq (Socket, &DdrFrequency);
  //
  // Return if this socket is disabled
  // or if this step is disabled via input option
  // or if PDA is disabled in setup
  // or if memory flow is disabled
  // or if this config is only populated with LRDIMMs
  // or if frequency is less than 2933
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0)
      || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)
      || ((Setup->mem.optionsExt & PDA_EN) == 0)
      || (!IsMemFlowEnabled (DramRxEqTraining))
      || (IsNonLrdimmPresentOnSocket (Socket) == FALSE)
      || (DdrFrequency < DDR_2933)) {
    return SUCCESS;
  }

  //
  // If there is no DIMM supporting programmable CTLE, return
  //
  if (!IsProgrammableCtleDimmAvailable ()) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Skipping Dram Rx Eq, there is no memory supporting programmable CTLE\n");
    return SUCCESS;
  }

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, DramRxEq, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0]  = TxVref;
  Group[1]  = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;

}

/**
  Optimizes MC ODT by margining Rx eye width and height at various settings for MC ODT

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainMcOdt (
  IN      PSYSHOST Host
  )
{
  UINT8              Socket;
  MRC_GT             Group[MAX_GROUPS];
  UINT8              NumGroups;
  POWER_TRAIN_PARAM  Param;
  UINT8              NumParams;
  SYS_SETUP          *Setup;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (McOdtTraining)) {
    return SUCCESS;
  }

  //
  // Parameters to Pass
  //
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, RxOdt, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0]  = RxVref;
  Group[1]  = RxDqsDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;

} // TrainMcOdt

/**
  Optimizes MC RON by margining TX eye width and height at various settings for MC RON

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainMcRon (
  IN      PSYSHOST Host
  )
{
  UINT8              Socket;
  MRC_GT             Group[MAX_GROUPS];
  UINT8              NumGroups;
  POWER_TRAIN_PARAM  Param;
  UINT8              NumParams;
  SYS_SETUP          *Setup;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (McROnTraining)) {
    return SUCCESS;
  }

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, TxRon, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0]  = TxVref;
  Group[1]  = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;

} // TrainMcRon

/**
  Optimizes TxEq by margining TX DQ eye width and height at various settings for TxEq

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainTxEq (
  IN      PSYSHOST Host
  )
{
  UINT8               Socket;
  MRC_GT              Group[MAX_GROUPS];
  UINT8               NumGroups;
  POWER_TRAIN_PARAM   Param;
  UINT8               NumParams;
  MRC_STATUS          Status;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;
  Status = MRC_STATUS_SUCCESS;

  if (!IsMemFlowEnabled (TxEqTraining)) {
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, TxEq, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  InitTxEqTunning (Host, Socket);

  // Define signal groups to be margined for this parameter
  Group[0]  = TxVref;
  Group[1]  = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  //
  // Control flow per segment/product
  //
  if (ExecuteTheNextOptPhaseOnThisProduct (Host, Param) == MRC_STATUS_SUCCESS) {
    //
    // Tune TxEq tap 2
    //
    Status = GetPowerParamFromSignalName (Host, Socket, TxEqTap2, &Param);
    if (Status == MRC_STATUS_SUCCESS) {
      TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);
    }
  }

  IO_Reset (Host, Socket);
  JedecInitSequence (Host, Socket, CH_BITMASK);

  return Status;
} // TrainTxEq

/**
  Optimizes CMD TXEQ by margining CMD eye width and height at various settings for CMD TXEQ.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainCmdTxEq (
  IN     PSYSHOST Host
  )
{
  UINT8                 Socket;
  MRC_GT                Group[MAX_GROUPS];
  UINT8                 NumGroups;
  POWER_TRAIN_PARAM     Param;
  UINT8                 NumParams;
  SYS_SETUP             *Setup;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  if (ShouldChipSkipCmdTxEq ()) {
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (CmdTxEqTraining)) {
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, CmdTxEq, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0]  = CmdVref;
  Group[1]  = CmdAll;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  IO_Reset (Host, Socket);
  JedecInitSequence (Host, Socket, CH_BITMASK);

  return SUCCESS;

} // TrainCmdTxEq

/**
  This function implements the TX Phase-based Xtalk Cancellation (PXC) algorithm.

  @param[in] Host     - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
TrainPXC (
  IN     PSYSHOST   Host
  )
{
  UINT8                 Ch;
  UINT8                 Socket;
  MRC_GT                Group[MAX_GROUPS];
  UINT8                 NumGroups;
  POWER_TRAIN_PARAM     Param;
  UINT8                 NumParams;
  UINT8                 ChBitMask = 0x3F; // default enable all channels
  INT16                 PxcControlValue;
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  //
  // Return if this step is disabled
  //
  if (!GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_PXC_TRAINING)) {
    return SUCCESS;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      ChBitMask &= ~(1 << Ch); // use the ChBitMask for disabled channels
      continue;
    }

    //
    // enable the Ch, to use this in WA
    //
    Host->var.mem.pxcEnabled |= (1 << Ch);

    // program the aggressors
    PxcProgramAggressors (Host, Socket, Ch);
    PxcProgramCapInd (Host, Socket, Ch);

  } // Ch loop

  PxcControlValue = PXC_ENABLE;
  GetSetDataGroup (Host, Socket, ALL_CH, NO_SUBCH, 0, 0, 0, 0, DdrLevel, PxcControl, GSM_FORCE_WRITE, &PxcControlValue);

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, PxcDelta, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0]  = TxVref;
  Group[1]  = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;

} // TrainPXC

/**
  Optimizes Tx IMODE by margining Tx Dq eye width and height at various settings for Tx IMODE.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainImode (
  IN     PSYSHOST Host
  )
{
  UINT8               Socket;
  UINT8               Ch;
  MRC_GT              Group[MAX_GROUPS];
  UINT8               NumGroups;
  POWER_TRAIN_PARAM   Param;
  UINT8               NumParams;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();
  Socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  //
  // Return if this step is disabled
  //
  if (!IsMemFlowEnabled (ImodeTraining)) {
    return SUCCESS;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // Make sure rfon is set so the Tx Eq changes take effect
    //
    SetRfOn (Host, Socket, Ch, RF_ON_ENABLE);
  } // ch loop

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, TxImode, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0]  = TxVref;
  Group[1]  = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;

} // TrainImode

/**
  Optimizes RX CTLE settings by margining Rx Dqs eye width and height at various settings for the RX CTLE controls.

  TrainCtleLegacy sweeps RxEq, CTLE-C and CTLE-R together. This is the original implementation of CTLE training.
  The new version - TrainCtle - sweeps RxEq separately before sweeping CTLE-C and CTLE-R, resulting in
  potentially better RX CTLE settings, but at the cost of additional execution time.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainCtleLegacy (
  IN     PSYSHOST Host
  )
{
  UINT8               Socket;
  MRC_GT              Group[MAX_GROUPS];
  UINT8               NumGroups;
  POWER_TRAIN_PARAM   Param[MAX_PARAMS];
  UINT8               NumParams;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer();

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  //
  // Return if this step is disabled
  //
  if (!IsMemFlowEnabled (RxCtleTraining)) {
    return SUCCESS;
  }

  // Define signal groups to be margined for these parameters
  Group[0]  = RxVref;
  Group[1]  = RxDqsDelay;
  NumGroups = 2;

  if (Setup->mem.dfxMemSetup.DfxRxCtleExtendedRange != 0) {
    // Parameters to Pass
    NumParams = 4;
    if ((GetPowerParamFromSignalName(Host, Socket, RxEq, &Param[0]) != MRC_STATUS_SUCCESS) ||
        (GetPowerParamFromSignalName(Host, Socket, RxBias, &Param[1]) != MRC_STATUS_SUCCESS) ||
        (GetPowerParamFromSignalName(Host, Socket, RxCtleC, &Param[2]) != MRC_STATUS_SUCCESS) ||
        (GetPowerParamFromSignalName(Host, Socket, RxCtleR, &Param[3]) != MRC_STATUS_SUCCESS)) {
      return FAILURE;
    }
  }
  else {
    // Parameters to Pass
    NumParams = 3;
    if ((GetPowerParamFromSignalName(Host, Socket, RxEq, &Param[0]) != MRC_STATUS_SUCCESS) ||
        (GetPowerParamFromSignalName(Host, Socket, RxCtleC, &Param[1]) != MRC_STATUS_SUCCESS) ||
        (GetPowerParamFromSignalName(Host, Socket, RxCtleR, &Param[2]) != MRC_STATUS_SUCCESS)) {
      return FAILURE;
    }
  }

  TrainDDROptParam (Host, Socket, Group, NumGroups, Param, NumParams);

  return SUCCESS;

} // TrainCtleLegacy

/**
  Optimizes RX CTLE settings by margining Rx Dqs eye width and height at various settings for the RX CTLE controls.

  TrainCtle sweeps RxEq first, then CTLE-C and CTLE-R together. This version of CTLE training takes longer to execute
  than TrainCtleLegacy, but may result in improved margins compared to TrainCtleLegacy.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainCtle (
  IN     PSYSHOST Host
  )
{
  UINT8               Socket;
  MRC_GT              Group[MAX_GROUPS];
  UINT8               NumGroups;
  POWER_TRAIN_PARAM   Param[MAX_PARAMS];
  UINT8               NumParams;
  SYS_SETUP           *Setup;

  Socket = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer ();

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  //
  // Return if this step is disabled
  //
  if (!IsMemFlowEnabled (RxCtleTraining)) {
    return SUCCESS;
  }

  if (IsSiliconWorkaroundEnabled ("S14010791427")) {
    return SUCCESS;
  }

  // Define signal groups to be margined for these parameters
  Group[0]  = RxVref;
  Group[1]  = RxDqsDelay;
  NumGroups = 2;

  //
  // Train CTLE-C/CTLE-R/RXEQ
  //
  NumParams = 3;
  if ((GetPowerParamFromSignalName (Host, Socket, RxCtleC, &Param[0]) != MRC_STATUS_SUCCESS) ||
      (GetPowerParamFromSignalName (Host, Socket, RxCtleR, &Param[1]) != MRC_STATUS_SUCCESS) ||
      (GetPowerParamFromSignalName (Host, Socket, RxEq, &Param[2]) != MRC_STATUS_SUCCESS)) {
    return FAILURE;
  }

  TrainDDROptParam (Host, Socket, Group, NumGroups, Param, NumParams);

  return SUCCESS;

} // TrainCtle

/**
  Optimizes Rx DFE settings by margining Rx Dqs eye width and height at various settings for the Rx DFE controls.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainRxDfe (
  IN     PSYSHOST Host
  )
{
  UINT8               Socket;
  MRC_GT              Group[MAX_GROUPS];
  UINT8               NumGroups;
  POWER_TRAIN_PARAM   Param[MAX_PARAMS];
  UINT8               NumParams;

  Socket = Host->var.mem.currentSocket;
  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }
  //
  // Return if this step is disabled
  //
  if (!GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_T_RX_DFE)) {
    return SUCCESS;
  }
  //
  // Parameters to Pass
  //
  NumParams = NUM_RX_DFE_PARAMS;

  if (GetPowerParamFromSignalName (Host, Socket, RxDqDelay, &Param[0]) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  //
  //The first pass of the Rx DFE power training algorithm uses a subset of RxDfeCoefficient values, so the ParamGsm
  //is temporarily set to RxDfe so that the GetPowerParameterSettings routine knows that this is the first pass
  //and the Param value is fixed up before calling the TrainDDROptParam routine
  //
  if (GetPowerParamFromSignalName (Host, Socket, RxDfe, &Param[1]) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  //
  //Fix up the ParamGsm value so the routine uses the correct parameter
  //
  Param[1].ParamGsm = RxDfeCoefficient;
  // Define signal groups to be margined for these parameters
  Group[0]  = RxVref;
  Group[1]  = RxDqsDelay;
  NumGroups = 2;
  TrainDDROptParam (Host, Socket, Group, NumGroups, Param, NumParams);

  if (GetPowerParamFromSignalName (Host, Socket, RxDfeCoefficient, &Param[0]) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }
  // Define signal groups to be margined for these parameters
  Group[0] = RxVref;
  Group[1] = RxDqsDelay;
  NumGroups = 2;
  NumParams = 1;
  TrainDDROptParam (Host, Socket, Group, NumGroups, Param, NumParams);

  return SUCCESS;

} // TrainRxDfe

#ifdef LRDIMM_SUPPORT
/*++

  Data Buffer DFE training for LRDIMM

  @param Host                - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
TrainLrdimmDataBufferDfe (
  IN  PSYSHOST        Host
  )
{
  SYS_SETUP           *Setup;
  UINT8               Socket;
  MRC_GT              Group[MAX_GROUPS];
  UINT8               NumGroups;
  POWER_TRAIN_PARAM   Param;
  UINT8               NumParams;

  Socket = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer ();

  //
  // Return if this step is disabled
  //
  if (!IsMemFlowEnabled (LrdimmDbDfe)) {
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  //
  // Return if there is no LRDIMM + BPS channel and running 3200 Per socket level
  //
  if (!GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_T_LRDIMM_DB_DFE)) {
    return SUCCESS;
  }

  RcDebugPrint (SDBG_MAX,
    "Train Lrdimm DataBuffer DFE Tap1 \n");

  if (GetPowerParamFromSignalName (Host, Socket, DbDfeTap1, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  //
  // Define signal groups to be margined for this parameter
  //
  Group[0]  = TxVref;
  NumGroups = 1;
  NumParams = 1;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  RcDebugPrint (SDBG_MAX,
    "Train Lrdimm DataBuffer DFE Tap2 \n");

  if (GetPowerParamFromSignalName (Host, Socket, DbDfeTap2, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  //
  // Define signal groups to be margined for this parameter
  //
  Group[0]  = TxVref;
  NumGroups = 1;
  NumParams = 1;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;

} // TrainLrdimmDataBufferDfe
#endif //LRDIMM_SUPPORT

/**
  Display TxRiseFallSlewRate final results.
  This parameter is not directly trained but may be modified by TxFallSlewRate training
  and therefore requires a special results display handler.

  @param[in]    Host    - Pointer to sysHost structure
  @param[in]    Socket  - Socket number

  @retval MRC_STATUS_SUCCESS if parameter is supported
  @retval MRC_STATUS_FAILURE otherwise
**/
MRC_STATUS
DisplayTxRiseFallSlewRate (
  IN     PSYSHOST Host,
  IN     UINT8    Socket
  )
{
  POWER_TRAIN_PARAM   Param;
  INT16               Values[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS];

  ZeroMem (Values, sizeof (Values));

  if (GetPowerParamFromSignalName (Host, Socket, TxRiseFallSlewRate, &Param) != MRC_STATUS_SUCCESS) {
    return MRC_STATUS_FAILURE;
  }

  RcDebugPrint (SDBG_MAX,
    "\nSTART_OPTIMAL_TRAINING_RESULTS\n%a\n", Param.Name);

  GetParamPerChPerStrobeOrByte (Host, Socket, &Param, 1, Values);

  RcDebugPrint (SDBG_MAX,
    "END_OPTIMAL_TRAINING_RESULTS\n");

  return MRC_STATUS_SUCCESS;
}

/**
  Optimizes TX rise and fall slew rates by margining Tx Dq eye width and height at various
    TX rise/fall slew rate settings.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainTxRiseFallSlewRate (
  IN     PSYSHOST Host
  )
{
  UINT8               Socket;
  MRC_GT              Group[MAX_GROUPS];
  UINT8               NumGroups;
  POWER_TRAIN_PARAM   Param;
  UINT8               NumParams;

  Socket = Host->var.mem.currentSocket;
  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  //
  // Return if this step is disabled
  //
  if (!GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_T_TX_RISE_FALL_SLEW_RATE)) {
    return SUCCESS;
  }

  //
  // Perform DDRIO-specific tasks before training
  //
  MemPowerTrainingDdrioHook (Socket, TxSlewRatePreTest);

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, TxFallSlewRate, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for these parameters
  Group[0] = TxVref;
  Group[1] = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  //
  // Perform DDRIO-specific tasks after training and display changes
  //
  MemPowerTrainingDdrioHook (Socket, TxSlewRatePostTest);

  //
  // Display final setting for TxRiseFallSlewRate, which isn't trained
  // but may be modified by TxFallSlewRate training.
  //
  DisplayTxRiseFallSlewRate (Host, Socket);

  return SUCCESS;

} // TrainTxRiseFallSlewRate


/**
  Optimizes TX TCO by margining Tx Dq eye width and height at various settings for TX TCO.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainTcoComp (
  IN     PSYSHOST Host
  )
{
  UINT8              Socket;
  MRC_GT             Group[MAX_GROUPS];
  UINT8              NumGroups;
  POWER_TRAIN_PARAM  Param;
  UINT8              NumParams;
  UINT8              DdrFrequency;
  SYS_SETUP          *Setup;

  Setup = GetSysSetupPointer ();
  Socket = Host->var.mem.currentSocket;

  //
  // Return if this training is disabled by Setup
  //
  if ((Setup->mem.optionsExt & TCO_COMP_EN) == 0) {
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //
  GetEncodedDdrFreq (Socket, &DdrFrequency);
  //
  // Return if this socket is disabled
  // or if memory flow is disabled
  // or if frequency is less than 2933
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0)
      || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)
      || (!IsMemFlowEnabled (TcoCompTraining))
      || (DdrFrequency < DDR_2933)) {
    return SUCCESS;
  }

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, TxTco, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0] = TxVref;
  Group[1] = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  return SUCCESS;

} // TrainTcoComp

/**
  This function implements the TX Data Dependent Jitter Cancellation (DDJC) algorithm.

  @param[in] Host     - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
TrainTxDdjc (
  IN     PSYSHOST   Host
  )
{
  UINT8                 Socket;
  INT16                 PxcControlValue;
  INT16                 PxcControlValueSave;
  MRC_GT                Group[MAX_GROUPS];
  UINT8                 NumGroups;
  POWER_TRAIN_PARAM     Param;
  UINT8                 NumParams;
  struct channelNvram   (*ChannelNvList)[MAX_CH];

  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  //
  // Return if this step is disabled
  //
  if (!GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_DDJC_TRAINING)) {
    return SUCCESS;
  }

  //
  // Disable PXC while running DDJC training
  //
  GetSetDataGroup (Host, Socket, ALL_CH, NO_SUBCH, 0, 0, 0, 0, DdrLevel, PxcControl, GSM_READ_ONLY, &PxcControlValueSave);
  if (PxcControlValueSave == PXC_ENABLE) {
    PxcControlValue = PXC_DISABLE;
    GetSetDataGroup (Host, Socket, ALL_CH, NO_SUBCH, 0, 0, 0, 0, DdrLevel, PxcControl, GSM_FORCE_WRITE, &PxcControlValue);
  }

  // Parameters to Pass
  NumParams = 1;
  if (GetPowerParamFromSignalName (Host, Socket, DdjcDelta, &Param) != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  // Define signal groups to be margined for this parameter
  Group[0] = TxVref;
  Group[1] = TxDqDelay;
  NumGroups = 2;

  TrainDDROptParam (Host, Socket, Group, NumGroups, &Param, NumParams);

  //
  // Re-enabled PXC after DDJC training, if it was previously enabled
  //
  if (PxcControlValueSave == PXC_ENABLE) {
    GetSetDataGroup (Host, Socket, ALL_CH, NO_SUBCH, 0, 0, 0, 0, DdrLevel, PxcControl, GSM_FORCE_WRITE, &PxcControlValueSave);
  }

  return SUCCESS;

} // TrainTxDdjc

/**
  For each channel, save the current setting for the Parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer to array of parameter structures
  @param[in]      ParamIndex    - Index of parameter passed in
  @param[in,out]  Values        - Pointer to array to store current values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetParamPerCh (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN OUT  INT16             Values[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Ch;
  MRC_GT              ParamGsm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ParamGsm = Param->ParamGsm;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm,
      GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &Values[Ch][0][0][0]);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "%2d\n", Values[Ch][0][0][0]);
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  For each rank, save the current setting for the Parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer to array of parameter structures
  @param[in]      ParamIndex    - Index of parameter passed in
  @param[in,out]  Values        - Pointer to array to store current values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetParamPerRank (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN OUT  INT16             Values[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  MRC_GT              ParamGsm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ParamGsm = Param->ParamGsm;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, 0, ALL_BITS, DdrLevel, ParamGsm,
          GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &Values[Ch][Dimm][Rank][0]);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                "%2d\n", Values[Ch][Dimm][Rank][0]);
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  For each strobe or byte in each channel, save the current setting for the Parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer to array of parameter structures
  @param[in]      ParamIndex    - Index of parameter passed in
  @param[in,out]  Values        - Pointer to array to store current values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetParamPerChPerStrobeOrByte (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN OUT  INT16             Values[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Ch;
  UINT8               Strobe;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  RcDebugPrint (SDBG_MAX, "Strobe  Param  ");
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    if ((Strobe >= MSVx8) && (ParamType == PerBytePerCh)) {
      break;
    }
    RcDebugPrint (SDBG_MAX,
                            "%2d  ",Strobe);
  }
  RcDebugPrintLine (SDBG_MAX, 84, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  // Fill in margin entries for strobes for per Ch per Strobe

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    %d  ", ParamIndex);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      if ((Strobe >= MSVx8) && (ParamType == PerBytePerCh)) {
        break;
      }
      GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, Strobe, ALL_BITS, DdrLevel, ParamGsm,
        GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &Values[Ch][0][0][Strobe]);
      RcDebugPrint (SDBG_MAX,
        "%3d ", Values[Ch][0][0][Strobe]);
    } // Strobe loop
    RcDebugPrint (SDBG_MAX,
      "\n");

  } // Ch loop
  RcDebugPrint (SDBG_MAX, "\n");

  return MRC_STATUS_SUCCESS;
}

/**
  For each strobe or byte in each rank, save the current setting for the Parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer to array of parameter structures
  @param[in]      ParamIndex    - Index of parameter passed in
  @param[in,out]  Values        - Pointer to array to store current values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetParamPerStrobeOrByte (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN OUT  INT16             Values[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  RcDebugPrint (SDBG_MAX, "Strobe        ");
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    if ((ParamType == PerBytePerRankPerCh) && (Strobe >= MSVx8)) {
      break;
    }
    RcDebugPrint (SDBG_MAX, "%2d  ",Strobe);
  }

  RcDebugPrintLine (SDBG_MAX, 84, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // If DRAM Rx Eq training is running and the DIMM does not support programmable CTLE, continue
      //
      if ((ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        //
        // This is the print statement during pre-training
        //
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "DIMM does not support programmable CTLE.\n");
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, " ");
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          if ((ParamType == PerBytePerRankPerCh) && (Strobe >= MSVx8)) {
            break;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel,
            ParamGsm, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &Values[Ch][Dimm][Rank][Strobe]);
          RcDebugPrint (SDBG_MAX,
            "%2d  ", Values[Ch][Dimm][Rank][Strobe]);
        } // Strobe loop
        RcDebugPrint (SDBG_MAX, "\n");
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  For each bit in each rank, save the current setting for the Parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer to array of parameter structures
  @param[in]      ParamIndex    - Index of parameter passed in
  @param[in,out]  Values        - Pointer to array to store current values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetParamPerRankPerBit (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN OUT  INT16             DefaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Bit;
  UINT8               MaxBits;
  UINT8               Strobe;
  MRC_GT              ParamGsm;
  MRC_LT              Level;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ParamGsm = Param->ParamGsm;

  GetMrcLevel (ParamGsm, &Level);
  MaxBits = GetMaxBitsValid (Host);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        if (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, ParamGsm)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "Per-bit -  Param: %d\n", ParamIndex);
        for (Bit = 0; Bit < MaxBits; Bit++) {
          if (Level == LrbufLevel) {
            Strobe = Bit / 8;
          } else {
            if ((Bit / 4) % 2) {
              Strobe = (Bit / 8) + MSVx8;
            } else {
              Strobe = Bit / 8;
            }
          }

          if ((Bit == 0) || (Bit == (MaxBits / 2))) {
            PrintPerBitHeading (Host, Socket, Bit, MaxBits);
          }

          // Read value from specified bit
          if (Level == LrbufLevel) {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, (Bit % MAX_BITS_IN_BYTE), Level,
            ParamGsm, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &DefaultValues[Ch][Dimm][Rank][Bit]);
          } else {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, (Bit % BITS_PER_NIBBLE), Level,
            ParamGsm, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &DefaultValues[Ch][Dimm][Rank][Bit]);
          }
          RcDebugPrint (SDBG_MAX,
            " %3d", DefaultValues[Ch][Dimm][Rank][Bit]);
        } // Bit loop
        RcDebugPrint (SDBG_MAX, "\n\n");
      } // Rank
    } // Dimm
  } // ch

  return MRC_STATUS_SUCCESS;
}

/**
  For each parameter in numParams, save current setting

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer to array of parameter structures
  @param[in]      NumParams     - Number of parameter structures passed in
  @param[in,out]  DefaultValues - Pointer to array to store current values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SaveCurrentSettings (
  PSYSHOST          Host,
  UINT8             Socket,
  POWER_TRAIN_PARAM *Param,
  UINT8             NumParams,
  INT16             DefaultValues[MAX_PARAMS][MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8       ParamIndex;
  MPT_PT      ParamType;
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;

  RcDebugPrint (SDBG_MAX,
    "\nPrinting initialized array of cached values...\n\n");

  //------------------------------
  // Cycle through all parameters
  //------------------------------
  for (ParamIndex = 0; ParamIndex < NumParams; ParamIndex++) {
    RcDebugPrint (SDBG_MAX,
      "%a\n", Param[ParamIndex].Name);
    ParamType = Param[ParamIndex].ParamType;

    switch (ParamType) {
    case PerCh:
    case PerMc:
      Status = GetParamPerCh (Host, Socket, &Param[ParamIndex], ParamIndex, DefaultValues[ParamIndex]);
      break;
    case PerRankPerCh:
      Status = GetParamPerRank (Host, Socket, &Param[ParamIndex], ParamIndex, DefaultValues[ParamIndex]);
      break;
    case PerNibblePerCh:
    case PerBytePerCh:
      Status = GetParamPerChPerStrobeOrByte (Host, Socket, &Param[ParamIndex], ParamIndex, DefaultValues[ParamIndex]);
      break;
    case PerNibblePerRankPerCh:
    case PerBytePerRankPerCh:
      Status = GetParamPerStrobeOrByte (Host, Socket, &Param[ParamIndex], ParamIndex, DefaultValues[ParamIndex]);
      break;
    case PerBitPerRankPerCh:
      Status = GetParamPerRankPerBit (Host, Socket, &Param[ParamIndex], ParamIndex, DefaultValues[ParamIndex]);
      break;
    default:
        RcDebugPrint (SDBG_MAX,
          "\nUnrecognized parameter type: 0x%x\n", ParamType);
      Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
      break;
    }
  } // ParamIndex
  return Status;
} // SaveCurrentSettings

/**
  Program the same power training parameter setting for all channels.
  This is used for parameters that have one setting per-channel.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerCh (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  UINT8               Ch;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               Setting;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Setting = Param->SettingList[SettingIndex];
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm,
      GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Program the same power training parameter setting for all ranks.
  This is used for parameters that have one setting per-rank.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerRank (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               Setting;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Setting = Param->SettingList[SettingIndex];
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      // skip this settting if its not valid (DDR4 does not support RTTWRT = 60 for example)
      if (InValidSettingCheck (Host, Socket, Ch, Dimm, ParamGsm, Setting)) {
        continue;
      }

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, ParamGsm)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }
        GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, 0, ALL_BITS, DdrLevel, ParamGsm,
          GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Program the same power training parameter setting for all strobes/bytes in all ranks
  This is used for parameters that have one setting per-strobe for each rank.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerStrobeOrByte (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               Setting;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Setting = Param->SettingList[SettingIndex];
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  if (ParamGsm == TxEqTap2) {
    //
    // EQPost2Sign 0 for index 0-2 and 1 for 3-6
    //
    if (SettingIndex == 0) {
      ToggleEqPost2Sign (Host, Socket, ALL_DRAMS, INVERT_TRUE);
    }
    if (SettingIndex == 3) {
      ToggleEqPost2Sign (Host, Socket, ALL_DRAMS, INVERT_FALSE);
    }
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // If DRAM Rx Eq training is running and the DIMM does not support programmable CTLE, continue
      //
      if ((ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        continue;
      }

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          if ((ParamType == PerBytePerRankPerCh) && (Strobe >= MSVx8)) {
            break;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, ParamGsm,
            GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);

          //
          // RxEq, TxEq, TxEqTap2 are per nibble register, but it has DQ swizzling in strobe pair,
          // need to set same value for strobe pair
          //
          if ((ParamType == PerBytePerRankPerCh) && ((ParamGsm == TxEqTap2) || (ParamGsm == TxEq) || (ParamGsm == RxEq)))  {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, ALL_BITS, DdrLevel, ParamGsm,
              GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
          }
        } // Strobe loop
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Program the power training parameter setting Per Bit Per Rank Per Channel

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerBitPerRankPerCh (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Bit;
  UINT8               MSVx4 = GetMaxStrobeValid (Host);
  UINT8               MSVx8 = MSVx4 / 2;
  UINT8               MaxChDdr;
  UINT8               Strobe;
  UINT8               MaxBits = GetMaxBitsValid (Host);
  INT16               Setting;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  MRC_LT              Level;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  ParamGsm = Param->ParamGsm;
  GetMrcLevel (ParamGsm, &Level);
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Setting = Param->SettingList[SettingIndex];
  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }
        if (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, ParamGsm)) {
          continue;
        }

        for (Bit = 0; Bit < MaxBits; Bit++) {
          if (Level == LrbufLevel) {
            Strobe = Bit / 8;
          } else {
            if ((Bit / 4) % 2) {
              Strobe = (Bit / 8) + MSVx8;
            } else {
              Strobe = Bit / 8;
            }
          }

          if (Level == LrbufLevel) {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, (Bit % MAX_BITS_IN_BYTE), Level,
            ParamGsm, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
          } else {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, (Bit % BITS_PER_NIBBLE), Level,
            ParamGsm, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
          }
        } // Bit loop
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
} //SetParamPerBitPerRankPerCh

/**
  Program the same power training parameter setting for all strobes/bytes in all channels
  This is used for parameters that have one setting per-strobe for each channel.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerChannelPerStobeOrByte (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  UINT8               Ch;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               Setting;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Setting = Param->SettingList[SettingIndex];
  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  // Fill in margin entries for strobes for per Ch per Strobe
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    //
    // Set all strobes to the same value
    //
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, ParamGsm,
      GSM_UPDATE_CACHE | GSM_FORCE_WRITE, &Setting);
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Program a specfic setting for a power training parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParameter (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  MRC_STATUS          Status;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;

  ParamGsm = Param->ParamGsm;
  ParamType = Param->ParamType;

  //
  // Set parameters for each setting
  //
  switch (ParamType) {
  case PerCh:
  case PerMc:
    Status = SetParamPerCh (Host, Socket, Param, SettingIndex);
    break;
  case PerRankPerCh:
    Status = SetParamPerRank (Host, Socket, Param, SettingIndex);
    break;
  case PerNibblePerRankPerCh:
  case PerBytePerRankPerCh:
    Status = SetParamPerStrobeOrByte (Host, Socket, Param, SettingIndex);
    break;
  case PerNibblePerCh:
  case PerBytePerCh:
    Status = SetParamPerChannelPerStobeOrByte (Host, Socket, Param, SettingIndex);
    break;
  case PerBitPerRankPerCh:
    Status = SetParamPerBitPerRankPerCh (Host, Socket, Param, SettingIndex);
    break;
  default:
      RcDebugPrint (SDBG_MAX,
        "\nUnrecognized parameter type: 0x%x\n", ParamType);
    Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    break;
  }

  return Status;
} // SetParameter

/**
  Display a specfic setting for a power training parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a parameter structures
  @param[in]      ParamIndex    - Index into the parameter structure list
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
DisplayParameterSetting (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN      UINT8             SettingIndex
  )
{
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               Setting;

  Setting = Param[ParamIndex].SettingList[SettingIndex];
  ParamGsm = Param[ParamIndex].ParamGsm;
  ParamType = Param[ParamIndex].ParamType;

  if ((ParamGsm == DramDrvStr) || (ParamGsm == RxOdt)) {
    RcDebugPrint (SDBG_MAX,
                            "%a  = %d Ohms\t", Param[ParamIndex].Name, Setting);
  } else if (ParamGsm == WrOdt) {
    if (Setting == 0) {
      RcDebugPrint (SDBG_MAX,
        "%a 0x%x ==> Disable\t", Param[ParamIndex].Name, Setting);
    } else {
      RcDebugPrint (SDBG_MAX,
        "%a %d Ohms\t", Param[ParamIndex].Name, Setting);
    }
  } else if (ParamGsm == TxRon) {
    RcDebugPrint (SDBG_MAX,
      "%a %d Ohms \t", Param[ParamIndex].Name, Setting);
  } else if (ParamGsm == RxEq) {
    RcDebugPrint (SDBG_MAX,
      "Rx Eq 0x%x ", Setting);
  } else if (ParamGsm == RxBias) {
    RcDebugPrint (SDBG_MAX,
      "Rx Bias 0x%x ", Setting);
  } else if (ParamGsm == RxCtleC) {
    RcDebugPrint (SDBG_MAX,
      "CTLE C 0x%x ", Setting);
  } else if (ParamGsm == RxCtleR) {
    RcDebugPrint (SDBG_MAX,
      "CTLE R 0x%x ", Setting);
  } else if (ParamGsm == TxTco) {
    RcDebugPrint (SDBG_MAX,
      "TxTco 0x%x\n", Setting);
  } else if (ParamGsm == CmdTxEq) {
    RcDebugPrint (SDBG_MAX,
      "CmdTxEq 0x%x\n", Setting);
  } else if (ParamGsm == RxDqDelay) {
    RcDebugPrint (SDBG_MAX,
      "RxDqDelay 0x%x\n", Setting);
  } else if (ParamGsm == RxDfeCode) {
    RcDebugPrint (SDBG_MAX,
      "RxDfeCode 0x%x\n", Setting);
  } else if (ParamGsm == TxEchoCoef) {
    RcDebugPrint (SDBG_MAX,
      "TxEchoCoef 0x%x\n", Setting);
  } else if (ParamGsm == DramRxEq) {
    RcDebugPrint (SDBG_MAX,
      "DRAM Rx Eq 0x%x\n", Setting);
  } else if (ParamGsm == DbDfeTap1) {
    RcDebugPrint (SDBG_MAX,
      "DB DFE Tap 1 Coef 0x%x\n", Setting);
  } else if (ParamGsm == DbDfeTap2) {
    RcDebugPrint (SDBG_MAX,
      "DB DFE Tap 2 Coef 0x%x\n", Setting);
  } else if (ParamGsm == DbDfeTap3) {
    RcDebugPrint (SDBG_MAX,
      "DB DFE Tap 3 Coef 0x%x\n", Setting);
  } else if (ParamGsm == DbDfeTap4) {
    RcDebugPrint (SDBG_MAX,
      "DB DFE Tap 4 Coef 0x%x\n", Setting);
  } else {
    RcDebugPrint (SDBG_MAX,
      "%a 0x%x\n", Param[ParamIndex].Name, Setting);
  }
  return MRC_STATUS_SUCCESS;
} //DisplayParameterSetting

/**
  Save or Restore the current EyeSizeScaling value

  @param[in]      Host                 - Pointer to sysHost
  @param[in]      Socket               - Socket number
  @param[in,out]  EyeSizeScalingSaved  - Pointer to caller's storage
  @param[in]      SaveRestore          - RESTORE_SCALING to restore, else save
                                         to caller's storage

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SaveRestoreEyeSizeScalingSim (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             *EyeSizeScalingSaved,
  IN      UINT8             SaveRestore
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Initialize eyeSizeScaling for a specific power training parameter and setting
  if needed for simulation.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structure
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetEyeSizeScalingSim (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  )
{
  MRC_STATUS          Status = MRC_STATUS_SUCCESS;
  return Status;
}

/**
  Initialize margin results stuctures for a specific parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structure
  @param[in]      GroupIndex    - Index into Group array for group being margined
  @param[in,out]  MarginResults - Pointer to MarginResults structure
  @param[in,out]  Margins       - Pointer to Margins array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
InitializeMarginResults (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             GroupIndex,
  IN OUT  MARGIN_RESULTS    *MarginResults,
  IN OUT  INT16             Margins[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3]
  )
{
  UINT8               Ch;
  UINT8               Ch0Index;
  UINT16              Size;
  UINT8               MaxChDdr;
  UINT8               NumChPerMc;

  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  Size = sizeof (*MarginResults);

  // Initialize to 0 for all strobe values
  ZeroMem ((UINT8 *) MarginResults, Size);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    // populate CH0 of each IMC with max value margins in case that channel is not populated
    if (Param->ParamType == PerMc) {
      Ch0Index = (Ch / NumChPerMc) * NumChPerMc;
      Margins[Ch0Index][0][0][GroupIndex][0] = 0xff;
    }
  } // Ch loop
  return MRC_STATUS_SUCCESS;
}

/**
  Execute the signal margining

  @param[in]      Host                Pointer to sysHost
  @param[in]      Socket              Socket number
  @param[in]      Param               Pointer a specific parameter structure
  @param[in]      Group               GSM group that is being margined
  @param[in,out]  MarginResults       Pointer to MarginResults structure
  @param[in]      SafeMarginsArbiter  Pointer to a structure containing the context and callbacks used to set safe
                                      values for over-margined signals and restore the trained values afterward.

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteGetMargins (
  IN      PSYSHOST              Host,
  IN      UINT8                 Socket,
  IN      POWER_TRAIN_PARAM     *Param,
  IN      UINT8                 Group,
  IN OUT  MARGIN_RESULTS        *MarginResults,
  IN      SAFE_MARGINS_ARBITER  *SafeMarginsArbiter
  )
{
  struct bitMask      Filter;
  MRC_TT              TestType;


  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, Socket, &Filter);
  if (CheckCMDGroup (Group) || CheckCTLGroup (Group)) {
    TestType = CmdVrefTxEq;
  } else if (Param->ParamGsm == DramRxEq) {
    TestType = DramRxEqTest;
  } else {
    TestType = RmtDqTest;
  }

  // Get margins
  switch (Param->MarginType) {
  case GetMargin:
    if (Param->ParamType == PerBitPerRankPerCh) {
      if ((Param->ParamGsm == DbDfeTap1) || (Param->ParamGsm == DbDfeTap2) || (Param->ParamGsm == DbDfeTap3) || (Param->ParamGsm == DbDfeTap4)) {
        GetMargins (Host, Socket, DdrLevel, Group, Param->Mode, SCOPE_BIT, &Filter, &MarginResults->BitMargins,
                    Param->PatternLength, 1, 0, 0, WDB_BURST_LENGTH, TestType, SafeMarginsArbiter);
      } else {
        GetMargins (Host, Socket, DdrLevel, Group, Param->Mode, SCOPE_BIT, &Filter, &MarginResults->BitMargins,
                    Param->PatternLength, 0, 0, 0, WDB_BURST_LENGTH, TestType, SafeMarginsArbiter);
      }
    } else {
      GetMargins (Host, Socket, DdrLevel, Group, Param->Mode, SCOPE_STROBE, &Filter, &MarginResults->StrobeMargins,
        Param->PatternLength, 0, 0, 0, WDB_BURST_LENGTH, TestType, SafeMarginsArbiter);
    }
    break;
  default:
    break;
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Calculate margin eyesize from margining results

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Group         - GSM group that is being margined
  @param[in]      GroupIndex    - Index into Group array for group being margined
  @param[in]      Param         - Pointer a specific parameter structure
  @param[in,out]  MarginResults - Pointer to MarginResults structure
  @param[in,out]  Margins       - Pointer to Margins array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
CalcMarginEyeSize  (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group,
  IN      UINT8             GroupIndex,
  IN      POWER_TRAIN_PARAM *Param,
  IN OUT  MARGIN_RESULTS    *MarginResults,
  IN OUT  INT16             Margins[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3]
  )
{
  UINT8               Bit;
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  UINT8               MaxBits;
  UINT8               MaxStrobe;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxBits = GetMaxBitsValid (Host);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // Do not run margins if the mem-phase is DRAM Rx Eq and the DIMM does not support CTLE
      //
      if ((Param->ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, Group, DdrLevel);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }
        if (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm)) {
          continue;
        }

        if (Param->ParamType == PerBitPerRankPerCh) {
          for (Bit = 0; Bit < MaxBits; Bit++) {
            if (MarginResults->BitMargins.bits[Ch][ (*RankList)[Rank].rankIndex][Bit].n > MarginResults->BitMargins.bits[Ch][ (*RankList)[Rank].rankIndex][Bit].p) {
              MarginResults->BitMargins.bits[Ch][ (*RankList)[Rank].rankIndex][Bit].p = 0;
              MarginResults->BitMargins.bits[Ch][ (*RankList)[Rank].rankIndex][Bit].n = 0;
            }

            Margins[Ch][ (*RankList)[Rank].rankIndex][Bit][GroupIndex][0] =
              MarginResults->BitMargins.bits[Ch][ (*RankList)[Rank].rankIndex][Bit].p -
              MarginResults->BitMargins.bits[Ch][ (*RankList)[Rank].rankIndex][Bit].n;

          } // Bit
        } else { // ParamType
          for (Strobe = 0; Strobe < MSVx4; Strobe++) {
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }

            if (MarginResults->StrobeMargins.strobe[Ch][ (*RankList)[Rank].rankIndex][Strobe].n > MarginResults->StrobeMargins.strobe[Ch][ (*RankList)[Rank].rankIndex][Strobe].p) {
              MarginResults->StrobeMargins.strobe[Ch][ (*RankList)[Rank].rankIndex][Strobe].p = 0;
              MarginResults->StrobeMargins.strobe[Ch][ (*RankList)[Rank].rankIndex][Strobe].n = 0;
            }
            if ((MaxStrobe == MSVx8) && (Strobe >= MSVx8)) { // Copy over nibbble0 to nibble1 for each byte
              Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe-MSVx8][GroupIndex][0];
            } else {
              Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] =
                MarginResults->StrobeMargins.strobe[Ch][ (*RankList)[Rank].rankIndex][Strobe].p -
                MarginResults->StrobeMargins.strobe[Ch][ (*RankList)[Rank].rankIndex][Strobe].n;
            }
          } // Strobe loop
        } // ParamType
      } // Rank loop
    } // Dimm loop
  } // Ch loop
  return MRC_STATUS_SUCCESS;
} //CalcMarginEyeSize

/**
  Display margining eyesize results for per-rank, per-bit margins

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Group         - Group being margined
  @param[in]      GroupIndex    - Index into Group array for group being margined
  @param[in]      Param         - Pointer a specific parameter structure
  @param[in]      SettingIndex  - Index of current parameter setting
  @param[in,out]  Margins       - Pointer to Margins array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
DisplayMarginResultsPerRankPerBit (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group,
  IN      UINT8             GroupIndex,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex,
  IN OUT  INT16             Margins[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3]
  )
{
  UINT8               Bit;
  UINT8               EncodedNibble;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  UINT8               MaxBits;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  MaxBits = GetMaxBitsValid (Host);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "Per-bit\n");
        for (Bit = 0; Bit < MaxBits; Bit++) {
          if ((Bit / 4) % 2) {
            EncodedNibble = (Bit / 8) + MSVx8;
          } else {
            EncodedNibble = Bit / 8;
          }
          if ((Bit == 0) || (Bit == (MaxBits / 2))) {
            PrintPerBitHeading (Host, Socket, Bit, MaxBits);
          }

          if ((InValidSettingCheck (Host, Socket, Ch, Dimm, Param->ParamGsm, Param->SettingList[SettingIndex]) == 0) &&
              (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm) == 0)) {
            RcDebugPrint (SDBG_MAX,
              " %3d", Margins[Ch][(*RankList)[Rank].rankIndex][Bit][GroupIndex][0]);
          }
        } // Bit loop
        RcDebugPrint (SDBG_MAX, "\n\n");
      } // Rank
    } // Dimm
  } // Ch

  return MRC_STATUS_SUCCESS;
}

/**
  Display margining eyesize results for per-rank, per-strobe margins

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Group         - Group being margined
  @param[in]      GroupIndex    - Index into Group array for group being margined
  @param[in]      Param         - Pointer a specific parameter structure
  @param[in]      SettingIndex  - Index of current parameter setting
  @param[in,out]  Margins       - Pointer to Margins array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
DisplayMarginResultsPerRankPerStrobe (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group,
  IN      UINT8             GroupIndex,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex,
  IN OUT  INT16             Margins[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3]
  )
{
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  UINT8               MaxStrobe;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               MSVx4 = GetMaxStrobeValid (Host);
  BOOLEAN             FaultyPartFlag;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MAX, "Strobe        ");

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_MAX,
                "%2d  ",Strobe);
  }
  RcDebugPrintLine (SDBG_MAX, 84, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      FaultyPartFlag = FALSE;
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // If DRAM Rx Eq training is running and the DIMM does not support programmable CTLE, continue
      //
      if ((Param->ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        //
        // This is the print statement during training
        //
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "DIMM does not support programmable CTLE.\n");
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, Group, DdrLevel);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        if ((InValidSettingCheck (Host, Socket, Ch, Dimm, Param->ParamGsm, Param->SettingList[SettingIndex]) == 0) &&
            (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm) == 0)) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "");
        }
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }

          if ((InValidSettingCheck (Host, Socket, Ch, Dimm, Param->ParamGsm, Param->SettingList[SettingIndex]) == 0) &&
              (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm) == 0)) {
            RcDebugPrint (SDBG_MAX,
              "%3d ", Margins[Ch][(*RankList)[Rank].rankIndex][Strobe][GroupIndex][0]);
          }
        } // Strobe loop

        //
        // For FPT(Faulty Part Tracking) feature: To avoid divide by 0, use neighbor nibble setting to replace
        //
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }

          if ((*RankList)[Rank].faultyParts[Strobe]) {
            FaultyPartFlag = TRUE;
            if (Strobe == 9) {
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe + 1][GroupIndex][0];
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe - 9][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe - 9 + 1][GroupIndex][0];
            } else if (Strobe > 9){
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe - 1][GroupIndex][0];
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe - 9][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe - 9 - 1][GroupIndex][0];
            } else if (Strobe == 0) {
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe + 1][GroupIndex][0];
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe + 9][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe + 9 + 1][GroupIndex][0];
            } else {
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe - 1][GroupIndex][0];
              Margins[Ch][(*RankList)[Rank].rankIndex][Strobe + 9][GroupIndex][0] = Margins[Ch][(*RankList)[Rank].rankIndex][Strobe + 9 - 1][GroupIndex][0];
            }
          }
        } // strobe loop

        if (FaultyPartFlag) {
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_SUBCH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "\nAdjust for faulty part -------------------------------------------------------------- \n");
          RcDebugPrintWithDevice (SDBG_MAX, Strobe, Ch, NO_SUBCH, Dimm, Rank, NO_STROBE, NO_BIT, "");
          for (Strobe = 0; Strobe < MSVx4; Strobe++) {
            if ((InValidSettingCheck (Host, Socket, Ch, Dimm, Param->ParamGsm, Param->SettingList[SettingIndex]) == 0) &&
              (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm) == 0)){
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_SUBCH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
              "%3d ", Margins[Ch][(*RankList)[Rank].rankIndex][Strobe][GroupIndex][0]);
            }
          }
        }

        RcDebugPrint (SDBG_MAX, "\n");
      } // Rank loop
    } // dimm loop
  } // Ch loop
  return MRC_STATUS_SUCCESS;
}

/**
  Display margining eyesize results

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Group         - Group being margined
  @param[in]      GroupIndex    - Index into Group array for group being margined
  @param[in]      Param         - Pointer a specific parameter structure
  @param[in]      SettingIndex  - Index of current parameter setting
  @param[in,out]  Margins       - Pointer to Margins array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
DisplayMarginResults (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group,
  IN      UINT8             GroupIndex,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex,
  IN OUT  INT16             Margins[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3]
  )
{
  CHAR8               *GroupString;

  // Print results
  switch (Group) {
  case TxDqDelay:
    GroupString = "Tx Eye Widths";
    break;
  case TxVref:
    GroupString = "Tx Eye Heights";
    break;
  case RxDqsDelay:
    GroupString = "Rx Eye Widths";
    break;
  case RxVref:
    GroupString = "Rx Eye Heights";
    break;
  case RxSampler:
    GroupString = "Rx Sampler Heights";
    break;
  case CmdAll:
    GroupString = "CMD Eye Widths";
    break;
  case CmdVref:
    GroupString = "CMD Eye Heights";
    break;
  default:
    GroupString = "Invalid group";
    break;
  }
  RcDebugPrint (SDBG_MAX, "\n%a\n", GroupString);

  switch (Param->ParamType) {
  case PerBitPerRankPerCh:
    DisplayMarginResultsPerRankPerBit (Host, Socket, Group, GroupIndex, Param, SettingIndex, Margins);
    break;
  default:
    DisplayMarginResultsPerRankPerStrobe (Host, Socket, Group, GroupIndex, Param, SettingIndex, Margins);
    break;
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Calculate the composite margin eye, if required

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Group         - The signal group being margined
  @param[in]      GroupIndex    - Index into Group array for group being margined
  @param[in]      Param         - Pointer a specific parameter structure
  @param[in]      SettingIndex  - Index of current parameter setting
  @param[in,out]  Margins       - Pointer to Margins array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
CalcCompositeEye (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group,
  IN      UINT8             GroupIndex,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex,
  IN OUT  INT16             Margins[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3]
  )
{
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  UINT8               Ch0Index;
  UINT8               MaxStrobe;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;
  UINT8 NumChPerMc;

  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (Param->ParamType == PerBitPerRankPerCh) {
    return MRC_STATUS_SUCCESS;
  }

  if ((Param->ParamType == PerNibblePerRankPerCh) && (Param->ParamGsm != DramRxEq)){
    return MRC_STATUS_SUCCESS;
  }
  // Fill in margin entries for strobes for each case and determine the composite eye --> place it in 0 index
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // Skip x4 DRAM DramRxEq since its ParamType is PerNibblePerRankPerCh
      //
      if ((Param->ParamGsm == DramRxEq) && ((*DimmNvList)[Dimm].x4Present == 1)) {
        continue;
      }

      //
      // If DRAM Rx Eq training is running and the DIMM does not support programmable CTLE, continue
      //
      if ((Param->ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, Group, DdrLevel);

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          // Per Ch per byte or TXVREF margin on x8 DIMM

          // zero out the margins if its an invalid Settings
          if (InValidSettingCheck (Host, Socket, Ch, Dimm, Param->ParamGsm, Param->SettingList[SettingIndex]) ||
              (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm))) {
            Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] = 0;
          }

          if (Param->ParamType == PerBytePerRankPerCh) {
            if ((Strobe >= MSVx8) && (MaxStrobe == MSVx4)) {
              // lower strobes Margins are already in the right place, this copies over upper strobes
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe-MSVx8][GroupIndex][0]) {
                Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe-MSVx8][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            }
          } // per Ch per byte

          //
          // x8 DRAM RXEQ need to be calculated Composite eye from lower and upper nibble
          //
          if ((Param->ParamGsm == DramRxEq) && (Strobe >= MSVx8) && ((*DimmNvList)[Dimm].x4Present == 0)) {
              // lower strobes Margins are already in the right place, this copies over upper strobes
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe-MSVx8][GroupIndex][0]) {
                Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe-MSVx8][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
          }

          if (Param->ParamType == PerBytePerCh) {
            if (Strobe < MSVx8) {
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][0][Strobe][GroupIndex][0]) {
                Margins[Ch][0][Strobe][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            } else if (MaxStrobe == MSVx4) {
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][0][Strobe-MSVx8][GroupIndex][0]) {
                Margins[Ch][0][Strobe-MSVx8][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            }
          } // per Ch per byte

          // Per Ch per Strobe
          if (Param->ParamType == PerNibblePerCh) {
            if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][0][Strobe][GroupIndex][0]) {
              Margins[Ch][0][Strobe][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
            }
          } // per Ch per byte

          // Per Rank
          if (Param->ParamType == PerRankPerCh) {
            if (Strobe < MSVx8) {
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][ (*RankList)[Rank].rankIndex][0][GroupIndex][0]) {
                Margins[Ch][ (*RankList)[Rank].rankIndex][0][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            } else if (MaxStrobe == MSVx4) { // if all 18 strobes are in use
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][ (*RankList)[Rank].rankIndex][0][GroupIndex][0]) {
                Margins[Ch][ (*RankList)[Rank].rankIndex][0][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            }
          } // per Rank

          // Per Ch  save in D0:R0:S0 for each CH                                   // special case for nontarget ODT training
          if ((Param->ParamType == PerCh) || (Param->ParamGsm == NomOdt) || (Param->ParamGsm == ParkOdt)) {
            if (Strobe < MSVx8) {
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][0][0][GroupIndex][0]) {
                Margins[Ch][0][0][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            } else if (MaxStrobe == MSVx4) {
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch][0][0][GroupIndex][0]) {
                Margins[Ch][0][0][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            }
          } // PerCh

          // per MC - get composite Margins and save in CH0:D0:R0:S0 for each IMC
          if (Param->ParamType == PerMc) {
            Ch0Index = (Ch / NumChPerMc) * NumChPerMc;
            if (Strobe < MSVx8) {
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch0Index][0][0][GroupIndex][0]) {
                Margins[Ch0Index][0][0][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            } else if (MaxStrobe == MSVx4) {
              if (Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0] < Margins[Ch0Index][0][0][GroupIndex][0]) {
                Margins[Ch0Index][0][0][GroupIndex][0] = Margins[Ch][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              }
            }
          } // PerMc
        } // Strobe loop
      } // Rank loop
    } // Dimm loop
  } // Ch loop
  return MRC_STATUS_SUCCESS;
}

/**
  Calculate the average margin eye, if required

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Group           - The signal group being margined
  @param[in]      GroupIndex      - Group index
  @param[in]      Param           - Pointer a specific parameter structure
  @param[in]      SettingIndex    - Index of current parameter setting
  @param[in,out]  Margins         - Pointer to Margins array
  @param[in,out]  AverageMargins  - Pointer to AveragesMargins structure

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
CalcAverageMargins (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group,
  IN      UINT8             GroupIndex,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex,
  IN OUT  INT16             Margins[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3],
  IN OUT  AVERAGE_MARGINS   *AverageMargins
  )
{
  UINT8               Bit;
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               FirstDimm;
  UINT8               Ch;
  UINT8               Ch0Index;
  UINT8               MaxBits;
  MPT_PT              ParamType;
  INT16               Margin;
  INT16               MinusOneMargin;
  INT16               PlusOneMargin;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;
  UINT8 NumChPerMc;

  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Calculate Average Margins
  //
  MaxBits = GetMaxBitsValid (Host);
  ParamType = Param->ParamType;

  // need new channel loop for worste case eye purposes PerMc
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // If DRAM Rx Eq training is running and the DIMM does not support programmable CTLE, continue
      //
      if ((Param->ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        if (!FindFirstDimmInChannel (Socket, Ch, &FirstDimm)) {
          RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
        }

        if (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm)) {
          continue;
        }

        // skip unused variables/ranks
        if ((ParamType == PerMc || ParamType == PerBytePerCh) && ((Dimm != FirstDimm) || (Rank > 0))) {
          continue;
        }
        if ((ParamType == PerDimmPerCh) && (Rank > 0)) {
          continue;
        }

        if (ParamType == PerBitPerRankPerCh) {
          for (Bit = 0; Bit < MaxBits; Bit++) {
            Margin = Margins[Ch][ (*RankList)[Rank].rankIndex][Bit][GroupIndex][0];
            Margin = Margin * 10;
            AverageMargins->PerBit[Ch][ (*RankList)[Rank].rankIndex][Bit][GroupIndex][SettingIndex] = Margin;
          }
        } else {
          for (Strobe = 0; Strobe < MSVx4; Strobe++) {
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }

            // skip unused variables/strobes
            if ((ParamType == PerRankPerCh || ParamType == PerCh || ParamType == PerMc || ParamType == PerDimmPerCh) && (Strobe > 0)) {
              break;
            }
            if ((ParamType == PerBytePerRankPerCh || ParamType == PerBytePerCh) & (Strobe >= MSVx8)) {
              break;
            }
            //
            // Margin for the current SettingIndex = (Margin[SettingIndex - 1] + (2 * Margin[SettingIndex]) + Margin[SettingIndex + 1]) * 10 / 4
            // The "+ 2" is to make the rounding correct
            //
            if (ParamType == PerMc) {
              Ch0Index = (Ch / NumChPerMc) * NumChPerMc;
            } else {
              Ch0Index = Ch;
            }

            // can't start calculations until > 0
            if ((Param->ParamGsm == TxEq) || (Param->ParamGsm == TxEqTap2) || (Param->ParamGsm == TxImode) || (Param->ParamGsm == RxCtleC)) {
              if (SettingIndex > 0) {
                Margin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][1];
                PlusOneMargin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
                if (SettingIndex == 1) {
                  MinusOneMargin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][1];
                } else {
                  MinusOneMargin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][2];
                }
                // calculate average Margin
                AverageMargins->PerStrobe[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][SettingIndex-1] =
                  ((MinusOneMargin + (2 * Margin) + PlusOneMargin) *10) / 4;
              }

              // shift Margins over in saved array
              Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][2] = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][1];
              Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][1] = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];

              // last Margin point
              if (SettingIndex == (Param->NumSettings - 1)) {
                Margin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][1];
                PlusOneMargin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][1];
                MinusOneMargin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][2];
                AverageMargins->PerStrobe[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][SettingIndex] =
                  ((MinusOneMargin + (2 * Margin) + PlusOneMargin) / 4) * 10;
              }

            } else {
              Margin = Margins[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][0];
              if ((Param->ParamGsm != NomOdt) && (Param->ParamGsm != ParkOdt)) {
                Margin = Margin * 10;
              }
              AverageMargins->PerStrobe[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe][GroupIndex][SettingIndex] = Margin;
            }
          } // Strobe loop
        } // PerBitPerRankPerCh
      } // Rank loop
    } // Dimm loop
  } // Ch loop
  return MRC_STATUS_SUCCESS;
} //CalcAverageMargins

/**
  Find the best setting for the parameter

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      NumGroups       - Number of signal groups being margined
  @param[in]      Param           - Pointer a specific parameter structure
  @param[in,out]  AverageMargins  - Pointer to AveragesMargins structure
  @param[in,out]  BestSetting     - Pointer to best settings array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
FindBestSetting (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             NumGroups,
  IN      POWER_TRAIN_PARAM *Param,
  IN      AVERAGE_MARGINS   *AverageMargins,
  IN OUT  INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS]
  )
{
  UINT8               Bit;
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               FirstDimm;
  UINT8               Ch;
  UINT8               Ch0Index;
  UINT8               MaxBits;
  MPT_PT              ParamType;
  UINT16              Size;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;
  UINT8 NumChPerMc;

  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxBits = GetMaxBitsValid (Host);
  Size = sizeof (INT16) * MAX_CH * MAX_RANK_CH * MAX_BITS;
  ZeroMem ((UINT8 *) BestSetting, Size);

  ParamType = Param->ParamType;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // Do not run margins if the mem-phase is DRAM Rx Eq and the DIMM does not support CTLE
      //
      if ((Param->ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, Param->ParamGsm)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        if (!FindFirstDimmInChannel (Socket, Ch, &FirstDimm)) {
          RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
        }

        // skip unused variables
        if ((ParamType == PerMc || ParamType == PerBytePerCh) && ((Dimm != FirstDimm) || (Rank > 0))) {
          continue;
        }
        if ((ParamType == PerDimmPerCh) && (Rank > 0)) {
          continue;
        }

        if (ParamType == PerBitPerRankPerCh) {
          for (Bit = 0; Bit < MaxBits; Bit++) {
            if (IsBitNotValid (Host, Bit)) {
              continue;
            }
            //
            // Conditional for 5 tick minimum check to reset to default values
            //
            BestSetting[Ch][ (*RankList)[Rank].rankIndex][Bit] = FindOptimalTradeOff (Host, Socket, Ch, Dimm, Rank, NO_STROBE, Bit,
                                                                   (PER_GROUP_SETTINGS *) AverageMargins->PerBit[Ch][ (*RankList)[Rank].rankIndex][Bit],
                                                                   Param->NumSettings, NumGroups, ParamType);
          }
        } else {
          for (Strobe = 0; Strobe < MSVx4; Strobe++) {
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }
            if ((ParamType == PerRankPerCh || ParamType == PerCh || ParamType == PerMc || ParamType == PerDimmPerCh) && (Strobe > 0)) {
              break;
            }
            if ((ParamType == PerBytePerRankPerCh || ParamType == PerBytePerCh) & (Strobe >= MSVx8)) {
              break;
            }

            if (ParamType == PerMc) {
              Ch0Index = (Ch / NumChPerMc) * NumChPerMc;
            } else {
              Ch0Index = Ch;
            }

            //
            // Conditional for 5 tick minimum check to reset to default values
            //
            BestSetting[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe] =
            FindOptimalTradeOff (Host, Socket, Ch, Dimm, Rank, Strobe, NO_BIT, (PER_GROUP_SETTINGS *) AverageMargins->PerStrobe[Ch0Index][ (*RankList)[Rank].rankIndex][Strobe],
              Param->NumSettings, NumGroups, ParamType);

          } // Strobe loop
        } // PerBitPerRankPerCh
      } // Rank loop
    } // Dimm loop
  } // Ch loop
  return MRC_STATUS_SUCCESS;
} // FindBestSetting

/**
  Program the optimal settings for a parameter
  This applies to parameters that are programmed per-channel

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Param           - Pointer the parameter
  @param[in]      BestSetting     - Pointer to best settings array
  @param[in]      DefaultValues   - Pointer to list of default values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetOptimalSettingsPerCh (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS],
  IN      INT16             DefaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Ch;
  MRC_GT              ParamGsm;
  INT16               CurVal = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ParamGsm = Param->ParamGsm;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if (BestSetting[Ch][0][0] == POWERTRAINING_DEFAULT_SETTING) {
      CurVal = DefaultValues[Ch][0][0][0];
    } else {
      CurVal = Param->SettingList[BestSetting[Ch][0][0]];
    }
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm, GSM_UPDATE_CACHE, &CurVal);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "%2d\n", CurVal);
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Program the optimal settings for a parameter
  This applies to parameters that are programmed per-MC

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Param           - Pointer the parameter
  @param[in]      BestSetting     - Pointer to best settings array
  @param[in]      DefaultValues   - Pointer to list of default values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetOptimalSettingsPerMc (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS],
  IN      INT16             DefaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Ch;
  UINT8               Ch0Index;
  UINT8               Mode;
  MRC_GT              ParamGsm;
  INT16               CurVal = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;
  UINT8               NumChPerMc;

  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ParamGsm = Param->ParamGsm;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    Ch0Index = (Ch / NumChPerMc) * NumChPerMc;
    if (ParamGsm == TxTco) {
      Mode = GSM_UPDATE_CACHE | GSM_FORCE_WRITE;
    } else {
      Mode = GSM_UPDATE_CACHE;
    }
    if (BestSetting[Ch0Index][0][0] == POWERTRAINING_DEFAULT_SETTING) {
      CurVal = DefaultValues[Ch][0][0][0];
    } else {
      CurVal = Param->SettingList[BestSetting[Ch0Index][0][0]];
    }
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, 0, 0, 0, ALL_BITS, DdrLevel, ParamGsm, Mode, &CurVal);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "%2d\n", CurVal );
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Program the optimal settings for a parameter
  This applies to parameters that have one setting per rank

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Param           - Pointer the parameter
  @param[in]      BestSetting     - Pointer to best settings array
  @param[in]      DefaultValues   - Pointer to list of default values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetOptimalSettingsPerRank (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS],
  IN      INT16             DefaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  MRC_GT              ParamGsm;
  INT16               CurVal = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ParamGsm = Param->ParamGsm;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        if ((BestSetting[Ch][ (*RankList)[Rank].rankIndex][0] == POWERTRAINING_DEFAULT_SETTING) ||
            SkipRankCheck (Host, Socket, Ch, Dimm, Rank, ParamGsm)) {
          CurVal = DefaultValues[Ch][Dimm][Rank][0];
        } else {
          CurVal = Param->SettingList[BestSetting[Ch][ (*RankList)[Rank].rankIndex][0]];
        }
        GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, 0, ALL_BITS, DdrLevel, ParamGsm, GSM_UPDATE_CACHE, &CurVal);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "%2d\n", CurVal );
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Program the optimal settings for a parameter
  This applies to parameters that have one setting per strobe or byte in every rank.

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Param           - Pointer the parameter
  @param[in]      BestSetting     - Pointer to best settings array
  @param[in]      DefaultValues   - Pointer to list of default values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetOptimalSettingsPerStrobe (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS],
  IN      INT16             DefaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               CurVal = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ParamType = Param->ParamType;
  ParamGsm = Param->ParamGsm;

  RcDebugPrint (SDBG_MAX, "Strobe        ");
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    if ((ParamType == PerBytePerRankPerCh) && (Strobe >= MSVx8)) {
      break;
    }
    RcDebugPrint (SDBG_MAX,
      "%2d  ",Strobe);
  }

  RcDebugPrintLine (SDBG_MAX, 84, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // Do not run margins if the mem-phase is DRAM Rx Eq and the DIMM does not support CTLE
      //
      if ((ParamGsm == DramRxEq) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        //
        // This is the print statement for the optimal values
        //
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "DIMM does not support programmable CTLE.\n");
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, " ");
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {

          if ((ParamGsm == TxEq) || (ParamGsm == TxEqTap2)) {
            //
            // For the TxEq/TxEqTap2, all LRDIMM ranks inside a dimm are the same package rank and they should
            // have the same setting of rank 0 of the dimm.
            //
            if (IsLrdimmPresent (Socket, Ch, Dimm)) {
              DefaultValues[Ch][Dimm][Rank][Strobe] = DefaultValues[Ch][Dimm][0][Strobe];
              BestSetting[Ch][(*RankList)[Rank].rankIndex][Strobe] =
                BestSetting[Ch][(*RankList)[0].rankIndex][Strobe];
            }

            //
            // TxEq tap2 needs to have EqPost2Sign set based on the Setting List index:
            // EQPost2Sign 0 for index 0-2 and 1 for 3-6 see the training spec for more reference
            //
            if (ParamGsm == TxEqTap2) {
              if (BestSetting[Ch][ (*RankList)[Rank].rankIndex][Strobe] < 3) {
                ToggleEqPost2Sign (Host, Socket, Strobe, INVERT_TRUE);
              } else {
                ToggleEqPost2Sign (Host, Socket, Strobe, INVERT_FALSE);
              }
            }
          }
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }

          //
          // RxEq should have the same value per channel
          // Update all ranks to rank0 (of the channel) RxEq setting since CalccompositeEye and FindBestSetting always using rank0 to calculate
          //
          if (ParamGsm == RxEq) {
            DefaultValues[Ch][Dimm][Rank][Strobe] = DefaultValues[Ch][0][0][Strobe];
            BestSetting[Ch][(*RankList)[Rank].rankIndex][Strobe] = BestSetting[Ch][0][Strobe];
          }

          if ((ParamType == PerBytePerRankPerCh) && (Strobe >= MSVx8)) {
            break;
          }

          //
          // The upper nibble of x8 DRAM RxEq need to be skipped
          //
          if ((ParamGsm == DramRxEq) && (Strobe >= MSVx8) && ((*DimmNvList)[Dimm].x4Present == 0)) {
            break;
          }

          if (BestSetting[Ch][ (*RankList)[Rank].rankIndex][Strobe] == POWERTRAINING_DEFAULT_SETTING) {
            CurVal = DefaultValues[Ch][Dimm][Rank][Strobe];
          } else {
            CurVal = Param->SettingList[BestSetting[Ch][ (*RankList)[Rank].rankIndex][Strobe]];
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, ParamGsm, GSM_UPDATE_CACHE, &CurVal);

          //
          // RxEq, TxEq, TxEqTap2 are per nibble register, but it has DQ swizzling in strobe pair,
          // need to set same value for strobe pair
          //
          if ((ParamType == PerBytePerRankPerCh) && ((ParamGsm == TxEqTap2) || (ParamGsm == TxEq) || (ParamGsm == RxEq)))  {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, ALL_BITS, DdrLevel, ParamGsm, GSM_UPDATE_CACHE, &CurVal);
          }

          RcDebugPrint (SDBG_MAX, "%2d  ", CurVal);
        } // Strobe loop
        RcDebugPrint (SDBG_MAX, "\n");
      } // Rank loop
    } // Dimm loop
  } // Ch loop

  return MRC_STATUS_SUCCESS;
}

/**
  Program the optimal settings for a parameter
  This applies to parameters that have one setting per strobe or byte in every channel.

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Param           - Pointer the parameter
  @param[in]      ParamIndex      - Index into the parameter
  @param[in]      BestSetting     - Pointer to best settings array
  @param[in]      DefaultValues   - Pointer to list of default values for this parameter

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetOptimalSettingsPerChPerStrobe (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN      INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS],
  IN      INT16             DefaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  INT16               CurVal = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = MSVx4 / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ParamType = Param->ParamType;
  ParamGsm = Param->ParamGsm;

  RcDebugPrint (SDBG_MAX, "Strobe  Param  ");
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    if ((Strobe >= MSVx8) && (ParamType == PerBytePerCh)) {
      break;
    }
    RcDebugPrint (SDBG_MAX,
      "%2d  ",Strobe);
  }
  RcDebugPrintLine (SDBG_MAX, 84, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  // Fill in margin entries for strobes for per Ch per Strobe
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    // Per Ch per byte
    Dimm = 0;
    Rank = 0;

    RankList = GetRankNvList (Host, Socket, Ch, Dimm);

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "    %d  ", ParamIndex);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      if ((Strobe >= MSVx8) && (ParamType == PerBytePerCh)) {
        break;
      }
      if (BestSetting[Ch][ (*RankList)[Rank].rankIndex][Strobe] == POWERTRAINING_DEFAULT_SETTING) {
        CurVal = DefaultValues[Ch][Dimm][Rank][Strobe];
      } else {
        CurVal = Param->SettingList[BestSetting[Ch][ (*RankList)[Rank].rankIndex][Strobe]];
      }
      GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, ParamGsm,
        GSM_UPDATE_CACHE, &CurVal);
      RcDebugPrint (SDBG_MAX,
        "%3d ", CurVal);
    } // Strobe loop
    RcDebugPrint (SDBG_MAX, "\n");

  } // Ch loop
  RcDebugPrint (SDBG_MAX, "\n");

  return MRC_STATUS_SUCCESS;
}

/**
  Program the optimal settings for a parameter
  This applies to parameters that have one setting per bit in every rank.

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Param           - Pointer the parameter
  @param[in]      ParamIndex      - Index into the parameter list
  @param[in]      BestSetting     - Pointer to best settings array
  @param[in]      DefaultValues   - Pointer to list of default values for this parameter

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetOptimalSettingsPerRankPerBit (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             ParamIndex,
  IN      INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS],
  IN      INT16             DefaultValues[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  UINT8               Bit;
  UINT8               MaxBits;
  UINT8               Strobe;
  UINT8               Rank;
  UINT8               Dimm;
  UINT8               Ch;
  MPT_PT              ParamType;
  MRC_GT              ParamGsm;
  MRC_LT              Level;
  INT16               CurVal = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  ParamGsm = Param->ParamGsm;
  GetMrcLevel (ParamGsm, &Level);
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ParamType = Param->ParamType;
  MaxBits = GetMaxBitsValid (Host);

  // Fill in margin entries for bits
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (SkipRankCheck (Host, Socket, Ch, Dimm, Rank, ParamGsm)) {
          continue;
        }

        if (IsLrdimmPresent (Socket, Ch, Dimm) && Rank > 0) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "Per-bit -  Param: %d\n", ParamIndex);

        for (Bit = 0; Bit < MaxBits; Bit++) {
          if (Level == LrbufLevel) {
            Strobe = Bit / 8;
          } else {
            if ((Bit / 4) % 2) {
              Strobe = (Bit / 8) + MSVx8;
            } else {
              Strobe = Bit / 8;
            }
          }

          if ((Bit == 0) || (Bit == (MaxBits / 2))) {
            PrintPerBitHeading (Host, Socket, Bit, MaxBits);
          }

          if (BestSetting[Ch][ (*RankList)[Rank].rankIndex][Bit] == POWERTRAINING_DEFAULT_SETTING) {
            CurVal = DefaultValues[Ch][Dimm][Rank][Bit];
          } else {
            CurVal = Param->SettingList[BestSetting[Ch][ (*RankList)[Rank].rankIndex][Bit]];
          }
          if (Level == LrbufLevel) {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, (Bit % MAX_BITS_IN_BYTE), Level, ParamGsm, GSM_UPDATE_CACHE, &CurVal);
          } else {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, (Bit % BITS_PER_NIBBLE), Level, ParamGsm, GSM_UPDATE_CACHE, &CurVal);
          }
          RcDebugPrint (SDBG_MAX, " %3d", CurVal);
        } // bit loop
        RcDebugPrint (SDBG_MAX, "\n\n");
      } // Rank
    } // Dimm
  } // Ch loop
  RcDebugPrint (SDBG_MAX, "\n");

  return MRC_STATUS_SUCCESS;
}

/**
  Program the optimal settings for all parameters

  @param[in]      Host            - Pointer to sysHost
  @param[in]      Socket          - Socket number
  @param[in]      Param           - Pointer the parameter list
  @param[in]      NumParams       - Number of paramters
  @param[in]      BestSetting     - Pointer to best settings array
  @param[in]      DefaultValues   - Pointer to list of default values

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetOptimalSettings (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             NumParams,
  IN      INT16             BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS],
  IN      INT16             DefaultValues[MAX_PARAMS][MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS]
  )
{
  MRC_STATUS          Status = MRC_STATUS_SUCCESS;
  MRC_GT              ParamGsm;
  MPT_PT              ParamType;
  UINT8               ParamIndex;

  //
  // Set optimal Settings
  //
  RcDebugPrint (SDBG_MAX,
    "START_OPTIMAL_TRAINING_RESULTS\n");

  for (ParamIndex = 0; ParamIndex < NumParams; ParamIndex++) {
    RcDebugPrint (SDBG_MAX,
      "%a\n", Param[ParamIndex].Name);

    ParamType = Param[ParamIndex].ParamType;
    ParamGsm = Param[ParamIndex].ParamGsm;

    switch (ParamType) {
    case PerCh:
      Status = SetOptimalSettingsPerCh (Host, Socket, &Param[ParamIndex], BestSetting, DefaultValues[ParamIndex]);
      break;
    case PerMc:
      Status = SetOptimalSettingsPerMc (Host, Socket, &Param[ParamIndex], BestSetting, DefaultValues[ParamIndex]);
      break;
    case PerRankPerCh:
      Status = SetOptimalSettingsPerRank (Host, Socket, &Param[ParamIndex], BestSetting, DefaultValues[ParamIndex]);
      break;
    case PerNibblePerRankPerCh:
    case PerBytePerRankPerCh:
      Status = SetOptimalSettingsPerStrobe (Host, Socket, &Param[ParamIndex], BestSetting, DefaultValues[ParamIndex]);
      break;
    case PerNibblePerCh:
    case PerBytePerCh:
      Status = SetOptimalSettingsPerChPerStrobe (Host, Socket, &Param[ParamIndex], ParamIndex, BestSetting, DefaultValues[ParamIndex]);
      break;
    case PerBitPerRankPerCh:
      Status = SetOptimalSettingsPerRankPerBit (Host, Socket, &Param[ParamIndex], ParamIndex, BestSetting, DefaultValues[ParamIndex]);
      break;
    default:
        RcDebugPrint (SDBG_MAX,
          "\nUnrecognized parameter type: 0x%x\n", ParamType);
      Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
      break;
    }
    if (Status != MRC_STATUS_SUCCESS) {
      // Don't continue if something went wrong
      break;
    }
  } // optimal setting loop

  RcDebugPrint (SDBG_MAX,
    "END_OPTIMAL_TRAINING_RESULTS\n");

  return Status;
}

/**
  Set safe margins for power training.

  @param[in]  SafeMarginsArbiter  Pointer to a structure containing the context and callbacks used to set safe values
                                  for over-margined signals and restore the trained values afterward.

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
SetSafePowerTrainingMargins (
  IN  SAFE_MARGINS_ARBITER  *SafeMarginsArbiter
  )
{
  INT16                               BestSetting[MAX_CH][MAX_RANK_CH][MAX_BITS];
  SAFE_MARGINS_POWER_TRAINING_CONTEXT *Context;
  PSYSHOST                            Host;
  UINT8                               NumParams;
  POWER_TRAIN_PARAM                   *Param;
  INT16                               (*SafeValues)[MAX_PARAMS][MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS];
  UINT32                              Signature;
  UINT8                               Socket;

  Host = GetSysHostPointer ();

  // Initialize BestSetting to the force the default value in SetOptimalSettings.
  SetMem (BestSetting, sizeof (BestSetting), POWER_TRAINING_DEFAULT_SETTING_UINT8);

  // Confirm that the context is a SAFE_MARGINS_POWER_TRAINING_CONTEXT instance.
  Context = (SAFE_MARGINS_POWER_TRAINING_CONTEXT *)(SafeMarginsArbiter->Context);
  Signature = Context->Signature;
  RC_FATAL_ERROR (Signature == POWER_TRAINING_SIGNATURE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_183);

  // Get the context members.
  NumParams = Context->NumParams;
  Param = Context->Param;
  SafeValues = Context->SafeValues;
  Socket = Context->Socket;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Setting safe margins\n");
  return SetOptimalSettings (Host, Socket, Param, NumParams, BestSetting, *SafeValues);
}

/**
  Restore power training margins.

  @param[in]  SafeMarginsArbiter  Pointer to a structure containing the context and callbacks used to set safe values
                                  for over-margined signals and restore the trained values afterward.

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
RestorePowerTrainingMargins (
  IN  SAFE_MARGINS_ARBITER  *SafeMarginsArbiter
  )
{
  SAFE_MARGINS_POWER_TRAINING_CONTEXT *Context;
  PSYSHOST                            Host;
  UINT8                               NumParams;
  POWER_TRAIN_PARAM                   *Param;
  UINT8                               ParamIndex;
  MRC_STATUS                          TempStatus = MRC_STATUS_SUCCESS;
  UINT8                               SettingIndex;
  UINT8                               Socket;
  UINT32                              Signature;
  MRC_STATUS                          Status = MRC_STATUS_SUCCESS;

  Host = GetSysHostPointer ();

  // Confirm that the context is a SAFE_MARGINS_POWER_TRAINING_CONTEXT instance.
  Context = (SAFE_MARGINS_POWER_TRAINING_CONTEXT *)(SafeMarginsArbiter->Context);
  Signature = Context->Signature;
  RC_FATAL_ERROR (Signature == POWER_TRAINING_SIGNATURE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_184);

  // Get the context members.
  NumParams = Context->NumParams;
  Param = Context->Param;
  SettingIndex = Context->CurrentSettingIndex;
  Socket = Context->Socket;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Restore margins for setting %d\n", SettingIndex);
  for (ParamIndex = 0; ParamIndex < NumParams; ParamIndex++) {
    TempStatus = SetParameter (Host, Socket, &(Param[ParamIndex]), SettingIndex);

    // Update the status to the first failing code that is encountered.
    if ((Status == MRC_STATUS_SUCCESS) && (TempStatus != MRC_STATUS_SUCCESS)) {
      Status = TempStatus;
    }
  }

  return Status;
}

/**

  This function implements a generic 1-D parameter optimization

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Group         - Array of signal groups to margin
  @param[in]      NumGroups     - Number of signal groups to margin
  @param[in]      Param         - Pointer to list of parameter structures
  @param[in]      NumParams     - Number of parameters in Param

  @retVal MRC_STATUS_SUCCESS

**/
MRC_STATUS
TrainDDROptParam (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group[MAX_GROUPS],
  IN      UINT8             NumGroups,
  IN      POWER_TRAIN_PARAM Param[MAX_PARAMS],
  IN      UINT8             NumParams
  )
{
  UINT8                               ParamIndex;
  UINT8                               GroupIndex;
  INT16                               (*Margins)[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][3];
  AVERAGE_MARGINS                     *AverageMargins;
  INT16                               (*BestSetting)[MAX_CH][MAX_RANK_CH][MAX_BITS];
  INT16                               (*DefaultValues)[MAX_PARAMS][MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS];
  INT16                               (*PerParameterDefaultValues)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_BITS];
  MARGIN_RESULTS                      *MarginResults;
  UINT8                               Settings;
  UINT8                               EyeSizeScalingSaved;
  SAFE_MARGINS_ARBITER                SafeMarginsArbiter;
  SAFE_MARGINS_POWER_TRAINING_CONTEXT SafeMarginsContext;

  AverageMargins = RcAllocatePool (sizeof(*AverageMargins) + sizeof(*Margins) + sizeof(*MarginResults) +
                                  sizeof(*BestSetting) + (sizeof(*PerParameterDefaultValues) * NumParams));

  if (AverageMargins == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }
  Margins = (VOID *) ((UINTN) AverageMargins + sizeof (*AverageMargins));
  MarginResults = (VOID *) ((UINTN) Margins + sizeof (*Margins));
  BestSetting = (VOID *) ((UINTN) MarginResults + sizeof (*MarginResults));
  DefaultValues = (VOID *) ((UINTN) BestSetting + sizeof (*BestSetting));

  ZeroMem (DefaultValues, (sizeof (*PerParameterDefaultValues) * NumParams));

  //
  // Save the current EyeSizeScaling value in the Host structure - it will be modified by this routine
  //
  SaveRestoreEyeSizeScalingSim (Host, Socket, &EyeSizeScalingSaved, SAVE_SCALING);

  //
  // For each Parameter in numParams, save and print current setting
  //
  SaveCurrentSettings (Host, Socket, Param, NumParams, *DefaultValues);

  SafeMarginsContext.CurrentSettingIndex = 0;
  SafeMarginsContext.NumParams = NumParams;
  SafeMarginsContext.Param = Param;
  SafeMarginsContext.SafeValues = DefaultValues;
  SafeMarginsContext.Signature = POWER_TRAINING_SIGNATURE;
  SafeMarginsContext.Socket = Socket;

  SafeMarginsArbiter.Context = (VOID *)(&SafeMarginsContext);
  SafeMarginsArbiter.SetSafeMargins = SetSafePowerTrainingMargins;
  SafeMarginsArbiter.RestoreTrainedMargins = RestorePowerTrainingMargins;

  //--------------------------------------------------------
  // Loop through settings for the Parameter(s) to find the optimal Margins
  //--------------------------------------------------------
  for (Settings = 0; Settings < Param[0].NumSettings; Settings++) {
    RcDebugPrint (SDBG_MAX,
        "\nSetting %d    Params ", Settings );
    for (ParamIndex = 0; ParamIndex < NumParams; ParamIndex++) {

      DisplayParameterSetting (Host, Socket, Param, ParamIndex, Settings);
      //
      // Set parameters for each Settings
      //
      SetParameter (Host, Socket, &Param[ParamIndex], Settings);

      SetEyeSizeScalingSim (Host, Socket, &Param[ParamIndex], Settings);

    } // ParamIndex

    if ((Param->ParamGsm == DbDfeTap1) || (Param->ParamGsm == DbDfeTap2) || (Param->ParamGsm == DbDfeTap3) || (Param->ParamGsm == DbDfeTap4)) {
      TxPerBitDeskew (Host);
    }

    SafeMarginsContext.CurrentSettingIndex = Settings;

    //
    // Get margins for each Setting for every Goup
    //
    for (GroupIndex = 0; GroupIndex < NumGroups; GroupIndex++) {

      InitializeMarginResults (Host, Socket, Param, GroupIndex, MarginResults, *Margins);

      // Call GetMargins
      ExecuteGetMargins (Host, Socket, Param, Group[GroupIndex], MarginResults, &SafeMarginsArbiter);

      // Determine margin eye heights/widths for the signal Group at this Parameter Setting
      CalcMarginEyeSize (Host, Socket, Group[GroupIndex], GroupIndex, Param, MarginResults, *Margins);

      DisplayMarginResults (Host, Socket, Group[GroupIndex], GroupIndex, Param, Settings, *Margins);

      // Calculate composit eye, if needed
      CalcCompositeEye (Host, Socket, Group[GroupIndex], GroupIndex, Param, Settings, *Margins);

      // Calculate average eye for this signal Group at this Paramter Setting
      CalcAverageMargins (Host, Socket, Group[GroupIndex], GroupIndex, Param, Settings, *Margins, AverageMargins);
    } // group loop
  } // Settings loop

  //
  // Determine best setting for the Parameter(s) given the margin results
  //
  FindBestSetting (Host, Socket, NumGroups, Param, AverageMargins, *BestSetting);

  if (Param->ParamGsm == DramRxEq) {
    // Allow the SetOptimalSettings routine to set the values per DRAM
    Host->nvram.mem.socket[Socket].TrainingStepDone.DramRxEq = 1;
  }

  //
  // Program and display best setting for the Parameter(s)
  //
  SetOptimalSettings (Host, Socket, Param, NumParams, *BestSetting, *DefaultValues);

  if ((Param->ParamGsm == DbDfeTap1) || (Param->ParamGsm == DbDfeTap2) || (Param->ParamGsm == DbDfeTap3) || (Param->ParamGsm == DbDfeTap4)) {
    TxPerBitDeskew (Host);
  }

  //
  // Restore the original EyeSizeScaling value in the Host structure
  //
  SaveRestoreEyeSizeScalingSim (Host, Socket, &EyeSizeScalingSaved, RESTORE_SCALING);

  RcFreePool (AverageMargins);

  return MRC_STATUS_SUCCESS;
} // TrainDDROptParam

/**

  Find the optimal trade off between margins and power

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket Id
  @param[in] Channel         - Channel Id
  @param[in] Dimm            - Dimm Id
  @param[in] Rank            - Rank Id
  @param[in] Strobe          - Strobe Id
  @param[in] Bit             - Bit Id
  @param[in] AverageMargins  - Average margins
  @param[in] NumSettings     - Number of settings
  @param[in] NumGroups       - Number of groups
  @param[in] ParamType       - Parameter type (PerNibblePerRankPerCh, PerBytePerRankPerCh, etc)

  @retval Index of the optimal setting

**/
INT16
FindOptimalTradeOff (
  IN    PSYSHOST            Host,
  IN    UINT8               Socket,
  IN    UINT8               Channel,
  IN    UINT8               Dimm,
  IN    UINT8               Rank,
  IN    UINT8               Strobe,
  IN    UINT8               Bit,
  IN    PER_GROUP_SETTINGS  *AverageMargins,
  IN    UINT8               NumSettings,
  IN    UINT8               NumGroups,
  IN    MPT_PT              ParamType
  )
{
  UINT32  MaxMargin[MAX_GROUPS];
  UINT32  Values[MAX_GROUPS][MAX_SETTINGS];
  UINT32  MinValue[MAX_SETTINGS];
  UINT32  MaxValue;
  UINT8   Group;
  UINT8   Group2;
  UINT8   DisplayStrobe;
  UINT8   DisplayBit;
  INT16   Setting;
  UINT32  SmallestValue;
  UINT32  Score;
  UINT32  Total[MAX_SETTINGS];

  if (ParamType == PerBitPerRankPerCh) {
    DisplayStrobe = NO_STROBE;
    DisplayBit = Bit;
  } else {
    DisplayStrobe = Strobe;
    DisplayBit = NO_BIT;
  }

  //
  // The following equation is implemented below:
  // Total[n] = GV0[n]*Max(GV1) + GV1[n]*Max(GV0) + Min(GV0[n]* Max(GV1),GV1[n]*Max(GV0))
  // Score[n] = if(Min[Total[n]/Max(Total[n])] > 95 {Choose max GV0 or max GV1 based on Training WG recommendations} else {Total[n]/Max(Total)}
  //

  for (Setting = 0; Setting < NumSettings; Setting++) {
    MinValue[Setting] = 0xFFFFFFFF;
  }

  //
  // Get Maximum margins per group
  //
  for (Group = 0; Group < MAX_GROUPS; Group++) {
    MaxMargin[Group] = 0;
    if (Group >= NumGroups) {
      MaxMargin[Group] = 1;
    } else {
      if (ParamType == PerBitPerRankPerCh) {
        for (Setting = 0; Setting < NumSettings; Setting++) {
          if (MaxMargin[Group] < (UINT32) AverageMargins->PerBitGroupSetting[Group][Setting]) {
            MaxMargin[Group] = (UINT32) AverageMargins->PerBitGroupSetting[Group][Setting];
          }
        } // Setting
      } else { // PerBitPerRankPerCh
        for (Setting = 0; Setting < NumSettings; Setting++) {
          if (MaxMargin[Group] < (UINT32) AverageMargins->PerStrobeGroupSetting[Group][Setting]) {
            MaxMargin[Group] = (UINT32) AverageMargins->PerStrobeGroupSetting[Group][Setting];
          }
        } // Setting
      } // ParamType
    }
  } // Group

  //
  // Generate value array based on margin * max of other margins
  //
  for (Setting = 0; Setting < NumSettings; Setting++) {
    for (Group = 0; Group < NumGroups; Group++) {
      if (ParamType == PerBitPerRankPerCh) {
        Values[Group][Setting] = AverageMargins->PerBitGroupSetting[Group][Setting];
      } else {
        Values[Group][Setting] = AverageMargins->PerStrobeGroupSetting[Group][Setting];
      } // ParamType
      for (Group2 = 0; Group2 < NumGroups ; Group2++) {
        if (Group != Group2) {
          Values[Group][Setting] = Values[Group][Setting] * MaxMargin[Group2];
        }
      } // Group2

      //
      // Find the minimum value for this setting across goups
      //
      if (MinValue[Setting] > Values[Group][Setting]) {
        MinValue[Setting] = Values[Group][Setting];
      }
    } // Group
  } // Setting

  //
  // Figuring out the maxValue and total value per setting
  //
  MaxValue = 0;
  for (Setting = 0; Setting < NumSettings; Setting++) {
    Total[Setting] = MinValue[Setting];
    for (Group = 0; Group < NumGroups; Group++) {
      Total[Setting] = Total[Setting] + Values[Group][Setting];
    } // Group
    if (Total[Setting] > MaxValue) {
      MaxValue = Total[Setting];
    }
  } // Setting

  if (PcdGetBool (PcdMrcDebugPowerTraining) == TRUE) {

    RcDebugPrint (SDBG_MAX,
      "Param GV0  GV1  GV0*Max1  GV1*Max0  Min  Total  Total/Max\n");
    RcDebugPrint (SDBG_MAX,
      "--------------------------------------------------------------------------------------------------------------------------\n");
    for (Setting = 0; Setting < NumSettings; Setting++) {
      if (MaxValue == 0) {     // Divide by 0 error check
          RcDebugPrint (SDBG_MAX,
            "MaxValue is 0...Divide by 0!!\n");
        continue;
      }
      if (ParamType == PerBitPerRankPerCh) {
        RcDebugPrint (SDBG_MAX,
                                " %2d   %3d      %3d  %11d  %11d   %11d  %13d  %3d\n",
                                Setting,
                                AverageMargins->PerBitGroupSetting[0][Setting],
                                AverageMargins->PerBitGroupSetting[1][Setting],
                                Values[0][Setting],
                                Values[1][Setting],
                                MinValue[Setting],
                                Total[Setting],
                                Total[Setting] / (MaxValue / 100));
      } else {
        RcDebugPrint (SDBG_MAX,
                                " %2d   %3d      %3d  %11d  %11d   %11d  %13d  %3d\n",
                                Setting,
                                AverageMargins->PerStrobeGroupSetting[0][Setting],
                                AverageMargins->PerStrobeGroupSetting[1][Setting],
                                Values[0][Setting],
                                Values[1][Setting],
                                MinValue[Setting],
                                Total[Setting],
                                Total[Setting] / (MaxValue / 100));
      }
    }

  }

  //
  //If the smallest value is >= PERCENTAGE_TOLERANCE(95) at the end of the check, use the default value and exit
  //
  SmallestValue = 100;
  for (Setting = 0; Setting < NumSettings; Setting++) {
    if (MaxValue == 0) {     // Divide by 0 error check
        RcDebugPrint (SDBG_MAX,
          "MaxValue is 0...Divide by 0!!\n");
      continue;
    }

    Score = (Total[Setting] / (MaxValue / 100));

    if (Score < SmallestValue) {
      SmallestValue = Score;
    }
    //
    // Check to see if the smallest is >= PERCENTAGE_TOLERANCE(95)
    //
    if (SmallestValue >= PERCENTAGE_TOLERANCE && Setting == (NumSettings-1)) {
      //
      // The default values for all functions were stored at the beginning of TrainDDROptParam
      // Empirically demonstrated that there is little impact in margins from 95 to 100
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, DisplayStrobe, DisplayBit,
        "Gain < %d%%, return default setting\n", (100 - PERCENTAGE_TOLERANCE));

      return POWERTRAINING_DEFAULT_SETTING;
    } else {
      continue;
    }
  } // Setting

  //
  // Return the optimal index
  //
  for (Setting = 0; Setting < NumSettings; Setting++) {
    if (MaxValue == 0) {     // Divide by 0 error check
      RcDebugPrint (SDBG_MAX,
        "MaxValue is 0...Divide by 0!!\n");
      continue;
    }

    if (Total[Setting] / MaxValue) {
      return Setting;
    }
  } // Setting

  Setting = POWERTRAINING_DEFAULT_SETTING;

  return Setting;
} // FindOptimalTradeOff

/*++

Routine Description:

  Checks to see if this is valid setting to test

Arguments:

  channel, dimm, parameter, setting

Returns:

  1 = failure
  0 = pass

--*/
UINT8
InValidSettingCheck (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    param,
  INT16    setting
  )
{

  // not LRDIMM and RTT_WRT = 60
  if ((IsLrdimmPresent (socket, ch, dimm) == 0) && (param == WrOdt) && (setting == RTTWR_60)) {
    return 1;
  }

  // not LRDIMM and DRAM_RON = 40
  if ((IsLrdimmPresent (socket, ch, dimm) == 0) && (param == DramDrvStr) && (setting == DDR_RON_40)) {
    return 1;
  }

  return 0;
}

/*++

Routine Description:

  Checks to see if this is valid setting to test

Arguments:

  channel, dimm, rank, parameter, setting

Returns:

  1 = skip
  0 = dont skip

--*/
UINT8
SkipRankCheck (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     param
  )
{
  INT16               rttwrt;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();
  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList = GetDimmNvList (Host, socket, ch);

  // Dont train rttwr if its disabled
  if ((param == WrOdt)) {
    GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, 0, ALL_BITS, DdrLevel, param, GSM_READ_CSR | GSM_READ_ONLY, &rttwrt);
    if (rttwrt == RTTWR_DIS) {
      return 1;
    }
  }

  // skip 1DPC SR case
  if ((param == NomOdt) || (param == ParkOdt)) {
    if (((*channelNvList)[ch].maxDimm == 1) && (((*dimmNvList)[dimm].numRanks == 1) || IsLrdimmPresent (socket, ch, dimm))) {
      return 1;
    }
  }

  if ((param == DbDfeTap1) || (param == DbDfeTap2) || (param == DbDfeTap3) || (param == DbDfeTap4)) {
    // skip non-LRDIMM
    if (IsLrdimmPresent (socket, ch, dimm) == FALSE) {
      return 1;
    }

    // skip non-DCPMM rank if DBDFE_PMEM_ONLY is enabled in knob
    if (Setup->mem.DataBufferDfe == DBDFE_PMEM_ONLY) {
      if (((*channelNvList)[ch].maxDimm == 1) || ((*dimmNvList)[dimm].DcpmmPresent == 0)) {
        return 1;
      }
    }
  }
  return 0;
}

/**

  Return DdrLevel or LrbufLevel for each setting accordingly

  @param[in]  ParamGsm       - Parameter
  @param[out] *Level          - Level

  @retval EFI_SUCCESS

**/
EFI_STATUS
GetMrcLevel (
  IN  MRC_GT    ParamGsm,
  OUT MRC_LT    *Level
  )
{
  switch (ParamGsm) {
    case RxOdt:
    case TxRon:
    case TxEq:
    case TxEqTap2:
    case CmdTxEq:
    case PxcDelta:
    case TxImode:
    case TxRiseFallSlewRate:
    case TxFallSlewRate:
    case TxTco:
    case DdjcDelta:
    case RxDfe:
    case RxDfeCoefficient:
    case RxEq:
    case RxBias:
    case RxCtleC:
    case RxCtleR:
    case DramRxEq:
    case TxEchoCoef:
    case TxEchoUiSel:
    case RxDfeCode:
    case RxDfeDelaySel:
    case DramDrvStr:
    case RxDqDelay:
    case ParkOdt:
    case WrOdt:
    case NomOdt:
      *Level = DdrLevel;
      break;
    case DbDfeTap1:
    case DbDfeTap2:
    case DbDfeTap3:
    case DbDfeTap4:
      *Level = LrbufLevel;
      break;
    default:
      return EFI_NOT_FOUND;
      break;
  }
  return EFI_SUCCESS;
}
