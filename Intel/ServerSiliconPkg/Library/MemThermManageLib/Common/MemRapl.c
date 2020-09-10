/** @file
  This file Implements Memory RAPL feature

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
#include <SysHost.h>
#include <SysHostChip.h>
#include <Library/SysHostPointerLib.h>
#include <Include/MemRapl.h>
#include <Library/MemPwrTempTableLib.h>
#include <Include/MemThrot.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Memory/MemDefaults.h>

/**

  This outine will scale the calculated power values to be programmed for DRAM RAPL based upon the looked up values
  based on the refresh rate and a linear scaling factor from looked up values.

  @param[in] Socket         Socket number
  @param[in] Slope          Slope of the Power Input
  @param PowerInput     Unscaled power value

  @retval Scaled power value

**/
UINT16
RefreshScaledPwr (
  IN UINT8    Socket,
  IN UINT32   Slope,
  IN UINT16   PowerInput
  )
{
  UINT16    ScaledPower = 0;
  UINT32    RefreshRate = 0;
  UINT32    RefreshAdder = 0;
  PSYSHOST  Host = NULL;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();

  //
  // If refresh rate is >1x, then scale DRAM power calculations
  //
  RefreshRate = (UINT32)Host->nvram.mem.socket[Socket].refreshRate;

  if ((Setup->mem.dramraplRefreshBase >= 10) && (RefreshRate > Setup->mem.dramraplRefreshBase)) {
    RefreshAdder = (((UINT32)Slope * (RefreshRate - Setup->mem.dramraplRefreshBase)) / 10000);
  }
  ScaledPower = PowerInput + (UINT16)RefreshAdder;

  return ScaledPower;
} //RefreshScaledPwr

/**

  This routine will progam registers for Initializing DRAM RAPL

  @param[in] Socket                      - Socket number
  @param[in] pointer DRamRaplDataStruct  - Strucutre containing the values to be should be Programmed

  @retval SUCCESS

**/
VOID
DramRaplProg (
  IN UINT8              Socket,
  IN OUT DramRaplDataStruct *RaplData
  )
{
  UINT16 DramTdp;
  UINT16 DramMinPwr;
  UINT16 DramMaxPwr;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();


  if (Setup->mem.DramRaplExtendedRange != 0) {
    RaplData->RAPLDramPwrInfoMinVal = RaplData->RAPLDramPwrInfoMinVal / 2;
  }

  //
  //... Program lower dword
  //
  DramTdp = RaplData->RAPLDramPwrInfoTdpVal;
  DramMinPwr = (RaplData->RAPLDramPwrInfoMinVal / 10);
  DramMaxPwr = RaplData->RAPLDramPwrInfoMaxVal;


  ProgramDramRaplValues (Socket, DramTdp, DramMinPwr, DramMaxPwr);

  return;
} //DramRaplProg

/**

  This outine will calculate the values to be programmed for DRAM RAPL based upon the looked up values

  @param[in] Socket        - Socket number
  @param[in] Channel       - channel
  @param[in] Dimm          - Dimm number
  @param[in][out] pointer DRamRaplDataStruct  - Strucutre containing the values to be should be Programmed
  @param[in][out] pointer DramRaplLUTData     - Strucutre containing looked up which the calculations will be based upon

  @retval N/A

**/
VOID
DramRaplProgCalc (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN OUT DramRaplDataStruct *RaplData,
  IN OUT DramRaplLUTDataStruct *RaplLutData
  )
{
  PSYSHOST             Host = NULL;
  struct dimmNvram     (*DimmNvList)[MAX_DIMM];
  THROTTLE_VALUE       ThrottleValue;

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  ThrottleValue.MemThrotAllowIsoch = FALSE;
  GetSetThrottleValues (MemTechDdr, Socket, Channel, NO_DIMM_OPTION, READ_THROTTLE_VALUES, EnumMemThrotAllowIsoch, &ThrottleValue);

  // ... Calculate CKE policy mode
  // ICX_TODO

  // ... Set Page mode policy

  if (IsMcMtrClosePageMapping (Socket, Channel)) {
    RaplData->RAPL3SigmaPolicy |= BIT1;    // Indicate close page (i.e. BIT1=1)
  } else {
    RaplData->RAPL3SigmaPolicy &= ~BIT1;    // Indicate open page (i.e. BIT1=0)
  }

  //
  // Initialize DRAM RAPL with values from lookup table
  //

  // Get DramMaxPwr & DramTdp based upon page mode policy open/closed
  if ((RaplData->RAPL3SigmaPolicy & BIT1) != 0) {
    // Init DramMaxPwr
    RaplData->RAPLDramPwrInfoMaxVal = RaplData->RAPLDramPwrInfoMaxVal + RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMaxPwr);
    // Init DramTdp
    RaplData->RAPLDramPwrInfoTdpVal = RaplData->RAPLDramPwrInfoTdpVal + RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramTdp);
  } else {
    // Init DramMaxPwr
    RaplData->RAPLDramPwrInfoMaxVal = RaplData->RAPLDramPwrInfoMaxVal + RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMaxPwrOpenPage);
    // Init DramTdp
    RaplData->RAPLDramPwrInfoTdpVal = RaplData->RAPLDramPwrInfoTdpVal + RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramTdpOpenPage);
  }

  // Get Min pwr based upon page... NOTE: the end result is cumlative for all the DIMMS in a channel
  if ((RaplData->RAPL3SigmaPolicy & BIT1) != 0) {
    if (ThrottleValue.MemThrotAllowIsoch) {
      if (((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR4) || ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5)) {
        RaplData->RAPLDramPwrInfoMinVal = RaplData->RAPLDramPwrInfoMinVal + (RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMinPwrClosedPage) * 13);
      } else {
        RaplData->RAPLDramPwrInfoMinVal = RaplData->RAPLDramPwrInfoMinVal + 24 + (RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMinPwrClosedPage) * 12);
      }
    } else {
      RaplData->RAPLDramPwrInfoMinVal = RaplData->RAPLDramPwrInfoMinVal + (RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMinPwrClosedPage) * 10);
    }
  } else {
    if (ThrottleValue.MemThrotAllowIsoch) {
      if (((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR4) || ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5)) {
        RaplData->RAPLDramPwrInfoMinVal = RaplData->RAPLDramPwrInfoMinVal + (RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMinPwrOpenPage) * 13);
      } else {
        RaplData->RAPLDramPwrInfoMinVal = RaplData->RAPLDramPwrInfoMinVal + 24 + (RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMinPwrOpenPage) * 12);
      }
    } else {
      RaplData->RAPLDramPwrInfoMinVal = RaplData->RAPLDramPwrInfoMinVal + (RefreshScaledPwr (Socket, RaplLutData->DramRaplDataDramRefreshRateSlope, RaplLutData->DramRaplDataDramMinPwrOpenPage) * 10);
    }
  }
  return;
} //DramRaplProgCalc

/**

  This routine will calculate the values to be programmed for DRAM RAPL based upon the looked up values

  @param[in] Socket        - Socket number
  @param[in] DimmInWeightTable    Indicates dimm is in WTT Table
  @param[in][out] PtrDimmWieghtTable          - Pointer to DimmWT
  @param[in] WeightTableCurrentDimm         Pointer to current weight table
  @param[in][out] pointer DramRaplDataStruct  - structure containing the values to be should be Programmed
  @param[in] Channel  - Current Channel to configure
  @param[in] Dimm     - Current Dimm to program

  @retval N/A

**/
VOID
ConfigureDramRaplFeature (
  IN UINT8 Socket,
  IN BOOLEAN  DimmInWeightTable,
  IN OUT VOID   *PtrDimmWieghtTable,
  IN VOID *WeightTableCurrentDimm,
  IN OUT DramRaplDataStruct *RaplData,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  PSYSHOST               Host = NULL;
  struct dimmNvram       (*DimmNvList)[MAX_DIMM];
  DramRaplLUTDataStruct  RaplLutData;
  DimmWTRowEntryStruct   *PtrDimmWT = NULL;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();
  Host = GetSysHostPointer ();

  PtrDimmWT = (DimmWTRowEntryStruct *)PtrDimmWieghtTable;
  if (Setup->mem.DramRaplEnable == DRAM_RAPL_DISABLE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "DRAM RAPL disabled in BIOS Knob\n\n");
    return;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Init DRAM RAPL\n\n");

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RaplLutData.DramRaplDataDramPmWritePowerCoeffCval = 0;
  RaplLutData.DramRaplDataDramMaxPwr = 0;
  RaplLutData.DramRaplDataDramTdp = 0;
  RaplLutData.DramRaplDataDramMinPwrClosedPage = 0;
  RaplLutData.DramRaplDataDramWritePwrScaleClosedPage = 0;
  RaplLutData.DramRaplDataDramMaxPwrOpenPage = 0;
  RaplLutData.DramRaplDataDramTdpOpenPage = 0;
  RaplLutData.DramRaplDataDramMinPwrOpenPage = 0;
  RaplLutData.DramRaplDataDramWritePwrScaleOpenPage = 0;
  RaplLutData.DramRaplDataDramRefreshRateSlope = 0;
  RaplLutData.DramRaplDataDramSelfRefreshPower = 0;

  if (!DimmInWeightTable) {
    if ((*DimmNvList)[Dimm].keyByte2 == SPD_UDIMM) {
      RaplLutData.DramRaplDataDramPmWritePowerCoeffCval = UDIMM_POWER_COEFF_DEFAULT;
      RaplLutData.DramRaplDataDramMaxPwr = UDIMM_MAX_POWER_DEFAULT;
      RaplLutData.DramRaplDataDramTdp = UDIMM_DRAM_TDP_DEFAULT;
      RaplLutData.DramRaplDataDramMinPwrClosedPage = UDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramWritePwrScaleClosedPage = UDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramMaxPwrOpenPage = UDIMM_MAX_PWR_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramTdpOpenPage = UDIMM_TDP_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramMinPwrOpenPage = UDIMM_MIN_PWR_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramWritePwrScaleOpenPage = UDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramRefreshRateSlope = UDIMM_REFRESH_RATE_SLOPE_DEFAULT;
      RaplLutData.DramRaplDataDramSelfRefreshPower = UDIMM_SELF_REFRESH_POWER_DEFAULT;
    } else if ((*DimmNvList)[Dimm].actKeyByte2 == SPD_LRDIMM) {
      RaplLutData.DramRaplDataDramPmWritePowerCoeffCval = LRDIMM_POWER_COEFF_DEFAULT;
      RaplLutData.DramRaplDataDramMaxPwr = LRDIMM_MAX_POWER_DEFAULT;
      RaplLutData.DramRaplDataDramTdp = LRDIMM_DRAM_TDP_DEFAULT;
      RaplLutData.DramRaplDataDramMinPwrClosedPage = LRDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramWritePwrScaleClosedPage = LRDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramMaxPwrOpenPage = LRDIMM_MAX_PWR_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramTdpOpenPage = LRDIMM_TDP_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramMinPwrOpenPage = LRDIMM_MIN_PWR_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramWritePwrScaleOpenPage = LRDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramRefreshRateSlope = LRDIMM_REFRESH_RATE_SLOPE_DEFAULT;
      RaplLutData.DramRaplDataDramSelfRefreshPower = LRDIMM_SELF_REFRESH_POWER_DEFAULT;
    } else {  // assume RDIMM
      RaplLutData.DramRaplDataDramPmWritePowerCoeffCval = RDIMM_POWER_COEFF_DEFAULT;
      RaplLutData.DramRaplDataDramMaxPwr = RDIMM_MAX_POWER_DEFAULT;
      RaplLutData.DramRaplDataDramTdp = RDIMM_DRAM_TDP_DEFAULT;
      RaplLutData.DramRaplDataDramMinPwrClosedPage = RDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramWritePwrScaleClosedPage = RDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramMaxPwrOpenPage = RDIMM_MAX_PWR_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramTdpOpenPage = RDIMM_TDP_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramMinPwrOpenPage = RDIMM_MIN_PWR_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramWritePwrScaleOpenPage = RDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT;
      RaplLutData.DramRaplDataDramRefreshRateSlope = RDIMM_REFRESH_RATE_SLOPE_DEFAULT;
      RaplLutData.DramRaplDataDramSelfRefreshPower = RDIMM_SELF_REFRESH_POWER_DEFAULT;
    }
  } else {
    //
    // Initialize DRAM RAPL with values from lookup table
    //
    RaplLutData.DramRaplDataDramPmWritePowerCoeffCval = PtrDimmWT->DramPmWritePowerCoeff;
    RaplLutData.DramRaplDataDramMaxPwr = PtrDimmWT->DramMaxPwr;
    RaplLutData.DramRaplDataDramTdp = PtrDimmWT->DramTdp;
    RaplLutData.DramRaplDataDramMinPwrClosedPage = PtrDimmWT->DramMinPwrClosedPage;
    RaplLutData.DramRaplDataDramWritePwrScaleClosedPage = PtrDimmWT->DramWritePwrScaleClosedPage;
    RaplLutData.DramRaplDataDramMaxPwrOpenPage = PtrDimmWT->DramMaxPwrOpenPage;
    RaplLutData.DramRaplDataDramTdpOpenPage = PtrDimmWT->DramTdpOpenPage;
    RaplLutData.DramRaplDataDramMinPwrOpenPage = PtrDimmWT->DramMinPwrOpenPage;
    RaplLutData.DramRaplDataDramWritePwrScaleOpenPage = PtrDimmWT->DramWritePwrScaleOpenPage;
    RaplLutData.DramRaplDataDramRefreshRateSlope = PtrDimmWT->DramRefreshRateSlope;
    RaplLutData.DramRaplDataDramSelfRefreshPower = PtrDimmWT->DramSelfRefreshPower;
  }

  // Print debug info we used to look of the 2sigma values
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DIMM Type = 0x%02X\n ", ((DimmWTRowEntryStruct *)WeightTableCurrentDimm)->WTDimmType);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Density = 0x%02X\n ", ((DimmWTRowEntryStruct *)WeightTableCurrentDimm)->WTLvAndDramDensity);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "#ranks/width = 0x%02X\n ", ((DimmWTRowEntryStruct *)WeightTableCurrentDimm)->WTRankNumIoWidth);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DIMM Position = 0x%02X\n ", ((DimmWTRowEntryStruct *)WeightTableCurrentDimm)->WTDimmPos);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Frequency = 0x%02X\n ", ((DimmWTRowEntryStruct *)WeightTableCurrentDimm)->WTFrequency);

  // We are going to dump the data that is used for the DRAM RAPL calculations
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Write Power Coeff (Constant) = %04d\n ", RaplLutData.DramRaplDataDramPmWritePowerCoeffCval);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Power Info DRAMMaxPwr (closed page) = %04d\n ", RaplLutData.DramRaplDataDramMaxPwr);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Power Info DRAMTDP (closed page) = %04d\n ", RaplLutData.DramRaplDataDramTdp);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Power Info DRAMMinPwr (closed page) = %04d\n ", RaplLutData.DramRaplDataDramMinPwrClosedPage);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Write PWR BW Scale (s va.lue) [closed page]) = %04d\n ", RaplLutData.DramRaplDataDramWritePwrScaleClosedPage);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Power Info DRAMMaxPwr (open page) = %04d\n ", RaplLutData.DramRaplDataDramMaxPwrOpenPage);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Power Info DRAMTDP (open page) = %04d\n ", RaplLutData.DramRaplDataDramTdpOpenPage);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Power Info DRAMMinPwr (open page) = %04d\n ", RaplLutData.DramRaplDataDramMinPwrOpenPage);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Write PWR BW Scale (s va.lue) [open page]) = %04d\n ", RaplLutData.DramRaplDataDramWritePwrScaleOpenPage);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM Refresh Rate Slope = %04d\n ", RaplLutData.DramRaplDataDramRefreshRateSlope);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DRAM SelfRefresh Power [Need to be updated!] = %04d\n ", RaplLutData.DramRaplDataDramSelfRefreshPower);

  DramRaplProgCalc (Socket, Channel, Dimm, RaplData, &RaplLutData);
  return;
} //ConfigureDramRaplFeature
