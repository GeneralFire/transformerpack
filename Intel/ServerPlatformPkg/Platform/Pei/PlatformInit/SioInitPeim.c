/** @file
  Functions for LpcSio initilization

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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
// APTIOV_SERVER_OVERRIDE_RC_START : Use AMI SIO module for SIO initialization
#if 0
#include "PlatformInitPreMem.h"


STATIC SIO_INDEX_DATA  mW83527Table [] = {
  //
  // Enter Configuration Mode
  //
  { W83527_SIO_INDEX_PORT, 0x87 },
  { W83527_SIO_INDEX_PORT, 0x87 },
  //
  // GPIO Config
  //
  { W83527_SIO_INDEX_PORT, W83527_LOGICAL_DEVICE },       // 0x7
  { W83527_SIO_DATA_PORT,  W83527_SIO_GPIO },             // 0x9
  //
  // Disable  GPIO 2 to make Keyboard work
  //
  { W83527_SIO_INDEX_PORT, W83527_ACTIVATE_REG }, //CR 0x60
  { W83527_SIO_DATA_PORT,  0x0 },                 // 0x00
  //
  // Exit Configuration Mode
  //
  { W83527_SIO_INDEX_PORT, 0xAA }
};


/**

  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers.

  @param PeiServices  -  PeiService point.
  @param CpuIo        -  CpuIo PPI to read/write IO ports.

  @retval EFI_SUCCESS  -  Init succeed.

**/
VOID
LpcSioEarlyInit (
  IN     PEI_BOARD_SIO_INFO              *BoardSioInfo
  )
{
  UINT8 Index;
  UINT32 SioExist;
  EFI_STATUS Status;

#ifdef ASPEED_ENABLE
    PchLpcGenIoRangeSet ((0x600  & 0xFF0), 0x10);

    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_UNLOCK);
    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_UNLOCK);

    //
    //mailbox
    //
    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, REG_LOGICAL_DEVICE);
    IoWrite8 (ASPEED2500_SIO_DATA_PORT, ASPEED2500_SIO_MAILBOX);

    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, BASE_ADDRESS_HIGH0);
    IoWrite8 (ASPEED2500_SIO_DATA_PORT, (UINT8)(0x600 >> 8));

    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, BASE_ADDRESS_LOW0);
    IoWrite8 (ASPEED2500_SIO_DATA_PORT, (UINT8)(0x600 & 0xFF));
    //
    //active mailbox
    //
    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ACTIVATE);
    IoWrite8 (ASPEED2500_SIO_DATA_PORT, 1);

    IoWrite8 (ASPEED2500_SIO_INDEX_PORT, ASPEED2500_SIO_LOCK);
#endif
  //
  //due to following code only int pilotiv, skit it if pilotiv is not exist.
  //
  SioExist = IsSioExist();
  if((SioExist & PILOTIV_EXIST) == 0){
    return;
  }

  //
  // Do early SIO GPIO init
  //
  for (Index = 0; Index < BoardSioInfo->NumSioItems; Index++) {
    IoWrite8 (
      BoardSioInfo->SioIndexPort,
      BoardSioInfo->mSioInitTable[Index].Index
      );

    IoWrite8 (
      BoardSioInfo->SioDataPort,
      BoardSioInfo->mSioInitTable[Index].Value
      );
  }
  Status = PchLpcGenIoRangeSet(PILOTIV_KCS3_DATA_BASE_ADDRESS & ~0xF, 0x10);
  ASSERT_EFI_ERROR(Status);
  DEBUG((DEBUG_INFO, "[SIO] PilotIV KCS base at 0x%X\n", PILOTIV_KCS3_DATA_BASE_ADDRESS & ~0xF));

  Status = PchLpcGenIoRangeSet(PILOTIV_MAILBOX_BASE_ADDRESS, 0x10);
  ASSERT_EFI_ERROR(Status);
  DEBUG((DEBUG_INFO, "[SIO] PilotIV mailbox at  0x%X\n", PILOTIV_MAILBOX_BASE_ADDRESS));
}
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Use AMI SIO module for SIO initialization

