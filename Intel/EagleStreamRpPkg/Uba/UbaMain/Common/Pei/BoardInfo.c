/** @file
  Install Board Info Data.

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

#include "PeiCommonBoardInitLib.h"
#include <Library/UbaBoardSioInfoLib.h>
#include <SioRegs.h>
#include <Platform.h>

STATIC SIO_INDEX_DATA mSioInitTable[] = {
  //
  // Init GPIO
  //
  {
    PILOTIV_LOGICAL_DEVICE,
    PILOTIV_SIO_GPIO
  },
  {
    PILOTIV_ACTIVATE,
    0x01
  },
  {
    PILOTIV_BASE_ADDRESS_HIGH0,
    (UINT8) ((UINT16) SIO_GPIO_BASE_ADDRESS >> 8)
  },
  {
    PILOTIV_BASE_ADDRESS_LOW0,
    (UINT8) (SIO_GPIO_BASE_ADDRESS & 0x00ff)
  }
};

//
// Platform board sio information structure
//
static PEI_BOARD_SIO_INFO BoardSioInfoData = {
  BOARD_SIO_INFO_DATA_SIGNATURE,
  BOARD_SIO_INFO_DATA_VERSION,
  //
  // SIO Initialization table
  //
  PILOTIV_SIO_INDEX_PORT,                  // SIO Index port
  PILOTIV_SIO_DATA_PORT,                   // SIO Data port
  mSioInitTable,                            // mSioInitTable - contains the settings for initializing the SIO.
  sizeof(mSioInitTable)/sizeof(SIO_INDEX_DATA) // NumSioItems - Number of items in the SIO init table.
};


EFI_STATUS
InstallBoardInfoData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;

   Status = UbaConfigPpi->AddData (
                                  UbaConfigPpi,
                                  &gPlatformBoardSioInfoDataGuid,
                                  &BoardSioInfoData,
                                  sizeof(BoardSioInfoData)
                                  );

  return Status;
}


