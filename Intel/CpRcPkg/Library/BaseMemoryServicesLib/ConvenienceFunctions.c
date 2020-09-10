/** @file
  ConvenienceFunctions.c

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

#include <Uefi.h>
#include <SysHost.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemRcLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/SpdDecodeLib.h>

/**
  Determine whether a channel is enabled.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check.

  @retval TRUE        The channel is enabled.
  @retval FALSE       The channel is not enabled.

**/

BOOLEAN
EFIAPI
IsChannelEnabled (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  PSYSHOST  Host;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList)[Channel].enabled == 0) {
    return FALSE;
  }

  return TRUE;

} // IsChannelEnabled

/**
  Determine whether a Dimm is present.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check on.
  @param[in]  Dimm    The dimm to check.

  @retval TRUE        The dimm is present.
  @retval FALSE       The dimm is not present.

**/

BOOLEAN
EFIAPI
IsDimmPresent (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  PSYSHOST  Host;
  DIMM_NVRAM_STRUCT (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if ((*DimmNvList)[Dimm].dimmPresent == 0) {
    return FALSE;
  }

  return TRUE;

} // IsDimmPresent

/**
  Determine whether a rank is present.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check on.
  @param[in]  Dimm    The dimm to check.
  @param[in]  Rank    The rank to check.

  @retval TRUE        The rank is present.
  @retval FALSE       The rank is not present.

**/

BOOLEAN
EFIAPI
IsRankPresent (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 Rank
  )
{
  PSYSHOST  Host;
  DDR_RANK_STRUCT   (*RankList)[MAX_RANK_DIMM];

  Host = GetSysHostPointer ();

  RankList = GetRankNvList (Host, Socket, Channel, Dimm);

  if ((*RankList)[Rank].enabled == 0) {
    return FALSE;
  }

  return TRUE;

} // IsRankPresent

/**
  Determine whether a Data Center Persistent
  Memory Module is present in the given DIMM slot

  @param[in]  Socket  Socket ID to check
  @param[in]  Channel Channel ID to check
  @param[in]  Dimm    Slot number to check

  @retval     TRUE    DCPMM DIMM is present
  @retval     FALSE   DCPMM DIMM is not present
**/
BOOLEAN
EFIAPI
IsDcpmmPresentDimm (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  PSYSHOST  Host;
  DIMM_NVRAM_STRUCT (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
    return FALSE;
  }

  return TRUE;

} // IsDcpmmPresentDimm

/**
  Determine whether a Data Center Persistent
  Memory Module is present in the given channel

  @param[in]  Socket  Socket ID to check
  @param[in]  Channel Channel ID to check

  @retval     TRUE    DCPMM DIMM is present
  @retval     FALSE   DCPMM DIMM is not present
**/
BOOLEAN
EFIAPI
IsDcpmmPresentChannel (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  PSYSHOST            Host;
  struct channelNvram (*ChannelNvList) [MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList)[Channel].enabled == 0) {
    return FALSE;
  }

  if ((*ChannelNvList)[Channel].ddrtEnabled == 1) {
    return TRUE;
  }

  return FALSE;
} // IsDcpmmPresentChannel

/**
  Determine whether a Data Center Persistent
  Memory Module is present in the given socket

  @param[in]  Socket  Socket ID to check

  @retval     TRUE    DCPMM DIMM is present
  @retval     FALSE   DCPMM DIMM is not present
**/
BOOLEAN
EFIAPI
IsDcpmmPresentSocket (
  IN UINT8 Socket
  )
{
  UINT8   Channel;

  if (IsSocketEnabled (Socket) != TRUE) {
    return FALSE;
  }

  for (Channel = 0; Channel < MAX_CH; Channel++) {
    if (IsDcpmmPresentChannel (Socket, Channel)) {
      return TRUE;
    }
  }

  return FALSE;
} // IsDcpmmPresentChannel

/**
  Return the number of DDR dimms present on a channel excluding
  Data Center Persistent Memory Modules.

  @param[in]  Socket  The socket to check on.
  @param[in]  Channel The channel to check on.

  @retval UINT8 Ddr Dimms Per Channel

**/

UINT8
EFIAPI
DdrDimmsPerChannel (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  UINT8 Dimm;
  UINT8 DdrDPC;

  DdrDPC = 0;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((IsDimmPresent (Socket, Channel, Dimm)) && (!(IsDcpmmPresentDimm (Socket, Channel, Dimm)))) {
      DdrDPC++;
    }
  }

  return DdrDPC;

} // DdrDimmsPerChannel


/**

  Determines whether the specified memory controller is populated

  @param[in]  Socket  The socket to check on
  @param[in]  McId    The memory controller to check on

  @retval TRUE        At least one DIMM is present on the memory controller
  @retval FALSE       No DIMMs are present on the memory controller

**/
BOOLEAN
EFIAPI
IsMcPopulated (
  IN UINT8 Socket,
  IN UINT8 McId
  )
{
  UINT8 McChannel;
  UINT8 SocketChannel;
  UINT8 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  for (McChannel = 0; McChannel < NumChPerMc; McChannel++) {
    SocketChannel = (McId * NumChPerMc) + McChannel;

    if (IsChannelEnabled (Socket, SocketChannel)) {
      return TRUE;
    }
  }

  return FALSE;
} // IsMcPopulated

/**
  Get Last posted Major checkpoint value
  Gets value from SysHost variable Host->var.mem.lastCheckpoint

  @param[in] Socket - 0 based Socket number

  @retval UINT8 - Major checkpoint
**/
UINT8
EFIAPI
GetLastMajorCheckPoint (
  IN UINT8 Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  return (((Host->var.mem.lastCheckpoint[Socket]) >> 24) & 0x000000FF);
}

/**
  Get Last posted Minor checkpoint value
  Gets value from SysHost variable Host->var.mem.lastCheckpoint

  @param[in] Socket - 0 based Socket number

  @retval UINT8 - Minor checkpoint
**/
UINT8
EFIAPI
GetLastMinorCheckPoint (
  IN UINT8 Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  return (((Host->var.mem.lastCheckpoint[Socket]) >> 16) & 0x000000FF);

}

/**
  This API gets the value for Current socket being initialized

  @param None

  @retval UINT8  The Socket number that is currently initialized.
**/
UINT8
EFIAPI
GetCurrentSocketId (
  VOID
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.currentSocket;
}

/**
  This API Check and returns status of Turnaround Initialization completion

  @param[in] Socket - 0 based Socket number

  @retval TRUE  - Turnaround Init done
  @retval FALSE - Turnaround Init not done
**/
BOOLEAN
EFIAPI
IsTurnAroundInitDone (
  IN UINT8 Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  return Host->var.mem.socket[Socket].TurnaroundInitDone;
}

/**

  Determines if this rank is valid.

  @param[in] Socket           - Socket to check
  @param[in] Channel          - Channel to check
  @param[in] Dimm             - DIMM to check
  @param[in] Rank             - Rank to check
  @param[in] IncludeMappedOut - Whether to include mapped out ranks

  @retval TRUE - Rank is valid
  @retval FALSE - Rank is not valid

**/

BOOLEAN
EFIAPI
IsRankValid (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN BOOLEAN   IncludeMappedOut
  )
{
  PSYSHOST  Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  struct ddrRank    (*RankList)[MAX_RANK_DIMM];

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  RankList = GetRankNvList (Host, Socket, Channel, Dimm);

  if (Rank >= (*DimmNvList) [Dimm].numRanks) {
    return FALSE;
  }

  if (((*RankList)[Rank].enabled == 0) && (GetCurrentTestType (Socket) != EarlyRcdCsLoopbackTest)) {
    return FALSE;
  }

  if ((IncludeMappedOut == TRUE) && (Host->var.mem.checkMappedOutRanks == 0)) {

    if ((*DimmNvList)[Dimm].mapOut[Rank]) {
      return FALSE;
    }

  }

  return TRUE;

} // IsRankValid

/**

  Returns the number of ranks for a dimm.

  @param[in] Socket           - Socket to check
  @param[in] Channel          - Channel to check
  @param[in] Dimm             - DIMM to check

  @retval Number of Ranks

**/

UINT8
EFIAPI
GetNumberOfRanksOnDimm (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  )
{
  PSYSHOST  Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  return (*DimmNvList)[Dimm].numRanks;

} // GetNumberOfRanksOnDimm

/**

  Check if a socket is enabled.

  @param[in] Socket - Socket to check

  @retval Whether socket is enabled or not

**/

BOOLEAN
EFIAPI
IsSocketEnabled (
  IN UINT8 Socket
  )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  if (Host->nvram.mem.socket[Socket].enabled == 0) {
    return FALSE;
  }

  return TRUE;
}

/**

  Check if a dimm is X4.

  @param[in] Socket   - Socket to check
  @param[in] Channel  - Channel to check
  @param[in] Dimm     - DIMM number in the channel

  @retval Whether dimm is X4 or not

**/

BOOLEAN
EFIAPI
IsX4Dimm (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  )
{
  PSYSHOST  Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if ((*DimmNvList)[Dimm].x4Present != 0) {
    return TRUE;
  }

  return FALSE;

} // IsX4Dimm

/**

  Check if a dimm is X16

  @param[in] Socket   - Socket to check
  @param[in] Channel  - Channel to check
  @param[in] Dimm     - DIMM number in the channel

  @retval Whether dimm is X16 or not

**/
BOOLEAN
EFIAPI
IsX16Dimm (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  )
{
  PSYSHOST          Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = (PSYSHOST) GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if ((*DimmNvList)[Dimm].dimmPresent == 0) {
    return FALSE;
  }

  if (((*DimmNvList)[Dimm].dramIOWidth) == SPD_DRAM_IO_WIDTH_X16) {
    return TRUE;
  }

  return FALSE;

} // IsX16Dimm

/**

  Get the number of dimms in a socket.

  @param[in] Socket  - Socket to check

  @retval Number of dimms in a socket.

**/

UINT8
EFIAPI
GetMaxDimmPop (
  IN UINT8     Socket
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();

  return Host->nvram.mem.socket[Socket].maxDimmPop;

} // GetMaxDimmPop


/**

  Get maximum channel number of socket per Memory technology type

  @param[in] MemTechType    - Memory technology type

  @return the maximum channel number of socket

**/
UINT8
EFIAPI
GetMaxChPerTechType (
  IN MEM_TECH_TYPE  MemTechType
  )
{
  if (MemTechType == MemTechHbm) {
    return GetMaxChHbm ();
  } else {
    return GetMaxChDdr ();
  }
}

/**

  Determine whether a channel is enabled per Memory technology type

  @param[in]  MemTechType - Memory technology type
  @param[in]  Socket      - The socket to check on.
  @param[in]  Channel     - The channel to check.

  @retval TRUE        The channel is enabled.
  @retval FALSE       The channel is not enabled.

**/

BOOLEAN
EFIAPI
IsChEnabledPerTechType (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket,
  IN UINT8          Channel
  )
{
  if (MemTechType == MemTechHbm) {
    return IsHbmChannelEnabled (Socket, Channel);
  } else {
    return IsChannelEnabled (Socket, Channel);
  }
}

/**
  Get the rank index for a DIMM rank.

  @param[in] Socket           - Socket to check.
  @param[in] Channel          - Channel to check.
  @param[in] Dimm             - DIMM to check.
  @param[in] Rank             - Rank to check.

  @return   Rank index of the DIMM rank.
**/
UINT8
GetRankIndex (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  PSYSHOST          Host;
  DDR_RANK_STRUCT   (*RankList)[MAX_RANK_DIMM];

  Host = GetSysHostPointer ();
  RankList = GetRankNvList (Host, Socket, Channel, Dimm);

  return (*RankList)[Rank].rankIndex;
}

/**
  Function to check if Both DDR and DDRT present in given channel

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval TRUE  - DDRT and DDR4 both present
          FALSE  - Otherwise

**/
BOOLEAN
EFIAPI
IsDdrAndDdrtPresent (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  if (IsDcpmmPresentChannel (Socket, Ch) && (DdrDimmsPerChannel (Socket, Ch) > 0)) {
    return TRUE;
  }
  return FALSE;
}


/**
  If 3DS dimm present in given Socket\Channel return TRUE
  otherwise return FALSE

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval TRUE  - 3DS present
          FALSE - 3DS not present
**/
BOOLEAN
EFIAPI
Is3dsDimmPresentInChannel (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  PSYSHOST  Host;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  if (((*ChannelNvList)[Channel].encodedCSMode == 2) && IsChannelEnabled (Socket, Channel)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Returns SDRAM Capacity of given dimm

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number
  @param[in] Dimm     - Dimm number

  @retval UINT8 - SDRAM Capacity defines
**/
UINT8
EFIAPI
GetDramCapacity (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  PSYSHOST  Host;
  DIMM_NVRAM_STRUCT (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  return (*DimmNvList)[Dimm].sdramCapacity;
}

/**
  Check if 16GB 3DS dimm present in given channel

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval  TRUE  - 16GB3DS dimm present
           FALSE - 16GB3DS dimm not present

**/
BOOLEAN
EFIAPI
Is16Gb3DsDimmPresent (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  UINT8 Dimm = 0;
  if (Is3dsDimmPresentInChannel (Socket, Channel)) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (GetDramCapacity (Socket, Channel, Dimm) == SPD_16Gb) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
/**
  Get QCS QCA copy number of the DIMM.

  @param[in] Socket           - Socket to check.
  @param[in] Channel          - Channel to check.
  @param[in] Dimm             - DIMM to check.

  @return   Copy number of the DIMM.
**/
UINT8
EFIAPI
GetMaxBacksideCopyNum (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  )
{
  if (IsX4Dimm (Socket, Channel, Dimm) == TRUE) {
    return COPY_NUM;
  }

  return 1;
}

/**
  Function to return Minimum of Non Zero value in given Array

  @param[in] InputValue - An array consisting of input values
  @param[in] InputSize  - Array Size

  @retval Non Zero - Found minimum Value
  @retval Zero     - Array content is 0

**/
UINT32
EFIAPI
GetMinimumOfNonZeroNumberN (
  IN UINT32 InputValue [],
  IN UINT32 InputSize
  )
{
  UINT32  MinimumVal = 0xFFFFFFFF;
  UINT32  i = 0;

  for (i = 0; i < InputSize; i++) {
    if (MinimumVal > InputValue[i] && InputValue[i] != 0) {
      MinimumVal = InputValue[i];
    }
  }
  return MinimumVal;
}

/**

  Determines if CMD signals are normalized or not

  @param[in] Socket    - Current Socket

  @retval TRUE         - CMD signals are normalized
  @retval FALSE        - CMD signals are not normalized

**/
BOOLEAN
EFIAPI
IsCmdNormalized (
  IN UINT8    Socket
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();
  return (Host->nvram.mem.socket[Socket].normalizationFactorEn == 1);
}

/**

  Set NormalizationFactorEn

  @param[in] Socket    - Current Socket
  @param[in] FactorEn  - Normalization Status
                         1 : normalized
                         0 : not normalized

  @retval N/A

**/
VOID
EFIAPI
SetNormalizationFactorEn (
  IN UINT8    Socket,
  IN INT8     FactorEn
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();
  Host->nvram.mem.socket[Socket].normalizationFactorEn = FactorEn;
}

/**

  Converts any raw number (in Two's Complement format) to a signed number

  @param[in]    RawValue          Number in complement's 2 format
  @param[in]    SignBitPosition   Bit position that represents the sign

  @return       Signed number that represents the raw value

**/
INT32
EFIAPI
ConvertTwosComplementToInt (
  IN        UINT32          RawValue,
  IN        UINT8           SignBitPosition
)
{
  INT32   SignedValue;
  UINT32  MagnitudSection;
  //
  // Create a mask to only leave the positive side of the raw value based on the sign bit position
  // for example if sign bit position is 9, this will create a mask
  // (1 << 9) = 0x00000200
  // 0x00000200 - 1 = 0x000001FF
  //
  MagnitudSection = (1 << SignBitPosition) - 1;

  if ((RawValue & (1 << SignBitPosition)) != 0) {
    //
    // Apply complement 2 to get the signed number
    //
    SignedValue = (INT32)~RawValue;
    SignedValue = (INT32)(SignedValue & MagnitudSection);
    SignedValue += 1;
    SignedValue = 0 - SignedValue;

  } else {
    //
    // Only leave the positive part of it
    //
    SignedValue = (INT32)(RawValue & MagnitudSection);
  }
  return SignedValue;
}

/**
   Get Max valid Sub channel mask

  @retval MaxValidSubChMask Bitmap of enabled Subchannel

**/

UINT8
EFIAPI
GetMaxValidSubChannelMask (
  )
{
  return ((1 << SUB_CH) - 1);
} // GetMaxValidSubChannelMask

/**
  Get Host frequency
  @param[in]  Socket        Processor Socket

  @retval Encoded Host frequency
**/
UINT8
EFIAPI
GetHostFrequency (
    IN UINT8    Socket
  )
{
  PSYSHOST Host = GetSysHostPointer ();
#if FixedPcdGetBool(PcdMcrSupport)
  UINT8    McrFreq;

  McrFreq = Host->nvram.mem.socket[Socket].ddrFreq;

  if (IsMcrPresentSocket (Socket)) {
    switch (Host->nvram.mem.socket[Socket].ddrFreq) {
    case DDR_3200:
      McrFreq = DDR_6400;
      break;

    case DDR_3400:
      McrFreq = DDR_6800;
      break;

    case DDR_3600:
      McrFreq = DDR_7200;
      break;

    case DDR_3800:
      McrFreq = DDR_7600;
      break;

    case DDR_4000:
      McrFreq = DDR_8000;
      break;

    case DDR_4200:
      McrFreq = DDR_8400;
      break;

    case DDR_4400:
      McrFreq = DDR_8800;
      break;

    default:
      // invalid / unknown frequency, use safe value of 6400
      McrFreq = DDR_6400;
      break;
    } // switch
  }

  return McrFreq;
#else
  return Host->nvram.mem.socket[Socket].ddrFreq;
#endif // FixedPcdGetBool(PcdMcrSupport)
} // GetHostFrequency


/**
  Determine if a MCR is present in the given Channel

  @param[in]      Socket        Processor Socket
  @param[in]      Channel       Channel

  @retval TRUE if MCR Present
  @retval FALSE if MCR is not present
**/
BOOLEAN
EFIAPI
IsMcrPresentChannel (
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
#if FixedPcdGetBool(PcdMcrSupport)
  struct channelNvram (*ChannelNvList)[MAX_CH];
  PSYSHOST Host = GetSysHostPointer ();

  //
  // MCR_TODO: Need to fix IsMcrPresentChannel() if more than 1 MCR DIMM per CH is required
  //
  ChannelNvList = GetChannelNvList (Host, Socket);
  return ((*ChannelNvList)[Channel].McrDimmPresent != 0);
#else
  return FALSE;
#endif // FixedPcdGetBool(PcdMcrSupport)
} // IsMcrPresentChannel

/**
  Determine if a MCR is present in the given Socket

  @param[in]      Socket        Processor Socket

  @retval TRUE if MCR Present
  @retval FALSE if MCR is not present
**/
BOOLEAN
EFIAPI
IsMcrPresentSocket (
  IN UINT8    Socket
  )
{
#if FixedPcdGetBool(PcdMcrSupport)

  PSYSHOST Host = GetSysHostPointer ();

  return (Host->nvram.mem.socket[Socket].McrDimmPresent != 0);
#else
  return FALSE;
#endif // FixedPcdGetBool(PcdMcrSupport)
} // IsMcrPresentSocket

