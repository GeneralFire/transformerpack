/** @file
  This file contains VER3 specific GPIO information

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 -2020 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GpioLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/GpioPrivateLib.h>
#include <Library/GpioNativePads.h>
#include <Pins/GpioPinsVer3.h>
#include <Register/GpioRegs.h>
#include <Register/IshRegs.h>

#include "GpioNativePrivateLibInternal.h"

/**
  This function provides GPIO Native Pad for a given native function

  @param[in]  PadFunction            PadFunction for a specific native signal. Please refer to GpioNativePads.h

  @retval     NativePad              GPIO pad with encoded native function
**/
GPIO_NATIVE_PAD
GpioGetNativePadByFunction (
  IN  UINT32                      PadFunction
  )
{
  switch (PadFunction) {
    case GPIO_FUNCTION_SERIAL_IO_UART_RX(0):    return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B12,          1, PadFunction);
    case GPIO_FUNCTION_SERIAL_IO_UART_RX(1):    return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B16,          1, PadFunction);
    case GPIO_FUNCTION_SERIAL_IO_UART_TX(0):    return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B13,          1, PadFunction);
    case GPIO_FUNCTION_SERIAL_IO_UART_TX(1):    return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B17,          1, PadFunction);
    case GPIO_FUNCTION_SERIAL_IO_UART_RTS(0):   return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B14,          1, PadFunction);
    case GPIO_FUNCTION_SERIAL_IO_UART_RTS(1):   return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B18,          1, PadFunction);
    case GPIO_FUNCTION_SERIAL_IO_UART_CTS(0):   return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B15,          1, PadFunction);
    case GPIO_FUNCTION_SERIAL_IO_UART_CTS(1):   return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_B19,          1, PadFunction);
    case GPIO_FUNCTION_HDA_BCLK:                return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_I12,          2, PadFunction);
    case GPIO_FUNCTION_HDA_RSTB:                return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_I13,          2, PadFunction);
    case GPIO_FUNCTION_HDA_SYNC:                return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_I14,          2, PadFunction);
    case GPIO_FUNCTION_HDA_SDO:                 return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_I15,          2, PadFunction);
    case GPIO_FUNCTION_HDA_SDI_0:               return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_I16,          2, PadFunction);
    case GPIO_FUNCTION_HDA_SDI_1:               return GPIO_NATIVE_PAD_DEF (GPIO_VER3_GPP_I17,          2, PadFunction);
    default: break;
  }
  return GPIO_PAD_NONE;
}

/**
  This function provides recommended GPIO IO Standby configuration for a given native function

  @param[in]  PadFunction            PadFunction for a specific native signal. Please refer to GpioNativePads.h
  @param[out] StandbyState           IO Standby State for specified native function
  @param[out] StandbyTerm            IO Standby Termination for specified native function

  @retval Status
**/
EFI_STATUS
GpioGetFunctionIoStandbyConfig (
  IN  UINT32                PadFunction,
  OUT GPIO_IOSTANDBY_STATE  *StandbyState,
  OUT GPIO_IOSTANDBY_TERM   *StandbyTerm
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This function provides SCS SD CARD detect pin

  @retval GpioPin             SD CARD Detect pin
**/
GPIO_PAD
GpioGetScsSdCardDetectPin (
  VOID
  )
{
  ASSERT (FALSE);
  return 0;
}

/**
  This function provides SSP/I2S interface pins

  @param[in]  SspInterfaceNumber       SSP/I2S interface

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetHdaSspPins (
  IN  UINT32                      SspInterfaceNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  ASSERT (FALSE);
}

/**
  This function sets HDA SSP Master Clock into native mode

  @param[in]  MclkIndex       MCLK index

  @retval Status
**/
EFI_STATUS
GpioEnableHdaSspMasterClock (
  IN UINT32  MclkIndex
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

/**
  This function provides SNDW interface pins

  @param[in]  SndwInterfaceNumber      SNDWx interface number

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetHdaSndwPins (
  IN  UINT32                      SndwInterfaceNumber,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  ASSERT (FALSE);
}

/**
  This function provides SPI IO pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[in]  IoIndex                   IoIndex Valid from 0 (SPI_IO_0) to 3 (SPI_IO_3)

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex or IoIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiIo (
  IN  UINT32                      SpiIndex,
  IN  UINT32                      IoIndex
  )
{
  ASSERT (FALSE);
  return (GPIO_PAD_NATIVE_FUNCTION){0};
}

/**
  This function provides SPI ChipSelect pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiCs (
  IN  UINT32                      SpiIndex
  )
{
  ASSERT (FALSE);
  return (GPIO_PAD_NATIVE_FUNCTION){0};
}

/**
  This function provides SPI Clock pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiClk (
  IN  UINT32                      SpiIndex
  )
{
  ASSERT (FALSE);
  return (GPIO_PAD_NATIVE_FUNCTION){0};
}

/**
  This function provides SPI Reset pin for Touch Host Controller

  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     NativePin                 Native Pin Configuration, 0 if SpiIndex is invalid
**/
GPIO_PAD_NATIVE_FUNCTION
GpioGetThcSpiReset (
  IN  UINT32                      SpiIndex
  )
{
  ASSERT (FALSE);
  return (GPIO_PAD_NATIVE_FUNCTION){0};
}

//
// GPIO pins for SMBUS
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchSmbusGpio[PCH_GPIO_SMBUS_NUMBER_OF_PINS] =
{
  {GPIO_VER3_GPP_D0, GpioPadModeNative1},// SMB_CLK
  {GPIO_VER3_GPP_D1, GpioPadModeNative1} // SMB_DATA
};

/**
  This function provides SMBUS interface pins

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetSmbusPins (
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  *NativePinsTable = mPchSmbusGpio;
}

//
// SMBUS Alert pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchSmbusAlertGpio = {GPIO_VER3_GPP_D2,  GpioPadModeNative1};
/**
  This function sets SMBUS ALERT pin into native mode

  @retval Status
**/
EFI_STATUS
GpioEnableSmbusAlert (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION SmbusAlertGpio;
  SmbusAlertGpio = mPchSmbusAlertGpio;

  return GpioSetPadMode (SmbusAlertGpio.Pad, SmbusAlertGpio.Mode);
}

//
// Sata SGPIO pins
//
GLOBAL_REMOVE_IF_UNREFERENCED SGPIO_PINS mPchSataSGpio[] =
{
  {{GPIO_VER3_GPP_E8, GpioPadModeNative1},  {GPIO_VER3_GPP_E9, GpioPadModeNative1},  {GPIO_VER3_GPP_E10, GpioPadModeNative1}},
  {{GPIO_VER3_GPP_E11, GpioPadModeNative1}, {GPIO_VER3_GPP_E12, GpioPadModeNative1}, {GPIO_VER3_GPP_E13, GpioPadModeNative1}},
  {{GPIO_VER3_GPP_E14, GpioPadModeNative1}, {GPIO_VER3_GPP_E15, GpioPadModeNative1}, {GPIO_VER3_GPP_E16, GpioPadModeNative1}}
};

/**
  This function provides Serial GPIO pins

  @param[in]  SataCtrlIndex       SATA controller index
  @param[out] SgpioPins           SATA Serial GPIO pins
**/
VOID
GpioGetSataSgpioPins (
  IN  UINT32        SataCtrlIndex,
  OUT SGPIO_PINS    *SgpioPins
  )
{
  GPIO_PAD_NATIVE_FUNCTION TempGpioPadNativeFunction = {0, 0};

  if (SataCtrlIndex >= MaxSataControllerNum ()) {
    SgpioPins->Sclock = TempGpioPadNativeFunction;
    SgpioPins->Sload = TempGpioPadNativeFunction;
    SgpioPins->Sdataout = TempGpioPadNativeFunction;
    ASSERT (FALSE);
    return;
  }
  *SgpioPins = mPchSataSGpio[SataCtrlIndex];
}

/**
  This function provides SATA DevSlp pin data

  In EBG SATA controller DevSlp signals have been aggregated to single GPIO pin.
  DevSlp signals for 8 SATA controller ports are connected to "OR" gate.
  As result of this we have one DevSlp GPIO pin per controller.

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
  GPIO_PAD_NATIVE_FUNCTION TempGpioPadNativeFunction = {0, 0};
  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    NativePin->Pad = GPIO_VER3_GPP_E9;
    NativePin->Mode = GpioPadModeNative2;
    return;
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    NativePin->Pad = GPIO_VER3_GPP_E13;
    NativePin->Mode = GpioPadModeNative2;
    return;
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    NativePin->Pad = GPIO_VER3_GPP_E16;
    NativePin->Mode = GpioPadModeNative2;
    return;
  }
  *NativePin = TempGpioPadNativeFunction;
  ASSERT (FALSE);
}

/**
  This function provides SATA GP pin data

  In EBG SATA controller GP signals have been aggregated to single GPIO pin.
  GP signals for 8 SATA controller ports are connected to "OR" gate.
  As result of this we have one GP GPIO pin per controller.

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
  GPIO_PAD_NATIVE_FUNCTION TempGpioPadNativeFunction = {0, 0};

  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    NativePin->Pad = GPIO_VER3_GPP_E10;
    NativePin->Mode = GpioPadModeNative2;
    return;
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    NativePin->Pad = GPIO_VER3_GPP_E12;
    NativePin->Mode = GpioPadModeNative2;
    return;
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    NativePin->Pad = GPIO_VER3_GPP_E15;
    NativePin->Mode = GpioPadModeNative2;
    return;
  }
  *NativePin = TempGpioPadNativeFunction;
  ASSERT (FALSE);
}

//
// SATA LED pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchSata1LedGpio = {GPIO_VER3_GPP_E8,  GpioPadModeNative2};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchSata2LedGpio = {GPIO_VER3_GPP_E11, GpioPadModeNative2};
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchSata3LedGpio = {GPIO_VER3_GPP_E14, GpioPadModeNative2};

/**
  This function sets SATA LED pin into native mode. SATA LED indicates
  SATA controller activity
  @param[in]  SataCtrlIndex       SATA controller index
  @retval     Status
**/
EFI_STATUS
GpioEnableSataLed (
  IN  UINT32                    SataCtrlIndex
  )
{
  GPIO_PAD_NATIVE_FUNCTION SataLedGpio = {0, 0};
  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    SataLedGpio = mPchSata1LedGpio;
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    SataLedGpio = mPchSata2LedGpio;
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    SataLedGpio = mPchSata3LedGpio;
  } else {
    ASSERT (FALSE);
  }
  return GpioSetPadMode (SataLedGpio.Pad, SataLedGpio.Mode);
}

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchUsbOcGpioPins[] =
{
  {GPIO_VER3_GPP_B5,  GpioPadModeNative1},// USB_OC_0
  {GPIO_VER3_GPP_B6,  GpioPadModeNative1},// USB_OC_1
  {GPIO_VER3_GPP_B7,  GpioPadModeNative1},// USB_OC_2
  {GPIO_VER3_GPP_B8,  GpioPadModeNative1},// USB_OC_3
  {GPIO_VER3_GPP_B9,  GpioPadModeNative1},// USB_OC_4
  {GPIO_VER3_GPP_B10, GpioPadModeNative1},// USB_OC_5
  {GPIO_VER3_GPP_B11, GpioPadModeNative1},// USB_OC_6
  {GPIO_VER3_GPP_D22, GpioPadModeNative1} // USB_OC_7
};

/**
  This procedure will set Native Function IOSF-SB Virtual Wire Message Generation bit
  in DW0 of requested GPIO Pad

  @param[in] GPIO_PAD   GpioPad
**/
VOID
GpioSetNafVweBit (
  IN CONST GPIO_PAD PadCfg
  )
{
  return;
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
  GPIO_PAD_NATIVE_FUNCTION  OcGpio;

  if (OcPinNumber >= ARRAY_SIZE (mPchUsbOcGpioPins)) {
    ASSERT(FALSE);
    return EFI_UNSUPPORTED;
  }
  OcGpio = mPchUsbOcGpioPins[OcPinNumber];
  return GpioSetPadMode (OcGpio.Pad, OcGpio.Mode);
}

//
// GPIO pin for PCIE SRCCLKREQB
// SRCCLKREQB_x -> GPIO pin y
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchPcieSrcClkReqbPinToGpioMap[] =
{
  {GPIO_VER3_GPP_A10, GpioPadModeNative1},
  {GPIO_VER3_GPP_A11, GpioPadModeNative1},
  {GPIO_VER3_GPP_A12, GpioPadModeNative1},
  {GPIO_VER3_GPP_A13, GpioPadModeNative1},
  {GPIO_VER3_GPP_A14, GpioPadModeNative1},
  {GPIO_VER3_GPP_A15, GpioPadModeNative1},
  {GPIO_VER3_GPP_A16, GpioPadModeNative1},
  {GPIO_VER3_GPP_A17, GpioPadModeNative1},
  {GPIO_VER3_GPP_A18, GpioPadModeNative1},
  {GPIO_VER3_GPP_A19, GpioPadModeNative1}
};

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
  GPIO_PAD_NATIVE_FUNCTION TempGpioPadNativeFunction = {0, 0};
  if (ClkreqIndex < ARRAY_SIZE (mPchPcieSrcClkReqbPinToGpioMap)) {
    *NativePin = mPchPcieSrcClkReqbPinToGpioMap[ClkreqIndex];
    return;
  }
  *NativePin = TempGpioPadNativeFunction;
  ASSERT (FALSE);
}
//
// PCHHOTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchPchHotbPin = {GPIO_VER3_GPP_D12,  GpioPadModeNative1};

/**
  This function sets PCHHOT pin into native mode

  @retval Status
**/
EFI_STATUS
GpioEnablePchHot (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION PchHotbPin;

  PchHotbPin = mPchPchHotbPin;

  return GpioSetPadMode (PchHotbPin.Pad, PchHotbPin.Mode);
}

//
// VRALERTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchVrAlertbPin = {GPIO_VER3_GPP_D15, GpioPadModeNative1};
/**
  This function sets VRALERTB pin into native mode

  @retval Status
**/
EFI_STATUS
GpioEnableVrAlert (
  VOID
  )
{
  GPIO_PAD_NATIVE_FUNCTION  VrAlertGpio;
  VrAlertGpio = mPchVrAlertbPin;
  return GpioSetPadMode (VrAlertGpio.Pad, VrAlertGpio.Mode);
}

//
// CPU_C10_GATE pin
//
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

//
// CPU GP pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchCpuGpPinMap[PCH_GPIO_CPU_GP_NUMBER_OF_PINS] =
{
  {GPIO_VER3_GPPC_S2, GpioPadModeNative1}, // CPU_GP_0
  {GPIO_VER3_GPPC_S3, GpioPadModeNative1}, // CPU_GP_1
  {GPIO_VER3_GPPC_S4, GpioPadModeNative1}, // CPU_GP_2
  {GPIO_VER3_GPPC_S5, GpioPadModeNative1}  // CPU_GP_3
};

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
  GPIO_PAD_NATIVE_FUNCTION CpuGpPin;

  if (CpuGpPinNum >= ARRAY_SIZE (mPchCpuGpPinMap)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  CpuGpPin = mPchCpuGpPinMap[CpuGpPinNum];

  return GpioSetPadMode (CpuGpPin.Pad, CpuGpPin.Mode);
}

//
// DDP1/2/3/4/A/B/C  CTRLCLK and CTRLDATA pins
//

/**
  This function provides DDPx interface pins

  @param[in]  DdpInterface   DDPx interface

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetDdpPins (
  IN  GPIO_DDP                    DdpInterface,
  OUT GPIO_PAD_NATIVE_FUNCTION    **NativePinsTable
  )
{
  ASSERT(FALSE);
}

/**
  This function provides CNVi BT interface select pin

  @retval GpioPad          GPIO pad for CNVi BT interface select
**/
GPIO_PAD
GpioGetCnviBtIfSelectPin (
  VOID
  )
{
  ASSERT (FALSE);
  return 0;
}

/**
  This function provides CNVi BT Charging pin

  @retval GpioPad          GPIO pad for CNVi BT Charging select
**/
GPIO_PAD
GpioGetCnviBtChargingPin (
  VOID
  )
{
  ASSERT (FALSE);
  return 0;
}

/**
  This function provides CNVi A4WP pin

  @param[out] GpioNativePad       GPIO native pad for CNVi A4WP
**/
VOID
GpioGetCnviA4WpPin (
  OUT GPIO_PAD_NATIVE_FUNCTION  *GpioNativePad
  )
{
  ASSERT (FALSE);
  *GpioNativePad = (GPIO_PAD_NATIVE_FUNCTION){0};
}

/**
  This function provides CNVi BT host wake int pin

  @retval GpioPad          GPIO pad BT host wake int
**/
GPIO_PAD
GpioGetCnviBtHostWakeIntPin (
  VOID
  )
{
  ASSERT (FALSE);
  return 0;
}

//
// TBT LSX 0/1/2/3  RX and TX pins
//

/**
  This function sets TBT_LSx pin into native mode

  @param[in]  TbtLsxDdiPort     TBT_LSx for DDI Port Number

  @retval     Status
**/
EFI_STATUS
GpioEnableTbtLsxInterface (
  IN GPIO_TBT_LSX  TbtLsxDdiPort
  )
{
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

//
// CNVi Bluetooth UART pads
//

/**
  This function provides CNVi BT UART pins

  @param[in]  ConnectionType           CNVi BT UART connection type
  @param[out] VCnviBtUartPad           Table with vCNV_BT_UARTx pads
  @param[out] VCnviBtUartPadMode       vCNV_BT_UARTx pad mode
  @param[out] VUartForCnviBtPad        Table with vUART0 pads
  @param[out] VUartForCnviBtPadMode    vUART0 pad mode
**/
VOID
GpioGetCnviBtUartPins (
  IN  VGPIO_CNVI_BT_UART_CONNECTION_TYPE  ConnectionType,
  OUT GPIO_PAD                            **VCnviBtUartPad,
  OUT GPIO_PAD_MODE                       *VCnviBtUartPadMode,
  OUT GPIO_PAD                            **VUartForCnviBtPad,
  OUT GPIO_PAD_MODE                       *VUartForCnviBtPadMode
  )
{
  ASSERT (FALSE);
  return;
}

/**
  This function provides CNVi BT UART external pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnviBtUartExternalPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  ASSERT (FALSE);
  return;
}

//
// CNVi Bluetooth I2S pads
//
/**
  This function provides CNVi BT I2S pins

  @param[in]  ConnectionType          CNVi BT I2S connection type
  @param[out] VCnviBtI2sPad           Table with vCNV_BT_I2Sx pads
  @param[out] VCnviBtI2sPadMode       vCNV_BT_I2Sx pad mode
  @param[out] VSspForCnviBtPad        Table with vSSP2 pads
  @param[out] VSspForCnviBtPadMode    vSSP2 pad mode
**/
VOID
GpioGetCnviBtI2sPins (
  IN  VGPIO_CNVI_BT_I2S_CONNECTION_TYPE  ConnectionType,
  OUT GPIO_PAD                 **VCnviBtI2sPad,
  OUT GPIO_PAD_MODE            *VCnviBtI2sPadMode,
  OUT GPIO_PAD                 **VSspForCnviBtPad,
  OUT GPIO_PAD_MODE            *VSspForCnviBtPadMode
  )
{
  ASSERT (FALSE);
  return;
}

/**
  This function provides CNVi BT I2S external pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnviBtI2sExternalPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  ASSERT (FALSE);
  return;
}

//
// CNVi MFUART1 pads
//

/**
  This function provides CNVi MFUART1 pins

  @param[in]  ConnectionType          CNVi MFUART1 connection type
  @param[out] VCnviBtI2sPad           Table with vCNV_MFUART1x pads
  @param[out] VCnviBtI2sPadMode       vCNV_MFUART1x pad mode
  @param[out] VSspForCnviBtPad        Table with vISH_UART0 pads
  @param[out] VSspForCnviBtPadMode    vISH_UART0 pad mode
**/
VOID
GpioGetCnviMfUart1Pins (
  IN  VGPIO_CNVI_MF_UART1_CONNECTION_TYPE  ConnectionType,
  OUT GPIO_PAD                 **VCnviMfUart1Pad,
  OUT GPIO_PAD_MODE            *VCnviMfUart1PadMode,
  OUT GPIO_PAD                 **VUartForCnviMfUart1Pad,
  OUT GPIO_PAD_MODE            *VUartForCnviMfUart1PadMode
  )
{
  ASSERT (FALSE);
  return;
}

/**
  This function provides CNVi MFUART1 external pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnviMfUart1ExternalPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  ASSERT (FALSE);
  return;
}

/**
  This function provides CNVi Bluetooth Enable pad

  @retval GpioPad           CNVi Bluetooth Enable pad
**/
GPIO_PAD
GpioGetCnviBtEnablePin (
  VOID
  )
{
  ASSERT (FALSE);
  return 0;
}

//
// CNVi BRI(Bluetooth Radio Interface) and RGI(Radio Generic Interface) buses from PCH to CRF(Companion RF) module
//

/**
  This function provides CNVi BRI RGI GPIO pads

  @param[out] NativePinsTable          Table with pins
**/
VOID
GpioGetCnvBriRgiPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable
  )
{
  ASSERT (FALSE);
  return;
}


/**
  This function provides IMGCLKOUT pins

  @param[out] NativePinsTable          Table with pins
  @param[out] NoOfNativePins            Number of pins
**/
VOID
GpioGetImgClkOutPins (
  OUT GPIO_PAD_NATIVE_FUNCTION **NativePinsTable,
  OUT UINT32                   *NoOfNativePins
  )
{
  ASSERT (FALSE);
  return;
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

  return GPIO_VER3_GPP_O3;
}

/**
  The function sets VCCIOSEL

  @param[in]  GpioPad             GPIO pad
  @param[in]  VccioSel            Pad voltage

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         The Pin is owned by others
  @retval EFI_INVALID_PARAMETER   Invalid group or parameter
**/
EFI_STATUS
GpioSetVccLevel (
  IN  GPIO_PAD        GpioPad,
  IN  GPIO_VCCIO_SEL  VccioSel
  )
{
  if (VccioSel >= MaxVccioSel) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioWritePadCfgReg (
    GpioPad,
    2,
    (UINT32)~B_GPIO_PCR_VCCIOSEL,
    VccioSel << N_GPIO_PCR_VCCIOSEL
    );
  return EFI_SUCCESS;
}

/**
  SBU (Sideband use) pins are used as auxiliary signals for Type C connector,
  which are hard-wired to BSSB_LS natively for debug function.
  when USB-C is enablde and debug not needed, disable pins (BSSB) used for debug through TypeC connector,
  program SBU pins to high-Z/open circuit per USB-C spec.

  @param[in]  UsbTcPortEnBitmap   USB Type C port enabled bitmap

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         SBU pads are not supported
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
**/
EFI_STATUS
GpioDisableTypeCSbuDebug (
  IN UINT32           UsbTcPortEnBitmap
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

/**
  When 2-wire DCI OOB is connected via SBU from Type C port, need set IO Standby state to masked (to operate as if no standby signal asserted)
  to remain connection in low power state.

  @param[in]     DCI connection port ID

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         SBU pads are not supported
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
**/
EFI_STATUS
Gpio2WireDciOobSetting (
  IN UINT8               DciPortId
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get the pins used for VW CLKREQ messages between FIA and GPIO controller.

  @param[in]  PciePortIndex       Index of the CPU PCIe root port
  @param[out] VwBusPinsTable      Pointer to table describing VW bus GPIO pins
  @param[out] NoOfNativePins      Size of the table with VW bus GPIO pins

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Current GPIO controller doesn't support this functionality
  @retval EFI_INVALID_PARAMETER   Invalid group or parameter
**/
EFI_STATUS
GpioGetCpuPcieVwMsgBusPins (
  IN  UINT32                    PciePortIndex,
  OUT GPIO_PAD_NATIVE_FUNCTION  **VwBusPinsTable,
  OUT UINT32                    *NoOfNativePins
  )
{
  ASSERT (FALSE);
  return EFI_UNSUPPORTED;
}

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchTimeSyncToGpioMap[] =
{
  {GPIO_VER3_GPPC_S0,  GpioPadModeNative1},
  {GPIO_VER3_GPPC_S1,  GpioPadModeNative2}
};

/**
  This function sets Time Sync Gpio into native mode

  @param[in]  Index       index

  @retval Status
**/
EFI_STATUS
GpioEnableTimeSync (
  IN UINT32  Index
  )
{
  EFI_STATUS                Status;
  GPIO_PAD_NATIVE_FUNCTION  *TimeSyncToGpioMap;
  UINT32                    TimeSyncToGpioMapSize;

  TimeSyncToGpioMap = mPchTimeSyncToGpioMap;
  TimeSyncToGpioMapSize = ARRAY_SIZE (mPchTimeSyncToGpioMap);

  if (Index < TimeSyncToGpioMapSize) {
    Status = GpioSetPadMode (TimeSyncToGpioMap[Index].Pad, TimeSyncToGpioMap[Index].Mode);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  This function is to be used In GpioLockPads() to override a lock request by SOC code.

  @param[in]  Group               GPIO group
  @param[in]  DwNum               Register number for current group (parameter applicable in accessing whole register).
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] *UnlockCfgPad       DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: to be locked, 1: Leave unlocked
  @param[out] *UnlockTxPad        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: to be locked, 1: Leave unlocked

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
**/
EFI_STATUS
GpioUnlockOverride (
  IN  GPIO_GROUP  Group,
  IN  UINT32      DwNum,
  OUT UINT32      *UnlockCfgPad,
  OUT UINT32      *UnlockTxPad
  )
{

  if ((UnlockCfgPad == NULL) || (UnlockTxPad == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *UnlockCfgPad = 0;
  *UnlockTxPad = 0;
  return EFI_SUCCESS;
}

/**
  Checks if GPIO PinMux corresponds to I2C4 B

  @param[in] SdaPinMux    GPIO pad pinmux for SDA
  @param[in] SclPinMux    GPIO pad pinmux for SCL

  @retval TRUE         PinMux corresponds to I2C4 B
          FALSE        PinMux equals to I2C4 A
**/
EFI_STATUS
GpioIsSerialIoI2c4bMuxed (
  IN UINT32  SdaPinMux,
  IN UINT32  SclPinMux
  )
{
  // @todo add support for Ver3
  return FALSE;
}