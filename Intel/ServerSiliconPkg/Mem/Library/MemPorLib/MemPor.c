/** @file
  MemPor.c - produces the memory POR table

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/PcdLib.h>
#include <SysHost.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemPorLib.h>
#include <MemCommon.h>
#include "Include/MemPorLibInternal.h"
#include <Library/MemMcIpLib.h>
#include <Library/KtiApi.h>
#include <Library/SpdDecodeLib.h>
#include <PlatformInfoTypes.h>

/**

  Fills out the POR_CHANNEL_DIMM_INFO struct with the appropriate info based
  on how the system is configured.

  @param[in] Host     - Pointer to SysHost
  @param[in] Socket   - Socket ID
  @param[in] Ch       - Channel number (0-based)
  @param[in] DimmInfo - Pointer to POR_CHANNEL_DIMM_INFO struct to be filled out

  @retval N/A

**/
VOID
GetDimmInfo (
  IN  PSYSHOST               Host,
  IN  UINT8                  Socket,
  IN  UINT8                  Ch,
  OUT POR_CHANNEL_DIMM_INFO  *DimmInfo
  )
{
  UINT8                 Dimm;
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];

  // Initalize structure values
  DimmInfo->TotalRanks = 0;
  DimmInfo->EccPresent = FALSE;
  DimmInfo->DimmX16Present = FALSE;
  DimmInfo->DimmStack = StackSdp;

  ChannelNvList = GetChannelNvList (Host, Socket);

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  DimmInfo->MaxDimm = (*ChannelNvList)[Ch].maxDimm;

  // Loop for each DIMM (supported by POR table)
  for (Dimm = 0; Dimm < MAX_POR_DIMM; Dimm++) {

    // Skip if no DIMM present
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    // Default to the system value
    DimmInfo->DimmType[Dimm] = Host->nvram.mem.dimmTypePresent;

    DimmInfo->TotalRanks += (*DimmNvList)[Dimm].numDramRanks;

    // change to LRDIMM
    if (IsLrdimmPresent (Socket, Ch, Dimm)){
      DimmInfo->DimmType[Dimm] = LRDIMM;
    }

    // Change to VLPRDIMM.
    if (((*DimmNvList)[Dimm].VlpRdimmPresent) && (IsCpuAndRevision (CPU_ICXD, REV_ALL))) {
      DimmInfo->DimmType[Dimm] = VLPRDIMM;
    }

    // If DCPMM present, change type to NVMDIMM
    if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
      DimmInfo->DimmType[Dimm] = NVMDIMM;
      // Skip checking DIMM stack type if DCPMM is present since we only care about stack type of
      // non-DCPMM DIMMs
      continue;
    }

    // Check for presence of DDP
    if (((*DimmNvList)[Dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_MULTI_LOAD_TYPE) {
      // The only valid multi-load type is DDP, as QDP DIMMs don't exist.
      DimmInfo->DimmStack = StackDdp;
    }

    // Check for presence of 3DS DIMM
    if (((*DimmNvList)[Dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) {
      DimmInfo->DimmStack = Stack3ds;
    }
  } // Dimm loop

  // Check if bus width extension is present to detect ECC
  if (IsThereAnyEccDimmInChannel (Socket, Ch)) {
    DimmInfo->EccPresent = TRUE;
  }

  // Check if there are x16 DRAM DIMMs in the channel
  if (IsThereAnyDimmX16InChannel (Socket, Ch)) {
    DimmInfo->DimmX16Present = TRUE;
  }

  DimmInfo->DimmConnectorType = GetMemoryConnectorType (Host, Socket, Ch);

}

/**

  Determines the minimum socket configuration (if any) that an entry in the memory POR table
  is valid for.

  @param[in]  TableSocketEntry   - The PorSocketConfig element of the DIMM_POR_ENTRY struct
  @param[out] MinAllowedSockets  - The minimum socket configuration this entry in the POR table is
                                    valid for.
                                    i.e. A value of 1 indicates that the config is valid
                                          for all socket configurations of 1 or more
                                         A  value of 2 indicates that the config is valid
                                          for all socket configurations of 2 or more
                                         NO_MIN_ALLOWED indicates the detected socket config
                                          must exactly match the entry in the POR table

  @retval N/A

**/
VOID
GetMinAllowedSockets (
  IN  UINT8  TableSocketEntry,
  OUT UINT8  *MinAllowedSockets
  )
{
  switch (TableSocketEntry) {
    case SERVER_1S_PLUS:
      *MinAllowedSockets = ONE_SOCKET;
      break;
    case SOCKET_2S_PLUS:
      *MinAllowedSockets = TWO_SOCKETS;
      break;
    case SOCKET_4S_PLUS:
      *MinAllowedSockets = FOUR_SOCKETS;
      break;
    default:
      *MinAllowedSockets = NO_MIN_ALLOWED;
      break;
  }
}


/**

  Find the highest DDR frequency supported by this configuration

  @param[in] Socket             - Socket Id
  @param[in] Ch                 - Channel number (0-based)
  @param[in] Vdd                - Vdd
  @param[out] PorDdrFrequency   - Highest frequency supported by the DDR configuration on the specified channel.
                                  = NOT_SUP if the configuration is not supported.

  @retval EFI_SUCCESS if configuration is supported
          EFI_UNSUPPORTED otherwise

**/
EFI_STATUS
EFIAPI
GetPorDdrFrequency (
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Vdd,
     OUT  UINT8     *PorDdrFrequency
  )
{
  PSYSHOST                Host;
  EFI_STATUS              Status = EFI_SUCCESS;
  POR_CHANNEL_DIMM_INFO   DimmInfo;
  UINT8                   PorTableIndex;
  UINT8                   PorSocketConfig;
  UINT8                   Chop;
  UINT16                  TableLength;
  DIMM_POR_ENTRY          *MemPorTable;
  SYS_SETUP               *Setup;
  UINT8                   NumSockets;
  UINT8                   MinAllowedSockets;
  UINT8                   FirstDimm;
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];

  *PorDdrFrequency = NOT_SUP;
  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Get default memory POR table
  //
  GetMemoryPorTable (&MemPorTable, &TableLength);

  //
  // Get platform memory POR table if it exists
  //
  CoreLookupFreqTable (Host, &MemPorTable, &TableLength);

  // Retrieve DIMM information
  GetDimmInfo (Host, Socket, Ch, &DimmInfo);

  // Get the socket configuration of the system.
  Status = GetPorSocketConfig (&PorSocketConfig);
  NumSockets = GetNumSockets ();

  Chop = (UINT8) GetChopType (Socket);

  // Get the first DIMM in the channel
  if (!FindFirstDimmInChannel (Socket, Ch, &FirstDimm)) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "DIMM not found in channel\n");
    return EFI_INVALID_PARAMETER;
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Memory Configuration: Vdd = %d, PorSocketConfig = %d, Chop = %d, DramType = %d, Spc = %d, Dpc = %d, DimmType = %d, DimmStack = %d, TotalRanks = %d\n",
      Vdd,
      PorSocketConfig,
      Chop,
      Host->nvram.mem.dramType,
      Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots,
      DimmInfo.MaxDimm,
      DimmInfo.DimmType[FirstDimm],
      DimmInfo.DimmStack,
      DimmInfo.TotalRanks);

  for (PorTableIndex = 0; PorTableIndex < TableLength; PorTableIndex++) {
    // Get the min allowed sockets allowed for this table entry configuration
    GetMinAllowedSockets (MemPorTable[PorTableIndex].PorSocketConfig, &MinAllowedSockets);

    if (((MemPorTable[PorTableIndex].PorSocketConfig == PorSocketConfig) || (MinAllowedSockets <= NumSockets)) &&
        ((MemPorTable[PorTableIndex].Chop == Chop) || (MemPorTable[PorTableIndex].Chop == CHOP_IGNORE)) &&
        (MemPorTable[PorTableIndex].DramType == Host->nvram.mem.dramType) &&
        (MemPorTable[PorTableIndex].Spc == Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots) &&
        (MemPorTable[PorTableIndex].Dpc == DimmInfo.MaxDimm) &&
        ((MemPorTable[PorTableIndex].DimmType[0] == DimmInfo.DimmType[0]) || ((*DimmNvList)[0].dimmPresent == 0) || (MemPorTable[PorTableIndex].DimmType[0] == DIMM_TYPE_IGNORE)) &&
        ((MemPorTable[PorTableIndex].DimmType[1] == DimmInfo.DimmType[1]) || ((*DimmNvList)[1].dimmPresent == 0) || (MemPorTable[PorTableIndex].DimmType[1] == DIMM_TYPE_IGNORE)) &&
        ((MemPorTable[PorTableIndex].DimmStack == DimmInfo.DimmStack)  || (MemPorTable[PorTableIndex].DimmStack == StackIgnore)) &&
        ((MemPorTable[PorTableIndex].DimmConnectorType == DimmInfo.DimmConnectorType) || (MemPorTable[PorTableIndex].DimmConnectorType == DimmConnectorIgnore)) &&
        (MemPorTable[PorTableIndex].MaxChannelRanks >= DimmInfo.TotalRanks) && // could make this exact in the future, if POR table included all possible rank count combinations
        (MemPorTable[PorTableIndex].Vdd == Vdd)) {

      //
      // SNR does not support SODIMM with ECC
      //
      if ((IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) && (DimmInfo.DimmType[FirstDimm] == SODIMM) && DimmInfo.DimmX16Present && DimmInfo.EccPresent) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SODIMM x16 ECC populated - Not supported\n");
        continue;
      }

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              " Table Index: %d\n", PorTableIndex);
      if (Setup->mem.enforcePOR == ENFORCE_POR_EN) {
        // POR setting
        *PorDdrFrequency = MemPorTable[PorTableIndex].PorFreq;
      } else if (Setup->mem.enforcePOR == ENFORCE_STRETCH_EN) {
        // Stretch Goal
        *PorDdrFrequency = MemPorTable[PorTableIndex].StretchFreq;
      }
      break;
    }
  } // PorTableIndex

  //
  // Check if we found a valid config
  //
  if (PorTableIndex == TableLength) {
    RcDebugPrint (SDBG_MINMAX,
                  "Configuration not in POR table!\n");
    RcDebugPrint (SDBG_MINMAX,
      "Memory Configuration: Vdd = %d, PorSocketConfig = %d, Chop = %d, DramType = %d, Spc = %d, Dpc = %d, DimmType = %d, DimmStack = %d, TotalRanks = %d, ECC = %d\n",
      Vdd,
      PorSocketConfig,
      Chop,
      Host->nvram.mem.dramType,
      Host->var.mem.socket[Socket].channelList[Ch].numDimmSlots,
      DimmInfo.MaxDimm,
      DimmInfo.DimmType[FirstDimm],
      DimmInfo.DimmStack,
      DimmInfo.TotalRanks,
      DimmInfo.EccPresent);
    Status = EFI_UNSUPPORTED;
  }

  RcDebugPrint (SDBG_MAX, "GetPORDDRFreq returns ddrfreq = %d\n", *PorDdrFrequency);

  return Status;
} // GetPorDdrFrequency

/** Print the system's socket configuration that corresponds with POR table entries for the DIMM Info table.

  @retval EFI_SUCCESS   The configuration was successfully printed.
  @retval !EFI_SUCCESS  The configuration was not successfully printed.

**/
EFI_STATUS
EFIAPI
PrintPorSocketConfigDimmInfo (
  VOID
  )
{
  EFI_STATUS  EfiStatus;
  UINT8       PorSocketConfig;

  EfiStatus = GetPorSocketConfig (&PorSocketConfig);

  switch (PorSocketConfig) {
    case SOCKET_1S:
      RcDebugPrint (SDBG_DEFAULT, " - Socket 1S\n");
      break;
    case SOCKET_1SWS:
      RcDebugPrint (SDBG_DEFAULT, " - Socket 1SWS\n");
      break;
    case SOCKET_HEDT:
      RcDebugPrint (SDBG_DEFAULT, " - Socket HEDT\n");
      break;
    case SOCKET_2S:
      RcDebugPrint (SDBG_DEFAULT, " - Socket 2S\n");
      break;
    case SOCKET_4S:
      RcDebugPrint (SDBG_DEFAULT, " - Socket 4S\n");
      break;
    case SOCKET_8S:
      RcDebugPrint (SDBG_DEFAULT, " - Socket 8S\n");
      break;
    default:
      RcDebugPrint (SDBG_DEFAULT, "Invalid POR socket config: %d\n", PorSocketConfig);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_156);
      EfiStatus = EFI_UNSUPPORTED;
      break;
  }

  return EfiStatus;
}
