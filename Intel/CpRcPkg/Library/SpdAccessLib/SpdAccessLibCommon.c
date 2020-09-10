/** @file
  The SPD Access Lib Common file provides common functions for the PEI, DXE, and
  SMM phases of the implmentation of SpdAccessLib.

  This common file provides functions which are universal between PEI, DXE, and
  SMM.  The implementations for preparing the SMBus data and selecting the
  appropriate SPD page offset are common between phases so they are implemented
  here to mitigate replicated code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#include <SysRegs.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <ScratchpadList.h>
#include "SpdAccessLibCommon.h"
#include <Library/BaseLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemMcIpLib.h>


/**
  Fills the SMB_DEVICE_STRUC with the relevant SMBus address information in preparation
  for an SMBus transaction.

  @param[in] Socket      - Socket ID
  @param[in] Chan        - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[out] SpdDev     - Pointer to a SMB_DEVICE structure

  @retval EFI_SUCCESS       SMBUS read was successful
  @retval EFI_DEVICE_ERROR  SMBUS read was unsuccessful
**/
EFI_STATUS
PrepSmbData (
  IN UINT8               Socket,
  IN UINT8               Chan,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *SpdDev
  )
{
  EFI_STATUS  Status;

  // Fill out relevant data for a SPD SMbus transaction
  SpdDev->compId = SPD;
  SpdDev->address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  SpdDev->address.deviceType = DTI_EEPROM;

  // Get the SMBus address for the specified DIMM
  Status = GetSmbAddress (Socket, Chan, Dimm, SpdDev);

  return Status;
}

/**
  Calculate page by dividing the offset by the page size

  @param[in] DramType      - DDR technology
  @param[in] ByteOffset    - SPD Byte Offset to be read

  @retval
**/
UINT8
CalculateSpdPageNum (
  IN UINT16   DramType,
  IN UINT16   ByteOffset
  )
{
  UINT16      PageSize;

  if (DramType == SPD_TYPE_DDR5) {
    PageSize = SPD_DDR5_PAGE_SIZE;
  } else {
    PageSize = SPD_EE_PAGE_SIZE;
  }

  return (UINT8) (ByteOffset / PageSize);
}
