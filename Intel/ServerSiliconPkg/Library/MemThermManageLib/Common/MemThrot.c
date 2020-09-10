/** @file
  Includes implementation for Memory Throttling

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
#include <SysHostChip.h>
#include <UncoreCommonIncludes.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemPwrTempTableLib.h>
#include <Include/MemThrot.h>
#include <Library/MemRcLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/MemVrSvidMapLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

#include <ImonVrSvid.h>

/**

  VR SVID's for MC on Oem Platform

  @param[in] Socket              - Socket number
  @param[in] McId                - Memory Controller Id

  @retval - VR SVID for given Socket\McId

**/

UINT8
GetVRsSVID (
  IN UINT8     Socket,
  IN UINT8     McId
  )
{
  UINT8 SvidValue = 0;
  EFI_STATUS    Status = EFI_SUCCESS;

  Status = GetSvidMap (Socket, McId, &SvidValue);
  if(Status == EFI_NOT_FOUND) {
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PcdMemSrVidMap = NULL\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SocketId = %d, McId = %d, SvidValue = %d\n", Socket, McId, SvidValue);
  }

  return SvidValue;

} // GetVRsSVID

/**
  Enables  IMON polling for given SVID.
  Data is provided by PcdImonAddr.

  @param[in] Socket - 0 based socket number

  @retval EFI_SUCCESS - Successfully programmed value
  @retval !EFI_SUCCESS - Function not completed successfully
**/
EFI_STATUS
EnableImon (
  IN UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 i = 0;
  VCC_IMON *VccImon = NULL;

  VccImon = (VCC_IMON *) PcdGetPtr (PcdImonAddr);
  if (VccImon == NULL) {
    RcDebugPrint (SDBG_ERROR, "EnableImon() - PcdImonAddr == NULL\n");
    return EFI_NOT_FOUND;
  }

  for (i = 0; i < MAX_IMON_COUNT; i++) {
    if (VccImon->VrSvid[i] == IMON_ADDR_LIST_END) {
      RcDebugPrint (SDBG_INFO, "EnableImon() - Reached VccImon Array end\n");
      break;
    }
    RcDebugPrint (SDBG_INFO, "EnableImon() - Enabling IMON polling for Socket = %d, VrSvid = %d\n",
      Socket, VccImon->VrSvid[i]);
    Status = PcuEnableImon (Socket, VccImon->VrSvid[i]);
    RcDebugPrint (SDBG_INFO, "EnableImon() - Status %r\n", Status);
  }
  if (i == MAX_IMON_COUNT) {
      RcDebugPrint (SDBG_ERROR, "EnableImon() - PcdImonAddr Array should end with 0xFF\n");
  }
  return Status;
}
/**

  This function issues PCODE command for Dram Power management

  @param[in]  Socket               Socket Number

  @retval N/A

**/
VOID
IssueDramPowerManagementCommandToPcu (
  UINT8 Socket
  )
{
  UINT8     MemoryControllerId;
  UINT8     ImcEnableMask;
  UINT8     MaxImc;
  PSYSHOST  Host = NULL;
  Host   = GetSysHostPointer ();
  MaxImc = GetMaxImc ();

  ImcEnableMask = 0;

  //Set MC Bit Mask to indicate populated MCs on Skt
  for (MemoryControllerId = 0; MemoryControllerId < MaxImc; MemoryControllerId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[MemoryControllerId] == 1) {
      ImcEnableMask |= (1 << MemoryControllerId);
    }
  }

  //
  // SPR A0: PMON clock must be running for DRAM RAPL
  //
  EnablePmonClock (Socket);

  //
  // BIOS issues ENABLE DRAM_PM
  // MC0 - [0]; MC1 - [1]  BitMask - 0x03
  //
  PcuEnableDramPm (Socket, ImcEnableMask);
} // IssueDramPowerManagementCommandToPcu

/**

  Control PM and thermal enable using Pcode

  @param flag    Flag to indicate enable\disable Dram power and thermal management

  @retval VOID

**/
VOID
ControlDramPMThermalViaPcode (
  UINT8     Flag
  )
{
  UINT8               Socket;
  UINT8               Channel;
  UINT8               McId;
  UINT8               Dimm;
  UINT8               VrSvid;
  UINT8               MaxImc;
  BOOLEAN             DDRPresentFlag[MAX_IMC] = { 0 };
  BOOLEAN             DDRTPresentFlag[MAX_IMC] = { 0 };
  UINT8               DDRDimmMask[MAX_CH] = { 0 };
  UINT8               DDRTDimmMask[MAX_CH] = { 0 };
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               ImcEnMask = 0;
  PSYSHOST            Host = NULL;
  UINT8               NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Host       = GetSysHostPointer ();
  MaxImc     = GetMaxImc ();
  Socket     = Host->var.mem.currentSocket;

  // Enable IMON Polling
  EnableImon (Socket);

#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Socket)) {
    // Only Report VROD for DDR5 System
    PcuReportVrod (Socket);
  }
#endif

  //Set MC Bit Mask to indicate populated MCs on Skt
  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 1) {
      ImcEnMask |= (1 << McId);
    }
  } // mcId loop

  //
  // Collect the Dimm type information present in the Memory controller
  //
  ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;

  if (Flag == DISABLE_DRAM_MPM) {
    //
    // BIOS issues Disable_DRAM_PM
    // BIOS polls the Disable_Check
    // (MC0 - [0]; MC1 - [1] RGT0 - [2] ; RGT1 [3] ---till RGT7 - [9]) BitMask - 0x1FF
    //
    PcuDisableDramPm (Socket, ImcEnMask);
    //
    // BIOS issues Disable_DRAM_THERMAL
    // BIOS polls the Disable_Check
    // MC0 - [0]; MC1 - [1]  BitMask - 0x03
    //
    PcuDisableDramThermal (Socket, DRAM_THERMAL_DATA_MC_DISABLE_MASK);

  } else {

    //Collect the Dimm type information present in the Memory controller .
    for (McId = 0; McId < MaxImc; McId++) {
      if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
        continue;
      }
      ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;

      for (Channel = 0; Channel < NumChPerMc; Channel++) {
        if ((*ChannelNvList)[Channel + (NumChPerMc * McId)].enabled == 0) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Channel + (NumChPerMc * McId));

        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          if ((*DimmNvList)[Dimm].dimmPresent == 0) {
            continue;
          }

          if ((*DimmNvList)[Dimm].DcpmmPresent != 0) {
            DDRTDimmMask[(McId * NumChPerMc) + Channel] |= (1 << Dimm); //Collect DIMM Mask of NVMDIMM DIMMs
            DDRTPresentFlag[McId] = 1;
          } else {
            DDRPresentFlag[McId] = 1;
            DDRDimmMask[(McId * NumChPerMc) + Channel] |= (1 << Dimm); //Collect DIMM Mask of Normal DDR DIMMs
          }
        } // Dimm loop
      } // Channel loop
    } // McId loop

    for (McId = 0; McId < MaxImc; McId++) {

      EFI_STATUS Status;
      MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
      MRC_STATUS HookStatus;

      if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
        continue;
      }

      //
      // If Hooks PlatformVRsSVID returns not implemented, or
      // if the hooks are not present, get the generic VR SVID.
      //

      HookStatus = MRC_FUNCTION_NOT_IMPLEMENTED;
      VrSvid = 0;

      Status = LocateInterface (
        &gMrcPlatformHooksInterfaceGuid,
        0,
        (VOID **)&MrcPlatformHooksInterface
        );
      if (!EFI_ERROR (Status)) {
        HookStatus = MrcPlatformHooksInterface->PlatformVRsSVID (Socket, McId, &VrSvid);
      }

      if (HookStatus == MRC_FUNCTION_NOT_IMPLEMENTED) {
        VrSvid = GetVRsSVID (Socket, McId);
      }

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "McId = %d, VR SVID = %x\n", McId, VrSvid);
      if ((DDRPresentFlag[McId]) || (DDRTPresentFlag[McId])) {
        PcuProgramVrMcMapping (Socket, McId, VrSvid);
      }

      //
      // Program 12v VR sensor mapping
      // For DDR4 systems, only needed if a DDRT DIMM is installed on the MC.
      // For DDR5 systems (excluding BRS), needed on every MC
      //
      if ((DDRTPresentFlag[McId]) ||
          ((IsDdr5Present (Host, Socket)) && (!IsBrsPresent (Host, Socket))))
      {
        PcuProgramVrSensorMapping (Socket, McId, VrSvid);
      }

      // BITS 25:24 - 12V sensor dimm mask and BITS 17:16 - DDR DIMM MASK. BITS 9:8 - channel id and BIT 0 -MCID
      for (Channel = 0; Channel < NumChPerMc; Channel++) {
        //if(channel is enabled) check should be added here?- Needs to be checked with Nikhil.
        PcuProgramVrDimmMapping (Socket, DDRTDimmMask[(McId * NumChPerMc) + Channel], DDRDimmMask[(McId * NumChPerMc) + Channel], McId, Channel);
      } // Channel loop
    } // McId loop
  }

  return;
} // ControlDramPMThermalViaPcode

/**

  This function issues PCODE command for Dram Power management

  @param[in]  Socket               Socket Number
  @param[in]  ThrottlingOption     CLTT_EN\CLTT_PECI_EN option

  @retval N/A

**/
VOID
IssueDramThermalManagementCommandToPcu (
  UINT8 Socket,
  UINT8 ThrottlingOption
  )
{
  PSYSHOST  Host = NULL;
  PCU_THROTTLE_TYPE ThrottlingType;

  Host = GetSysHostPointer ();

  if (ThrottlingOption == CLTT_ENABLE) {
    //
    // BIOS issues ENABLE DRAM_THERMAL
    //
    ThrottlingType = PCU_THROTTLE_CLTT;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\nCLTT, write to B2P DRAM_THERMAL_ENABLE.\n");
  } else if (ThrottlingOption == CLTT_PECI_ENABLE) {
    //
    // BIOS issues ENABLE DRAM_THERMAL for CLTT w/ PECI
    //
    ThrottlingType = PCU_THROTTLE_CLTT_PECI;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\nCLTT w/PECI, write to B2P DRAM_THERMAL_ENABLE.\n");
  } else {
    return;
  }

  PcuEnableDramThermal (Socket, DRAM_THERMAL_DATA_MC_ENABLE_MASK, ThrottlingType);

  return;
} // IssueDramThermalManagementCommandToPcu

/**

  This function checks for the dimm in thermal throttling mode and returns the result and the Index in the table

  @param[in] Socket                         - Socket number
  @param[in] Channel                        - Channel to test
  @param[out] DimmInThermalThrottlingMode   - Result for search in table
  @param[out] ThermalThrottlingRowIndex     - Index in the table

  @retval    N/A

**/
VOID
CheckDimmInThermalThrottlingMode (
  UINT8 Socket,
  UINT8 Channel,
  BOOLEAN *DimmInThermalThrottlingMode,
  UINT8 *ThermalThrottlingRowIndex
  )
{
  PSYSHOST                     Host = NULL;
  const DimmTThrottlingStruct  *PtrDimmThermalThrottlingMode = NULL;
  DimmTThrottlingStruct        ThermalThrottlingModecurrentDimm;
  struct channelNvram          (*ChannelNvList)[MAX_CH];
  struct dimmNvram             (*DimmNvList)[MAX_DIMM];
  UINT16                       ThermalThrottlingOptions;
  UINT8                        ThermalThrottlingModeRow = 0;
  SYS_SETUP                     *Setup;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ThermalThrottlingOptions = Setup->mem.thermalThrottlingOptions;
  *DimmInThermalThrottlingMode = FALSE;
  if ((ThermalThrottlingOptions & (OLTT_EN | CLTT_EN | CLTT_PECI_EN)) != 0) {
    ThermalThrottlingModecurrentDimm.TTMode = 0;
    if (((ThermalThrottlingOptions & (CLTT_EN | CLTT_PECI_EN)) != 0) && ((ThermalThrottlingOptions & OLTT_EN) == 0)) {
      ThermalThrottlingModecurrentDimm.TTMode = 1;
    }

    ThermalThrottlingModecurrentDimm.TTFREQ = Host->nvram.mem.socket[Socket].ddrFreq;
    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    if (((!(*DimmNvList)[0].DcpmmPresent) && (*DimmNvList)[1].DcpmmPresent) ||
      ((*DimmNvList)[0].DcpmmPresent) && (!(*DimmNvList)[1].DcpmmPresent)) {
      ThermalThrottlingModecurrentDimm.TTDIMMPC = 1;
    } else {
      ThermalThrottlingModecurrentDimm.TTDIMMPC = (*ChannelNvList)[Channel].maxDimm;
    }

    for (ThermalThrottlingModeRow = 0; ThermalThrottlingModeRow < GetDimmTTModeTableSize (); ThermalThrottlingModeRow++) {
      GetDimmTtModeTable (&PtrDimmThermalThrottlingMode, ThermalThrottlingModeRow);
      if ((PtrDimmThermalThrottlingMode->TTMode == ThermalThrottlingModecurrentDimm.TTMode) &&
        (PtrDimmThermalThrottlingMode->TTDIMMPC == ThermalThrottlingModecurrentDimm.TTDIMMPC) &&
        (PtrDimmThermalThrottlingMode->TTFREQ == ThermalThrottlingModecurrentDimm.TTFREQ)) {
        *DimmInThermalThrottlingMode = TRUE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          ":: Found ThermalThrottlingModeRow: %d,   ", ThermalThrottlingModeRow);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "TTMode: %d,  TTDIMMPC: %d,  TTFREQ: %x,  TTBW_THROTTLE_TF: %x,  TTTHRT_HI: %x\n ",
          PtrDimmThermalThrottlingMode->TTMode,
          PtrDimmThermalThrottlingMode->TTDIMMPC,
          PtrDimmThermalThrottlingMode->TTFREQ,
          PtrDimmThermalThrottlingMode->TTBW_THROTTLE_TF,
          PtrDimmThermalThrottlingMode->TTTHRT_HI
          );
        break;
      }
    } // ThermalThrottlingModeRow loop
  }

  *ThermalThrottlingRowIndex = ThermalThrottlingModeRow;
  return;
}  // CheckDimmInThermalThrottlingMode

/**

  This function gets throttle window size

  @param[in] Socket                     - Socket number
  @param[in] Channel                    - Channel to test
  @param[in] ThermalThrottlingModeRow   - Result for search in table
  @param[in] BandWidthLimitTfMultiPlier - Index in the table
  @param[out] BandWidthLimitTfData      - Data returned by the function

  @retval    N / A

**/
VOID
GetBwThrottleWindowSize (
  UINT8      Socket,
  UINT8      Channel,
  UINT8      BandWidthLimitTfMultiPlier,
  UINT32     *BandWidthLimitTfData
  )
{
  PSYSHOST                     Host = NULL;
  MC_BIOS_REQ_PCU_FUN1_STRUCT  McBiosReq;
  UINT8                        RefClkType;
  UINT8                        QclkRatio;
  UINT8                        BclkFreq;
  UINT8                        BclkMcReg;
  UINT32                       DdrFreqMts;
  UINT32                       Dclk;
  THROTTLE_VALUE               ThrottleValue;

  Host = GetSysHostPointer ();
  BclkFreq = GetBclkFreq (Socket);

  McBiosReq.Data = UsraCsrRead (Socket, 0, MC_BIOS_REQ_PCU_FUN1_REG);

  QclkRatio = (UINT8) McBiosReq.Bits.req_data; // The encoding of this field is BCLK multiplier for QCLK.
  RefClkType = (UINT8) McBiosReq.Bits.req_type;

  if (RefClkType == BCLK_TYPE_133) {
    BclkMcReg = IMC_BCLK_133;
  } else if (RefClkType == BCLK_TYPE_100) {
    BclkMcReg = IMC_BCLK_100;
  } else {
    //
    // If unsupported use the value from GetBclkFreq function
    //
    BclkMcReg = BclkFreq;
  }

  if (BclkFreq != BclkMcReg) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n:: BCLK mismatched: GetBclkFreq():%d != Bclk read from mc reg:%d\n ", BclkFreq, BclkMcReg);
  }

  if (MRC_STATUS_SUCCESS == QclkRatioToDdrFreq (Host, Socket, QclkRatio, &DdrFreqMts)) {
    Dclk = DdrFreqMts / QCLK_PER_DCLK;
    *BandWidthLimitTfData = (BandWidthLimitTfMultiPlier * (Dclk / 8)) & BAND_WIDTH_LIMIT_THROTTLE_MASK;
  } else {
    // Just use the existing value
    GetSetThrottleValues (MemTechDdr, Socket, Channel, NO_DIMM_OPTION, READ_THROTTLE_VALUES, EnumBandWidthThrottleWindowSize, &ThrottleValue);
    *BandWidthLimitTfData = ThrottleValue.BandWidthThrottleWindowSize;
  }
} // GetBwThrottleWindowSize
