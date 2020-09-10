/** @file
  This file contains routines for GPIO native and chipset specific purpose
  used by Reference Code only.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation. <BR>

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

//
// SMBUS Alert pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSmbusAlertGpio = {GPIO_CDF_GPP_C2,  GpioPadModeNative1};

//
// SATA LED pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSataLedGpio = {GPIO_CDF_GPP_L14, GpioPadModeNative1};

//
// USB2 OC pins
//

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHUsbOcGpioPins[] =
{
  {GPIO_CDF_GPP_L0,  GpioPadModeNative1}//USB_OC_0
};
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
//
// GPIO pin for PCIE SCRCLKREQB
// SCRCLKREQB_x -> GPIO pin y
//

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHPcieSrcClkReqbPinToGpioMap[]  =
{
  {GPIO_CDF_GPP_B5,  GpioPadModeNative1},
  {GPIO_CDF_GPP_B6,  GpioPadModeNative1},
  {GPIO_CDF_GPP_B7,  GpioPadModeNative1},
  {GPIO_CDF_GPP_B8,  GpioPadModeNative1},
  {GPIO_CDF_GPP_B9,  GpioPadModeNative1},
  {GPIO_CDF_GPP_B10, GpioPadModeNative1},
  {GPIO_CDF_GPP_H0,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H1,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H2,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H3,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H4,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H5,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H6,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H7,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H8,  GpioPadModeNative1},
  {GPIO_CDF_GPP_H9,  GpioPadModeNative1}
};

//
// PCHHOTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHPchHotbPin = {GPIO_CDF_GPP_B13,  GpioPadModeNative1};

//
// VRALERTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHVrAlertbPin = {GPIO_CDF_GPP_B2, GpioPadModeNative1};

//
// CPU GP pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHCpuGpPinMap[PCH_GPIO_CPU_GP_NUMBER_OF_PINS] =
{
  {GPIO_CDF_GPP_C0, GpioPadModeNative1}, // CPU_GP_0
  {GPIO_CDF_GPP_C1, GpioPadModeNative1}, // CPU_GP_1
  {GPIO_CDF_GPP_C2, GpioPadModeNative1}, // CPU_GP_2
  {GPIO_CDF_GPP_C3, GpioPadModeNative1}, // CPU_GP_3
};

//
// DDSP_HPD pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHDdspHpdPins[PCH_GPIO_DDSP_HPD_NUMBER_OF_PINS] =
{
  {GPIO_CDF_GPP_I0, GpioPadModeNative1},//DDSP_HPD_0
  {GPIO_CDF_GPP_I1, GpioPadModeNative1},//DDSP_HPD_1
  {GPIO_CDF_GPP_I2, GpioPadModeNative1},//DDSP_HPD_2
  {GPIO_CDF_GPP_I3, GpioPadModeNative1} //DDSP_HPD_3
};

//
// EDP_HPD pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHEdpHpdPin = {GPIO_CDF_GPP_I4, GpioPadModeNative1};

//
// DDPB/C/D/F  CTRLCLK and CTRLDATA pins
//

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHDdpInterfacePins[PCH_GPIO_DDP_NUMBER_OF_INTERFACES][PCH_GPIO_DDP_NUMBER_OF_PINS] =
{
  {//DDPB
    {GPIO_CDF_GPP_I5,  GpioPadModeNative1}, //DDPB_CTRLCLK
    {GPIO_CDF_GPP_I6,  GpioPadModeNative1}, //DDPB_CTRLDATA
  },
  {//DDPC
    {GPIO_CDF_GPP_I7,  GpioPadModeNative1}, //DDPC_CTRLCLK
    {GPIO_CDF_GPP_I8,  GpioPadModeNative1}, //DDPC_CTRLDATA
  },
  {//DDPD
    {GPIO_CDF_GPP_I9,  GpioPadModeNative1}, //DDPD_CTRLCLK
    {GPIO_CDF_GPP_I10, GpioPadModeNative1}, //DDPD_CTRLDATA
  },
  {//DDPF
    {GPIO_CDF_GPP_I9, GpioPadModeNative1}, //DDPF_CTRLCLK
    {GPIO_CDF_GPP_I10, GpioPadModeNative1}, //DDPF_CTRLDATA
  }
};

//
// Chipset specific data
//

//SCS
extern GPIO_PAD_NATIVE_FUNCTION mPchScsEmmcGpio[11];
//SMBUS
extern GPIO_PAD_NATIVE_FUNCTION mPchHSmbusGpio[PCH_GPIO_SMBUS_NUMBER_OF_PINS];

//SMBUS Alert
extern GPIO_PAD_NATIVE_FUNCTION mPchHSmbusAlertGpio;

//USB OC
extern GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHUsbOcGpioPins[];

//SATA Led
extern GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSataLedGpio;

//PCIe SRCCLKREQB
extern GPIO_PAD_NATIVE_FUNCTION mPchHPcieSrcClkReqbPinToGpioMap[];

//PCH_HOT
extern GPIO_PAD_NATIVE_FUNCTION mPchHPchHotbPin;

//VRALERTB pin
extern GPIO_PAD_NATIVE_FUNCTION mPchHVrAlertbPin;

//CPU GP
extern GPIO_PAD_NATIVE_FUNCTION mPchHCpuGpPinMap[PCH_GPIO_CPU_GP_NUMBER_OF_PINS];

// DDSP_HPD pins
extern GPIO_PAD_NATIVE_FUNCTION mPchHDdspHpdPins[PCH_GPIO_DDSP_HPD_NUMBER_OF_PINS];

// EDP_HPD pin
extern GPIO_PAD_NATIVE_FUNCTION mPchHEdpHpdPin;

// DDPB/C/D  CTRLCLK and CTRLDATA pins
extern GPIO_PAD_NATIVE_FUNCTION mPchHDdpInterfacePins[PCH_GPIO_DDP_NUMBER_OF_INTERFACES][PCH_GPIO_DDP_NUMBER_OF_PINS];

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
  GPIO_PAD_NATIVE_FUNCTION SmbusAlertGpio;

  SmbusAlertGpio = mPchHSmbusAlertGpio;

  return GpioSetPadMode (SmbusAlertGpio.Pad, SmbusAlertGpio.Mode);
}

//
// SATADevSlpPin to GPIO pin mapping
// SATA_DEVSLP_x -> GPIO pin y
//

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata1DevSlpPinToGpioMap[] =
{
/// @todo SERVER- update for SATA 1
  {GPIO_CDF_GPP_E4, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata2DevSlpPinToGpioMap[] =
{
/// @todo SERVER- update for SATA 2
  {GPIO_CDF_GPP_E4, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSata3DevSlpPinToGpioMap[] =
{
/// @todo SERVER- update for SATA 3
  {GPIO_CDF_GPP_E4, GpioPadModeNative1}
};

GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mCdfPchSataGpToGpioMap[]  =
{
  {GPIO_CDF_GPP_L17, GpioPadModeNative1}, {GPIO_CDF_GPP_L18, GpioPadModeNative1}, {GPIO_CDF_GPP_L22, GpioPadModeNative1}
};

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
  GPIO_PAD_NATIVE_FUNCTION  *OcGpio;

  if(OcPinNumber >= ARRAY_SIZE (mPchHUsbOcGpioPins)){
    ASSERT(FALSE);
    return EFI_UNSUPPORTED;
    }
  OcGpio = mPchHUsbOcGpioPins;

  return GpioSetPadMode (OcGpio[OcPinNumber].Pad, OcGpio[OcPinNumber].Mode);
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
  GPIO_PAD_NATIVE_FUNCTION TempGpioPadNativeFunction = {0, 0};
  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    if (SataPort < ARRAY_SIZE (mCdfPchSataGpToGpioMap)) {
      *NativePin = mCdfPchSataGpToGpioMap[SataPort];
      return;
    }
  }

  *NativePin = TempGpioPadNativeFunction;
  ASSERT (FALSE);
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
  GPIO_PAD_NATIVE_FUNCTION  SataLedGpio;

  SataLedGpio = mPchHSataLedGpio;

  return GpioSetPadMode (SataLedGpio.Pad, SataLedGpio.Mode);
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
  GPIO_PAD_NATIVE_FUNCTION PchHotbPin;

  PchHotbPin = mPchHPchHotbPin;

  return GpioSetPadMode (PchHotbPin.Pad, PchHotbPin.Mode);
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
  GPIO_PAD_NATIVE_FUNCTION  VrAlertGpio;

  VrAlertGpio = mPchHVrAlertbPin;

  return GpioSetPadMode (VrAlertGpio.Pad, VrAlertGpio.Mode);
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
  EFI_STATUS               Status;
  GPIO_PAD_NATIVE_FUNCTION *CpuGpPins;

  ASSERT (CpuGpPinNum < PCH_GPIO_CPU_GP_NUMBER_OF_PINS);

  CpuGpPins = mPchHCpuGpPinMap;

  Status = GpioSetPadMode (CpuGpPins[CpuGpPinNum].Pad, CpuGpPins[CpuGpPinNum].Mode);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
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
  GPIO_PAD_NATIVE_FUNCTION  EdpHpdGpio;

  EdpHpdGpio = mPchHEdpHpdPin;

  return GpioSetPadMode (EdpHpdGpio.Pad, EdpHpdGpio.Mode);
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
  EFI_STATUS    Status;
  UINTN         Index;
  GPIO_PAD_NATIVE_FUNCTION *DdpGpio;
  UINT32                   DdpInterfaceIndex;

  if ((DdpInterface >= GpioDdpB) &&
      (DdpInterface <= GpioDdpD)) {
    //DDPB/C/D
    DdpInterfaceIndex = (DdpInterface & 0xF) - 1;
  } else if (DdpInterface == GpioDdpF) {
    //DDPF
    DdpInterfaceIndex = 3;
  } else {
    return EFI_UNSUPPORTED;
  }

  DdpGpio = mPchHDdpInterfacePins[DdpInterfaceIndex];

  for (Index = 0; Index < PCH_GPIO_DDP_NUMBER_OF_PINS; Index++) {
    Status = GpioSetPadMode (DdpGpio[Index].Pad, DdpGpio[Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}


GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchScsEmmcGpio[11] =
{
  {GPIO_CDF_GPP_O0, GpioPadModeNative1},// EMMC_CMD
  {GPIO_CDF_GPP_O1, GpioPadModeNative1},// EMMC_STROBE
  {GPIO_CDF_GPP_O2, GpioPadModeNative1},// EMMC_CLK
  {GPIO_CDF_GPP_O3, GpioPadModeNative1},// EMMC_DATA_0
  {GPIO_CDF_GPP_O4, GpioPadModeNative1},// EMMC_DATA_1
  {GPIO_CDF_GPP_O5, GpioPadModeNative1},// EMMC_DATA_2
  {GPIO_CDF_GPP_O6, GpioPadModeNative1},// EMMC_DATA_3
  {GPIO_CDF_GPP_O7, GpioPadModeNative1},// EMMC_DATA_4
  {GPIO_CDF_GPP_O8, GpioPadModeNative1},// EMMC_DATA_5
  {GPIO_CDF_GPP_O9, GpioPadModeNative1},// EMMC_DATA_6
  {GPIO_CDF_GPP_O10, GpioPadModeNative1}// EMMC_DATA_7
};

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
    *NativePinsTable = mPchScsEmmcGpio;
    *NoOfNativePins = ARRAY_SIZE (mPchScsEmmcGpio);
}

//
// GPIO pins for SMBUS
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIO_PAD_NATIVE_FUNCTION mPchHSmbusGpio[PCH_GPIO_SMBUS_NUMBER_OF_PINS] =
{
  {GPIO_CDF_GPP_C12, GpioPadModeNative3}, //SMB_CLK
  {GPIO_CDF_GPP_C13, GpioPadModeNative3}  //SMB_DATA
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
    *NativePinsTable = mPchHSmbusGpio;
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
  GPIO_PAD_NATIVE_FUNCTION TempGpioPadNativeFunction = {0, 0};
  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    if (SataPort < ARRAY_SIZE (mCdfPchSata1DevSlpPinToGpioMap)) {
      *NativePin = mCdfPchSata1DevSlpPinToGpioMap[SataPort];
      return;
    }
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    if (SataPort < ARRAY_SIZE (mCdfPchSata2DevSlpPinToGpioMap)) {
      *NativePin = mCdfPchSata2DevSlpPinToGpioMap[SataPort];
      return;
    }
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    if (SataPort < ARRAY_SIZE (mCdfPchSata3DevSlpPinToGpioMap)) {
      *NativePin = mCdfPchSata3DevSlpPinToGpioMap[SataPort];
      return;
    }
  }
  *NativePin = TempGpioPadNativeFunction;
  ASSERT (FALSE);
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
  GPIO_PAD_NATIVE_FUNCTION TempGpioPadNativeFunction = {0, 0};
  if (ClkreqIndex < ARRAY_SIZE (mPchHPcieSrcClkReqbPinToGpioMap)) {
    *NativePin = mPchHPcieSrcClkReqbPinToGpioMap[ClkreqIndex];
    return;
  }
  *NativePin = TempGpioPadNativeFunction;
  ASSERT (FALSE);
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
