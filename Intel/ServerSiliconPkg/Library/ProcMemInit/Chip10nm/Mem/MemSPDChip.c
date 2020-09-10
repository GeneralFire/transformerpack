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

#include <Library/MemoryCoreLib.h>
#include <SysHostChip.h>
#include <Library/ProcMemInitChipLib.h>
#include "MemHost.h"
#include <Chip/Include/CpuPciAccess.h>
#include <Library/CrcLib.h>
#include <Library/BaseMemoryLib.h>
#include <ScratchpadList.h>
#include <Library/SystemInfoLib.h>
#include <Library/DdrtCoreLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/MemRcLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/KtiApi.h>
#include <Memory/JedecDefinitions.h>
#include <Library/UsraCsrLib.h>
#include <Library/SpdDecodeLib.h>


#define SPD_CRC_DATA_COUNT              126   // Except SPD CRC Data
#define SPD_BASE_CONFIGURATION_BASE     0
#define SPD_BASE_BLOCK1                 128
#define SPD_MANUFACTURING_INFO_BASE     320

STATIC BOOLEAN
IsDDR4CRCValid (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm
  )
{
  UINT8       SPDData[SPD_CRC_DATA_COUNT];
  UINT16      spdCRC;
  UINT16      calculatedCRC;
  UINT8       spdIndex;
  EFI_STATUS  Status;

  ZeroMem ((VOID *) SPDData, SPD_CRC_DATA_COUNT);

  //
  // Read SPD Base configuration and DRAM Parameter Data [0:125]
  //
  for (spdIndex = 0; spdIndex < SPD_CRC_DATA_COUNT; spdIndex++){
    SpdReadByte(socket, ch, dimm, (SPD_BASE_CONFIGURATION_BASE + spdIndex), &SPDData[spdIndex]);
  } // spdIndex

  //
  // Read SPD Base configuration and DRAM Parameter CRC Data [126:127]
  //
  SpdReadByte(socket, ch, dimm, SPD_CRC_LSB, (UINT8*)&spdCRC);
  SpdReadByte(socket, ch, dimm, SPD_CRC_MSB, (UINT8*)&spdCRC + 1);

  Status = CalculateCrc16 ((VOID *) &SPDData, SPD_CRC_DATA_COUNT, &calculatedCRC);

  //RcDebugPrint (SDBG_MAX,
  //              "IsDDR4CRCValid: Base configuration and DRAM Parameter SpdCRC = 0x%x, CalculatedCRC = 0x%x\n", SpdCRC, CalculatedCRC);

  if (EFI_ERROR (Status) || (calculatedCRC != spdCRC)) {
    return FALSE;
  }

  //
  // Get ready for block 1
  //
  ZeroMem ((VOID *) SPDData, SPD_CRC_DATA_COUNT);

  //
  // Read SPD data from block 1
  //
  for (spdIndex = 0; spdIndex < SPD_CRC_DATA_COUNT; spdIndex++) {
    SpdReadByte(socket, ch, dimm, (SPD_BASE_BLOCK1 + spdIndex), &SPDData[spdIndex]);
  } // spdIndex

  //
  // Read SPD Block 1 CRC
  //
  SpdReadByte(socket, ch, dimm, SPD_CRC_LSB_BLOCK1, (UINT8*)&spdCRC);
  SpdReadByte(socket, ch, dimm, SPD_CRC_MSB_BLOCK1, (UINT8*)&spdCRC + 1);

  Status = CalculateCrc16 ((VOID *) &SPDData, SPD_CRC_DATA_COUNT, &calculatedCRC);

   if (EFI_ERROR(Status) || (calculatedCRC != spdCRC)) {
    return FALSE;
  }

  return TRUE;

} // IsDDR4CRCValid


UINT32
DetectSpdTypeDIMMConfig (
                        PSYSHOST         Host,
                        UINT8            socket,
                        UINT8            ch,
                        UINT8            dimm,
                        UINT8            keyByte
                        )
{
  UINT8                  SPDRegTemp;
  UINT8                  SPDRegTemp1;
  UINT8                  SPDRegTemp2;
  UINT32                 Status = 1;
  struct                 dimmNvram (*dimmNvList)[MAX_DIMM];
  struct                 channelNvram (*channelNvList)[MAX_CH];
  SYS_SETUP              *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);
  dimmNvList = GetDimmNvList(Host, socket, ch);

  if (keyByte == SPD_TYPE_DDR4) {

    //
    // Check DDR4 CRC before using SPD Data. Mapout the channel if calculated CRC is not matched with CRC Data offset [126:127]
    //
    if(Setup->mem.optionsExt & SPD_CRC_CHECK) {
      if(!IsDDR4CRCValid(Host, socket, ch, dimm)) {

        RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Error : Invalid DIMM SPD contents!\n");
        OutputError (ERR_DIMM_COMPAT, ERR_INVALID_DDR4_SPD_CONTENT, socket, ch, dimm, NO_RANK);
        DisableChannelSw (Host, socket, ch);

        return Status;
      }
    }

    // for RDIMM
    SpdGetBaseModuleType (socket, ch, dimm, &SPDRegTemp);
    // read for manufacturer register
    SpdReadByte (socket, ch, dimm, SPD_REG_VEN_MSB_DDR4, &SPDRegTemp1);
    // read for register revision number
    SpdReadByte (socket, ch, dimm, SPD_REG_REV_DDR4, &SPDRegTemp2);

    // check RDIMM and TI-A6 register, ignore the dimm and raise an error and move on
    if ((SPDRegTemp == SPD_RDIMM) && (SPDRegTemp1 == (MFGID_TI >> 8)) && (SPDRegTemp2 == TI_SPD_REGREV_A6)) {
      // clear this info for this dimm, as not needed
      (*dimmNvList)[dimm].keyByte = 0x0;
      //
      // clear this info for this dimm, as not needed
      //
      (*dimmNvList)[dimm].dimmPresent = 0;

      //
      // decrement this value for this dimm, as not needed
      //
      (*channelNvList)[ch].maxDimm--;

      RcDebugPrint (SDBG_MAX,
                    "DIMM is NOT supported\n");
      return Status;
    } else {
      Status = DetectDDR4DIMMConfig (Host, socket, ch, dimm);
    }

#ifdef LRDIMM_SUPPORT
    if (IsLrdimmPresent (socket, ch, dimm)) {
      GetLrdimmSpdDataDdr4 (Host, socket, ch, dimm);
    }
#endif  // #ifdef LRDIMM_SUPPORT

  } else if (keyByte == SPD_TYPE_AEP) {
    Host->nvram.mem.DcpmmPresent = 1;
    (*dimmNvList)[dimm].DcpmmPresent = 1;
    Status = DetectAepDIMMConfig (Host, socket, ch, dimm);

    //
    // Collect necessary SPD data for CR DXE/SMM driver even in DDRT channel/dimm be disabled condition
    //
    GatherDdrtSPDData (Host, socket, ch, dimm);

#ifdef LRDIMM_SUPPORT
    if (IsLrdimmPresent (socket, ch, dimm)) {
      GetLrdimmSpdDataDdrt (Host, socket, ch, dimm);
    }
#endif  // #ifdef LRDIMM_SUPPORT

  }
#ifdef DDR5_SUPPORT
  else if (keyByte == SPD_TYPE_DDR5) {

    Status = DetectDDR5DIMMConfig(Host, socket, ch, dimm);

#ifdef LRDIMM_SUPPORT
    if (IsLrdimmPresent (socket, ch, dimm)) {
      GetLrdimmSpdDataDdr5 (Host, socket, ch, dimm);
    }
#endif  // #ifdef LRDIMM_SUPPORT

  }
#endif // DDR5_SUPPORT
  return Status;
}


UINT32
GatherSPDData (
              PSYSHOST Host
              )
/*++

      Calls to gather device specific SPDData

    @param Host  - Point to sysHost

    @retval SUCCESS

  --*/
{

#ifdef DDR5_SUPPORT
  GatherSPDDataDDR5 ();
#else
  GatherSPDDataDDR4 ();
#endif
  return SUCCESS;
}


VOID
WaitForMailboxReady (PSYSHOST Host, UINT8 socket)
{
  //
  // Wait for NVMCTLR to indicate that Mailbox interface is ready.
  //

  if ((FeaturePcdGet (PcdCteBuild) == FALSE) || (FeaturePcdGet (PcdCosimBuild) == TRUE)) {
    FnvPollingBootStatusRegister (Host, socket, FNV_MB_READY);
  }
  return;
}

/**

  Notify all sockets if NVMDIMM is populated on any socket, and check for surprise clock stop

  @param None

  @retval None

**/
VOID
DetectSurpriseClockStop (
  VOID
  )
{
  UINT8                     Socket;
  UINT8                     Ch;
  UINT8                     Dimm;
  BIOS_SCRATCHPAD0_STRUCT   BiosScratchPad0;
#ifdef DDRT_SUPPORT
  BIOS_SCRATCHPAD7_STRUCT   BiosScratchPad7;
  UINT8                     TargetSocket;
#endif // DDRT_SUPPORT
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;
  BOOLEAN                   SurpriseClockStop = FALSE;
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  struct dimmNvram          (*DimmNvList)[MAX_DIMM];
  SYS_SETUP                 *Setup;
  PSYSHOST                  Host = NULL;
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer();

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

#ifdef DDRT_SUPPORT
  //
  // Check whether DDRT is in the system when in non-warm-boot modes, and take action if it is.
  //
  if (Host->var.mem.subBootMode != WarmBoot && Host->var.mem.subBootMode != WarmBootFast) {
    if (Host->nvram.mem.DcpmmPresent == 1) {
      for (TargetSocket = 0; TargetSocket < MAX_SOCKET; ++TargetSocket) {
        if (!(GetSocketPresentBitMap () & (1 << TargetSocket))) {
          continue;
        }
        BiosScratchPad7.Data = UsraCsrRead (TargetSocket, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG);
 
        if (!(BiosScratchPad7.Bits.AepDimmPresent)) {
          BiosScratchPad7.Bits.AepDimmPresent = 1;
          UsraCsrWrite (TargetSocket, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG, BiosScratchPad7.Data);
        }
 
      } // TargetSocket loop
    } // DcpmmPresent
  }  // if (Host->var.mem.subBootMode != WarmBoot && Host->var.mem.subBootMode != WarmBootFast)
#endif // DDRT_SUPPORT


  for (Socket = 0; Socket < MAX_SOCKET; ++Socket) {

    if (Host->nvram.mem.socket[Socket].enabled == 0) {
      continue;
    }

    ChannelNvList = GetChannelNvList(Host, Socket);

    //
    // NVMDIMM surprise clock stop detection.
    //
    if (Host->nvram.mem.DcpmmPresent == 1) {

      BiosScratchPad0.Data = UsraCsrRead (Socket, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
      if (BiosScratchPad0.Bits.TrainingMode == 1) {
        SurpriseClockStop = TRUE;
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Surprise warm reset happened when iMC or PMem in training mode\n");
        break;
      }
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Bsr = 0x%x\n", Bsr);
        if ((Bsr.Data != 0xffffffff) && ((Bsr.Data & BIT29) == BIT29)) {
          SurpriseClockStop = TRUE;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Bsr = 0x%x, PMem receive surprise clock stop\n", Bsr);
        }
      }
    }

    if (SurpriseClockStop == TRUE) {
      if (Setup->mem.NvmdimmPowerCyclePolicy != 1) {
        RcDebugPrint (SDBG_MINMAX, "Please generate a power cycle to recover\n");
        OutputWarning (WARN_NVDIMM_SURPRISE_CLOCK_STOP, WARN_POWER_CYCLE_POLICY_NOT_APPLY, Socket, NO_CH, NO_DIMM, NO_RANK);
      } else {
        RcDebugPrint (SDBG_MINMAX, "Power cycle policy is enabled, issue power cycle\n");
        OutputWarning (WARN_NVDIMM_SURPRISE_CLOCK_STOP, WARN_POWER_CYCLE_POLICY_APPLY, Socket, NO_CH, NO_DIMM, NO_RANK);
        SetSysResetRequired (GetSysResetRequired() | POST_RESET_POWERGOOD);
      }
    }
  } // Socket loop
} // DetectSurpriseClockStop
