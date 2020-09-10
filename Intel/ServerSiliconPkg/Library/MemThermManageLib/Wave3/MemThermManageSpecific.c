/** @file
  Includes implementation for API calls of Memory Throttling\Power Management

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

#include <Library/MemoryCoreLib.h>
#include "SysHost.h"
#include "Chip10nm/Include/MemHostChip10nm.h"
#include "Include/MemThrot.h"
#include <SysHostChip.h>
#include <UncoreCommonIncludes.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemPwrTempTableLib.h>
#include <Library/MemThermManageLib.h>
#include <Include/MemTsod.h>
#include <Include/MemRapl.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/SpdDecodeLib.h>

#define OLTT_TEMP_LO_SPR  82
#define OLTT_TEMP_MID_SPR 83
#define OLTT_TEMP_HI_SPR  101

#define TEMP_LO_SPR_DDRT  82
#define TEMP_MID_SPR_DDRT 83
#define TEMP_HI_SPR_DDRT  85

#define DIMMTEMP_THRT_DDRT 0xFF

/**

  This function initializes temperature thershold values

  @param[in] Socket                              Socket number
  @param[in] Channel                             Channel Number
  @param[in] Dimm                                Dimm Number
  @param[in] ThersholdHigh                       Thershold High value
  @param[in] DimmInThermalThrottlingModeTable    Result of Dimm in TT mode table

  @retval    N/A

**/
VOID
InitalizeTempThreshold (
  UINT8 Socket,
  UINT8 Channel,
  UINT8 Dimm,
  UINT8 ThersholdHigh,
  BOOLEAN DimmInThermalThrottlingModeTable
  )
{
  PSYSHOST                               Host = NULL;
  UINT16                                 ThermalThrottlingOptions;
  struct channelNvram                    (*ChannelNvList)[MAX_CH];
  struct dimmNvram                       (*DimmNvList)[MAX_DIMM];
  THROTTLE_VALUE                         ThrottleValue;
  SYS_SETUP                              *Setup;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  ThermalThrottlingOptions = Setup->mem.thermalThrottlingOptions;
  //
  // Initialize the temp thresholds
  //
  ZeroMem ((UINT8 *) &ThrottleValue, sizeof (THROTTLE_VALUE));

  ThrottleValue.TempThrottleMid = DIMMTEMP_THRT_DDRT;  // same value for DDR5 and DDRT2

  if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP) {

    ThrottleValue.TempThrottleCritical = DIMMTEMP_THRT_DDRT;
    ThrottleValue.TempThrottleHigh = DIMMTEMP_THRT_DDRT;

    ThrottleValue.TempThersholdLow = TEMP_LO_SPR_DDRT;
    ThrottleValue.TempThersholdMid = TEMP_MID_SPR_DDRT;
    ThrottleValue.TempThersholdHigh = TEMP_HI_SPR_DDRT;

  } else {
    ThrottleValue.TempThrottleCritical = DIMMTEMP_THRT_CRIT_DEFAULT;

    if (ThermalThrottlingOptions & (CLTT_EN | CLTT_PECI_EN)) {

      if (DimmInThermalThrottlingModeTable) {
        ThrottleValue.TempThrottleHigh = ThersholdHigh;
      } else {
        ThrottleValue.TempThrottleHigh = CLTT_THRT_HI_DEFAULT / (*ChannelNvList)[Channel].maxDimm;  //devide by #DIMMs installed per channel
      }

      if ((!Disable2xRefreshCh (Host, Socket, Channel))) {
        ThrottleValue.TempThersholdLow = PcdGet8 (PcdClttTempLoDoubleRefreshDefault);
        ThrottleValue.TempThersholdMid = PcdGet8 (PcdClttTempMidDoubleRefreshDefault);
        ThrottleValue.TempThersholdHigh = PcdGet8 (PcdClttTempHiDoubleRefreshDefault);
      } else {
        ThrottleValue.TempThersholdLow = PcdGet8 (PcdClttTempLoSingleRefreshDefault);
        ThrottleValue.TempThersholdMid = PcdGet8 (PcdClttTempMidSingleRefreshDefault);
        ThrottleValue.TempThersholdHigh = PcdGet8 (PcdClttTempHiSingleRefreshDefault);
      }
    } else if ((ThermalThrottlingOptions & OLTT_EN) != 0) {

      if (DimmInThermalThrottlingModeTable == 1) {
        ThrottleValue.TempThrottleHigh = ThersholdHigh;
      } else {
        ThrottleValue.TempThrottleHigh = ((Setup->mem.olttPeakBWLIMITPercent * 255) / 100) / (*ChannelNvList)[Channel].maxDimm;  //devide by #DIMMs installed per channel
      }

      ThrottleValue.TempThersholdLow = OLTT_TEMP_LO_SPR;
      ThrottleValue.TempThersholdMid = OLTT_TEMP_MID_SPR;
      ThrottleValue.TempThersholdHigh = OLTT_TEMP_HI_SPR;
    }
  }

  if (IsCustomRefreshRate () == SUCCESS) {
    ThrottleValue.TempThersholdLow = 0;
    ThrottleValue.TempThrottleMid = DEFAULT_TEMP_THROTTLE_MID;
  }

  // As per MPM Architects 3DS 4H 16Gb, requries the templow = 82 celcius, tempmid = 83 celcius
  if (Is3Ds4H16GbDimm (Socket, Channel, Dimm)) {
    ThrottleValue.TempThersholdLow = TEMP_3DS_DIMM_4H_16GB_LOW;
    ThrottleValue.TempThersholdMid = TEMP_3DS_DIMM_4H_16GB_MID;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n:MPM: DimmTempThrottlingLimit  thrt_mid:%d,  thrt_hi:%d,  thrt_crit:%d\n ", ThrottleValue.TempThrottleMid, ThrottleValue.TempThrottleHigh, ThrottleValue.TempThrottleCritical);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n:MPM: DimmTempThrottling  temp_lo:%d,  temp_mid:%d,  temp_hi:%d\n ", ThrottleValue.TempThersholdLow, ThrottleValue.TempThersholdMid, ThrottleValue.TempThersholdHigh);

  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdLow, &ThrottleValue);
  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdMid, &ThrottleValue);
  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdHigh, &ThrottleValue);

  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleMid, &ThrottleValue);
  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleHigh, &ThrottleValue);
  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleCritical, &ThrottleValue);

  if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP) {
    ThrottleValue.TempMemTrip = TEMP_HI_SPR_DDRT;
  } else {
    ThrottleValue.TempMemTrip = PcdGet8 (PcdTempMemTripDefault);
  }

  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempMemTrip, &ThrottleValue);
} // InitalizeTempThreshold

/**

  Initialize memory throttling

  @param[in] Host  - Point to sysHost

  @retval SUCCESS

**/
UINT32
InitThrottling (
  IN PSYSHOST  Host
  )
{
  UINT8                                       B2PFlag;
  UINT8                                       Socket;
  UINT8                                       Ch;
  UINT8                                       Mc;
  UINT8                                       Dimm;
  DIMM_IN_TABLE                               CheckDimmInThrottleModeTable;
  DIMM_IN_TABLE                               CheckDimmInThrottleTable;
  DIMM_IN_TABLE                               CheckDimmInWeightTable;
  UINT16                                      TTRow = 0;
  UINT16                                      WTRow = 0;
  UINT32                                      bwlimittfdata = 0;
  const DimmTTRowEntryStruct                  *ptrDimmTT = NULL;
  DimmTTRowEntryStruct                        TTcurrentDimm;
  const DimmWTRowEntryStruct                  *ptrDimmWT = NULL;
  DimmWTRowEntryStruct                        WTcurrentDimm;
  const DimmTThrottlingStruct                 *ptrDimmTTMODE = NULL;
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  UINT16                                      varThermalThrottlingOptions;
  UINT16                                      perChMaxMemPwr[MAX_CH] = { 0 };
  UINT16                                      perChSelfRefreshPwr[MAX_CH] = { 0 };
  UINT8                                       TTMRow;
  UINT8                                       BwLimitTfMultiPlier;
  UINT16                                      DimmWtTableSize;
  UINT16                                      DimmWtaepTableSize;
  DramRaplDataStruct                          RaplData;
  THROTTLE_VALUE                              ThrottleValue;
  SYS_SETUP                                   *Setup;
  EWL_ENTRY_MEMORY_LOCATION                   MemoryLocation;
  EWL_SEVERITY                                Severity;
  UINT8                                       MaxChDdr;
  UINT32                                      M2MemMode = 0;
  UINT8                                       TempStatusCurrentDimmTemp = 0;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ASSERT (Setup != NULL);
  if (Setup == NULL) {
    return SUCCESS;
  }

  if (HybridSystemLevelEmulationEnabled ()) { //Skip only for HSLE
    RcDebugPrint (SDBG_MAX,
      "Skipping InitThrottling\n\n");
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (InitThermalThrottling)) {
    return SUCCESS;
  }
  Socket = Host->var.mem.currentSocket;

  //
  //  Return if no memory found
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Initialize Throttling\n\n");
#endif // DEBUG_CODE_BLOCK
  ChannelNvList = GetChannelNvList (Host, Socket);

  CheckForTsodDimm (Socket);
  varThermalThrottlingOptions = Setup->mem.thermalThrottlingOptions;

  // Flag Value (i) came from BIOS knob:
  // 0 - Disable both DRAM RAPL and DRAM Thermal Throttling
  // 1 - Enable DRAM Thermal Throttling
  // 2 - Enable DRAM RAPL
  B2PFlag = DISABLE_DRAM_MPM; //init flag value

  if (varThermalThrottlingOptions & (CLTT_EN | CLTT_PECI_EN)) {
    B2PFlag |= ENABLE_DRAM_THERMAL;
  }

  if (Setup->mem.CmsEnableDramPm != 0) {
    B2PFlag |= ENABLE_DRAM_PM;
  }

  RcDebugPrint (SDBG_MAX,
    "ControlDramPMThermalViaPcode (%d)\n\n", B2PFlag);

  ControlDramPMThermalViaPcode (B2PFlag);
  ConfigureMemHotSenseAssertPeriod (Socket);
  ConfigureMemoryThermalFeature (Socket, ConfigureMemHotFeatureType);
  ConfigureMemoryThermalFeature (Socket, ConfigureMemTripFeatureType);

  ZeroMem (&RaplData, sizeof (DramRaplDataStruct));

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_INIT_THROTTLING, CHECKPOINT_MINOR_RMT, (UINT16)((CHECKPOINT_DATA_INIT_THROTTLING << 8) | (Ch << 4) | Socket)));

    Mc = GetMCID (Host, Socket, Ch);

    CheckDimmInThrottleModeTable.TableEntryPresent = FALSE;
    CheckDimmInThrottleModeTable.CfgValid = FALSE;

    if ((*ChannelNvList)[Ch].enabled != 0) {
      if (varThermalThrottlingOptions & (OLTT_EN | CLTT_EN | CLTT_PECI_EN)) {
        ThrottleValue.BwLimitThrottleEnable = TRUE;
      } else {
        // be sure to set it 0, irrespective of depending on previous state
        ThrottleValue.BwLimitThrottleEnable = FALSE;
      }
      CheckDimmInThermalThrottlingMode (Socket, Ch, &CheckDimmInThrottleModeTable.TableEntryPresent, &TTMRow);

      if (CheckDimmInThrottleModeTable.TableEntryPresent) {
        GetDimmTtModeTable (&ptrDimmTTMODE, TTMRow);
      } else {
        //Output EWL 26 MPM Warning
        MemoryLocation.Socket = Socket;
        MemoryLocation.Channel = Ch;
        MemoryLocation.Dimm = NO_DIMM;
        MemoryLocation.Rank = NO_RANK;
        Severity = EwlSeverityWarning;

        EwlOutputType26 (WARN_MEMORY_POWER_MANAGEMENT, WARN_MEMORY_PM_THERMAL_TABLE_NOT_FOUND, MemoryLocation, Severity);
      }
      BwLimitTfMultiPlier = Setup->mem.dramraplbwlimittf;
      if ((!CheckDimmInThrottleModeTable.TableEntryPresent) && (Setup->mem.dramraplbwlimittf == 0)) {
        BwLimitTfMultiPlier = 1;
      }
      if ((BwLimitTfMultiPlier == 0) && (CheckDimmInThrottleModeTable.TableEntryPresent)) {
        bwlimittfdata = ptrDimmTTMODE->TTBW_THROTTLE_TF;
      } else {
        // Override bw_limit_tf setting based on user configuration
        GetBwThrottleWindowSize (Socket, Ch, BwLimitTfMultiPlier, &bwlimittfdata);
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\n:: ChnTempCfgReg.Bits.bw_limit_tf = 0x%04X\n ", bwlimittfdata);

      if ((varThermalThrottlingOptions & OLTT_EN) != 0)  {
        ThrottleValue.OlttEnable = TRUE;
      } else {
        ThrottleValue.OlttEnable = FALSE;
      }
      if ((varThermalThrottlingOptions & (OLTT_EN | CLTT_EN | CLTT_PECI_EN)) != 0)  {
        if (CheckDimmInThrottleModeTable.TableEntryPresent)  {
          ThrottleValue.ThrottleExternal = 4 * ptrDimmTTMODE->TTTHRT_HI;  // THRT_EXT Equal to THRT_HI times 4 per Table 6-2 in EGS CLTT Whitepaper
        } else {
          if ((varThermalThrottlingOptions & OLTT_EN) != 0) {
            ThrottleValue.ThrottleExternal = OLTT_THRT_HI_DEFAULT; // HSD 4987488
          } else if (((varThermalThrottlingOptions & (CLTT_EN | CLTT_PECI_EN)) != 0)) {
            ThrottleValue.ThrottleExternal = CLTT_THRT_HI_DEFAULT; // HSD 4987488
          }
        }
        GetSetThrottleValues (MemTechDdr, Socket, Ch, NO_DIMM_OPTION, WRITE_THROTTLE_VALUES, EnumThrottleExternal, &ThrottleValue);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\n:: ThrottleExternal = %x\n ", ThrottleValue.ThrottleExternal);
      }

      //
      // Set imcM_cC_chn_temp_cfg.thrt_allow_isoch as default
      //
      M2MemMode = GetM2mModeValue (Socket, Mc);
      if ((M2MemMode & ISOCHRONOUS_FLOW_ENABLED_BIT) != 0) {
        ThrottleValue.MemThrotAllowIsoch = TRUE;
      } else {
        ThrottleValue.MemThrotAllowIsoch = FALSE;
      }

      ThrottleValue.BandWidthThrottleWindowSize = bwlimittfdata;
      GetSetThrottleValues (MemTechDdr, Socket, Ch, NO_DIMM_OPTION, WRITE_THROTTLE_VALUES, EnumMemThrotAllowIsoch, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, Socket, Ch, NO_DIMM_OPTION, WRITE_THROTTLE_VALUES, EnumBandWidthThrottleWindowSize, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, Socket, Ch, NO_DIMM_OPTION, WRITE_THROTTLE_VALUES, EnumBwLimitThrottleEnable, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, Socket, Ch, NO_DIMM_OPTION, WRITE_THROTTLE_VALUES, EnumOlttEnable, &ThrottleValue);

      // Configure CKE and ODT contributions
      ConfigureCkeAndOdtContributions (Socket, Ch);

      //
      // Initialize per Dimm throttling and power related registers
      //
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Look up DIMM command weight value in the fixed table
        // First get the data needed to match a table entry for this Dimm
        //
        CheckDimmInWeightTable.CfgValid = TRUE;
        CheckDimmInWeightTable.TableEntryPresent = FALSE;

        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          WTcurrentDimm.WTDimmType = (0xF0 | SPD_TYPE_AEP);
        } else {
          WTcurrentDimm.WTDimmType = (*DimmNvList)[Dimm].actKeyByte2;
        }

        if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5) {
          if (WTcurrentDimm.WTDimmType == SPD_LRDIMM_DDR5) {
            WTcurrentDimm.WTDimmType = SPD_LRDIMM;
          }

          if (!((WTcurrentDimm.WTDimmType == SPD_RDIMM) ||
            (WTcurrentDimm.WTDimmType == SPD_UDIMM) ||
            (WTcurrentDimm.WTDimmType == SPD_UDIMM_ECC) ||
            (WTcurrentDimm.WTDimmType == SPD_LRDIMM))) {
            CheckDimmInWeightTable.CfgValid = FALSE;
          }

          if ((WTcurrentDimm.WTDimmType == SPD_UDIMM) && ((*DimmNvList)[Dimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT)) {
            WTcurrentDimm.WTDimmType |= 0x10;
          }
        }

        if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5) {
          WTcurrentDimm.WTLvAndDramDensity = (*DimmNvList)[Dimm].sdramCapacity;
        } else {
          WTcurrentDimm.WTLvAndDramDensity = (*DimmNvList)[Dimm].aepTechIndex + 5;
        }

        if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5) {
          if ((WTcurrentDimm.WTLvAndDramDensity < SPD_1Gb) || (WTcurrentDimm.WTLvAndDramDensity > SPD_16Gb)) {
            CheckDimmInWeightTable.CfgValid = FALSE;
          }
        }

        WTcurrentDimm.WTLvAndDramDensity |= DEFAULT_MEMWEIGHT_VOLTAGE;
        RcDebugPrint (SDBG_MAX,
          "\nMPM:: WTcurrentDimm.WTLvAndDramDensity = %x\n", WTcurrentDimm.WTLvAndDramDensity);

        if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5) {
          WTcurrentDimm.WTRankNumIoWidth = (*DimmNvList)[Dimm].SPDModuleOrg;
        } else {
          WTcurrentDimm.WTRankNumIoWidth = 0;
        }

        WTcurrentDimm.WTFrequency = Host->nvram.mem.socket[Socket].ddrFreq;

        WTcurrentDimm.WTSignalLoading = ((*DimmNvList)[Dimm].SPDDeviceType & SPD_SIGNAL_LOADING);
        WTcurrentDimm.WTDieCount = (*DimmNvList)[Dimm].dieCount - SPD_DIE_BASE; //DieCount value has a +1 to make it 1 based

        if (WTcurrentDimm.WTFrequency > GetMaxCpuDdrFreq (Host, Socket)) {
          CheckDimmInWeightTable.CfgValid = FALSE;
        }

        WTcurrentDimm.WTDimmPos = Dimm;

        if (CheckDimmInWeightTable.CfgValid) {
          RcDebugPrint (SDBG_MAX,
            "\nChecking...    WTcurrentDimm.WTDimmType: %x, WTLvAndDramDensity: %x, WTRankNumIoWidth: %x, WTDimmPos: %x, WTFrequency: %x\n\n",
            WTcurrentDimm.WTDimmType,
            WTcurrentDimm.WTLvAndDramDensity,
            WTcurrentDimm.WTRankNumIoWidth,
            WTcurrentDimm.WTDimmPos,
            WTcurrentDimm.WTFrequency);

          //
          // DDR5 DIMM Check
          //

          if (((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5)) {
            RcDebugPrint (SDBG_MAX,
              "\n::MEM DDR5:: DDR5 Memory is detected in system. Look at DDR5 Power table now...\n");

            DimmWtTableSize = GetDimmWeightTableSize ();
            for (WTRow = 0; WTRow < DimmWtTableSize; WTRow++) {
              GetDimmWeightTable (&ptrDimmWT, WTRow);
              if ((ptrDimmWT->WTDimmType == WTcurrentDimm.WTDimmType) &&
                (ptrDimmWT->WTLvAndDramDensity == WTcurrentDimm.WTLvAndDramDensity) &&
                (ptrDimmWT->WTRankNumIoWidth == WTcurrentDimm.WTRankNumIoWidth) &&
                (ptrDimmWT->WTFrequency == WTcurrentDimm.WTFrequency) &&
                (ptrDimmWT->WTDimmPos == WTcurrentDimm.WTDimmPos) &&
                (ptrDimmWT->WTSignalLoading == WTcurrentDimm.WTSignalLoading) &&
                (ptrDimmWT->WTDieCount == WTcurrentDimm.WTDieCount)) {
                CheckDimmInWeightTable.TableEntryPresent = TRUE;
                RcDebugPrint (SDBG_MAX,
                  "\n::MEM DDR5:: DDR5 Table is available to use. CheckDimmInWeightTable.TableEntryPresent = %x\n\n", CheckDimmInWeightTable.TableEntryPresent ? 1 : 0);
                RcDebugPrint (SDBG_MAX,
                  "::MEM DDR5:: (%d)        %x, %x, %x, %x, %x, %x, %x, ",
                  WTRow,
                  ptrDimmWT->WTDimmType,
                  ptrDimmWT->WTLvAndDramDensity,
                  ptrDimmWT->WTRankNumIoWidth,
                  ptrDimmWT->WTSignalLoading,
                  ptrDimmWT->WTDieCount,
                  ptrDimmWT->WTDimmPos,
                  ptrDimmWT->WTFrequency);
                RcDebugPrint (SDBG_MAX,
                  "%d,  %d,  %d,  %d,  %d,  %d,  ",
                  ptrDimmWT->WTPmCmdPwrData,
                  ptrDimmWT->DramPmWritePowerCoeff,
                  ptrDimmWT->DramMaxPwr,
                  ptrDimmWT->DramTdp,
                  ptrDimmWT->DramMinPwrClosedPage,
                  ptrDimmWT->DramWritePwrScaleClosedPage);
                RcDebugPrint (SDBG_MAX,
                  "%d,  %d,  %d,  %d,  %d,  %d\n\n",
                  ptrDimmWT->DramMaxPwrOpenPage,
                  ptrDimmWT->DramTdpOpenPage,
                  ptrDimmWT->DramMinPwrOpenPage,
                  ptrDimmWT->DramWritePwrScaleOpenPage,
                  ptrDimmWT->DramRefreshRateSlope,
                  ptrDimmWT->DramSelfRefreshPower);
                break;
              }
            } // DDR5 WTRow loop
          } else if (((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP)) {
            RcDebugPrint (SDBG_MAX,
              "\n::MEM NVMDIMM:: NVMDIMM Memory is detected in system. Look at NVMDIMM Power table now...\n");
            //
            // NVDIMM Check
            //
            DimmWtaepTableSize = GetDimmWtaepTableSize ();
            for (WTRow = 0; WTRow < DimmWtaepTableSize; WTRow++) {
              GetDimmWtaepTable (&ptrDimmWT, WTRow);
              if ((ptrDimmWT->WTDimmType == WTcurrentDimm.WTDimmType) &&
                (ptrDimmWT->WTLvAndDramDensity == WTcurrentDimm.WTLvAndDramDensity) &&
                (ptrDimmWT->WTDimmPos == WTcurrentDimm.WTDimmPos)) {
                CheckDimmInWeightTable.TableEntryPresent = TRUE;
                RcDebugPrint (SDBG_MAX,
                  "::MEM NVMDIMM:: (%d)        %x, %x, %x, ",
                  ptrDimmWT->WTLvAndDramDensity,
                  ptrDimmWT->WTRankNumIoWidth,
                  ptrDimmWT->WTDimmPos,
                  ptrDimmWT->WTFrequency);
                RcDebugPrint (SDBG_MAX,
                  "0x%x,  %d,  %d,  %d,  %d,  %d,  ",
                  ptrDimmWT->WTPmCmdPwrData,
                  ptrDimmWT->DramPmWritePowerCoeff,
                  ptrDimmWT->DramMaxPwr,
                  ptrDimmWT->DramTdp,
                  ptrDimmWT->DramMinPwrClosedPage,
                  ptrDimmWT->DramWritePwrScaleClosedPage);
                RcDebugPrint (SDBG_MAX,
                  "%d,  %d,  %d,  %d,  %d,  %d\n\n",
                  ptrDimmWT->DramMaxPwrOpenPage,
                  ptrDimmWT->DramTdpOpenPage,
                  ptrDimmWT->DramMinPwrOpenPage,
                  ptrDimmWT->DramWritePwrScaleOpenPage,
                  ptrDimmWT->DramRefreshRateSlope,
                  ptrDimmWT->DramSelfRefreshPower);
                break;
              }
            } // NVMDIMM WTRow loop
          }
        } else {
          RcDebugPrint (SDBG_MAX, "\nMPM: ERROR: unsupported DIMM SPD type: %d\n", (*DimmNvList)[Dimm].keyByte);
        }
        if (CheckDimmInWeightTable.TableEntryPresent) {
          ConfigurePwrPerCmd (Socket, Ch, Dimm, ptrDimmWT->WTPmCmdPwrData);
        } else {
          // Output EWL 26 MPM Warning
          MemoryLocation.Socket = Socket;
          MemoryLocation.Channel = Ch;
          MemoryLocation.Dimm = Dimm;
          MemoryLocation.Rank = NO_RANK;
          Severity = EwlSeverityWarning;

          EwlOutputType26 (WARN_MEMORY_POWER_MANAGEMENT, WARN_MEMORY_PM_WEIGHT_TABLE_NOT_FOUND, MemoryLocation, Severity);
          ConfigurePwrPerCmd (Socket, Ch, Dimm, 0);
        }

        //
        // Initialize Socket DRAM TDP structure for use by the DRAM VR Phase Shedding initialization code
        //
        if (CheckDimmInWeightTable.TableEntryPresent) {
          // Read the TDP from the table
          perChMaxMemPwr[Ch] = perChMaxMemPwr[Ch] + (ptrDimmWT->DramMaxPwr);
          perChSelfRefreshPwr[Ch] = perChSelfRefreshPwr[Ch] + (ptrDimmWT->DramSelfRefreshPower);
        } else {
          //load default value
          switch ((*DimmNvList)[Dimm].keyByte2) {
          case SPD_UDIMM:
            perChMaxMemPwr[Ch] = perChMaxMemPwr[Ch] + CHANNEL_MAX_MEM_PWR_UDIMM;
            perChSelfRefreshPwr[Ch] = perChSelfRefreshPwr[Ch] + CHANNEL_SEFL_REFRESH_PWR_UDIMM;
            break;
          case SPD_UDIMM_ECC:
            perChMaxMemPwr[Ch] = perChMaxMemPwr[Ch] + CHANNEL_MAX_MEM_PWR_UDIMM_ECC;
            perChSelfRefreshPwr[Ch] = perChSelfRefreshPwr[Ch] + CHANNEL_SEFL_REFRESH_PWR_UDIMM_ECC;
            break;
          case SPD_LRDIMM:
            perChMaxMemPwr[Ch] = perChMaxMemPwr[Ch] + CHANNEL_MAX_MEM_PWR_LRDIMM;
            perChSelfRefreshPwr[Ch] = perChSelfRefreshPwr[Ch] + CHANNEL_SEFL_REFRESH_PWR_LRDIMM;
            break;
          case SPD_RDIMM:
          default:
            perChMaxMemPwr[Ch] = perChMaxMemPwr[Ch] + CHANNEL_MAX_MEM_PWR_RDIMM;
            perChSelfRefreshPwr[Ch] = perChSelfRefreshPwr[Ch] + CHANNEL_SEFL_REFRESH_PWR_RDIMM;
          } // end switch
        }
        //
        // Init DRAM RAPL Settings here
        //
        ConfigureDramRaplFeature (Socket, CheckDimmInWeightTable.TableEntryPresent, (VOID *)ptrDimmWT, (VOID *)&WTcurrentDimm, (VOID *)&RaplData, Ch, Dimm);
        //
        // Initialize the temp thresholds and bandwidth limits
        //
        if (CheckDimmInThrottleModeTable.TableEntryPresent) {
          InitalizeTempThreshold (Socket, Ch, Dimm, ptrDimmTTMODE->TTTHRT_HI, CheckDimmInThrottleModeTable.TableEntryPresent);
        } else {
          if ((varThermalThrottlingOptions & OLTT_EN) != 0) {
            InitalizeTempThreshold (Socket, Ch, Dimm, OLTT_THRT_HI_DEFAULT, CheckDimmInThrottleModeTable.TableEntryPresent);
          } else if ((varThermalThrottlingOptions & (CLTT_EN | CLTT_PECI_EN))) {
            InitalizeTempThreshold (Socket, Ch, Dimm, CLTT_THRT_HI_DEFAULT, CheckDimmInThrottleModeTable.TableEntryPresent);
          }
        }
        //
        // Look up DIMM thermal threshold offset in the fixed table
        // First get the data needed to match a table entry for this Dimm
        //
        CheckDimmInThrottleTable.CfgValid = TRUE;
        CheckDimmInThrottleTable.TableEntryPresent = FALSE;
        TTcurrentDimm.TTDimmType = (*DimmNvList)[Dimm].actKeyByte2;
        if (TTcurrentDimm.TTDimmType == SPD_LRDIMM_DDR5) {
          TTcurrentDimm.TTDimmType = SPD_LRDIMM;
        }

        if (!((TTcurrentDimm.TTDimmType == SPD_RDIMM) ||
          (TTcurrentDimm.TTDimmType == SPD_LRDIMM))) {
          TTcurrentDimm.TTDimmType = SPD_RDIMM;
        }

        TTcurrentDimm.TTDramDensity = (*DimmNvList)[Dimm].sdramCapacity;
        // Substitute 16 Gb for anything other than 16 Gb
        if (TTcurrentDimm.TTDramDensity != SPD_16Gb) {
          TTcurrentDimm.TTDramDensity = SPD_16Gb;
        }

        TTcurrentDimm.TTRankNumIoWidth = (*DimmNvList)[Dimm].actSPDModuleOrg;

        // For x16, substitute x8
        if ((TTcurrentDimm.TTRankNumIoWidth & 0x7) == SPD_DRAM_IO_WIDTH_X16) {
          TTcurrentDimm.TTRankNumIoWidth &= 0xF8;
          TTcurrentDimm.TTRankNumIoWidth |= SPD_DRAM_IO_WIDTH_X8;
        }

        if ((((TTcurrentDimm.TTRankNumIoWidth & 0x7) > SPD_DRAM_IO_WIDTH_X8) ||
          ((TTcurrentDimm.TTRankNumIoWidth & 0x38) > (SPD_NUM_RANKS_8 << 3))) && (TTcurrentDimm.TTDimmType == SPD_LRDIMM)) {
          CheckDimmInThrottleTable.CfgValid = FALSE;
          RcDebugPrint (SDBG_DEFAULT, "MPM: Invalid Config\n");
        }

        TTcurrentDimm.TTFrequency = Host->nvram.mem.socket[Socket].ddrFreq;
        TTcurrentDimm.TTDPC =  (*ChannelNvList)[Ch].maxDimm;

        // UDIMM substitute RDIMM
        if ((TTcurrentDimm.TTDimmType == SPD_UDIMM) || (TTcurrentDimm.TTDimmType == SPD_UDIMM_ECC)) {
          TTcurrentDimm.TTDimmType = SPD_RDIMM;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket,  Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "\nMPM: TTDimmType =%d,TTDramDensity =%d, TTRankNumIoWidth = %d, TTFrequency = %d, TTDPC = %d\n",
                 TTcurrentDimm.TTDimmType, TTcurrentDimm.TTDramDensity, TTcurrentDimm.TTRankNumIoWidth, TTcurrentDimm.TTFrequency, TTcurrentDimm.TTDPC);

        if (CheckDimmInThrottleTable.CfgValid) {
          for (TTRow = 0; TTRow < GetDimmTTTableSize (); TTRow++) {
            GetDimmTTTable (&ptrDimmTT, TTRow);
            if ((ptrDimmTT->TTDimmType == TTcurrentDimm.TTDimmType) &&
              (ptrDimmTT->TTDramDensity == TTcurrentDimm.TTDramDensity) &&
              ((ptrDimmTT->TTRankNumIoWidth == TTcurrentDimm.TTRankNumIoWidth) || (ptrDimmTT->TTRankNumIoWidth == 0)) &&
              (ptrDimmTT->TTFrequency == TTcurrentDimm.TTFrequency) &&
              (ptrDimmTT->TTDPC == TTcurrentDimm.TTDPC)) {
              CheckDimmInThrottleTable.TableEntryPresent = TRUE;
              RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "\nMPM: Found TTStaticData = 0x%x, TTPower = 0x%x in Table DimmTT Row (%d)\n", ptrDimmTT->TTStaticData, ptrDimmTT->TTPower, TTRow);
              break;
            }
          } // TTRow loop
        }
        if ((varThermalThrottlingOptions & CLTT_EN) != 0) {
          //
          // Skip this programming if using PECI Mode
          //
          if (CheckDimmInThrottleTable.TableEntryPresent) {
            if (((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP)) {
              ThrottleValue.ThrottlingDimmTempOffsetValue = 0;
            } else {
              ThrottleValue.ThrottlingDimmTempOffsetValue = ptrDimmTT->TTStaticData;
            }
          } else {
            ThrottleValue.ThrottlingDimmTempOffsetValue = TT_DIMM_TEMP_OFFSET_DDR5;
          }

          //In some dimms, default temperature status causes the system to trip
          //This is a permanent hardware workaround to decrease the default temperature before
          //enabling thermal management in CPL3\4
          GetSetMemTempStatusRegValues (Socket, Ch, Dimm, READ_THROTTLE_VALUES, EnumTempStatusCurrentDimmTemp, &TempStatusCurrentDimmTemp);
          if (TempStatusCurrentDimmTemp >= ThrottleValue.ThrottlingDimmTempOffsetValue) {
            TempStatusCurrentDimmTemp -= ThrottleValue.ThrottlingDimmTempOffsetValue;
          }
          GetSetMemTempStatusRegValues (Socket, Ch, Dimm, WRITE_THROTTLE_VALUES, EnumTempStatusCurrentDimmTemp, &TempStatusCurrentDimmTemp);
          GetSetThrottleValues (MemTechDdr, Socket, Ch, Dimm, WRITE_THROTTLE_VALUES, EnumThrottlingDimmTempOffsetValue, &ThrottleValue);

          //
          // Initialize Tcrit to 105(MAX_CRITICAL) - temp offset
          //
          if ((*DimmNvList)[Dimm].DcpmmPresent) {
            GetSetThrottleValues (MemTechDdr, Socket, Ch, Dimm, READ_THROTTLE_VALUES, EnumThrottlingDimmTempOffsetValue, &ThrottleValue);
            SetFmcTempCrit (Host, Socket, Ch, Dimm, (((PcdGet8 (PcdMaxTCriticalValue) - (UINT16)ThrottleValue.ThrottlingDimmTempOffsetValue) << 4) & 0x0FFF));
          } else {
            InitializeTsod (Socket, Ch, Dimm);
          }
        } // CLTT_EN

        if ((varThermalThrottlingOptions & OLTT_EN) != 0) {
          GetSetMemTempStatusRegValues (Socket, Ch, Dimm, READ_THROTTLE_VALUES, EnumTempStatusCurrentDimmTemp, &TempStatusCurrentDimmTemp);
          TempStatusCurrentDimmTemp = OLTT_STATIC_DIMM_TEMP_VALUE;
          GetSetMemTempStatusRegValues (Socket, Ch, Dimm, WRITE_THROTTLE_VALUES, EnumTempStatusCurrentDimmTemp, &TempStatusCurrentDimmTemp);
        }//OLTT_EN

      } // Dimm loop
    } // Ch enabled
  } // Ch loop

  RaplData.RAPLMode = 1;
  DramRaplProg (Socket, &RaplData);
  ConfigureMemoryThermalFeature (Socket, ConfigurePcuBiosSpare2RegType);

  if (B2PFlag & ENABLE_DRAM_PM) {
    IssueDramPowerManagementCommandToPcu (Socket);
  }

  if (varThermalThrottlingOptions & CLTT_EN) {
    IssueDramThermalManagementCommandToPcu (Socket, CLTT_ENABLE);
  }

  if (varThermalThrottlingOptions & CLTT_PECI_EN) {
    IssueDramThermalManagementCommandToPcu (Socket, CLTT_PECI_ENABLE);
  }

  ConfigureMinOps (Socket);

#ifdef DEBUG_CODE_BLOCK
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
  return SUCCESS;
} //InitThrottling
