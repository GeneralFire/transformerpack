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

  Trains command and address signals relative to clock

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EarlyCaClk (
  PSYSHOST  Host
  )
{
  return SUCCESS;
}

/**

  Trains backside command and address signals relative to clock

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EarlyCaClkBackside (
  PSYSHOST  Host
  )
{
  return SUCCESS;
}

/**

  Program QCA Delay.

  @param Host                  - Pointer to sysHost
  @param Socket                - Socket number
  @param Ch                    - Channel number
  @param Dimm                  - Dimm number
  @param SubCh                 - Subchannel number
  @param Rank                  - Rank number
  @param Signal                - The signal to program
  @param Mode                  - Mode
  @param Delay                 - Delay value
  @param PassThroughMode       - It's in pass through mode or not

  @retval SUCCESS

**/
UINT32
ProgramQcaDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     SubCh,
  IN UINT8     Rank,
  IN GSM_CSN   Signal,
  IN UINT8     Mode,
  IN INT8      Delay,
  IN BOOLEAN   PassThroughMode
  )
{
  return SUCCESS;
}

/**

  Program Final QCA Delay result.

  @param[in] Host                  - Pointer to sysHost
  @param[in] Socket                - Socket number
  @param[in] Ch                    - Channel number
  @param[in] Dimm                  - Dimm number
  @param[in] SubCh                 - Subchannel number
  @param[in] Rank                  - Rank number
  @param[in] BacksideCopy          - The signal backside copy number
  @param[in] Mode                  - Mode
  @param[in] Delay                 - Delay value
  @param[in] PassThroughMode       - It's in pass through mode or not

  @retval SUCCESS or FAILURE

**/
UINT32
EFIAPI
ProgramFinalQcaDelayResult (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     SubCh,
  IN UINT8     Rank,
  IN UINT8     BacksideCopy,
  IN UINT8     Mode,
  IN INT8      Delay,
  IN BOOLEAN   PassThroughMode
  )
{
  return SUCCESS;
}

/**
Set QCA training results.
@param[in] Host                 - Pointer to Host structure
@param[in] Socket               - Processor socket within the system (0-based)
@param[in] Ch                   - DDR channel number within the processor socket (0-based)
@param[in] Level                - IO level to access
@param[in] Mode                 - Bit-field of different modes
@param[in] ListType             - Selects type of each list
@param[in] ListSize             - Number of entries in each list
@param[in] RcdBacksidesignalListEdgeCh  - Array of pointers to listType

@retval MRC_STATUS

**/
MRC_STATUS
EFIAPI
SetCombinedCaClkBacksideDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Subch,
  IN MRC_LT    Level,
  IN UINT8     Mode,
  IN UINT8     ListType,
  IN UINT16    ListSize,
  IN GSM_CSEDGE_CA (*RcdBacksidesignalListEdgeCh) [SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP]
  )
{
  return SUCCESS;
} // SetCombinedCaClkBacksideDdr5

/**

  Executes Rcd Dca Dck Duty Cycle Training

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EFIAPI
RcdDcaDckDutyCycle (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} //RcdDcaDckDutyCycle

/**
  Optimizes DCA TCO by margining DCA eye width at various settings for DCA TCO.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainDcaTco (
  IN     PSYSHOST Host
  )

{
  return SUCCESS;
} //TrainDcaTco

/**
  Sweep DCA signals and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in]      SignalCount       Specifies the maximum signal count in the margin results structure - "DcaResults"
  @param[in, out] DcaResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetDcaMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN     UINT8             SignalCount,
  IN OUT struct baseMargin *DcaResults
  )
{
  return MRC_STATUS_SUCCESS;
} // GetDcaMargins

/**
  Sweep QCA signals and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in]      SignalCount       Specifies the maximum signal count in the margin results structure - "QcaResults"
  @param[in, out] QcaResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetQcaMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN     UINT8             SignalCount,
  IN OUT struct baseMargin *QcaResults
  )
{
  return MRC_STATUS_SUCCESS;
} // GetQcaMargins