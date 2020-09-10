/** @file
  MemDramDutyCycleAdjuster.c
  Includes functions required to execute the DRAM Duty Cycle Adjuster training

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "Ddr5Core.h"
#include <Library/MemoryCoreLib.h>
#include <Memory/Ddr5MrRegs.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/RcDebugLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemRoutingLib.h>
#include "MemDramDutyCycleAdjuster.h"

#if defined(USE_LATEST_DRAM_SPEC)

#define ALL_CHANNELS_MASKED_OFF     MAX_UINT32
#define ALL_RANKS_MASKED_OFF        MAX_UINT32

//
// Values to use when margining RxDqsDelay
//
// Starting negative offset from the center point
#define SWEEP_START_OFFSET_LEFT        (-10)
// Starting positive offset from the center point
#define SWEEP_START_OFFSET_RIGHT       (10)
// Maximum offset on either side
#define RXDQSDELAY_OFFSET_LIMIT        (128)

//
// Number of consecutive UIs to margin in parallel
//
#define CHUNK_MASK_WIDTH        2
#define CHUNK_MASK ((1 << CHUNK_MASK_WIDTH) - 1)

//
// ============ DRAM Duty Cycle Adjuster Register Access Functions ============
//
/**
  Determine if all items in a list are equal

  @param[in] Data         - Pointer to the list
  @param[in] ListLength   - Length of the list

  @retval = TRUE   if all items match
  @retval = FALSE  otherwise
**/
BOOLEAN
AllListItemsMatch (
  IN UINT8    *List,
  IN UINT8    ListLength
  )
{
  UINT32      CommonValue;
  UINT8       Index;

  CommonValue = List[0];
  for (Index = 1; Index < ListLength; Index++) {

    if (List[Index] != CommonValue) {
      return FALSE;
    }
  } // Index

  return TRUE;
}

/**
  Compare two lists of UINT8

  @param[in] ListA        - Pointer to first list
  @param[in] ListB        - Pointer to second list
  @param[in] ListLength   - Length of the lists

  @retval = TRUE   if lists are the same
  @retval = FALSE  otherwise
**/
BOOLEAN
ListMatchUint8 (
  IN UINT8    *ListA,
  IN UINT8    *ListB,
  IN UINT8    ListLength
  )
{
  UINT8       Index;

  for (Index = 0; Index < ListLength; Index++) {

    if (ListA[Index] != ListB[Index]) {
      return FALSE;
    }
  } // Index

  return TRUE;
}

/**
  Convert DRAM Duty Cycle Adjuster value to the register format
  Assumes register value limits have been applied

  @param[in]  Group               - Enumerated group type - DramDcaQClk, DramDcaIbClk or DramDcaQbClk
  @param[in]  Value               - Value of the group
  @param[in]  OriginalRegData     - Original contents of the entire register
  @param[out] NewRegData          - Contents of the entire register after applying the new Value
  @param[out] RegAddress          - Address of the mode register

  @retval = MRC_STATUS_SUCCESS if conversion is successful
  @retval = MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
ConvertValueToDramDutyCycleAdjusterRegFormat (
  IN      MRC_GT    Group,
  IN      INT16     Value,
  IN      UINT8     OriginalRegData,
     OUT  UINT8     *NewRegData,
     OUT  UINT8     *RegAddress
  )
{
  UINT8                         SignBit;
  UINT8                         AbsValue;
  DDR5_MODE_REGISTER_43_STRUCT  Mr43;
  DDR5_MODE_REGISTER_44_STRUCT  Mr44;

  if ((NewRegData == NULL) ||
      (RegAddress == NULL))
  {
    RC_FATAL_ERROR (FALSE, ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_004);
    return MRC_STATUS_FAILURE;
  }

  //
  // Convert the new signed value to register format
  // Function assumes caller has applied register limits to Value
  //
  if (Value < 0) {
    SignBit = 1;
  } else {
    SignBit = 0;
  }
  AbsValue = (UINT8)(ABS (Value));

  if ((Group == DramDcaQClk) || (Group == DramDcaIbClk) || (Group == DramDcaClk)) {

    Mr43.Data = OriginalRegData;

    if ((Group == DramDcaQClk) || (Group == DramDcaClk)) {

      Mr43.Bits.dca_for_single_two_phase_clock_s_or_qclk_in_4_phase_clocks = AbsValue;
      Mr43.Bits.sign_bit_for_op_2_0                                        = SignBit;

    } else {

      Mr43.Bits.dca_for_ibclk_in_4_phase_clocks = AbsValue;
      Mr43.Bits.sign_bit_for_op_6_4             = SignBit;

    }
    *NewRegData = Mr43.Data;
    *RegAddress = DDR5_MODE_REGISTER_43_REG;

  } else if (Group == DramDcaQbClk) {

    Mr44.Data = OriginalRegData;

    Mr44.Bits.dca_for_qbclk_in_4_phase_clocks     = AbsValue;
    Mr44.Bits.sign_bit_for_qbclk_in_4phase_clocks = SignBit;

    *NewRegData = Mr44.Data;
    *RegAddress = DDR5_MODE_REGISTER_44_REG;
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Read the DRAM Duty Cycle Adjuster for a specified clock
  The clock is identified by the Group parameter.
  Valid Group parameter values are: DramDcaQClk, DramDcaIbClk and DramDcaQbClk

  @param[in] Host               - Pointer to SysHost
  @param[in] Socket             - Socket number
  @param[in] Channel            - Channel number within the socket
  @param[in] Dimm               - DIMM number within the channel
  @param[in] Rank               - Rank number of the DIMM
  @param[in] Strobe             - Logical strobe number in the full channel
  @param[in] Bit                - Bit number in the strobe
  @param[in] Group              - Enumerated group type - DramDcaQClk, DramDcaIbClk or DramDcaQbClk
  @param[in] Mode               - GSM_READ_CSR - Read the data from hardware and not cache
                                - GSM_READ_ONLY - Do not write
                                - GSM_WRITE_OFFSET - Write offset and not value
                                - GSM_FORCE_WRITE - Force the write
  @param[out] OriginalRegData   - Contents of the entire register
  @param[out] Value             - Value of Group

  @retval MRC_STATUS_SUCCESS if the transaction succeeds
  @retval MRC_STATUS_FAILURE if input parameter is invalid

**/
MRC_STATUS
GetDramDutyCycleAdjuster (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    Dimm,
  IN     UINT8    Rank,
  IN     UINT8    Strobe,
  IN     UINT8    Bit,
  IN     MRC_GT   Group,
  IN     UINT8    Mode,
     OUT UINT8    *OriginalRegData,
     OUT INT16    *Value
  )
{
  UINT8                         Address;
  UINT8                         SignBit;
  UINT8                         SubChannel;
  UINT8                         StrobeSubChannel;
  UINT8                         MSVx4;
  DDR5_MODE_REGISTER_43_STRUCT  Mr43;
  DDR5_MODE_REGISTER_44_STRUCT  Mr44;

  MSVx4       = GetMaxStrobeValid (Host);

  if ((Strobe >= MSVx4) ||
      (Bit != ALL_BITS)      ||
      ((Mode & GSM_READ_CSR) != 0))
  {
    //
    // Function does not support per-bit registers, ALL_STROBES or GSM_READ_CSR
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_239);
    return MRC_STATUS_FAILURE;
  }

  //
  // ReadDramModeRegCachePerDevice takes a subchannel and subchannel-based strobe number
  //
  StrobeSubChannel = Strobe;
  SubChannel = Strobe / (MSVx4 / 2);
  if (SubChannel == SUB_CH_B) {
    StrobeSubChannel = Strobe - (MSVx4 / 2);
  }

  if ((Group == DramDcaClk) || (Group == DramDcaQClk) || (Group == DramDcaIbClk)) {

    Address = DDR5_MODE_REGISTER_43_REG;
    ReadDramModeRegCachePerDevice (Socket, Channel, SubChannel, Dimm, StrobeSubChannel, Rank, Address, &Mr43.Data);

    if ((Group == DramDcaQClk) || (Group == DramDcaClk)) {
      *Value = Mr43.Bits.dca_for_single_two_phase_clock_s_or_qclk_in_4_phase_clocks;
      SignBit  = Mr43.Bits.sign_bit_for_op_2_0;
    } else {
      *Value = Mr43.Bits.dca_for_ibclk_in_4_phase_clocks;
      SignBit  = Mr43.Bits.sign_bit_for_op_6_4;
    }
    *OriginalRegData = Mr43.Data;

  } else if (Group == DramDcaQbClk) {

    Address = DDR5_MODE_REGISTER_44_REG;
    ReadDramModeRegCachePerDevice (Socket, Channel, SubChannel, Dimm, StrobeSubChannel, Rank, Address, &Mr44.Data);

    *Value = Mr44.Bits.dca_for_qbclk_in_4_phase_clocks;
    SignBit = Mr44.Bits.sign_bit_for_qbclk_in_4phase_clocks;
    *OriginalRegData = Mr44.Data;

  } else {

    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_236);
    return MRC_STATUS_FAILURE;

  }

  if (SignBit == 1) {
    *Value = -(*Value);
  }
  return MRC_STATUS_SUCCESS;
} // GetDramDutyCycleAdjuster

/**

  Write the DRAM Duty Cycle Adjuster for a specified clock
  The clock is identified by the Group parameter.
  Valid Group parameter values are: DramDcaQClk, DramDcaIbClk and DramDcaQbClk
  This function will perform the fewest register writes possible and avoid using PDA when appropriate.

  @param[in] Host               - Pointer to SysHost
  @param[in] Socket             - Socket number
  @param[in] Channel            - Channel number within the socket
  @param[in] Dimm               - DIMM number within the channel
  @param[in] Rank               - Rank number of the DIMM
  @param[in] Strobe             - Logical strobe number in the full channel
  @param[in] Bit                - Bit number in the strobe
  @param[in] Group              - Enumerated group type - DramDcaQClk, DramDcaIbClk or DramDcaQbClk
  @param[in] Mode               - GSM_READ_CSR - Read the data from hardware and not cache
                                - GSM_READ_ONLY - Do not write
                                - GSM_WRITE_OFFSET - Write offset and not value
                                - GSM_FORCE_WRITE - Force the write
  @param[in] OriginalRegData    - Current contents of the entire register for every strobe
  @param[in] Value              - Value of Group for every strobe

  @retval MRC_STATUS_SUCCESS if the transaction succeeds
  @retval MRC_STATUS_FAILURE if input parameter is invalid

**/
MRC_STATUS
SetDramDutyCycleAdjuster (
  IN     PSYSHOST   Host,
  IN     UINT8      Socket,
  IN     UINT8      Channel,
  IN     UINT8      Dimm,
  IN     UINT8      Rank,
  IN     UINT8      Strobe,
  IN     UINT8      Bit,
  IN     MRC_GT     Group,
  IN     UINT8      Mode,
  IN     UINT8      OriginalRegData[MAX_STROBE],
  IN     INT16      Value[MAX_STROBE]
  )
{
  UINT8                         RegAddress;
  UINT8                         SubChannel;
  UINT8                         SubChannelStrobe;
  UINT8                         StrobeLocal;
  UINT8                         StartStrobe;
  UINT8                         EndStrobe;
  INT16                         MinLimit;
  INT16                         MaxLimit;
  INT16                         NewValue;
  UINT8                         NewRegData[MAX_STROBE];
  UINT8                         MSVx4;
  UINT8                         MSVx4SubCh;
  UINT8                         MSVx8SubCh;
  UINT8                         MaxStrobeSubCh;

  RegAddress = 0;
  MSVx4       = GetMaxStrobeValid (Host);
  MSVx4SubCh  = GetSubChMaxStrobeValid (Host);
  MSVx8SubCh  = MSVx4SubCh / 2;

  MaxStrobeSubCh = MSVx8SubCh;
  if (IsX4Dimm (Socket, Channel, Dimm)) {
    MaxStrobeSubCh = MSVx4SubCh;
  }

  if (((Strobe >= MSVx4) && (Strobe != ALL_STROBES)) ||
      (OriginalRegData == NULL)                      ||
      (Bit != ALL_BITS))
  {
    //
    // Function does not support per-bit registers
    // OriginalRegData must be valid
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_237);
    return MRC_STATUS_FAILURE;
  }

  ZeroMem (NewRegData, sizeof (NewRegData));

  if (Strobe < MAX_STROBE) {
    StartStrobe = Strobe;
    EndStrobe = Strobe + 1;
  } else {
    StartStrobe = 0;
    EndStrobe = MSVx4;
  }

  MinLimit = MIN_DRAM_CLOCK_DCA;
  MaxLimit = MAX_DRAM_CLOCK_DCA;

  //
  // Convert raw Value into register format
  //
  for (StrobeLocal = StartStrobe; StrobeLocal < EndStrobe; StrobeLocal++) {

    //
    // Skip strobes not used by this DIMM
    // For x4 devices, valid logical strobe numbers in the channel are 0-9 and 10-19
    // For x8/x16 devices, valid logical strobe numbers in the channel are 0-4 and 10-14
    //
    if (IsStrobeNotValidChDdr5 (Host, StrobeLocal) ||
        (!VALID_LOGICAL_STROBE (StrobeLocal, MSVx4, MaxStrobeSubCh)))
    {
        continue;
    }

    //
    // Enforce register limits
    //
    NewValue = Value[StrobeLocal];
    if (Value[StrobeLocal] < MinLimit) {
      RcDebugPrintWithDevice(SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, StrobeLocal, NO_BIT,
            "%a Out of range!! NewValue = 0x%x, MinLimit = 0x%x\n", GetGroupStr (Group), Value[StrobeLocal], MinLimit);
      NewValue = MinLimit;
    }
    if (Value[StrobeLocal] > MaxLimit) {
      RcDebugPrintWithDevice(SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, StrobeLocal, NO_BIT,
            "%a Out of range!! NewValue = 0x%x, MaxLimit = 0x%x\n", GetGroupStr (Group), Value[StrobeLocal], MaxLimit);
      NewValue = MaxLimit;
    }
    ConvertValueToDramDutyCycleAdjusterRegFormat (Group, NewValue, OriginalRegData[StrobeLocal], &NewRegData[StrobeLocal], &RegAddress);

  } // StrobeLocal

  //
  // Write to the DRAM register and update the cache
  //
  if (Strobe != ALL_STROBES) {
    //
    // Writing to a single strobe
    //
    SubChannel = Strobe / (MAX_STROBE / 2);
    SubChannelStrobe = Strobe % (MAX_STROBE / 2);
    //
    // Skip the update if this specific register didn't changed
    //
    if (NewRegData[Strobe] != OriginalRegData[Strobe]) {
      //
      // Write to the DRAM and update the cache
      // Convert logical channel strobe to physical subchannel strobe number
      //
      DramModeRegWritePda (Socket, Channel, 1 << SubChannel, Dimm, Rank, SubChannelStrobe, RegAddress, NewRegData[Strobe]);
    }

    return MRC_STATUS_SUCCESS;
  }

  //
  // Writing to all strobes in the rank
  //
  for (SubChannel = SUB_CH_A; SubChannel < SUB_CH; SubChannel++) {
    //
    // Find the correct Start and End strobe numbers (logical channel-based strobe numbers) for SubChannel
    // For ALL_STROBES, Start is the first strobe in SubChannel and End is Start + (number of strobes in a subchannel)
    //
    StartStrobe = SubChannel * (MAX_STROBE / 2);
    EndStrobe   = StartStrobe + (MAX_STROBE / 2);

    //
    // Broadcast writes can be used if the entire register is the same for all devices in the subchannel
    // Otherwise, must use PDA
    //
    if (AllListItemsMatch (&NewRegData[StartStrobe], MaxStrobeSubCh)) {
      //
      // All strobe values are the same - broadcast to the entire rank with a single MRW
      // Skip the update if the register data didn't change
      //
      if (!ListMatchUint8 (&NewRegData[StartStrobe], &OriginalRegData[StartStrobe], EndStrobe - StartStrobe)) {

        DramModeRegWrite (Socket, Channel, 1 << SubChannel, Dimm, Rank, RegAddress, NewRegData[StartStrobe]);

      }

    } else { // AllListItemsMatch

      for (StrobeLocal = StartStrobe; StrobeLocal < EndStrobe; StrobeLocal++) {

        if (IsStrobeNotValidChDdr5 (Host, StrobeLocal) ||
            (!VALID_LOGICAL_STROBE (StrobeLocal, MSVx4, MaxStrobeSubCh)))
        {
            continue;
        }

        //
        // Skip the update if this specific register didn't changed
        //
        if (NewRegData[StrobeLocal] != OriginalRegData[StrobeLocal]) {
          //
          // Write to the DRAM and update the cache
          // Convert logical channel strobe to physical subchannel strobe number
          //
          SubChannelStrobe = StrobeLocal % (MAX_STROBE / 2);
          DramModeRegWritePda (Socket, Channel, 1 << SubChannel, Dimm, Rank, SubChannelStrobe, RegAddress, NewRegData[StrobeLocal]);
        }

      } // StrobeLocal

    } // AllListItemsMatch
  } // SubChannel

  return MRC_STATUS_SUCCESS;
} // SetDramDutyCycleAdjuster

/**

  Read or write the DRAM Duty Cycle Adjuster for a specified clock
  The clock is identified by the Group parameter.
  Valid Group parameter values are: DramDcaQClk, DramDcaIbClk and DramDcaQbClk

  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number of the DIMM
  @param[in] Strobe           - Logical strobe number in the full channel
  @param[in] Bit              - Bit number in the strobe
  @param[in] Group            - Enumerated group type - DramDcaQClk, DramDcaIbClk or DramDcaQbClk
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program or read

  @retval MRC_STATUS_SUCCESS if the transaction succeeds
  @retval MRC_STATUS_FAILURE if input parameter is invalid

**/
MRC_STATUS
GetSetDramDutyCycleAdjuster (
  IN    UINT8     Socket,
  IN    UINT8     Channel,
  IN    UINT8     Dimm,
  IN    UINT8     Rank,
  IN    UINT8     Strobe,
  IN    UINT8     Bit,
  IN    MRC_GT    Group,
  IN    UINT8     Mode,
  IN OUT INT16    *Value
  )
{
  PSYSHOST                      Host;
  UINT8                         StrobeLocal;
  UINT8                         StartStrobe;
  UINT8                         EndStrobe;
  INT16                         OriginalValue[MAX_STROBE];
  INT16                         NewValue[MAX_STROBE];
  UINT8                         MaxChDdr;
  UINT8                         OriginalRegData[MAX_STROBE];
  UINT8                         MSVx4;
  UINT8                         MSVx4SubCh;
  UINT8                         MSVx8SubCh;
  UINT8                         MaxStrobeSubCh;

  MaxChDdr = GetMaxChDdr ();
  if ((Socket     >= MAX_SOCKET)       ||
      (Channel    >= MaxChDdr)         ||
      (Dimm       >= MAX_DIMM)         ||
      (Rank       >= MAX_RANK_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING - GetSetDramDutyCycleAdjuster called with an invalid parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_238);
    return MRC_STATUS_FAILURE;
  }

  Host        = GetSysHostPointer();
  MSVx4       = GetMaxStrobeValid (Host);
  MSVx4SubCh  = GetSubChMaxStrobeValid (Host);
  MSVx8SubCh  = MSVx4SubCh / 2;

  MaxStrobeSubCh = MSVx8SubCh;
  if (IsX4Dimm (Socket, Channel, Dimm)) {
    MaxStrobeSubCh = MSVx4SubCh;
  }

  if (((Strobe >= MSVx4) && (Strobe != ALL_STROBES)) ||
      (Bit != ALL_BITS)) {
    //
    // Function does not support per-bit programming
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_239);
    return MRC_STATUS_FAILURE;
  }

  ZeroMem (OriginalValue, sizeof (OriginalValue));
  ZeroMem (NewValue, sizeof (NewValue));
  ZeroMem (OriginalRegData, sizeof (OriginalRegData));

  if (Strobe < MSVx4) {
    StartStrobe = Strobe;
    EndStrobe = Strobe + 1;
  } else {
    StartStrobe = 0;
    EndStrobe = MSVx4;
  }

  for (StrobeLocal = StartStrobe; StrobeLocal < EndStrobe; StrobeLocal++) {

    //
    // Skip strobes not used by this DIMM
    //
    if (IsStrobeNotValidChDdr5 (Host, StrobeLocal) || 
        (!VALID_LOGICAL_STROBE (StrobeLocal, MSVx4, MaxStrobeSubCh)))
    {
      continue;
    }

    //
    // Read the current Group value and the value of the entire register for later comparison
    //
    GetDramDutyCycleAdjuster (Host, Socket, Channel, Dimm, Rank, StrobeLocal, Bit, Group, Mode, &OriginalRegData[StrobeLocal], &OriginalValue[StrobeLocal]);

  } // StrobeLocal

  if (Mode & GSM_READ_ONLY) {

    //
    // For reads, return value of the strobe specified by the caller (or strobe 0 for ALL_STROBES)
    //
    *Value = OriginalValue[StartStrobe];

    return MRC_STATUS_SUCCESS;

  }

  for (StrobeLocal = StartStrobe; StrobeLocal < EndStrobe; StrobeLocal++) {

    //
    // Skip strobes not used by this DIMM
    //
    if (IsStrobeNotValidChDdr5 (Host, StrobeLocal) || 
        (!VALID_LOGICAL_STROBE (StrobeLocal, MSVx4, MaxStrobeSubCh)))
    {
      continue;
    }

    if (Mode & GSM_WRITE_OFFSET) {

      //
      // Add the offset to the current value
      //
      NewValue[StrobeLocal] = OriginalValue[StrobeLocal] + *Value;

    } else {

      //
      // Write absolute value
      //
      NewValue[StrobeLocal] = *Value;

    }
  } // StrobeLocal

  SetDramDutyCycleAdjuster (Host, Socket, Channel, Dimm, Rank, Strobe, Bit, Group, Mode, OriginalRegData, NewValue);


  return MRC_STATUS_SUCCESS;

} // GetSetDramDutyCycleAdjuster

//
// ============ DRAM Duty Cycle Adjuster Training Functions ============
//

/**
  Return a pointer to a string corresponding to the DCA Type

  @param[in]  DcaType   - The DCA type

  @retval Pointer to string
**/
CHAR8 *
GetDcaTypeString (
  IN      DCA_TYPE      DcaType
  )
{
  CHAR8     *DcaTypeString;
  CHAR8     *StringList[] = {"DcaNotSupported", "DcaSingleOrTwoPhase", "DcaFourPhase"};

  DcaTypeString = "Unknown";

  if (DcaType < ARRAY_SIZE (StringList)) {
    DcaTypeString = StringList[DcaType];
  }
  return DcaTypeString;
}

/**

  Determine if a specific type of DRAM Duty Cycle Adjuster is supported by the Rank

  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  Dimm            - Dimm number
  @param[in]  DcaTypeToCheck  - The type of DCA to check for

  @retval TRUE if the DIMM supports the specified DCA type
  @retval FALSE otherwise

**/
BOOLEAN
IsDramDcaSupportedRank (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  DCA_TYPE  DcaTypeToCheck
  )
{
  DCA_TYPE                      DcaType;
  DDR5_MODE_REGISTER_42_STRUCT  Mr42;

  DcaType = DcaNotSupported;
  //
  // Assume DRAM capabilities are the same across both subchannels and only read register for subchannel A
  //
  if (ReadDramModeRegCachePerRank (Socket, Channel, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_42_REG, &Mr42.Data) == MRC_STATUS_SUCCESS) {

    DcaType = Mr42.Bits.dca_types_supported;
  }

  if (DcaType == DcaTypeToCheck) {
    //
    // Some rank in the DIMM supports the caller's DRAM Duty Cycle Adjuster type
    //
    return TRUE;
  }
  return FALSE;
}

/**

  Determine if a specific type of DRAM Duty Cycle Adjuster is supported by any DIMM in the socket.
  Return channel mask and rank mask indicating which channels and ranks support the DCA type.

  @param[in]  Socket          - Socket number
  @param[in]  ChannelMask       One-hot encoding to mask a channel (0: DcaType supported, 1: DcaType not supported)
  @param[in]  RankMask          One-hot encoding to mask a rank per channel (0: DcaType supported, 1: DcaType not supported)
  @pararm[in] DcaTypeToCheck  - The type of DCA to check for

  @retval N/A

**/
BOOLEAN
GetDramDcaSupportedSocket (
  IN      UINT8         Socket,
     OUT  UINT32        *ChannelMask,
     OUT  UINT32        RankMask[MAX_CH],
  IN      DCA_TYPE      DcaTypeToCheck
  )
{
  PSYSHOST    Host;
  UINT8       Channel;
  UINT8       Dimm;
  UINT8       Rank;
  UINT8       NumRanks;
  UINT8       MaxChDdr;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  //
  // Initialize no channels and no ranks enabled
  //
  *ChannelMask = MAX_UINT32;
  SetMem (RankMask, sizeof (*RankMask) * MAX_CH, MAX_UINT8);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      //
      // JEDEC currently does not define support for LRDIMM DCA training
      //
      if (IsLrdimmPresent (Socket, Channel, Dimm) == 1) {
        continue;
      }

      NumRanks = GetNumberOfRanksOnDimm (Socket, Channel, Dimm);

      for (Rank = 0; Rank < NumRanks; Rank++) {
        if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
          continue;
        }

        if (IsDramDcaSupportedRank (Socket, Channel, Dimm, Rank, DcaTypeToCheck)) {
          //
          // Mark this channel as needing training
          // One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
          //
          *ChannelMask &= ~(1 << Channel);
          //
          // Mark this rank as neeing training
          // One-hot encoding to mask a rank (0: executes if enabled/populated, 1: skip/masked)
          //
          RankMask[Channel] &= ~(1 << ((Dimm * MAX_RANK_DIMM) + Rank));
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "DCA Supported - type: 0x%x(%a)\n", DcaTypeToCheck, GetDcaTypeString (DcaTypeToCheck));
        }
      } // Rank
    } // Dimm
  } // Channel

  if (*ChannelMask != MAX_UINT32) {
    //
    // At least one channel has a rank that supports the caller's DRAM Duty Cycle Adjuster type
    //
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Debug only - Initialize the DRAM MRW cache for a single socket with default values

  @param[in]     Socket     - Socket to initialize

  @retval N/A
**/
VOID
InjectDramDcaCapabilities (
  IN      UINT8             Socket
  )
{
}

/**

  Update mode register cache with contents of MR42 - DCA Types Supported - for later use

  @param  Socket     Socket number

  @retval N/A

**/
VOID
CacheDramDcaCapabilities (
  IN  UINT8    Socket
  )
{
  PSYSHOST    Host;
  UINT8       Channel;
  UINT8       Dimm;
  UINT8       Rank;
  UINT8       MaxChDdr;
  UINT8       NumRanks;
  UINT8       Data[SUB_CH][MAX_STROBE_DDR5 / 2];

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      //
      // JEDEC currently does not define support for LRDIMM DCA training
      //
      if (IsLrdimmPresent (Socket, Channel, Dimm) == 1) {
        continue;
      }

      NumRanks = GetNumberOfRanksOnDimm (Socket, Channel, Dimm);

      for (Rank = 0; Rank < NumRanks; Rank++) {
        if (IsRankValid (Socket, Channel, Dimm, Rank, FALSE) == FALSE) {
          continue;
        }
        //
        // DramModeRegRead will update the Mode Register cache for later use
        //
        DramModeRegRead (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_42_REG, &Data);

      } // Rank
    } // Dimm
  } // Channel

  //
  // Load any required non-zero initial mode register values into the cache (simulation only)
  //
  InjectDramDcaCapabilities (Socket);

}

/**
  Program the DRAM DCA Training Mode

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[in]  TrainingAssistMode        - The DCA training assist mode
                                          DCA_TRAINING_ASSIST_DISABLE
                                          DCA_TRAINING_ASSIST_IBCLK
                                          DCA_TRAINING_ASSIST_ICLK

  @retval N/A

**/
VOID
SetDramTrainingAssistMode (
  IN  DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext,
  IN  UINT8                       TrainingAssistMode
)
{
  PSYSHOST                      Host;
  UINT8                         Socket;
  UINT8                         Channel;
  UINT8                         SubChannel;
  UINT8                         MaxChDdr;
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         LogicalRank;
  DDR5_MODE_REGISTER_42_STRUCT  Mr42;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  Socket = DramDcaTrainingContext->Socket;

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {

    if (((1 << Channel) & DramDcaTrainingContext->ChannelMask) != 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

        if (((1 << LogicalRank) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

          ReadDramModeRegCachePerRank (Socket, Channel, SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_42_REG, &Mr42.Data);

          Mr42.Bits.dca_training_assist_mode = TrainingAssistMode;

          DramModeRegWrite (Socket, Channel, 1 << SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_42_REG, Mr42.Data);
        } // SubChannel
      } // Rank
    } // Dimm
  } // Channel
}

/**

  DRAM Duty Cycle Adjuster Training - Setup test pattern in DRAM and CPGC

  @param  Socket     Socket number

  @retval N/A

**/
VOID
DramDcaPatternInit (
  IN UINT8    Socket
  )
{
  PSYSHOST          Host;
  UINT8             Channel;
  UINT8             MaxChDdr;
  UINT16            UiPattern;
  UINT16            PatternInvert;
  READ_PATTERN_MODE PatternMode;


  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  //
  // Initialize read test pattern in the DRAM
  //
  PatternMode.Data = 0;
  PatternMode.Bits.PatternFormat = READ_TRAINING_PATTERN_SERIAL;
  //
  // Use 32-bit CPGC pattern definition. UI pattern in DRAM is only 16-bits.
  //
  UiPattern = (UINT16)(DRAM_DCA_PATTERN_DDR5 & 0xFFFF);
  PatternInvert = 0;
  DramReadPatternInit (Host, Socket, PatternMode, UiPattern, PatternInvert);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }
    //
    // Set mpr_train_ddr_on in host side to filter ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Channel, FILTER_ACT_PRE_ENABLE);
  }  // Channel loop

}

/**

  DRAM Duty Cycle Adjuster Training - Cleanup test pattern, DRAM and CPGC

  @param  Socket     Socket number

  @retval N/A

**/
VOID
DramDcaCleanup (
  IN UINT8    Socket
  )
{
  PSYSHOST    Host;
  UINT8       Channel;
  UINT8       SubChannel;
  UINT8       MaxChDdr;
  UINT32      ChunkMask;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  //
  // Renable error checking on all UIs
  //
  ChunkMask = (1 << MAX_UI) - 1;

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }
    //
    // Set mpr_train_ddr_on in host side to filter ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Channel, FILTER_ACT_PRE_DISABLE);

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

      SetCpgcErrChkMaskChunks (Host, Socket, Channel, SubChannel, ChunkMask);

    } // SubChannel
  } // Channel

}

/**

  Callback function to configure the engine before start doing the RxDqsDelay margin sweep

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

**/
MRC_STATUS
EFIAPI
SetupDramDcaSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8                       Channel;
  UINT8                       SubChannel;
  UINT8                       MaxChDdr;
  UINT32                      ChunkMask;
  DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext;

  MaxChDdr = GetMaxChDdr ();
  DramDcaTrainingContext = (DRAM_DCA_TRAINING_CONTEXT *)CommonParameters;

  SetupSweepAdvance (CommonParameters);

  //
  // Only UIs with a corresponding bit value of 1 will be checked for errors.
  // UIs in the lower half (those from 0 to 7) are expected to have similar response to
  // DCA changes as UIs in the the upper half (those from 8 to 15). Therefore, both lower
  // and upper UIs are included in the mask.
  //
  ChunkMask = (CHUNK_MASK << DramDcaTrainingContext->UiIndex);  // Lower chunks
  ChunkMask |= (CHUNK_MASK << (DramDcaTrainingContext->UiIndex + MAX_UI / 2)); // Upper chunks

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (((1 << Channel) & CommonParameters->ChannelMask) != 0) {
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

      SetCpgcErrChkMaskChunks (CommonParameters->Host, CommonParameters->Socket, Channel, SubChannel, ChunkMask);

    } // SubChannel
  } // Channel

  return MRC_STATUS_SUCCESS;
}

/**
  For each DRAM, store the results of the CLK phase alignment discovery step

  @param[in] DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[in] RankInChannel             - Rank number in the channel

  @retval N/A
**/
VOID
SaveClkPhaseAlignment (
  IN      DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext,
  IN      UINT8                       RankInChannel
  )
{
  UINT8                       Socket;
  UINT8                       StrobeLogical;
  UINT8                       Channel;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT8                       MaxChDdr;
  UINT8                       BitIndex;
  UINT8                       KnobIndex;
  UINT32                      DebugLevel;
  UINT16                      CompositeCondition;
  RESULTS_1D_CONDITION        *ResultConditionPtr;
  COMMON_1D_PARAMETERS        *CommonParameters;

  CommonParameters = (COMMON_1D_PARAMETERS *)DramDcaTrainingContext;
  Socket = DramDcaTrainingContext->Socket;
  DebugLevel = SDBG_MAX;
  MaxChDdr = GetMaxChDdr ();

  RcDebugPrint (DebugLevel, "Internal clock alignment results:\n");
  RcDebugPrint (DebugLevel, "Strobe:      ");
  for (StrobeLogical = 0; StrobeLogical < MAX_STROBE; StrobeLogical++) {

    RcDebugPrint (DebugLevel, "     %2d", StrobeLogical);

  } // Strobe
  RcDebugPrint (DebugLevel, "\n");

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    //
    // Use ChannelMask from CommonParameters here instead of ChannelMask from DramDcaTrainingContext
    // The test is run based on the CommonParameters value.
    //
    if (((1 << Channel) & CommonParameters->ChannelMask) != 0) {
      continue;
    }

    if (((1 << RankInChannel) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
      continue;
    }
    Rank = RankInChannel % MAX_RANK_DIMM;
    Dimm = RankInChannel / MAX_RANK_DIMM;
    RcDebugPrintWithDevice (DebugLevel, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "");
    for (StrobeLogical = 0; StrobeLogical < MAX_STROBE; StrobeLogical++) {

      BitIndex = 0;  // The test is per strobe so BitIndex doesn't matter

      //
      // If either knob (RxDqsPDelay or RxDqsNDelay) indicates a failure, treat it as a fail condidition for the strobe
      //
      CompositeCondition = PASS_CONDITION;
      for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, Channel, StrobeLogical, BitIndex);

        if (ResultConditionPtr->Current == FAIL_CONDITION) {
          CompositeCondition = FAIL_CONDITION;
          break;
        }
      } // KnobIndex

      //
      // If CPGC miscompare happens (DQs are all high) then the MRR command is not locked to the ICLK.
      // If CPGC miscompare does not happen (DQ pattern matches) then the MRR command is locked to the ICLK.
      //
      if (CompositeCondition == FAIL_CONDITION) {
        DramDcaTrainingContext->ClkPhaseAlignment[StrobeLogical].AlignedClk[Channel][RankInChannel] = IBCLK_PHASE_ALIGNED;
        RcDebugPrint (DebugLevel, "  IBCLK");
      } else {
        DramDcaTrainingContext->ClkPhaseAlignment[StrobeLogical].AlignedClk[Channel][RankInChannel] = ICLK_PHASE_ALIGNED;
        RcDebugPrint (DebugLevel, "   ICLK");
      }

    } // Strobe
    RcDebugPrint (DebugLevel, "\n");
  } // Channel
}

/**

  Discover DRAM device internal clock alignment in preparation for DCA Training.
  Peform a point test of RxDqsDelay at the passing offset with DRAM DCA Training Assist mode enabled.

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context

  @retval MRC_STATUS_SUCCESS if the sweep completes
  @retval MRC_STATUS_FAILURE if input parameter is invalid

**/
MRC_STATUS
DramDcaClkPhaseAlignmentDiscovery (
  IN OUT  DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext
  )
{
  UINT8                       Channel;
  UINT8                       RankInChannel;
  UINT32                      ChannelMask;
  UINT8                       MaxChDdr;
  MRC_RT                      DimmRank[MAX_CH];
  RESULTS_1D                  Results;
  COMMON_1D_PARAMETERS        *CommonParameters;

  if (DramDcaTrainingContext->DcaType != DcaFourPhase) {
    //
    // Only needed for DCA 4-phase training
    //
    return MRC_STATUS_SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();

  CommonParameters = (COMMON_1D_PARAMETERS *)DramDcaTrainingContext;

  //
  // Enable DCA Training Assist Mode to block reads synchronized with IBCLK
  // In this mode if MRRs sent to the DIMM are locked to the IBCLK then DQs will be driven HIGH.
  //
  SetDramTrainingAssistMode (DramDcaTrainingContext, DCA_TRAINING_ASSIST_IBCLK);

  //
  // The test is performed one rank at a time for all channels
  // Only test the ranks that appear in the rank mask for each channel
  //
  for (RankInChannel = 0; RankInChannel < MAX_RANK_CH; RankInChannel++) {

    ZeroMem (DimmRank, sizeof (DimmRank));
    ChannelMask = ALL_CHANNELS_MASKED_OFF;

    for (Channel = 0; Channel < MaxChDdr; Channel++) {

      if (((1 << Channel) & DramDcaTrainingContext->ChannelMask) != 0) {
        continue;
      }

      if (((1 << RankInChannel) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
        continue;
      }

      DimmRank[Channel].Dimm = RankInChannel / MAX_RANK_DIMM;
      DimmRank[Channel].Rank = RankInChannel % MAX_RANK_DIMM;
      ChannelMask &= ~(1 << Channel);

    } // Channel

    if (ChannelMask == ALL_CHANNELS_MASKED_OFF) {
      continue;
    }

    //
    // Run a single point test at the center point
    //
    CreateContextParameters (
      CommonParameters,
      DdrLevel,
      DramDcaTrainingContext->Socket,
      ChannelMask,
      0,
      ALL_SUBCH,
      DimmRank,
      DdrLevel, RxDqsDelay, 0,
      2,                                /* Step size     */
      1,                                /* DitherValue   */
      0,                                /* OffsetLeft    */
      0,                                /* OffsetRight   */
      1,                                /* LimitOffset   */
      DefaultStartOffset | LimitSearch,
      DefaultResults | ResultsRelative,
      DefaultFsm
      );

    AllocatePoolForResults1D (CommonParameters, &Results);

    ProcessSweep1D (
      CommonParameters,
      NULL,
      SetupSweepAdvance,
      ExecuteSweepAdvance,
      NULL,
      NULL,
      ResultProcessingAdvance,
      &Results
      );

    //
    // Print Results
    //
    PrintCurrentResults1D (CommonParameters, &Results);

    //
    // Interpret and save results for later use
    //
    SaveClkPhaseAlignment (DramDcaTrainingContext, RankInChannel);

    //
    // Free the Resource.
    //
    ReleaseResources1D (CommonParameters, &Results);

  } // RankInChannel

  //
  // Disable DRAM DCA training assist mode
  //
  SetDramTrainingAssistMode (DramDcaTrainingContext, DCA_TRAINING_ASSIST_DISABLE);

  return MRC_STATUS_SUCCESS;

}

/**
  Program the optimal DCA setting for each Channel/Rank/Strobe

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[in]  OptimalDcaSettingIndex    - Calculated optimal DCA setting index per strobe

  @retval N/A
**/
VOID
ProgramOptimalDcaSettings (
  IN       DRAM_DCA_TRAINING_CONTEXT    *DramDcaTrainingContext,
  IN       UINT8                        OptimalDcaSettingIndex[MAX_CH][MAX_RANK_CH][MAX_STROBE]
  )
{
  PSYSHOST                    Host;
  UINT8                       Socket;
  UINT8                       Channel;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT8                       LogicalRank;
  UINT8                       Strobe;
  UINT8                       MaxChDdr;
  INT16                       DcaSetting;
  MRC_GT                      Group;
  UINT8                       MSVx4;
  UINT8                       MSVx4SubCh;
  UINT8                       MaxStrobeSubCh;
  UINT32                      DebugLevel;

  DebugLevel = SDBG_MAX;
  
  Host    = DramDcaTrainingContext->CommonParameters.Host;
  Socket  = DramDcaTrainingContext->Socket;
  Group   = DramDcaTrainingContext->Group;

  DcaSetting  = 0;
  MSVx4       = GetMaxStrobeValid (Host);
  MSVx4SubCh  = GetSubChMaxStrobeValid (Host);
  MaxChDdr    = GetMaxChDdr ();

  RcDebugPrint (DebugLevel, "ProgramOptimalDcaSettings\n");

  RcDebugPrint (DebugLevel, "Strobe:       ");
  for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {

    RcDebugPrint (DebugLevel, "   %2d", Strobe);

  } // Strobe
  RcDebugPrint (DebugLevel, "\n");

  for (Channel = 0; Channel < MaxChDdr; Channel++) {

    if (((1 << Channel) & DramDcaTrainingContext->ChannelMask) != 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      MaxStrobeSubCh = MSVx4SubCh / 2;
      if (IsX4Dimm (Socket, Channel, Dimm)) {
        MaxStrobeSubCh = MSVx4SubCh;
      }

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

        if (((1 << LogicalRank) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
          continue;
        }

        RcDebugPrintWithDevice (DebugLevel, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "");
        for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {

          //
          // Skip strobes not used by this DIMM
          // For x4 devices, valid logical strobe numbers in the channel are 0-9 and 10-19
          // For x8/x16 devices, valid logical strobe numbers in the channel are 0-4 and 10-14
          //
          if (IsStrobeNotValidChDdr5 (Host, Strobe) ||
              (!VALID_LOGICAL_STROBE (Strobe, MSVx4, MaxStrobeSubCh)))
          {
              RcDebugPrint (DebugLevel, "   - ", DcaSetting);
              continue;
          }

          //
          // Convert the sweep index into a physical DRAM DCA value and write it to the DRAM
          //
          DcaSetting = SWEEP_INDEX_TO_DCA_VALUE (OptimalDcaSettingIndex[Channel][LogicalRank][Strobe]);
          RcDebugPrint (DebugLevel, "  %3d", DcaSetting);
          GetSetDramDutyCycleAdjuster (Socket, Channel, Dimm, Rank, Strobe, ALL_BITS, Group, GSM_UPDATE_CACHE, &DcaSetting);

        } // Strobe
        RcDebugPrint (DebugLevel, "\n");
      } // Rank
    } // Dimm
  } // Channel
}

/**
  Find the optimal DCA setting index for each Channel/Rank/Strobe

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[in]  EyeWidthPerDcaSetting     - Array of eye widths for eacy UI found at each DCA setting
  @param[out] OptimalDcaSettingIndex    - Calculated optimal DCA setting index per strobe

  @retval N/A
**/
VOID
ProcessDramDcaResults (
  IN       DRAM_DCA_TRAINING_CONTEXT    *DramDcaTrainingContext,
  IN       EYE_WIDTH_ALL_UI             EyeWidthAllUi[DRAM_DCA_SWEEP_SIZE],
      OUT  UINT8                        OptimalDcaSettingIndex[MAX_CH][MAX_RANK_CH][MAX_STROBE]
  )
{
  PSYSHOST                    Host;
  UINT8                       Socket;
  UINT8                       Channel;
  UINT8                       Dimm;
  UINT8                       Rank;
  UINT8                       LogicalRank;
  UINT8                       Strobe;
  UINT8                       MaxChDdr;
  UINT8                       UiIndex;
  UINT8                       DcaSettingIndex;
  UINT8                       BestDcaSettingIndex;
  INT16                       EyeWidthDiff;
  INT16                       MinEyeWidthDiff;
  INT16                       EwUi[MAX_UI / 2];
  MRC_GT                      Group;
  UINT8                       ClkPhaseAlignment;
  UINT8                       MSVx4;
  UINT8                       MSVx4SubCh;
  UINT8                       MSVx8SubCh;
  UINT8                       MaxStrobeSubCh;

  Host        = GetSysHostPointer ();
  Socket      = DramDcaTrainingContext->Socket;
  MaxChDdr    = GetMaxChDdr ();
  MSVx4       = GetMaxStrobeValid (Host);
  MSVx4SubCh  = GetSubChMaxStrobeValid (Host);
  MSVx8SubCh  = MSVx4SubCh / 2;
  Group       = DramDcaTrainingContext->Group;

  for (Channel = 0; Channel < MaxChDdr; Channel++) {

    if (((1 << Channel) & DramDcaTrainingContext->ChannelMask) != 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      MaxStrobeSubCh = MSVx8SubCh;
      if (IsX4Dimm (Socket, Channel, Dimm)) {
        MaxStrobeSubCh = MSVx4SubCh;
      }

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

        if (((1 << LogicalRank) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
          continue;
        }

        for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {

          //
          // Skip strobes not used by this DIMM
          // For x4 devices, valid logical strobe numbers in the channel are 0-9 and 10-19
          // For x8/x16 devices, valid logical strobe numbers in the channel are 0-4 and 10-14
          //
          if (IsStrobeNotValidChDdr5 (Host, Strobe) ||
              (!VALID_LOGICAL_STROBE (Strobe, MSVx4, MaxStrobeSubCh)))
          {
            continue;
          }

          //
          // Get clock phase alignment for this device
          //
          ClkPhaseAlignment = DramDcaTrainingContext->ClkPhaseAlignment[Strobe].AlignedClk[Channel][LogicalRank];

          //
          // Find optimal DCA setting for this device
          //
          MinEyeWidthDiff = MAX_EYE_WIDTH;
          BestDcaSettingIndex = DRAM_DCA_SWEEP_SIZE / 2;

          for (DcaSettingIndex = 0; DcaSettingIndex < DRAM_DCA_SWEEP_SIZE; DcaSettingIndex++) {

            for (UiIndex = 0; UiIndex < MAX_UI / 2; UiIndex++) {
              EwUi[UiIndex] = EyeWidthAllUi[DcaSettingIndex].EyeWidthAllStrobe[UiIndex].EyeWidth[Channel][LogicalRank][Strobe];
            }

            switch (Group) {
              case DramDcaClk:
              case DramDcaIbClk:
                //
                // CLK single/double-phase
                // IBCLK 4-phase
                // Diff = Max(Abs([UI0 + UI1] – [UI2+UI3]), Abs([UI4 + UI5] – [UI6+UI7]))
                //
                EyeWidthDiff = MAX (ABS ((EwUi[0] + EwUi[1]) - (EwUi[2] + EwUi[3])), ABS ((EwUi[4] + EwUi[5]) - (EwUi[6] + EwUi[7])));
                break;

              case  DramDcaQClk:
                //
                // QCLK
                // ICLK aligned:  Diff = Max(Abs(UI0-UI1), Abs(UI4 – UI5))
                // IBCLK aligned: Diff = Max(Abs(UI2-UI3), Abs(UI6 – UI7))
                //
                if (ClkPhaseAlignment == ICLK_PHASE_ALIGNED) {
                  EyeWidthDiff = MAX (ABS (EwUi[0] - EwUi[1]), ABS (EwUi[4] - EwUi[5]));
                } else {
                  EyeWidthDiff = MAX (ABS (EwUi[2] - EwUi[3]), ABS (EwUi[6] - EwUi[7]));
                }
                break;

              case DramDcaQbClk:
                //
                // QBCLK
                // ICLK aligned:  Diff = Max(Abs(UI2-UI3), Abs(UI6 – UI7))
                // IBCLK aligned: Diff = Max(Abs(UI1-UI2), Abs(UI5 – UI6))
                //
                if (ClkPhaseAlignment == ICLK_PHASE_ALIGNED) {
                  EyeWidthDiff = MAX (ABS (EwUi[2] - EwUi[3]), ABS (EwUi[6] - EwUi[7]));
                } else {
                  EyeWidthDiff = MAX (ABS (EwUi[1] - EwUi[2]), ABS (EwUi[5] - EwUi[6]));
                }
                break;

              default:
                EyeWidthDiff = MAX_EYE_WIDTH;
                RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_240);
                break;
            }

            if (MinEyeWidthDiff > EyeWidthDiff) {
              MinEyeWidthDiff = EyeWidthDiff;
              BestDcaSettingIndex = DcaSettingIndex;
            }

          } // DcaSettingIndex

          OptimalDcaSettingIndex[Channel][LogicalRank][Strobe] = BestDcaSettingIndex;
        } // Strobe
      } // Rank
    } // Dimm
  } // Channel

}

/**
  Calculate the eye width for each channel/rank/strobe for a specific UI

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[in]  UiIndexBase               - UI (cacheline chunk) number
  @param[in]  RankInChannel             - Rank number in the channel
  @param[in]  Results                   - Pointer to the Results for the rank
  @param[out] EyeWidthAllUi             - Eye width for all channel/rank/strobe/UI

  retval N/A

**/
VOID
CalculateEyeWidth (
  IN      DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext,
  IN      UINT8                       UiIndexBase,
  IN      UINT8                       RankInChannel,
     OUT  RESULTS_1D                  *Results,
     OUT  EYE_WIDTH_ALL_UI            *EyeWidthAllUi
  )
{
  UINT8                   Channel;
  UINT8                   Strobe;
  UINT8                   UiIndex;
  UINT8                   KnobIndex;
  UINT8                   MaxChDdr;
  struct baseMargin       *StrobeMargin;
  COMMON_1D_PARAMETERS    *CommonParameters;

  if (UiIndexBase >= MAX_UI / 2) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_241);
  }
  MaxChDdr    = GetMaxChDdr ();
  CommonParameters = (COMMON_1D_PARAMETERS *)DramDcaTrainingContext;

  for (Channel = 0; Channel < MaxChDdr; Channel++) {

    if (((1 << Channel) & CommonParameters->ChannelMask) != 0) {
      continue;
    }

    if (((1 << RankInChannel) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
      continue;
    }

    for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
      //
      // RxDqsDelay is swept with 2 Knobs:
      //   KnobIndex 0 = RxDqsPDelay (even UI)
      //   KnobIndex 1 = RxDqsNDelay (odd UI)
      //
      // Sweeps can be run with the CPGC chunkmask register set to measure results for 1, 2 or more consecutive
      // UIs at a time. The number of UIs in the test is CHUNK_MASK_WIDTH.
      // The loop below ensures correct EyeWidth results per UI are reported regardless of how many UIs are in the mask.
      //
      for (UiIndex = UiIndexBase; UiIndex < (UiIndexBase + CHUNK_MASK_WIDTH); UiIndex++) {

        KnobIndex = UiIndex % CommonParameters->KnobsPerGroup;
        StrobeMargin = GetResultPtrAtStrobeLevel (
                      CommonParameters,
                      KnobIndex,
                      Channel,
                      Strobe,
                      Results);

        EyeWidthAllUi->EyeWidthAllStrobe[UiIndex].EyeWidth[Channel][RankInChannel][Strobe] =
          ABS (StrobeMargin->p - StrobeMargin->n);

      } // KnobIndex
    } // Strobe
  } // Channel
}

/**

  Print eyewidths

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[out] EyeWidthAllUi             - Per-UI eyewidth

  @retval MRC_STATUS_SUCCESS if the sweep completes
  @retval MRC_STATUS_FAILURE if input parameter is invalid

**/
VOID
PrintEyeWidth (
  IN      DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext,
  IN      EYE_WIDTH_ALL_UI            *EyeWidthAllUi
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST      Host;
  UINT8         Socket;
  UINT8         Channel;
  UINT32        ChannelMask;
  UINT8         Dimm;
  UINT8         Rank;
  UINT32        RankMask;
  UINT8         LogicalRank;
  UINT8         Strobe;
  UINT8         UiIndex;
  UINT8         MaxChDdr;
  UINT8         MSVx4;
  UINT8         MSVx4SubCh;
  UINT8         MSVx8SubCh;
  UINT8         MaxStrobeSubCh;

  Host        = GetSysHostPointer ();
  Socket      = DramDcaTrainingContext->Socket;
  MaxChDdr    = GetMaxChDdr ();
  MSVx4       = GetMaxStrobeValid (Host);
  MSVx4SubCh  = GetSubChMaxStrobeValid (Host);
  MSVx8SubCh  = MSVx4SubCh / 2;

  ChannelMask = DramDcaTrainingContext->ChannelMask;

  RcDebugPrint (SDBG_MAX, "RxDqsDelay Eye Width\n");

  for (Channel = 0; Channel < MAX_CH; Channel++) {
    if (((1 << Channel) & ChannelMask) != 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      MaxStrobeSubCh = MSVx8SubCh;
      if (IsX4Dimm (Socket, Channel, Dimm)) {
        MaxStrobeSubCh = MSVx4SubCh;
      }

      RankMask = DramDcaTrainingContext->RankMask[Channel];

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

        if (((1 << LogicalRank) & RankMask) != 0) {
          continue;
        }

        RcDebugPrint (SDBG_MAX, "Strobe:           ");
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {

          //
          // Skip strobes not used by this DIMM
          // For x4 devices, valid logical strobe numbers in the channel are 0-9 and 10-19
          // For x8/x16 devices, valid logical strobe numbers in the channel are 0-4 and 10-14
          //
          if (IsStrobeNotValidChDdr5 (Host, Strobe) ||
              (!VALID_LOGICAL_STROBE (Strobe, MSVx4, MaxStrobeSubCh)))
          {
            continue;
          }

          RcDebugPrint (SDBG_MAX, "   %2d", Strobe);
        } // Strobe
        RcDebugPrint (SDBG_MAX, "\n");

        for (UiIndex = 0; UiIndex < MAX_UI / 2; UiIndex++) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "UI%2d", UiIndex);
          for (Strobe = 0; Strobe < MSVx4; Strobe++) {

            //
            // Skip strobes not used by this DIMM
            // For x4 devices, valid logical strobe numbers in the channel are 0-9 and 10-19
            // For x8/x16 devices, valid logical strobe numbers in the channel are 0-4 and 10-14
            //
            if (IsStrobeNotValidChDdr5 (Host, Strobe) ||
                (!VALID_LOGICAL_STROBE (Strobe, MSVx4, MaxStrobeSubCh)))
            {
              continue;
            }

            RcDebugPrint (SDBG_MAX, "  %3d", EyeWidthAllUi->EyeWidthAllStrobe[UiIndex].EyeWidth[Channel][LogicalRank][Strobe]);
          } // Strobe
          RcDebugPrint (SDBG_MAX, "\n");
        } // UiIndex
      } // Rank
    } // Dimm
  } // Channel
#endif // DEBUG_CODE_BLOCK
}

/**

  Obtain RxDqsDelay Margins for each individual UI(Chunk)

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[out] EyeWidthAllUi             - Per-UI eyewidth

  @retval MRC_STATUS_SUCCESS if the sweep completes
  @retval MRC_STATUS_FAILURE if input parameter is invalid

**/
MRC_STATUS
GetRxDqsDelayMargins (
  IN      DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext,
     OUT  EYE_WIDTH_ALL_UI            *EyeWidthAllUi
  )
{
  UINT8                       Channel;
  UINT8                       RankInChannel;
  UINT32                      ChannelMask;
  UINT8                       MaxChDdr;
  UINT8                       UiIndex;
  MRC_RT                      DimmRank[MAX_CH];
  RESULTS_1D                  Results;
  COMMON_1D_PARAMETERS        *CommonParameters;

  MaxChDdr = GetMaxChDdr ();

  CommonParameters = (COMMON_1D_PARAMETERS *)DramDcaTrainingContext;

  //
  // The margining is performed one rank at a time for all channels
  // Only margin the ranks that appear in the rank mask for each channel
  //
  for (RankInChannel = 0; RankInChannel < MAX_RANK_CH; RankInChannel++) {

    ZeroMem (DimmRank, sizeof (DimmRank));
    ChannelMask = ALL_CHANNELS_MASKED_OFF;

    for (Channel = 0; Channel < MaxChDdr; Channel++) {

      if (((1 << Channel) & DramDcaTrainingContext->ChannelMask) != 0) {
        continue;
      }

      if (((1 << RankInChannel) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
        continue;
      }

      DimmRank[Channel].Dimm = RankInChannel / MAX_RANK_DIMM;
      DimmRank[Channel].Rank = RankInChannel % MAX_RANK_DIMM;
      ChannelMask &= ~(1 << Channel);

    } // Channel

    if (ChannelMask == ALL_CHANNELS_MASKED_OFF) {
      continue;
    }

    for (UiIndex = 0; UiIndex < MAX_UI / 2; UiIndex += CHUNK_MASK_WIDTH) {

      DramDcaTrainingContext->UiIndex = UiIndex;

      CreateContextParameters (
        CommonParameters,
        DdrLevel,
        DramDcaTrainingContext->Socket,
        ChannelMask,
        0,
        ALL_SUBCH,
        DimmRank,
        DdrLevel, RxDqsDelay, 0,
        1,                            /* Step size     */
        1,                            /* DitherValue   */
        SWEEP_START_OFFSET_LEFT,      /* OffsetLeft    */
        SWEEP_START_OFFSET_RIGHT,     /* OffsetRight   */
        RXDQSDELAY_OFFSET_LIMIT,      /* LimitOffset   */
        DefaultStartOffset | LimitSearch,
        DefaultResults | ResultsRelative,
        DefaultFsm
        );

      AllocatePoolForResults1D (CommonParameters, &Results);

      ProcessSweep1D (
        CommonParameters,
        NULL,
        SetupDramDcaSweep,
        ExecuteSweepAdvance,
        NULL,
        NULL,
        ResultProcessingAdvance,
        &Results
        );

      //
      // Print Results
      //
      PrintCurrentResults1D (CommonParameters, &Results);

      CalculateEyeWidth (DramDcaTrainingContext, UiIndex, RankInChannel, &Results, EyeWidthAllUi);
      //
      // Free the Resources including Results
      //
      ReleaseResources1D (CommonParameters, &Results);
    } // UiIndex
  } // RankInChannel


  return MRC_STATUS_SUCCESS;

}

/**

  Peform sweep of the DRAM Duty Cycle Adjuster for a specified clock
  The clock is identified by the Group parameter.
  Valid Group parameter values are: DramDcaClk, DramDcaQClk, DramDcaIbClk and DramDcaQbClk

  @param[in]  DramDcaTrainingContext    - Pointer to DRAM DCA training context
  @param[out] EyeWidthAllUi             - Per-UI eyewidth

  @retval MRC_STATUS_SUCCESS if the sweep completes
  @retval MRC_STATUS_FAILURE if input parameter is invalid

**/
MRC_STATUS
DramDcaTrainingSweep (
  IN  DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext,
     OUT  EYE_WIDTH_ALL_UI        EyeWidthAllUi[DRAM_DCA_SWEEP_SIZE]
  )
{
  PSYSHOST    Host;
  UINT8       Socket;
  UINT8       Channel;
  UINT8       Dimm;
  UINT8       Rank;
  UINT8       LogicalRank;
  UINT8       MaxChDdr;
  INT16       DcaSetting;
  UINT8       DcaSettingIndex;
  MRC_GT      Group;

  Host = GetSysHostPointer ();
  Socket = DramDcaTrainingContext->Socket;
  MaxChDdr = GetMaxChDdr ();
  Group = DramDcaTrainingContext->Group;

  RcDebugPrintWithDevice(SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "DramDcaTrainingSweep - Group: %a, ChannelMask: 0x%x\n", GetGroupStr (Group), DramDcaTrainingContext->ChannelMask);

  for (DcaSettingIndex = 0; DcaSettingIndex < DRAM_DCA_SWEEP_SIZE; DcaSettingIndex += DRAM_DCA_SWEEP_STEP_SIZE) {
    //
    // Convert sweep index to DCA value to be programmed
    //
    DcaSetting = SWEEP_INDEX_TO_DCA_VALUE (DcaSettingIndex);
    RcDebugPrint (SDBG_MEM_TRAIN, "DCA Setting = %d\n", DcaSetting);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {

          LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

          if (((1 << LogicalRank) & DramDcaTrainingContext->RankMask[Channel]) != 0) {
            continue;
          }

          GetSetDramDutyCycleAdjuster (Socket, Channel, Dimm, Rank, ALL_STROBES, ALL_BITS, Group, GSM_UPDATE_CACHE, &DcaSetting);

        } // Channel

      } // Rank
    } // Dimm
    //
    // Setup and execute the test on all channels
    //
    GetRxDqsDelayMargins (DramDcaTrainingContext, &EyeWidthAllUi[DcaSettingIndex]);

    PrintEyeWidth (DramDcaTrainingContext, &EyeWidthAllUi[DcaSettingIndex]);
  } // DcaSetting

  return MRC_STATUS_SUCCESS;
}

/**

  DRAM Duty Cycle Adjuster Training - Single, Double and Four-phase flows

  @param  Socket              - Socket number
  @param  DcaType             - Type of Duty Cycle Adjuster test to run - DcaSingleOrTwoPhase or DcaFourPhase
  @param  NumGroupsToSweep    - Number of groups in the GroupsToSweep parameter
  @param  GroupsToSweep       - List of DCA groups to sweep. Possible values include:
                                   DramDcaClk, DramDcaQClk, DramDcaIbClk and DramDcaQbClk

  @retval MRC_STATUS_SUCCESS  Training succeeds or is unsupported by the DRAM
  @retval MRC_STATUS_FAILURE  Training fails

**/
MRC_STATUS
DramDcaTrainingWorker (
  IN UINT8      Socket,
  IN DCA_TYPE   DcaType,
  IN UINT8      NumGroupsToSweep,
  IN MRC_GT     *GroupsToSweep
  )
{
  UINT32                      ChannelMask;
  UINT32                      RankMask[MAX_CH];
  UINT8                       GroupIndex;
  DRAM_DCA_TRAINING_CONTEXT   *DramDcaTrainingContext;
  EYE_WIDTH_ALL_UI            (*EyeWidthAllUi)[DRAM_DCA_SWEEP_SIZE];
  UINT8                       (*OptimalDcaSettingIndex)[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  UINT32                      SizeToAllocate;

  //
  // Verify the socket has a DIMM that supports this training step
  // Also get ChannelMask and DimmMask to reflect which channels/ranks support DcaType
  //
  if (GetDramDcaSupportedSocket (Socket, &ChannelMask, RankMask, DcaType) == FALSE) {
    return MRC_STATUS_SUCCESS;
  }

  SizeToAllocate = sizeof (*DramDcaTrainingContext) + sizeof (*EyeWidthAllUi) + sizeof (*OptimalDcaSettingIndex);

  DramDcaTrainingContext = RcAllocatePool (SizeToAllocate);

  if (DramDcaTrainingContext == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }

  ZeroMem (DramDcaTrainingContext, SizeToAllocate);

  EyeWidthAllUi          = (VOID *) ((UINTN) DramDcaTrainingContext + sizeof (*DramDcaTrainingContext));
  OptimalDcaSettingIndex = (VOID *) ((UINTN) EyeWidthAllUi + sizeof (*EyeWidthAllUi));

  DramDcaTrainingContext->Socket        = Socket;
  DramDcaTrainingContext->DcaType       = DcaType;
  DramDcaTrainingContext->ChannelMask   = ChannelMask;
  CopyMem (DramDcaTrainingContext->RankMask, RankMask, sizeof (RankMask));
  //
  // Configure DRAMs and CPGC with the test pattern
  //
  DramDcaPatternInit (Socket);

  SetCurrentTestType (Socket, DramDutyCycleAdjusterTest);
  //
  // Prepare memory controller for DRAM DCA training
  //
  DramDcaMemoryControllerConfiguration (Socket, ChannelMask, TRUE);
  //
  // Discover the CLK phase alignment of each DRAM - only executes for DRAMs that support 4-phase clocks
  //
  DramDcaClkPhaseAlignmentDiscovery (DramDcaTrainingContext);

  //
  // Do the Dram Duty Cycle Adjuster training
  // For IBCLK, QCLK and QBCLK, determine which DCA setting produces the best eye widths for all UIs
  //
  for (GroupIndex = 0; GroupIndex < NumGroupsToSweep; GroupIndex++) {

    DramDcaTrainingContext->Group = GroupsToSweep[GroupIndex];

    DramDcaTrainingSweep (DramDcaTrainingContext, *EyeWidthAllUi);

    ProcessDramDcaResults (DramDcaTrainingContext, *EyeWidthAllUi, *OptimalDcaSettingIndex);

    ProgramOptimalDcaSettings (DramDcaTrainingContext, *OptimalDcaSettingIndex);

  } // GroupIndex

  //
  // Restore memory controller
  //
  DramDcaMemoryControllerConfiguration (Socket, ChannelMask, FALSE);

  DramDcaCleanup (Socket);

  SetCurrentTestType (Socket, DefaultTest);

  RcFreePool (DramDcaTrainingContext);

  return MRC_STATUS_SUCCESS;
}

/**

  DRAM Duty Cycle Adjuster Training - Single or Two-Phase Internal Clock flow

  @param  Socket     Socket number

  @retval MRC_STATUS_SUCCESS  Training succeeds or is unsupported by the DRAM
  @retval MRC_STATUS_FAILURE  Training fails

**/
MRC_STATUS
DramDca1or2PhaseTraining (
  IN UINT8    Socket
  )
{
  DCA_TYPE                  DcaType;
  UINT8                     NumGroupsToSweep;
  MRC_GT                    GroupsToSweep;

  DcaType           = DcaSingleOrTwoPhase;
  GroupsToSweep     = DramDcaClk;
  NumGroupsToSweep  = 1;

  return DramDcaTrainingWorker (Socket, DcaType, NumGroupsToSweep, &GroupsToSweep);
}

/**

  DRAM Duty Cycle Adjuster Training - Four-Phase Internal Clock flow

  @param  Socket     Socket number

  @retval MRC_STATUS_SUCCESS  Training succeeds or is unsupported by the DRAM
  @retval MRC_STATUS_FAILURE  Training fails

**/
MRC_STATUS
DramDcaPhase4Training (
  IN UINT8    Socket
  )
{
  DCA_TYPE                  DcaType;
  UINT8                     NumGroupsToSweep;
  MRC_GT                    GroupsToSweep[] = {DramDcaQClk,
                                               DramDcaIbClk,
                                               DramDcaQbClk};

  DcaType           = DcaFourPhase;
  NumGroupsToSweep  = ARRAY_SIZE (GroupsToSweep);

  return DramDcaTrainingWorker (Socket, DcaType, NumGroupsToSweep, GroupsToSweep);
}

/**

  DRAM Duty Cycle Adjuster Training entry

  @param  Host     Pointer to sysHost

  @retval SUCCESS  Training succeeds or not enabled
  @retval FAILURE  Training fails

**/
UINT32
DramDutyCycleAdjuster (
  IN PSYSHOST  Host
  )
{
  UINT8           Socket;
  UINT32          Status;
  UINT32          PrevDebugLevel;
  UINT32          DebugLevel;

  if (!IsMemFlowEnabled (DramDutyCycleAdjusterTraining)) {
    return SUCCESS;
  }

  Socket = GetCurrentSocketId ();

  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  PrevDebugLevel = GetDebugLevel ();

  DebugLevel = PrevDebugLevel; // | SDBG_DEFAULT | SDBG_MEM_TRAIN | SDBG_MINMAX | SDBG_SWEEP_L0 | SDBG_SWEEP_L1 | SDBG_SWEEP_L2 | SDBG_SWEEP_L3;
  SetDebugLevel (DebugLevel);

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_DRAM_DUTY_CYCLE_ADJUSTER, Socket));

  //
  // Update mode register cache with contents of MR42 - DCA Types Supported - for later use
  //
  CacheDramDcaCapabilities (Socket);

  //
  // Devices may support single/two-phase internal clocks or 4-phase internal clocks
  // DCA training flow is different depending on the device support
  // Call both flows - execution will only be peformed on devices that support the flow
  //
  Status = SUCCESS;

  if (DramDca1or2PhaseTraining (Socket) != MRC_STATUS_SUCCESS) {
    Status = FAILURE;
  }
  if (DramDcaPhase4Training (Socket) != MRC_STATUS_SUCCESS) {
    Status = FAILURE;
  }

  //
  // Receive Enable will be locked to either the ICLK or IBCLK, so after duty cycle adjust is complete, receive enable
  // could potentially be locked to the wrong cycle. This could cause a 180° phase error. Therefore, receive enable
  // traububg needs to be rerun to train to the correct cycle.
  //
  ReceiveEnableDdr5 (Host);

  DisplayResultsDdr5 (Host, Socket, RecEnDelay);
  DisplayResultsDdr5 (Host, Socket, RxCoarseDelay);

  SetDebugLevel (PrevDebugLevel);

  return Status;
}

#else //  defined(USE_LATEST_DRAM_SPEC) && (FixedPcdGetBool(PcdDramDutyCycleAdjusterSupport) == 1)

/**

  DRAM Duty Cycle Adjuster Training entry

  @param  Host     Pointer to sysHost

  @retval SUCCESS  Training succeeds or not enabled
  @retval FAILURE  Training fails

**/
UINT32
DramDutyCycleAdjuster (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
}

#endif // defined(USE_LATEST_DRAM_SPEC) && (FixedPcdGetBool(PcdDramDutyCycleAdjusterSupport) == 1)
