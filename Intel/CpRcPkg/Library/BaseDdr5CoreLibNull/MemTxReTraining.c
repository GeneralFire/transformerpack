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

Perform Periodic Tx Retraining

@param[in] Host - Pointer to sysHost, the system Host (root) structure

@retval SUCCESS

**/
UINT32
EFIAPI
PeriodicTxRetrain (
  IN PSYSHOST Host
  )
{
  return SUCCESS;
} // PeriodicTxRetrain

/**

  Configures the data buffer to DRAM tDQS2DQ Tracking Initialization Mode or DRAM
  tDQS2DQ Tracking Mode based on input paramater.

  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number
  @param[in] Dimm          - DIMM number in Channel
  @param[in] Rank          - Rank number on DIMM
  @param[in] TrackingMode  - 0 for Tracking Initialization
                           - 1 for Tracking Mode

  @retval N/A

**/
VOID
EFIAPI
ConfigureDbDqs2Dq (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    TrackingMode
  )
{
  return;
}