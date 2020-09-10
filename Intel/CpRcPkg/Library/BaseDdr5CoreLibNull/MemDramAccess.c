/** @file
  MemDramAccess.c
  Functions needed to send commands to DDR5 DRAMs

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
  Validate the DRAM data structure backing store

  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateDramDataStucture (
  VOID
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Send MRW Command to DRAM

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
DramModeRegWrite (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Return the cached value of the per-rank MR specified by Address.
  If the MR is in the cache list, return the contents in the caller's Data buffer.
  If the MR is not in the cache list, do not modify the caller's Data buffer.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[out] Data      - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS cached data is returned
  @retval MRC_STATUS_FAILURE if the mode register is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
ReadDramModeRegCachePerRank (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Update the MRW cache for a per-rank MRW specified by Address

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Mask of subchannel numbers to be updated
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
WriteDramModeRegCachePerRank (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Return the cached value of the per-device MR specified by Address.
  If the MR is in the cache list, return the contents in the caller's Data buffer.
  If the MR is not in the cache list, do not modify the caller's Data buffer.
  ASSERT if the MR is not in the cache list.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[out] Data      - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS if cached data is returned
  @retval MRC_STATUS_FAILURE if the MR is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
ReadDramModeRegCachePerDevice (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**
  Update the MRW cache for a per-device MRW specified by Address
  ASSERT if the MR is not in the cache list

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Mask of subchannel numbers to be updated
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
WriteDramModeRegCachePerDevice (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Send MRW PDA Command to DRAM and update the MRW cache

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram device number (sub-channel-based)
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
DramModeRegWritePda (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}
