/** @file
  This file contains routines for GPIO GSX initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include "GpioGsxLibrary.h"
#include <Library/GpioGsxLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Register/GpioRegs.h>


#define GSX_BUSY_TIMEOUT 5

extern GSX_GROUP_INFO mGsxGroupInfo[];
extern UINT16 mGsxGroupCount;

/**
    Wait for GSX Busy field to be cleared. Timeout otherwise.

    @param GpioCom    GPIO Community where GSX lives. Refer to GSX_GROUP_INFO in GpioGsxLib.h.

    @retval Status EFI_SUCCESS       GSX is ready to be accessed.
    @retval Status EFI_UNSUPPORTED   GSX is busy and cannot be accessed.
**/
EFI_STATUS
IsGsxBusy (
  UINT16 GpioCom
  )
{
  UINT32 Busy;
  UINT16 Delay;

  if (GpioCom >= mGsxGroupCount) {
    return EFI_INVALID_PARAMETER;
  }

  Delay = GSX_BUSY_TIMEOUT;
  do {
    //
    //Read the Busy field to check if it's cleared
    //
    Busy  = MmioRead32 (
              PCH_PCR_ADDRESS (mGsxGroupInfo[GpioCom].Community, mGsxGroupInfo[GpioCom].C0CmdOffset)
            ) & B_GPIO_PCH_GPIO_GSX_PCR_C0CMD_BUSY;
    if (Busy == 0) {
      break;
    }

    //
    // Stall for 10 microseconds.
    //
    MicroSecondDelay (10);

    Delay--;
  } while (Delay > 0);

  if (Delay == 0) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
    GSX initialization code to configure input and output numbers.

    @param GpioCom    GPIO Community where GSX lives. Refer to GSX_GROUP_INFO in GpioGsxLib.h.
    @param NIn        Number of input expanders.
    @param NOut       Number of output expanders.
**/
EFI_STATUS
InitGSX (
  UINT16 GpioCom,
  UINT16 NIn,
  UINT16 NOut
  )
{
  UINT32 Data32;

  if (GpioCom >= mGsxGroupCount) {
    return EFI_INVALID_PARAMETER;
  }

  Data32 = NIn & B_GPIO_PCH_GPIO_GSX_PCR_C0CAP_DW0_NIN;
  Data32 |= (NOut << N_GPIO_PCH_GPIO_GSX_PCR_C0CAP_DW0_NOUT) & B_GPIO_PCH_GPIO_GSX_PCR_C0CAP_DW0_NOUT;

  MmioWrite32 (
    PCH_PCR_ADDRESS (mGsxGroupInfo[GpioCom].Community, mGsxGroupInfo[GpioCom].C0CapDw0Offset),
    Data32
    );

  return EFI_SUCCESS;
}

/**
    Returns the value of the CxGPI received over the GSX channel.

    @param GpioCom    GPIO Community where GSX lives. Refer to GSX_GROUP_INFO in GpioGsxLib.h.
    @param *Data      Address where Dword0 and Dword1 from CxGPI will be returned.

    @retval Status EFI_SUCCESS       GSX read success.
    @retval Status EFI_UNSUPPORTED   GSX is busy and cannot be accessed.
**/
EFI_STATUS
GsxRead (
  UINT16                GpioCom,
  UINT32                *Data
  )
{
  EFI_STATUS           Status;

  if ((GpioCom >= mGsxGroupCount) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check there are no pending transactions
  //
  Status = IsGsxBusy(GpioCom);
  if (Status == EFI_TIMEOUT) {
    DEBUG ((DEBUG_ERROR, "GSX timed out \n"));
    return EFI_TIMEOUT;
  }

  //
  // SW Start serialization process
  //
  MmioOr32 (
    PCH_PCR_ADDRESS (mGsxGroupInfo[GpioCom].Community, mGsxGroupInfo[GpioCom].C0CmdOffset),
    B_GPIO_PCH_GPIO_GSX_PCR_C0CMD_START
    );

  //
  // Wait for the read cycle to finish
  //
  DEBUG ((DEBUG_INFO, "Waiting GSX command to complete \n"));
  Status = IsGsxBusy(GpioCom);
  if (Status == EFI_TIMEOUT) {
    DEBUG ((DEBUG_ERROR, "GSX timed out \n"));
    return EFI_TIMEOUT;
  }

  //
  // Read GPIO Input bits [31:0]
  //
  Data[0] = MmioRead32 (
              PCH_PCR_ADDRESS (mGsxGroupInfo[GpioCom].Community, mGsxGroupInfo[GpioCom].C0GpiLvlDw0Offset)
              );
  //
  // Read GPIO Input bits [63:32]
  //
  Data[1] = MmioRead32 (
              PCH_PCR_ADDRESS (mGsxGroupInfo[GpioCom].Community, mGsxGroupInfo[GpioCom].C0GpiLvlDw1Offset)
              );

  return EFI_SUCCESS;
}
