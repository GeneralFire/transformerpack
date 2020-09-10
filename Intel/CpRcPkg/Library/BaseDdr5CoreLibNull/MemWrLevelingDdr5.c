/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

Perform Write Leveling training for DDR5

@param Host  - Pointer to sysHost

@retval SUCCESS

**/

UINT32
WriteLevelingDdr5 (
  PSYSHOST Host
  )
{
  return SUCCESS;
}

/**

This function execute the write leveling Cleanup data evaluation for DDR5 (and DDRT2).

@param Host                - Pointer to sysHost
@param ByteOff             - Byte Offset
@param ByteSum             - Byte Sum
@param ByteCount           - Byte Count
@param Done                - Done indication from centering flow
@param CRAddDelay          - Additive Delay
@param WLChipCleanUpStruct - WL Delays
@param DdrTech             - DDR Technology 0 DDR4 1 DDRT

@retval NA

**/
VOID
WriteLevelingCleanUpDataEvalTechDdr5 (
  IN PSYSHOST                Host,
  IN INT16                   ByteOff [MAX_CH][SUB_CH][MAX_RANK_CH],
  IN INT16                   ByteSum[2][MAX_CH][SUB_CH],
  IN UINT16                  ByteCount[2][MAX_CH][SUB_CH],
  IN UINT8                   Done,
  IN CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct,
  IN UINT8                   DdrTech)
{
}

/**

Add training Delay

@param[in] Host           - Pointer to sysHost
@param[in] CteDelay       - CTE Delay
@param[in] PhysicalDelay  - Delay in real platform

@retval N/A

**/
VOID
EFIAPI
TrainingDelay (
  IN PSYSHOST Host,
  IN UINT32   CteDelay,
  IN UINT32   PhysicalDelay
  )
{
  return;
}

/**

  Prints the DDR5 WL Coarse Results

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Current Socket
  @param[in] Ch            - Channel number
  @param[in] Dimm          - Dimm number
  @param[in] Rank          - Rank number
  @param[in] SubChannel    - subchannel number
  @param[in] MaxStrobe     - Maximum number of strobes

  @retval N/A

**/
VOID
EFIAPI
DisplayWlCoarseResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubChannel,
  IN UINT8     MaxStrobe
  )
{
  return;
} // DisplayWlCoarseResultsDdr5