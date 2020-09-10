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

#include <SysHost.h>
#include <Library/Ddr5CoreLib.h>

/**

  Send MRW Command to RCD and update the RCD CW cache

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
RcdControlWordWrite (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Read a DWORD from DDR5 RCD over SMBUS
  Note the protocol only allows accesses to addresses on
  DWORD boundaries. The RCD will mask off the lower 2 address
  bits.
  To read a specific byte, use ReadRcdSmbByteDdr5.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubCh        - SubChannel number (0-based)
  @param[in]  Rank         - Rank number (0-based)
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[out] CwData       - Pointer to the data buffer

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadRcdSmbDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubCh,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
     OUT  UINT32      *CwData
  )
{
   return SUCCESS;
}

/**

  Read a BYTE from DDR5 RCD over SMBUS

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubCh        - SubChannel number (0-based)
  @param[in]  Rank         - Rank number (0-based)
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[out] CwData       - Pointer to the data buffer

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadRcdSmbByteDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubCh,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
     OUT  UINT8       *CwData
  )
{
  return SUCCESS;
}

/**

  Write a BYTE to the DDR5 RCD over SMBUS

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubChMask    - SubChannel mask
  @param[in]  Rank         - Rank number (0-based)
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[in]  CwData       - Data to write

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
WriteRcdSmbDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubChMask,
  IN      UINT8       Size,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
  IN      UINT32      *CwData
  )
{
   return SUCCESS;
}

/**
  Perform DDR5 Rambus D0 errata

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval  TRUE  - Errata is applied
           FALSE - Errata is not applied
**/
BOOLEAN
EFIAPI
Ddr5RambusD0Errata (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  return FALSE;
}

