/** @file
  This file contains GPIO routines for RC usage

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Library/PeiDxeSmmGpioLib/GpioLibrary.h>
#include <Private/Library/GpioNameBufferLib.h>
#include <Private/GpioNativePads.h>
#include <Register/PchRegsPcr.h>

#include "GpioNativePrivateLibInternal.h"

/**
  This procedure is used to check if GpioPad is valid for certain chipset

  @param[in]  GpioPad             GPIO pad

  @retval TRUE                    This pin is valid on this chipset
          FALSE                   Incorrect pin
**/
BOOLEAN
GpioIsCorrectPadForThisChipset (
  IN  GPIO_PAD        GpioPad
  )
{
  return FALSE;
}


/**
  This procedure will get value of selected gpio register

  @param[in]  Group               GPIO group number
  @param[in]  Offset              GPIO register offset
  @param[out] RegVal              Value of gpio register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetReg (
  IN  GPIO_GROUP              Group,
  IN  UINT32                  Offset,
  OUT UINT32                  *RegVal
  )
{
  return EFI_INVALID_PARAMETER;
}


/**
  This procedure will set value of selected gpio register

  @param[in] Group               GPIO group number
  @param[in] Offset              GPIO register offset
  @param[in] RegVal              Value of gpio register

  @retval EFI_SUCCESS            The function completed successfully
  @retval EFI_INVALID_PARAMETER  Invalid group or pad number
**/
EFI_STATUS
GpioSetReg (
  IN GPIO_GROUP              Group,
  IN UINT32                  Offset,
  IN UINT32                  RegVal
  )
{
  return EFI_INVALID_PARAMETER;
}

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI.

  @param[in]  Index                   GPI SMI number
  @param[out] GpioPin                 GPIO pin
  @param[out] GpiSmiBitOffset         GPI SMI bit position within GpiSmi Registers
  @param[out] GpiHostSwOwnRegAddress  Address of HOSTSW_OWN register
  @param[out] GpiSmiStsRegAddress     Address of GPI SMI status register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadAndSmiRegs (
  IN UINT32            Index,
  OUT GPIO_PAD         *GpioPin,
  OUT UINT8            *GpiSmiBitOffset,
  OUT UINT32           *GpiHostSwOwnRegAddress,
  OUT UINT32           *GpiSmiStsRegAddress
  )
{
  return EFI_INVALID_PARAMETER;
}


/**
  This procedure will set GPIO Driver IRQ number

  @param[in]  Irq                 Irq number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid IRQ number
**/
EFI_STATUS
GpioSetIrq (
  IN  UINT8          Irq
  )
{
  return EFI_INVALID_PARAMETER;
}


/**
  This procedure will return Port ID of GPIO Community from GpioPad

  @param[in] GpioPad            GpioPad

  @retval GpioCommunityPortId   Port ID of GPIO Community
**/
UINT8
GpioGetGpioCommunityPortIdFromGpioPad (
  IN GPIO_PAD        GpioPad
  )
{
  return 0;
}


/**
  This procedure will return PadCfg address from GpioPad

  @param[in] GpioPad            GpioPad

  @retval GpioPadCfgAddress     PadCfg Address of GpioPad
**/
UINT32
GpioGetGpioPadCfgAddressFromGpioPad (
  IN GPIO_PAD        GpioPad
  )
{
  return 0;
}



/**
  This procedure will check if GpioPad is owned by host.

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is owned by host
  @retval FALSE            GPIO pad is not owned by host and should not be used with GPIO lib API
**/
BOOLEAN
GpioIsPadHostOwned (
  IN GPIO_PAD             GpioPad
  )
{
  return FALSE;
}


/**
  This procedure will check if GpioPad argument is valid.
  Function will check below conditions:
   - GpioPad represents a pad for current PCH
   - GpioPad belongs to valid GpioGroup
   - GPIO PadNumber is not greater than number of pads for this group

  @param[in] GpioPad       GPIO pad

  @retval TRUE             GPIO pad is valid and can be used with GPIO lib API
  @retval FALSE            GPIO pad is invalid and cannot be used with GPIO lib API
**/
BOOLEAN
GpioIsPadValid (
  IN GPIO_PAD             GpioPad
  )
{
  return FALSE;
}


/**
  This procedure will read GPIO Pad Configuration register

  @param[in] GpioPad          GPIO pad
  @param[in] DwReg            Choose PADCFG register: 0:DW0, 1:DW1

  @retval PadCfgRegValue      PADCFG_DWx value
**/
UINT32
GpioReadPadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg
  )
{
  return 0;
}


/**
  This procedure will write or read GPIO Pad Configuration register

  @param[in] GpioPad              GPIO pad
  @param[in] DwReg                Choose PADCFG register: 0:DW0, 1:DW1
  @param[in] PadCfgAndMask        Mask to be AND'ed with PADCFG reg value
  @param[in] PadCfgOrMask         Mask to be OR'ed with PADCFG reg value

  @retval none
**/
VOID
GpioWritePadCfgReg (
  IN GPIO_PAD             GpioPad,
  IN UINT8                DwReg,
  IN UINT32               PadCfgAndMask,
  IN UINT32               PadCfgOrMask
  )
{

}

/**
  This procedure will set GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[in]  PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetPadMode (
  IN GPIO_PAD                GpioPad,
  IN GPIO_PAD_MODE           PadModeValue
  )
{
  return EFI_INVALID_PARAMETER;
}


/**
  This procedure will set GPIO pad to native mode.
  To be used if no other settings are to be configured when enabling native mode.

  @param[in]  GpioNativePad       GPIO Pad with native mode information

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetNativePad (
  IN GPIO_NATIVE_PAD  GpioNativePad
  )
{
  return EFI_INVALID_PARAMETER;
}


/**
  This procedure will get GPIO native pad based on provided native function and platform muxing selection (if needed).

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioNativePads.h
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval  NativePad              GPIO pad with encoded native function
**/
GPIO_NATIVE_PAD
GpioGetNativePadByFunctionAndPinMux (
  IN UINT32  PadFunction,
  IN UINT32  PinMux
  )
{
  return 0;
}


/**
  This procedure will set GPIO pad to native function based on provided native function
  and platform muxing selection (if needed).

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioNativePads.h
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioSetNativePadByFunction (
  IN UINT32  PadFunction,
  IN UINT32  PinMux
  )
{
  return EFI_INVALID_PARAMETER;
}

/**
  This procedure will get GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadMode (
  IN  GPIO_PAD                 GpioPad,
  OUT GPIO_PAD_MODE            *PadModeValue
  )
{
  return EFI_INVALID_PARAMETER;
}


/**
  The function performs GPIO Power Management programming.
**/
VOID
GpioConfigurePm (
  VOID
  )
{

}

/**
  This procedure is used to unlock all GPIO pads.
  This function can only be called when platform is still in HOSTIA_BOOT_SAI.
**/
VOID
GpioUnlockAllPads (
  VOID
  )
{

}

/**
  This procedure is used to set termination for all GPIO pads owned by host,
  except eMMC.
**/
VOID
GpioSetAllPadsTermination (
  VOID
  )
{

}

/**
  Generates GPIO name from GpioPad
  This function returns pointer to the static buffer

  @param[in] GpioPad  GpioPad

  @retval CHAR8*  Pointer to the gpio name string
**/
CHAR8*
GpioName (
  IN GPIO_PAD  GpioPad
  )
{
  return NULL;
}

/**
  Generates GPIO name from GpioNativePad
  This function returns pointer to the static buffer.

  @param[in] GpioNativePad  GpioNativePad

  @retval CHAR8*  Pointer to the GPIO name
**/
CHAR8*
GpioPinMuxName (
  IN GPIO_NATIVE_PAD  GpioNativePad
  )
{
  return NULL;
}

//
// For GPIO debounce feature glitch filter clock is used
// which in CNL is driven by RTC clock with f = 32kHz (T = 31.25us)
//
#define GPIO_DEB_CLK_PERIOD_IN_NS  31250

/**
  This procedure enables debounce feature on a selected pad configured in input mode
  Debounce time can be specified in microseconds. GPIO HW supports only certain values
  according to below formula:
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
  RTC clock with f = 32 KHz is used for glitch filter.
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
  Supported DebounceTime values are following:
   DebounceTime = 0 -> Debounce feature disabled
   DebounceTime > 0 && < 250us -> Not supported
   DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
  For values not supported by GPIO HW, function will round down
  to closest supported

  @param[in] GpioPad              GPIO pad
  @param[in, out] DebounceTime    Debounce Time in microseconds
                                  If Debounce Time = 0, Debouncer feature will be disabled
                                  Function will set DebounceTime argument to rounded supported value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad or unsupported DebounceDuration value
  @retval EFI_UNSUPPORTED         GpioPad is not owned by host
**/
EFI_STATUS
GpioSetDebounceTimer (
  IN GPIO_PAD                  GpioPad,
  IN OUT UINT32                *DebounceTime
  )
{
  return EFI_INVALID_PARAMETER;
}

