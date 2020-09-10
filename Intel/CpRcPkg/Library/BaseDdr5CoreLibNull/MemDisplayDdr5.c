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

#include "Ddr5Core.h"

/**
Displays the bitwise error status

@param Host:         Pointer to sysHost
@param socket:       Processor socket
@param ch:           Memory Channel
@param SubCh:        Memory Sub Channel
@param dimm:         Dimm Number
@param rank:         Rank Number
@param bwSerr[3]     BitWise error status
@param margin:       margin

@retval N/A

**/
VOID
DisplayBwSerrDdr5 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     SubCh,
  UINT8     dimm,
  UINT8     rank,
  UINT32    bwSerr[3],
  INT16     margin
  )
{
} // DisplayBwSerrDdr5

    /*++
    Displays the training results

    @param Host:         Pointer to sysHost
    @param socket:       Processor socket
    @param group:        MRC_GT group

    @retval N/A

    --*/
VOID
EFIAPI
DisplayResultsDdr5 (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group
  )
{
} // DisplayResultsDdr5

/**

Displays the DDR5 CWL_ADJ results

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param socket  - Socket Id
@param mode    - Program mode.
@param group   - MRC_GT group

@retval None

**/
VOID
DisplayCwlAdjDdr5(
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     mode,
  IN INT16     (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH]
  )
{
} // DisplayCwlAdjDdr5

/**

  Displays the DDR5 training results for specific Rank.

  @param[in] Host           - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket         - Socket Id
  @param[in] Ch             - Ch number
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] Mode           - program Mode
  @param[in] DisplayINTCAL  - display internal cycle alignment or not
  @param[in] CwlAdjInput    - cwl_adj value input
  @param[in] Group          - MRC_GT Group

  @retval None

**/
VOID
DisplaySpecificRankResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     rank,
  IN UINT8     mode,
  IN BOOLEAN   DisplayINTCAL,
  IN INT16     (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH],
  IN MRC_GT    group
  )
{
} // DisplaySpecificRankResultsDdr5

/**

Displays the DDR5 training results for specific rank.

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param socket  - Socket Id
@param mode    - program mode
@param group   - MRC_GT group

@retval None

**/
VOID
DisplayTxDqsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     mode,
  IN MRC_GT    group
  )
{
} // DisplayTxDqsDdr5

/**

Displays the DDR5 Internal Cycle Alignment training results .

@param Host                         - Pointer to sysHost, the system Host (root) structure struct
@param socket                       - Socket Id
@param ch                           - ch number
@param dimm                         - dimm number
@param rank                         - rank number
@param mode                         - program mode
@param InternalCycleAlignmentInput  - Internal cycle alingment value input
@param group                        - MRC_GT group

@retval None

**/
VOID
DisplayInternalCycleAlingmentDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode,
  IN INT16     InternalCycleAlignment [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  IN MRC_GT    Group
  )
{
} // DisplaySpecificRankResultsDdr5

/**

  Return a pointer to a signal name string

  @param[in]  SignalName    - The signal name

  @retval Pointer to the string

**/
CHAR8*
GetSignalStrDdr5 (
  IN  GSM_CSN   SignalName
  )
{
  return NULL;
}

/**
  Displays the training results for all parameters

  @param[in] Host:         Pointer to sysHost

  @retval N/A
**/
UINT32
DisplayTrainResultsDdr5 (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
}


/**

  Displays Advanced Centering data

  @param[in] Host:              Pointer to sysHost
  @param[in] Socket:            Processor socket
  @param[in] Ch                 Channel number
  @param[in] Sub-Ch             SubChannel number
  @param[in] Dimm               DIMM number
  @param[in] Rank               Rank number
  @param[in] Group              Parameter to center
  @param[in] Results            Pointer to the structure to store the margin results
  @param[in] Delay              Pointer to the delay value
  @param[in] OldDelay           Pointer to the old delay value
  @param[in] MaxVref            Max vref
  @param[in] StepSize           Setp size
  @param[in] NumPoints          Num points

  @retval N/A

**/
VOID
DisplayAdvancedCenteringResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Subchannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_GT    Group,
  IN VOID      *Results,
  IN INT16     *Delay,
  IN INT16     *OldDelay,
  IN UINT8     MaxVref,
  IN UINT8     StepSize,
  IN UINT8     NumPoints
  )
{
} //DisplayAdvancedCenteringResultsDdr5

/**

  Displays And Adjust Vref Centering data

  @param[in] Host            Pointer to sysHost
  @param[in] Socket          Processor socket
  @param[in] Group           Parameter to group
  @param[in] BitResults      Pointer to the bit results
  @param[in] StrobeResults   Pointer to the strobe results
  @param[in] PerBit          Vref training mode perbit

  @retval N/A

**/
VOID
DisplayAndAdjustVrefCenteringResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    Group,
  IN VOID      *BitResults,
  IN VOID      *StrobeResults,
  IN UINT8     PerBit
  )
{
} //DisplayAndAdjustVrefCenteringResultsDdr5
