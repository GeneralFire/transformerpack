/** @file
  This file contains routines for GPIO native and chipset specific purpose
  used by Reference Code only.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Private/GpioNativePads.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Library/SataSocLib.h>
#include "GpioNativePrivateLibInternal.h"
#include <Register/PchRegsGpio.h>
#include <Register/PchRegsSerialIo.h>
#include <Register/PchRegsIsh.h>
/**
  This function sets SCS eMMC controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableScsEmmc (
  VOID
  )
{
  EFI_STATUS                Status;
  UINTN                     Index;
  GPIO_PAD_NATIVE_FUNCTION  *EmmcGpio;
  UINT32                    NumOfEmmcPins;

  GpioGetScsEmmcPins (
    &EmmcGpio,
    &NumOfEmmcPins
    );

  if (EmmcGpio == NULL) {
    return EFI_UNSUPPORTED;
  }

  for (Index = 0; Index < NumOfEmmcPins; Index++) {
    Status = GpioSetPadMode (EmmcGpio[Index].Pad, EmmcGpio[Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }
  return EFI_SUCCESS;
}

/**
  This function sets SMBUS controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSmbus (
  VOID
  )
{
  EFI_STATUS               Status;
  UINTN                    Index;
  GPIO_PAD_NATIVE_FUNCTION *SmbusGpio;

  GpioGetSmbusPins (&SmbusGpio);

  if (SmbusGpio == NULL) {
    return EFI_UNSUPPORTED;
  }

  for (Index = 0; Index < PCH_GPIO_SMBUS_NUMBER_OF_PINS; Index++) {
    Status = GpioSetPadMode (SmbusGpio[Index].Pad, SmbusGpio[Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}

/**
  This function sets SATA DevSlp pins into native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number

  @retval Status
**/
EFI_STATUS
GpioEnableSataDevSlpPin (
  IN  UINT32  SataCtrlIndex,
  IN  UINTN   SataPort
  )
{
  GPIO_PAD_NATIVE_FUNCTION  DevSlpGpio;

  GpioGetSataDevSlpPin (
    SataCtrlIndex,
    SataPort,
    &DevSlpGpio
    );

  GpioSetPadResetConfig (DevSlpGpio.Pad, GpioResumeReset);

  return GpioSetPadMode (DevSlpGpio.Pad, DevSlpGpio.Mode);
}

/**
  This function checks if SataDevSlp pin is in native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port
  @param[out] DevSlpPad           DevSlpPad
                                  This is an optional parameter and may be NULL.

  @retval TRUE                    DevSlp is in native mode
          FALSE                   DevSlp is not in native mode
**/
BOOLEAN
GpioIsSataDevSlpPinEnabled (
  IN  UINT32          SataCtrlIndex,
  IN  UINTN           SataPort,
  OUT GPIO_PAD        *DevSlpPad  OPTIONAL
  )
{
  GPIO_PAD_NATIVE_FUNCTION  DevSlpNativePad;
  GPIO_PAD_MODE             GpioMode;
  EFI_STATUS                Status;

  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  GpioGetSataDevSlpPin (
    SataCtrlIndex,
    SataPort,
    &DevSlpNativePad
    );

  Status = GpioGetPadMode (DevSlpNativePad.Pad, &GpioMode);

  if (EFI_ERROR (Status) || (GpioMode != DevSlpNativePad.Mode)) {
    if (DevSlpPad != NULL) {
      *DevSlpPad = GPIO_PAD_NONE;
    }
    return FALSE;
  } else {
    if (DevSlpPad != NULL) {
      *DevSlpPad = DevSlpNativePad.Pad;
    }
    return TRUE;
  }
}

/**
  This function sets SATAGPx pin into native mode

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number

  @retval Status
**/
EFI_STATUS
GpioEnableSataGpPin (
  IN  UINT32  SataCtrlIndex,
  IN  UINTN   SataPort
  )
{
  GPIO_PAD_NATIVE_FUNCTION  SataGpGpio;

  GpioGetSataGpPin (
    SataCtrlIndex,
    SataPort,
    &SataGpGpio
    );

  DEBUG_CODE_BEGIN ();
  GPIO_PAD_MODE  PadMode;
  GpioGetPadMode (SataGpGpio.Pad, &PadMode);
  if (PadMode == GpioPadModeNative2) {
    DEBUG ((DEBUG_ERROR, "GPIO ERROR: Cannot enable SATAGP%d, %a already used for SATA%d_SLD\n",
        SataPort,
        GpioName (SataGpGpio.Pad),
        SataPort));
    return EFI_UNSUPPORTED;
  }
  DEBUG_CODE_END ();

  return GpioSetPadMode (SataGpGpio.Pad, SataGpGpio.Mode);
}

/**
  Returns a pad for given CLKREQ# index.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return CLKREQ# pad.
**/
GPIO_PAD
GpioGetClkreqPad (
  IN     UINT32   ClkreqIndex
  )
{
  GPIO_PAD_NATIVE_FUNCTION  ClkReqGpio;

  GpioGetPcieClkReqPin (
    ClkreqIndex,
    &ClkReqGpio
    );

  return ClkReqGpio.Pad;
}

/**
  Enables CLKREQ# pad in native mode.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return none
**/
VOID
GpioEnableClkreq (
  IN     UINT32   ClkreqIndex
  )
{
  GPIO_CONFIG               PadConfig;
  GPIO_PAD_NATIVE_FUNCTION  ClkReqGpio;

  ZeroMem (&PadConfig, sizeof (PadConfig));

  GpioGetPcieClkReqPin (
    ClkreqIndex,
    &ClkReqGpio
    );

  PadConfig.PadMode      = ClkReqGpio.Mode;
  PadConfig.Direction    = GpioDirNone;
  PadConfig.PowerConfig  = GpioHostDeepReset;
  DEBUG ((DEBUG_INFO, "Enabling CLKREQ%d\n", ClkreqIndex));
  GpioSetPadConfig (ClkReqGpio.Pad, &PadConfig);
}
/**
  Power button debounce configuration
  Debounce time can be specified in microseconds. Only certain values according
  to below formula are supported:
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
  RTC clock with f = 32 KHz is used for glitch filter.
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
  Supported DebounceTime values are following:
   DebounceTime = 0 -> Debounce feature disabled
   DebounceTime > 0 && < 250us -> Not supported
   DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
  For values not supported by HW, they will be rounded down to closest supported one

  @param[in] DebounceTime    Debounce Time in microseconds
                             If Debounce Time = 0, Debouncer feature will be disabled
                             Function will set DebounceTime argument to rounded supported value
**/
VOID
GpioSetPwrBtnDebounceTimer (
  IN UINT32                DebounceTime
  )
{
  GpioSetDebounceTimer (GpioGetPwrBtnPin (), &DebounceTime);
}
