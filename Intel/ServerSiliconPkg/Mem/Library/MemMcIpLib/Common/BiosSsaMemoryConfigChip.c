/** @file
  This file contains the BIOS implementation of the BIOS-SSA Memory Configuration API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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
#include "Chip10nm/Include/RcRegs10nm.h"
#include <SysHostChip.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/ProcMemInitChipLib.h>
#include <BiosSsaChipFunc.h>
#include <SysRegs.h>
#include <Memory/I3cDefinitions.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>

#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/SiliconWorkaroundLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>


/**
  Find if there is a LRDIMM present on the platform

  @retval TRUE/FALSE     TRUE: LRDIMM is present
**/
BOOLEAN
SSAIsLrdimmPresent (
  PSYSHOST         Host
  )
{
  UINT8                     dimm, ch, socket;
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  struct channelNvram       (*channelNvList)[MAX_CH];
  struct socketNvram        (*socketNvram)[MAX_SOCKET];
  UINT8                     MaxChDdr;

  MaxChDdr    = GetMaxChDdr ();
  socketNvram = &Host->nvram.mem.socket;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((*socketNvram)[socket].enabled == 0) {
      continue;
    }
    channelNvList = GetChannelNvList(Host, socket);
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList(Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if (IsLrdimmPresent (socket, ch, dimm)) {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

/**
  Find if there is a DDRT(AEP) DIMM present on the platform

  @retval TRUE/FALSE     TRUE: DDRT(AEP) DIMM is present
**/
BOOLEAN
SSAIsDdrtDimmPresent (
  PSYSHOST         Host
  )
{
  if (Host->nvram.mem.DcpmmPresent) {
    return TRUE;
  }

  return FALSE;
}

/**
  Find if there is any RegisterRev2 Dimm is present on the platform

  @retval TRUE/FALSE     TRUE: RegisterRev2 Dimm is present
**/
BOOLEAN
SSAIsRegRev2DimmPresent(
  PSYSHOST         Host
  )
{
  UINT8                     dimm, ch, socket;
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  struct channelNvram       (*channelNvList)[MAX_CH];
  struct socketNvram        (*socketNvram)[MAX_SOCKET];
  UINT8                     MaxChDdr;

  MaxChDdr    = GetMaxChDdr ();
  socketNvram = &Host->nvram.mem.socket;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((*socketNvram)[socket].enabled == 0) {
      continue;
    }
    channelNvList = GetChannelNvList(Host, socket);
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList(Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }
        if (IsDdr4RegisterRev2(Host, socket, ch, dimm)) {
        return TRUE;
    }
  }
    }
  }

  return FALSE;
}

//
//This BSSA file contains subsets of all BSSA function calls made from the Core files; all CSR accesses are made here
//

/**

  This routine gets the Temperature for specific Thermal Sensor

  @param[in]  Socket      - Socket Id
  @param[in]  Tsod        - SMB_DEVICE_STRUCT
  @param[out] Temperature - Pointer to where the DIMM's temperature will be written

  @retval N/A

**/
VOID
BiosGetMemTempChipWorker (
  IN   UINT8              Socket,
  IN   SMB_DEVICE_STRUCT  Tsod,
  OUT  INT32              *Temperature
  )
{
  PSYSHOST                  Host;
  UINT16                    MstData;
  INT16                     Temp;
  UINT8                     RegOffset;
  UINT8                     TsodDeviceTypeId[MAX_TS_PER_DIMM];
  UINT8                     MaxTempSensor;
  UINT8                     TempSensor;

  *Temperature = MIN_INT32;
  Host = GetSysHostPointer ();
  Tsod.compId = MTS;
  Tsod.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;

  // For DDR4
  RegOffset = MTS_TEMP;
  MaxTempSensor = 1;
  TsodDeviceTypeId[0] = DTI_TSOD;
  TsodDeviceTypeId[1] = DTI_TSOD;

#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Socket)) {
    RegOffset = TS5_MR49_TEMP;
    MaxTempSensor = MAX_TS_PER_DIMM;
    TsodDeviceTypeId[0] = DDR5_TS0;
    TsodDeviceTypeId[1] = DDR5_TS1;  // Support MAX_TS_PER_DIMM is 2
  }
#endif // DDR5_SUPPORT

  for (TempSensor = 0; TempSensor < MaxTempSensor; TempSensor++) {

    MstData = 0;
    Tsod.address.deviceType = TsodDeviceTypeId[TempSensor];

    ReadProcSmb (Tsod, RegOffset, (UINT8 *) &MstData); // Get data from device = TSOD for offset = TS5_MR49_TEMP Mode register 49

    //xxx0 0000 0010 11xx + 2.75 °C
    //xxx1 1111 1111 11xx - 0.25 °C

    MstData &= 0x1FF0; // Taking only bits 11:4 for the dimm temp, BIT12 IS Sign
    if (MstData & BIT12) {
      Temp = (INT16) (MstData | 0xE000);  // Sign extend negative
    } else {
      Temp = MstData;
    }
    Temp = Temp / 16;                      // Shift to 1 units

    //
    // Take the maximum value of two temp sensor values (if both the sensors are valid).
    //
    if (Temp > *Temperature) {
      *Temperature = Temp;
    }
  }
}


/**

  This routine gets the CSRs required by BiosGetMemTemp

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Dimm                       - Zero based Dimm number (0-based)
  @param Temperature                - Pointer to where the DIMM's temperature will be written

  @retval N/A

**/
VOID
BiosGetMemTempChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Dimm,
  INT32                 *Temperature
  )
{
  UINT8                                NumChPerMc;
  SMB_DEVICE_STRUCT                    Tsod;
  DIMMTEMPSTAT_0_MCDDC_CTL_STRUCT      DimmTempStat;
  UINT32                               DimmTempStatRegOffset[2] = {DIMMTEMPSTAT_0_MCDDC_CTL_REG,
                                                                   DIMMTEMPSTAT_1_MCDDC_CTL_REG};

  *Temperature = 0;
  NumChPerMc   = GetNumChannelPerMc ();

  if (EFI_ERROR (GetSmbAddress (Socket, ChannelInSocket, Dimm, &Tsod))) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, ChannelInSocket, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error retrieving DIMM SMBus address. Cannot write throttling data.\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
  }

  if (GetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (ChannelInSocket / NumChPerMc)) == 1) {
    DimmTempStat.Data = MemReadPciCfgEp (Socket, ChannelInSocket, DimmTempStatRegOffset[Dimm]);
    *Temperature = DimmTempStat.Bits.dimm_temp;
  } else {
    BiosGetMemTempChipWorker (Socket, Tsod, Temperature);
  }
}

/**

  This routine sets CSRs required by BiosIoReset

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param resetIo                    - Reset/Clear DDRIO (0-based)

  @retval N/A

**/
VOID
BiosIoResetChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 resetIo
  )
{
  MC_INIT_STATE_G_MC_MAIN_STRUCT           mcInitStateG;

  mcInitStateG.Data = MemReadPciCfgMC (Socket, Controller, MC_INIT_STATE_G_MC_MAIN_REG);
  mcInitStateG.Bits.reset_io = resetIo;
  MemWritePciCfgMC_AllCh (Socket, Controller, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
}

/**

  This routine sets CSRs required by BiosInitCpgc

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param SetupCleanup               - Specifies setup or cleanup action

  @retval N/A

**/
VOID
BiosInitCpgcChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  SETUP_CLEANUP         SetupCleanup
  )
{
  MCMTR_MC_MAIN_STRUCT     McMtr;
  UINT8                    Channel;
  UINT8                    NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();

  for (Channel = Controller * NumChPerMc; Channel < ((Controller + 1) * NumChPerMc); Channel++ ) {
    //Enter or exit CPGC mode
    McMtr.Data = MemReadPciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG);
    McMtr.Bits.trng_mode = (SetupCleanup == Setup)? 1 : 0;
    //At the postcode where the BSSA runs, the "normal" mode bit is still 0
    //McMtr.Bits.normal = 0;
    MemWritePciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG, McMtr.Data);
  } //Channel Loop
}

/**

  BiosGetMarginParamSpecificityChip: This routine gets the specificity of a given margin parameter.

  @param Host                       - Pointer to sysHost
  @param IoLevel                    - I/O level.
  @param MarginGroup                - Margin group.
  @param Specificity                - Pointer to where the margin parameter specificity mask will be stored.

  @retval UnsupportedValue if the IoLevel or MarginGroup parameter value is not supported.  Else Success.

**/
SSA_STATUS
BiosGetMarginParamSpecificityChip (
  PSYSHOST                 Host,
  MRC_LT                   IoLevel,
  MRC_GT                   MarginGroup,
  MARGIN_PARAM_SPECIFICITY *Specificity
  )
{
  SSA_STATUS                     Status = Success;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  *Specificity = ChannelSpecific|ControllerSpecific|SocketSpecific;  //Default value

  switch (MarginGroup) {
    case CtlAll:
    case CtlGrp0:
    case CtlGrp1:
    case CtlGrp2:
    case CtlGrp3:
    case CmdAll:
    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdGrp3:
    case CmdGrp4:
    case CmdVref:
      if (IoLevel == DdrLevel && ((MarginGroup != CmdGrp3) && (MarginGroup != CmdGrp4))) {
        *Specificity = ChannelSpecific|ControllerSpecific|SocketSpecific;
      } else { //For 'LrbufLevel' && Rev2 RDIMM & LRDIMM - backside CmdAll/CtlAll/CmdVref is supported
        if ((MarginGroup == CmdVref) && (SSAIsRegRev2DimmPresent(Host) || (SSAIsDdrtDimmPresent (Host)))) {
          *Specificity = RankSpecific | ChannelSpecific | ControllerSpecific | SocketSpecific;
        } else if (((MarginGroup == CmdAll) || (MarginGroup == CtlAll)) && (SSAIsRegRev2DimmPresent(Host) || (SSAIsDdrtDimmPresent(Host) && Setup->mem.enableNgnBcomMargining))) {
          *Specificity = RankSpecific | ChannelSpecific | ControllerSpecific | SocketSpecific;
        } else if (((MarginGroup == CmdGrp3) || (MarginGroup == CmdGrp4)) && (SSAIsDdrtDimmPresent(Host)) && (Setup->mem.enableNgnBcomMargining)) {
          *Specificity = RankSpecific | ChannelSpecific | ControllerSpecific | SocketSpecific;
        } else {
          Status = UnsupportedValue;
        }
      }
      break;

    case RxDqsDelay:
    case TxDqDelay:
    case RxSamplerEvenOdd:
    case RxSamplerEven:
    case RxSamplerOdd:
    case RxVref:
    case TxVref:
      if ((IoLevel == DdrLevel) || ((IoLevel == LrbufLevel) && (SSAIsLrdimmPresent (Host)))) {
        *Specificity = StrobeSpecific|RankSpecific|ChannelSpecific|ControllerSpecific|SocketSpecific;
      } else {
        Status = UnsupportedValue;
      }
      break;

    case RecEnDelay:
    case TxDqsDelay:
    case WrLvlDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxDqDelay:
    case RxEq:
    case TxEq:
    case TxEqTap2:

      if (IoLevel == DdrLevel) {
        *Specificity = StrobeSpecific|RankSpecific|ChannelSpecific|ControllerSpecific|SocketSpecific;
      } else {
        Status = UnsupportedValue;
      }
      break;

    case RxDqsBitDelay:
    case TxDqBitDelay:
      if (IoLevel == DdrLevel) {
        *Specificity = LaneSpecific|StrobeSpecific|RankSpecific|ChannelSpecific|ControllerSpecific|SocketSpecific;
      } else {
        Status = UnsupportedValue;
      }
      break;

    case EridDelay:
      if ((IoLevel == DdrLevel) && (SSAIsDdrtDimmPresent(Host))) {
        *Specificity = RankSpecific|ChannelSpecific|ControllerSpecific|SocketSpecific;
      } else {
        Status = UnsupportedValue;
      }
      break;

    case EridVref:
      if ((IoLevel == DdrLevel) && (SSAIsDdrtDimmPresent(Host))) {
        *Specificity = ChannelSpecific|ControllerSpecific|SocketSpecific;
      } else {
        Status = UnsupportedValue;
      }
      break;

    default:
      Status = UnsupportedValue;
      break;
  }

  return Status;
}


/**

  This routine gets the CSRs required by BiosServerGet3dsRankPerCs

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Dimm                       - Zero based Dimm number (0-based)
  @param RankCount                  - Pointer to where the rank count per chip select signal will be stored

  @retval N/A

**/
VOID
BiosServerGet3dsRankPerCsChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Dimm,
  UINT8                 *RankCount
  )
{
  DIMMMTR_0_MC_MAIN_STRUCT  dimmMtr;

  if (Dimm < MAX_DIMM) {
    dimmMtr.Data = MemReadPciCfgEp (Socket, ChannelInSocket, GetDimmMtrRegOffset (Dimm));

    if (dimmMtr.Bits.ddr4_3dsnumranks_cs == 1) {
      *RankCount = 2;
    } else if (dimmMtr.Bits.ddr4_3dsnumranks_cs == 2) {
      *RankCount = 4;
    } else if (dimmMtr.Bits.ddr4_3dsnumranks_cs == 3) {
      *RankCount = 8;
    }
  }
}

// end file BiosSsaMemoryConfig.c
