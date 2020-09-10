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

#include <Memory/JedecDefinitions.h>
#include <Memory/Ddr5Cmds.h>
#include <Library/MemoryCoreLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>

#if defined (DDR5_SUPPORT)

#define MAX_DRAM_RON_NUMBER 3
UINT16 DramRonMapping [MAX_DRAM_RON_NUMBER][2] = {
   {DDR5_DRAM_RON_34_BITS, DDR_RON_34},
   {DDR5_DRAM_RON_40_BITS, DDR_RON_40},
   {DDR5_DRAM_RON_48_BITS, DDR_RON_48},
};

#define MAX_DB_HOST_RON_NUMBER 2
UINT16 DbHostRonMapping [MAX_DB_HOST_RON_NUMBER][2] = {
   {DDR5_DB_HOST_RON_34_BITS, DDR_RON_34},
   {DDR5_DB_HOST_RON_48_BITS, DDR_RON_48},
};

#define MAX_DQ_DQS_RTT_NUMBER 8
UINT16 DqDqsRttMapping [MAX_DQ_DQS_RTT_NUMBER][2] = {
  {DDR5_RTT_NA , DDR5_RTT_NA_OHMS },
  {DDR5_RTT_240, DDR5_RTT_240_OHMS},
  {DDR5_RTT_120, DDR5_RTT_120_OHMS},
  {DDR5_RTT_80 , DDR5_RTT_80_OHMS },
  {DDR5_RTT_60 , DDR5_RTT_60_OHMS },
  {DDR5_RTT_48 , DDR5_RTT_48_OHMS },
  {DDR5_RTT_40 , DDR5_RTT_40_OHMS },
  {DDR5_RTT_34 , DDR5_RTT_34_OHMS }
};

#define MAX_CK_CS_CA_RTT_NUMBER 7
UINT16 CkCsCaRttMapping [MAX_CK_CS_CA_RTT_NUMBER][2] = {
  {DDR5_RTT_CK_CS_CA_RTT_OFF_BITS, DDR5_RTT_CK_CS_CA_RTT_OFF_OHMS},
  {DDR5_RTT_CK_CS_CA_RTT_480_BITS, DDR5_RTT_CK_CS_CA_RTT_480_OHMS},
  {DDR5_RTT_CK_CS_CA_RTT_240_BITS, DDR5_RTT_CK_CS_CA_RTT_240_OHMS},
  {DDR5_RTT_CK_CS_CA_RTT_120_BITS, DDR5_RTT_CK_CS_CA_RTT_120_OHMS},
  {DDR5_RTT_CK_CS_CA_RTT_80_BITS , DDR5_RTT_CK_CS_CA_RTT_80_OHMS},
  {DDR5_RTT_CK_CS_CA_RTT_60_BITS , DDR5_RTT_CK_CS_CA_RTT_60_OHMS},
  {DDR5_RTT_CK_CS_CA_RTT_40_BITS , DDR5_RTT_CK_CS_CA_RTT_40_OHMS}
};


BOOLEAN
IsRttRonValid (
  IN MRC_GT    Group,
  IN UINT16    EncodeValue
  )
{
  UINT8  Index = 0;

  switch (Group) {
    case DramRonPullDown:
    case DramRonPullUp:
      for (Index = 0; Index < MAX_DRAM_RON_NUMBER; Index ++) {
        if (EncodeValue == DramRonMapping [Index][0]) {
          return TRUE;
        }
      }
      return FALSE;
      break;

    case RttWr:
    case RttPark:
    case RttNomWr:
    case RttNomRd:
    case DqsRttPark:
      for (Index = 0; Index < MAX_DQ_DQS_RTT_NUMBER; Index ++) {
        if (EncodeValue == DqDqsRttMapping [Index][0]) {
          return TRUE;
        }
      }
      return FALSE;
      break;
    //
    // These is no clear request to handle these signals at current stage. This is a place holder.
    //
    case RttCkGroupA:
    case RttCkGroupB:
    case RttCsGroupA:
    case RttCsGroupB:
    case RttCaGroupA:
    case RttCaGroupB:
      for (Index = 0; Index < MAX_CK_CS_CA_RTT_NUMBER; Index ++) {
        if (EncodeValue == CkCsCaRttMapping [Index][0]) {
          return TRUE;
        }
      }
      return FALSE;
      break;
    default:
      break;
  }

  return FALSE;
}

/**

  Convert ODT MR register bits to readable ohms or vice verse.

  @param[in]   Group       - CSM_GT signal group to program.
  @param[in]   Value       - Decoded RTT value in unit of omhs.
  @param[in]   Operation   - Encode or Decode RTT value.

  @retval The coverted value.
*/
UINT16
EFIAPI
ConvertOdtValueDdr5 (
  IN MRC_GT    Group,
  IN UINT16    Value,
  IN UINT8     Operation
  )
{
  UINT8  Index = 0;

  switch (Group) {
    case DramRonPullDown:
    case DramRonPullUp:
      for (Index = 0; Index < MAX_DRAM_RON_NUMBER; Index ++) {
        if (Operation == DECODE_RTT) {
          if (Value == DramRonMapping [Index][0]) {
            return DramRonMapping [Index][1];
          }
        } else {
          if (Value == DramRonMapping [Index][1]) {
            return DramRonMapping [Index][0];
          }
        }
      }
      break;

    case RttWr:
    case RttNomWr:
    case RttNomRd:
    case DqsRttPark:
      for (Index = 0; Index < MAX_DQ_DQS_RTT_NUMBER; Index ++) {
        if (Operation == DECODE_RTT) {
          if (Value == DqDqsRttMapping [Index][0]) {
            return DqDqsRttMapping [Index][1];
          }
        } else {
          if (Value == DqDqsRttMapping [Index][1]) {
            return DqDqsRttMapping [Index][0];
          }
        }
      }
      break;

    case RttCkGroupA:
    case RttCkGroupB:
    case RttCsGroupA:
    case RttCsGroupB:
    case RttCaGroupA:
    case RttCaGroupB:
      for (Index = 0; Index < MAX_CK_CS_CA_RTT_NUMBER; Index ++) {
        if (Operation == DECODE_RTT) {
          if (Value == CkCsCaRttMapping [Index][0]) {
            return CkCsCaRttMapping [Index][1];
          }
        } else {
          if (Value == CkCsCaRttMapping [Index][1]) {
            return CkCsCaRttMapping [Index][0];
          }
        }
      }
      break;

    case DbHostRonPullUp:
    case DbHostRonPullDown:
      for (Index = 0; Index < MAX_DB_HOST_RON_NUMBER; Index ++) {
        if (Operation == DECODE_RTT) {
          if (Value == DbHostRonMapping [Index][0]) {
            return DbHostRonMapping [Index][1];
          }
        } else {
          if (Value == DbHostRonMapping [Index][1]) {
            return DbHostRonMapping [Index][0];
          }
        }
      }
      break;

    default:
      break;
  }

  return 0;
} // ConvertOdtValueDdr5


/**
  Read or update multiple RTT RON values from/to the MRC cache

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number
  @param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in][out] Value   - Encoded RTT value for Mode Registers.

  @retval MRC_STATUS_SUCCESS    The function executes sucessfully.
  @retval MRC_STATUS_GSM_LIMIT  There is invalid parameter.

*/

MRC_STATUS
EFIAPI
GetSetDimmRttRonDdr5 (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
  UINT8                RttEnCode = 0x00; // The RTT in bit formate for MR register.
  struct ddrRank       (*RankList)[MAX_RANK_DIMM];
  struct rankDevice    (*RankStruct)[MAX_RANK_DIMM];
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  DDR5_MODE_REGISTER_5_STRUCT   Mr5;
  DDR5_MODE_REGISTER_34_STRUCT  Mr34;
  DDR5_MODE_REGISTER_33_STRUCT  Mr33;
  DDR5_MODE_REGISTER_35_STRUCT  Mr35;
#ifdef  LRDIMM_SUPPORT
  DDR5_DATA_BUFFER_RW86_STRUCT  DataBufferRw86;
  DDR5_DATA_BUFFER_RW87_STRUCT  DataBufferRw87;
  DDR5_DATA_BUFFER_RW88_STRUCT  DataBufferRw88;
  DDR5_DATA_BUFFER_RW93_STRUCT  DataBufferRw93;
#endif

  RankList = GetRankNvList(Host, Socket, Ch, Dimm);
  RankStruct = GetRankStruct(Host, Socket, Ch, Dimm);
  ChannelNvList = GetChannelNvList(Host, Socket);

  //
  // Read old RTT value from cache.
  //
#ifdef  LRDIMM_SUPPORT
  if (IsLrdimmPresent (Socket, Ch, Dimm)) {
    //
    // LRDIMM DB Rtt setting is not per-Rank
    //
    if (Rank != 0) {
      return MRC_STATUS_SUCCESS;
    }
    switch (Group) {
    case RttWr:
      RttEnCode = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw87.Bits.dq_rtt_wr;
      break;
    case RttPark:
      RttEnCode = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw87.Bits.dq_rtt_park;
      break;
    case RttNomWr:
      RttEnCode = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw88.Bits.dq_rtt_nom_wr;
    case RttNomRd:
      RttEnCode = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw88.Bits.dq_rtt_nom_rd;
    case DqsRttPark:
      RttEnCode = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw86.Bits.dqs_rtt_park;
      break;
    default:
      break;
    }

  } else
#endif
  {
    ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_5_REG,  &Mr5.Data);
    ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_34_REG, &Mr34.Data);
    ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_33_REG, &Mr33.Data);
    ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_35_REG, &Mr35.Data);

    switch (Group) {
    case DramRonPullDown:
      RttEnCode= Mr5.Bits.pull_down_output_driver_impedance;
      break;
    case DramRonPullUp:
      RttEnCode = Mr5.Bits.pull_up_output_driver_impedance;
      break;
    case RttWr:
      RttEnCode = Mr34.Bits.rtt_wr;
      break;
    case RttPark:
      RttEnCode = Mr34.Bits.rtt_park;
      break;
    case RttNomWr:
      RttEnCode = Mr35.Bits.rtt_nom_wr;
      break;
    case RttNomRd:
      RttEnCode = Mr35.Bits.rtt_nom_rd;
      break;
    case DqsRttPark:
      RttEnCode = Mr33.Bits.dqs_rtt_park;
      break;
    //
    // These is no clear request to handle these signals at current stage. This is a place holder.
    //
    case RttCkGroupA:
      break;
    case RttCkGroupB:
      break;
    case RttCsGroupA:
      break;
    case RttCsGroupB:
      break;
    case RttCaGroupA:
      break;
    case RttCaGroupB:
      break;

    default:
      break;
    }
  }

  if (Mode & GSM_READ_ONLY) {

    //
    // If read only, save the value read into the output parameter.
    //
    *Value = RttEnCode;
  } else {

    if ((*Value == RttEnCode) && ((Mode & GSM_FORCE_WRITE) == FALSE)) {
      return MRC_STATUS_SUCCESS;
    }

    //
    // Write RTT.
    //

    if (Mode & GSM_WRITE_OFFSET) {

      return MRC_STATUS_GSM_LIMIT;

    } else {

      //
      // Write absolute value.
      //

      RttEnCode = (UINT8)(*Value);
    }

    //
    // Check if RTT or Ron is valid.
    //
    if (!(IsRttRonValid(Group, RttEnCode))) {
      return MRC_STATUS_GSM_LIMIT;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode.
    //

#ifdef  LRDIMM_SUPPORT
    if (IsLrdimmPresent (Socket, Ch, Dimm)) {
      //
      // Select all data buffers.
      //
      DataBufferRw93.Data = 0;
      DataBufferRw93.Bits.pba_select_id = PBA_SELECT_ALL;
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBufferRw93.Data);

      switch (Group) {
        case RttWr:
          DataBufferRw87.Data = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw87.Data;
          DataBufferRw87.Bits.dq_rtt_wr = RttEnCode;
          DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW87_REG, DataBufferRw87.Data);
          break;

        case RttPark:
          DataBufferRw87.Data = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw87.Data;
          DataBufferRw87.Bits.dq_rtt_park = RttEnCode;
          DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW87_REG, DataBufferRw87.Data);
          break;

        case RttNomWr:
          DataBufferRw88.Data = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw88.Data;
          DataBufferRw88.Bits.dq_rtt_nom_wr = RttEnCode;
          DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW88_REG, DataBufferRw88.Data);
          break;

        case RttNomRd:
          DataBufferRw88.Data = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw88.Data;
          DataBufferRw88.Bits.dq_rtt_nom_rd = RttEnCode;
          DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW88_REG, DataBufferRw88.Data);
          break;

        case DqsRttPark:
          DataBufferRw86.Data = (*ChannelNvList)[Ch].dimmList[Dimm].DbRw86.Data;
          DataBufferRw86.Bits.dqs_rtt_park = RttEnCode;
          DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW86_REG, DataBufferRw86.Data);
          break;
        default:
          break;
      }
    } else
#endif
    {
      switch (Group) {
        case DramRonPullUp:
          Mr5.Bits.pull_up_output_driver_impedance = RttEnCode;
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_5_REG, Mr5.Data);
          break;

        case DramRonPullDown:
          Mr5.Bits.pull_down_output_driver_impedance = RttEnCode;
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_5_REG, Mr5.Data);
          break;

        case RttWr:
          Mr34.Bits.rtt_wr = RttEnCode;
          Mr34.Bits.rtt_park = 0; // Read-only bits.
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_34_REG, Mr34.Data);
          break;

        case RttPark:
          //
          // MR34.RTT_PARK is programmed via an explicit MPC command only.
          //
          WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, (RTT_PARK | RttEnCode), TIMING_4N, TRUE);
          break;

        case RttNomWr:
          Mr35.Bits.rtt_nom_wr = RttEnCode;
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_35_REG, Mr35.Data);
          break;

        case RttNomRd:
          Mr35.Bits.rtt_nom_rd = RttEnCode;
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_35_REG, Mr35.Data);
          break;

        case DqsRttPark:
          //
          // MR33.DQS_RTT_PARK is programmed via an explicit MPC command only.
          // Keep the MR33 cache updated
          //
          Mr33.Bits.dqs_rtt_park = RttEnCode;
          WriteDramModeRegCachePerRank (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_33_REG, Mr33.Data);
          WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, (DQS_RTT_PARK | RttEnCode), TIMING_4N, TRUE);
          break;

        //
        // These is no clear request to handle these signals at current stage. This is a place holder.
        //
        case RttCkGroupA:
          break;
        case RttCkGroupB:
          break;
        case RttCsGroupA:
          break;
        case RttCsGroupB:
          break;
        case RttCaGroupA:
          break;
        case RttCaGroupB:
          break;
        default:
          break;
      }
    }
   }

  //
  // Save to cache.
  //
  if (Mode & GSM_UPDATE_CACHE) {
#ifdef  LRDIMM_SUPPORT
    if (IsLrdimmPresent (Socket, Ch, Dimm)) {
      switch (Group) {
        case RttWr:
         (*ChannelNvList)[Ch].dimmList[Dimm].DbRw87.Bits.dq_rtt_wr = RttEnCode;
          break;

        case RttPark:
          (*ChannelNvList)[Ch].dimmList[Dimm].DbRw87.Bits.dq_rtt_park = RttEnCode;
          break;

        case RttNomWr:
          (*ChannelNvList)[Ch].dimmList[Dimm].DbRw88.Bits.dq_rtt_nom_wr = RttEnCode;
          break;

        case RttNomRd:
          (*ChannelNvList)[Ch].dimmList[Dimm].DbRw88.Bits.dq_rtt_nom_rd = RttEnCode;
          break;

        case DqsRttPark:
          (*ChannelNvList)[Ch].dimmList[Dimm].DbRw86.Bits.dqs_rtt_park = RttEnCode;
          break;

        default:
          break;
      }
    } else
#endif
    {
      switch (Group) {

      //
      // These is no request to cache these signals at current stage. This is a place holder.
      //
      case RttCkGroupA:
        break;
      case RttCkGroupB:
        break;
      case RttCsGroupA:
        break;
      case RttCsGroupB:
        break;
      case RttCaGroupA:
        break;
      case RttCaGroupB:
        break;

      //
      // DramModeRegWrite always updates the cache so nothing to do here
      //
      case DramRonPullUp:
      case DramRonPullDown:
      case RttWr:
      case RttPark:
      case RttNomWr:
      case RttNomRd:
      case DqsRttPark:
      default:
        break;
      }
    }
  }
  return MRC_STATUS_SUCCESS;
}
#endif

