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
#include "SysHost.h"
#include <Memory/Ddr5MrRegs.h>
#include <Memory/Ddr5SpdRegisters.h>
#include <Memory/I3cDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/SpdDecodeLib.h>
#include <Memory/JedecDefinitions.h>
#include <MemRegs.h>

VOID   GetDimmTimeBaseDdr5(PSYSHOST Host, UINT8 Spdmtb, UINT8 Spdftb, INT32 * Mtb, INT32 * Ftb);

//
// Supported DRAM tecnologies (SDRAM addressing)
//
//                                               |          8Gb           |               16Gb          |              24Gb           |               32Gb           |             64Gb            |
const   UINT8   SdramCapacityDdr5[MAX_TECH]   = { SPD_8Gb, SPD_8Gb, SPD_8Gb, SPD_16Gb, SPD_16Gb, SPD_16Gb, SPD_24Gb, SPD_24Gb, SPD_24Gb, SPD_32Gb, SPD_32Gb, SPD_32Gb, SPD_64Gb, SPD_64Gb, SPD_64Gb};   // Primary SDRAM density
const   UINT8   PrimaryWidthDdr5[MAX_TECH]    = {       4,       8,      16,        4,        8,       16,        4,        8,       16,        4,        8,       16,        4,        8,       16};   // Primary SDRAM device width
const   UINT8   RowBitsDdr5[MAX_TECH]         = {      16,      16,      16,       16,       16,       16,       17,       17,       17,       17,       17,       17,       18,       18,       18};   // Number of row address bits
const   UINT8   ColumnBitsDdr5[MAX_TECH]      = {      11,      10,      10,       11,       10,       10,       11,       10,       10,       11,       10,       10,       11,       10,       10};   // Number of column address bits
const   UINT8   BankGroupBitsDdr5[MAX_TECH]   = {       3,       3,       2,        3,        3,        2,        3,        3,        2,        3,        3,        2,        3,        3,        2};   // Number of bank group bits
const   UINT8   BankAddressBitsDdr5[MAX_TECH] = {       1,       1,       1,        2,        2,        2,        2,        2,        2,        2,        2,        2,        2,        2,        2};   // Number of bank address bits

/**

  Apply workaround for Montage SPD Hub A0 Errata(Integrated Thermal Sensor)

  @param[in] Socket  - Socket to initizlize
  @param[in] Channel - Channel on the socket
  @param[in] Dimm    - Dimm on the channel

  @retval NA

**/
VOID
Ddr5MontageSpdHubA0Errata (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
);

/**
  Store the common DDR5 DIMM configuration in the Host structure

  @param[in,out] Host   - Pointer to sysHost
  @param[in]     Socket - CPU Socket ID
  @param[in]     Ch     - Channel to be accessed
  @param[in]     Dimm   - DIMM to be accessed

  @retval N/A

**/
VOID
GetCommonDDR5DIMMConfig (
  IN OUT PSYSHOST Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm
  )
{
  UINT8             SPDReg;
  UINT8             OperableEndurant;
  EFI_STATUS        Status;
  struct dimmDevice (*ChdimmList)[MAX_DIMM];
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChdimmList = GetChDimmList (Host, Socket, Ch);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Get Min TCK
  //
  Status = SpdGetMiniCycleTime (Host, Socket, Ch, Dimm, &((*DimmNvList)[Dimm].minTCK));
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_MAX, "Read Min Tck Time failed.\n");
  }
  (*ChdimmList)[Dimm].minTCK = (*DimmNvList)[Dimm].minTCK;

  OperableEndurant = SPD_VDD_110;
  //
  // Get Vdd DIMM Support Info (Only support 1.10v for DDR5)
  //
  SpdGetOperableEndurant (Socket, Ch, Dimm, &OperableEndurant);
  if (OperableEndurant != SPD_VDD_110) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\n ERROR: DIMM does not support 1.10v\n");
    (*ChannelNvList)[Ch].v110NotSupported = 1;
  }

  //
  // Only 1.10v is supported for DDR5 ddrVoltage
  //
  Host->nvram.mem.socket[Socket].ddrVoltage = SPD_VDD_110;

  //
  // Get Raw Card Number
  //
  SpdGetRawCardRefDesign (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDRawCard);
  SpdGetRawCardRefDesignRev (Socket, Ch, Dimm, &SPDReg);
  (*DimmNvList)[Dimm].SPDRawCard |= SPDReg << 5;

  //
  // Get Bus Width
  //
  SpdGetPrimaryBusWidth (Socket, Ch, Dimm, & (*DimmNvList)[Dimm].SPDMemBusWidth);
  SpdGetBusWidthExtension (Socket, Ch, Dimm, &SPDReg);
  (*DimmNvList)[Dimm].SPDMemBusWidth |= SPDReg << 3;
  SpdGetChannelNumPerDimm (Socket, Ch, Dimm, &SPDReg);
  (*DimmNvList)[Dimm].SPDMemBusWidth |= SPDReg << 5;

  //
  // Registering Clock Driver (RCD) Manufacturer ID Code
  //
  SpdGetRcdVendor (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDRegVen);

  //
  // Registering Clock Driver (RCD) Device Revision
  //
  SpdGetRcdRevision (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDRegRev);

} // GetCommonDDR5DIMMConfig

/**

  Detect DDR5 configuration

  @param Host   - Point to sysHost
  @param Socket - CPU Socket ID
  @param Ch     - channel to be accessed
  @param Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
DetectDDR5DIMMConfig (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  UINT8               NumColBits;
  UINT8               NumColBitsSecondary;
  UINT8               NumRowBits;
  UINT8               NumRowBitsSecondary;
  UINT8               NumBankGroupBits;
  UINT8               NumBankGroupBitsSecondary;
  UINT8               NumBankAddressBits;
  UINT8               NumBankAddressBitsSecondary;
  UINT8               SdramCapacity;
  UINT8               SdramCapacitySecondary;
  UINT8               BaseModuleType = 0;
  UINT8               PackageRanks = 0;
  UINT8               DramIOWidth;
  UINT8               DramIOWidthSecondary;
  BOOLEAN             RankMix;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];

  DimmNvList    = GetDimmNvList(Host, Socket, Ch);
  ChannelNvList = GetChannelNvList(Host, Socket);

  Ddr5MontageSpdHubA0Errata (Socket, Ch, Dimm);

  GetCommonDDR5DIMMConfig (Host, Socket, Ch, Dimm);

  //
  // Read SPD Bytes containing DDR module info
  //
  SpdGetBaseModuleType (Socket, Ch, Dimm, &BaseModuleType);
  (*DimmNvList)[Dimm].keyByte2    = BaseModuleType;
  (*DimmNvList)[Dimm].actKeyByte2 = BaseModuleType;

#ifdef LRDIMM_SUPPORT
  if (BaseModuleType == SPD_LRDIMM_DDR5) {
    (*DimmNvList)[Dimm].lrDimmPresent   = 1;
    (*ChannelNvList)[Ch].lrDimmPresent  = 1;
    Host->nvram.mem.socket[Socket].lrDimmPresent = 1;

    // Take RDIMM path to minimize changes
    (*DimmNvList)[Dimm].keyByte2 = SPD_RDIMM;
    (*DimmNvList)[Dimm].actKeyByte2 = SPD_LRDIMM_DDR5;
  }
#endif

#if FixedPcdGetBool(PcdMcrSupport)
  if (BaseModuleType == SPD_MCRDIMM_DDR5) {
    (*DimmNvList)[Dimm].McrDimmPresent   = 1;
    (*ChannelNvList)[Ch].McrDimmPresent  = 1;
    Host->nvram.mem.socket[Socket].McrDimmPresent = 1;

    (*DimmNvList)[Dimm].lrDimmPresent   = 1;
    (*ChannelNvList)[Ch].lrDimmPresent  = 1;
    Host->nvram.mem.socket[Socket].lrDimmPresent = 1;

    // Take RDIMM path to minimize changes
    (*DimmNvList)[Dimm].keyByte2 = SPD_RDIMM;
    (*DimmNvList)[Dimm].actKeyByte2 = SPD_MCRDIMM_DDR5;
  }
#endif // FixedPcdGetBool(PcdMcrSupport)

  //
  // Get SDRAM Capacity
  //
  SpdGetEncodedDramCapacity (Socket, Ch, Dimm, PrimarySdramNumber, &SdramCapacity);
  (*DimmNvList)[Dimm].sdramCapacity = SdramCapacity;

  //
  // Get the number of bank address bits
  //
  SpdGetBankAddressBits (Socket, Ch, Dimm, PrimarySdramNumber, &NumBankAddressBits);
  (*DimmNvList)[Dimm].numBankAddressBits = NumBankAddressBits;
  //
  // Get the number of bank group bits
  //
  SpdGetBankGroupBits (Socket, Ch, Dimm, PrimarySdramNumber, &NumBankGroupBits);
  (*DimmNvList)[Dimm].numBankGroupBits = NumBankGroupBits;

  //
  // Get number of row bits, which works for SDRAM in symmetrical DIMMs and also the primary SDRAM in asymmetrical DIMMs
  //
  SpdGetNumRowBits (Socket, Ch, Dimm, PrimarySdramNumber, &NumRowBits);
  (*DimmNvList)[Dimm].numRowBits = NumRowBits;

  //
  // Get number of column bits
  //
  SpdGetNumColBits (Socket, Ch, Dimm, PrimarySdramNumber, &NumColBits);
  (*DimmNvList)[Dimm].numColBits = NumColBits;

  //
  // Get the encoded value of dram IO width
  //
  SpdGetEncodedDramIOWidth (Socket, Ch, Dimm, PrimarySdramNumber, &DramIOWidth);
  (*DimmNvList)[Dimm].dramIOWidth = DramIOWidth;

  //
  // Get Number of ranks and device width
  // SPDModuleOrg and actSPDModuleOrg apply format from SPD register spec
  // Bit3 - Bit5: Number of Package Ranks per DIMM
  //
  (*DimmNvList)[Dimm].SPDModuleOrg = DramIOWidth;
  SpdGetPackageRanks (Socket, Ch, Dimm, &PackageRanks);
  (*DimmNvList)[Dimm].SPDModuleOrg |= PackageRanks << 3;
  (*DimmNvList)[Dimm].actSPDModuleOrg = (*DimmNvList)[Dimm].SPDModuleOrg;

  //
  // Get Primary SDRAM Device Type and Die Count, which works for SDRAM in symmetrical DIMMs and also the primary SDRAM in asymmetrical DIMMs
  //
  SpdGetDramDeviceType (Socket, Ch, Dimm, PrimarySdramNumber, &(*DimmNvList)[Dimm].SPDDeviceType);
  SpdGetDramDieCount (Socket, Ch, Dimm, PrimarySdramNumber, &(*DimmNvList)[Dimm].dieCount);

  //
  // Get rank mix indicator of DIMM
  //
  SpdGetRankMixIndicator (Socket, Ch, Dimm, &RankMix);
  (*DimmNvList)[Dimm].rankMix = RankMix;

  if ((*DimmNvList)[Dimm].rankMix) {  // If the DIMM is asymmetric, the Secondary SDRAM configuration need to be read and detected
    //
    // Get Secondary SDRAM Device Type and Die Count
    //
    SpdGetDramDeviceType (Socket, Ch, Dimm, SecondarySdramNumber, &(*DimmNvList)[Dimm].SPDSecondaryDeviceType);
    SpdGetDramDieCount (Socket, Ch, Dimm, SecondarySdramNumber, &(*DimmNvList)[Dimm].dieCountSecondary);

    //
    // Get number of column bits for Secondary SDRAM
    //
    SpdGetNumColBits (Socket, Ch, Dimm, SecondarySdramNumber, &NumColBitsSecondary);
    (*DimmNvList)[Dimm].numColBitsSecondary = NumColBitsSecondary;

    //
    // Get number of row bits for Secondary SDRAM
    //
    SpdGetNumRowBits (Socket, Ch, Dimm, SecondarySdramNumber, &NumRowBitsSecondary);
    (*DimmNvList)[Dimm].numRowBitsSecondary = NumRowBitsSecondary;

    //
    // Get the number of bank group bits for Secondary SDRAM
    //
    SpdGetBankGroupBits (Socket, Ch, Dimm, SecondarySdramNumber, &NumBankGroupBitsSecondary);
    (*DimmNvList)[Dimm].numBankGroupBitsSecondary = NumBankGroupBitsSecondary;

    //
    // Get the number of bank address bits for Secondary SDRAM
    //
    SpdGetBankAddressBits (Socket, Ch, Dimm, SecondarySdramNumber, &NumBankAddressBitsSecondary);
    (*DimmNvList)[Dimm].numBankAddressBitsSecondary = NumBankAddressBitsSecondary;

    //
    // Get SDRAM Capacity for Secondary SDRAM
    //
    SpdGetEncodedDramCapacity (Socket, Ch, Dimm, SecondarySdramNumber, &SdramCapacitySecondary);
    (*DimmNvList)[Dimm].sdramCapacitySecondary = SdramCapacitySecondary;

    // Get the encoded value of IO width for Secondary SDRAM
    //
    SpdGetEncodedDramIOWidth (Socket, Ch, Dimm, SecondarySdramNumber, &DramIOWidthSecondary);
    (*DimmNvList)[Dimm].dramIOWidthSecondary = DramIOWidthSecondary;
  }

  return SUCCESS;
} // DetectDDR5DIMMConfig


/**

  Collects data from SPD for DDR5 LRDIMM

  @param[in] Host   - Point to sysHost
  @param[in] Socket - CPU socket ID
  @param[in] Ch     - Channel to be accessed
  @param[in] Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
GetLrdimmSpdDataDdr5 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  )
{
#ifdef LRDIMM_SUPPORT

  //
  // There is no SPD bytes for DRAM Vref / DB Vref / DB Drive strength / DRAM ODT field in DDR5 SPD Spec
  //

#endif  // #ifdef LRDIMM_SUPPORT

  return SUCCESS;
}


/**

  Collects data from SPD for DDR5

  @retval SUCCESS

**/
UINT32
GatherSPDDataDDR5 (
  VOID
  )
{
  PSYSHOST            Host;
  UINT8               Socket;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               SPDReg;
  UINT16              SpdRegUint16;
#ifdef DEBUG_CODE_BLOCK
  UINT8               i;
#endif //DEBUG_CODE_BLOCK
  UINT16              Cycles;
  UINT8               PageSize = 0;
  UINT8               Ti;
  UINT64              CasSup = 0;
  UINT16              TRC;
  UINT16              TRAS;
  UINT16              tRFC;
  UINT16              nRFC;
  UINT8               TFAW;
  UINT8               TWTRL;
  UINT8               TWTR;
  UINT8               TWR;
  UINT8               TRTP;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
#ifdef DEBUG_CODE_BLOCK
  struct dimmDevice   (*ChdimmList)[MAX_DIMM];
#endif  // DEBUG_CODE_BLOCK
  UINT8               MaxChDdr;
  REFRESH_MODE        RefreshMode;

  MaxChDdr = GetMaxChDdr ();
  Host = (PSYSHOST) GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  //
  // Return if this Socket is disabled
  //

  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  //
  // Switch To I3C native Mode for SPR
  //
  I3cSwitchToI3cMode (Socket);

  //
  // Don't access the SPD if not necessary
  //
  if ((Host->nvram.mem.DataGood != 1) || ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot))) {

    //
    // Only gather SPD info if we don't already have it
    //
    ChannelNvList = GetChannelNvList (Host, Socket);

    // Check strap addresses on each channel moving near to far
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
#ifdef DEBUG_CODE_BLOCK
      ChdimmList  = &Host->var.mem.socket[Socket].channelList[Ch].dimmList;
#endif  // DEBUG_CODE_BLOCK

      //
      // Initialize to all CAS Latencies supported. DDR5 only use 5 bytes
      //
      (*ChannelNvList)[Ch].common.casSup = MAX_UINT64;

      //
      // Initialize to ASR supported. This bit will get cleared if any DIMM on the channel does not support ASR
      //
      (*ChannelNvList)[Ch].features |= ASR_SUPPORT;

      (*ChannelNvList)[Ch].tCCDAdder = 0;

      //
      // Get tRRD_S and tRRD_L
      //
      (*ChannelNvList)[Ch].common.nRRD = (UINT8)GettRRD (Host, Socket, Ch);
      (*ChannelNvList)[Ch].common.nRRD_L = (UINT8)GettRRD_L (Host, Socket, Ch);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Get SPD Spec revision
        //
        SpdGetSpecRevision (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDSpecRev);

        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          //
          // Lookup geometry in DIMM technology table to determine page size

          Ti = GetTechIndexDdr5 (Host, Socket, Ch, Dimm);

          switch (Ti) {
          case 2:
          case 5:
          case 8:
          case 11:
          case 14:
            PageSize = PAGE_SIZE_2K;    // 2K Page Size
            break;

          default:
            PageSize = PAGE_SIZE_1K;    // 1K Page Size
            break;
          }

          TFAW = (UINT8)GettFAW (Host, Socket, Ch, PageSize);

          if ((*ChannelNvList)[Ch].common.nFAW < TFAW) {
            (*ChannelNvList)[Ch].common.nFAW = TFAW;
          }

          //
          // Use lookup table for tWTR_L
          //
          TWTRL = (UINT8)GettWTR_L (Host, Socket, Ch);

          if ((*ChannelNvList)[Ch].common.nWTR_L < TWTRL) {
            (*ChannelNvList)[Ch].common.nWTR_L = TWTRL;
          }

          //
          // Use lookup table for tWTR_S
          //
          TWTR = (UINT8)GettWTR_S (Host, Socket, Ch);

          if ((*ChannelNvList)[Ch].common.nWTR < TWTR) {
            (*ChannelNvList)[Ch].common.nWTR = TWTR;
          }

          //
          // Use lookup table for TWR
          //
          TWR = (UINT8)GettWR (Host, Socket, Ch);

          if ((*ChannelNvList)[Ch].common.nWR < TWR) {
            (*ChannelNvList)[Ch].common.nWR = TWR;
          }

          //
          // Use lookup table for TRTP
          //
          TRTP = (UINT8)GettRTP (Host, Socket, Ch);

          if ((*ChannelNvList)[Ch].common.nRTP < TRTP) {
            (*ChannelNvList)[Ch].common.nRTP = TRTP;
          }

          //
          // Get tRP
          //
          SpdGetTrp (Socket, Ch, Dimm, &SpdRegUint16);
#ifdef  DEBUG_CODE_BLOCK
          (*ChdimmList)[Dimm].tRP = SpdRegUint16;
#endif  // DEBUG_CODE_BLOCK
          (*ChannelNvList)[Ch].dimmList[Dimm].tRP = SpdRegUint16;
          (*ChannelNvList)[Ch].dimmList[Dimm].ftbTRP = 0;

          if ((*ChannelNvList)[Ch].common.tRP < (*ChannelNvList)[Ch].dimmList[Dimm].tRP) {
            (*ChannelNvList)[Ch].common.tRP = (*ChannelNvList)[Ch].dimmList[Dimm].tRP;

            // Convert to number of Cycles
            Cycles = PicosecondToClocks (Socket, (*ChannelNvList)[Ch].dimmList[Dimm].tRP);

            if ((*ChannelNvList)[Ch].common.nRP < Cycles) {
              (*ChannelNvList)[Ch].common.nRP = (UINT8)Cycles;
            }
          }

          //
          // Get TRAS
          //
          SpdGetTras (Socket, Ch, Dimm, &TRAS);
          if ((*ChannelNvList)[Ch].common.tRAS < TRAS) {
            (*ChannelNvList)[Ch].common.tRAS = TRAS;

            // Convert to number of Cycles
            Cycles = PicosecondToClocks (Socket, TRAS);
            if ((*ChannelNvList)[Ch].common.nRAS < Cycles) {
              (*ChannelNvList)[Ch].common.nRAS = (UINT8)Cycles;
            }
          }

          //
          // Get TRC
          //
          SpdGetTrc (Socket, Ch, Dimm, &TRC);
          if ((*ChannelNvList)[Ch].common.tRC < TRC) {
            (*ChannelNvList)[Ch].common.tRC = TRC;

            // Convert to number of Cycles
            Cycles = PicosecondToClocks (Socket, TRC);
            if ((*ChannelNvList)[Ch].common.nRC < Cycles) {
              (*ChannelNvList)[Ch].common.nRC = (UINT8)Cycles;
            }
          }

          //
          // Get tRCD
          //
          SpdGetTrcd (Socket, Ch, Dimm, &SpdRegUint16);
#ifdef  DEBUG_CODE_BLOCK
          (*ChdimmList)[Dimm].tRCD = SpdRegUint16;
#endif  // DEBUG_CODE_BLOCK
          (*ChannelNvList)[Ch].dimmList[Dimm].tRCD = SpdRegUint16;
          (*ChannelNvList)[Ch].dimmList[Dimm].ftbTRCD = 0;

          if ((*ChannelNvList)[Ch].common.tRCD < (*ChannelNvList)[Ch].dimmList[Dimm].tRCD) {
            (*ChannelNvList)[Ch].common.tRCD = (*ChannelNvList)[Ch].dimmList[Dimm].tRCD;

            // Convert to number of Cycles
            Cycles = PicosecondToClocks (Socket, (*ChannelNvList)[Ch].common.tRCD);

            if ((*ChannelNvList)[Ch].common.nRCD < Cycles) {
              (*ChannelNvList)[Ch].common.nRCD = (UINT8)Cycles;
            }
          }

          //
          // Get tRFC
          //
          RefreshMode = GetRefreshModePolicy ();
          if (GetTrfcMcOverride (Socket, Ch, Dimm, RefreshMode, &tRFC, &nRFC) != MRC_STATUS_SUCCESS ) {
            SpdGetTrfc (Socket, Ch, Dimm, RefreshMode, &tRFC, &nRFC);
          }
          //
          // Update Host with largest tRFC/nRFC in the channel
          //
          if ((*ChannelNvList)[Ch].common.tRFC < tRFC) {
            (*ChannelNvList)[Ch].common.tRFC = tRFC;

            if ((*ChannelNvList)[Ch].common.nRFC < nRFC) {
              (*ChannelNvList)[Ch].common.nRFC = nRFC;
            }
          }

          //
          // Get tAA
          //
          SpdGetTaa (Socket, Ch, Dimm, &SpdRegUint16);
          (*ChannelNvList)[Ch].dimmList[Dimm].tCL = SpdRegUint16;
#ifdef  DEBUG_CODE_BLOCK
          (*ChdimmList)[Dimm].tCL = (*ChannelNvList)[Ch].dimmList[Dimm].tCL ;
#endif  // DEBUG_CODE_BLOCK

          if ((*ChannelNvList)[Ch].common.tCL < (*ChannelNvList)[Ch].dimmList[Dimm].tCL) {
            (*ChannelNvList)[Ch].common.tCL = (*ChannelNvList)[Ch].dimmList[Dimm].tCL;

            Cycles = PicosecondToClocks (Socket, (*ChannelNvList)[Ch].common.tCL);

            if ((*ChannelNvList)[Ch].common.nCL < Cycles) {
              (*ChannelNvList)[Ch].common.nCL = (UINT8)Cycles;
            }
          }
          //
          // Get CasSup
          //
          SpdGetCasSupported (Socket, Ch, Dimm, &CasSup);

          //
          // Mask off the unsupported CLs
          //
          (*ChannelNvList)[Ch].common.casSup &= CasSup;

          //
          // Get DIMM thermal sensor number
          //
          (*DimmNvList)[Dimm].dimmTs = 0;
          if (SpdIsThermalSensorInstalled (Socket, Ch, Dimm, 1)) {
            (*DimmNvList)[Dimm].dimmTs |= BIT6; // TS1 is installed
          }
          if (SpdIsThermalSensorInstalled (Socket, Ch, Dimm, 0)) {
            (*DimmNvList)[Dimm].dimmTs |= BIT7; // TS0 is installed
          }

          //
          // Get SDRAM optional features
          //
          (*DimmNvList)[Dimm].SPDOptionalFeature = 0; // TODO 1507230186

          //if (((*DimmNvList)[Dimm].SPDOptionalFeature & SPD_TRR_IMMUNE) == 0){
          //  rankNvList = GetRankNvList(Host, Socket, Ch, Dimm);
          //  for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          //    if ((*rankNvList)[rank].enabled == 0) continue;
          //    DisableRank(Host, Socket, Ch, Dimm, rank);
          //    OutputWarning (WARN_MEMTEST_DIMM_DISABLE, WARN_FPT_MINOR_MEM_TEST, Socket, Ch, Dimm, rank);
          //  }
          //}

          (*DimmNvList)[Dimm].SPDOtherOptFeatures = 0;
          if (SpdIsPprSupported (Socket, Ch, Dimm, PprOneRowPerBankGroup)) {
            (*DimmNvList)[Dimm].SPDOtherOptFeatures |= BIT6;
          }
          if (SpdIsPprSupported (Socket, Ch, Dimm, PprSoft)) {
            (*DimmNvList)[Dimm].SPDOtherOptFeatures |= BIT5;
          }

        }  // if ((*DimmNvList)[Dimm].DcpmmPresent == 0)

        //
        // Common part for DDR5 and DDRT
        //

        //
        // Get module mfg location from SPD 514
        //
        SpdGetModuleManufacturingLocation (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDMMfLoc);

        //
        // Get module date code from SPD 515-516.
        //
        SpdGetModuleManufacturingDate (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDModDate);

        //
        // Get module Serial Number from SPD 517-520.
        //
        SpdGetModuleSerialNumber (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDModSN[0]);

        //
        // Get module mfg id from SPD 512-513.
        //
        SpdGetModuleManufacturerId (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDMMfgId);

        //
        // Get module part number from SPD 521-550.
        //
        SpdGetModulePartNum (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDModPartDDR5[0]);

#ifdef  DEBUG_CODE_BLOCK
        //
        // Get Intel DIMM serial number from SPD 1019-1023
        //
        for (i = 0; i < 5; i++) {
          SpdReadByte (Socket, Ch, Dimm, SPD_INTEL_DIMM_SERIAL_NUMBER + i, &SPDReg);
          (*DimmNvList)[Dimm].SPDIntelSN[i] = SPDReg;
        }
#endif // DEBUG_CODE_BLOCK

        //
        // Get DRAM mfg id from SPD 552-553.
        //
        SpdGetDramManufacturerId (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDDramMfgId);

        //
        // Get DRAM revision id from SPD 554.
        //
        SpdGetDramStepping (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDDramRev);

        //
        // Get SPD Spec revision
        //
        SpdGetSpecRevision (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].SPDSpecRev);

        //
        // RDIMM specific bytes
        //
        if (Host->nvram.mem.dimmTypePresent == RDIMM) {

          //
          // Register Output Drive Strength for Control
          //
          (*DimmNvList)[Dimm].SPDODCtl = 0; // TODO 1507230186

          //
          // Register Output Drive Strength for Clock
          //
          (*DimmNvList)[Dimm].SPDODCk = 0; // TODO 1507230186

          //
          // Get RDIMM/LRDIMM module attributes
          // Initialize dimmRevType to 1
          //
          (*DimmNvList)[Dimm].DimmRevType = RCD_REV_1;
          (*ChannelNvList)[Ch].dimmRevType = RCD_REV_1;

          // RCD Revision
          SpdReadByte (Socket, Ch, Dimm, SPD_RDIMM_REGISTERING_CLOCK_DRIVER_RCD_DEVICE_TYPE_REG, &SPDReg);
          (*DimmNvList)[Dimm].lrbufGen = (SPDReg & 0xF);
          // Device Type 0000: DDR5RCD01 (see JESD82-511) All other codes reserved
          if ((*DimmNvList)[Dimm].lrbufGen != 0) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                    "SPD ERROR: Unsupported RCD_DEVICE_TYPE 0x%x", (*DimmNvList)[Dimm].lrbufGen);
            (*DimmNvList)[Dimm].lrbufGen = 0;
          }

#ifdef  LRDIMM_SUPPORT
          //
          // LRDIMM specific bytes
          //
          if (IsLrdimmPresent (Socket, Ch, Dimm)) {

            //
            // Second Byte of the data buffer ID
            //
            SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_1_REG, &SPDReg);
            (*DimmNvList)[Dimm].SPDLrbufVen = (UINT16) (SPDReg << 8);

            //
            // First Byte of the data buffer ID
            //
            SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_DBS_DB_MANUFACTURER_ID_CODE_0_REG, &SPDReg);
            (*DimmNvList)[Dimm].SPDLrbufVen |= (UINT16) (SPDReg & ~BIT7);

            //
            // Data Buffer Revision
            //
            SpdReadByte (Socket, Ch, Dimm, SPD_LRDIMM_DBS_DB_DEVICE_REVISION_REG, &SPDReg);
            (*DimmNvList)[Dimm].lrbufRid = SPDReg;

            //
            // Get LRDIMM module attributes
            //
            if (!(*DimmNvList)[Dimm].DcpmmPresent) {
              SpdGetRcdDeviceType (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].lrbufGen);
            }
          }
#endif
          // SPD bytes for RDIMM module
        }

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();

          //
          // Module part number
          //
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "DIMM Module Part Number: ");
          for (i = 0; i < SPD_MODULE_PART_DDR5; i++) {
            if ((*DimmNvList)[Dimm].SPDModPartDDR5[i] >= 32 && (*DimmNvList)[Dimm].SPDModPartDDR5[i] < 127) {
              RcDebugPrint (SDBG_DEFAULT, "%c", (*DimmNvList)[Dimm].SPDModPartDDR5[i]);
            } else {
              RcDebugPrint (SDBG_DEFAULT, "(%d)", (*DimmNvList)[Dimm].SPDModPartDDR5[i]);
            }
          }

          // New line
          RcDebugPrint (SDBG_DEFAULT, "\n");
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK

        // Get Module Nominal Height Byte 230 for VLP RDIMM
        if ((IsLrdimmPresent (Socket, Ch, Dimm) == 0) && (Host->nvram.mem.dimmTypePresent == RDIMM)) {
          SpdGetNominalHeight (Socket, Ch, Dimm, &SPDReg);
          if (SPDReg <= 0x4) {
            (*DimmNvList)[Dimm].VlpRdimmPresent = 1;
          }
        }

#if defined(DEBUG_CODE_BLOCK) && defined(DDRT_SUPPORT)
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          //prints for Buffer Gen, FM Controller, and Stepping to ease debugging
          RcDebugPrint (SDBG_MAX, "Revision Code (Stepping) = 0x%x\n", (*DimmNvList)[Dimm].fmcRev);
          RcDebugPrint (SDBG_MAX, "FMC Type: ");
          switch ((*DimmNvList)[Dimm].fmcType) {
          case FMC_CWV_TYPE:
            RcDebugPrint (SDBG_MAX, "CWV\n");
            break;
          default:
            RcDebugPrint (SDBG_MAX, "0x%x\n", (*DimmNvList)[Dimm].fmcType);
          }
          RcDebugPrint (SDBG_MAX, "Gen Buffer: Gen %d\n", ((*DimmNvList)[Dimm].lrbufGen) + 1);
          RcDebugPrint (SDBG_MAX, "Buffer Vendor: ");
          if (GetDebugLevel () & SDBG_MAX) {
            DisplayManf (Host, (*DimmNvList)[Dimm].SPDRegVen);
          }
          RcDebugPrint (SDBG_MAX, "\n");
        }
#endif // DEBUG_CODE_BLOCK && DDRT_DUPPORT
      } // Dimm loop
    } // Ch loop
  }

  return SUCCESS;
} // GatherSPDDataDDR5

/**

  Lookup DDR5 rank in the technology table

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket to initizlize
  @param[in] Ch      - Channel on the socket
  @param[in] Dimm    - Dimm on the channel

  @retval Tech table index

**/
UINT8
GetTechIndexDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
)
{
  UINT8               Ti = 0;
  struct dimmNvram(*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP) {
    Ti = 0;
  } else if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR5) {

    //
    // DDR5
    //
    for (Ti = 0; Ti < MAX_TECH_DDR5; Ti++) {

      if ((PrimaryWidthDdr5[Ti] == (4 << (*DimmNvList)[Dimm].dramIOWidth)) &&
          (SdramCapacityDdr5[Ti] == (*DimmNvList)[Dimm].sdramCapacity) &&
          (RowBitsDdr5[Ti] == (*DimmNvList)[Dimm].numRowBits) &&
          (ColumnBitsDdr5[Ti] == (*DimmNvList)[Dimm].numColBits) &&
          (BankGroupBitsDdr5[Ti] == (*DimmNvList)[Dimm].numBankGroupBits) &&
          (BankAddressBitsDdr5[Ti] == (*DimmNvList)[Dimm].numBankAddressBits)
          ) {
        break;
      }
    } // Ti loop

    if (Ti >= MAX_TECH_DDR5) {
      Ti = 0xFF;
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SDRAM Capacity = %d\n", EncodedSpdToDramDensity ((*DimmNvList)[Dimm].sdramCapacity));
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Device Width = x%d\n", (4 << (*DimmNvList)[Dimm].dramIOWidth));
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Number of Row = %d\n", (*DimmNvList)[Dimm].numRowBits);
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Number of Column = %d\n", (*DimmNvList)[Dimm].numColBits);
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Number of Bank Group = %d\n", (*DimmNvList)[Dimm].numBankGroupBits);
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Number of Bank Address = %d\n", (*DimmNvList)[Dimm].numBankAddressBits);
    }

  }

  return Ti;
} // GetTechIndexDdr5


/**

  Calculate package rank size (in 64MB unit) for asymmetric or symmeteric DIMM gived the target package rank number.

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] Dimm              - Dimm number
  @param[in] Rank              - Package rank number
  @param[out] RankSize         - Pointer to package rank size in 64MB unit

  @retval MRC_STATUS_SUCCESS

**/

MRC_STATUS
EFIAPI
CalculateDimmPackageRankSizeDdr5 (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Dimm,
  IN     UINT8   Rank,
     OUT UINT16  *RankSize
  )
{
  PSYSHOST            Host;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               TechIndex;
  UINT8               ChannelNumPerDimm;
  UINT8               PrimaryBusWidthPerCh;
  UINT8               DramIOWidth = 0;
  UINT16              DramDensity = 0;
  UINT8               DieCount;
  BOOLEAN             RankMixIndicator;

  Host                 = GetSysHostPointer ();
  ChannelNvList        = GetChannelNvList (Host, Socket);
  DimmNvList           = GetDimmNvList (Host, Socket, Ch);
  ChannelNumPerDimm    = (((*DimmNvList)[Dimm].SPDMemBusWidth & (BIT6 | BIT5)) >> SPD_CHNUMBER_PER_DIMM_FIELD_OFFSET);
  ChannelNumPerDimm    = (SPD_TO_ACTUAL_CHNUMBER_PER_DIMM_SHIFT_BASE << ChannelNumPerDimm);
  PrimaryBusWidthPerCh = ((*DimmNvList)[Dimm].SPDMemBusWidth & (BIT2 | BIT1 | BIT0));
  PrimaryBusWidthPerCh = (SPD_TO_ACTUAL_PRIMARY_BUS_WIDTH_PER_CH_SHIFT_BASE << PrimaryBusWidthPerCh);
  DieCount             = (*DimmNvList)[Dimm].dieCount;
  RankMixIndicator     = (*DimmNvList)[Dimm].rankMix;

  if ((*DimmNvList)[Dimm].mapOut[Rank]) {
    *RankSize = 0;
  } else {
    if ((*DimmNvList)[Dimm].DcpmmPresent) {
      TechIndex = (*DimmNvList)[Dimm].aepTechIndex;
      *RankSize  = rankSizeDdrt[TechIndex];
    } else {
      //
      // Hand all ranks for symmetric Dimm or even rank for asymmetric Dimm
      //
      DramIOWidth   = (SPD_TO_ACTUAL_DRAM_IO_WIDTH_SHIFT_BASE << (*DimmNvList)[Dimm].dramIOWidth);
      DramDensity   = EncodedSpdToDramDensity ((*DimmNvList)[Dimm].sdramCapacity);
      //
      // Capacity in bytes = Logical rank size * Die per package * Package ranks per channel (seen in DDR5 SPD Sepec 0.88),
      // where Logical rank size = Number of channels per DIMM * Primary bus width per channel
      //                           / SDRAM I/O Width * SDRAM density per die / 8
      //
      *RankSize     = (ChannelNumPerDimm * PrimaryBusWidthPerCh / DramIOWidth) * DramDensity * 2; // Convert Gbits unit to 64MB unit by *2
      //
      // Handle odd ranks for asymmetric Dimm
      //
      if (RankMixIndicator && ((Rank % 2) != 0)) { // Odd rank for asymmetric Dimm
        DieCount      = (*DimmNvList)[Dimm].dieCountSecondary;
        DramIOWidth   = (SPD_TO_ACTUAL_DRAM_IO_WIDTH_SHIFT_BASE << (*DimmNvList)[Dimm].dramIOWidthSecondary);
        DramDensity   = EncodedSpdToDramDensity ((*DimmNvList)[Dimm].sdramCapacitySecondary);
        *RankSize     = (ChannelNumPerDimm * PrimaryBusWidthPerCh / DramIOWidth) * DramDensity * 2; // Convert Gbits unit to 64MB unit by *2
      }
    }
  }

  if (DieCount && ((*ChannelNvList)[Ch].encodedCSMode == 2)) {
    (*RankSize) *= DieCount;
  }

  return MRC_STATUS_SUCCESS;
} // CalculateDimmPackageRankSizeDdr5

/**
Calculate the medium and fine timebases, using integer math.

@param Mtb     - Location to store the calculated medium timebase, in femtoseconds.
Example: Mtb = 125ps. ((125/1,000) * 1,000,000) = 125,000
@param Ftb     - Location to store the calculated fine timebase, in femtoseconds.
Example: Ftb = 1ps.  (1 * 1,000) = 1,000

@retval N/A

**/
VOID
GetDimmTimeBaseDdr5 (
PSYSHOST  Host,
UINT8     Spdmtb,
UINT8     Spdftb,
INT32     * Mtb,
INT32     * Ftb
)
{
  if (Spdftb > 0) {
    *Ftb = Spdftb * FREQUENCY_FTB_OFFSET;
  } else {
    *Ftb = 0;
  }

  if (Spdmtb > 0) {
    *Mtb = Spdmtb * (FREQUENCY_MTB_OFFSET / 1000);
  } else {
    *Mtb = 0;
  }
} // GetDimmTimeBaseDdr5

/**

  Apply workaround for Montage SPD Hub A0 Errata(Integrated Thermal Sensor)

  @param[in] Socket  - Socket to initizlize
  @param[in] Channel - Channel on the socket
  @param[in] Dimm    - Dimm on the channel

  @retval NA

**/
VOID
Ddr5MontageSpdHubA0Errata (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
)
{
  UINT8        SPDReg;
  UINT8        DeviceRevision;
  UINT16       VendorId;
  UINT16       DeviceType;

  //
  // Get Internal Thermal Sensor Device ID
  // MR0:MR1
  //
  SpdHubReadByte (Socket, Channel, Dimm, TS5_MR0_DEVICE_TYPE_MSB, &SPDReg);
  DeviceType = SPDReg << 8;
  SpdHubReadByte (Socket, Channel, Dimm, TS5_MR1_DEVICE_TYPE_LSB, &SPDReg);
  DeviceType |= SPDReg;

  //
  // Get Internal Thermal Sensor Device Revision
  // MR2[5:1]
  //
  SpdHubReadByte (Socket, Channel, Dimm, TS5_MR2_DEVICE_REVISION, &DeviceRevision);
  DeviceRevision = (DeviceRevision >> 1) & 0x1f;

  //
  // Get Internal Thermal Sensor Vendor ID
  //
  SpdHubReadByte (Socket, Channel, Dimm, TS5_MR3_VENDOR_ID_0, &SPDReg);
  VendorId = SPDReg;
  SpdHubReadByte (Socket, Channel, Dimm, TS5_MR4_VENDOR_ID_1, &SPDReg);
  VendorId |= (SPDReg << 8);

  if ((VendorId == 0x3286) && (DeviceType == 0x5118) && (DeviceRevision == 0)) {
    // set MR28 Thermal Sensor High Limit Configuration - Low Byte, default 0x70
    SPDReg = 0x70;
    SpdHubWriteByte (Socket, Channel, Dimm, TS5_MR28_TEMP_HIGH_0, &SPDReg);

    // set MR29 Thermal Sensor High Limit Configuration - High Byte, default 0x03
    SPDReg = 0x03;
    SpdHubWriteByte (Socket, Channel, Dimm, TS5_MR29_TEMP_HIGH_1, &SPDReg);

    // set MR32 Thermal Sensor Critical Temperature High Limit Configuration - Low Byte, default 0x50
    SPDReg = 0x50;
    SpdHubWriteByte (Socket, Channel, Dimm, TS5_MR32_CRITICAL_TEMP_HIGH_0, &SPDReg);

    // set MR33 Thermal Sensor Critical Temperature High Limit Configuration- High Byte, default 0x05
    SPDReg = 0x05;
    SpdHubWriteByte (Socket, Channel, Dimm, TS5_MR33_CRITICAL_TEMP_HIGH_1, &SPDReg);

    // MR19 [3]: CLR_TS_CRIT_LOW
    // MR19 [2]: CLR_TS_CRIT_HIGH
    // MR19 [1]: CLR_TS_LOW
    // MR19 [0]: CLR_TS_HIGH
    SPDReg = 0x0f;
    SpdHubWriteByte (Socket, Channel, Dimm, TS5_MR19_CLEAR_ERROR_CMD, &SPDReg);

    // MR27[7]: CLR_GLOBAL clear global status
    SPDReg = 0x80;
    SpdHubWriteByte (Socket, Channel, Dimm, TS5_MR27_INTEERUPT_CONFIG, &SPDReg);
  }
}
