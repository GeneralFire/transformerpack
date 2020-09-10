/** @file
  This library exposes the SPD functions for use during PEI, DXE, and SMM.

  This implementation works as an interface for an agent that needs
  to access a DIMM SPD and the SPD.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Library/MemMcIpLib.h>
#include <Library/SpdAccessLib.h>

/**
  Initializes any SPD related data structures and prepares the system for
  initiating SPD reads via the SMBus if necessary.  This must be called
  before attempting to read/write to SPD.

  @param[in] Socket  - Socket ID
  @param[in] DdrType  - Type of DDR Technology supported

  @retval EFI_SUCCESS      Successfully initialized SPD related functionality
  @retval EFI_DEVICE_ERROR Error initializing devices
**/
EFI_STATUS
EFIAPI
InitializeSpd (
  IN UINT8                Socket,
  IN DDR_TECHNOLOGY_TYPE  DdrType
  )
{
  return EFI_SUCCESS;
}

/**
  Performs SPD byte write operations.

  @param [in    ] Socket     - Socket ID
  @param [in    ] Channel    - Memory channel number
  @param [in    ] Dimm       - Memory DIMM slot
  @param [in    ] ByteOffset - SPD byte to be returned
  @param [   out] Data       - Pointer to store the SPD byte

  @retval EFI_SUCCESS           The read completed successfully
  @retval EFI_DEVICE_ERROR      Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER Input parameter out of bounds
  @retval EFI_NOT_FOUND         Specifed DIMM SPD information was not found
**/
EFI_STATUS
EFIAPI
SpdWriteByte (
  IN     UINT8  Socket,
  IN     UINT8  Channel,
  IN     UINT8  Dimm,
  IN     UINT16 ByteOffset,
     OUT UINT8  *Data
  )
{
  DEBUG ((EFI_D_ERROR, "SPD Writes are not allowed for SPDs stored in FV!\n"));

  return EFI_NOT_FOUND;
}

/**
  Indicates to the Library that DIMM detection is complete.

  @param  None

  @retval EFI_SUCCESS   DIMM detection done successfully communicated.
  @retval EFI_NOT_READY Cannot store DIMM detection done indicator.
**/
EFI_STATUS
EFIAPI
SpdDimmDetectDone (
  VOID
  )
{
  //
  // DIMM Detection is complete
  //
  return EFI_SUCCESS;
}