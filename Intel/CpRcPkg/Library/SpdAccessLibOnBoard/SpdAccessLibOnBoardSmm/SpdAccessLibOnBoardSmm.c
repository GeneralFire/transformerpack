/** @file
  This library exposes the SPD read function for use during SMM.

  This implementation works as an interface between an agent that needs
  to read a DIMM SPD and the SPD.  The read function accepts DIMM location information as well
  as the SPD byte offset.

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
#include <Library/SpdAccessLibOnBoard.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>

extern EFI_GUID gSpdTableHobGuid;

//
// Module variable
//
// Use Max Spd Byte Number for DDR4 and DDR5
UINT8 mSpdTableDDR[MAX_CH][MAX_DIMM][MAX_SPD_BYTE_DDR];
UINT8 mSpdModuleType;

/**
  Performs SPD byte read operations.

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
SpdReadByte (
  IN     UINT8  Socket,
  IN     UINT8  Channel,
  IN     UINT8  Dimm,
  IN     UINT16 ByteOffset,
     OUT UINT8  *Data
  )
{
  //
  // Sanity check of incoming data
  //
  if ((Socket >= MAX_SOCKET) || (Channel >= MAX_CH) || (Dimm >= MAX_DIMM) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Do parameter check and Spd data read work by different DDR type.
  switch (mSpdModuleType) {
    case SPD_TYPE_DDR4:
    case SPD_TYPE_AEP:
      if (ByteOffset >= MAX_SPD_BYTE_DDR4) {
        return EFI_INVALID_PARAMETER;
      }
      break;

    case SPD_TYPE_DDR5:
      if (ByteOffset >= MAX_SPD_BYTE_DDR5) {
        return EFI_INVALID_PARAMETER;
      }
      break;

    default:
      DEBUG ((EFI_D_ERROR, "SpdReadByte: Unsupported mSpdModuleType = 0x%x\n", mSpdModuleType));
      return EFI_UNSUPPORTED;
  }

  if (mSpdTableDDR[Channel][Dimm][0] == SPD_BYTE0_UNDEFINED) {
    return EFI_NOT_FOUND;
  } else {
    *Data = mSpdTableDDR[Channel][Dimm][ByteOffset];
  }

  return EFI_SUCCESS;
}

/**
  The constructor function searches the SPD table HOB and stores its content in a module
  variable.

  @param[in] ImageHandle - The firmware allocated handle for the EFI image.
  @param[in] SystemTable - A pointer to the EFI System Table.

  @retval EFI_SUCCESS - The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SpdAccessLibOnBoardConstructor (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  VOID              *SpdTable;
  EFI_HOB_GUID_TYPE *GuidHob = NULL;

  //
  // Get SPD table HOB
  //
  GuidHob = GetFirstGuidHob (&gSpdTableHobGuid);

  if (GuidHob != NULL) {
    SpdTable = GET_GUID_HOB_DATA (GuidHob);
    // Read the SPD_KEY_BYTE in Channel 0, Dimm 0, do not support DIMM mixed plugged.
    mSpdModuleType = *((UINT8 *) SpdTable + SPD_KEY_BYTE);
    // Do SpdTable copy work by different DDR type.
    switch (mSpdModuleType) {
      case SPD_TYPE_DDR4:
      case SPD_TYPE_AEP:
      case SPD_TYPE_DDR5:
        CopyMem ((VOID *) mSpdTableDDR, SpdTable, sizeof (mSpdTableDDR));
        break;

      default:
        DEBUG ((EFI_D_ERROR, "SpdAccessLibOnBoardConstructor: Unsupported mSpdModuleType = 0x%x\n", mSpdModuleType));
        return EFI_UNSUPPORTED;
    }
  } else {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
  }
  
  return EFI_SUCCESS;
}