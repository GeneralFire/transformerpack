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

#include "DdrOdtPrivate.h"
#include <Library/OdtTableLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#ifdef DDR5_SUPPORT
#include "Ddr5BacksideOdtValueTable.h"
#include <Library/RcMemAllocLib.h>
#endif
//
// ODT programming
//

#define  RD_TARGET      0
#define  WR_TARGET      1

// The DDR5 QCA/QCS VREF range is encoded such that the value is 2 * (97.5% - target percent).
// 2 * 97.5 = 195.
#define QCS_QCA_VREF_TOP_RANGE_TIMES_TWO 195

//Default QCS Vref Value is 76.5% * 2 = 133
#define QCS_VREF_INITIAL_DEFAULT_VALUE_TIMES_TWO 133

//Default QCA Vref Value is 63% * 2 = 126

#define QCA_VREF_INITIAL_DEFAULT_VALUE_TIMES_TWO 126

//Default QCA ODT Value
// Encoding matches ODT_BACKSIDE_POSITION_TABLE_VALUE with
// P1/2 = Off, P3/4 = 480 Ohm, P5 = 80 Ohm

#define QCA_ODT_INITIAL_DEFAULT_VALUE (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 6)  |  (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12)

//Default QCK ODT Value
// Encoding matches ODT_BACKSIDE_POSITION_TABLE_VALUE with
// P1/2/3/4 = Off, P5 = 60 Ohm

#define QCK_ODT_INITIAL_DEFAULT_VALUE (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12)

//Default QCS ODT Value
// Encoding matches ODT_BACKSIDE_POSITION_TABLE_VALUE with
// P1/2/3/4 = Off, P5 = 40 Ohm

#define QCS_ODT_INITIAL_DEFAULT_VALUE (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12)

//Default RCD QxCK R-On ODT Value

#define RCD_QCK_RON_ODT_INITIAL_DEFAULT_VALUE DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS

//Default RCD QCA R-On ODT Value

#define RCD_QCA_RON_ODT_INITIAL_DEFAULT_VALUE DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS

//Default RCD QCA R-On ODT Value

#define RCD_QCS_RON_ODT_INITIAL_DEFAULT_VALUE DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS

/**
  Get the current DIMM frequency and population, formatted as an index for the ODT value table

  @param [IN] Socket    - Socket number
  @param [IN] Ch        - Channel number (0-based)
  @param [IN] FreqLimit - Frequency Limit

  @retval N/A
**/
ODT_VALUE_TABLE_ENTRY_INDEX
GetCurrentDimmFrequencyAndPopulationOdt (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 FreqLimit
  )
{
  PSYSHOST                     Host;
  struct dimmNvram             (*dimmNvList)[MAX_DIMM];
  ODT_VALUE_TABLE_ENTRY_INDEX  CurrentDimmFrequencyAndPopulation;
  UINT8                        Dimm;
  UINT8                        DimmTypeInSlot = 0;

  Host = GetSysHostPointer ();
  dimmNvList = GetDimmNvList (Host, Socket, Ch);
  CurrentDimmFrequencyAndPopulation.Data = GetCurrentDimmFrequencyOdt (Socket);

  if (CurrentDimmFrequencyAndPopulation.Bits.freq > FreqLimit) {
    CurrentDimmFrequencyAndPopulation.Bits.freq = FreqLimit;
  }
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    if ((*dimmNvList)[Dimm].dimmPresent) {

      if (IsLrdimmPresent (Socket, Ch, Dimm)){
        DimmTypeInSlot = LR_DIMM;
      } else {
        switch ((*dimmNvList)[Dimm].numRanks) {
        case 1:
          DimmTypeInSlot = SR_DIMM;
          break;
        case 2:
          DimmTypeInSlot = DR_DIMM;
          break;
        case 4:
          DimmTypeInSlot = QR_DIMM;
          break;
        default:
          // error
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_80);
          } // end switch
        } // if LRDIMM
    } else DimmTypeInSlot = EMPTY_DIMM;

    switch (Dimm) {
    case 0:
      CurrentDimmFrequencyAndPopulation.Bits.slot0 = DimmTypeInSlot;
      break;
    case 1:
      CurrentDimmFrequencyAndPopulation.Bits.slot1 = DimmTypeInSlot;
      break;
    case 2:
      CurrentDimmFrequencyAndPopulation.Bits.slot2 = DimmTypeInSlot;
      break;
    default:
      // error
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_81);
      } // end switch
   } // for dimm
   return CurrentDimmFrequencyAndPopulation;
} // GetCurrentDimmFrequencyAndPopulationOdt

/**
  Searches the ODT Value table for an entry that matches the current DIMM population

  @param [IN] Socket - Socket number
  @param [IN] Ch     - Channel number

  @retval Returns a pointer to the matching table entry (which holds the ODT impedance values), or NULL if no match is found
**/
struct ddrOdtValueTableEntry*
GetValuesForCurrentConfigOdt (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  ODT_VALUE_TABLE_ENTRY_INDEX  CurrentDimmFrequencyAndPopulation;
  UINT32                       EntryNumber;
  UINT16                       ddrOdtValueTableSize;
  struct ddrOdtValueTableEntry *ddrOdtValueTableEntryPtr;
  UINT8                         DdrMaxFrequency = 0;

  //Get Maximum frequency supported
  GetMaxFreqData (Socket, &DdrMaxFrequency);

  RcDebugPrint (SDBG_MAX, "\nDdrMaxFrequency = %d\n", DdrMaxFrequency);

  GetValuesTableOdt (&ddrOdtValueTableEntryPtr, &ddrOdtValueTableSize);

  // Determine the current system config (the DIMM population and frequency)
  CurrentDimmFrequencyAndPopulation = GetCurrentDimmFrequencyAndPopulationOdt (Socket, Ch, DdrMaxFrequency);

  // Search for the matching "entry" on the Value table
  for (EntryNumber = 0; EntryNumber < ddrOdtValueTableSize; EntryNumber++) {
    if (ddrOdtValueTableEntryPtr->freqPopData == CurrentDimmFrequencyAndPopulation.Data) {
      break;
    }
    ddrOdtValueTableEntryPtr++;
  }
  // Check for error
  if (EntryNumber >= ddrOdtValueTableSize) {
    OutputWarning (ERR_MRC_STRUCT, ERR_ODT_STRUCT, Socket, Ch, NO_DIMM, NO_RANK);
    return NULL;
  }

  // Return pointer to the matching entry
  return ddrOdtValueTableEntryPtr;
} // GetValuesForCurrentConfigOdt


#ifdef DDR5_SUPPORT

/**
  Get the current DIMM frequency formatted as an "index" for the ODT value table
  (other fields are set to 0)

  @param [IN] Socket  - Socket number

  @retval The current DIMM frequency formatted as an "index" for the ODT value table
  (other fields are set to 0)
**/
ODT_BACKSIDE_VALUE_TABLE_ENTRY_INDEX
GetCurrentDimmFrequencyBacksideOdt (
  IN UINT8 Socket
  )
{
  PSYSHOST                    Host;
  ODT_BACKSIDE_VALUE_TABLE_ENTRY_INDEX CurrentDimmFrequency;

  Host = GetSysHostPointer ();
  // Construct config index
  CurrentDimmFrequency.Data = 0;
  CurrentDimmFrequency.Bits.Frequency = GetClosestFreq (Host, Socket);

  // Cap frequencies above DDR_4800
  if (CurrentDimmFrequency.Bits.Frequency > DDR_4800) {
    CurrentDimmFrequency.Bits.Frequency = DDR_4800;
  }

  // Adjust frequencies below DDR_4000
  if (CurrentDimmFrequency.Bits.Frequency < DDR_4000) {
    CurrentDimmFrequency.Bits.Frequency = DDR_4000;
  }

  return CurrentDimmFrequency;
} // GetCurrentDimmFrequencyOdt

/**
  Get the current DIMM frequency and Raw Card Type formatted as an index for the
  Backside ODT value table

  @param [IN] Socket    - Socket number
  @param [IN] Ch        - Channel number (0-based)
  @param [IN] Dimm      - Dimm number
  @param [IN] FreqLimit - Frequency Limit

  @retval N/A
**/
ODT_BACKSIDE_VALUE_TABLE_ENTRY_INDEX
GetCurrentDimmFrequencyAndRawCardOdt (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 FreqLimit
  )
{
  PSYSHOST                              Host;
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];
  ODT_BACKSIDE_VALUE_TABLE_ENTRY_INDEX  CurrentDimmFrequencyAndRawCard;

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  CurrentDimmFrequencyAndRawCard = GetCurrentDimmFrequencyBacksideOdt (Socket);

  if (CurrentDimmFrequencyAndRawCard.Bits.Frequency > FreqLimit) {
    CurrentDimmFrequencyAndRawCard.Bits.Frequency = FreqLimit;
  }

  //Only LRDIMM and RDIMM types are supported, so if it's not LRDIMM, it's assumed to be RDIMM
  if (IsLrdimmPresent (Socket, Ch, Dimm)){
    CurrentDimmFrequencyAndRawCard.Bits.DimmType = LRDIMM;
  } else {
    CurrentDimmFrequencyAndRawCard.Bits.DimmType = RDIMM;
  }
  CurrentDimmFrequencyAndRawCard.Bits.RawCard = ((*DimmNvList)[Dimm].SPDRawCard & 0x1F);
  CurrentDimmFrequencyAndRawCard.Bits.RawCardRev = ((*DimmNvList)[Dimm].SPDRawCard >> 5);

  return CurrentDimmFrequencyAndRawCard;
} // GetCurrentDimmFrequencyAndRawCardOdt

/**
  Searches the Backside ODT Value table for an entry that matches the current DIMM and Vref Type

  @param [IN] Socket   - Socket number
  @param [IN] Ch       - Channel number
  @param [IN] Dimm     - Dimm number
  @param [IN] VrefType - Enum value BACKSIDE_VREF_TYPE of the desired reference voltage type

  @retval Returns a pointer to the matching table entry (which holds the ODT impedance and vref values), or NULL if no match is found
**/
DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY*
GetBacksideValuesForCurrentConfigOdt (
  IN UINT8              Socket,
  IN UINT8              Ch,
  IN UINT8              Dimm,
  IN BACKSIDE_VREF_TYPE VrefType
  )
{
  ODT_BACKSIDE_VALUE_TABLE_ENTRY_INDEX  CurrentDimmFrequencyAndRawCard;
  UINT32                                EntryNumber;
  UINT16                                DdrOdtValueTableSize;
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY    *DdrOdtValueTableEntryPtr;
  UINT8                                 DdrMaxFrequency = 0;

  //Get Maximum frequency supported
  GetMaxFreqData (Socket, &DdrMaxFrequency);

  GetBacksideValuesTableOdt (&DdrOdtValueTableEntryPtr, &DdrOdtValueTableSize);

  // Determine the current system config (the DIMM raw card and frequency)

  CurrentDimmFrequencyAndRawCard = GetCurrentDimmFrequencyAndRawCardOdt (Socket, Ch, Dimm, DdrMaxFrequency);
  CurrentDimmFrequencyAndRawCard.Bits.VrefType = VrefType;

  // Search for the matching "entry" on the Value table

  for (EntryNumber = 0; EntryNumber < DdrOdtValueTableSize; EntryNumber++) {
    if (DdrOdtValueTableEntryPtr->FreqRawCardAndVrefTypeData == CurrentDimmFrequencyAndRawCard.Data) {
      break;
    }
    DdrOdtValueTableEntryPtr++;
  }

  // Check for error

  if (EntryNumber >= DdrOdtValueTableSize) {
    RcDebugPrintWithDevice (SDBG_WARN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "DIMM Not Found In Backside ODT Table, using default\n");
    return NULL;
  }

  // Return pointer to the matching entry
  return DdrOdtValueTableEntryPtr;
} // GetBacksideValuesForCurrentConfigOdt
#endif

/**
  Searches the ODT Activation table for an entry that matches the current DIMM population

  @param [IN] Socket - Socket number
  @param [IN] Ch     - Channel number
  @param [IN] Dimm   - DIMM number
  @param [IN] Rank   - Rank number

  @retval Returns a pointer to the matching table entry (which holds the ODT activation values), or NULL if no match is found
**/
struct ddrOdtActivationTableEntry*
GetActivationForCurrentTargetDimmRankAndPopulationOdt (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Rank
  )
{
  PSYSHOST                           Host;
  ODT_ACTIVATION_TABLE_ENTRY_INDEX   TargetDimmRankAndPopulation;
  ODT_VALUE_TABLE_ENTRY_INDEX        DimmFrequencyAndPopulation;
  UINT8                              Dimms;
  UINT8                              DimmTypeInSlot = 0;
  UINT32                             EntryNumber;
  UINT32                             OdtActTableSize;
  struct ddrOdtActivationTableEntry  *OdtActTablePtr = NULL;
  struct dimmNvram                   (*DimmNvList)[MAX_DIMM];
  BOOLEAN                            UbiosOrEmu;
  BOOLEAN                            LrdimmPresentCurrentCh = (IsLrdimmPresent (Socket, Ch, 0) || IsLrdimmPresent (Socket, Ch, 1));

  UbiosOrEmu = (UbiosGenerationOrHsleEnabled () || (FeaturePcdGet (PcdCteBuild) == TRUE));

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  GetActivationTableOdt (&OdtActTablePtr, &OdtActTableSize);

  DimmFrequencyAndPopulation.Data = GetCurrentDimmFrequencyOdt (Socket);
  TargetDimmRankAndPopulation.Data = 0;
  // Copy the DIMM population values (current system config) so we know what entry we'll search for on the Activation table
  TargetDimmRankAndPopulation.Bits.slot0 = DimmFrequencyAndPopulation.Bits.slot0;
  TargetDimmRankAndPopulation.Bits.slot1 = DimmFrequencyAndPopulation.Bits.slot1;
  TargetDimmRankAndPopulation.Bits.slot2 = DimmFrequencyAndPopulation.Bits.slot2;
  TargetDimmRankAndPopulation.Bits.dimmNum = Dimm;
  TargetDimmRankAndPopulation.Bits.rankNum = Rank;
  for (Dimms = 0; Dimms < MAX_DIMM; Dimms++) {

    if ((*DimmNvList)[Dimms].dimmPresent) {

      if ((UbiosOrEmu && LrdimmPresentCurrentCh) || (!UbiosOrEmu && IsLrdimmPresent (Socket, Ch, Dimms))) {
        DimmTypeInSlot = GetSlotTypeForActivationLookupOdt ((*DimmNvList)[Dimms].DcpmmPresent);
      } else {
        switch ((*DimmNvList)[Dimms].numRanks) {
        case 1:
          DimmTypeInSlot = SR_DIMM;
          break;
        case 2:
          DimmTypeInSlot = DR_DIMM;
          break;
        case 4:
          DimmTypeInSlot = QR_DIMM;
          break;
        default:
          // error
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_82);
        } // end switch
      }
    } else { //((*dimmNvList)[d].dimmPresent)
      DimmTypeInSlot = EMPTY_DIMM;
    }

    switch (Dimms) {
    case 0:
      TargetDimmRankAndPopulation.Bits.slot0 = DimmTypeInSlot;
      break;
    case 1:
      TargetDimmRankAndPopulation.Bits.slot1 = DimmTypeInSlot;
      break;
    case 2:
      TargetDimmRankAndPopulation.Bits.slot2 = DimmTypeInSlot;
      break;
    default:
      // error
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_83);
    }
  }

  // Search for the matching "entry" on the Activation table
  for (EntryNumber = 0; EntryNumber < OdtActTableSize ; EntryNumber++) {
    if ((OdtActTablePtr + EntryNumber)->tgtDimmRankPopData == TargetDimmRankAndPopulation.Data) {
      break;
    }
  }
  // Check for error
  if (EntryNumber >= OdtActTableSize) {
    OutputError (ERR_DIMM_COMPAT, ERR_INVALID_POP, Socket, Ch, Dimm, Rank);
    return NULL;
  }

  // Return pointer to the matching entry
  return (OdtActTablePtr + EntryNumber);
} // GetActivationForCurrentTargetDimmRankAndPopulationOdt

/**
  Set MC ODT values in Ohms (0-255)

  @param [IN] Socket - Socket number
  @param [IN] Ch     - Channel number (0-based)
  @param [IN] Mcodt  - MC ODT values in Ohms (0-255)

  @retval N/A
**/
VOID
SetMcOdtValue (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Mcodt
  )
{

} // SetMcOdtValue

/*++

  Returns the  MC ODT values in Ohms (0-255)

  @param Socket - Socket number
  @param Ch     - Channel number (0-based)
  @param Mcodt  - MC ODT values in Ohms (0-255)

  @retval The MCODT value between 0 and 255 ohms

--*/
UINT8
GetMcOdtValue (
  UINT8  Socket,
  UINT8  Ch,
  UINT16 *Mcodt
  )
{
  PSYSHOST                      Host;
  UINT8                         McOdtOhms = 0;
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr = NULL;

  Host = GetSysHostPointer ();
  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);
  // Lookup ODT values for this config
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR4 || Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);
    if (DdrOdtValuesPtr == NULL) {
      DisableChannelSw (Host, Socket, Ch);
    } else {
      McOdtOhms = DdrOdtValuesPtr->mcOdt;
    }// end if null ptr
  } // if dramType ==

  if (Mcodt != NULL && DdrOdtValuesPtr != NULL) {
    *Mcodt = DdrOdtValuesPtr->mcOdt;
  } // if not NULL

  return (McOdtOhms);
} // GetMcOdtValue

/*++

  Looks for the MC-RON value for Socket in the ODT table.
  If found, it returns a value in Ohms (0-255).

  @param [IN    ] Socket - Socket number
  @param [IN    ] Ch     - Channel number (0-based)
  @param [   OUT] McRon  - MC-RON value in Ohms (0-255)

  @retval EFI_UNSUPPORTED if called from a BRS build
  @retval EFI_NOT_FOUND if McRon for the current config is not found
  @retval EFI_SUCCESS if McRon for the current config is found

--*/
EFI_STATUS
EFIAPI
GetMcRonValue (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
     OUT UINT16 *McRon
  )
{
#ifdef DDR5_SUPPORT
                                               // DR5_TODO: Consider adding the CpuRon field to DDR5 OdtTableLib instances,
                                               // or re-arrange struct definitions and logic differently for all OdtTableLib instances
return EFI_UNSUPPORTED;
#else
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);
  if (DdrOdtValuesPtr == NULL) {
    McRon = NULL;
    return EFI_NOT_FOUND;
  } else {
    *McRon = DdrOdtValuesPtr->CpuRon;
    return EFI_SUCCESS;
  }
#endif
} // GetMcRonValue

/**

  This function copies the DRAM RTT & Activation values, to RankNvList (i.e. to the Host structure)
  These values are later programmed to the DRAMs by the "JEDEC init" functions.
  This function also copies backside Vref and ODT values for LR-DIMMs to ChannelNvList (again, to Host)

  Emphasis: the above means that a subset of ODT values can then be used by **any piece of code** that fetches a pointer
  to RankNvList or ChannelNvList (indeed, there are pieces of code that do ODT programming without calling a single OdtTableLib API)

  Initial values for many other ODT parameters (like McOdt) are programmed by the MMRC (BaseMspChipLib), not by this function.

  The set of ODT Impedance and Activation values for multiple system configurations
  (that is, for multiple DIMM populations and frequencies) are sometimes collectively called "the ODT Matrix"

  @param [IN] Socket - Socket number
  @param [IN] Ch     - Channel number (0-based)

  @retval N/A

**/
VOID
BufferDramAndLrdimmBacksideOdtToHost (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  PSYSHOST                           Host;
  struct ddrRank                     (*RankNvList)[MAX_RANK_DIMM];
  UINT8                              Dimm;
  UINT8                              Rank;
  UINT8                              ImcRank;
  UINT8                              Dimms;
  UINT8                              Ranks;
  UINT8                              OdtPin;
  UINT32                             RdOdtTableData;
  UINT32                             WrOdtTableData;
  struct dimmNvram                   (*DimmNvList)[MAX_DIMM];
  struct ddrRank                     (*RankList)[MAX_RANK_DIMM];
  struct channelNvram                (*ChannelNvList)[MAX_CH];
  struct ddrOdtActivationTableEntry  *ActivationTableEntryPtr;
  struct ddrOdtValueTableEntry       *ValueTableEntryPtr = NULL;
  ODT_VALUE_TABLE_ENTRY_RTT          DramRttEntryData;
#ifdef DDR5_SUPPORT
  UINT8                              Ddr5RttLookup[] = {0, 240, 120, 80, 60, 48, 40, 34};
#else // DDR4 RTT
  UINT8                              Ddr4RttWrLookup[] = {0, 120, 240, 0, 80, 60};
  UINT8                              Ddr4RttNomLookup[] = {0, 60, 120, 40, 240, 48, 80, 34};
#endif // END DDR RTT

#ifdef DEBUG_CODE_BLOCK
  // Debug only!
  /*
  if (GetDebugLevel () & SDBG_MAX) {
    RcDebugPrint (SDBG_DEFAULT, "\n");
    RcDebugPrint (SDBG_DEFAULT, "Sizeof odtValueStruct: %d\n", sizeof(struct odtValueStruct));
    RcDebugPrint (SDBG_DEFAULT, "Sizeof odtValueTable: %d\n", sizeof(odtValueTable));
    RcDebugPrint (SDBG_DEFAULT, "Calculated num entries: %d\n", sizeof(odtValueTable) / sizeof(struct odtValueStruct));

    RcDebugPrint (SDBG_DEFAULT, "\n");
    RcDebugPrint (SDBG_DEFAULT, "Sizeof ddrOdtActivationTableEntry: %d\n", sizeof(struct ddrOdtActivationTableEntry));
    RcDebugPrint (SDBG_DEFAULT, "Sizeof odtActTable: %d\n", sizeof(odtActTable));
    RcDebugPrint (SDBG_DEFAULT, "Calculated num entries: %d\n", sizeof(odtActTable) / sizeof(struct ddrOdtActivationTableEntry));
  }
  */
#endif // DEBUG_CODE_BLOCK

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList    = GetDimmNvList (Host, Socket, Ch);

  //
  // Loop for each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Skip if no DIMM present
    //
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    RdOdtTableData  = 0;
    WrOdtTableData  = 0;

    //
    // Loop for each rank
    //
    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      //
      // Skip if no rank
      //
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      // Lookup ODT values for this config
      ValueTableEntryPtr = GetValuesForCurrentConfigOdt (Socket, Ch);
      if (ValueTableEntryPtr == NULL) {
        DisableChannelSw (Host, Socket, Ch);
        continue;
      } else {
        // Set MCODT values for this channel
        // SetMcOdtValue (Socket, Ch, ValueTableEntryPtr->mcOdt); // This "call" to SetMcOdtValue() was an empty lie
                                                                  // Initial values for many ODT parameters (like McOdt)
                                                                  // are programmed by the MMRC (BaseMspChipLib), not by this function.
                                                                  //
                                                                  // Further, some of the ODT values initially programmed
                                                                  // by the MMRC are re-trained at a later point.
      }// end if null ptr

      // Get the ODT values
      DramRttEntryData = *(ODT_VALUE_TABLE_ENTRY_RTT *)&ValueTableEntryPtr->dramRttData[Dimm][Rank];

#ifdef  LRDIMM_SUPPORT
#ifndef DDR5_SUPPORT // TODO: Update this section for DDR5 LRDIMM
      if (IsLrdimmPresent (Socket, Ch, Dimm)) {
        if (Rank == 0) {
          //DDR4 LRDIMM, the ODT settings from the table are for the Host side ODT
          //The BIOS will only program these values in the NVRAM structure for this DIMM here,
          // the programming of the values is done during the JEDEC init.
          (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC00 = (UINT8) DramRttEntryData.Bits.rttNom;
          (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC02 = (UINT8) DramRttEntryData.Bits.rttPrk;
          switch (DramRttEntryData.Bits.rttWr) {
            case DDR4_RTT_WR_DIS:
              if ((*ChannelNvList)[Ch].maxDimm == 1) {
                (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC01 = DDR4_RTT_NOM_60;
                (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC00 = DDR4_RTT_NOM_60;
              } else {
                (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC01 = DDR4_RTT_NOM_DIS;
              }
              break;

            case DDR4_RTT_WR_60:
              (*DimmNvList)[Dimm].lrBuf_BC01 = DDR4_RTT_NOM_60;
              break;

            case DDR4_RTT_WR_80:
              (*DimmNvList)[Dimm].lrBuf_BC01 = DDR4_RTT_NOM_80;
              break;

            case DDR4_RTT_WR_120:
              (*DimmNvList)[Dimm].lrBuf_BC01 = DDR4_RTT_NOM_120;
              break;

            case DDR4_RTT_WR_240:
              (*DimmNvList)[Dimm].lrBuf_BC01 = DDR4_RTT_NOM_240;
              break;

            case DDR4_RTT_WR_INF:
              (*DimmNvList)[Dimm].lrBuf_BC01 = (BIT2 | BIT1 | BIT0);
              break;
              default:
                //Automated add of default case. Please review.
                break;
          }
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                          "LRDIMM Host Side Rtt_wr = %d, Rtt_park = %d, Rtt_nom = %d\n",
                          Ddr4RttNomLookup[(*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC01],
                          Ddr4RttNomLookup[DramRttEntryData.Bits.rttPrk],
                          Ddr4RttNomLookup[DramRttEntryData.Bits.rttNom]);
          //Host Interface DQ/DQS Output Driver Impedance Control, set to 34 Ohms  = value of 1.
          (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC03 = 1;

          if (((*DimmNvList)[Dimm].SPDSpecRev > 7) || ((*DimmNvList)[Dimm].DcpmmPresent)) { //4986147: BIOS not setting LR-DIMM backside Vref and ODT according to SPD values
            if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_1866)  {
              (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC04 = ((*DimmNvList)[Dimm].SPDLrbufDbDsRttLe1866 & (BIT2 + BIT1 + BIT0));
              (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC05 = (((*DimmNvList)[Dimm].SPDLrbufDbDsRttLe1866 >> 4) & (BIT2 + BIT1 + BIT0));
            } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2400) {
              (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC04 = ((*DimmNvList)[Dimm].SPDLrbufDbDsRttGt1866Le2400 & (BIT2 + BIT1 + BIT0));
              (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC05 = (((*DimmNvList)[Dimm].SPDLrbufDbDsRttGt1866Le2400 >> 4) & (BIT2 + BIT1 + BIT0));
            } else {
              (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC04 = ((*DimmNvList)[Dimm].SPDLrbufDbDsRttGt2400Le3200 & (BIT2 + BIT1 + BIT0));
              //
              // SPD Byte 1 >= 141 (0x8d) will have byte 147=0x12 (does not need BIOS WA)
              //
              if (((*DimmNvList)[Dimm].fmcType == FMC_BWV_TYPE) && ((*DimmNvList)[Dimm].SPDSpecRev < 141)) {
                (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC05 = BIT0;
              } else {
                (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC05 = (((*DimmNvList)[Dimm].SPDLrbufDbDsRttGt2400Le3200 >> 4) & (BIT2 + BIT1 + BIT0));
              }
            }
          } else {
            //Buffer ODT = 48 Ohms
            (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC04 = 5;
            // backside buffer driver impedance control = 34 Ohms = RZQ/7
            (*ChannelNvList)[Ch].dimmList[Dimm].lrBuf_BC05 = 1;
          } // if SPDSpecRev
        } // end switch

        if (((*DimmNvList)[Dimm].SPDSpecRev > 7) || ((*DimmNvList)[Dimm].DcpmmPresent)) { //4986147: BIOS not setting LR-DIMM backside Vref and ODT according to SPD values
          // For SPD Rev > 0.7, read the DRAM ODT values from SPD, otherwise used fixed values based on number of backside ranks
          if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_1866)  {
            (*RankList)[Rank].RttWr = (((*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomLe1866 & (BIT5 + BIT4 + BIT3)) << (DDR4_RTT_WR_ODT_SHIFT - 3));
            if (Rank > 1) {
              //ODT pin on ranks 2 and 3 is GND which makes RttNom = Disabled
              (*RankList)[Rank].RttNom = DDR4_RTT_NOM_DIS << DDR4_RTT_NOM_ODT_SHIFT;
              (*RankList)[Rank].RttPrk = (((*DimmNvList)[Dimm].SPDLrbufDramOdtParkLe1866 & (BIT5 + BIT4 + BIT3)) << (DDR4_RTT_PRK_ODT_SHIFT - 3));
            } else {
              (*RankList)[Rank].RttNom = (((*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomLe1866 & (BIT2 + BIT1 + BIT0)) << DDR4_RTT_NOM_ODT_SHIFT);
              (*RankList)[Rank].RttPrk = (((*DimmNvList)[Dimm].SPDLrbufDramOdtParkLe1866 & (BIT2 + BIT1 + BIT0)) << DDR4_RTT_PRK_ODT_SHIFT);
            } // if rank
          } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2400) {
            (*RankList)[Rank].RttWr = (((*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt1866Le2400 & (BIT5 + BIT4 + BIT3)) << (DDR4_RTT_WR_ODT_SHIFT - 3));
            if (Rank > 1) {
              //ODT pin on ranks 2 and 3 is GND which makes RttNom = Disabled
              (*RankList)[Rank].RttNom = DDR4_RTT_NOM_DIS << DDR4_RTT_NOM_ODT_SHIFT;
              (*RankList)[Rank].RttPrk = (((*DimmNvList)[Dimm].SPDLrbufDramOdtParkGt1866Le2400 & (BIT5 + BIT4 + BIT3)) << (DDR4_RTT_PRK_ODT_SHIFT - 3));
            } else {
              (*RankList)[Rank].RttNom = (((*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt1866Le2400 & (BIT2 + BIT1 + BIT0)) << DDR4_RTT_NOM_ODT_SHIFT);
              (*RankList)[Rank].RttPrk = (((*DimmNvList)[Dimm].SPDLrbufDramOdtParkGt1866Le2400 & (BIT2 + BIT1 + BIT0)) << DDR4_RTT_PRK_ODT_SHIFT);
            } // if rank
          } else {
            (*RankList)[Rank].RttWr = (((*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt2400Le3200 & (BIT5 + BIT4 + BIT3)) << (DDR4_RTT_WR_ODT_SHIFT - 3));
            if (Rank > 1) {
              //ODT pin on ranks 2 and 3 is GND which makes RttNom = Disabled
              (*RankList)[Rank].RttNom = DDR4_RTT_NOM_DIS << DDR4_RTT_NOM_ODT_SHIFT;
              (*RankList)[Rank].RttPrk = (((*DimmNvList)[Dimm].SPDLrbufDramOdtParkGt2400Le3200 & (BIT5 + BIT4 + BIT3)) << (DDR4_RTT_PRK_ODT_SHIFT - 3));
            } else {
              (*RankList)[Rank].RttNom = (((*DimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt2400Le3200 & (BIT2 + BIT1 + BIT0)) << DDR4_RTT_NOM_ODT_SHIFT);
              (*RankList)[Rank].RttPrk = (((*DimmNvList)[Dimm].SPDLrbufDramOdtParkGt2400Le3200 & (BIT2 + BIT1 + BIT0)) << DDR4_RTT_PRK_ODT_SHIFT);
            } // if rank
          } // if DDR freq
        } else {
          //
          // Ranks 0 and 1 dual rank RTT_WR=240, RTT_NOM=240, RTT_PAR=60
          // Ranks 0 and 1 Quad rank (the ones with ODT pins) RTT_WR=240, RTT_NOM=240, RTT_PAR=34
          // Ranks 2 and 3 (the ones without ODT pins) RTT_WR=240, RTT_NOM=NA, RTT_PAR=240
          //

          //RttWr = 240 Ohms
          (*RankList)[Rank].RttWr = DDR4_RTT_WR_240 << DDR4_RTT_WR_ODT_SHIFT;
          if (Rank > 1) {
            // ranks 2 and 3
            //RttPrk = 240 Ohms
            (*RankList)[Rank].RttPrk = DDR4_RTT_PARK_240 << DDR4_RTT_PRK_ODT_SHIFT;
            //RttNom = Disabled
            (*RankList)[Rank].RttNom = DDR4_RTT_NOM_DIS << DDR4_RTT_NOM_ODT_SHIFT;
          } else {
            //RttNom = 240 Ohms
            (*RankList)[Rank].RttNom = DDR4_RTT_NOM_240 << DDR4_RTT_NOM_ODT_SHIFT;

            if ((*DimmNvList)[Dimm].numDramRanks == 4) {
              // ranks 0 and 1 for a quad rank dimm
              //RttPrk = 48 Ohms
              (*RankList)[Rank].RttPrk = DDR4_RTT_PARK_48 << DDR4_RTT_PRK_ODT_SHIFT;
            } else {
              // ranks 0 and 1 for a dual rank dimm
              //RttPrk = 60 Ohms
              (*RankList)[Rank].RttPrk = DDR4_RTT_PARK_60 << DDR4_RTT_PRK_ODT_SHIFT;
            } // if numDramRanks
          } // if rank
        } // if SPDSpecRev

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                        "DRAM Rtt_wr = %d, Rtt_park = %d, Rtt_nom = %d\n",
                        Ddr4RttWrLookup[(*RankList)[Rank].RttWr >> DDR4_RTT_WR_ODT_SHIFT],
                        Ddr4RttNomLookup[(*RankList)[Rank].RttPrk >> DDR4_RTT_PRK_ODT_SHIFT],
                        Ddr4RttNomLookup[(*RankList)[Rank].RttNom >> DDR4_RTT_NOM_ODT_SHIFT]);

      } else
#endif // Update this section for DDR5 LRDIMM
#endif // LRDIMM_SUPPORT
      {
#ifdef DDR5_SUPPORT
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                        "DRAM DqsRttPrk = %d, Rtt_wr = %d, Rtt_park = %d, Rtt_nom_rd = %d, Rtt_nom_wr = %d\n",
                        Ddr5RttLookup[DramRttEntryData.Bits.dqsRttPrk],
                        Ddr5RttLookup[DramRttEntryData.Bits.rttWr],
                        Ddr5RttLookup[DramRttEntryData.Bits.rttPrk],
                        Ddr5RttLookup[DramRttEntryData.Bits.rttNomRd],
                        Ddr5RttLookup[DramRttEntryData.Bits.rttNomWr]);

        // Set encodings for DDR5 DRAM Mode Registers
        (*RankList)[Rank].DqsRttPrk = DramRttEntryData.Bits.dqsRttPrk ;
        (*RankList)[Rank].RttWr = DramRttEntryData.Bits.rttWr;
        (*RankList)[Rank].RttPrk = DramRttEntryData.Bits.rttPrk;
        (*RankList)[Rank].RttNomRd = DramRttEntryData.Bits.rttNomRd;
        (*RankList)[Rank].RttNomWr = DramRttEntryData.Bits.rttNomWr;
#else // DDR4 RTT
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                        "DRAM Rtt_wr = %d, Rtt_park = %d, Rtt_nom = %d\n",
                        Ddr4RttWrLookup[DramRttEntryData.Bits.rttWr],
                        Ddr4RttNomLookup[DramRttEntryData.Bits.rttPrk],
                        Ddr4RttNomLookup[DramRttEntryData.Bits.rttNom]);

        // Set encodings for DDR4 DRAM Mode Registers
        (*RankList)[Rank].RttWr = DramRttEntryData.Bits.rttWr << DDR4_RTT_WR_ODT_SHIFT;
        (*RankList)[Rank].RttPrk = DramRttEntryData.Bits.rttPrk << DDR4_RTT_PRK_ODT_SHIFT;
        (*RankList)[Rank].RttNom = DramRttEntryData.Bits.rttNom << DDR4_RTT_NOM_ODT_SHIFT;
#endif // END DDR RTT
      } // if LRDIMM

      // Lookup ODT activations for this config
      ActivationTableEntryPtr = GetActivationForCurrentTargetDimmRankAndPopulationOdt (Socket, Ch, Dimm, Rank);

      // Set encodings for ODT signals in IMC registers
      for (Dimms = 0; Dimms < MAX_DIMM; Dimms++) {
        RankNvList = GetRankNvList(Host, Socket, Ch, Dimms);
        if ((*DimmNvList)[Dimms].dimmPresent == 0) {
          continue;
        }
        for (Ranks = 0; Ranks < Host->var.mem.socket[Socket].maxRankDimm; Ranks++) {

          // Skip if no rank
          if ((*RankList)[Ranks].enabled == 0) {
            continue;
          }
          // Get ODT signal associated with this rank
          OdtPin = (*RankNvList)[Ranks].ODTIndex;
          ImcRank = Dimms * MAX_RANK_DIMM + Ranks;

          // Set ODT signals for target read
          if ((ActivationTableEntryPtr !=  NULL) && (ActivationTableEntryPtr->actBits[RD_TARGET] & (1 << ImcRank))) {
             if (IsLrdimmPresent (Socket, Ch, Dimms)) {
               if ((*ChannelNvList)[Ch].encodedCSMode != 2) {
                 RdOdtTableData |= (1 << (Rank * 8 + OdtPin));
               }
          } else {
              RdOdtTableData |= (1 << (Rank * 8 + OdtPin));
            }
          }
          // Set ODT signals for target write
          if ((ActivationTableEntryPtr !=  NULL) && (ActivationTableEntryPtr->actBits[WR_TARGET] & (1 << ImcRank))) {
             if (IsLrdimmPresent (Socket, Ch, Dimms)) {
               if ((*ChannelNvList)[Ch].encodedCSMode != 2) {
                 WrOdtTableData |= (1 << (Rank * 8 + OdtPin));
               }
            } else {
              WrOdtTableData |= (1 << (Rank * 8 + OdtPin));
            }
          }
        } // matrix rank
      } // matrix dimm
    } // target rank

    if ((*ChannelNvList)[Ch].encodedCSMode == 2){
      switch (Dimm) {
        case 0:
          RdOdtTableData = 0x102;
          break;
        case 1:
          RdOdtTableData = 0x408;
          break;
        case 2:
          RdOdtTableData = 0x1020;
          break;
        default:
          //Automated add of default case. Please review.
          break;
      }
      WrOdtTableData = 0;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Activations: Read: 0x%x Write: 0x%x\n", RdOdtTableData,WrOdtTableData);
    SetOdtActMatrixRegs (Socket, Ch, Dimm, RdOdtTableData, WrOdtTableData);
  } // target dimm
} // BufferDramAndLrdimmBacksideOdtToHost

/*++

  Gets the DRAM Vref Value

  @param [IN    ] Socket    - Socket number
  @param [IN    ] Ch        - Channel number (0-based)
  @param [IN    ] Dimm      - Dimm number (0-based)
  @param [   OUT] VrefValue - Pointer to store the Vref value

  @retval True or False, depending if the Vref value is found in the ODT Value table
--*/
BOOLEAN
GetDramVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT16  *VrefValue
  )
{
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_52);
    return FALSE;
  } else {
    *VrefValue = ((UINT16) DdrOdtValuesPtr->dramVref[Dimm] * ODT_TABLE_LIB_VREF_PERCENT_FACTOR) / ODT_TABLE_STORAGE_VREF_PERCENT_FACTOR;
    return TRUE;
  }
} // GetDramVrefValueOdt

/*++

  Returns the Mc Vref value

  @param [IN    ] Socket - Socket number
  @param [IN    ] Ch     - Channel number (0-based)
  @param [   OUT] McVref - Pointer to store the Mc Vref value

  @retval True or False, depending if the pointer in the ODT Value table is found
--*/
BOOLEAN
GetMcVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  OUT   UINT16  *McVref
  )
{
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_35);
    return FALSE;
  } else {
    *McVref = ((UINT16) DdrOdtValuesPtr->mcVref * ODT_TABLE_LIB_VREF_PERCENT_FACTOR) / ODT_TABLE_STORAGE_VREF_PERCENT_FACTOR;
    return TRUE;
  }
} // GetMcVrefValueOdt

/*++

  Returns the Read and Write activation DRAM Vref values

  @param [IN    ] Socket   - Socket number
  @param [IN    ] Ch       - Channel number (0-based)
  @param [IN    ] Dimm     - Dimm number (0-based)
  @param [IN    ] Rank     - Rank number (0-based)
  @param [   OUT] ReadAct  - Pointer to store the read activation value
  @param [   OUT] WriteAct - Pointer to store the write activation value

  @retval True or False, depending if the pointer in the ODT Value table is found
--*/
BOOLEAN
GetReadWriteActivationValuesOdt (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  Dimm,
  IN     UINT8  Rank,
     OUT UINT16 *ReadAct,
     OUT UINT16 *WriteAct
  )
{
  struct ddrOdtActivationTableEntry *OdtActPtr;

  OdtActPtr = GetActivationForCurrentTargetDimmRankAndPopulationOdt (Socket, Ch, Dimm, Rank);

  if (OdtActPtr == NULL) {
    return FALSE;
  } else {
    *ReadAct = OdtActPtr->actBits[RD_TARGET];
    *WriteAct = OdtActPtr->actBits[WR_TARGET];
    return TRUE;
  }
} // GetReadWriteActivationValuesOdt

/**
  Determine if this is a Termination Rank

  @param [IN] Socket - Socket number
  @param [IN] Ch     - Channel number
  @param [IN] Dimm   - DIMM number
  @param [IN] Rank   - Rank number

  @retval 0: This is not a Termination Rank
  @retval 1: This is a Termination Rank
**/
UINT32
IsTermRank (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Rank
  )
{
  PSYSHOST       Host;
  struct ddrRank (*RankNvList)[MAX_RANK_DIMM];
  UINT16         ReadAct;
  UINT16         WriteAct;
  UINT8          Dimms;
  UINT8          Ranks;

  Host = GetSysHostPointer ();

  // Check for non-target termination on this rank
  if ((Rank < 2) && IsLrdimmPresent (Socket, Ch, Dimm)){
    return 1;
  }
  for (Dimms = 0; Dimms < MAX_DIMM; Dimms++) {
    RankNvList = GetRankNvList(Host, Socket, Ch, Dimms);
    for (Ranks = 0; Ranks < MAX_RANK_DIMM; Ranks++) {

      if ((*RankNvList)[Ranks].enabled == 0) {
        continue;
      }

      // Skip target rank
      if ((Dimms == Dimm) && (Ranks == Rank)) {
        continue;
      }

      // Lookup ODT activations for this config
      if (GetReadWriteActivationValuesOdt(Socket, Ch, Dimms, Ranks, &ReadAct, &WriteAct)) {
        if ((ReadAct & (BIT0 << ((Dimm * MAX_RANK_DIMM) + Rank))) ||
            (WriteAct & (BIT0 << ((Dimm * MAX_RANK_DIMM) + Rank)))) {
          return 1;
        }
      }
    }
  }

  return 0;
}

//
// TODO: got to update SPR code to use DDR5 ODT, its DDR5 build would break if these are introduced now
//

/*++

  Gets the DRAM Ibt CA Value

  @param [IN    ] Socket     - Socket number
  @param [IN    ] Ch         - Channel number (0-based)
  @param [IN    ] Dimm       - Dimm number (0-based)
  @param [   OUT] IbtCaValue - Pointer to store the Ibt CA value

  @retval True or False, depending if the Ibt CA value is found in the ODT Value table
--*/
BOOLEAN
GetIbtCaValueOdt (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *IbtCaValue
  )
{
#ifdef DDR5_SUPPORT
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_52);
    return FALSE;
  } else {
    *IbtCaValue = DdrOdtValuesPtr->dramIbtCa[Dimm];
    return TRUE;
  }
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetIbtCaValueOdt


/*++

  Gets the DRAM CA Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] DcaVrefValue - Pointer to store the DRAM CA Vref value

  @retval True or False, depending if the DRAM CA Vref value is found in the ODT Value table
--*/
BOOLEAN
GetDcaVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT16  *DcaVrefValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_ODT_VALUE_TABLE_ENTRY *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_52);
    return FALSE;
  } else {
    *DcaVrefValue = ((UINT16) DdrOdtValuesPtr->dramCaVref[Dimm] * ODT_TABLE_LIB_VREF_PERCENT_FACTOR) / ODT_TABLE_STORAGE_VREF_PERCENT_FACTOR;
    return TRUE;
  }
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetDcaVrefValueOdt


/*++

  Gets the DRAM Ibt CS Value

  @param [IN    ] Socket     - Socket number
  @param [IN    ] Ch         - Channel number (0-based)
  @param [IN    ] Dimm       - Dimm number (0-based)
  @param [   OUT] IbtCsValue - Pointer to store the Ibt CS value

  @retval True or False, depending if the Ibt CS value is found in the ODT Value table
--*/
BOOLEAN
GetIbtCsValueOdt (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *IbtCsValue
  )
{
#ifdef DDR5_SUPPORT
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_52);
    return FALSE;
  } else {
    *IbtCsValue = DdrOdtValuesPtr->dramIbtCs[Dimm];
    return TRUE;
  }
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetIbtCsValueOdt


/*++

  Gets the DRAM DCS Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] DcsVrefValue - Pointer to store the DRAM CS Vref value

  @retval True or False, depending if the DRAM CS Vref value is found in the ODT Value table
--*/
BOOLEAN
GetDcsVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT16  *DcsVrefValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_ODT_VALUE_TABLE_ENTRY *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_52);
    return FALSE;
  } else {
    *DcsVrefValue = ((UINT16) DdrOdtValuesPtr->dramCsVref[Dimm] * ODT_TABLE_LIB_VREF_PERCENT_FACTOR) / ODT_TABLE_STORAGE_VREF_PERCENT_FACTOR;
    return TRUE;
  }
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetDcsVrefValueOdt

/*++

  Gets the DRAM QCS Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] QcsVrefValue    - Pointer to store the 2x DRAM CS Vref value

  @retval True or False, depending if the DRAM CS Vref value is found in the ODT Value table
--*/
BOOLEAN
GetQcsVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   INT8    *QcsVrefValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCS_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just return the default value

    *QcsVrefValue = QCS_QCA_VREF_TOP_RANGE_TIMES_TWO - QCS_VREF_INITIAL_DEFAULT_VALUE_TIMES_TWO;
  } else {

    // The encoding of QCS vref is 2 * (97.5% - target percent)
    // i.e. the encoding for a QCS vref of 76.5% is 2 * (97.5 - 76.5) = 42d = 0x2A
    // To avoid integer math, we muliply 97.5 * 2 ( = 195) and use a VrefValue already * 2

    *QcsVrefValue = QCS_QCA_VREF_TOP_RANGE_TIMES_TWO - DdrOdtValuesPtr->VrefValueTimesTwo;
  }
  return TRUE;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetQcsVrefValueOdt

/*++

  Gets the DRAM QCA Vref Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] QcsVrefValue    - Pointer to store the 2x DRAM CA Vref value

  @retval True or False, depending if the DRAM QCA Vref value is found in the Backside ODT Value table
--*/
BOOLEAN
GetQcaVrefValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   INT8    *QcaVrefValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCA_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just return the default value

    *QcaVrefValue = QCS_QCA_VREF_TOP_RANGE_TIMES_TWO - QCA_VREF_INITIAL_DEFAULT_VALUE_TIMES_TWO;
  } else {

    // The encoding of QCA vref is 2 * (97.5% - target percent)
    // i.e. the encoding for a QCS vref of 63% is 2 * (97.5 - 63) = 69d = 0x45
    // To avoid integer math, we muliply 97.5 * 2 ( = 195) and use a VrefValue already * 2

    *QcaVrefValue = QCS_QCA_VREF_TOP_RANGE_TIMES_TWO - DdrOdtValuesPtr->VrefValueTimesTwo;
  }
  return TRUE;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetQcaVrefValueOdt

// Strobe to RCD posiltion relative index map
// The index of the array represents the strobe and the value represents the relative index
UINT8 StrobeToRelativePositionIndex[MAX_STROBE_DDR5] =
{
  STRAP_POSITION, 4, 3, 2, 1, STRAP_POSITION, 4, 3, 2, 1,
  2, 3, 4, STRAP_POSITION, 1, 2, 3, 4, STRAP_POSITION, 1
};

/**
  Converts a strobe number to a relative DRAM position value
  The strobe number corresponds to the DQS# signal from the CPU
  The relative position number relates to the position of the DRAM
  relative to the RCD, such that 1 is closest to the RCD and 5 is
  furthest (STRAP).

  @param [IN    ] Strobe          - Strobe (0-based)
**/
UINT8
GetPositionFromStrobe (
  IN    UINT8   Strobe
)
{
  return StrobeToRelativePositionIndex[Strobe];
}

/*++

  Gets the DRAM QCK ODT Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [IN    ] Strobe          - Strobe (0-based)
  @param [   OUT] QckOdtValue     - Pointer to store the encoded DRAM QCK ODT value

  @retval True or False, depending if the DRAM QCK ODT` value is found in the backside ODT Value table
--*/
BOOLEAN
GetDramQckValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  IN    UINT8   Strobe,
  OUT   UINT16  *QckOdtValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;
  UINT8                               RelativePosition = 5;
  ODT_BACKSIDE_POSITION_TABLE_VALUE   PositionOdtData;
  BOOLEAN                             FoundInTable = FALSE;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCK_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just use a default value

    PositionOdtData.Data = QCK_ODT_INITIAL_DEFAULT_VALUE;
  } else {

    PositionOdtData.Data = DdrOdtValuesPtr->PerDramOdtValues;
    FoundInTable = TRUE;
  }

  RelativePosition = GetPositionFromStrobe (Strobe);


  switch (RelativePosition) {
    case 1:
      *QckOdtValue = PositionOdtData.Bits.P1;
      break;
    case 2:
      *QckOdtValue = PositionOdtData.Bits.P2;
      break;
    case 3:
      *QckOdtValue = PositionOdtData.Bits.P3;
      break;
    case 4:
      *QckOdtValue = PositionOdtData.Bits.P4;
      break;
    case STRAP_POSITION:
      *QckOdtValue = PositionOdtData.Bits.P5;
      break;
  }

  return FoundInTable;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
}

/*++

  Gets the RCD QCK R-On ODT Value

  @param [IN    ] Socket            - Socket number
  @param [IN    ] Ch                - Channel number (0-based)
  @param [IN    ] Dimm              - Dimm number (0-based)
  @param [   OUT] RcdQckRonOdtValue - Pointer to store the encoded DRAM QCK ODT value

  @retval True or False, depending if the RCD QCK R-On ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetRcdQckRonValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT8   *RcdQckRonOdtValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;
  BOOLEAN                             FoundInTable = FALSE;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCK_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just use a default value

    *RcdQckRonOdtValue = RCD_QCK_RON_ODT_INITIAL_DEFAULT_VALUE;
  } else {

    *RcdQckRonOdtValue = DdrOdtValuesPtr->RcdOutputDriveStrength;
    FoundInTable = TRUE;
  }

  return FoundInTable;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
}

/*++

  Gets the RCD QCA R-On ODT Value

  @param [IN    ] Socket            - Socket number
  @param [IN    ] Ch                - Channel number (0-based)
  @param [IN    ] Dimm              - Dimm number (0-based)
  @param [   OUT] RcdQcaRonOdtValue - Pointer to store the encoded DRAM QCA ODT value

  @retval True or False, depending if the RCD QCA R-On ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetRcdQcaRonValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT8   *RcdQcaRonOdtValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;
  BOOLEAN                             FoundInTable = FALSE;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCA_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just use a default value

    *RcdQcaRonOdtValue = RCD_QCA_RON_ODT_INITIAL_DEFAULT_VALUE;
  } else {

    *RcdQcaRonOdtValue = DdrOdtValuesPtr->RcdOutputDriveStrength;
    FoundInTable = TRUE;
  }

  return FoundInTable;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
}

/*++

  Gets the RCD QCS R-On ODT Value

  @param [IN    ] Socket            - Socket number
  @param [IN    ] Ch                - Channel number (0-based)
  @param [IN    ] Dimm              - Dimm number (0-based)
  @param [   OUT] RcdQcsRonOdtValue - Pointer to store the encoded DRAM QCS ODT value

  @retval True or False, depending if the RCD QSA R-On ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetRcdQcsRonValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  OUT   UINT8   *RcdQcsRonOdtValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;
  BOOLEAN                             FoundInTable = FALSE;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCS_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just use a default value

    *RcdQcsRonOdtValue = RCD_QCS_RON_ODT_INITIAL_DEFAULT_VALUE;
  } else {

    *RcdQcsRonOdtValue = DdrOdtValuesPtr->RcdOutputDriveStrength;
    FoundInTable = TRUE;
  }

  return FoundInTable;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
}


/*++

  Gets the DRAM QCS ODT Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [IN    ] Strobe          - Strobe (0-based)
  @param [   OUT] QcsOdtValue     - Pointer to store the encoded DRAM QCS ODT value

  @retval True or False, depending if the DRAM QCS ODT value is found in the backside ODT Value table
--*/
BOOLEAN
GetDramQcsValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  IN    UINT8   Strobe,
  OUT   UINT16  *QcsOdtValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;
  UINT8                               RelativePosition = 5;
  ODT_BACKSIDE_POSITION_TABLE_VALUE   PositionOdtData;
  BOOLEAN                             FoundInTable = FALSE;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCS_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just use a default value

    PositionOdtData.Data = QCS_ODT_INITIAL_DEFAULT_VALUE;
  } else {

    PositionOdtData.Data = DdrOdtValuesPtr->PerDramOdtValues;
    FoundInTable = TRUE;
  }

  RelativePosition = GetPositionFromStrobe (Strobe);
  switch (RelativePosition) {
    case 1:
      *QcsOdtValue = PositionOdtData.Bits.P1;
      break;
    case 2:
      *QcsOdtValue = PositionOdtData.Bits.P2;
      break;
    case 3:
      *QcsOdtValue = PositionOdtData.Bits.P3;
      break;
    case 4:
      *QcsOdtValue = PositionOdtData.Bits.P4;
      break;
    case STRAP_POSITION:
      *QcsOdtValue = PositionOdtData.Bits.P5;
      break;
  }

  return FoundInTable;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
}


/*++

  Gets the DRAM QCA ODT Value

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [IN    ] Strobe          - Strobe (0-based)
  @param [   OUT] QcaOdtValue     - Pointer to store the encoded DRAM QCA ODT value

  @retval True or False, depending if the DRAM QCA ODT` value is found in the backside ODT Value table
--*/
BOOLEAN
GetDramQcaValueOdt (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   Dimm,
  IN    UINT8   Strobe,
  OUT   UINT16  *QcaOdtValue
  )
{
#ifdef DDR5_SUPPORT
  DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY  *DdrOdtValuesPtr = NULL;
  UINT8                               RelativePosition = 5;
  ODT_BACKSIDE_POSITION_TABLE_VALUE   PositionOdtData;
  BOOLEAN                             FoundInTable = FALSE;


  DdrOdtValuesPtr = GetBacksideValuesForCurrentConfigOdt (Socket, Ch, Dimm, QCA_VREF);

  if (DdrOdtValuesPtr == NULL) {

    // Entry not found in the table, so, just use a default value

    PositionOdtData.Data = QCA_ODT_INITIAL_DEFAULT_VALUE;
  } else {

    PositionOdtData.Data = DdrOdtValuesPtr->PerDramOdtValues;
    FoundInTable = TRUE;
  }

  RelativePosition = GetPositionFromStrobe (Strobe);
  switch (RelativePosition) {
    case 1:
      *QcaOdtValue = PositionOdtData.Bits.P1;
      break;
    case 2:
      *QcaOdtValue = PositionOdtData.Bits.P2;
      break;
    case 3:
      *QcaOdtValue = PositionOdtData.Bits.P3;
      break;
    case 4:
      *QcaOdtValue = PositionOdtData.Bits.P4;
      break;
    case STRAP_POSITION:
      *QcaOdtValue = PositionOdtData.Bits.P5;
      break;
  }

  return FoundInTable;
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
}

/*++

  Gets the DRAM Ibt CLK Value

  @param [IN    ] Socket      - Socket number
  @param [IN    ] Ch          - Channel number (0-based)
  @param [IN    ] Dimm        - Dimm number (0-based)
  @param [   OUT] IbtClkValue - Pointer to store the Ibt CLK value

  @retval True or False, depending if the Ibt CLK value is found in the ODT Value table
--*/
BOOLEAN
GetIbtClkValueOdt (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *IbtClkValue
  )
{
#ifdef DDR5_SUPPORT
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_52);
    return FALSE;
  } else {
    *IbtClkValue = DdrOdtValuesPtr->dramIbtClk[Dimm];
    return TRUE;
  }
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetIbtClkValueOdt


/**
  Return pointer to the ODT value table.
  The table is an array of ODT value structures.
  Also returns the size of the array.

  @param [OUT] OdtValueTablePtr  - Pointer to caller's ODT table pointer
  @param [OUT] OdtValueTableSize - Pointer to caller's ODT table size variable.

  @retval N/A
**/
VOID
GetBacksideValuesTableOdt (
  OUT DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY **OdtValueTablePtr,
  OUT UINT16                             *OdtValueTableSize
  )
{
#ifdef DDR5_SUPPORT
  *OdtValueTablePtr = &Ddr5BacksideOdtValueTable[0];
  *OdtValueTableSize = ARRAY_SIZE (Ddr5BacksideOdtValueTable);
#endif // DDR5_SUPPORT
} // GetValuesTableOdt

/*++

  Gets the RDIMM DRAM RON or LRDIMM DB HOST RON

  @param [IN    ] Socket          - Socket number
  @param [IN    ] Ch              - Channel number (0-based)
  @param [IN    ] Dimm            - Dimm number (0-based)
  @param [   OUT] DramOrDbHostRon - RDIMM DRAM RON or LRDIMM DB Host RON

  @retval True or False, depending if the RDIMM DRAM RON or LRDIMM DB HOST RON is found in the ODT Value table
--*/
BOOLEAN
GetDramOrDbHostRon (
  IN    UINT8 Socket,
  IN    UINT8 Ch,
  IN    UINT8 Dimm,
  OUT   UINT8 *DramOrDbHostRon
  )
{
#ifdef DDR5_SUPPORT
  struct ddrOdtValueTableEntry *DdrOdtValuesPtr = NULL;

  DdrOdtValuesPtr = GetValuesForCurrentConfigOdt (Socket, Ch);

  if (DdrOdtValuesPtr == NULL) {
    RC_FATAL_ERROR (DdrOdtValuesPtr != NULL, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_52);
    return FALSE;
  } else {
    *DramOrDbHostRon = DdrOdtValuesPtr->DramOrDbHostRon;
    return TRUE;
  }
#else // DDR5_SUPPORT
  return FALSE;
#endif // DDR5_SUPPORT
} // GetDramOrDbHostRon
