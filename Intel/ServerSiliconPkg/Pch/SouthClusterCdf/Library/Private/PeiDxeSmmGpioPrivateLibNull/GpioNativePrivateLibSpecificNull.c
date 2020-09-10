/** @file
  This file contains routines for GPIO native and chipset specific purpose
  used by Reference Code only.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Private/Library/GpioPrivateLib.h>
#include <Private/GpioNativePads.h>
#include <Library/PchInfoLib.h>
#include <Pch/SouthClusterCdf/Library/PeiDxeSmmGpioLib/GpioLibrary.h>
#include <GpioPinsCdf.h>

/**
  This function provides GPIO Native Pad for a given native function

  @param[in]  PadFunction            PadFunction for a specific native signal. Please refer to GpioNativePads.h

  @retval     NativePad              GPIO pad with encoded native function
**/
// TODO: need implementation for CDF
GPIO_NATIVE_PAD
GpioGetNativePadByFunction (
  IN  UINT32                      PadFunction
  )
{
  return GPIO_NATIVE_PAD_DEF (GPIO_CDF_GPP_A0, 1, PadFunction);
}

/**
  This function sets SCS SD card controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableScsSdCard (
  VOID
  )
{
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
}

/**
  This function enables SCS Sd Card controller card detect pin

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableScsSdCardDetect (
  VOID
  )
{
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
}


/**
  This function sets SMBUS ALERT pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSmbusAlert (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This function enables USB OverCurrent pins by setting
  USB2 OCB pins into native mode

  @param[in]  OcPinNumber            USB OC pin number

  @retval Status
**/
EFI_STATUS
GpioEnableUsbOverCurrent (
  IN  UINTN   OcPinNumber
  )
{
  return EFI_SUCCESS;
}

/**
  This function provides SATA GP pin data

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number
  @param[out] NativePin           SATA GP pin
**/
VOID
GpioGetSataGpPin (
  IN  UINT32                    SataCtrlIndex,
  IN  UINTN                     SataPort,
  OUT GPIO_PAD_NATIVE_FUNCTION  *NativePin
  )
{

}

/**
  This function sets SATA LED pin into native mode. SATA LED indicates
  SATA controller activity

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableSataLed (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This function sets PCHHOT pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnablePchHot (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This function sets VRALERTB pin into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioEnableVrAlert (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
This function sets CPU C10 Gate pins into native mode

@retval Status
**/
EFI_STATUS
GpioEnableCpuC10GatePin (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This function sets CPU GP pins into native mode

  @param[in]  CpuGpPinNum               CPU GP pin number

  @retval Status
**/
EFI_STATUS
GpioEnableCpuGpPin (
  IN  UINT32                            CpuGpPinNum
  )
{
  return EFI_SUCCESS;
}

/**
  This function sets EDP_HPD pin into native mode

  @retval Status
**/
EFI_STATUS
GpioEnableEdpHotPlugDetect (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This function sets DDP pins into native mode

  @param[in]  DdpInterface   DDPx interface

  @retval Status
**/
EFI_STATUS
GpioEnableDpInterface (
  IN  GPIO_DDP            DdpInterface
  )
{
  return EFI_SUCCESS;
}

/**
  This function provides SCS eMMC controller pins

  @param[out] NativePinsTable                Table with pins
  @param[out] NoOfNativePins                 Number of pins
**/
VOID
GpioGetScsEmmcPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable,
  OUT UINT32                      *NoOfNativePins
  )
{

}

/**
  This function provides SMBUS interface pins

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetSmbusPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{

}

/**
  This function provides SATA DevSlp pin data

  @param[in]  SataCtrlIndex       SATA controller index
  @param[in]  SataPort            SATA port number
  @param[out] NativePin           SATA DevSlp pin
**/
VOID
GpioGetSataDevSlpPin (
  IN  UINT32                    SataCtrlIndex,
  IN  UINTN                     SataPort,
  OUT GPIO_PAD_NATIVE_FUNCTION  *NativePin
  )
{

}

/**
  This function provides PCIe CLKREQ pin data

  @param[in]  ClkreqIndex        CLKREQ# number
  @param[out] NativePin          Native pin data
**/
VOID
GpioGetPcieClkReqPin (
  IN  UINT32                      ClkreqIndex,
  OUT GPIO_PAD_NATIVE_FUNCTION    *NativePin
  )
{

}

/**
  This function provides PWRBTN pin

  @retval GpioPad          PWRTBTN pin
**/
GPIO_PAD
GpioGetPwrBtnPin (
  VOID
  )
{
  //
  // Power Button pin in CDF in NOT GPIO pin
  //
  return 0;
}

/**
  This function is to be used In GpioLockPads() to override a lock request by SOC code.

  @param[in]  Group          GPIO group
  @param[in]  DwNum          Register number for current group (parameter applicable in accessing whole register).
                             For group which has less then 32 pads per group DwNum must be 0.
  @param[out] *UnlockCfgPad  DWORD bitmask for pads which are going to be left unlocked
                             Bit position - PadNumber
                             Bit value - 0: to be locked, 1: Leave unlocked
  @param[out] *UnlockTxPad   DWORD bitmask for pads which are going to be left unlocked
                             Bit position - PadNumber
                             Bit value - 0: to be locked, 1: Leave unlocked
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
GpioUnlockOverride (
  IN  GPIO_GROUP  Group,
  IN  UINT32      DwNum,
  OUT UINT32      *UnlockCfgPad,
  OUT UINT32      *UnlockTxPad
  )
{

  return EFI_SUCCESS;
}
