/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/UbaCfgDb.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmBase2.h>


//
// UBA and GPIO headers
//

#include <Library/UbaGpioPlatformConfig.h>
#include <Library/GpioLib.h>


STATIC PLATFORM_GPIO_CONFIG_TABLE             mGpioParams;


/**

The library constructor call. Gets required protocols and stores for later usage
This also applies for SMM mode usage

@param[in]  None

@retval EFI_SUCCESS             The function completed successfully

**/
EFI_STATUS
EFIAPI
InitializeDxeUbaPlatLib (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PROTOCOL          *UbaConfigProtocol = NULL;
  UINTN                                 TableSize;


    Status = gBS->LocateProtocol (
                    &gUbaConfigDatabaseProtocolGuid,
                    NULL,
                    &UbaConfigProtocol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

  TableSize = sizeof (PLATFORM_GPIO_CONFIG_TABLE);
  Status = UbaConfigProtocol->GetData (
                                UbaConfigProtocol,
                                &gPlatformGpioPlatformConfigDataGuid,
                                &mGpioParams,
                                &TableSize
                                );

  return Status;

}



/**

    Reads GPIO pin to get DFX jumper status

    @param[out] DfxJumper - The pointer to the DFX jumper input

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetDfxPadVal (
  OUT UINT32 *DfxJumper
  )
{
  EFI_STATUS           Status;

  if (mGpioParams.DfxJumperPad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = GpioGetInputValue (mGpioParams.DfxJumperPad, DfxJumper);
  return Status;

}

/**

    Reads GPIO pin to get recovery jumper status

    @param[out] RcvJumper - The pointer to the Recovery jumper input

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetRcvPadVal (
  OUT UINT32 *RcvJumper
)
{
  EFI_STATUS           Status;
  if (mGpioParams.RcvJumper == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }
  Status = GpioGetInputValue (mGpioParams.RcvJumper, RcvJumper);
  return Status;
}


/**

    Reads GPIO pin to get FM ADR trigger pin

    @param[out] FmAdrTrigger - The pointer to the ADR trigger input

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetFmAdrTriggerPadVal (
  OUT UINT32 *FmAdrTrigger
)
{
  EFI_STATUS           Status;

  if (mGpioParams.FmAdrTrigger == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = GpioGetInputValue (mGpioParams.FmAdrTrigger, FmAdrTrigger);
  return Status;
}

/**

    Sets GPIO pin to enable ADR on the board

    @param Set[in] - If TRUE means the pas should go 'high', otherwise 'low'

    @retval Status - Success if GPIO set properly

**/
EFI_STATUS
GpioSetAdrEnablePadOutVal (
  IN BOOLEAN Set
)
{
  EFI_STATUS           Status;

  if (mGpioParams.AdrEnable == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  if (Set) {
    Status = GpioSetOutputValue (mGpioParams.AdrEnable, GpioOutHigh);
  }
  else {
    Status = GpioSetOutputValue (mGpioParams.AdrEnable, GpioOutLow);
  }
  return Status;
}

/**

    Reads GPIO pin to Force to S1 config mode pad

    @param[out] ForceS1ConfigPad - Input value of the Froce S1 Config pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetForcetoS1ConfigModePadVal (
  OUT UINT32 *ForceS1ConfigPad
)
{
  EFI_STATUS           Status;

  if (mGpioParams.ForceTo1SConfigModePad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = GpioGetInputValue (mGpioParams.ForceTo1SConfigModePad, ForceS1ConfigPad);
  return Status;
}

/**

    Reads GPIO pin related to QAT

    @param[out] QATPad - Input value of the QAT pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetQATPadVal (
  OUT UINT32 *QATPad
)
{
  EFI_STATUS           Status;

  if (mGpioParams.QATGpio == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = GpioGetInputValue (mGpioParams.QATGpio, QATPad);
  return Status;

}

/**

    Get GPIO pin for SCI detection for WHEA RAS functionality

    @param[out] WheaSciPad - Input value of the Whea SCI pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetWheaSciPad (
  OUT UINT32 *WheaSciPad
)
{

  if (mGpioParams.WheaSciPad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  *WheaSciPad = (UINT32) mGpioParams.WheaSciPad;
  return EFI_SUCCESS;

}

/**

    Get GPIO pin for FPGA error detection RAS functionality

    @param[out] FpgaErrorPad -The input value of the FPGA error 1 pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetFpgaErrorPad1 (
  OUT UINT32 *FpgaErrorPad
)
{

  if (mGpioParams.FpgaErrorSingnalPad1 == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  *FpgaErrorPad = (UINT32) mGpioParams.FpgaErrorSingnalPad1;
  return EFI_SUCCESS;

}

/**

    Get GPIO pin for FPGA error detection RAS functionality

    @param[out] FpgaErrorPad -The input value of the FPGA error 2 pad

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetFpgaErrorPad2 (
  OUT UINT32 *FpgaErrorPad
)
{

  if (mGpioParams.FpgaErrorSingnalPad2 == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }
  *FpgaErrorPad = (UINT32) mGpioParams.FpgaErrorSingnalPad2;
  return EFI_SUCCESS;

}

/**

    Get GPIO pin for CPU HP SMI detection for RAS functionality

    @retval Status - Success if GPIO's pad read properly

**/
EFI_STATUS
GpioGetCpuHpSmiPad (
  OUT UINT32 *CpuHpSmiPad
)
{

  if (mGpioParams.CpuHpSmiPad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  *CpuHpSmiPad = (UINT32) mGpioParams.CpuHpSmiPad;
  return EFI_SUCCESS;

}

/**

    Reads GPIO pin that is first bit of the Board ID indication word

    @param[out] BoardID0Gpio - Input value of the first Board ID pad

    @retval Status - Success if GPIO's are read properly

**/
EFI_STATUS
GpioGetBoardId0PadVal (
  OUT UINT32 *BoardID0Gpio
)
{
  EFI_STATUS           Status;

  if (mGpioParams.BoardID0Gpio == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  Status = GpioGetInputValue (mGpioParams.BoardID0Gpio, BoardID0Gpio);
  return Status;

}

/**

    Sets GPIO's used for Boot Mode

    @param None

    @retval Status - Success if GPIO's are configured

**/
EFI_STATUS
GpioConfigForMFGMode (
  VOID
  )
{
  EFI_STATUS                  Status;

  if (mGpioParams.GpioMfgPad.GpioPad == UNUSED_GPIO) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((EFI_D_INFO, "Start ConfigureGpio() for BootMode Detection.\n"));

  Status = GpioSetPadConfig (mGpioParams.GpioMfgPad.GpioPad,
    &mGpioParams.GpioMfgPad.GpioConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "End ConfigureGpio() for BootMode Detection.\n"));
  return Status;
}

/**

    Checks whether the MDF jumper has been set

    @param None

    @retval ManufacturingMode - TRUE when MFG jumper is on, FALSE otherwise

**/
BOOLEAN
IsManufacturingMode (
  VOID
)
{

  BOOLEAN ManufacturingMode = TRUE;

  EFI_STATUS Status;
  UINT32 GpiValue;

  if (mGpioParams.GpioMfgPad.GpioPad == UNUSED_GPIO) {
    return FALSE;
  }

  Status = GpioConfigForMFGMode ();
  ASSERT_EFI_ERROR (Status);

  Status = GpioGetInputValue (mGpioParams.GpioMfgPad.GpioPad, &GpiValue);
  ASSERT_EFI_ERROR (Status);

  if (!GpiValue) {
    ManufacturingMode = FALSE;
  }
  return ManufacturingMode;

}


