/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation. <BR>

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

#include "MemTestFunc.h"
#include <ReferenceCodeFatalErrors.h>


//
// Code section
//

/**

  Get status whether PPR resource is available

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket
  @param Ch                 - Channel number
  @param Dimm               - Dimm number
  @param Rank               - Rank number
  @param SubRank            - SubRank number
  @param Nibble             - Nibble number used to identify DRAM
  @param BankGroup          - Bank Group number

  @retval TRUE if PPR resource available; FALSE if not available

**/
BOOLEAN
GetPprResourceAvailable (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     SubRank,
  UINT8     Nibble,
  UINT8     BankGroup
  )
{
  UINT8 RankOnCh;
  UINT8 Dram;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  //
  // Assert that array indices do not go out of bounds
  //
  RC_FATAL_ERROR (Ch < MAX_CH, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);
  RC_FATAL_ERROR (Nibble < MAX_STROBE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);

  ChannelNvList = GetChannelNvList (Host, Socket);
  if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
    RankOnCh = (Dimm * MAX_RANK_DIMM_3DS) + (Rank * MAX_SUBRANK_3DS) + SubRank;
  } else {
    RankOnCh = (Dimm * MAX_RANK_DIMM) + Rank;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if ((*DimmNvList)[Dimm].x4Present) {
    Dram = Nibble;
  } else {
    Dram = Nibble / 2;
  }
  if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
    Dram = Nibble / 4;
  }

  //
  // Assert that array indices do not go out of bounds
  //
  RC_FATAL_ERROR (RankOnCh < (MAX_DIMM * MAX_RANK_DIMM_3DS * MAX_SUBRANK_3DS), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);

  //
  // Resource not available when bit is set
  //
  if (Host->var.mem.PprResourceAvailable[Ch][RankOnCh][Dram] & (1 << BankGroup)) {

    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "GetPprResourceAvailable: Warning - Dram %d, Subrank %d, BankGroup %d PPR resource not available.\n", Dram, SubRank, BankGroup);
    return FALSE;
  }
  return TRUE;
} // GetPprResourceAvailable


/**

  Set status whether PPR resource is available

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket
  @param Ch                 - Channel number
  @param Dimm               - Dimm number
  @param Rank               - Rank number
  @param SubRank            - SubRank number
  @param Nibble             - Nibble number used to identify DRAM
  @param BankGroup          - Bank Group number
  @param ResourceAvailable  - TRUE/FALSE whether PPR resource available

  @retval n/a

**/
VOID
SetPprResourceAvailable (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     SubRank,
  UINT8     Nibble,
  UINT8     BankGroup,
  BOOLEAN   ResourceAvailable
)
{
  UINT8 RankOnCh;
  UINT8 Dram;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

    //
  // Assert that array indices do not go out of bounds
  //
  RC_FATAL_ERROR (Ch < MAX_CH, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);
  RC_FATAL_ERROR (Nibble < MAX_STROBE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);

  ChannelNvList = GetChannelNvList (Host, Socket);
  if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
    RankOnCh = (Dimm * MAX_RANK_DIMM_3DS) + (Rank * MAX_SUBRANK_3DS) + SubRank;
  } else {
    RankOnCh = (Dimm * MAX_RANK_DIMM) + Rank;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if ((*DimmNvList)[Dimm].x4Present) {
    Dram = Nibble;
  } else {
    Dram = Nibble / 2;
  }
  if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
    Dram = Nibble / 4;
  }

  //
  // Assert that array indices do not go out of bounds
  //
  RC_FATAL_ERROR (RankOnCh < (MAX_DIMM * MAX_RANK_DIMM_3DS * MAX_SUBRANK_3DS), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);

  if (ResourceAvailable) {
    Host->var.mem.PprResourceAvailable[Ch][RankOnCh][Dram] &= ~(1 << BankGroup);
  } else {
    Host->var.mem.PprResourceAvailable[Ch][RankOnCh][Dram] |= (1 << BankGroup);
  }
} // SetPprResourceAvailable


/**

  Get a pointer to Row fail range entry at the given list index

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number
  @param FailIndex          - index into the Row fail list

  @retval Pointer to Row fail range entry

**/
ROW_FAIL_RANGE *
GetRowFailRange (
  PSYSHOST  Host,
  UINT8     ch,
  INT32     FailIndex
  )
{
  return &Host->var.mem.FailRange[ch][FailIndex];
}

/**

  Get the number of entries in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number

  @retval Number of entries in the Row fail list

**/
UINT32
GetRowFailMax (
  PSYSHOST  Host,
  UINT8     ch
  )
{
  return Host->var.mem.FailMax[ch];
}

/**

  Set the number of entries in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number
  @param FailMax            - Number of entries in Row fail list

  @retval   None

**/
VOID
SetRowFailMax (
  PSYSHOST  Host,
  UINT8     ch,
  UINT32    FailMax
  )
{
  Host->var.mem.FailMax[ch] = FailMax;
}

/**

  Get the current index in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number

  @retval Current row fail range index

**/
UINT32
GetRowFailIndex (
  PSYSHOST  Host,
  UINT8     ch
  )
{
  return Host->var.mem.FailIndex[ch];
}

/**

  Set the current index in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number
  @param FailIndex          - Current row fail range index

  @retval   None

**/
VOID
SetRowFailIndex (
  PSYSHOST  Host,
  UINT8     ch,
  UINT32    FailIndex
  )
{
  Host->var.mem.FailIndex[ch] = FailIndex;
}

/**

  Get the current retry state

  @param Host               - Pointer to sysHost, the system Host (root) structure

  @retval Current retry state

**/
UINT8
GetAmtRetryState (
  PSYSHOST  Host
  )
{
  return Host->var.mem.RetryState;
}

/**

  Set the current retry state

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param RetryCount         - AMT retry state

  @retval n/a

**/
VOID
SetAmtRetryState (
  PSYSHOST  Host,
  UINT8     RetryState
)
{
  Host->var.mem.RetryState = RetryState;
}

/**

  Get the last failure info

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Ch                 - Channel number
  @param *LastFail          - Structure containing info about last test failure

  @retval n/a

**/
VOID
GetAmtLastFail (
  PSYSHOST  Host,
  UINT8     Ch,
  ROW_FAIL_RANGE *LastFail
)
{
  *LastFail = Host->var.mem.LastFail[Ch];
}

/**

  set the last failure info

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Ch                 - Channel number
  @param LastFail           - Structure containing info about last test failure

  @retval n/a

**/
VOID
SetAmtLastFail (
  PSYSHOST  Host,
  UINT8     Ch,
  ROW_FAIL_RANGE LastFail
)
{
  Host->var.mem.LastFail[Ch] = LastFail;
}

/**

  Get the Error injection mask for Advanced Memtest pattern

  @param Host               - Pointer to sysHost, the system Host (root) structure

  @retval  Bitmask of DQ lanes to inject error

**/
UINT64_STRUCT
GetAdvMemtestErrInjMask (
  PSYSHOST  Host
  )
{
  return Host->var.mem.AdvMemtestErrInjMask;
}

/**

  Set the Error injection mask for Advanced Memtest pattern

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param AdvMemtestErrInjMask - Bitmask of DQ lanes to inject error

  @retval   None

**/
VOID
SetAdvMemtestErrInjMask (
  PSYSHOST  Host,
  UINT64_STRUCT AdvMemtestErrInjMask
  )
{
  Host->var.mem.AdvMemtestErrInjMask = AdvMemtestErrInjMask;
}

/**

  Returns whether memory error injection is enabled

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param Ch     - Ddr channel within the socket

  @retval 1 = Error injection enabled; 0 = disabled

**/UINT8
GetInjectErrorMATS (
  PSYSHOST  Host,
  UINT8     Ch
  )
{
  return Host->var.mem.InjectErrorMATS[Ch];
}

/**

  Set the Error injection flag for Advanced Memtest operation

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param InjectErrorMATS    - Flag to enable/disable error injection

  @retval   None

**/
VOID
SetInjectErrorMATS (
  PSYSHOST  Host,
  UINT8     Ch,
  UINT8     InjectErrorMATS
  )
{
  Host->var.mem.InjectErrorMATS[Ch] = InjectErrorMATS;
}

/**
  This function checks if the DDR memory initialization with all 0's pattern is done.

  @retval TRUE        DDR memory initialization is done.
  @retval FALSE       DDR memory initialization is not done.
**/
BOOLEAN
IsDdrMemInitDone (
  VOID
  )
{
  PSYSHOST   Host;

  Host = GetSysHostPointer ();
  return Host->var.mem.IsDdrMemInitDone;
}

/**

  Initializes data structures for the AMT feature

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Socket             - Socket Id

  @retval   None

**/
VOID
InitAmtDataStructures (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  if (Host->var.mem.FailRangeInitDone[Socket] == 0) {
    ZeroMem (Host->var.mem.FailRange, sizeof (Host->var.mem.FailRange));
    ZeroMem (Host->var.mem.FailMax, sizeof (Host->var.mem.FailMax));
    ZeroMem (Host->var.mem.FailIndex, sizeof (Host->var.mem.FailIndex));
    ZeroMem (Host->var.mem.LastFail, sizeof (Host->var.mem.LastFail));
    ZeroMem (Host->var.mem.InjectErrorMATS, sizeof (Host->var.mem.InjectErrorMATS));
    ZeroMem (&Host->var.mem.RetryState, sizeof (Host->var.mem.RetryState));
    ZeroMem (&Host->var.mem.AdvMemtestErrInjMask, sizeof (Host->var.mem.AdvMemtestErrInjMask));
    ZeroMem (&Host->var.mem.AmtErrInjDone, sizeof (Host->var.mem.AmtErrInjDone));
    ZeroMem (&Host->var.mem.PprResourceAvailable, sizeof (Host->var.mem.PprResourceAvailable));
    Host->var.mem.FailRangeInitDone[Socket]++;
  }
}
