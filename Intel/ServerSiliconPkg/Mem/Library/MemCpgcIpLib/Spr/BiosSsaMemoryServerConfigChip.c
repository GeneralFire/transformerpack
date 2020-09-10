/** @file
  This file contains the BIOS implementation of the BIOS-SSA Memory Configuration API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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
#include <Library/MemCpgcIpLib.h>
#include "Include/MemCpgcRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/SysHostPointerLib.h>
#include "Include/CpgcChip.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpTargetLib.h>
#include <Library/MemorySsaLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>

//
//This BSSA file contains subsets of all BSSA function calls made from the Core files; all CSR accesses are made here
//


/**

  This routine sets MaxNumberErrorCounters required by BiosServerGetCpgcInfo

  @param Host                       - Pointer to sysHost
  @param Socket                     - Pointer to MRC_SERVER_CPGC_INFO struct

  @retval N/A

**/
VOID
BiosServerGetCpgcInfoChip (
  IN     PSYSHOST                Host,
  IN OUT MRC_SERVER_CPGC_INFO    *CpgcInfoStruct
  )
{
  CpgcInfoStruct->MaxNumberErrorCounters = MAX_STROBE;
}

/**
  Calculates the logicalRank[MAX_RANK_CH] values for a channel  based on the Dimm/Rank pair values entered from the array "DimmRankPairs".
  In the logicalRankMapping[8] array where the "logicalRank" values are getting saved for a channel. By default,
  all array elements are set to 0XF and are modified with the "correct" logical rank value only if that Dimm/Rank is valid/"enabled", else we leave the value as 0XF.
  @param[in]      MrcData   - Pointer to the MRC global data area.
  @param[in]      Socket    - Zero based CPU socket number.
  @param[in]      Channel   - Zero based memory channel number.
  @param[in]      Dimm      - Zero based memory DIMM number.
  @param[in]      Rank      - Zero based memory Physical Rank number.
  @param[out]      logicalRankMapping - Pointer to an array where the logical Rank values for a channel get saved
  @retval N/A.
**/
VOID
GetLogicalRankMapping (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Channel,
  IN UINT8                 Dimm,
  IN UINT8                 Rank,
  OUT UINT8                *logicalRankMapping
  )
{
  struct channelNvram  (*channelNvList)[MAX_CH];
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  struct ddrRank  (*rankNvList)[MAX_RANK_DIMM];
  channelNvList = GetChannelNvList(Host, Socket);
  dimmNvList = GetDimmNvList(Host, Socket, Channel);
  rankNvList = GetRankNvList(Host, Socket, Channel, Dimm);
//Based on the Dimm-Rank pairs we calculate the corresponding logicalRanks
  if (((*channelNvList)[Channel].enabled) &&
     ((*dimmNvList)[Dimm].dimmPresent) &&
     ((*rankNvList)[Rank].enabled) &&
     (((*dimmNvList)[Dimm].mapOut[Rank] == 0) || (((*dimmNvList)[Dimm].mapOut[Rank] == 1) && ((*dimmNvList)[Dimm].DcpmmPresent == 1)))) {

     //For invalid Dimm/Rank values, that location in the logicalRankArray will have a value of 0xFF
     *logicalRankMapping = GetLogicalRank(Host, Socket, Channel, Dimm , Rank);  // Get logical rank
  }
}
/**

  This routine sets the CSRs required by BiosServerSetCreditWaitConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param Enable                     - Specifies whether CPGC credit-wait feature is enabled.

  @retval N/A

**/
VOID
BiosServerSetCreditWaitConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  BOOLEAN               Enable
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetWpqInOrderConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param ChannelInSocket            - Channel number (0-based)
  @param Enable                     - Specifies whether the WPQ-in-order feature are enabled

  @retval N/A

**/
VOID
BiosServerSetWpqInOrderConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  BOOLEAN               Enable
  )
{
  CpgcInorderModeEnable (Socket, ChannelInSocket, Enable);
}

/**

  This routine sets the CSRs required by BiosServerSetMultipleWrCreditConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param ChannelInSocket            - Channel number (0-based)
  @param Enable                     - Specifies whether multiple write credits are enabled.

  @retval N/A

**/
VOID
BiosServerSetMultipleWrCreditConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  BOOLEAN               Enable
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbIndexes

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param ChannelInSocket            - Channel number (0-based)
  @param StartIndex                 - Starting index in the DqDB.  The value must be less than or equal to the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.
  @param EndIndex                   - Ending index in the DqDB.  The value must be less than or equal to the StartIndex value and less than or equal to the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.
  @param IncRateMode                - Increment rate mode.
  @param IncRate                    - Increment rate value.  If the IncRateMode is ExponentialDqdbIncRateMode then the IncRate value must be less than or equal to the MaxDqdbIncRateExponentVal from the GetCpgcInfo() function.  If the IncRateMode is LinearDqdbIncRateMode then the IncRate value must be less than or equal to the MaxDqdbIncRateLinearVal from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbIndexesChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT8                 StartIndex,
  UINT8                 EndIndex,
  DQDB_INC_RATE_MODE    IncRateMode,
  UINT8                 IncRate
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbEccDataSource

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param EccDataSource              - Specifies the ECC signal data source

  @retval N/A

**/
VOID
BiosServerSetDqdbEccDataSourceChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  ECC_DATA_SOURCE       EccDataSource
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqMode

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param Mode                       - DqDB unisequencer mode.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqModeChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  DQDB_UNISEQ_MODE      Mode
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqWrSeed

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param Seed                       - DqDB write unisequencer seed value.  The value must be less than or equal to the MaxDqdbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqWrSeedChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  UINT32                Seed
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqRdSeed

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param Seed                       - DqDB write unisequencer seed value. The value must be less than or equal to the MaxDqdbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqRdSeedChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  UINT32                Seed
  )
{

}

/**

  This routine gets the CSRs required by BiosServerGetDqdbUniseqWrSeed

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param Seed                       - Pointer to where the DqDB write unisequencer seed value will be stored.

  @retval N/A

**/
VOID
BiosServerGetDqdbUniseqWrSeedChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  UINT32                *Seed
  )
{

}

/**

  This routine gets the CSRs required by BiosServerGetDqdbUniseqRdSeed

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param Seed                       - Pointer to where the DqDB write unisequencer seed value will be stored.

  @retval N/A

**/
VOID
BiosServerGetDqdbUniseqRdSeedChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  UINT32                *Seed
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqLmn

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param L                          - DqDB unisequencer L counter value.  The value must be less than or equal to the MaxDqdbUniseqLVal value from the GetCpgcInfo() function.
  @param M                          - DqDB unisequencer M counter value.  The value must be less than or equal to the MaxDqdbUniseqMVal value from the GetCpgcInfo() function.
  @param N                          - DqDB unisequencer N counter value.  The value must be less than or equal to the MaxDqdbUniseqNVal value from the GetCpgcInfo() function.
  @param LDataSel                   - DqDB unisequencer L data select value.  The value must be 0 or 1.
  @param EnableFreqSweep            - Enables the Frequency Sweep feature.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqLmnChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  UINT16                L,
  UINT16                M,
  UINT16                N,
  UINT8                 LDataSel,
  BOOLEAN               EnableFreqSweep
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbUniseqLfsrConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param ReloadRate                 - DqDB unisequencer LFSR seed reload rate.  The value must be less than or equal to the MaxDqdbUniseqSeedReloadRateVal value from the GetCpgcInfo() function.
  @param SaveRate                   - DqDB unisequencer LFSR seed save rate.  The value must be less than or equal to the MaxDqdbUniseqSeedSaveRateVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbUniseqLfsrConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 ReloadRate,
  UINT8                 SaveRate
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbInvertDcConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Mode                       - Invert/DC mode.
  @param DcPolarity                 - DC polarity (when operating in the DC mode). 0 = DC low. 1 = DC high.
  @param ShiftRateExponent          - Exponent of the bitmask shift rate.  The value must be less than or equal to the MaxDqdbInvertDcShiftRateExponentVal value from the GetCpgcInfo() function.
  @param DqMaskLow                 - Lower 32 bits of the DQ Mask (0-based)
  @param DqMaskHigh                - Upper 32 bits of the DQ Mask (0-based)
  @param EccMask                    - Ecc Mask (0-based)

  @retval N/A

**/
VOID
BiosServerSetDqdbInvertDcConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  INVERT_DC_MODE        Mode,
  UINT8                 DcPolarity,
  UINT8                 ShiftRateExponent,
  UINT32                DqMaskLow,
  UINT32                DqMaskHigh,
  UINT8                 EccMask
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetCadbConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param EnableAlwaysOn             - Specifies whether CADB patterns will be driven out on all command and address pins every Dclk while a test is running.
  @param EnableOnDeselect           - Specifies whether CADB patterns will be driven out during deselect packets.
  @param EnableParityNTiming        - Specifies parity timing.  If this parameter is TRUE, CADB parity will be driven on same clock as CMD/Address (i.e., N timing for UDIMM DDR4 only).  If this parameter is FALSE, CADB parity will be driven on the clock after CMD/Address (i.e,. N+1 timing for all other devices).
  @param EnableOnePassAlwaysOn      - This parameter only applies if the EnableAlwaysOn parameter is TRUE.  If this parameter is TRUE, the test stops after all 16 entries have been issued out of CADB.
  @param EnablePdaDoubleLength      - If this parameter is TRUE, any PBA (Per Buffer Addressable) /PDA (Per DRAM addressable) writes issued from CADB will be double length (i.e. if lockstep is disabled, 16 UI instead of 8 UI or if lockstep is enabled, 8 UI instead of 4 UI).  This is to allow more setup up time for PBA accesses during training.  If this parameter is FALSE, PBA/PDA writes will look exactly the same as normal writes.

  @retval N/A

**/
VOID
BiosServerSetCadbConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  BOOLEAN               EnableAlwaysOn,
  BOOLEAN               EnableOnDeselect,
  BOOLEAN               EnableParityNTiming,
  BOOLEAN               EnableOnePassAlwaysOn,
  BOOLEAN               EnablePdaDoubleLength
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetCadbUniseqMode

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based CADB unisequencer number.
  @param DeselectMode               - CADB deselect cycle unisequencer mode.  The value must be less than or equal to the MaxCadbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetCadbUniseqModeChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  CADB_UNISEQ_MODE      DeselectMode
  )
{

}

/**
  Begin or end the process of loading the CADB buffer write pointer
  The function must be called with CADB_MUX_LOAD_START before the CADB Mux Pattern registers (CPGC_PATCADBMUX[0-3]PB) are programmed.
  This function must be called with CADB_MUX_LOAD_END after the CADB Mux Pattern registers (CPGC_PATCADBMUX[0-3]PB) are programmed.

  @param[in]  Socket      - Socket number
  @param[in]  Channel     - Channel number
  @param[in]  StartStop   - CADB_MUX_LOAD_START or CADB_MUX_LOAD_END

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
SsaCadbMuxPatternLoad (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Channel,
  IN    UINT8     StartStop
  )
{

  return MRC_STATUS_SUCCESS;

}

/**

  This routine sets the CSRs required by BiosServerSetCadbUniseqSeed

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based CADB unisequencer number.
  @param DeselectMode               - CADB deselect cycle unisequencer mode.  The value must be less than or equal to the MaxCadbUniseqSeedVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetCadbUniseqSeedChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  UINT32                DeselectSeed
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetCadbUniseqLmn

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Uniseq                     - Zero based DqDB unisequencer number(0-based)
  @param L                          - CADB unisequencer L counter value.  The value must be less than or equal to the MaxCadbUniseqLVal value from the GetCpgcInfo() function.
  @param M                          - CADB unisequencer M counter value.  The value must be less than or equal to the MaxCadbUniseqMVal value from the GetCpgcInfo() function.
  @param N                          - CADB unisequencer N counter value.  The value must be less than or equal to the MaxCadbUniseqNVal value from the GetCpgcInfo() function.
  @param LDataSel                   - CADB unisequencer L data select value.  The value must be 0 or 1.
  @param EnableFreqSweep            - Enables LMN frequency sweep feature.

  @retval N/A

**/
VOID
BiosServerSetCadbUniseqLmnChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Uniseq,
  UINT16                L,
  UINT16                M,
  UINT16                N,
  UINT8                 LDataSel,
  BOOLEAN               EnableFreqSweep
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetDummyReadLmn

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller (0-based)
  @param ChannelInSocket            - Channel number (0-based)
  @param L                          - Dummy Read unisequencer L counter value.  The value must be less than or equal to the MaxDummyReadLVal value from the GetCpgcInfo() function.
  @param M                          - Dummy Read unisequencer M counter value.  The value must be less than or equal to the MaxDummyReadMVal value from the GetCpgcInfo() function.
  @param N                          - Dummy Read unisequencer N counter value.  The value must be less than or equal to the MaxDummyReadNVal value from the GetCpgcInfo() function.
  @param LDataSel                   - Dummy Read unisequencer L data select value.  The value must be 0 or 1.
  @param EnableFreqSweep            - Enables LMN frequency sweep feature.

  @retval NotAvailable if the system does not support the Dummy Read feature.  Else Success.
**/

SSA_STATUS
BiosServerSetDummyReadLmnChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT16                L,
  UINT16                M,
  UINT16                N,
  UINT8                 LDataSel,
  BOOLEAN               EnableFreqSweep
  )
{
  return NotAvailable; //NA on
}

/**

  This routine sets the CSRs required by BiosServerSetDummyReadBankMask

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller (0-based)
  @param ChannelInSocket            - Channel number (0-based)
  @param BankMask                   - Dummy Read bank mask.(0-based)

  @retval NotAvailable if the system does not support the Dummy Read feature.  Else UnsupportedValue if the given BankMask value is out of range.  Else Success.
**/

SSA_STATUS
BiosServerSetDummyReadBankMaskChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT16                BankMask
  )
{
  return NotAvailable; //NA on
}

/**

  This routine sets the CSRs required by BiosServerSetSubseqConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param Subseq                            - Zero based subsequence number.  The value must be less than the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param BurstLengthMode,                  - Burst length mode.
  @param BurstLength                       - Burst length in cachelines.  If BurstLengthMode is ExponentialBurstLengthMode then the Burstlength value is an exponent and the value must be less than or equal to the MaxBurstLengthExponentVal value from the in GetCpgcInfo() function.  If BurstLengthMode is LinearBurstLengthMode then the Burstlength value is linear and the value must be less than or equal to the MaxBurstLengthLinearVal value from the in GetCpgcInfo() function.
  @param InterSubseqWait                   - Inter-subsequence wait in dclks.  The value must be less than or equal to the MaxInterSubseqWaitVal value from the GetCpgcinfo() function.
  @param SubseqType                        - Subsequence type.
  @param EnableSaveCurrentBaseAddrToStart  - Specifies whether the current base address is saved to the start address with this subsequence.
  @param EnableResetCurrentBaseAddrToStart - Specifies whether the current base address is reset to the start address with this subsequence.
  @param EnableAddrInversion               - Specifies whether address inversion enabled for this subsequence.
  @param EnableDataInversion               - Specifies whether data inversion enabled for this subsequence.

  @retval N/A

**/
VOID
BiosServerSetSubseqConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT8                 Subseq,
  BURST_LENGTH_MODE     BurstLengthMode,
  UINT32                BurstLength,
  UINT32                InterSubseqWait,
  SUBSEQ_TYPE           SubseqType,
  BOOLEAN               EnableSaveCurrentBaseAddrToStart,
  BOOLEAN               EnableResetCurrentBaseAddrToStart,
  BOOLEAN               EnableAddrInversion,
  BOOLEAN               EnableDataInversion
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetSubseqOffsetConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param Subseq                            - Zero based subsequence number.  The value must be less than the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param EnableBaseInvertData              - Specifies whether base inversion of the data is enabled.
  @param EnableRowIncrement                - Specifies whether row increment is enabled.
  @param RowIncrementOrder                 - Specifies row increment order.  The value must be 0 or 1.
  @param EnableColIncrement                - Specifies whether column increment is enabled.
  @param ColIncrementOrder                 - Specifies column increment order.  The value must be 0 or 1.
  @param BaseSubseqType                    - Every time the BaseSubseqRepeatRate is reached a Read or Write is performed for one command based on the following fields: BaseSubseqType indicates whether a read (BaseReadSubseqType) or write (BaseWriteSubseqType) operation will occur at the current base address.  EnableBaseInvertData indicates whether the read or write (BaseSubseqType) operation at the current base address is inverted (EnableBaseInvertData = TRUE) or not (EnableBaseInvertData = FALSE).
  @param BaseSubseqRepeatRate              - Specifies how often the BaseSubseqType operation occurs for 1 cacheline.  0 = No BaseSubseqType cacheline operations will occur during the Offset_Read or Offset_Write.  1 = Reserved value.  2 = Repeat the BaseSubseqType cacheline operation after every offset cacheline operation.  ...  31 = Repeat the BaseSubseqType cacheline operation after 30 offset cacheline operations.
  @param OffsetAddrUpdateRate              - Specifies the rate that the Current Offset Address is updated.  The value must be less than or equal to the MaxOffsetAddrUpdateRateVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetSubseqOffsetConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT8                 Subseq,
  BOOLEAN               EnableBaseInvertData,
  BOOLEAN               EnableRowIncrement,
  UINT8                 RowIncrementOrder,
  BOOLEAN               EnableColIncrement,
  UINT8                 ColIncrementOrder,
  SUBSEQ_TYPE           BaseSubseqType,
  UINT32                BaseSubseqRepeatRate,
  UINT32                OffsetAddrUpdateRate
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetSeqConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param StartDelay                        - Specifies the delay period (in dclks) between the start of the test and the actual entry to Loopback.Pattern where the first SubSequence is executed immediately.  The value must be less than or equal to the MaxStartDelayVal value from the GetCpgcInfo() function.
  @param SubseqStartIndex                  - Specifies the initial SubSequence pointer where a Sequence first enters Loopback.Pattern (start of test).  The value must be less than the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param SubseqEndIndex                    - When the subsequence pointed to by the SubseqEndIndex is completed the loop count is incremented by 1 and the current subsequence index is reset to the SubseqStartIndex.  The SubseqEndIndex must be greater than or equal to the SubseqStartIndex value.  The value must be less than the MaxNumberSubseqs value from the GetCpgcInfo() function.
  @param LoopCount                         - Specifies how many iterations of the complete sequence loop takes place before a test stops, where a sequence loop is defined by the completion of all subsequences indicated by the SubseqStartIndex to SubseqEndIndex. The value must be less than or equal to the MaxLoopCountVal from the GetCpgcInfo() function.  If the IsLoopCountExponential value from the GetCpgcInfo() function is TRUE then the number of loops is 2^(LoopCount-1).  If the IsLoopCountExponential value from the GetCpgcInfo() function is FALSE then the number of loops is simply LoopCount.  In both the linear of exponential cases, 0 is a special value means that the loop count is infinite.
  @param InitMode                          - Initialization mode.
  @param EnableGlobalControl               - Specifies whether global control is enabled for the given channel.
  @param EnableConstantWriteStrobe         - Specifies whether constant write strobe is enabled.
  @param EnableDummyReads                  - Specifies whether dummy reads are enabled.
  @param AddrUpdateRateMode                - Address update rate mode.

  @retval N/A

**/
VOID
BiosServerSetSeqConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT32                StartDelay,
  UINT8                 SubseqStartIndex,
  UINT8                 SubseqEndIndex,
  UINT32                LoopCount,
  CPGC_INIT_MODE        InitMode,
  BOOLEAN               EnableGlobalControl,
  BOOLEAN               EnableConstantWriteStrobe,
  BOOLEAN               EnableDummyReads,
  ADDR_UPDATE_RATE_MODE AddrUpdateRateMode
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetLoopCount

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param LoopCount                         - Specifies how many iterations of the complete sequence loop takes place before a test stops, where a sequence loop is defined by the completion of all subsequences indicated by the SubseqStartIndex to SubseqEndIndex. The value must be less than or equal to the MaxLoopCountVal from the GetCpgcInfo() function.  If the IsLoopCountExponential value from the GetCpgcInfo() function is TRUE then the number of loops is 2^(LoopCount-1).  If the IsLoopCountExponential value from the GetCpgcInfo() function is FALSE then the number of loops is simply LoopCount.  In both the linear of exponential cases, 0 is a special value means that the loop count is infinite.

  @retval N/A

**/
VOID
BiosServerSetLoopCountChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT32                LoopCount
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetSeqAddrConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param EnableRankWrapCarry               - If a base rank address wrap occurs  and EnableRankWrapCarry is set to TRUE then a carry increment or decrement from the rank address (order N) will propagate to the N+1 order address field(s).
  @param EnableBankWrapCarry               - If a base bank address wrap occurs  and EnableBankWrapCarry is set to TRUE then a carry increment or decrement from the bank address (order N) will propagate to the N+1 order address field(s).
  @param EnableRowWrapCarry                - If a base row address wrap occurs and EnableRowWrapCarry is set to TRUE then a carry increment or decrement from the row address (order N) will propagate to the N+1 order address field(s).
  @param EnableColWrapCarry                - If a base column address wrap occurs  and EnableColWrapCarry is set to TRUE then a carry increment or decrement from the column address (order N) will propagate to the N+1 order address field(s).
  @param EnableRankAddrInvert              - Setting to TRUE will cause the current rank address to be inverted with the periodicity described by the AddrInvertRate parameter.
  @param EnableBankAddrInvert              - Setting to TRUE will cause the current bank address to be inverted with the periodicity described by the AddrInvertRate parameter.
  @param EnableRowAddrInvert               - Setting to TRUE will cause the current row address to be inverted with the periodicity described by the AddrInvertRate parameter.
  @param EnableColAddrInvert               - Setting to TRUE will cause the current column address to be inverted with the periodicity described by the AddrInvertRate parameter.
  @param AddrInvertRate                    - Base address inversion rate.  The SetSeqConfig() function's AddrUpdateRateMode parameter specifies whether the base address inversion logic is based on number of cachelines written and read or on the number of sequences performed.  If AddrUpdateRateMode from the SetSeqConfig() function is CachelineAddrUpdateRate then the current base address is inverted based on 2^AddrInvertRate read and write cacheline transactions.  If AddrUpdateRateMode from the SetSeqConfig() function is LoopCountAddrUpdateRate then the current base address is inverted based on 2^AddrInvertRate loops through the sequence.  The value must be less than or equal to the MaxAddrInvertRateVal value from the GetCpgcInfo() function.
  @param RankAddrOrder                     - Specifies the relative ordering of the rank address update logic in the nested for loop in relation to the bank, row, and column address update logic.  Any address fields set to the same order that roll over will cause a distinct carry of +1 or -1 to the next higher order address field (multiple simultaneous carries will cause only one increment/decrement).  All fields can be arbitrarily placed in any order as INT32 as all address order fields are set in a continuous order starting from 0 and may not skip over an order number.  Example: setting 0,1,2,1 in the 4 fields (Col,Row,Bank,Rank) is legal; setting 0,2,2,3 is illegal (not continuous).  The value must be less than or equal to 3.
  @param BankAddrOrder                     - Specifies the relative ordering of the bank address update logic in the nested for loop in relation to the rank, row, and column address update logic.  The value must be less than or equal to 3.
  @param RowAddrOrder                      - Specifies the relative ordering of the row address update logic in the nested for loop in relation to the rank, bank, and column address update logic.  The value must be less than or equal to 3.
  @param ColAddrOrder                      - Specifies the relative ordering of the column address update logic in the nested for loop in relation to the rank, bank, and row address update logic.  The value must be less than or equal to 3.

  @retval N/A

**/
VOID
BiosServerSetSeqAddrConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  BOOLEAN               EnableRankWrapCarry,
  BOOLEAN               EnableBankWrapCarry,
  BOOLEAN               EnableRowWrapCarry,
  BOOLEAN               EnableColWrapCarry,
  BOOLEAN               EnableRankAddrInvert,
  BOOLEAN               EnableBankAddrInvert,
  BOOLEAN               EnableRowAddrInvert,
  BOOLEAN               EnableColAddrInvert,
  UINT8                 AddrInvertRate,
  UINT8                 RankAddrOrder,
  UINT8                 BankAddrOrder,
  UINT8                 RowAddrOrder,
  UINT8                 ColAddrOrder
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetSeqStartAddr

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param RankAddr                          - Rank start address.  This value is the index into the CPGC rank map (see the SetSeqRankMap() function).  The value must be less than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param BankAddr                          - Bank start address.  If the CPGC engine supports a bank map (i.e., the MaxNumberBankMapEntried value from the GetCpgcInfo() function is greater than 0) then this value is is the index into the CPGC bank map (see the SetSeqBankMap() function) and the value must be less than the MaxNumberBankMapEntried value from the GetCpgcInfo() function.  If the CPGC engine does not support a bank map then the value must be less than the BankCount value from the GetDimmInfo function for the DIMMs that will be tested.
  @param RowAddr                           - Row start address.  The value must be less than the RowSize value from the GetDimmInfo function for the DIMMs that will be tested.
  @param ColAddr                           - Column start address.  The value must be less than the ColumnSize value from the GetDimmInfo function for the DIMMs that will be tested.

  @retval N/A

**/
VOID
BiosServerSetSeqStartAddrChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT8                 RankAddr,
  UINT8                 BankAddr,
  UINT32                RowAddr,
  UINT32                ColAddr
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetSeqWrapAddr

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param RankAddr                          - Rank start address.  This value is the index into the CPGC rank map (see the SetSeqRankMap() function).  The value must be less than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param BankAddr                          - Bank start address.  If the CPGC engine supports a bank map (i.e., the MaxNumberBankMapEntried value from the GetCpgcInfo() function is greater than 0) then this value is is the index into the CPGC bank map (see the SetSeqBankMap() function) and the value must be less than the MaxNumberBankMapEntried value from the GetCpgcInfo() function.  If the CPGC engine does not support a bank map then the value must be less than the BankCount value from the GetDimmInfo function for the DIMMs that will be tested.
  @param RowAddr                           - Row start address.  The value must be less than the RowSize value from the GetDimmInfo function for the DIMMs that will be tested.
  @param ColAddr                           - Column start address.  The value must be less than the ColumnSize value from the GetDimmInfo function for the DIMMs that will be tested.

  @retval N/A

**/
VOID
BiosServerSetSeqWrapAddrChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT8                 RankAddr,
  UINT8                 BankAddr,
  UINT32                RowAddr,
  UINT32                ColAddr
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetSeqAddrInc

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param RankAddrIncMode                   - Rank address increment rate mode.
  @param RankAddrIncRate                   - Rank address increment rate.  In linear mode (see RankAddrIncMode), RankAddrIncRate+1 specifies how many times a specific rank address is repeated before adding the RankAddrIncVal.  In exponential mode, 2^RankAddrIncRate defines how many times a specific rank address is repeated before adding the RankAddrIncVal.  If RankAddrIncMode is ExponentialAddrIncMode then the value must be less than or equal to the MaxRankAddrIncRateExponentVal from the GetCpgcInfo() function.  If RankAddrIncMode is LinearAddrIncMode then the value must be less than or equal to the MaxRankAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param RankAddrIncVal                    - Rank start address increment value.  When dictated by RankAddrIncRate the current rank address will be modified by the RankAddrIncVal (which is a 2's compliment field).  A value of 0 means the rank address increment is effectively disabled since no offset will be applied.  Disabling the rank address increment by setting RankAddrIncVal to 0 does not affect a carry from a lower order field rolling over to a higher order field which will also result in an increment or decrement to the higher order field of +1 or -1.  Note that a positive value in this field will cause a +1 carry to propagate while a negative value will cause a -1 carry to propagate from order N to order N+1.  The value must be greater than or equal to the MinRankAddrIncVal from the GetCpgcInfo() function.  The value must be less than or equal to the MaxRankAddrIncVal from the GetCpgcInfo() function.
  @param BankAddrIncMode                   - Bank address increment rate mode.
  @param BankAddrIncRate                   - Bank address increment rate.  In linear mode (see BankAddrIncMode), BankAddrIncRate+1 specifies how many times a specific rank address is repeated before adding the BankAddrIncVal.  In exponential mode, 2^BankAddrIncRate defines how many times a specific rank address is repeated before adding the BankAddrIncVal.  If BankAddrIncMode is ExponentialAddrIncMode then the value must be less than or equal to the MaxBankAddrIncRateExponentVal from the GetCpgcInfo() function.  If BankAddrIncMode is LinearAddrIncMode then the value must be less than or equal to the MaxBankAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param BankAddrIncVal                    - Bank start address increment value.  When dictated by BankAddrIncRate the current bank address will be modified by the BankAddrIncVal (which is a 2's compliment field).  A value of 0 means the bank address increment is effectively disabled since no offset will be applied.  Disabling the bank address increment by setting BankAddrIncVal to 0 does not affect a carry from a lower order field rolling over to a higher order field which will also result in an increment or decrement to the higher order field of +1 or -1.  Note that a positive value in this field will cause a +1 carry to propagate while a negative value will cause a -1 carry to propagate from order N to order N+1.  The value must be greater than or equal to the MinBankAddrIncVal from the GetCpgcInfo() function.  The value must be less than or equal to the MaxBankAddrIncVal from the GetCpgcInfo() function.
  @param RowAddrIncMode                    - Row address increment rate mode.
  @param RowAddrIncRate                    - Row address increment rate.  In linear mode (see RowAddrIncMode), RowAddrIncRate+1 specifies how many times a specific rank address is repeated before adding the RowAddrIncVal.  In exponential mode, 2^RowAddrIncRate defines how many times a specific rank address is repeated before adding the RowAddrIncVal.  If RowAddrIncMode is ExponentialAddrIncMode then the value must be less than or equal to the MaxRowAddrIncRateExponentVal from the GetCpgcInfo() function.  If RowAddrIncMode is LinearAddrIncMode then the value must be less than or equal to the MaxRowAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param RowAddrIncVal                     - Row start address increment value.  When dictated by RowAddrIncRate the current row address will be modified by the RowAddrIncVal (which is a 2's compliment field.  A value of 0 means the row address increment is effectively disabled since no offset will be applied.  Disabling the row address increment by setting RowAddrIncVal to 0 does not affect a carry from a lower order field rolling over to a higher order field which will also result in an increment or decrement to the higher order field of +1 or -1.  Note that a positive value in this field will cause a +1 carry to propagate while a negative value will cause a -1 carry to propagate from order N to order N+1.  The value must be greater than or equal to the MinRowAddrIncVal from the GetCpgcInfo() function.  The value must be less than or equal to the MaxRowAddrIncVal from the GetCpgcInfo() function.
  @param ColAddrIncMode                    - Column address increment rate mode.
  @param ColAddrIncRate                    - Column address increment rate.  If ColAddrIncMode is ExponentialAddrIncMode then 2^ColAddrIncRate defines how many times a specific rank address is repeated before adding the ColAddrIncVal.  If ColAddrIncMode is LinearAddrIncMode then ColAddrIncRate+1 specifies how many times a specific rank address is repeated before adding the ColAddrIncVal.  If ColAddrIncMode is ExponentialAddrIncMode then the value must be less than or equal to the MaxColAddrIncRateExponentVal from the GetCpgcInfo() function.  If ColAddrIncMode is LinearAddrIncMode then the value must be less than or equal to the MaxColAddrIncRateLinearVal from the GetCpgcInfo() function.
  @param ColAddrIncVal                     - Column start address increment value.  When dictated by ColAddrIncRate the current column address will be modified by the ColAddrIncVal (which is a 2's compliment field).  A value of 0 means the column address increment is effectively disabled since no offset will be applied.  Disabling the column address increment by setting column address increment to 0 does not affect a carry from a lower order field rolling over to a higher order field which will also result in an increment or decrement to the higher order field of +1 or -1.  Note that a positive value in this field will cause a +1 carry to propagate while a negative value will cause a -1 carry to propagate from order N to order N+1.  The value must be greater than or equal to the MinColAddrIncVal from the GetCpgcInfo() function.  The value must be less than or equal to the MaxColAddrIncVal from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetSeqAddrIncChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  ADDR_INC_MODE         RankAddrIncMode,
  UINT32                RankAddrIncRate,
  INT8                  RankAddrIncVal,
  ADDR_INC_MODE         BankAddrIncMode,
  UINT32                BankAddrIncRate,
  INT8                  BankAddrIncVal,
  ADDR_INC_MODE         RowAddrIncMode,
  UINT32                RowAddrIncRate,
  INT16                 RowAddrIncVal,
  ADDR_INC_MODE         ColAddrIncMode,
  UINT32                ColAddrIncRate,
  INT16                 ColAddrIncVal
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetSeqRankMap

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param PairCount                         - Number of entries in the DimmRankPairs array.  The minimum value is 1.  The value must be less than or equal to the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  The valid DIMM/rank pairs can be determined via the GetDimmBitMask() and GetRankInDimm() functions.  Note that the same rank may appear more than once in the map.  Note also that all ranks do not need to appear in the map.

  @retval N/A

**/
VOID
BiosServerSetSeqRankMapChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT8                 PairCount,
  CONST DIMM_RANK_PAIR  *DimmRankPairs
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetSeqRowAddrSwizzle

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param SwizzleCount                      - Number of entries in the Swizzles array.  The value must be less than or equal to the MaxNumberRowAddrSwizzleEntries value from the GetCpgcInfo() function.  If the SwizzleCount value is 0 (and the Swizzles value is NULL) then the row address swizzling will be configured so that all the lower row addresses are mapped to themselves; effectively disabling the swizzling.
  @param Swizzles                          - Pointer to array of lower row address swizzle values.  The values must be unique and less than or equal to the MaxRowAddrSwizzleVal value from the GetCpgcInfo() function.  The array position indicates the row address that the specified row address will be swapped with, e.g., a value of 10 at the array index 1 means that row addresses 10 and 1 will be swapped.

  @retval N/A

**/
VOID
BiosServerSetSeqRowAddrSwizzleChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  UINT8                 SwizzleCount,
  CONST UINT8           *Swizzles
)
{

}

/**

  This routine gets the CSRs required by BiosServerGetSeqState

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param SeqState                          - Pointer to buffer where sequence state will be stored.

  @retval N/A

**/
VOID
BiosServerGetSeqStateChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 ChannelInSocket,
  SSA_SEQ_STATE         *SeqState
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetStopMode

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param StopMode                          - CPGC stop mode.
  @param StopOnNthErrorCount               - Error count for stop-on-nth-any-lane error mode.  Minimum value is 1.  The value must be less than or equal to the MaxStopOnNthErrorCountVal value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetStopModeChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  CPGC_STOP_MODE        StopMode,
  UINT32                StopOnNthErrorCount
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetErrorCounterMode

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Counter                           - Zero based error counter number.  The value must be less than or equal to the MaxNumberErrorCounters value from the GetCpgcInfo() function.
  @param Mode                              - Error counter mode.
  @param ModeIndex                         - Extra index used to provide additional information if needed by the mode selected. This indicates which lane, nibble, byte, or chunk has been selected.  If CounterMode value is LaneErrorCounterMode then the ModeIndex value must be less than the BusWidth value from the GetSystemInfo() function.  If CounterMode is NibbleErrorCounterMode then the ModeIndex value must be less than the BusWidth/4 value from the GetSystemInfo() function.  If CounterMode is ByteErrorCounterMode then the ModeIndex value must be less than the BusWidth/8 value from the GetSystemInfo() function.  If CounterMode is ChunkErrorCounterMode then the ModeIndex value must be less than 8.

  @retval N/A

**/
VOID
BiosServerSetErrorCounterModeChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Counter,
  ERROR_COUNTER_MODE    Mode,
  UINT32                ModeIndex
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetLaneValidationMask

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Counter                           - Zero based error counter number.  The value must be less than or equal to the MaxNumberErrorCounters value from the GetCpgcInfo() function.
  @param DqMaskLow                         - Lower 32 bits of the DQ Mask (0-based)
  @param DqMaskHigh                        - Upper 32 bits of the DQ Mask (0-based)
  @param EccMask                           - Ecc Mask (0-based)


  @retval N/A

**/
VOID
BiosServerSetLaneValidationMaskChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT32                DqMaskLow,
  UINT32                DqMaskHigh,
  UINT32                EccMask32
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetChunkValidationMask

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param ChunkPairsToCheck                 - One bit for each chunk of a cacheline. Used to identify each pair of chunks to be checked.

  @retval N/A

**/
VOID
BiosServerSetChunkValidationMaskChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 ChunkPairsToCheck
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetCachelineValidationMask

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param CachelineMask                     - Mask for cachelines to be enabled.  Bit value 1 = perform validation; bit value 0 = do not perform validation.  Bit position 0 = cacheline 0; bit position 1 = cacheline 1, etc.  Note that this may not be the same logic as the hardware.

  @retval N/A

**/
VOID
BiosServerSetCachelineValidationMaskChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 CachelineMask
)
{

}

/**

  This routine gets the CSRs required by BiosServerGetLaneErrorStatus

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param ChunkMask                         - Mask for the chunks. One bit for each chunk.  This mask should set to be the same value as the mask being passed to the SetChunkValidationMask() function.  Products don't have separate error status for even and odd UI should ignore this parameter.
  @param Status                            - Pointer to array where the lane error status values will be stored.  The number of array elements is the BusWidth value from the GetSystemInfo() function divided by 8.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.

  @retval N/A

**/
VOID
BiosServerGetLaneErrorStatusChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 ChunkMask,
  UINT8                 *Status
)
{

}

/**

  This routine gets the CSRs required by BiosServerGetChunkErrorStatus

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Status                            - Pointer to where the chunk error status will be stored.  Bit value 1 = at least one error occurred; bit value 0 = no errors occurred. Bit position 0 = chunk 0; bit position 1 = chunk 1, etc.

  @retval N/A

**/
VOID
BiosServerGetChunkErrorStatusChip (
  PSYSHOST                       Host,
  UINT8                          Socket,
  UINT8                          ChannelInSocket,
  UINT8                          *Status
)
{

}

/**

  This routine gets the CSRs required by BiosServerGetErrorAddress

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Address                           - Pointer to buffer where error address will be stored.

  @retval N/A

**/
VOID
BiosServerGetErrorAddressChip (
  PSYSHOST                    Host,
  UINT8                       Socket,
  UINT8                       ChannelInSocket,
  SSA_ADDRESS                 *Address
)
{

}

/**

  This routine gets the CSRs required by BiosServerGetErrorCount

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Counter                           - Zero based counter number.  The value must be less than or equal to the MaxNumberErrorCounters value from the GetCpgcInfo() function.
  @param Count                             - Pointer to where the error counter value will be stored.
  @param Overflow                          - Pointer to where the error counter overflow status will be stored.

  @retval N/A

**/
VOID
BiosServerGetErrorCountChip (
  PSYSHOST                 Host,
  UINT8                    Socket,
  UINT8                    ChannelInSocket,
  UINT8                    Counter,
  UINT32                   *Count,
  BOOLEAN                  *Overflow
)
{

}

/**

  This routine sets the CSRs required by BiosServerSetDqdbErrorCaptureConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Enable                            - If set to TRUE then cachelines for which errors are detected are written to the DqDB starting at the StartCacheline and continuing to the EndCacheline before wrapping around to the StartCacheline.  If set to FALSE then the DqDB error capture feature is disabled.
  @param StartCacheline                    - The starting cacheline in the DqDB where the error status will be captured.  The value must be less than the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.
  @param EndCacheline                      - The ending cacheline in the DqDB where the error status will be captured.  The value must be greater than or equal to the StartCacheline value.  The value must be less than the MaxNumberDqdbCachelines value from the GetCpgcInfo() function.

  @retval N/A

**/
VOID
BiosServerSetDqdbErrorCaptureConfigChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               Enable,
  UINT8                                 StartCacheline,
  UINT8                                 EndCacheline
  )
{

}

/**

  This routine gets the CSRs required by BiosServerGetDqdbErrorCaptureStatus

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param CurrentCacheline                  - Pointer to where the current DqDB error capture cacheline value will be stored.

  @retval N/A

**/
VOID
BiosServerGetDqdbErrorCaptureStatusChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 *CurrentCacheline
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetLocalTestControl

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller Id (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param ClearErrors                       - Setting this parameter to TRUE causes all the channel's error counters and status to be cleared.
  @param StartTest                         - Setting this parameter to TRUE starts the channel's CPGC engine.
  @param StopTest                          - Setting this parameter to TRUE stops the channel's CPGC engine.

  @retval N/A

**/
VOID
BiosServerSetLocalTestControlChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 Controller,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               ClearErrors,
  BOOLEAN                               StartTest,
  BOOLEAN                               StopTest
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetGlobalTestControl

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller Id (0-based)
  @param ClearErrors                       - Setting this parameter to TRUE causes all the channel's error counters and status to be cleared.
  @param StartTest                         - Setting this parameter to TRUE starts the channel's CPGC engine.
  @param StopTest                          - Setting this parameter to TRUE stops the channel's CPGC engine.

  @retval N/A

**/
VOID
BiosServerSetGlobalTestControlChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 Controller,
  BOOLEAN                               ClearErrors,
  BOOLEAN                               StartTest,
  BOOLEAN                               StopTest
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetAddrConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller Id (0-based)
  @param ChannelInSocket                   - Channel number (0-based)
  @param EnableDdr4                        - Setting this parameter to TRUE causes the CPGC engine to be in the DDR4 test mode, otherwise it's in the NVMDIMM test mode.

  @retval N/A

**/
VOID
BiosServerSetAddrConfigChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 Controller,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               EnableDdr4
  )
{

}


/**

  This routine gets/sets the CSRs required by BiosServerSetScramblerConfig

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Controller                        - Controller Id (0-based)
  @param Enable                            - Specifies whether the scrambler is enabled.
  @param PrevEnable                        - Pointer to where the previous scrambler enable setting will be stored.  This value may be NULL.

  @retval N/A

**/
VOID
BiosServerSetScramblerConfigChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 Controller,
  BOOLEAN                               Enable,
  BOOLEAN                               *PrevEnable
  )
{
  UINT8                                ChannelInSocket = 0;
  UINT8                                MaxChannel = 0;
  UINT8                                Iteration = 0;
  UINT8                                NumChPerMc;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT     ScrambleConfig;

  NumChPerMc = GetNumChannelPerMc ();

  ChannelInSocket = (Controller * NumChPerMc);
  MaxChannel = (Controller+1) * NumChPerMc ;

  for (ChannelInSocket; ChannelInSocket < MaxChannel; ChannelInSocket++) {

    if (IsChannelEnabled (Socket, ChannelInSocket) == FALSE) {
      continue;
    }

    ScrambleConfig.Data = MemReadPciCfgEp (Socket, ChannelInSocket, MCSCRAMBLECONFIG_MCDDC_DP_REG);

    if ((PrevEnable != NULL) && (Iteration == 0)) {  //Get Case... Save Previous value for only the First Enabled Channel
      *PrevEnable = (ScrambleConfig.Bits.ch_enable & 1);
    }

    if (Enable == TRUE) {
      ScrambleConfig.Bits.rx_enable = 1;
      ScrambleConfig.Bits.tx_enable = 1;
      ScrambleConfig.Bits.ch_enable = 1;
    } else {
      ScrambleConfig.Bits.rx_enable = 0;
      ScrambleConfig.Bits.tx_enable = 0;
      ScrambleConfig.Bits.ch_enable = 0;
    }

    MemWritePciCfgEp (Socket, ChannelInSocket, MCSCRAMBLECONFIG_MCDDC_DP_REG, ScrambleConfig.Data);
    Iteration = 1;
  }
}

/**

  This routine sets the CSRs required by BiosServerSetRefreshOverride

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param EnablePanicRefreshOnly            - If set to TRUE then opportunistic and high priority auto refreshes will be disabled for all ranks and refreshes will only occur when the panic refresh watermark has been exceeded.  If set to FALSE then refreshes will occur using the existing MC functional refresh logic.
  @param PairCount                         - Number of entries in the DimmRankPairs array.  If you wish to have no rank's refresh overridden then set PairCnt=0 and DimmRankPairs=NULL.  The value must be less than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank appears in the list, then the CPGC engine will disable executing any refresh to that rank while in REUT mode. If a DIMM/rank doesn't appear in the list then the CPGC engine will enable executing a refresh (assuming the MC has refreshes enabled) to that rank while in REUT mode based on the EnablePanicRefreshOnly parameter value.  The valid DIMM/rank pairs can be determined via the GetDimmBitMask() and GetRankInDimm() functions.

  @retval N/A

**/
VOID
BiosServerSetRefreshOverrideChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               EnablePanicRefreshOnly,
  UINT8                                 PairCount,
  CONST DIMM_RANK_PAIR                  *DimmRankPairs
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetZQCalOverride

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param EnableZQCalAfterRefresh           - If set to TRUE then a ZQ operation will always take place after a refresh occurs as INT32 as the DIMM/rank does not appear in the DimmRankPairs array.  If set to FALSE then a ZQ operation will take place at the normal programmed period after a refresh occurs as INT32 as the DIMM/rank does not appear in the DimmRankPairs array.
  @param PairCount                         - Number of entries in the DimmRankPairs array.  If you wish to have no rank's ZQCal overridden then set PairCnt=0 and DimmRankPairs=NULL.  The value must be less than the product of the MaxNumberDimms and MaxNumberRanks values from the GetSystemInfo() function.
  @param DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank appears in the list, then the CPGC engine will disable executing any ZQCal to that rank while in REUT mode.  The valid DIMM/rank pairs can be determined via the GetDimmBitMask() and GetRankInDimm() functions.

  @retval N/A

**/
VOID
BiosServerSetZQCalOverrideChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               EnableZQCalAfterRefresh,
  UINT8                                 PairCount,
  CONST DIMM_RANK_PAIR                  *DimmRankPairs
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetOdtOverride

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param EnableMprDdrTrain                 - This parameter is set to TRUE during DDR training.
  @param ValCount                          - Number of entries in the DimmRankPairs and OverrideVals arrays.  If you wish to have no rank's ODT overridden then set ValCnt=0, DimmRankPairs=NULL, and OverrideVals=NULL.  The value must be less than twice the MaxNumberDimms value from the GetSystemInfo() function.
  @param DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank appears in this array then its ODT value will be overridden with the value from the corresponding index in the OverrideVals array.  If a DIMM/rank does not appear in this array then its ODT will not be overridden.  The valid DIMM/rank pairs can be determined via the GetDimmBitMask() and GetRankInDimm() functions.
  @param OverrideVals                      - Pointer to array of override values.  Value 1 = enable ODT for that rank while in REUT mode; value 0 = disable ODT for that rank while in REUT mode.

  @retval N/A

**/
VOID
BiosServerSetOdtOverrideChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  BOOLEAN                               EnableMprDdrTrain,
  UINT8                                 ValCount,
  CONST DIMM_RANK_PAIR                  *DimmRankPairs,
  CONST UINT8                           *OverrideVals
  )
{

}

/**

  This routine sets the CSRs required by BiosServerSetCkeOverride

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - ChannelInSocket No. (0-based)
  @param OverrideVals                      - Pointer to array of override values.  Value 1 = enable ODT for that rank while in REUT mode; value 0 = disable ODT for that rank while in REUT mode.
  @param ValCount                          - Number of entries in the DimmRankPairs and OverrideVals arrays.  If you wish to have no rank's CKE overridden then set ValCnt=0, DimmRankPairs=NULL, and OverrideVals=NULL.  The value must be less than the MaxNumberRankMapEntries value from the GetCpgcInfo() function.
  @param DimmRankPairs                     - Pointer to array of DIMM and physical rank pair values.  If a DIMM/rank appears in this array then its CKE value will be overridden with the value from the corresponding index in the OverrideVals array.  If a DIMM/rank does not appear in this array then its CKE will not be overridden.  The valid DIMM/rank pairs can be determined via the GetDimmBitMask() and GetRankInDimm() functions.
  @param OverrideVals                      - Pointer to array of override values.  Value 1 = enable CKE for that rank while in REUT mode; value 0 = disable CKE for that rank while in REUT mode.

  @retval N/A

**/
VOID
BiosServerSetCkeOverrideChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 ValCount,
  CONST DIMM_RANK_PAIR                  *DimmRankPairs,
  CONST UINT8                           *OverrideVals
  )
{

}
// end file BiosSsaMemoryServerConfig.c


