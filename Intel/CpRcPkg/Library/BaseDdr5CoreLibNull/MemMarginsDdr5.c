/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include <Library/Ddr5CoreLib.h>

/**

  Display Right Edge and Left Edge margin results for DDR5.

  @param[in] Host          - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket        - current socket
  @param[in] Ch            - channel number
  @param[in] Subch         - Sub Channel number (0-based)
  @param[in] Dimm          - dimm number
  @param[in] Rank          - rank number (0-based)
  @param[in] ResultsBit    - Per Bit margin results
  @param[in] Group         - Command group

  @retval None

**/
VOID
DisplayREandLEMarginsDdr5 (
  IN PSYSHOST            Host,
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               SubCh,
  IN UINT8               Dimm,
  IN UINT8               Rank,
  IN struct bitMarginCh  *ResultsBit,
  IN MRC_GT              Group
  )
{
  return;
} // DisplayREandLEMarginsDdr5

/**

  This is a DDR5 wrapper for GetMargins.  GetMargins is only called when DDR5 is installed.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Processor socket to check
  @param[in] Level          - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param[in] Group          - Parameter to be margined
  @param[in] Mode           - Test mode to use
  @param[in] Scope          - Margin granularity
  @param[in] Mask           - Mask of bits to exclude from testing
  @param[in out] MarginData - Pointer to the structure to store the margin results. The structure type varies based on scope.
  @param[in] PatternLength  - RankMarginTest Pattern length
  @param[in] Update         - Update starting margins so the next pass will run faster or not
  @param[in] ChMask         - Bit Mask of channels to not be used
  @param[in] RankMask       - Bit Mask of ranks to not be used
  @param[in] BurstLength    - Number of cachelines to test for each write/read operation
  @param[in] TestType       - Type of test to be run

  @retval None

**/
VOID
GetLegacyRmtMarginsDdr5 (
  IN PSYSHOST        Host,
  IN UINT8           Socket,
  IN MRC_LT          Level,
  IN MRC_GT          Group,
  IN UINT16          Mode,
  IN UINT8           Scope,
  IN struct bitMask  *Mask,
  IN OUT VOID        *MarginData,
  IN UINT32          PatternLength,
  IN UINT8           Update,
  IN UINT8           ChMask,
  IN UINT8           RankMask,
  IN UINT8           BurstLength,
  IN MRC_TT          TestType
  )
{
  return;
}

/**

  Writes GetMargin results to Debug log.  Results are only displayed when DDR5 is installed.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Processor socket
  @param[in] ResultsQcs     - Pointer to array which contains QCS test results
  @param[in] ResultsQca     - Pointer to array which contains QCA test results
  @param[in] ResultsRxDqs   - Pointer to array which contains RxDqs test results
  @param[in] ResultsTxDq    - Pointer to array which contains TxDq test results
  @param[in] ResultsRxVref  - Pointer to array which contains RxVref test results
  @param[in] ResultsTxVref  - Pointer to array which contains TxVref test results
  @param[in] ResultsCmd     - Pointer to array which contains Cmd test results
  @param[in] ResultsCtl     - Pointer to array which contains Ctl test results
  @param[in] NumSubCh       - Number of elements in ResultsQcs and ResultsQca

**/
VOID
DisplayLegacyRmtMarginsDdr5 (
  IN PSYSHOST           Host,
  IN UINT8              Socket,
  IN struct rankMargin  *ResultsQcs,
  IN struct rankMargin  *ResultsQca,
  IN struct rankMargin  *ResultsRxDqs,
  IN struct rankMargin  *ResultsTxDq,
  IN struct rankMargin  *ResultsRxVref,
  IN struct rankMargin  *ResultsTxVref,
  IN struct rankMargin  *ResultsCmd,
  IN struct rankMargin  *ResultsCtl,
  IN UINT8              NumSubCh
  )
{
  return;
}

