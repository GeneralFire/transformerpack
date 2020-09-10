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
  Store the common DDR5 DIMM configuration in the Host structure

  @param[in,out] Host   - Pointer to sysHost
  @param[in]     Socket - CPU Socket ID
  @param[in]     Ch     - Channel to be accessed
  @param[in]     Dimm   - DIMM to be accessed

  @retval N/A

**/
VOID
GetCommonDDR5DIMMConfig (
  IN OUT PSYSHOST Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm
  )
{
} // GetCommonDDR5DIMMConfig

/**

  Detect DDR5 configuration

  @param Host   - Point to sysHost
  @param socket - CPU socket ID
  @param ch     - channel to be accessed
  @param dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
DetectDDR5DIMMConfig (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
{
  return SUCCESS;
} // DetectDDR5DIMMConfig

/**

  Collects data from SPD for DDR5 LRDIMM

  @param[in] Host   - Point to sysHost
  @param[in] Socket - CPU socket ID
  @param[in] Ch     - Channel to be accessed
  @param[in] Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
GetLrdimmSpdDataDdr5 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  )
{
  return SUCCESS;
} // GetLrdimmSpdDataDdr5

/**

  Collects data from SPD for DDR5

  @retval SUCCESS

**/
UINT32
GatherSPDDataDDR5 (
  VOID
  )
{
  return SUCCESS;
} // GatherSPDDataDDR5

/**

  Lookup DDR5 rank in the technology table

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket to initizlize
  @param[in] Ch      - Channel on the socket
  @param[in] Dimm    - Dimm on the channel

  @retval Tech table index

**/
UINT8
GetTechIndexDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
)
{
  return 0xFF;
} // GetTechIndexDdr5

/**

  Calculate package rank size (in 64MB unit) for asymmetric or symmeteric DIMM gived the target package rank number.

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] Dimm              - Dimm number
  @param[in] Rank              - Package rank number
  @param[out] RankSize         - Pointer to rank size in 64MB unit

  @retval MRC_STATUS_SUCCESS

**/

MRC_STATUS
EFIAPI
CalculateDimmPackageRankSizeDdr5 (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Dimm,
  IN     UINT8   Rank,
     OUT UINT16  *RankSize
  )
{
  return MRC_STATUS_SUCCESS;
} // CalculateDimmPackageRankSizeDdr5