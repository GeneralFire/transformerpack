/** @file
  Pch information library for EBG.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegsLpcTgl.h>
#include "PchInfoLibPrivate.h"
#include <Library/PmcPrivateLib.h>
#include <Library/PrintLib.h>
#include <Register/PchRegsLpc.h>
#include <Library/DebugLib.h>
#include <Register/PchRegsLpcEbg.h>

/**
  Determine Pch Series based on Device Id

  @param[in] LpcDeviceId      Lpc Device Id

  @retval PCH_SERIES          Pch Series
**/
PCH_SERIES
PchSeriesFromLpcDid (
  IN UINT16 LpcDeviceId
  )
{
  return PCH_H;
}

/**
  Return Pch Generation

  @retval PCH_GENERATION            Pch Generation
**/
PCH_GENERATION
PchGeneration (
  VOID
  )
{
  return EBG_PCH;
}

/**
  Check if this is PCH N series

  @retval TRUE                It's PCH N series
  @retval FALSE               It's not PCH N series
**/
BOOLEAN
IsPchN (
  VOID
  )
{
  return FALSE;
}


/**
  Check if this is Server PCH

  @retval TRUE                It's a Server PCH
  @retval FALSE               It's not a Server PCH
**/
BOOLEAN
IsPchServer (
  VOID
  )
{
  return TRUE;
}

/**
  Check if this is PCH H series

  @retval TRUE                It's PCH H series
  @retval FALSE               It's not PCH H series
**/
BOOLEAN
IsPchH (
  VOID
  )
{
  return (PchSeries () == PCH_H);
}

/**
  Check if this is PCH K series

  @retval TRUE                It's PCH K series
  @retval FALSE               It's not PCH K series
**/
BOOLEAN
IsPchK (
  VOID
  )
{
  return FALSE;
}

/**
  Get Pch Maximum Pcie Clock Number

  @retval Pch Maximum Pcie Clock Number
**/
UINT8
GetPchMaxPcieClockNum (
  VOID
  )
{
  return 16;
}

/**
  Get Pch Maximum Pcie ClockReq Number

  @retval Pch Maximum Pcie ClockReq Number
**/
UINT8
GetPchMaxPcieClockReqNum (
  VOID
  )
{
  return 10;
}

/**
  Check if this is Server SKU

  @retval TRUE                It's PCH Server series
  @retval FALSE               It's not PCH Server series
**/
BOOLEAN
IsPchServerSku (
  VOID
  )
{
  return TRUE;
}

/**
  Get PCH series ASCII string.

  @retval PCH Series string
**/
CHAR8*
PchGetSeriesStr (
  VOID
  )
{
  return "EBG PCH";
}

GLOBAL_REMOVE_IF_UNREFERENCED
struct PCH_SKU_STRING mSkuStrs[] = {
  {V_EBG_LPC_CFG_DEVICE_ID_1B80, "EBG SKU"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B81, "EBG A0 SKU"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B82, "EBG SKU 2"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B83, "EBG SKU 3"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B84, "EBG SKU 4"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B85, "EBG SKU 5"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B86, "EBG SKU 6"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B87, "EBG SKU 7"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B88, "EBG SKU 8"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B89, "EBG SKU 9"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B8A, "EBG SKU 10"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B8B, "EBG SKU 11"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B8C, "EBG SKU 12"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B8D, "EBG SKU 13"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B8E, "EBG SKU 14"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B8F, "EBG SKU 15"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B90, "EBG SKU 16"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B91, "EBG SKU 17"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B92, "EBG SKU 18"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B93, "EBG SKU 19"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B94, "EBG SKU 20"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B95, "EBG SKU 21"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B96, "EBG SKU 22"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B97, "EBG SKU 23"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B98, "EBG SKU 24"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B99, "EBG SKU 25"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B9A, "EBG SKU 26"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B9B, "EBG SKU 27"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B9C, "EBG SKU 28"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B9D, "EBG SKU 29"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B9E, "EBG SKU 30"},
  {V_EBG_LPC_CFG_DEVICE_ID_1B9F, "EBG SKU 31"},
  {0xFFFF, NULL}
};

/**
  Print Pch Stepping String

  @param[out] Buffer         Output buffer of string
  @param[in]  BufferSize     Buffer Size
  @param[in]  PchStep        Pch Stepping Type

  @retval VOID
**/
VOID
PchPrintSteppingStr (
  OUT CHAR8        *Buffer,
  IN  UINT32       BufferSize,
  IN  PCH_STEPPING PchStep
  )
{
  AsciiSPrint (Buffer, BufferSize, "%c%c", 'A' + (PchStep >> 4), '0' + (PchStep & 0xF));
}

/**
  Determine Pch Stepping based on Revision ID

  @param[in] RevId            Pch Revision Id

  @retval PCH_STEPPING        Pch Stepping
**/
PCH_STEPPING
PchSteppingFromRevId (
  IN UINT8 RevId
  )
{
  return RevId;
}

/**
  Get Pch Maximum Pcie Root Port Number

  @retval Pch Maximum Pcie Root Port Number
**/
UINT8
GetPchMaxPciePortNum (
  VOID
  )
{
  return 16;
}

/**
  Get Pch Usb2 Maximum Physical Port Number (not including USB-R ports)

  @retval Pch Usb2 Maximum Physical Port Number
**/
UINT8
GetPchUsb2MaxPhysicalPortNum (
  VOID
  )
{
  return 14;
}

/**
  Get Pch Maximum Usb2 Port Number of XHCI Controller (including USB-R ports)

  @retval Pch Maximum Usb2 Port Number of XHCI Controller
**/
UINT8
GetPchXhciMaxUsb2PortNum (
  VOID
  )
{
  return 16;
}

/**
  Get Pch Usb3 Maximum Physical Port Number

  @retval Pch Usb3 Maximum Physical Port Number
**/
UINT8
GetPchUsb3MaxPhysicalPortNum (
  VOID
  )
{
  return 10;
}

/**
  Get Pch Maximum Usb3 Port Number of XHCI Controller

  @retval Pch Maximum Usb3 Port Number of XHCI Controller
**/
UINT8
GetPchXhciMaxUsb3PortNum (
  VOID
  )
{
  return 10;
}


/**
  Get Pch Maximum Type C Port Number

  @retval Pch Maximum Type C Port Number
**/
UINT8
GetPchMaxTypeCPortNum (
  VOID
  )
{
  return 1;

}

/**
  Check whether integrated LAN controller is supported by PCH Series.

  @retval TRUE                    GbE is supported in current PCH
  @retval FALSE                   GbE is not supported on current PCH
**/
BOOLEAN
PchIsGbeSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Check whether ISH is supported by PCH Series.

  @retval TRUE                    ISH is supported in current PCH
  @retval FALSE                   ISH is not supported on current PCH
**/
BOOLEAN
PchIsIshSupported (
  VOID
  )
{
   return FALSE;
}

/**
  Check whether ATX Shutdown (PS_ON) is supported.

  @retval    TRUE           ATX Shutdown (PS_ON) is supported in PCH
  @retval    FALSE          ATX Shutdown (PS_ON) is not supported by PCH
**/
BOOLEAN
IsPchPSOnSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Get Pch Maximum Hda Sndw Link

  @retval Pch Maximum Hda Sndw Link
**/
UINT8
GetPchHdaMaxSndwLinkNum (
  VOID
  )
{
  return 0;
}


/**
  Check if given Audio Interface is supported

  @param[in] AudioLinkType   Link type support to be checked
  @param[in] AudioLinkIndex  Link number

  @retval    TRUE           Link supported
  @retval    FALSE          Link not supported
**/
BOOLEAN
IsAudioInterfaceSupported (
  IN HDAUDIO_LINK_TYPE     AudioLinkType,
  IN UINT32                AudioLinkIndex
  )
{
  return FALSE;
}

/**
  Check if given Display Audio Link T-Mode is supported

  @param[in] Tmode          T-mode support to be checked

  @retval    TRUE           T-mode supported
  @retval    FALSE          T-mode not supported
**/
BOOLEAN
IsAudioIDispTmodeSupported (
  IN HDAUDIO_IDISP_TMODE Tmode
  )
{
  return FALSE;
}

/**
  Get Pch Maximum Hda Ssp Link

  @retval Pch Maximum Hda Ssp Link
**/
UINT8
GetPchHdaMaxSspLinkNum (
  VOID
  )
{
  return 0;
}

/**
  Get Pch Maximum Hda Dmic Link

  @retval Pch Maximum Hda Dmic Link
**/
UINT8
GetPchHdaMaxDmicLinkNum (
  VOID
  )
{
  return 0;
}


/**
  Gets the maximum number of UFS controller supported by this chipset.

  @return Number of supported UFS controllers
**/
UINT8
PchGetMaxUfsNum (
  VOID
  )
{
  return 0;
}

/**
  Check if this chipset supports eMMC controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchEmmcSupported (
  VOID
  )
{
  return FALSE;
}


/**
  Check if this chipset supports SD controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchSdCardSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Check if this chipset supports UFS controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchUfsSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Check if link between PCH and CPU is an P-DMI

  @retval    TRUE           P-DMI link
  @retval    FALSE          Not an P-DMI link
**/
BOOLEAN
IsPchWithPdmi (
  VOID
  )
{
  return TRUE;
}

/**
  Check if link between PCH and CPU is an OP-DMI

  @retval    TRUE           OP-DMI link
  @retval    FALSE          Not an OP-DMI link
**/
BOOLEAN
IsPchWithOpdmi (
  VOID
  )
{
  return FALSE;
}

/**
  Check if link between PCH and CPU is an F-DMI

  @retval    TRUE           F-DMI link
  @retval    FALSE          Not an F-DMI link
**/
BOOLEAN
IsPchWithFdmi (
  VOID
  )
{
  return FALSE;
}

/**
  Get Pch Maximum Serial IO UART controllers number

  @retval Pch Maximum Serial IO UART controllers number
**/
UINT8
GetPchMaxSerialIoUartControllersNum (
  VOID
  )
{
  return 2;
}

/**
  Get Pch Maximum Serial IO I2C controllers number

  @retval Pch Maximum Serial IO I2C controllers number
**/
UINT8
GetPchMaxSerialIoI2cControllersNum (
  VOID
  )
{
  return 0;
}


/**
  Get Pch Maximum Serial IO SPI Chip Selects count

  @retval Pch Maximum Serial IO SPI Chip Selects number
**/
UINT8
GetPchMaxSerialIoSpiChipSelectsNum (
  VOID
  )
{
  return 0;
}


/**
  Get Pch Maximum Serial IO SPI controllers number

  @retval Pch Maximum Serial IO SPI controllers number
**/
UINT8
GetPchMaxSerialIoSpiControllersNum (
  VOID
  )
{
  return 0;
}

/**
  Get Pch Maximum THC count

  @retval Pch Maximum THC count number
**/
UINT8
GetPchMaxThcCount (
  VOID
  )
{
  return 0;
}

/**
  Returns USB2 PHY Reference Clock frequency value used by PCH
  This defines what electrical tuning parameters shall be used
  during USB2 PHY initialization programming

  @retval Frequency reference clock for USB2 PHY (for EBG it's 24MHz)
**/
USB2_PHY_REF_FREQ
GetUsb2PhyRefFreq (
  VOID
  )
{
  return FREQ_24_0;
}

/**
  Check if SPI in a given PCH generation supports an Extended BIOS Range Decode

  @retval TRUE or FALSE if PCH supports Extended BIOS Range Decode
**/
BOOLEAN
IsExtendedBiosRangeDecodeSupported (
  VOID
  )
{
  return TRUE;
}

#define SPI_PCH_DMI_TARGET  0x2398

/**
  Returns DMI target for current PCH SPI

  @retval PCH SPI DMI target value
**/
UINT16
GetPchSpiDmiTarget (
  VOID
  )
{
  return SPI_PCH_DMI_TARGET;
}