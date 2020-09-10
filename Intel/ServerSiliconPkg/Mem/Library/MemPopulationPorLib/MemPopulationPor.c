/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseMemoryLib.h>
#include <Library/MemPopulationPorLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryServicesLib.h>

/**

Check current memory population against the Memory POR table

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EnforcePopulationPor (
  PSYSHOST Host
  )
{
  UINT8                   Socket;
  UINT8                   Die;
  UINT8                   Channel;
  UINT8                   Dimm;
  UINT8                   Rank;
  UINT8                   ByteIndex;
  UINT8                   BitIndex;
  UINT8                   DcpmmBitmap8;
  UINT8                   DdrBitmap8;
  UINT8                   LocalDcpmmBitmap8[MAX_DIE][MPOR_BITMAPS_8];
  UINT8                   LocalDdrBitmap8[MAX_DIE][MPOR_BITMAPS_8];
  UINT8                   DcpmmMapoutBitmap8[MAX_DIE][MPOR_BITMAPS_8];
  UINT8                   DdrMapoutBitmap8[MAX_DIE][MPOR_BITMAPS_8];
  UINT16                  PopIndex;
  UINT16                  DdrCapacity = 0;
  UINT16                  DcpmmCapacity = 0;
  BOOLEAN                 DimmEnabled;
  BOOLEAN                 DdrCapacityMismatch;
  BOOLEAN                 DcpmmCapacityMismatch;
  BOOLEAN                 PopulationMatch;
  BOOLEAN                 PopulationEnforcementMismatch;
  BOOLEAN                 DimmsOnSocket;
  DIMM_DEVICE_INFO_STRUCT (*ChDimmList)[MAX_DIMM];
  DIMM_NVRAM_STRUCT       (*DimmNvList)[MAX_DIMM];
  SYS_SETUP               *Setup;
  MPOR_TBL                *MemPorTable = NULL;

  Setup = GetSysSetupPointer ();

  if ((Setup->mem.EnforcePopulationPor == ENFORCE_POPULATION_POR_DIS) || (Setup->mem.enforcePOR == ENFORCE_POR_DIS)) {
    RcDebugPrint (SDBG_MAX, "EnforcePopulationPor - functionality disabled\n");
    return SUCCESS;
  }

  MemPorTable = GetPorTablePtr(Host);

  if ((MemPorTable == NULL) || (MemPorTable->Signature != MPOR_SIGNATURE) || (MemPorTable->Revision != MPOR_REVISION)
      || (MemPorTable->DieCount > MAX_DIE) || (MemPorTable->DieCount == 0) || (MemPorTable->Channels != MAX_CH)
      || (MemPorTable->Slots != MAX_DIMM) || (MemPorTable->Populations > MAX_MPOR_POP)) {
    RcDebugPrint (SDBG_MINMAX, "EnforcePopulationPor - MPOR Table is invalid\n");
    EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MEM_POP_POR_TBL_INVALID, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
    return FAILURE;
  } else if (MemPorTable->Populations == 0) {
    RcDebugPrint (SDBG_MINMAX, "EnforcePopulationPor - MPOR Table is empty\n");
    return SUCCESS;
  }

  RcDebugPrint (SDBG_MAX, "EnforcePopulationPor Start\n");

  //
  // Look for a matching DIMM configuration on each socket
  //
  for (Socket = 0; Socket < (MAX_SOCKET / MemPorTable->DieCount); Socket++) {

    if (!CheckSocketEnabled (Socket)) {
      continue;
    }

    //
    // Initialize local variables for each socket
    //
    if (Setup->mem.EnforcePopulationPor == ENFORCE_POPULATION_POR_ENFORCE_SUPPORTED) {
      //
      // For Enforce Supported Populations, reset DIMM capacities for each socket
      //
      DdrCapacity = 0;
      DcpmmCapacity = 0;
    }
    DdrCapacityMismatch = FALSE;
    DcpmmCapacityMismatch = FALSE;
    PopulationEnforcementMismatch = FALSE;
    DimmsOnSocket = FALSE;
    ZeroMem (&LocalDcpmmBitmap8, sizeof (LocalDcpmmBitmap8));
    ZeroMem (&LocalDdrBitmap8, sizeof (LocalDdrBitmap8));
    ZeroMem (&DcpmmMapoutBitmap8, sizeof (DcpmmMapoutBitmap8));
    ZeroMem (&DdrMapoutBitmap8, sizeof (DdrMapoutBitmap8));

    for (Die = 0; Die < MemPorTable->DieCount; Die++) {
      //
      // Fill in local bitmap and check DIMM capacity symmetry for this socket
      //
      for (Channel = 0; Channel < MAX_CH; Channel++) {
        DimmNvList = GetDimmNvList (Host, (Socket * MemPorTable->DieCount) + Die, Channel);
        ChDimmList = &Host->var.mem.socket[(Socket * MemPorTable->DieCount) + Die].channelList[Channel].dimmList;
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          //
          // Check if DIMM is present and not mapped out or flagged as unmapped
          //
          if ((*DimmNvList)[Dimm].dimmPresent == 0) {
            continue;
          }
          DimmEnabled = FALSE;
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numDramRanks; Rank++) {
            if ((*DimmNvList)[Dimm].mapOut[Rank] == 0) {
              DimmEnabled = TRUE;
              break;
            }
          }
          if (DimmEnabled == FALSE) {
            continue;
          }
        if (((*DimmNvList)[Dimm].DcpmmPresent) &&
              (Host->var.mem.socket[(Socket * MemPorTable->DieCount) + Die].channelList[Channel].DdrtDimmList[0].NvdimmUnmapped == 1)) {
            continue;
          }

          DimmsOnSocket = TRUE;

          //
          // Update bitmaps and check for capacity mismatch
          //
          ByteIndex = ((Channel * MAX_DIMM) + Dimm) / 8;
          BitIndex = ((Channel * MAX_DIMM) + Dimm) % 8;
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
            LocalDcpmmBitmap8[Die][ByteIndex] |= (1 << BitIndex);
            if (DcpmmCapacity == 0) {
              DcpmmCapacity = (*DimmNvList)[Dimm].rawCap;
            } else if (DcpmmCapacity != (*DimmNvList)[Dimm].rawCap) {
              DcpmmCapacityMismatch = TRUE;
            }
          } else {
            LocalDdrBitmap8[Die][ByteIndex] |= (1 << BitIndex);
            if (DdrCapacity == 0) {
              DdrCapacity = (*ChDimmList)[Dimm].memSize;
            } else if (DdrCapacity != (*ChDimmList)[Dimm].memSize) {
              DdrCapacityMismatch = TRUE;
            }
          }
        }
      }
    }

    if (DimmsOnSocket == FALSE) {
      if (GetSysSbspSocketIdNv () == Socket) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "No DIMM on BSP Socket\n");
        FatalError (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_NO_DIMM_ON_SOCKET);
      } else {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "No DIMM on AP Socket\n");
        EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_NO_DIMM_ON_SOCKET, Socket, NO_CH, NO_DIMM, NO_RANK);
      }
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "System bitmaps:");
    for (Die = 0; Die < MemPorTable->DieCount; Die++) {
      for (ByteIndex = 0; ByteIndex < MPOR_BITMAPS_8; ByteIndex++) {
        RcDebugPrint (SDBG_MAX, " Ddr[%x][%x] = %x, PMem[%x][%x] = %x", Die, ByteIndex, LocalDdrBitmap8[Die][ByteIndex],
          Die, ByteIndex, LocalDcpmmBitmap8[Die][ByteIndex]);
      }
    }
    RcDebugPrint (SDBG_MAX, "\n");

    //
    // Search for best match configuration in the Memory POR Table starting at the most complex
    //
    PopIndex = MemPorTable->Populations;
    while (PopIndex != 0) {
      PopIndex -= MemPorTable->DieCount;
      if ((Setup->mem.EnforcePopulationPor == ENFORCE_POPULATION_POR_ENFORCE_VALIDATED)
          && (MemPorTable->Population[PopIndex].Flags.Validated == 0)) {
        // Only check validated configurations
        PopulationMatch = FALSE;
        continue;
      }
      PopulationMatch = TRUE;
      for (Die = 0; Die < MemPorTable->DieCount; Die++) {
        for (ByteIndex = 0; ByteIndex < MPOR_BITMAPS_8; ByteIndex++) {
          DcpmmBitmap8 = LocalDcpmmBitmap8[Die][ByteIndex] & MemPorTable->Population[PopIndex + Die].DcpmmBitmap8[ByteIndex];
          DdrBitmap8 = LocalDdrBitmap8[Die][ByteIndex] & MemPorTable->Population[PopIndex + Die].DdrBitmap8[ByteIndex];
          if ((DdrBitmap8 != MemPorTable->Population[PopIndex + Die].DdrBitmap8[ByteIndex])
              || (DcpmmBitmap8 != MemPorTable->Population[PopIndex + Die].DcpmmBitmap8[ByteIndex])) {
            PopulationMatch = FALSE;
            break;
          }
          DcpmmMapoutBitmap8[Die][ByteIndex] = LocalDcpmmBitmap8[Die][ByteIndex] ^ MemPorTable->Population[PopIndex + Die].DcpmmBitmap8[ByteIndex];
          DdrMapoutBitmap8[Die][ByteIndex] = LocalDdrBitmap8[Die][ByteIndex] ^ MemPorTable->Population[PopIndex + Die].DdrBitmap8[ByteIndex];
        }
        if (PopulationMatch == FALSE) {
          break;
        }
      }
      if (PopulationMatch) {
        break;
      }
    }

    //
    // Check to ensure a match was found
    //
    if (PopulationMatch == FALSE) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "ERROR No match found for this socket!\n");
      EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_POPULATION_POR_NOT_FOUND, Socket, NO_CH, NO_DIMM, NO_RANK);
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Population %d matches\n", PopIndex/MemPorTable->DieCount);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Population bitmaps:");
    for (Die = 0; Die < MemPorTable->DieCount; Die++) {
      for (ByteIndex = 0; ByteIndex < MPOR_BITMAPS_8; ByteIndex++) {
        RcDebugPrint (SDBG_MAX, " Ddr[%x][%x] = %x, PMem[%x][%x] = %x", Die, ByteIndex, MemPorTable->Population[PopIndex + Die].DdrBitmap8[ByteIndex],
          Die, ByteIndex, MemPorTable->Population[PopIndex + Die].DcpmmBitmap8[ByteIndex]);
      }
    }
    RcDebugPrint (SDBG_MAX, "\n");

    //
    // Map out DIMMs as necessary to make local bitmap match the table entry
    //
    for (Die = 0; Die < MemPorTable->DieCount; Die++) {
      for (ByteIndex = 0; ByteIndex < MPOR_BITMAPS_8; ByteIndex++) {
        if (DcpmmMapoutBitmap8[Die][ByteIndex] != 0) {
          //
          // Set DCPMM Unmapped indicator for each DIMM in the bitmap
          //
          for (BitIndex = 0; BitIndex < 8; BitIndex++) {
            if (DcpmmMapoutBitmap8[Die][ByteIndex] & (1 << BitIndex)) {
              Channel = ((ByteIndex * 8) + BitIndex) / MAX_DIMM;
              Dimm = ((ByteIndex * 8) + BitIndex) % MAX_DIMM;
              if (Channel < MAX_CH) {
                RcDebugPrintWithDevice (SDBG_MINMAX, (Socket * MemPorTable->DieCount) + Die, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Mapping out PMem\n");
                Host->var.mem.socket[(Socket * MemPorTable->DieCount) + Die].channelList[Channel].DdrtDimmList[0].NvdimmUnmapped = 1;
                Host->var.mem.socket[(Socket * MemPorTable->DieCount) + Die].channelList[Channel].DdrtDimmList[0].NvdimmUnmappedReason = NVDIMM_UNMAPPED_POPULATION_NOT_POR;
              }
            }
          }
          PopulationEnforcementMismatch = TRUE;
        }

        if (DdrMapoutBitmap8[Die][ByteIndex] != 0) {
          //
          // Map out each DIMM in the bitmap
          //
          for (BitIndex = 0; BitIndex < 8; BitIndex++) {
            if (DdrMapoutBitmap8[Die][ByteIndex] & (1 << BitIndex)) {
              Channel = ((ByteIndex * 8) + BitIndex) / MAX_DIMM;
              Dimm = ((ByteIndex * 8) + BitIndex) % MAX_DIMM;
              if (Channel < MAX_CH) {
                RcDebugPrintWithDevice (SDBG_MINMAX, (Socket * MemPorTable->DieCount) + Die, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Mapping out DDR\n");
                if (Dimm < MAX_DIMM) {
                MapOutDimm (Host, (Socket * MemPorTable->DieCount) + Die, Channel, Dimm, DIMM_RANK_MAP_OUT_POP_POR_VIOLATION);
                }
              }
            }
          }
          PopulationEnforcementMismatch = TRUE;
        }
      }
    }

    //
    // Check for mismatches
    //
    if (DcpmmCapacityMismatch) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "PMem capacity mismatch on Socket\n");
      EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDRT_CAPACITY_MISMATCH, Socket, NO_CH, NO_DIMM, NO_RANK);
    }
    if (DdrCapacityMismatch) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "DDR capacity mismatch on Socket\n");
      EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDR_CAPACITY_MISMATCH, Socket, NO_CH, NO_DIMM, NO_RANK);
    }
    if (PopulationEnforcementMismatch) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Memory population enforcement mismatch on Socket\n");
      EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_POPULATION_POR_MISMATCH, Socket, NO_CH, NO_DIMM, NO_RANK);
    }
  }

  RcDebugPrint (SDBG_MAX, "EnforcePopulationPor End\n");

  return SUCCESS;
}
