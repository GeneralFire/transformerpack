/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
#include <Library/EmulationConfigurationLib.h>
#include <Library/DdrtCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SpdAccessLib.h>
#ifdef DDR5_SUPPORT
#include <Library/Ddr5CoreLib.h>
#endif

/**
  SPD detection for DCPMM dimms. All generations will have byte2 = 0xD

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure
  @param[in] socket - Processor socket within the system (0-based)
  @param[in] ch     - DDR channel number within the processor socket (0-based)
  @param[in] dimm   - DIMM number within a DDR channel (0-based, farthest slot == 0)

  @retval SUCCESS

**/
UINT32
DetectAepDIMMConfig (
  IN  PSYSHOST         Host,
  IN  UINT8            socket,
  IN  UINT8            ch,
  IN  UINT8            dimm
  )
{
  UINT8  SPDReg;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  dimmNvList = GetDimmNvList (Host, socket, ch);
  channelNvList = GetChannelNvList (Host, socket);

  Host->nvram.mem.socket[socket].DcpmmPresent = 1;
  //
  // Get SPD Spec revision
  //
  SpdReadByte (socket, ch, dimm, SPD_REVISION, &SPDReg);
  (*dimmNvList)[dimm].SPDSpecRev = SPDReg;

  //
  // Read SPD Bytes containing DDR module info
  //
  SpdReadByte (socket, ch, dimm, SPD_KEY_BYTE2, &SPDReg);
  (*dimmNvList)[dimm].keyByte2 = SPDReg & 0xF;
  (*dimmNvList)[dimm].actKeyByte2 = SPDReg & 0xF;

  // Take RDIMM path to minimize changes
  (*dimmNvList)[dimm].keyByte2 = SPD_RDIMM;

  // If the SPD image says this NVM DIMM is an RDIMM, then treat it as such
  if ((*dimmNvList)[dimm].actKeyByte2 == SPD_RDIMM) {
    (*dimmNvList)[dimm].lrDimmPresent = 0;
    (*channelNvList)[ch].lrDimmPresent = 0;
    Host->nvram.mem.socket[socket].lrDimmPresent = 0;
  } else {
    (*dimmNvList)[dimm].lrDimmPresent = 1;
    (*channelNvList)[ch].lrDimmPresent = 1;
    Host->nvram.mem.socket[socket].lrDimmPresent = 1;
  }

  if ((UbiosGenerationOrHsleEnabled () != FALSE) &&
      (FeaturePcdGet (PcdCosimBuild) == FALSE)) {

    (*dimmNvList)[dimm].lrDimmPresent = 0;
    (*channelNvList)[ch].lrDimmPresent = 0;
    Host->nvram.mem.socket[socket].lrDimmPresent = 0;
  }

  (*dimmNvList)[dimm].SPDDeviceType = 0;
  (*dimmNvList)[dimm].sdramCapacity = 4;

  return SUCCESS;
}

/**
  Calculate DDRT Dimm Size

  @param[in]  Density     - NVM Die Density with unit MBytes
  @param[in]  DataWidth   - NVM Data Width with unit bits
  @param[in]  DeviceType  - NVM Package Type and Loading
                                   Bit 7    |      Bits 6-4     |               Bits 3-2            |       Bits 1-0
                              Package Type  | Package Die Count | Independent Ports Per NVM Package | DQ Loading Per NVM Port
                              0 = Mono Die  |      000 = 1      |               00 = 1              |    00 = RESERVED
                              1 = Multi Die |      001 = 2      |               01 = 2              |    01 = Multi Load Stack
                                            |      ...          |               1x = RESERVED       |    10 = Single Load Stack
                                            |      110 = 7      |                                   |    11 = RESERVED
                                            |      111 = 8
  @param[in]  ModuleOrg   - Module Organization
                                   Bit 7            |     Bits 6     |        Bits 5-3          |    Bits 2-0
                              NVM Channels per DIMM | Spare NVM/Rank | NVM Active Packages/Rank | NVM Die Width
                                  0 = 1 channel     |      0 = No    |     DDRT    | DDRT2      |  000 = 4 bits
                                  1 = 2 channels    |      1 = Yes   |     000 = 3  000 = 4     |  001 = 8 bits
                                                                     |     001 = 4  001 = 5     |  010 = 16 bits
                                                                     |      ...      ...        |  011 = 32 bits
                                                                     |     110 = 9  110 = 10    |  100 - 111 = RESERVED
                                                                     |     111 = 10 111 = 11    |

  @retval
**/
UINT32
CalculateDdrtDimmSize (
  IN UINT32   Density,
  IN UINT32   DataWidth,
  IN UINT8    DeviceType,
  IN UINT8    ModuleOrg
  )
{
  UINT32      DimmSize;

  //
  // Size = Individual NVM Die Density *
  //        Effective NVM Channel Data Width *
  //        (Package Die Count / Independent Ports Per NVM Package) *
  //        NVM Channels per DIMM
  //
  DimmSize =  Density *
              DataWidth *
              (((DeviceType & (BIT4 | BIT5 | BIT6)) >> 4) + 1) / (((DeviceType & (BIT2 | BIT3)) >> 2) + 1) *
              (((ModuleOrg & BIT7) >> 7) + 1) / (8 * 1024);
  //
  // Add 1 to convert the encoding value to actual number
  // Divide by 8 to convert Bits to Bytes
  // Divide by 1024 to convert MB to GB
  // Density in units of MBytes
  //
  return DimmSize;
}

/**

  Collects data from SPD for DDRT LRDIMM

  @param[in] Host   - Point to sysHost
  @param[in] Socket - CPU socket ID
  @param[in] Ch     - Channel to be accessed
  @param[in] Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
GetLrdimmSpdDataDdrt (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  )
{
#ifdef LRDIMM_SUPPORT
  UINT8  SPDReg;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // LR Data Buffer VrefDQ for DRAM Interface
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DB_VREFDQ, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDbVrefdq = SPDReg + DB_DRAM_VREF_RANGE2_OFFSET;
  //
  // LR Data Buffer MDQ Drive Strength and RTT for data rate <= 1866
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DS_RTT_LE1866, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDbDsRttLe1866 = SPDReg;
  //
  // LR Data Buffer MDQ Drive Strength and RTT for data rate > 1866 and <= 2400
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DS_RTT_GT1866_LE2400, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDbDsRttGt1866Le2400 = SPDReg;
  //
  // LR Data Buffer MDQ Drive Strength and RTT for data rate > 2400 and <= 3200
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DS_RTT_GT2400_LE3200, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDbDsRttGt2400Le3200 = SPDReg;
  //
  // LR Buffer DRAM Drive Strength (for data rates <1866, 1866 < data rate < 2400, and 2400 < data rate < 3200)
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DRAM_DS, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDramDs = SPDReg;
  //
  // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DRAM_ODT_WR_NOM_LE1866, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomLe1866 = SPDReg;
  //
  // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DRAM_ODT_WR_NOM_GT1866_LE2400, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt1866Le2400 = SPDReg;
  //
  // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DRAM_ODT_WR_NOM_GT2400_LE3200, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt2400Le3200 = SPDReg;
  //
  // LR Buffer DRAM ODT (RTT_PARK) for data rate <= 1866
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DRAM_ODT_PARK_LE1866, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDramOdtParkLe1866 = SPDReg;
  //
  // LR Buffer DRAM ODT (RTT_PARK) for data rate > 1866 and <= 2400
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DRAM_ODT_PARK_GT1866_LE2400, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDramOdtParkGt1866Le2400 = SPDReg;
  //
  // LR Buffer DRAM ODT (RTT_PARK) for data rate > 2400 and <= 3200
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LRBUF_DRAM_ODT_PARK_GT2400_LE3200, &SPDReg);
  (*DimmNvList)[Dimm].SPDLrbufDramOdtParkGt2400Le3200 = SPDReg;

#endif  // #ifdef LRDIMM_SUPPORT

  return SUCCESS;
}

