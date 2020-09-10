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
#include <Library/ProcMemInitChipLib.h>
#include <Library/BaseLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CheckpointLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/EmulationConfigurationLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/PmemMiscLib.h>
#include <Library/MemRcLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/DdrtCoreLib.h>

//
// Supported DRAM tecnologies (SDRAM addressing)
//
//                                            |      2Gb     |      4Gb     |      8Gb     |     16Gb     |
const   UINT8 primaryWidthDDR4[MAX_TECH] = {   4,   8,  16,   4,   8,  16,   4,   8,  16,   4,   8,  16};   // Primary SDRAM device width
const   UINT8 rowBitsDDR4[MAX_TECH] = {  15,  14,  14,  16,  15,  15,  17,  16,  16,  18,  17,  17};   // Number of row address bits
const   UINT8 columnBitsDDR4[MAX_TECH] = {  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10};   // Number of column address bits
const   UINT8 bankGroupBitsDDR4[MAX_TECH] = {   2,   2,   1,   2,   2,   1,   2,   2,   1,   2,   2,   1};   // Number of bank group bits
const   UINT8 bankAddressBitsDdr4[MAX_TECH] = {   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2};   // Number of bank address bits
const   UINT16 rankSizeDDR4[MAX_TECH] = {  64,  32,  16, 128,  64,  32, 256, 128,  64, 512, 256, 128};   // Rank size in 64 MB units
const   UINT16 rankSizeDdrt[MAX_TECH_DDRT] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384}; // Rank Size in 64 MB Units

// Prototypes
STATIC UINT16  PopulateDimmTypeMask(PSYSHOST Host, UINT8 socket);                                                // LOCAL

#define UDIMM_SOCKET                BIT0
#define RDIMM_SOCKET                BIT1
#define LRDIMM_STANDARD_SOCKET      BIT2
#define LRDIMM_3DS_SOCKET           BIT3
#define LRDIMM_AEP_SOCKET           BIT4
#define LRDIMM_BPS_SOCKET           BIT5
#define LRDIMM_CPS_SOCKET           BIT6
#define RDIMM_3DS_SOCKET            BIT7
#define AEPDIMM_SOCKET              BIT8
#define BPSDIMM_SOCKET              BIT9
#define CPSDIMM_SOCKET              BIT10

#define AEP_DIMM_SOCKET (LRDIMM_AEP_SOCKET | AEPDIMM_SOCKET)
#define BPS_DIMM_SOCKET (LRDIMM_BPS_SOCKET | BPSDIMM_SOCKET)
#define CPS_DIMM_SOCKET (LRDIMM_CPS_SOCKET | CPSDIMM_SOCKET)

#define DCPMM_DIMM_SOCKET (AEP_DIMM_SOCKET | BPS_DIMM_SOCKET | CPS_DIMM_SOCKET)

//
// Structure used to map SPD encoded DRAM densities to Gb units
//
typedef struct {
  UINT8   SpdEncodedDensity;
  UINT16  DramDensityGb;
} SPD_DENSITY_MAP;

//
// Timing tables are indexed by these frequencies:
// 800  1000  1066  1200  1333  1400  1600  1800  1866  2000  2133  2200  2400  2600  2666  2800  2933  3000  3200
//

// Table for how the Module Delay bytes are adjusted per frequency in MTP units
// The general equation for the delay adjustment value is ROUNDDOWN(((tCK - 2.5ns) * 3/4) / tMTB)
// 800:    (2.5ns   - 2.5ns) * 3/4 / 0.125 = 0      = 0
// 1066:   (1.875ns - 2.5ns) * 3/4 / 0.125 = 3.75   = 3
// 1333:   (1.5ns   - 2.5ns) * 3/4 / 0.125 = 6      = 6
// 1600:   (1.25ns  - 2.5ns) * 3/4 / 0.125 = 7.5    = 7
// 1876:   (1.066ns - 2.5ns) * 3/4 / 0.125 = 8.6    = 8
// 2133:   (0.938ns - 2.5ns) * 3/4 / 0.125 = 9.37   = 9
// 2400:   (0.833ns - 2.5ns) * 3/4 / 0.125 = 9.996  = 10
// 2666:   (0.75ns - 2.5ns) * 3/4 / 0.125 = 10.5    = 10
// 2933:   (0.682ns - 2.5ns) * 3/4 / 0.125 = 10.908 = 11
// 3200:   (0.625ns - 2.5ns) * 3/4 / 0.125 = 11.25  = 11

UINT8 lrDimmModuleDelayAdjust[MAX_SUP_FREQ] = { 0, 2, 3, 5, 6, 7, 7, 8, 8, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11};

/**

  Detect DIMM population

  @param Host  - Point to sysHost

  @retval SUCCESS

**/
UINT32
DetectDIMMConfig (
  PSYSHOST Host
  )
{
  UINT8                 Socket;
  UINT8                 Ch;
  UINT8                 Dimm;
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  SMB_DEVICE_STRUCT     Spd;
  CHAR8                 StrPresent[] = "Present";
  CHAR8                 StrNotPresent[] = "Not Present";
  CHAR8                 *StrPtr;
  DDR_TECHNOLOGY_TYPE   DdrType;
  SYS_SETUP             *Setup;
  UINT8                 SmbInstance;
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  Setup = GetSysSetupPointer ();

  DdrType = Ddr4Type;
  Host->nvram.mem.dramType = SPD_TYPE_DDR4;
#ifdef DDR5_SUPPORT
  DdrType = Ddr5Type;
  Host->nvram.mem.dramType = SPD_TYPE_DDR5;
#endif  // DDR5_SUPPORT

  InitializeSpd (Socket, DdrType);

  //
  // Initialize common parts of the smbDevice structure for all SPD devices
  //
  ZeroMem (&Spd, sizeof (Spd));
  Spd.compId = SPD;
  Spd.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  Spd.address.deviceType = DTI_EEPROM;

  if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {

    RcDebugPrint (SDBG_MAX, "%a%a",
      "Socket | Channel | DIMM |  Bus Segment | SMBUS Address\n",
      "-------|---------|------|--------------|--------------\n");

    //
    // Detect DIMMs on each channel
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      //
      // Detect DIMMs in each slot
      //
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      //
      // Initialize MaxDimm on this channel
      //
      (*ChannelNvList)[Ch].maxDimm = 0;
      for (Dimm = 0; Dimm < Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots; Dimm++) {

        OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DIMM_DETECT, CHECKPOINT_MINOR_INIT_SMB, (UINT16) ((Socket << 8) | (Ch << 4) | (Dimm))));
        //
        // Init to DIMM not present
        //
        (*DimmNvList)[Dimm].dimmPresent = 0;

        //
        // Read SPD device (type 0x0B) to detect DIMM presence
        //
        if (EFI_ERROR (GetSmbAddress (Socket, Ch, Dimm, &Spd))) {
          RcDebugPrint (SDBG_MAX,
            "   %d   |    %2d   |   %d  |      X       |       X      - Not Present (Could not retrieve SMBus Address)\n",
            Socket, Ch, Dimm);
          continue;
        }


        if (SpdGetModuleType (Socket, Ch, Dimm, &(*DimmNvList)[Dimm].keyByte) == MRC_STATUS_SUCCESS) {

          StrPtr = StrPresent;

          //
          // We know a DIMM is present if we arrive here
          //
          (*DimmNvList)[Dimm].dimmPresent = 1;

          //
          // Increment the number of DIMMs on this channel
          //
          (*ChannelNvList)[Ch].maxDimm++;
          //
          // Get TimeBases
          //

          DetectSpdTypeDIMMConfig (Host, Socket, Ch, Dimm, (*DimmNvList)[Dimm].keyByte);
        } else {
          StrPtr = StrNotPresent;
        }

        RcDebugPrint (SDBG_MAX,
          "   %d   |    %2d   |   %d  |      %d       |       %d      - %a\n",
          Socket, Ch, Dimm, Spd.address.SmbInstance, Spd.address.strapAddress, StrPtr);

      } // Dimm loop

    } // Ch loop

    //
    // Dump the SPD for each channel/slot.
    //
#ifdef DEBUG_CODE_BLOCK
    DisplaySpdContents (Socket);
#endif  // DEBUG_CODE_BLOCK

  } // if cold boot

  //
  // DIMM detection is done - adjust SMBus frequency to setup requested value
  for (SmbInstance = 0; SmbInstance < MAX_SMB_INSTANCE; SmbInstance++) {
    AdjustProcSmbFreq (Socket, SmbInstance, Setup->mem.SpdSmbSpeed);
  }

#if defined(CWV_A0_REFACTOR_POST_PO)
  ResetProcSmb (Socket, 0);
  ResetProcSmb (Socket, 1);
#endif

  //
  // Wait for NVMCTLR to indicate that Mailbox interface is ready
  //
  OutputCheckpoint (CHECKPOINT_MAJOR_DIMM_DETECT, CHECKPOINT_MINOR_MAILBOX_READY, 0);
  WaitForMailboxReady (Host, Socket);
#ifdef DDRT_SUPPORT

  if ((GetSysBootMode () != S3Resume) &&
      (!FeaturePcdGet (PcdCosimBuild)) &&
      (!FeaturePcdGet (PcdCteBuild)) &&
      (!UbiosGenerationOrHsleEnabled ()))
  {
    //
    // Display Falcon/Elk/Barlow Valley firmware revision information in serial port
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        ReportPrevBootDcpmError (Host, Socket, Ch, Dimm);
      } // dimm loop
    } // ch loop
    CheckAepFwAndEnableDramPoll (Host, Socket);
    DisableLastSystemShutdownLatch (Host, Socket);
    DisableExtendedAdr (Host, Socket);
    CheckDdrtIoTrainingStatus (Socket);
    DisplayFnvInfo (Host, Socket);
    SaveAepLogInfo (Host, Socket);
#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      DcpMemErrorLogDisplay (Host, Socket);
    }
#endif //DEBUG_CODE_BLOCK
    //
    // Check DDRT Frequency changed from setup menu
    //
    CheckDdrtFreqChange (Host, Socket);
  }
#endif // DDRT_SUPPORT

  return SUCCESS;
}

struct dimmDevice
(*GetChDimmList (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  ))[MAX_DIMM]
{
  return (&Host->var.mem.socket[socket].channelList[ch].dimmList);
}

/**

    Get DDR training DIMM list

    @param Ch   - Channel number

    @retval - Pointer to DIMM list array

**/
struct DimmDeviceTraining
(*GetChDimmListTraining (
  UINT8  Ch
  ))[MAX_DIMM]
{
  PSYSHOST  Host;

  Host = (PSYSHOST) GetSysHostPointer ();

  return (&Host->var.mem.TrainingVar.channelList[Ch].dimmList);
}

/**

  Clears map out flags for DIMMs mapped out during
  memory decode run on previous boot

  @param[in] Host - Pointer to Host System Data

  @return None

**/
VOID
EFIAPI
ClearMemMapMappedOutDimms (
  IN PSYSHOST Host
  )
{
  UINT8                 Socket;
  UINT8                 Ch;
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 MaxChDdr;
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  SYS_SETUP             *Setup;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets
    //
    if (!Host->nvram.mem.socket[Socket].enabled) {
      continue;
    }

    ChannelNvList = GetChannelNvList (Host, Socket);

    //
    // Look for DIMMs that will be mapped out and print warning if needed
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      //
      // Skip disabled channels
      //
      if (!(*ChannelNvList)[Ch].enabled) {
        continue;
      }

      DimmNvList = GetDimmNvList(Host, Socket, Ch);

      for (Dimm = 0; Dimm < Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots; Dimm++) {
        //
        // Skip not populated slots
        //
        if (!(*DimmNvList)[Dimm].dimmPresent) {
          continue;
        }
        //
        // Check if DIMM was mapped out
        //
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          //
          // Only one rank for DCPMM
          //
          if ((*DimmNvList)[Dimm].mapOut[0]) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "PMem rank mapped out (reason 0x%x)\n", (*DimmNvList)[Dimm].MapOutReason[0]);
            //
            // Check if mapped out rank should be re-enabled
            //
            if ((!Setup->mem.socket[Socket].ddrCh[Ch].dimmList[Dimm].mapOut[0]) &&
                (((*DimmNvList)[Dimm].MapOutReason[0] == DIMM_RANK_MAP_OUT_MEM_DECODE) ||
                 ((*DimmNvList)[Dimm].MapOutReason[0] == DIMM_RANK_MAP_OUT_POP_POR_VIOLATION))) {
              //
              // Rank disabled by memory decode/memory population POR enforcement - re-enable mapped out rank
              //
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "PMem enable mapped out rank\n");
              (*DimmNvList)[Dimm].mapOut[0] = 0;
              (*DimmNvList)[Dimm].MapOutReason[0] = DIMM_RANK_MAP_OUT_UNKNOWN;
            }
          }
        } else {
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numDramRanks; Rank++) {
            if ((*DimmNvList)[Dimm].mapOut[Rank]) {
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                             "DDR rank mapped out (reason 0x%x)\n", (*DimmNvList)[Dimm].MapOutReason[Rank]);
              //
              // Check if mapped out rank should be re-enabled
              //
              if ((!Setup->mem.socket[Socket].ddrCh[Ch].dimmList[Dimm].mapOut[Rank]) &&
                  (((*DimmNvList)[Dimm].MapOutReason[Rank] == DIMM_RANK_MAP_OUT_MEM_DECODE) ||
                   ((*DimmNvList)[Dimm].MapOutReason[Rank] == DIMM_RANK_MAP_OUT_POP_POR_VIOLATION))) {
                //
                // Rank disabled by memory decode/memory population POR enforcement - re-enable mapped out rank
                //
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                               "DDR enable mapped out rank\n");
                (*DimmNvList)[Dimm].mapOut[Rank] = 0;
                (*DimmNvList)[Dimm].MapOutReason[Rank] = DIMM_RANK_MAP_OUT_UNKNOWN;
              }
            }
          }
        }
      }
    }
  }
}

/**

  Detects Mapped out DIMMs and issues EWL accordingly

  @param[in] Host - Pointer to Host System Data

  @return None

**/
VOID
EFIAPI
CheckForMappedOutDimms (
  IN PSYSHOST Host
  )
{
  UINT8                 Socket;
  UINT8                 Ch;
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 MaxChDdr;
  BOOLEAN               DimmMappedOut;
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];

  MaxChDdr = GetMaxChDdr ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets
    //
    if (!Host->nvram.mem.socket[Socket].enabled) {
      continue;
    }

    ChannelNvList = GetChannelNvList (Host, Socket);

    //
    // Look for DIMMs that will be mapped out and print warning if needed
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      //
      // Skip disabled channels
      //
      if (!(*ChannelNvList)[Ch].enabled) {
        continue;
      }

      DimmNvList = GetDimmNvList(Host, Socket, Ch);

      for (Dimm = 0; Dimm < Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots; Dimm++) {
        //
        // Skip not populated slots
        //
        if (!(*DimmNvList)[Dimm].dimmPresent) {
          continue;
        }
        //
        // Check if DIMM was mapped out
        //
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          //
          // Only one rank for DCPMM
          //
          DimmMappedOut = ((*DimmNvList)[Dimm].mapOut[0]) ? TRUE : FALSE;
        } else {
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numDramRanks; Rank++) {
            if (!(*DimmNvList)[Dimm].mapOut[Rank]) {
              //
              // Rank enabled (DIMM enabled)
              //
              break;
            }
          }
          DimmMappedOut = ((*DimmNvList)[Dimm].numDramRanks == Rank) ? TRUE : FALSE;
        }

        if (DimmMappedOut) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM is disabled! (DIMM mapped out)\n");
          EwlOutputType1 (WARN_USER_DIMM_DISABLE, WARN_USER_DIMM_DISABLE_MAPPED_OUT, Socket, Ch, Dimm, NO_RANK);
        }
      }
    }
  }
}

VOID
CheckPORMixingWithinSocket (
  PSYSHOST Host
  )
{
  UINT8   socket;
  UINT16  dimmTypeMask;
  UINT8   minorCode;

  dimmTypeMask = 0;
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (Host->nvram.mem.socket[socket].enabled == 0) {
      continue;
    }
    dimmTypeMask |= PopulateDimmTypeMask (Host, socket);
  }

  //
  // Population Rules
  //
  minorCode = 0;

  //
  // 1. UDIMM cannot be mixed with any other dimm type
  //
  if (dimmTypeMask & UDIMM_SOCKET) {
    if (dimmTypeMask & (~UDIMM_SOCKET)) {
      minorCode |= WARN_DIMM_POP_RUL_UDIMM_POPULATION;
    }
  }

  //
  // 2. RDIMM can only be mixed with NVMDIMM
  //
  if (dimmTypeMask & RDIMM_SOCKET) {
    if (dimmTypeMask & (~(RDIMM_SOCKET | DCPMM_DIMM_SOCKET))) {
      minorCode |= WARN_DIMM_POP_RUL_RDIMM_POPULATION;
    }
  }

  //
  // 3. LRDIMM Dual Die Package can only be mixed with NVMDIMM
  //
  if (dimmTypeMask & LRDIMM_STANDARD_SOCKET) {
    if (dimmTypeMask & (~(LRDIMM_STANDARD_SOCKET | DCPMM_DIMM_SOCKET))) {
      minorCode |= WARN_DIMM_POP_RUL_LRDIMM_DUAL_DIE_POPULATION;
    }
  }

  //
  // 4. LRDIMM 3DS can only be mixed with NVMDIMM
  //
  if (dimmTypeMask & LRDIMM_3DS_SOCKET) {
    if (dimmTypeMask & (~(LRDIMM_3DS_SOCKET | DCPMM_DIMM_SOCKET))) {
      minorCode |= WARN_DIMM_POP_RUL_LRDIMM_3DS_POPULATION;
    }
  }

  //
  // 5. RDIMM 3DS can only be mixed with NVMDIMM
  //
  if (dimmTypeMask & RDIMM_3DS_SOCKET) {
    if (dimmTypeMask & (~(RDIMM_3DS_SOCKET | DCPMM_DIMM_SOCKET))) {
      minorCode |= WARN_DIMM_POP_RUL_RDIMM_3DS_POPULATION;
    }
  }

  if ((minorCode != 0)) {
    if ((dimmTypeMask & LRDIMM_3DS_SOCKET) || (dimmTypeMask & RDIMM_3DS_SOCKET)) {
      RcDebugPrint (SDBG_MAX,
                     "Mixing Population Rules across Sockets\n");
      OutputWarning (WARN_DIMM_POP_RUL_EXTENDED, minorCode, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
    } else {
      RcDebugPrint (SDBG_MAX,
                     "Mixing Population Rules across Sockets\n");
      OutputWarning (WARN_DIMM_POP_RUL, minorCode, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
    }
  }

  //
  // 6. AEP dimm should not be mixed with BPS dimm
  //
  if ((dimmTypeMask & AEP_DIMM_SOCKET) && (dimmTypeMask & BPS_DIMM_SOCKET)) {
    RcDebugPrint (SDBG_MAX,
                     "Mixing Population Rules for PMems across Sockets\n");
    OutputWarning (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DCPDIMM_MIXING, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
  }
}

STATIC UINT16
PopulateDimmTypeMask (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT8                ch;
  UINT8                dimm;
  UINT16               dimmTypeMask;
  struct dimmNvram     (*dimmNvList)[MAX_DIMM];
  struct channelNvram  (*channelNvList)[MAX_CH];
  UINT8                MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  dimmTypeMask = 0;
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if ((*dimmNvList)[dimm].actKeyByte2 == SPD_UDIMM) {
        dimmTypeMask |= UDIMM_SOCKET;
      }
      if ((*dimmNvList)[dimm].actKeyByte2 == SPD_RDIMM) {
        if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
          dimmTypeMask |= RDIMM_3DS_SOCKET;
        } else {
          dimmTypeMask |= RDIMM_SOCKET;
        }
        // Check for 3DS
      }

      if ((*dimmNvList)[dimm].lrDimmPresent) {
        if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
          //
          // Currently "DcpmmPresent" is used to indicate presence of DCPMM (AEP/BPS/CPS)
          // (Check Subsystem Device ID for DCPMM generation)
          //
          if ((*dimmNvList)[dimm].SubsystemDeviceID == EKV) {
            dimmTypeMask |= LRDIMM_AEP_SOCKET;
          }  else if ((*dimmNvList)[dimm].SubsystemDeviceID == BWV) {
            dimmTypeMask |= LRDIMM_BPS_SOCKET;
          } else if ((*dimmNvList)[dimm].SubsystemDeviceID == CWV) {
            dimmTypeMask |= LRDIMM_CPS_SOCKET;
          }
        } else if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
          dimmTypeMask |= LRDIMM_3DS_SOCKET;
        } else {
          dimmTypeMask |= LRDIMM_STANDARD_SOCKET;
        }
      } //LRDIMM

      //
      // Check for Apache Pass , Barlow Pass, and Crow Pass dimm.
      //
      if ((*dimmNvList)[dimm].SubsystemDeviceID == EKV) {
        dimmTypeMask |= AEPDIMM_SOCKET;
      } else if ((*dimmNvList)[dimm].SubsystemDeviceID == BWV) {
        dimmTypeMask |= BPSDIMM_SOCKET;
      } else if ((*dimmNvList)[dimm].SubsystemDeviceID == CWV) {
        dimmTypeMask |= CPSDIMM_SOCKET;
      }
    } // dimm
  } // ch
  return dimmTypeMask;
}

VOID
GetModuleSerialNumber (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     dimm,
  IN OUT UINT8     SPDModSN[SPD_MODULE_SERIAL]
  )
{
  SpdGetModuleSerialNumber (socket, ch, dimm, &SPDModSN[0]);
}

/**

  GetDdr4SpdPageEn - Detect DDR4 vs. DDR3 SPD devices on SMBus

  @param Host       - Pointer to sysHost
  @param socket     - Socket ID

  @retval ddr4SpdPageEn = 1 for DDR4 SPD, 0 for DDR3 SPD

**/
UINT8
GetDdr4SpdPageEn (
  PSYSHOST         Host,
  UINT8            socket
  )
{
  return 1;
}

CHAR8 *
GetDimmTypeStr (
  IN  UINT8 DramType,
  IN  UINT8 DimmType,
  OUT CHAR8 *StringBuffer
  )
{
#ifdef DEBUG_CODE_BLOCK
  if (DramType == SPD_TYPE_AEP) {
    AsciiStrCpyS (StringBuffer, MRC_STRING_LENGTH, "ODCDIMM");
    return StringBuffer;
  }

  switch (DimmType) {
#if FixedPcdGetBool(PcdMcrSupport)
  case SPD_MCRDIMM_DDR5:
    AsciiStrCpyS (StringBuffer, MRC_STRING_LENGTH, "MCR-DIMM");
    break;
#endif // FixedPcdGetBool(PcdMcrSupport)
  case SPD_UDIMM:
    AsciiStrCpyS (StringBuffer, MRC_STRING_LENGTH, "UDIMM");
    break;
  case SPD_RDIMM:
    AsciiStrCpyS (StringBuffer, MRC_STRING_LENGTH, "RDIMM");
    break;
  case SPD_LRDIMM_DDR4:
    AsciiStrCpyS (StringBuffer, MRC_STRING_LENGTH, "LRDIMM");
    break;
  case SPD_SODIMM:
    AsciiStrCpyS (StringBuffer, MRC_STRING_LENGTH, "SODIMM");
    break;
  default:
    AsciiStrCpyS (StringBuffer, MRC_STRING_LENGTH, "Unknown DIMM");
    break;
  }
#endif  // DEBUG_CODE_BLOCK
  return StringBuffer;
}

UINT8
GetTechIndex (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  PSYSHOST            Host;
  UINT8               Ti = 0;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  Host = (PSYSHOST) GetSysHostPointer ();

#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Socket)) {
    return GetTechIndexDdr5 (Host, Socket, Ch, Dimm);
  }
#endif

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_AEP) {
    Ti = 0;
  } else if ((*DimmNvList)[Dimm].keyByte == SPD_TYPE_DDR4) {
    //
    // DDR4
    //
    for (Ti = 0; Ti < MAX_TECH_DDR4; Ti++) {

      if (((primaryWidthDDR4[Ti] >> 3) == (*DimmNvList)[Dimm].dramIOWidth) &&
          (rowBitsDDR4[Ti] == (*DimmNvList)[Dimm].numRowBits) &&
          (columnBitsDDR4[Ti] == (*DimmNvList)[Dimm].numColBits) &&
          (bankGroupBitsDDR4[Ti] == (*DimmNvList)[Dimm].numBankGroupBits)
          ) {
        break;
      }
    } // Ti loop

    if (Ti >= MAX_TECH_DDR4) {
      Ti = 0xFF;
    }
  }

  return Ti;
} // GetTechIndex

/**
  Convert the encoded DRAM density from the SPD to Gb units

  @param[in]  SpdEncodedDensity   - SPD density value to convert to Gb units

  @retval DRAM density in Gb

**/
UINT16
EFIAPI
EncodedSpdToDramDensity (
  IN  UINT8         SpdEncodedDensity
  )
{
  UINT8             Index;
  UINT8             MaxIndex;
  UINT16            DramDensityGb;
  SPD_DENSITY_MAP   SpdToDramDensity[] = {{SPD_1Gb,     1},
                                          {SPD_2Gb,     2},
                                          {SPD_4Gb,     4},
                                          {SPD_8Gb,     8},
                                          {SPD_12Gb,   12},
                                          {SPD_16Gb,   16},
                                          {SPD_24Gb,   24},
                                          {SPD_32Gb,   32},
                                          {SPD_48Gb,   48},
                                          {SPD_64Gb,   64}
                                         };


  DramDensityGb = 0;
  MaxIndex      = ARRAY_SIZE (SpdToDramDensity);
  for (Index = 0; Index < MaxIndex; Index++) {
    if (SpdToDramDensity[Index].SpdEncodedDensity == SpdEncodedDensity) {
      DramDensityGb = SpdToDramDensity[Index].DramDensityGb;
      break;
    }
  }

  if (Index == MaxIndex) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_194);
  }

  return DramDensityGb;
}
