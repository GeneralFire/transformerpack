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
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/CteNetLib.h>
#include <Library/KtiApi.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/AdvMemTestLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemFmcIpLib.h>
#include "BiosSsaLibInternal.h"


/**
  Calculate the minimum and maximum offset to the specified data byte, given the minimum and maximum values.
  So for 9/18 strobes/nibbles (72bits) this function will calculate the Min(of all max's) and Max(of all min's) offset possible that can be used for all strobes
  @param[in]      MrcData   - Pointer to the MRC global data area.
  @param[in]      DataByte  - The current data byte.
  @param[in]      MinValue  - The smallest value that the data byte can be.
  @param[in]      MaxValue  - The largest value that the data byte can be.
  @param[out]     MinOffset - Minimum offset supported by the given margin group.
  @param[out]     MaxOffset - Maximum offset supported by the given margin group.

  @retval Nothing.
**/
static
VOID
SSACalcMinMax (
  PSYSHOST             Host,
  INT16                DataByte,
  INT16                MinValue,
  INT16                MaxValue,
  INT16 *const         MinOffset,
  INT16 *const         MaxOffset
  )
{
  INT16                Min = 0;
  INT16                Max = 0;

  Min = MinValue - DataByte;
  *MinOffset = MAX(*MinOffset, Min);

  Max = MaxValue - DataByte;
  *MaxOffset = MIN(*MaxOffset, Max);
 }

#ifdef DDR5_SUPPORT
/**
  Helper function used to check whether or not a logical strobe is valid or enabled for margin test

           DDR5                x4               x8
  CPGC Strobe[Lane/Bit]   Logical Strobe   Logical Strobe
  0 [3:0]                 0                0
  1 [7:4]                 5
  2 [11:8]                1                1
  3 [15:12]               6
  4 [19:16]               2                2
  5 [23:20]               7
  6 [27:24]               3                3
  7 [31:28]               8
  8 [35:32]               4                4
  9 [39:36]               9
  10 [43:40]              10               5
  11 [47:44]              15
  12 [51:48]              11               6
  13 [55:52]              16
  14 [59:56]              12               7
  15 [63:60]              17
  16 [67:64]              13               8
  17 [71:68]              18
  18 [75:72]              14               9
  19 [79:76]              19

  @param[in]       MaxStrobe                  - MaxStrobe value
  @param[in]       Strobe                     - Logical strobe to be checked
  @param[in]       DqMask1Low                 - 32 bits of DQMask for Ddr5 sub-ch A
  @param[in]       DqMask1High                - 32 bits of DQMask for Ddr5 sub-ch B
  @param[in]       EccMask                    - 16 bit EccMask, lower 8 bit for sub-ch A, high 8 bit for sub-ch B

  @retval  -       TRUE                       - Strobe is valid for margin test
                   FALSE                      - Strobe is invalid for margin test
**/
BOOLEAN
IsStrobeValidOrEnabledDdr5 (
  IN UINT8    MaxStrobe,
  IN UINT8    Strobe,
  IN UINT32   DqMask1Low,
  IN UINT32   DqMask1High,
  IN UINT16   EccMask
)
{
  UINT8     MSVx4;
  UINT8     MSVx8;
  BOOLEAN   IsValid = TRUE;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();

  MSVx4 = GetMaxStrobeValid(Host);
  MSVx8 = GetMaxStrobeValid(Host) / 2;

  if (MaxStrobe == MSVx8) { // x8
    //
    // Check if this is an ECC strobe when ECC is disabled
    //
    if ((!Host->nvram.mem.eccEn) && ((Strobe == LOGICAL_STROBE_4) || (Strobe == LOGICAL_STROBE_9))) {
      IsValid = FALSE;
    }

    //
    // Check if EccMask disabled for sub-ch A/B
    //
    if ((((EccMask & BYTE_MASK) == 0) && (Strobe == LOGICAL_STROBE_4)) ||
       (((EccMask & ((BYTE_MASK) << BYTE_WIDTH)) == 0) && (Strobe == LOGICAL_STROBE_9))) {
      IsValid = FALSE;
    }

    //
    // Check if Dq is masked
    //
    if (((Strobe < LOGICAL_STROBE_4) && (((DqMask1Low >> (Strobe * LOGICAL_STROBE_8)) & BYTE_MASK) == 0)) ||
        (((Strobe >= LOGICAL_STROBE_5) && (Strobe < LOGICAL_STROBE_9)) &&
        (((DqMask1High >> ((Strobe - LOGICAL_STROBE_5) * BYTE_WIDTH)) & BYTE_MASK) == 0))) {
      IsValid = FALSE;
    }
  } else if (MaxStrobe == MSVx4) {  // x4
    //
    // Check if this is an ECC strobe when ECC is disabled
    //
    if ((!Host->nvram.mem.eccEn) &&
        ((Strobe == LOGICAL_STROBE_4) || (Strobe == LOGICAL_STROBE_9) ||
         (Strobe == LOGICAL_STROBE_14) || (Strobe == LOGICAL_STROBE_19))) {
      IsValid = FALSE;
    }

    //
    // Check if EccMask disabled for sub-ch A/B
    //
    if ((((EccMask & NIBBLE_MASK) == 0) && (Strobe == LOGICAL_STROBE_4)) ||
         (((EccMask & ((NIBBLE_MASK << NIBBLE_WIDTH))) == 0) && (Strobe == LOGICAL_STROBE_9)) ||
         (((EccMask & ((NIBBLE_MASK << 2 * NIBBLE_WIDTH))) == 0) && (Strobe == LOGICAL_STROBE_14)) ||
         (((EccMask & ((NIBBLE_MASK << 3 * NIBBLE_WIDTH))) == 0) && (Strobe == LOGICAL_STROBE_19))) {
      IsValid = FALSE;
    }

    //
    // Check if Dq is masked
    //
    if ((Strobe < LOGICAL_STROBE_4) && (((DqMask1Low >> (Strobe * BYTE_WIDTH)) & NIBBLE_MASK) == 0)) {
      IsValid = FALSE;
    }
    if (((Strobe >= LOGICAL_STROBE_5) && (Strobe < LOGICAL_STROBE_9)) && (((DqMask1Low >> ((Strobe - LOGICAL_STROBE_5) * BYTE_WIDTH)) & ((NIBBLE_MASK) << NIBBLE_WIDTH)) == 0)) {
      IsValid = FALSE;
    }
    if (((Strobe >= LOGICAL_STROBE_10 ) && (Strobe < LOGICAL_STROBE_14)) && (((DqMask1High >> ((Strobe - LOGICAL_STROBE_10) * BYTE_WIDTH)) & NIBBLE_MASK) == 0)) {
      IsValid = FALSE;
    }
    if (((Strobe >= LOGICAL_STROBE_15) && (Strobe < LOGICAL_STROBE_19)) && (((DqMask1High >> ((Strobe - LOGICAL_STROBE_15) * BYTE_WIDTH)) & ((NIBBLE_MASK) << NIBBLE_WIDTH)) == 0)) {
      IsValid = FALSE;
    }

  }

  return IsValid;
}

/**
  Helper function used to calculate the MaxMinOffset/NewValue values for SSAGetSetMarginMaxMinOffset (Not used for the CMD/CTL margin groups)

  @param[in]       Host                       - Pointer to sysHost
  @param[in]       Socket                     - Zero based socket number.
  @param[in]       ChannelInSocket            - Channel number w.r.t. the socket
  @param[in]       Dimm                       - Zero based Dimm number.
  @param[in]       Rank                       - Zero based Rank number.
  @param[in]       SetMarginFunctionFlag      - Set to have SSAGetSetMarginMaxMinOffset() behave as Get or SetMargin
  @param[in]       IoLevel                    - IoLevel of MarginGroup
  @param[in]       MarginGroup                - MarginGroup
  @param[in]       MinOffsetPntr              - Pointer to MinOffset value to be programmed
  @param[in]       MaxOffsetPntr              - Pointer to MaxOffset value to be programmed
  @param[in]       MinOffsetDefault           - MinOffsetDefault value
  @param[in]       MaxOffsetDefault           - MaxOffsetDefault value
  @param[in]       maxStrobe                  - maxStrobe value
  @param[in]       DataBytePntr               - Value modified by GetSetDataGroup() used to get Max/Min offset used for GetMarginOffset() - The difference which will be saved into GetSetDataGroup and used as the piDelay
  @param[in]       NewValuePntr               - Value modified by GetSetDataGroup() used to get Max/Min offset used for SetMarginOffset()
  @param[in]       DqMask1Low                 - 32 bits of DQMask for Ddr5 sub-ch A
  @param[in]       DqMask1High                - 32 bits of DQMask for Ddr5 sub-ch B
  @param[in]       EccMask                    - EccMask

  @retval  -       None
**/
static
VOID
SSAGetSetMarginMaxMinOffsetDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     ChannelInSocket,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SetMarginFunctionFlag,
  IN MRC_LT    IoLevel,
  IN MRC_GT    MarginGroup,
  IN INT16     *MinOffsetPntr,
  IN INT16     *MaxOffsetPntr,
  IN UINT16    MinOffsetDefault,
  IN UINT16    MaxOffsetDefault,
  IN UINT8     MaxStrobe,
  IN INT16     *DataBytePntr,
  IN INT16     *NewValuePntr,
  IN UINT32    DqMask1Low,
  IN UINT32    DqMask1High,
  IN UINT16    EccMask
  )
{

  UINT8 Strobe;

  for (Strobe = 0; Strobe < MaxStrobe; Strobe ++) {
    if (IsStrobeValidOrEnabledDdr5 (MaxStrobe, Strobe, DqMask1Low, DqMask1High, EccMask)) {
      if (SetMarginFunctionFlag != 1) {
        //
        // ie GetMargin()
        //
        GetSetDataGroup (Host, Socket, ChannelInSocket,  NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
        RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u value:%d\n", Socket, ChannelInSocket, Dimm, Rank, Strobe, *DataBytePntr);
        SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
      } else {
        //
        // ie SetMargin
        //
        GetSetDataGroup (Host, Socket, ChannelInSocket,  NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, IoLevel, MarginGroup, GSM_FORCE_WRITE | GSM_WRITE_OFFSET, NewValuePntr);
        RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u new_value:%d\n", Socket, ChannelInSocket, Dimm, Rank, Strobe, *NewValuePntr);
      }
    }
  }
} //SSAGetSetMarginMaxMinOffsetDdr5

#endif

/**
  Calculate the actual channel number with respect to the Socket

  @param[in]      Controller  - Zero based controller number.
  @param[in]      Channel     - Zero based channel number in Memory Controller.

  @retval ChannelInSocket     - Channel Value with respect to the Socket.
**/
UINT8
SSAGetChannelInSocket (
  UINT8                 Controller,
  UINT8                 Channel
  )
{
  UINT8                 ChannelInSocket = 0;
  UINT8               NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  ChannelInSocket = Channel;

  //If Controller = 0, Ch = 0,1 OR 0,1,2,3
  ChannelInSocket = Channel + (Controller * NumChPerMc);
  return ChannelInSocket;
}

/**
  Find if the bit is present in the bitMask for that nibble under consideration for x4 and x8 DIMMs for LowDQ, HighDQ and Ecc Masks

  @retval TRUE/FALSE     FALSE: Bit not present in bitMask for that nibble under consideration
**/

static
BOOLEAN
SSAPerBitMaskCheck (
  PSYSHOST              Host,
  UINT8                 nibble,
  UINT8                 bit,
  UINT8                 maxStrobe,
  UINT32                DqMask1Low,
  UINT32                DqMask1High,
  UINT8                 EccMask
  )
{
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  //DQ bits: For nibble=0, we check, DqMask1Low(Bits 0-3) and DqMask1High(Bits 0:3->32-35). Nibble=1, DqMask1Low(Bits 4-7), DqMask1High(Bits 4-7->35-38)... so on
  //ECC Bits: For nibble=8, we check, 64-67 and 68-71
  //If BitMask='0' that means no bits/lanes in that bitMask are participating in the margin parameter offset test

  if (!((((DqMask1Low >> (nibble * 4)) >> bit) & BIT0) && (DqMask1Low != 0))) { //0-32,bits 0 - 4
    return FALSE; //bit not set in mask
  }

  if (!((((DqMask1High >> (nibble * 4)) >> bit) & BIT0) && (DqMask1High != 0))) { //0-32,bits 0 - 4
    return FALSE;
  }

  //Reach upper bits of ECCMask
  if ((maxStrobe == MSVx4) && (nibble == 17)) {
    bit = bit + 4;
  }

  //For ECCMask, bit and bit+4 will be the I/Ps for each nibble
  if (!(((EccMask >> bit) & BIT0) && (EccMask != 0))) { //bit 0 - 8
    return FALSE;
  }

  return TRUE;
}

/**
  Helper function used to calculate the MaxMinOffset/NewValue values for SSAGetSetMarginMaxMinOffset (Not used for the CMD/CTL margin groups)

  @param[in]       Host                       - Pointer to sysHost
  @param[in]       Socket                     - Zero based socket number.
  @param[in]       channelInSocket            - Channel number w.r.t. the socket
  @param[in]       Dimm                       - Zero based Dimm number.
  @param[in]       Rank                       - Zero based Rank number.
  @param[in]       SetMarginFunctionFlag      - Set to have SSAGetSetMarginMaxMinOffset() behave as Get or SetMargin
  @param[in]       IoLevel                    - IoLevel of MarginGroup
  @param[in]       MarginGroup                - MarginGroup
  @param[in]       MinOffsetPntr              - Pointer to MinOffset value to be programmed
  @param[in]       MaxOffsetPntr              - Pointer to MaxOffset value to be programmed
  @param[in]       MinOffsetDefault           - MinOffsetDefault value
  @param[in]       MaxOffsetDefault           - MaxOffsetDefault value
  @param[in]       maxStrobe                  - maxStrobe value
  @param[in]       DataBytePntr               - Value modified by GetSetDataGroup() used to get Max/Min offset used for GetMarginOffset() - The difference which will be saved into GetSetDataGroup and used as the piDelay
  @param[in]       newValuePntr               - Value modified by GetSetDataGroup() used to get Max/Min offset used for SetMarginOffset()
  @param[in]       DqMask1Low                 - Lower 32 bits of DQMask. Mask of the lanes/strobes that participate to the margin parameter offset limit calculation.
  @param[in]       DqMask1High                - Upper 32 bits of DQMask. Mask of the lanes/strobes that participate to the margin parameter offset limit calculation.
  @param[in]       EccMask16                  - 16 bit EccMask. DDR4 only uses lower 8 bits.

  @retval  -       None
**/
static
VOID
SSAGetSetMarginMaxMinOffset (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 channelInSocket,
  UINT8                 Dimm,
  UINT8                 Rank,
  UINT8                 SetMarginFunctionFlag,
  MRC_LT                IoLevel,
  MRC_GT                MarginGroup,
  INT16                 *MinOffsetPntr,
  INT16                 *MaxOffsetPntr,
  UINT16                MinOffsetDefault,
  UINT16                MaxOffsetDefault,
  UINT8                 maxStrobe,
  INT16                 *DataBytePntr,
  INT16                 *newValuePntr,
  UINT32                DqMask1Low,
  UINT32                DqMask1High,
  UINT16                EccMask16
  )
{
  UINT8                       nibble;
  UINT8                       bit;
  INT16                       NewOffsets[BITS_PER_NIBBLE];
  BOOLEAN                     BitChange;
  UINT8                       EccMask;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

#ifdef DDR5_SUPPORT
  if ((MarginGroup == RxSamplerEvenOdd) ||
      (MarginGroup == RxSamplerEven)  ||
      (MarginGroup == RxSamplerOdd)) {
    SSAGetSetMarginMaxMinOffsetDdr5 (Host, Socket, channelInSocket, Dimm, Rank,
                                     SetMarginFunctionFlag, IoLevel, MarginGroup,
                                     MinOffsetPntr, MaxOffsetPntr, MinOffsetDefault, MaxOffsetDefault,
                                     maxStrobe, DataBytePntr, newValuePntr, DqMask1Low, DqMask1High, EccMask16);
    return;
  }
#endif

  EccMask = EccMask16 & BYTE_MASK;

  if (maxStrobe == (MSVx4/2)) { // 9 strobes, 8th is the ecc strobe
    for (nibble = 0; nibble < maxStrobe; nibble ++) {
      if ((nibble < 4) && (((DqMask1Low >> (nibble * 8)) & 0xFF) == 0)) {
        continue;
      }  // first 32bits are for first 4 strobes.. 0 to 3, if NOT enabled in mask, skip that strobe
      if (((nibble >= 4) && (nibble < 8)) && (((DqMask1High >> ((nibble - 4) * 8)) & 0xFF) == 0)) {
        continue;
      }  // second 32bits are for the next 4 strobes.. 4 to 7, if NOT enabled in mask, skip that strobe
      if ((!Host->nvram.mem.eccEn) && (nibble == 8)) {
        continue;
      }  // Skip if this is an ECC strobe when ECC is disabled
      if (((EccMask & 0xFF) == 0) && (nibble == 8)) {
        continue;
      }  //Continue if EccMask disabled

      if (SetMarginFunctionFlag != 1) { //ie GetMargin()
        if ((MarginGroup == RxDqsBitDelay) || (MarginGroup == TxDqBitDelay)) {
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            //Testing bits 0-3 on nibbles/strobes. IO/MC: strobes 0,9 , 1,10 2,11 3,12... regardless of x4 or x8 DIMM. If 0 for a Mask, all bit disabled from this test

            //DqMask1Low: Check bits 0:3(nibble=0) in DqMask1Low.. so on
            if ((nibble < 4) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, DqMask1Low, 0, 0) != FALSE)) {
              //
              // Nibble + 0 returns the IO swizzled data for the lower nibble of the byte
              //
              GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
              RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, bit, *DataBytePntr);
              SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
            }

            // DqMask1High: Check bits 0:3(nibble=0+9) in DqMask1High.. so on
            if ((nibble < 4) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, 0, DqMask1High, 0) != FALSE)) {
              //
              // Nibble + 9 returns the IO swizzled data for the upper nibble of the byte
              //
              GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble + 9, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
              RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble + 9, bit, *DataBytePntr);
              SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
            }

            //DqMask1Low: Check bits 16:19(nibble=4) in DqMask1Low.. so on
            if (((nibble > 3) && (nibble < 8)) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, DqMask1Low, 0, 0) != FALSE)) {
              //
              // Nibble + 0 returns the IO swizzled data for the lower nibble of the byte
              //
              GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
              RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, bit, *DataBytePntr);
              SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
            }

            // DqMask1High: Check bits 48:51(nibble=13) in DqMask1High.. so on
            if (((nibble > 3) && (nibble < 8)) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, 0, DqMask1High, 0) != FALSE)) {
              //
              // Nibble + 9 returns the IO swizzled data for the upper nibble of the byte
              //
              GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble + 9, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
              RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble + 9, bit, *DataBytePntr);
              SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
            }

            // EccMask. Nibble 8, ECC Lower Nibble Bits 0,1,2,3
            if ((nibble == 8) && (SSAPerBitMaskCheck (Host, 0, bit, maxStrobe, 0, 0, EccMask) != FALSE)) {
              GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
              RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, bit, *DataBytePntr);
              SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
            }

            if ((nibble == 8) && (SSAPerBitMaskCheck (Host, 0 , bit + 4, maxStrobe, 0, 0, EccMask) != FALSE)) { //bit + 4 for decoding the Upper Nibble Bits of EccMask
              // Nibble 17, Upper Nibble Bits 4,5,6,7
              GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble + 9, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
              RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble + 9, bit, *DataBytePntr);
              SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
            }

          } //for bit
        } else {
        GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, ALL_BITS, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
        RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, *DataBytePntr);
        SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
        }
      } else { //ie SetMargin
        // newValue = the difference which will be saved into GetSetDataGroup and used as the piDelay
        // GSM_UPDATE_CACHE caused the LRDIMM backside TxVref to malfunction because backside TxVref shares the same
        // training values for all byte groups and updating the cache for each group was erroneous.
       if ((MarginGroup == RxDqsBitDelay) || (MarginGroup == TxDqBitDelay)) {
         for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {

           //DqMask1Low
           if ((nibble < 4) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, DqMask1Low, 0, 0) != FALSE)) {
             //
             // Nibble + 0 returns the IO swizzled data for the lower nibble of the byte
             //
             GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
             RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble,  bit, *newValuePntr);
             SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
           }

           //DqMask1High
           if ((nibble < 4) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, 0, DqMask1High, 0) != FALSE)) {
             //
             // Nibble + 9 returns the IO swizzled data for the upper nibble of the byte
             //
             GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble + 9, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
             RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble + 9, bit, *newValuePntr);
             SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
           }

           //DqMask1Low
           if (((nibble > 3) && (nibble < 8)) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, DqMask1Low, 0, 0) != FALSE)) {
             //
             // Nibble + 0 returns the IO swizzled data for the lower nibble of the byte
             //
             GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
             RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, bit, *newValuePntr);
             SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
           }

           // DqMask1High
           if (((nibble > 3) && (nibble < 8)) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, 0, DqMask1High, 0) != FALSE)) {
             //
             // Nibble + 9 returns the IO swizzled data for the upper nibble of the byte
             //
             GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble + 9, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
             RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble + 9, bit, *newValuePntr);
             SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
           }

           //ECC. Nibble 8, ECC Lower Bits 0,1,2,3
           if ((nibble == 8) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, 0, 0, EccMask) != FALSE)) {
             GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
             RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, bit, *newValuePntr);
             SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
           }

           if ((nibble == 8) && (SSAPerBitMaskCheck (Host, nibble, bit + 4, maxStrobe, 0, 0, EccMask) != FALSE)) {
             //ECC. Nibble 17, Upper Bits 4,5,6,7
             GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble + 9, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
             RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble + 9, bit, *newValuePntr);
             SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
           }

         } //for bit
       } else {
        GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, ALL_BITS, IoLevel, MarginGroup, GSM_FORCE_WRITE | GSM_WRITE_OFFSET, newValuePntr);
        RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, *newValuePntr);
       }
     }
     }
  } else if (maxStrobe == MSVx4) {  // 18 strobes, 8 and 17 are the ecc strobes -> x4 Dimm
    for (nibble = 0; nibble < maxStrobe; nibble ++) {
      // first 8 strobes (0-7) are for the lower nibble of a byte. Eg. lane 0-3, 8-11, 16-19, etc.
      // the strobes(8-17) are for the upper nibble of a byte. Eg. lane 4-7, 12-15, etc.
      if ((!Host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) {
        continue;
      }  // Skip if this is an ECC strobe when ECC is disabled.
      //ECC - Nibble8 is bits 64 - 67; Nibble 17 is bits 68-71
      if (((EccMask & 0xF) == 0) && (nibble == 8)) {
        continue;
      }  //Continue if EccMask disabled.
      if (((EccMask & 0xF0) == 0) && (nibble == 17)) {
        continue;
      }  //Continue if EccMask disabled.

      if ((nibble < 4) && (((DqMask1Low >> (nibble * 8)) & 0xF) == 0)) {
        continue;
      }
      if (((nibble >= 4) && (nibble < 8)) && (((DqMask1High >> ((nibble - 4) * 8)) & 0xF) == 0)) {
        continue;
      }
      if (((nibble >= 9 ) && (nibble < 13)) && (((DqMask1Low >> ((nibble - 9) * 8)) & 0xF0) == 0)) {
        continue;
      }
      if (((nibble >= 13) && (nibble < 17)) && (((DqMask1High >> ((nibble - 13) * 8)) & 0xF0) == 0)) {
        continue;
      }

      if (SetMarginFunctionFlag != 1) { //GetMargin()
        if ((MarginGroup == RxDqsBitDelay) || (MarginGroup == TxDqBitDelay)) {
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {

           if (((nibble < 8) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, DqMask1Low, 0, 0) != FALSE))                     ||
               (((nibble > 8) && (nibble < 17)) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, 0, DqMask1High, 0) != FALSE)) ||
               (((nibble == 8) || (nibble == 17)) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, 0, 0, EccMask) != FALSE)))
           {
             GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, bit, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
             RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, bit, *DataBytePntr);
             SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
           }

          } //for bit
        } else {
        GetSetDataGroup (Host, Socket, channelInSocket,  NO_SUBCH, Dimm, Rank, nibble, ALL_BITS, IoLevel, MarginGroup, GSM_READ_ONLY, DataBytePntr);
        RcDebugPrint (SDBG_BSSA,"GetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, *DataBytePntr);
        SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
        }
      } else { //SetMargin
        // GSM_UPDATE_CACHE caused the LRDIMM backside TxVref to malfunction because backside TxVref shares the same
        // training values for all byte groups and updating the cache for each group was erroneous.
        if ((MarginGroup == RxDqsBitDelay) || (MarginGroup == TxDqBitDelay)) {
          //
          // Writes to RxDqsBitDelay and TxDqBitDelay must be done for all bits in the nibble
          // Determine which bits in the nibble are changing and write them all at once.
          //
          ZeroMem (NewOffsets, sizeof (NewOffsets));
          BitChange = FALSE;
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            if (((nibble < 8) && (SSAPerBitMaskCheck (Host, nibble, bit, maxStrobe, DqMask1Low, 0, 0) != FALSE))                          ||
                (((nibble > 8) && (nibble < 17)) && (SSAPerBitMaskCheck (Host, nibble, ALL_BITS, maxStrobe, 0, DqMask1High, 0) != FALSE)) ||
                (((nibble == 8) || (nibble == 17)) && (SSAPerBitMaskCheck (Host, nibble, ALL_BITS, maxStrobe, 0, 0, EccMask) != FALSE)))
            {
              NewOffsets[bit] = *newValuePntr;
              BitChange = TRUE;
              RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u bit:%u value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, bit, *DataBytePntr);
            }
          } // bit
          if (BitChange) {
            GetSetDataGroup (Host, Socket, channelInSocket, NO_SUBCH, Dimm, Rank, nibble, ALL_BITS, IoLevel, MarginGroup, GSM_FORCE_WRITE | GSM_WRITE_OFFSET, NewOffsets);
            SSACalcMinMax (Host, *DataBytePntr, MinOffsetDefault, MaxOffsetDefault, MinOffsetPntr, MaxOffsetPntr);
          }
        } else {
         GetSetDataGroup (Host, Socket, channelInSocket, NO_SUBCH, Dimm, Rank, nibble, ALL_BITS, IoLevel, MarginGroup, GSM_FORCE_WRITE | GSM_WRITE_OFFSET, newValuePntr);
         RcDebugPrint (SDBG_BSSA,"SetMarginParamOffset s:%u c:%u d:%u r:%u nibble:%u new_value:%d\n", Socket, channelInSocket, Dimm, Rank, nibble, *newValuePntr);
        }
      }
    } //for-nibble
  } // end else if
} //SSAGetSetMarginMaxMinOffset

/**
  Calculate and return the step size for the MarginGroup

  @param[in]       MarginGroup     - MarginGroup entered from MRC_GT_SSA.

  @retval MarginGroupStepSize                    - Step size for the MarginGroup
**/
//Create a helper function - SWITCH... I/P IoLevel/margingroup to return *StepSize for every margingroup --> TxVref/RxVref .. for other group values --> piStep  = (UINT8)16;
static
UINT16
SSAGetMarginGroupStepSize (
  MRC_GT            MarginGroup
  )
{
  UINT16            MarginGroupStepSize = 0;

  switch (MarginGroup) {
    case RxVref:
      MarginGroupStepSize = (UINT16) 520;
      break;
    case TxVref:
      MarginGroupStepSize = (UINT16) 533;
      break;
    default:  //All other groups default step size = piStep
      MarginGroupStepSize = (UINT16) 16; //piStep  = (UINT8)16;
      break;
  }

  return MarginGroupStepSize;
}


/**
  Function used to get information about the system.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     SystemInfo - Pointer to buffer to be filled with system information.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetSystemInfo) (
  SSA_BIOS_SERVICES_PPI *This,
  MRC_SYSTEM_INFO       *SystemInfo
  )
{
  UINT8     socket = 0;
  PSYSHOST  Host;

  const MRC_SYSTEM_INFO SysInfoConst = {
    MAX_SOCKET,                    // MaxNumberSockets
    MAX_IMC,                       // MaxNumberControllers
    MAX_MC_CH,                     // MaxNumberChannels Per Controller
    MAX_DIMM,                      // MaxNumberDimms
    MAX_RANK_DIMM,                 // MaxNumberRanks
    CPU_SOCKET_BIT_MASK,           // SocketBitMask
    BUS_WIDTH,                     // BusWidth default to 72bits with Ecc else 64bits
    BUS_FREQ,                      // DDR Bus Frequency in MHz
    FALSE,                         // IsEccEnabled -> default is FALSE, sysHost is checked and the value updated in struct
  };

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetSystemInfo ()\n");

  socket = Host->var.mem.currentSocket;

  CopyMem ((UINT8 *) SystemInfo, (UINT8 *) &SysInfoConst, sizeof (MRC_SYSTEM_INFO));

  SystemInfo->SocketBitMask = GetSocketPresentBitMap ();

  if (Host->nvram.mem.eccEn) {
    SystemInfo->BusWidth = GetMaxBitsValid (Host); // 72 bits when ECC is present
    SystemInfo->IsEccEnabled = TRUE;  //Default = FALSE
  }

  SystemInfo->BusFreq = (UINT32)Host->nvram.mem.socket[socket].ddrFreqMHz;      //Default = 1333

  RcDebugPrint (SDBG_BSSA, "End: BiosGetSystemInfo\n");
}

/**
  Function used to get the platform memory voltage (VDD).

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Voltage - Pointer to were the platform's memory voltage (in mV) will be written.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetMemVoltage) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT32                *Voltage
  )
{
  UINT8     socket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetMemVoltage ()\n");

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    //
    // Continue if this socket is not present
    //

    if (IsSocketDimmPopulated (Host, socket) != TRUE) {
      continue;
    }

    if (GetLvmode(Host, socket) == 0) {
      *Voltage = 1500;
    } else if (GetLvmode(Host, socket) == 1) {
      *Voltage = 1350;
    } else if (GetLvmode(Host, socket) == 3) {
      *Voltage = 1200;
    }

    break; //Get system volt. from any one socket
  }//for-loop

  RcDebugPrint (SDBG_BSSA, "End: BiosGetMemVoltage (Platform Voltage: 0x%x)\n", *Voltage);
}

/**
  Function used to set the platform memory voltage.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Voltage - The memory voltage (in mV) to be set on the platform.

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetMemVoltage) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT32                Voltage
  )
{
  UINT8     socket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetMemVoltage (Voltage:%d)\n", Voltage);

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (Host->nvram.mem.socket[socket].enabled) {
      if (Voltage == 1200) {
        Host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_120;
      } else if (Voltage == 1100) {
        Host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_110;
      }

      CheckVdd(Host); // To reset platform voltage
    }
  }

  RcDebugPrint (SDBG_BSSA, "End: BiosSetMemVoltage\n");
}

/**
  Function used to get the DIMM temperature.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket      - Zero based CPU socket number.
  @param[in]      Controller  - Zero based controller number.
  @param[in]      Channel     - Zero based channel number.
  @param[in]      Dimm        - Zero based DIMM number.
  @param[out]     Temperature - Pointer to where the DIMM's temperature will be written.  Units: Celsius with 1 degree precision.

  @retval NotAvailable if the DIMM does not support a temperature sensor.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosGetMemTemp) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  INT32                 *Temperature
  )
{
  UINT8             channelInSocket;
  UINT8             DimmTsMask;
  PSYSHOST          Host;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetMemTemp (Socket:%d, Controller:%d, Channel:%d, Dimm:%d)\n", Socket, Controller, Channel, Dimm);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  dimmNvList = GetDimmNvList(Host, Socket, channelInSocket);

#ifdef DDR5_SUPPORT
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    DimmTsMask = SPD_DIMM_TS_MASK_DDR5;
  } else
#endif  // #ifdef DDR5_SUPPORT
  {
    DimmTsMask = BIT7;
  }

  if (((*dimmNvList)[Dimm].dimmTs & DimmTsMask) != 0) { // if Bit7 != 0(DDR4) or Bits [7, 6] != 0(DDR5) (of byte 14), tsod present
    BiosGetMemTempChip (Host, Socket, channelInSocket, Dimm, Temperature);
  } else {
    return NotAvailable; //TSOD absent; DIMM does not support a temperature sensor
  }

  RcDebugPrint (SDBG_BSSA, "End: BiosGetMemTemp (Temperature: 0x%x)\n", *Temperature);
  return Success;
}

/**
  Function used to get the bitmask of populated memory controllers on a given CPU socket.

  @param[in, out] This    - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket  - Zero based CPU socket number.
  @param[out]     BitMask     - Pointer to where the memory controller bitmask will be stored.  Bit value 1 = populated; bit value 0 = absent. Bit position 0 = memory controller 0; bit position 1 = memory controller 1, etc.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetControllerBitMask) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 *BitMask
  )
{
  UINT8     mcId;
  UINT8     controllerBitmask = 0;
  PSYSHOST  Host;
  UINT8     MaxImc;


  Host = GetSysHostPointer ();
  MaxImc = GetMaxImc ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetControllerBitMask (Socket:%d)\n", Socket);

  if ((Host->nvram.mem.socket[Socket].enabled) && (Host->nvram.mem.socket[Socket].maxDimmPop)) { //IsSocketPresent
    for (mcId = 0; mcId < MaxImc; mcId++) {
      if (Host->var.mem.socket[Socket].imcEnabled[mcId] == 0) {
        continue;
      }
        controllerBitmask |= (1 << mcId);  //0001 OR 0011
    }
      *BitMask = controllerBitmask;
  }

  RcDebugPrint (SDBG_BSSA, "End: BiosGetControllerBitMask (BitMask: 0x%x)\n", *BitMask);
}

/**
  Function used to get the bitmask of populated memory channels on a given memory controller.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket      - Zero based CPU socket number.
  @param[in]      Controller  - Zero based controller number.
  @param[out]     BitMask     - Pointer to where the memory channel bit mask will be stored.  Bit value 1 = populated; bit value 0 = absent. Bit position 0 = memory channel 0; bit position 1 = memory channel 1, etc.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetChannelBitMask) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 *BitMask
  )
{
  UINT8                ch;
  UINT8                chBitmask = 0;
  PSYSHOST             Host;
  struct channelNvram  (*channelNvList)[MAX_CH];
  UINT8                MaxChDdr;
  UINT8                NumChPerMc;

  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetChannelBitMask (Socket:%d, Controller:%d)\n", Socket, Controller);
  channelNvList = GetChannelNvList(Host, Socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if (Host->var.mem.socket[Socket].channelList[ch].mcId != Controller) {
      continue;
    }  //Skip if not the 'Controller' entered to be checked

    //Bit Mask of Channels on a given Controller
    chBitmask |= (1 << (ch - (Controller * NumChPerMc)));
  }

  *BitMask = chBitmask;

  RcDebugPrint (SDBG_BSSA, "End: BiosGetChannelBitMask (BitMask: 0x%x)\n", *BitMask);
}

/**
  Function used to get the bitmask of populated DIMMs on a given memory channel.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Channel    - Zero based channel number.
  @param[out]     BitMask    - Pointer to where the DIMM bit mask will be stored.  Bit value 1 = populated; bit value 0 = absent. Bit position 0 = DIMM 0; bit position 1 = DIMM 1, etc.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetDimmBitMask) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 *BitMask
  )
{
  UINT8               dimm;
  UINT8               channelInSocket;
  UINT8               dimmBitmask = 0;
  PSYSHOST            Host;
  struct socketNvram  *socketNvram;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetDimmBitMask (Socket:%d, Controller:%d, Channel:%d)\n", Socket, Controller, Channel);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);
  socketNvram = &Host->nvram.mem.socket[Socket];
  dimmNvList = GetDimmNvList(Host, Socket, channelInSocket);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }

    dimmBitmask |= (1 << dimm);
  }

  *BitMask = dimmBitmask;

  RcDebugPrint (SDBG_BSSA,"End: BiosGetDimmBitMask (BitMask: 0x%x)\n", *BitMask);
}

/**
  Function used to get the number of ranks in a given DIMM.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket      - Zero based CPU socket number.
  @param[in]      Controller  - Zero based controller number.
  @param[in]      Channel     - Zero based channel number.
  @param[in]      Dimm        - Zero based DIMM number.
  @param[out]     *RankCount  - Pointer to where the rank count will be stored.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetRankInDimm) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  UINT8                 *RankCount
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  UINT8             channelInSocket;
  PSYSHOST          Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetRankInDimm (Socket:%d, Controller:%d, Channel:%d, Dimm:%d)\n", Socket, Controller, Channel, Dimm);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);
  dimmNvList = GetDimmNvList(Host, Socket, channelInSocket);

  *RankCount = (*dimmNvList)[Dimm].numDramRanks;

  RcDebugPrint (SDBG_BSSA,"End: BiosGetRankInDimm (RankCount: 0x%x)\n", *RankCount);
}

/**
  Function used to get the MC logical rank associated with a given DIMM and rank.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket      - Zero based CPU socket number.
  @param[in]      Controller  - Zero based controller number.
  @param[in]      Channel     - Zero based channel number.
  @param[in]      Dimm        - Zero based DIMM number.
  @param[in]      Rank        - Zero based rank number.
  @param[out]     LogicalRank - Pointer to where the logical rank will be stored.

  @retval None.
**/
VOID
(EFIAPI BiosGetLogicalRank) (
  struct _SSA_BIOS_SERVICES_PPI *This,
  UINT8                         Socket,
  UINT8                         Controller,
  UINT8                         Channel,
  UINT8                         Dimm,
  UINT8                         Rank,
  UINT8                         *LogicalRank
  )
{
  UINT8     channelInSocket;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetLogicalRank (Socket:%d, Controller:%d, Channel:%d, Dimm:%d, Rank:%d)\n", Socket, Controller, Channel, Dimm, Rank);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  *LogicalRank = GetLogicalRank(Host, Socket, channelInSocket, Dimm , Rank);

  RcDebugPrint (SDBG_BSSA,"End: BiosGetLogicalRank (LogicalRank: 0x%x)\n", *LogicalRank);
}

/**
  Function used to get DIMM information.

  @param[in, out] This           - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket         - Zero based CPU socket number.
  @param[in]      Controller     - Zero based controller number.
  @param[in]      Channel        - Zero based channel number.
  @param[in]      Dimm           - Zero based DIMM number.
  @param[out]     DimmInfoBuffer - Pointer to buffer to be filled with DIMM information.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetDimmInfo) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  MRC_DIMM_INFO         *DimmInfoBuffer
  )
{
  UINT8                channelInSocket;
  PSYSHOST             Host;
  struct dimmNvram     (*dimmNvList)[MAX_DIMM];
  struct channelNvram  (*channelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetDimmInfo (Socket:%d, Controller:%d, Channel:%d, Dimm:%d)\n", Socket, Controller, Channel, Dimm);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  dimmNvList  = GetDimmNvList(Host, Socket, channelInSocket);
  channelNvList = GetChannelNvList(Host, Socket);

  if ((*dimmNvList)[Dimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT) { //Checks EccEn on that Dimm
    DimmInfoBuffer->EccSupport = TRUE;
  } else {
    DimmInfoBuffer->EccSupport = FALSE;
  }

  DimmInfoBuffer->DimmCapacity = ((*dimmNvList)[Dimm].sdramCapacity << 6); //memSize of dimm in MB, 1unit =64mb
  DimmInfoBuffer->BankCount    = 0x1 << ((*dimmNvList)[Dimm].numBankAddressBits);
  DimmInfoBuffer->RowSize      = 0x1 << ((*dimmNvList)[Dimm].numRowBits);
  DimmInfoBuffer->ColumnSize   = 0x1 << ((*dimmNvList)[Dimm].numColBits);
  DimmInfoBuffer->DeviceWidth  = (UINT8) (4 << (*dimmNvList)[Dimm].dramIOWidth);

  switch ((*dimmNvList)[Dimm].keyByte) { //BYTE2
    case SPD_TYPE_DDR4:
      DimmInfoBuffer->MemoryTech = SsaMemoryDdr4;
      break;
    case SPD_TYPE_DDR5:
      DimmInfoBuffer->MemoryTech = SsaMemoryDdr5;
      break;
    case SPD_TYPE_AEP:
      DimmInfoBuffer->MemoryTech = SsaMemoryDdrT;
      break;
    default:
      break;
  }

  //Fill in the MEMORY_PACKAGE Values - First 3 values are the same for DDR4
  switch ((*dimmNvList)[Dimm].actKeyByte2) { //To get the Actual Module Type - BYTE3
    case SPD_RDIMM:
      DimmInfoBuffer->MemoryPackage = RDimmMemoryPackage;
      break;
    case SPD_UDIMM:
      DimmInfoBuffer->MemoryPackage = UDimmMemoryPackage;
      break;
    case SPD_SODIMM:
      DimmInfoBuffer->MemoryPackage = SoDimmMemoryPackage;
      break;
#ifdef  LRDIMM_SUPPORT
    case SPD_LRDIMM:
      DimmInfoBuffer->MemoryPackage = LrDimmMemoryPackage;
      break;
#endif
    case SPD_MICRO_DIMM:
      DimmInfoBuffer->MemoryPackage = MicroDimmMemoryPackage;
      break;
    case SPD_MINI_RDIMM:
      DimmInfoBuffer->MemoryPackage = MiniRDimmMemoryPackage;
      break;
    case SPD_MINI_UDIMM:
      DimmInfoBuffer->MemoryPackage = MiniUDimmMemoryPackage;
      break;
    case SPD_MINI_CDIMM:
      DimmInfoBuffer->MemoryPackage = MiniCDimmMemoryPackage;
      break;
    case SPD_ECC_SO_UDIMM:
      DimmInfoBuffer->MemoryPackage = SoUDimmEccMemoryPackage;
      break;
    case SPD_ECC_SO_RDIMM:
      DimmInfoBuffer->MemoryPackage = SoRDimmEccMemoryPackage;
      break;
    case SPD_16b_SO_DIMM:
      DimmInfoBuffer->MemoryPackage = SoDimm16bMemoryPackage;
      break;
    case SPD_32b_SO_DIMM:
      DimmInfoBuffer->MemoryPackage = SoDimm32bMemoryPackage;
      break;
    default:
      break;
  }

  //Overwrite for DDR4 or DDR5 the SPD_MICRO_DIMM value which is also equal 4
  if ((((Host->nvram.mem.dramType) == SPD_TYPE_DDR4) && (*dimmNvList)[Dimm].actKeyByte2 == SPD_LRDIMM_DDR4) ||
      (((Host->nvram.mem.dramType) == SPD_TYPE_DDR5) && (*dimmNvList)[Dimm].actKeyByte2 == SPD_LRDIMM_DDR5)) {
#ifdef  LRDIMM_SUPPORT
    DimmInfoBuffer->MemoryPackage = LrDimmMemoryPackage;
#endif
  }
  RcDebugPrint (SDBG_BSSA,"End: BiosGetDimmInfo\n");
}

/**
  Function used to get DIMM unique module ID.

  @param[in, out] This           - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket         - Zero based CPU socket number.
  @param[in]      Controller     - Zero based controller number.
  @param[in]      Channel        - Zero based channel number.
  @param[in]      Dimm           - Zero based DIMM number.
  @param[out]     UniqueModuleId - Pointer to buffer to be filled with DIMM unique module ID.

  @retval Nothing.
**/
VOID
(EFIAPI BiosGetDimmUniqueModuleId) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  SPD_UNIQUE_MODULE_ID  *UniqueModuleId
  )
{
  UINT8             i;
  UINT8             channelInSocket;
  PSYSHOST          Host;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetDimmUniqueModuleId (Socket:%d, Controller:%d, Channel:%d, Dimm:%d)\n", Socket, Controller, Channel, Dimm);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  dimmNvList  = GetDimmNvList(Host, Socket, channelInSocket);

  UniqueModuleId->Location  = (*dimmNvList)[Dimm].SPDMMfLoc;                         //1 byte location
  UniqueModuleId->IdCode    = (*dimmNvList)[Dimm].SPDMMfgId;
  UniqueModuleId->Date.Year = (UINT8)((*dimmNvList)[Dimm].SPDModDate & 0x00FF);      //SPDModDate- 0:7 = Year, 8-15 = Week
  UniqueModuleId->Date.Week = (UINT8)((*dimmNvList)[Dimm].SPDModDate >> 8);          //Masking bits 8-15 for year, and right shifting bits 8:15 for week

  for (i = 0; i < SPD_MODULE_SERIAL; i++) {
    UniqueModuleId->SerialNumber.SerialNumber8[i] = (*dimmNvList)[Dimm].SPDModSN[i];  // Module Serial Number: 4 bytes
  }

  RcDebugPrint (SDBG_BSSA,"End: BiosGetDimmUniqueModuleId\n");
}

/**
  Function used to get DIMM SPD data.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket       - Zero based CPU socket number.
  @param[in]      Controller   - Zero based controller number.
  @param[in]      Channel      - Zero based channel number.
  @param[in]      Dimm         - Zero based DIMM number.
  @param[in]      ByteOffset   - The byte offset in the SPD.
  @param[in]      ByteCount    - The number of bytes to read starting from the offset location specified by the "ByteOffset".
  @param[in]      ReadFromBus  - Flag to determine where to retrieve the SPD value.  TRUE = read from bus.  FALSE = return from MRC cache value.
  @param[out]     Data         - Pointer to buffer to be filled with  DIMM SPD data.

  @retval UnsupportedValue if (ByteOffset + ByteCnt) is larger than the SPD size.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosGetSpdData) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  UINT16                ByteOffset,
  UINT16                ByteCount,
  BOOLEAN               ReadFromBus,
  UINT8                 *Data
  )
{
  UINT8     arrayByteCount = 0;
  UINT8     channelInSocket;
  UINT16    currentByte = 0;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetSpdData (Socket:%d, Controller:%d, Channel:%d, Dimm:%d, ByteOffset:%d, ByteCount:%d, ReadFromBus:%d)\n", Socket, Controller, Channel, Dimm, ByteOffset, ByteCount, ReadFromBus);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);

  if (((ByteOffset + ByteCount) > MAX_SPD_BYTE_DDR4) && (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    return UnsupportedValue;
  }

  //For "ReadFromBus" = True we will always read from the SPD
  for (currentByte = ByteOffset; currentByte < (ByteOffset + ByteCount); currentByte++) {
    SpdReadByte (Socket, channelInSocket, Dimm, currentByte, (Data + arrayByteCount));
    arrayByteCount++;
  }
  //if ReadFromBus = FALSE read the Cached Value for SpdData.. is there a Host struct in NVRAM (MRC)?

  RcDebugPrint (SDBG_BSSA,"End: BiosGetSpdData (Data: 0x%x)\n", *Data);
  return Success;
}

/**
  Function used to perform a JEDEC reset for all the DIMMs on all channels of a given memory controller.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.

  @retval Nothing.
**/
VOID
(EFIAPI BiosJedecReset) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller
  )
{
  UINT8                ch;
  UINT8                chBitMask = 0;
  UINT32               PrevDebugLevel;
  PSYSHOST             Host;
  struct channelNvram  (*channelNvList)[MAX_CH];
  UINT8                MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosJedecReset (Socket:%d, Controller:%d)\n", Socket, Controller);
  channelNvList = GetChannelNvList(Host, Socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    if (Controller == GetMCID(Host, Socket, ch)) {
      chBitMask |= (1 << ch);
    }
  } // ch loop

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  JedecInitSequence (Host, Socket, chBitMask);
  SetDebugLevel (PrevDebugLevel);

  RcDebugPrint (SDBG_BSSA,"End: BiosJedecReset\n");
}

/**
  Function used to reset the I/O for a given memory controller.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.

  @retval Nothing.
**/
VOID
(EFIAPI BiosIoReset) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller
  )
{
  //Similar to IO_Reset, but this is written to reset a single Controller
  UINT8     resetIo = 0;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosIoReset (Socket:%d, Controller:%d)\n", Socket, Controller);

  //
  // Reset DDR IO
  //
  resetIo = 1;
  BiosIoResetChip (Host, Socket, Controller, resetIo);


  //
  // Wait 20 QCLK with reset high.
  //
  FixedQClkDelay (Host, 20);

  //
  // Clear DDR IO Reset
  //
  resetIo = 0;
  BiosIoResetChip (Host, Socket, Controller, resetIo);

  //
  // Wait again 20 QCLK for stable IO.
  //
  FixedQClkDelay (Host, 20);

  RcDebugPrint (SDBG_BSSA,"End: BiosIoReset\n");
}

/**
  Function used to get the specificity of a given margin parameter.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      IoLevel     - I/O level.
  @param[in]      MarginGroup - Margin group.
  @param[out]     Specificity - Pointer to where the margin parameter specificity mask will be stored.

  @retval UnsupportedValue if the IoLevel or MarginGroup parameter value is not supported.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosGetMarginParamSpecificity) (
  SSA_BIOS_SERVICES_PPI    *This,
  MRC_LT                   IoLevel,
  MRC_GT                   MarginGroup,
  MARGIN_PARAM_SPECIFICITY *Specificity
  )
{
  PSYSHOST    Host;
  SSA_STATUS  Status = Success;

  Host = GetSysHostPointer ();

  RcDebugPrint (SDBG_BSSA, "\nStart: BiosGetMarginParamSpecificity (IoLevel: %d, MarginGroup: %d) \n", IoLevel, MarginGroup);

  Status = BiosGetMarginParamSpecificityChip (Host, IoLevel, MarginGroup, Specificity);

  RcDebugPrint (SDBG_BSSA,"End: BiosGetMarginParamSpecificity (Specificity: 0x%x)\n", *Specificity);

  return Status;
}

/**
  Function used to initialize the system before/after a margin parameter's use.
  Prior to calling the GetMarginParamLimits or SetMarginParamOffset functions
  for a margin parameter, this function should be called with the SetupCleanup
  input parameter set to Setup.  When finished with the margin parameter, this
  function should be called with the SetupCleanup input parameter set to
  Cleanup.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket       - Zero based CPU socket number.
  @param[in]      IoLevel      - I/O level.
  @param[in]      MarginGroup  - Margin group.
  @param[in]      SetupCleanup - Specifies setup or cleanup action.

  @retval UnsupportedValue if the IoLevel or MarginGroup parameter value is not supported.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosInitMarginParam) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  MRC_LT                IoLevel,
  MRC_GT                MarginGroup,
  SETUP_CLEANUP         SetupCleanup
  )
{
  PSYSHOST                  Host;
#ifndef SPR_PO_FLAG
  UINT8                     channelInSocket;
#endif
  struct channelNvram       (*channelNvList)[MAX_CH];
  UINT8                     CurrentSocketSaved;
  UINT32                    PrevDebugLevel;
  UINT8                     Channel;
  BOOLEAN                   BpsIsFound = FALSE;
  UINT32                    DaTrainingEnOrg[MAX_CH];
  UINT32                    DTrainingEnOrg[MAX_CH];
  UINT32                    HsioTrainingEnOrg[MAX_CH];
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();

  // Save the current socket.
  CurrentSocketSaved = Host->var.mem.currentSocket;
  Host->var.mem.currentSocket = Socket;

  RcDebugPrint (SDBG_BSSA,"\nStart: BiosInitMarginParam (Socket:%d, IoLevel:%d, MarginGroup:%d, SetupCleanup:%d)\n", Socket, IoLevel, MarginGroup, SetupCleanup);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (GetFmcType (Socket, Channel) == FMC_BWV_TYPE) {
      BpsIsFound = TRUE;
      break;
    }
  }

  switch (MarginGroup) {
    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdAll:
    case CtlGrp0:
    case CtlGrp1:
    case CtlGrp2:
    case CtlGrp3:
    case CtlGrp4:
    case CtlGrp5:
    case CtlAll:
      PrevDebugLevel = GetDebugLevel ();
      channelNvList = GetChannelNvList(Host, Socket);
      // code to support additional shmoo direction. The bit0 defines - "setup"=1;  "clean"=0
      // The bits[2:1] define the margin direction: "low": 0, "high": 1, "high_and_low": 2,  "low_and_high": 3
      if (!(SetupCleanup & 0x1)) {
        // During the setup(BIT0=0x1), the order is to DENORMALIZE first, apply the cmd_oe_val, then backside
        if (IoLevel == DdrLevel) {  // only applicable to frontside
          if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
            SetDebugLevel (SDBG_ERROR);
          }

          //
          // Avoid clock movement for BPS A0
          //
          if ( FmcAvoidClockMovementInSocket (Host, Socket) == FALSE) {
            DeNormalizeCCC (Host, Socket, DENORMALIZE);
          }
          SetDebugLevel (PrevDebugLevel);
          // Set the CPGC DDRT RPQ credit limit, which will also cause a related timer to be enabled by DDRT API functions.
          SetDefaultCpgcRpqCreditLimitIfNeeded (Host, Socket);
        }

        //
        // [SPR A0 PO] The GetMarginsHook() change the setting of SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG
        // Skip this call to make sure this register value not change before and after BSSA test until
        // we finalize the setting.
        //
#ifndef SPR_PO_FLAG
        for (channelInSocket = 0; channelInSocket < MaxChDdr; channelInSocket++) {
          if ((*channelNvList)[channelInSocket].enabled == 0) {
            continue;
          }
          GetMarginsHook (Host, Socket, channelInSocket, 1); //sets 'cmd_oe_on' value
        } // channelInSocket loop
#endif
        if ((IoLevel == LrbufLevel) && (((SetupCleanup >> 1) & 0x3) == 0)) { //reading BITS[2:1] for margin direction
          if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
            SetDebugLevel (SDBG_ERROR);
          }

          if (BpsIsFound) {
            // Save Original DCPMM training mode for BWV
            for (Channel = 0; Channel < MaxChDdr; Channel++) {
              DaTrainingEnOrg[Channel] = 0;
              DTrainingEnOrg[Channel] = 0;
              HsioTrainingEnOrg[Channel] = 0;

              if (GetFmcType (Socket, Channel) == FMC_BWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, TRUE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], NULL);
              }
              else if (GetFmcType (Socket, Channel) == FMC_CWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, TRUE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], &HsioTrainingEnOrg[Channel]);
              }
            }

            // Disale DCPMM training step for BWV
            SetAepTrainingModeSMB (Host, Socket, DISABLE_TRAINING_STEP);
          }
          BacksideShift (Host, Socket, DENORMALIZE);
          // Restore DCPMM Training mode setting for BWV
          if (BpsIsFound) {
            for (Channel = 0; Channel < MaxChDdr; Channel++) {
              if (GetFmcType (Socket, Channel) == FMC_BWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, FALSE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], NULL);
              }
              else if (GetFmcType (Socket, Channel) == FMC_CWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, FALSE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], &HsioTrainingEnOrg[Channel]);
              }
            }
          }
          SetDebugLevel (PrevDebugLevel);
        }
      } else { //During the clean(BIT0=0x0)
        // during the cleanup, the order is backside first, then to apply the cmd_oe_val, finally RENORMALIZE.
        if ((IoLevel == LrbufLevel) && (((SetupCleanup >> 1) & 0x3) == 0)) {
          if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
            SetDebugLevel (SDBG_ERROR);
          }

          if (BpsIsFound) {
            // Save Original DCPMM training mode for BWV
            for (Channel = 0; Channel < MaxChDdr; Channel++) {
              DaTrainingEnOrg[Channel] = 0;
              DTrainingEnOrg[Channel] = 0;
              HsioTrainingEnOrg[Channel] = 0;

              if (GetFmcType (Socket, Channel) == FMC_BWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, TRUE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], NULL);
              }
              else if (GetFmcType (Socket, Channel) == FMC_CWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, TRUE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], &HsioTrainingEnOrg[Channel]);
              }
            }

            // Disale DCPMM training step for BWV
            SetAepTrainingModeSMB (Host, Socket, DISABLE_TRAINING_STEP);
          }
          BacksideShift (Host, Socket, RENORMALIZE);

          if (BpsIsFound) {
            // Restore DCPMM Training mode setting for BWV
            for (Channel = 0; Channel < MaxChDdr; Channel++) {
              if (GetFmcType (Socket, Channel) == FMC_BWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, FALSE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], NULL);
              }
              else if (GetFmcType (Socket, Channel) == FMC_CWV_TYPE) {
                GetRestoreAepTrainingModeSMB (Host, Socket, Channel, FALSE, &DaTrainingEnOrg[Channel], &DTrainingEnOrg[Channel], &HsioTrainingEnOrg[Channel]);
              }
            }
          }
          SetDebugLevel (PrevDebugLevel);
        }

        //
        // [SPR A0 PO] The GetMarginsHook() change the setting of SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG
        // Skip this call to make sure this register value not change before and after BSSA test until
        // we finalize the setting.
        //
#ifndef SPR_PO_FLAG
        for (channelInSocket = 0; channelInSocket < MaxChDdr; channelInSocket++) {
          if ((*channelNvList)[channelInSocket].enabled == 0) {
            continue;
          }
          GetMarginsHook (Host, Socket, channelInSocket, 0); //sets cmd_oe_on
        } // channelInSocket loop
#endif
        if (IoLevel == DdrLevel) {
          // Disable the CPGC DDRT RPQ credit limit, which will also prevent the related timer from being enabled by DDRT API functions
          ClearCpgcRpqCreditLimitIfNeeded (Host, Socket);
          if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
            SetDebugLevel (SDBG_ERROR);
          }

          //
          // Avoid clock movement for BPS A0
          //
          if (FmcAvoidClockMovementInSocket (Host, Socket) == FALSE) {
            DeNormalizeCCC (Host, Socket, RENORMALIZE);
          }
          SetDebugLevel (PrevDebugLevel);
        }
      }
      break;

    case TxDqsDelay:
    case WrLvlDelay:
      if (IoLevel == DdrLevel) {
        PrevDebugLevel = GetDebugLevel ();
        if ((SetupCleanup & 0x1) != 0) {
          // pull-in margin parameter to restore delays
          WLPushOut (Host, Socket, RENORMALIZE, 0);
        } else {
          // push-out margin parameter to increase low-side range
          WLPushOut (Host, Socket, DENORMALIZE, 64);
        }
        SetDebugLevel (PrevDebugLevel);
      }
      break;
    case CmdVref:
      if (IoLevel == DdrLevel) {
        PrevDebugLevel = GetDebugLevel ();
        if (!(SetupCleanup & 0x1)) {
          // Set the CPGC DDRT RPQ credit limit, which will also cause a related timer to be enabled by DDRT API functions.
          SetDefaultCpgcRpqCreditLimitIfNeeded (Host, Socket);
        } else { //During the clean(BIT0=0x0)
          // Disable the CPGC DDRT RPQ credit limit, which will also prevent the related timer from being enabled by DDRT API functions
          ClearCpgcRpqCreditLimitIfNeeded (Host, Socket);
        }
        SetDebugLevel (PrevDebugLevel);
      }
      break;
    default:
      break;
  }//MarginGroup

  // Restore the current socket
  Host->var.mem.currentSocket = CurrentSocketSaved;

  RcDebugPrint (SDBG_BSSA, "End: BiosInitMarginParam actual margin group:%d \n", MarginGroup);

  return Success;
}

/**
  Helper function to get DQ and ECC lanes bits mask according to the info from LaneMasks array

  @param[in]      Socket      - Zero based CPU socket number.
  @param[in]      LaneMasks   - Pointer to array of masks of the lanes/strobes that participate to the margin parameter offset limit calculation.
  @param[out]     DqMaskLow   - Pointer to lower 32 bits DQ bit mask for Ddr4, or 32 bits DQ bit mask for Ddr5 sub-ch A.
  @param[out]     DqMaskHigh  - Pointer to higher 32 bits DQ bit mask for Ddr4, or 32 bits DQ bit mask for Ddr5 sub-ch B.
  @param[out]     EccMask     - Pointer to 16 bit Ecc bit mask. Ddr4 only uses lower 8 bits. For Ddr5, lower 8 bits for sub-ch A and higher 8 bits for sub-ch B.

  @retval n/a
**/
VOID
GetDqAndEccMask (
  IN UINT8          Socket,
  IN CONST UINT8    *LaneMasks,
  OUT UINT32        *DqMaskLow,
  OUT UINT32        *DqMaskHigh,
  OUT UINT16        *EccMask
  )
{
  UINT8     ArrayElements = 0;
  PSYSHOST  Host;
  UINT8     Index;

  Host = GetSysHostPointer ();

  *DqMaskLow = 0;
  *DqMaskHigh = 0;
  *EccMask = 0;

  //
  // DDR4
  // Map lower 32bits lane error status to the LaneMasks[3:0]
  // Map upper 32bits lane error status to the LaneMasks[7:4]
  // Map 8 bits ECC lane error status to the LaneMasks[8]
  //
  if (Host->nvram.mem.eccEn) {
    ArrayElements = MAX_BITS / BYTE_WIDTH;
  } else {
    ArrayElements = MAX_BITS / BYTE_WIDTH - NUM_ECC_BYTE;
  }

  //
  //Separating the Array Elements into DQ Masks(lo,hi) and a ECC Mask
  //
  for (Index = 0; Index < ArrayElements; Index++) {
    if (Index <= OFFSET_LOWER_LANES_END) {
      *DqMaskLow |= (UINT32) (((*(LaneMasks + Index)) << (Index * BYTE_WIDTH)) & DWORD_MASK);
    } else if ((Index >= OFFSET_UPPER_LANES_START) && (Index <= OFFSET_UPPER_LANES_END)) {
      *DqMaskHigh |= (UINT32) (((*(LaneMasks + Index)) << ((Index - OFFSET_UPPER_LANES_START) * BYTE_WIDTH)) & DWORD_MASK);
    } else if (Index == OFFSET_ECC_LANES) {
      *EccMask |= (UINT16) (*(LaneMasks + Index) & BYTE_MASK);
    }
#ifdef DDR5_SUPPORT
    else if (Index == OFFSET_ECC_SUBCH_B_LANES) {
      //
      // DDR5
      // Map the SUB_CH0 32bits lane error status to the LaneMasks[3:0]
      // Map the SUB_CH0 8bits ECC lane error status to the LaneMasks[4]
      // Map the SUB_CH1 32bits lane error status to the LaneMasks[8:5]
      // Map the SUB_CH1 8bits ECC lane error status to the laneMasks[9]
      //
      *EccMask |= (UINT16) (((*(LaneMasks + Index)) << BYTE_WIDTH) & (BYTE_MASK << BYTE_WIDTH));
    }
#endif
  }

}

/**
  Function used to get the minimum and maximum offsets that can be applied to a given margin parameter and the time delay in micro seconds for the new value to take effect.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket      - Zero based CPU socket number.
  @param[in]      Controller  - Zero based controller number.
  @param[in]      Channel     - Zero based channel number.
  @param[in]      Dimm        - Zero based DIMM number.
  @param[in]      Rank        - Zero based physical rank number.
  @param[in]      LaneMasks   - Pointer to array of masks of the lanes/strobes that participate to the margin parameter offset limit calculation.  It is applicable only if margin parameter is per-strobe or per-bit/lane controllable.  The number of array elements is the BusWidth value from the GetSystemInfo() function divided by 8.  If a margin parameter is strobe specific and if any lane associated with that strobe is set in the mask then that strobe is selected.  For example, for a LaneMasks value of [0x00, ... 0x00, 0x01], only the min/max offset of the first strobe group is returned.
  @param[in]      IoLevel     - I/O level.
  @param[in]      MarginGroup - Margin group.
  @param[out]     MinOffset   - Pointer to where the minimum offset from the current setting supported by the margin parameter will be stored.  This is a signed value.
  @param[out]     MaxOffset   - Pointer to where the maximum offset from the current setting supported by the margin parameter parameter will be stored.  This is a signed value.
  @param[out]     Delay       - Pointer to where the wait time in micro-seconds that is required for the new setting to take effect will be stored.
  @param[out]     StepUnit    - Pointer to where the margin parameter's step size will be stored.  For timing parameters, the units are tCK / 2048.  For voltage parameters, the units are Vdd / 100.

  @retval UnsupportedValue if the IoLevel or MarginGroup parameter value is not supported.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosGetMarginParamLimits) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  UINT8                 Rank,
  CONST UINT8           *LaneMasks,
  MRC_LT                IoLevel,
  MRC_GT                MarginGroup,
  INT16                 *MinOffset,
  INT16                 *MaxOffset,
  UINT16                *Delay,
  UINT16                *StepUnit
  )
{
  PSYSHOST                  Host;
  SSA_STATUS                Status = Success;
  UINT8                     SetMarginFunctionFlag = 0;  //Set to have SSAGetSetMarginMaxMinOffset() behave as GetMarginParamLimits
  UINT8                     channelInSocket;
  UINT16                    EccMask = 0;
  UINT8                     maxStrobe = 0;
  UINT8                     Loop = 0;
  UINT8                     Clk = 0;
  UINT16                    MinDefaultDelayVal = 0;
  UINT16                    MaxDefaultDelayVal = 0;
  UINT16                    MinTrainingValOfGroup = 0;
  UINT16                    MaxTrainingValOfGroup = 0;
  INT16                     DataByte;
  INT16                     newValue = 0;
  INT16                     tempMaxOffsets[2] = {0,0};
  INT16                     tempMinOffsets[2] = {0,0};
  MRC_GT                    ActualMarginGroup;
  UINT64_STRUCT             DqMask1;
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  MARGIN_PARAM_SPECIFICITY  Specificity;
  SYS_SETUP                 *Setup;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosGetMarginParamLimits (Socket:%d, Controller:%d, Channel:%d, Dimm:%d, Rank:%d, IoLevel:%d, MarginGroup:%d)\n", Socket, Controller, Channel, Dimm, Rank, IoLevel, MarginGroup);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);
  dimmNvList = GetDimmNvList(Host, Socket, channelInSocket);

  //Check if the IoLevel and margin group is supported or not
  if (BiosGetMarginParamSpecificity (This, IoLevel, MarginGroup, &Specificity) == UnsupportedValue) {
    return UnsupportedValue;
  }

  //For 'LrbufLevel' a.) if margin groups CmdAll/CtlAll/CmdVref and "Rev02 register dimm" not present, Exit function. b.) LRDIMM not present, Exit.
  if (IoLevel == LrbufLevel) {
    // check if the margin group is support by Rev02 register or AEP DIMM
    if (((MarginGroup == CmdVref)) && ((!IsDdr4RegisterRev2(Host, Socket, channelInSocket, Dimm)) && (!((*dimmNvList)[Dimm].DcpmmPresent)))) {
      return UnsupportedValue;
    }
    if (((MarginGroup == CmdAll) || (MarginGroup == CtlAll)) && (!IsDdr4RegisterRev2 (Host, Socket, channelInSocket, Dimm) || ((*dimmNvList) [Dimm].DcpmmPresent && !(Setup->mem.enableNgnBcomMargining)))) {
      return UnsupportedValue;
    }
    // check if the margin group is supported by AEP DIMM
    if (((MarginGroup == CmdGrp3) || (MarginGroup == CmdGrp4)) && (!(((*dimmNvList)[Dimm].DcpmmPresent) && (Setup->mem.enableNgnBcomMargining)))) {
      return UnsupportedValue;
    }
    // check if the dimm is LRDIMM
    if (((MarginGroup == RxDqsDelay) || (MarginGroup == TxDqDelay) || (MarginGroup == RxVref) || (MarginGroup == TxVref)) && (!IsLrdimmPresent (Socket, channelInSocket, Dimm))){
      return UnsupportedValue;
    }
  }

  if (((MarginGroup == EridDelay) || (MarginGroup == EridVref)) && (!((*dimmNvList)[Dimm].DcpmmPresent))) {
    return UnsupportedValue;
  }

  DqMask1.lo = 0;
  DqMask1.hi = 0;

  *StepUnit = SSAGetMarginGroupStepSize(MarginGroup);  //Get the Step Size of the particular Margin Group

  //Getting the number of Array Elements
  GetDqAndEccMask (Socket, LaneMasks, &DqMask1.lo, &DqMask1.hi, &EccMask);

  switch (MarginGroup) {

    case RecEnDelay:
    case RxDqsDelay:
    case TxDqsDelay:
    case TxDqDelay:
    case RxDqsBitDelay:
    case TxDqBitDelay:
    case RxSamplerEvenOdd:
    case RxSamplerEven:
    case RxSamplerOdd:
    case RxVref:
    case TxVref:
    case WrLvlDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxDqDelay:
    case RxEq:
    case TxEq:
    case TxEqTap2:

      if ((MarginGroup == WrLvlDelay) || ((MarginGroup == RxDqsDelay) && (IoLevel == DdrLevel))) {
        Loop = 2;  //To get Limits for both (TxDqsDelay and TxDqDelay ~ WrLvlDelay) and (RxDqsPDelay and RxDqsNDelay) for the RxDqsDelay
      } else {
        Loop = 1;
      }

      while (Loop != 0) {
        if ((MarginGroup == WrLvlDelay) && (Loop == 2)) {
          ActualMarginGroup = TxDqsDelay;
        } else if ((MarginGroup == WrLvlDelay) && (Loop == 1)) {
          ActualMarginGroup = TxDqDelay;
        } else if ((MarginGroup == RxDqsDelay) && (IoLevel == DdrLevel) && (Loop == 2)) {
          ActualMarginGroup = RxDqsPDelay;
        }else if ((MarginGroup == RxDqsDelay) && (IoLevel == DdrLevel) && (Loop == 1)) {
          ActualMarginGroup = RxDqsNDelay;
        } else {
          ActualMarginGroup = MarginGroup;
        }

        GetDataGroupLimits(Host, IoLevel, ActualMarginGroup, &MinDefaultDelayVal, &MaxDefaultDelayVal, Delay);  // Get Min/MaxOffsetDefault and Delay
        UpdateDdrtGroupLimits (Host, Socket, channelInSocket, Dimm, IoLevel, ActualMarginGroup, &MinDefaultDelayVal, &MaxDefaultDelayVal);
        *MaxOffset = 0x7FFF; //MaxOffsetDefault; // Start with the largest value for a INT16
        *MinOffset = - (0x7FFF); //LARGE_MIN_OFFSET; // Start with the smallest value for a INT16

        if (MarginGroup == WrLvlDelay) {
          maxStrobe = MAX_STROBE;
        } else {
          maxStrobe =  GetMaxStrobe (Host, Socket, channelInSocket, Dimm, ActualMarginGroup, IoLevel);
        }

        //Calling the SSA Helper function which calculates the Min/Max Offset values based of the DataByte value (calculated from GetSetDataGroup() in the helper)
        SSAGetSetMarginMaxMinOffset(Host, Socket, channelInSocket, Dimm, Rank, SetMarginFunctionFlag, IoLevel, ActualMarginGroup, MinOffset, MaxOffset, MinDefaultDelayVal, MaxDefaultDelayVal, maxStrobe, &DataByte, &newValue, DqMask1.lo, DqMask1.hi, EccMask);


        if (((MarginGroup == WrLvlDelay) || ((MarginGroup == RxDqsDelay) && (IoLevel == DdrLevel))) && (Loop == 2)) {
          tempMaxOffsets[1] = *MaxOffset;
          tempMinOffsets[1] = *MinOffset;
        } else if (((MarginGroup == WrLvlDelay) || ((MarginGroup == RxDqsDelay) && (IoLevel == DdrLevel))) && (Loop == 1)) {
          tempMaxOffsets[0] = *MaxOffset;
          tempMinOffsets[0] = *MinOffset;
        }
        Loop = Loop - 1;
      } //while

      //WrLvlDelay margin parameter move both TxDqsDelay and TxDqDelay
      // RxDqsDelay is similar
      //WrLvlDelay limits: Take the Min of the 2 Max's and the Max of the two Min's
      if ((MarginGroup == WrLvlDelay) || ((MarginGroup == RxDqsDelay) && (IoLevel == DdrLevel))){
        *MaxOffset = MIN(tempMaxOffsets[1], tempMaxOffsets[0]);
        *MinOffset = MAX(tempMinOffsets[1], tempMinOffsets[0]);
      }

      //override timing parameter limit to within +/-32
      //override voltage parameter limit to within +/-64
      if ((MarginGroup == RxVref) || (MarginGroup == TxVref)) {
        if (*MaxOffset > MAX_VOLTAGE_MARGIN_PAMRAM_OFFSET ) {
          *MaxOffset = MAX_VOLTAGE_MARGIN_PAMRAM_OFFSET;
        }
        if (*MinOffset < MIN_VOLTAGE_MARGIN_PAMRAM_OFFSET) {
          *MinOffset = MIN_VOLTAGE_MARGIN_PAMRAM_OFFSET;
        }
      } else if ((MarginGroup == WrLvlDelay) || (MarginGroup == RecEnDelay) || (MarginGroup == TxDqsDelay) ) {
        if (*MaxOffset > (2 * MAX_TIMING_MARGIN_PAMRAM_OFFSET + 1)) {
          *MaxOffset = (2 * MAX_TIMING_MARGIN_PAMRAM_OFFSET + 1);
        }
        if (*MinOffset < 2 * MIN_TIMING_MARGIN_PAMRAM_OFFSET) {
          *MinOffset = 2 * MIN_TIMING_MARGIN_PAMRAM_OFFSET;
        }
      } else if ((MarginGroup == RxSamplerEvenOdd) || (MarginGroup == RxSamplerOdd) || (MarginGroup == RxSamplerEven)) {
        if (*MaxOffset > MAX_RXSAMPLER_MARGIN_PAMRAM_OFFSET ) {
          *MaxOffset = MAX_RXSAMPLER_MARGIN_PAMRAM_OFFSET;
        }
        if (*MinOffset < MIN_RXSAMPLER_MARGIN_PAMRAM_OFFSET) {
          *MinOffset = MIN_RXSAMPLER_MARGIN_PAMRAM_OFFSET;
        }
      } else {
        //timing margin parameter
        if (*MaxOffset > MAX_TIMING_MARGIN_PAMRAM_OFFSET) {
          *MaxOffset = MAX_TIMING_MARGIN_PAMRAM_OFFSET;
        }
        if (*MinOffset < MIN_TIMING_MARGIN_PAMRAM_OFFSET) {
          *MinOffset = MIN_TIMING_MARGIN_PAMRAM_OFFSET;
        }
      }  // end timing margin parameters

      break;

    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdGrp3:
    case CmdGrp4:
    case CmdAll:
      //Calling GetCmdGroupLimits() to return the Limits for MinOffsetDefault/MaxOffsetDefault for the CMD Group
      GetCmdGroupLimits (Socket, channelInSocket, Dimm, IoLevel, MarginGroup, &MinDefaultDelayVal, &MaxDefaultDelayVal);

      //For backside REV2 margining different min/max values are used
      if (IoLevel == LrbufLevel) {
        if (!((*dimmNvList)[Dimm].DcpmmPresent)) {
          MaxDefaultDelayVal = 31;
          MinDefaultDelayVal = 0;
        } else {
          MaxDefaultDelayVal = MAX_DDRT_BACKSIDE_CCC_RANGE;
          MinDefaultDelayVal = 0;
        }
      }

      //Initialize the min/max of all group value before calling GetSetCmdGroupDelay()
      //The GetSetCmdGroupDelay() iterate each groups, and compare it to the min/max, then update the min/max.
      MinTrainingValOfGroup = MaxDefaultDelayVal;
      MaxTrainingValOfGroup = MinDefaultDelayVal;

      //Calling GetSetCmdGroupDelay() to return the Limits for MinTrainingValOfGroup/MaxTrainingValOfGroup of that particular CMD Group entered
      GetSetCmdGroupDelayCore (Host, Socket, channelInSocket, Dimm, IoLevel, MarginGroup, GSM_READ_ONLY, &DataByte, &MinTrainingValOfGroup, &MaxTrainingValOfGroup);
      *MaxOffset = MaxDefaultDelayVal - MaxTrainingValOfGroup;
      *MinOffset = MinDefaultDelayVal - MinTrainingValOfGroup;
      break;

    case CmdVref:
      GetSetCmdVrefCore (Host, Socket, channelInSocket, Dimm, IoLevel, GSM_READ_ONLY, &DataByte);
      GetCmdGroupLimits (Socket, channelInSocket, Dimm, IoLevel, CmdVref, &MinDefaultDelayVal, &MaxDefaultDelayVal);
      if (!((*dimmNvList)[Dimm].DcpmmPresent) && ((IoLevel == LrbufLevel) || ((IoLevel == DdrLevel) && InternalRcdVrefCaEnabled () && (Host->nvram.mem.dimmTypePresent == RDIMM)))) {
        *MaxOffset = 20 - DataByte;      // based on the range defined in the GetSetCmdVrefCore() of the \Core\Mem\MemJedec.c
        *MinOffset = -20 - DataByte;
      } else {
        *MaxOffset = MaxDefaultDelayVal - DataByte;
        *MinOffset = MinDefaultDelayVal - DataByte;
      }

      break;

    case CtlGrp0:
    case CtlGrp1:
    case CtlGrp2:
    case CtlGrp3:
    case CtlGrp4:
    case CtlGrp5:
    case CtlAll:
      //Calling GetCmdGroupLimits() to return the Limits for MinOffsetDefault/MaxOffsetDefault for the CTL Group - CTL/CMD group limits same
      GetCmdGroupLimits (Socket, channelInSocket, Dimm, IoLevel, MarginGroup, &MinDefaultDelayVal, &MaxDefaultDelayVal);

      //For backside REV2 margining different min/max values are used
      if (IoLevel == LrbufLevel) {
        if (!((*dimmNvList)[Dimm].DcpmmPresent)) {
          MaxDefaultDelayVal = 31;
          MinDefaultDelayVal = 0;
        } else {
          MaxDefaultDelayVal = MAX_DDRT_BACKSIDE_CCC_RANGE;
          MinDefaultDelayVal = 0;
        }
      }

      //Initialize the min/max of all group value before calling GetSetCmdGroupDelay()
      //The GetSetCmdGroupDelay() iterate each groups, and compare it to the min/max, then update the min/max.
      MinTrainingValOfGroup = MaxDefaultDelayVal;
      MaxTrainingValOfGroup = MinDefaultDelayVal;

      //Calling GetSetCtlGroupDelay() to return the Limits for MinTrainingValOfGroup/MaxTrainingValOfGroup of that particular CTL Group entered
      GetSetCtlGroupDelayCore (Host, Socket, channelInSocket, Dimm, IoLevel, MarginGroup, GSM_READ_ONLY, &DataByte, &MinTrainingValOfGroup, &MaxTrainingValOfGroup);
      *MaxOffset = MaxDefaultDelayVal - MaxTrainingValOfGroup;
      *MinOffset = MinDefaultDelayVal - MinTrainingValOfGroup;
      break;

    case EridDelay:
       MinDefaultDelayVal = 0;
       MaxDefaultDelayVal = 128 * 255 + 64 + 63; // cycle delay, logical delay, pi delay
       //Calling GetSetClkDelay() to return the current logic + PI delay
       Clk = GetDdrioClkNumberDdrt (Host, Socket, channelInSocket, Dimm);
       GetSetClkDelay (Host, Socket, channelInSocket, Clk, GSM_READ_ONLY, (INT16 *)&DataByte);
       GetSetSxpGnt2Erid(Host, Socket, channelInSocket, Dimm, GSM_READ_ONLY, &newValue);
       *MaxOffset = MaxDefaultDelayVal - newValue * 128 - DataByte;
       *MinOffset = MinDefaultDelayVal - newValue * 128 - DataByte;

       // cap the range to +/-128
       if (*MaxOffset > (MAX_TIMING_MARGIN_PAMRAM_OFFSET * 4)) {
         *MaxOffset = MAX_TIMING_MARGIN_PAMRAM_OFFSET * 4 + 3; // 127
       }
       if (*MinOffset < (MIN_TIMING_MARGIN_PAMRAM_OFFSET * 4) ) {
         *MinOffset = MIN_TIMING_MARGIN_PAMRAM_OFFSET * 4;  // -128
       }
       break;

    case EridVref:
       GetDataGroupLimits(Host, IoLevel, MarginGroup, &MinDefaultDelayVal, &MaxDefaultDelayVal, Delay);
       GetSetEridVref(Host, Socket, channelInSocket, GSM_READ_ONLY, &DataByte);
       *MaxOffset = MaxDefaultDelayVal - DataByte;
       *MinOffset = MinDefaultDelayVal - DataByte;
       break;

    default:
      Status = UnsupportedValue;
      break;
  } // end of switch

  RcDebugPrint (SDBG_BSSA,"End: BiosGetMarginParamLimits (MinOffset: %d, MaxOffset: %d, Delay: %d, StepUnit: %d)\n", *MinOffset, *MaxOffset, *Delay, *StepUnit);
  return Status;
}

/**
  Function used to set the offset of a margin parameter.

  @param[in, out] This          - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket        - Zero based CPU socket number.
  @param[in]      Controller    - Zero based controller number.
  @param[in]      Channel       - Zero based channel number.
  @param[in]      Dimm          - Zero based DIMM number.
  @param[in]      Rank          - Zero based physical rank number.
  @param[in]      LaneMasks     - Pointer to array of masks of the lanes/strobes that participate to the margin parameter offset limit calculation.  It is applicable only if margin parameter is per-strobe or per-bit/lane controllable.  The number of array elements is the BusWidth value from the GetSystemInfo() function divided by 8.  If a margin parameter is strobe specific and if any lane associated with that strobe is set in the mask then that strobe is selected.  For example, for a LaneMasks value of [0x00, ... 0x00, 0x01], only the min/max offset of the first strobe group is returned.
  @param[in]      IoLevel       - Id of the I/O level to access. Can be VmseLevel(0), DdrLevel(1), LrbufLevel(2).
  @param[in]      MarginGroup   - Id of the margin group. Can be RcvEna(0), RdT(1), WrT(2), WrDqsT(3), RdV(4) or WrV(5).
  @param[in]      CurrentOffset - Signed value of the current offset setting.  The range of valid values is available  via the GetMarginParamLimits() function.
  @param[in]      NewOffset     - Signed value of the new offset setting.  The range of valid values is available  via the GetMarginParamLimits() function.

  @retval UnsupportedValue if the IoLevel or MarginGroup parameter value is not supported.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosSetMarginParamOffset) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  UINT8                 Channel,
  UINT8                 Dimm,
  UINT8                 Rank,
  CONST UINT8           *LaneMasks,
  MRC_LT                IoLevel,
  MRC_GT                MarginGroup,
  INT16                 CurrentOffset,
  INT16                 NewOffset
  )
{
  PSYSHOST                  Host;
  SSA_STATUS                SsaStatus = Success;
  UINT8                     SetMarginFunctionFlag = 1;  //Set to have SSAGetSetMarginMaxMinOffset() behave as SetMarginParamOffset
  UINT8                     channelInSocket;
  UINT16                    EccMask = 0;
  UINT8                     maxStrobe = 0;
  UINT8                     Clk = 0;
  UINT8                     Mode;
  INT16                     MinOffset = 0;  //Used only by GetMarginParamLimits, so set to 0
  INT16                     MaxOffset = 0;  //Used only by GetMarginParamLimits, so set to 0
  UINT16                    MinOffsetDefault = 0;
  UINT16                    MaxOffsetDefault = 0;
  INT16                     DataByte = 0;
  INT16                     newValue = 0;
  INT16                     totalValue;
  UINT64_STRUCT             DqMask1;
  UINT32                    PrevDebugLevel;
  struct channelNvram       (*channelNvList)[MAX_CH];
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  BOOLEAN                   IoRegisterHasBeenChanged;
  MARGIN_PARAM_SPECIFICITY  Specificity;
  SYS_SETUP                 *Setup;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetMarginParamOffset (Socket:%d, Controller:%d, Channel:%d, Dimm:%d, Rank:%d, IoLevel:%d, MarginGroup:%d, CurrentOffset:%d, NewOffset:%d)\n", Socket, Controller, Channel, Dimm, Rank, IoLevel, MarginGroup, CurrentOffset, NewOffset);
  channelNvList = GetChannelNvList(Host, Socket);
  channelInSocket = SSAGetChannelInSocket(Controller, Channel);
  dimmNvList = GetDimmNvList(Host, Socket, channelInSocket);

  // check if the IoLevel and margin group is supported or not
  if (BiosGetMarginParamSpecificity (This, IoLevel, MarginGroup, &Specificity) == UnsupportedValue) {
    return UnsupportedValue;
  }

  if (IoLevel == LrbufLevel) {
    // check if the margin group is support by Rev02 register dimm
    if (((MarginGroup == CmdVref)) && ((!IsDdr4RegisterRev2(Host, Socket, channelInSocket, Dimm)) && (!((*dimmNvList)[Dimm].DcpmmPresent)))) {
      return UnsupportedValue;
    }
    if (((MarginGroup == CmdAll) || (MarginGroup == CtlAll)) && (!IsDdr4RegisterRev2(Host, Socket, channelInSocket, Dimm) || (((*dimmNvList)[Dimm].DcpmmPresent) && !(Setup->mem.enableNgnBcomMargining)))) {
      return UnsupportedValue;
    }
    // check if the margin group is supported by AEP DIMM
    if (((MarginGroup == CmdGrp3) || (MarginGroup == CmdGrp4)) && (!(((*dimmNvList)[Dimm].DcpmmPresent) && (Setup->mem.enableNgnBcomMargining)))) {
      return UnsupportedValue;
    }
    // check if the dimm is LRDIMM
    if (((MarginGroup == RxDqsDelay) || (MarginGroup == TxDqDelay) || (MarginGroup == RxVref) || (MarginGroup == TxVref)) && (!IsLrdimmPresent (Socket, channelInSocket, Dimm))){
      return UnsupportedValue;
    }
  }

  DqMask1.lo = 0;
  DqMask1.hi = 0;

  IoRegisterHasBeenChanged = FALSE;

  //Getting the number of Array Elements
  GetDqAndEccMask (Socket, LaneMasks, &DqMask1.lo, &DqMask1.hi, &EccMask);

  if (SsaStatus == Success) {
    newValue = NewOffset - CurrentOffset;  //CurrentOffset - NewOffset; //newValue --> the difference which will be applied to Cmd/Ctl/CmdVref

    PrevDebugLevel = GetDebugLevel ();
    if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
      SetDebugLevel (SDBG_ERROR);
    }

    //Saving newValue into concerned MarginGroup using the GSM_WRITE_OFFSET mode
    switch (MarginGroup) {
      case RecEnDelay:
      case RxDqsDelay:
      case TxDqsDelay:
      case TxDqDelay:
      case RxDqsBitDelay:
      case TxDqBitDelay:
      case RxSampler:
      case RxSamplerEvenOdd:
      case RxSamplerEven:
      case RxSamplerOdd:
      case RxVref:
      case TxVref:
      case WrLvlDelay:
      case RxDqsPDelay:
      case RxDqsNDelay:
      case RxDqDelay:
      case RxEq:
      case TxEq:
      case TxEqTap2:

        if (MarginGroup == WrLvlDelay) {
          maxStrobe = MAX_STROBE;
        } else {
          maxStrobe = GetMaxStrobe (Host, Socket, channelInSocket, Dimm, MarginGroup, IoLevel);
        }

       //Calling the SSA Helper function which calculates/programs the "offset"  based of the newValue (calculated from GetSetDataGroup() in the helper)
       //WrLvlDelay: For this margin parameter we move TxDqsDelay and TxDqDelay; so the offset of both these parameters will be set when WrLvlDelay is called
       if ((IoLevel == LrbufLevel) && (MarginGroup == WrLvlDelay)) {
         //To move WrLvlDelay we need to move TxDqsDelay  and TxDqDelay simultaneously
         SSAGetSetMarginMaxMinOffset(Host, Socket, channelInSocket, Dimm, Rank, SetMarginFunctionFlag, IoLevel, TxDqsDelay, &MinOffset, &MaxOffset, MinOffsetDefault, MaxOffsetDefault, maxStrobe, &DataByte, &NewOffset, DqMask1.lo, DqMask1.hi, EccMask);
         SSAGetSetMarginMaxMinOffset(Host, Socket, channelInSocket, Dimm, Rank, SetMarginFunctionFlag, IoLevel, TxDqDelay, &MinOffset, &MaxOffset, MinOffsetDefault, MaxOffsetDefault, maxStrobe, &DataByte, &NewOffset, DqMask1.lo, DqMask1.hi, EccMask);
       } else {
         SSAGetSetMarginMaxMinOffset (Host, Socket, channelInSocket, Dimm, Rank, SetMarginFunctionFlag, IoLevel, MarginGroup, &MinOffset, &MaxOffset, MinOffsetDefault, MaxOffsetDefault, maxStrobe, &DataByte, &NewOffset, DqMask1.lo, DqMask1.hi, EccMask);
       }

        IoRegisterHasBeenChanged = TRUE;
        break;

      case CmdGrp0:
      case CmdGrp1:
      case CmdGrp2:
      case CmdGrp3:
      case CmdGrp4:
      case CmdAll:
      //
      // Skip Margining MA14, MA15, MA16 for Late Cmd Training for BPS dimms
      //
        Mode = GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE;
        if ((Setup->mem.dfxMemSetup.dfxOptions & CR_CMD_FUB_MARGIN) != 0) {
          Mode |= GSM_SKIP_CMD_FUB;
        }
        GetSetCmdGroupDelayCore (Host, Socket, channelInSocket, Dimm, IoLevel, MarginGroup, Mode, &newValue, &MinOffsetDefault, &MaxOffsetDefault); //Min, Max value gets updated on calling the function
        IoRegisterHasBeenChanged = TRUE;
        break;

      case CmdVref:
        Mode = GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE;
        if ((Setup->mem.dfxMemSetup.dfxOptions & CR_CMD_FUB_MARGIN) != 0) {
          Mode |= GSM_SKIP_CMD_FUB;
        }
        GetSetCmdVrefCore (Host, Socket, channelInSocket, Dimm, IoLevel, Mode, &newValue);
        IoRegisterHasBeenChanged = TRUE;
        break;

      case CtlGrp0:
      case CtlGrp1:
      case CtlGrp2:
      case CtlGrp3:
      case CtlGrp4:
      case CtlGrp5:
      case CtlAll:
        GetSetCtlGroupDelayCore (Host, Socket, channelInSocket, Dimm, IoLevel, MarginGroup, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &newValue, &MinOffsetDefault, &MaxOffsetDefault); //Min, Max value gets updated on calling the function
        IoRegisterHasBeenChanged = TRUE;
        break;

      case EridDelay:
        // read the current MC cycle delay value and IO logical/PI delay, calculate the new non-offset value
        Clk = GetDdrioClkNumberDdrt (Host, Socket, channelInSocket, Dimm);
        GetSetClkDelay (Host, Socket, channelInSocket, Clk, GSM_READ_ONLY, &DataByte);
        GetSetSxpGnt2Erid (Host, Socket, channelInSocket, Dimm, GSM_READ_ONLY, &newValue);
        totalValue = (newValue * 128) + DataByte + NewOffset - CurrentOffset;
        newValue = totalValue / 128;
        GetSetSxpGnt2Erid (Host, Socket, channelInSocket, Dimm, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &newValue);
        DataByte = totalValue % 128;
        GetSetClkDelay (Host, Socket, channelInSocket, Clk, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &DataByte);
        IoRegisterHasBeenChanged = TRUE;
        break;

      case EridVref:
        GetSetEridVref(Host, Socket, channelInSocket, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &newValue);
        IoRegisterHasBeenChanged = TRUE;
        break;

      default:
        SsaStatus = UnsupportedValue;
        break;
    }//MarginGroup

    SetDebugLevel (PrevDebugLevel);

    // need to reset IO
    if (IoRegisterHasBeenChanged == TRUE) {
     BiosIoReset(This, Socket, Controller);
    }
  }//if (SsaStatus == Success)

  RcDebugPrint (SDBG_BSSA,"End: BiosSetMarginParamOffset\n");
  return SsaStatus;
} //BiosSetMarginParamOffset

/**
  Function used to set the ZQCal configuration.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Enable     - Specifies whether ZQCal is enabled.  TRUE enables ZQCal; FALSE disables it.
  @param[out]     PrevEnable - Pointer to where the previous ZQCal enable setting will be stored.  This value may be NULL.

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetZQCalConfig) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  BOOLEAN               Enable,
  BOOLEAN               *PrevEnable
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetZQCalConfig (Socket:%d, Controller:%d, Enable:%d)\n", Socket, Controller, Enable);

  BiosSetZQCalConfigChip (Host, Socket, Controller, Enable, PrevEnable);

  if (PrevEnable != NULL) {
    RcDebugPrint (SDBG_BSSA,"End: BiosSetZQCalConfig:Previous Enable = 0x%x\n", *PrevEnable);
  } else {
    RcDebugPrint (SDBG_BSSA,"End: BiosSetZQCalConfig:Previous Enable is Not Available\n");
  }
}

/**
  Function used to set the RComp update configuration.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Enable     - Specifies whether RComp updates are enabled.  TRUE enables RComp updates; FALSE disables them.
  @param[out]     PrevEnable - Pointer to where the previous RComp update enable setting will be stored.  This value may be NULL.

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetRCompUpdateConfig) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  BOOLEAN               Enable,
  BOOLEAN               *PrevEnable
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetRCompUpdateConfig (Socket:%d, Controller:%d, Enable:%d)\n", Socket, Controller, Enable);

  if (PrevEnable != NULL) {
    RcDebugPrint (SDBG_BSSA,"End: BiosSetRCompUpdateConfig:Previous Enable = 0x%x\n", *PrevEnable);
  } else {
    RcDebugPrint (SDBG_BSSA,"End: BiosSetRCompUpdateConfig:Previous Enable is Not Available\n");
  }
}

/**
  Function used to set the page-open configuration.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket     - Zero based CPU socket number.
  @param[in]      Controller - Zero based controller number.
  @param[in]      Enable     - Specifies whether page-open is enabled.  TRUE enables page-open; FALSE disables it.
  @param[out]     PrevEnable - Pointer to where the previous page-open enable setting will be stored.  This value may be NULL.

  @retval Nothing.
**/
VOID
(EFIAPI BiosSetPageOpenConfig) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  BOOLEAN               Enable,
  BOOLEAN               *PrevEnable
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosSetPageOpenConfig (Socket:%d, Controller:%d, Enable:%d)\n", Socket, Controller, Enable);

  BiosSetPageOpenConfigChip (Host, Socket, Controller, Enable, PrevEnable);

  if (PrevEnable != NULL) {
    RcDebugPrint (SDBG_BSSA,"End: BiosSetPageOpenConfig:Previous Enable = 0x%x\n", *PrevEnable);
  } else {
    RcDebugPrint (SDBG_BSSA,"End: BiosSetPageOpenConfig:Previous Enable is Not Available\n");
  }
}

/**
  Function used to clear all memory.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.

  @retval NotAvailable if function is not supported by the BIOS.  Else Success.
**/
SSA_STATUS
(EFIAPI BiosScrubMemory) (
  SSA_BIOS_SERVICES_PPI *This
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer (); // wipeMemory -> Write 0 to all memory
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosScrubMemory ()\n");

  if (0 != DdrMemInit(Host)) { //If successfull MemInit() return 0 (Success = 0)
    return Failure;
  }

  RcDebugPrint (SDBG_BSSA,"End: BiosScrubMemory\n");
  return Success;
}

/**
  Function used to initialize CPGC engine(s) for all channels of a given memory
  controller.  Prior to configuring/using the CPGC engine, this function should
  be called with the SetupCleanup input parameter set to Setup.  When finished
  with the CPGC engine, this function should be called with the SetupCleanup
  input parameter set to Cleanup.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Socket       - Zero based CPU socket number.
  @param[in]      Controller   - Zero based controller number.
  @param[in]      SetupCleanup - Specifies setup or cleanup action.

  @retval Nothing.
**/
VOID
(EFIAPI BiosInitCpgc) (
  SSA_BIOS_SERVICES_PPI *This,
  UINT8                 Socket,
  UINT8                 Controller,
  SETUP_CLEANUP         SetupCleanup
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  RcDebugPrint (SDBG_BSSA,"\nStart: BiosInitCpgc (Socket:%d, Controller:%d, SetupCleanup:%d)\n", Socket, Controller, SetupCleanup);

  BiosInitCpgcChip (Host, Socket, Controller, SetupCleanup);

  RcDebugPrint (SDBG_BSSA,"End: BiosInitCpgc\n");
}
// end file BiosSsaMemoryConfig.c

