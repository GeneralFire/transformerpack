/** @file
  MemRcdCwCache.c
  Functions needed to cache DDR5 Registering Clock Driver (RCD) Control Words

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
  Validate the RCD data structure backing store

  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateRcdDataStucture (
  VOID
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Write an entry in the RCD CW cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Subchannel mask within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
WriteRcdCwCache (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Page,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Return an entry from the RCD CW cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[out] Data      - Poitner to caller's data buffer

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
ReadRcdCwCache (
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh,
  IN      UINT8       Dimm,
  IN      UINT8       Page,
  IN      UINT8       Address,
     OUT  UINT8       *Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

