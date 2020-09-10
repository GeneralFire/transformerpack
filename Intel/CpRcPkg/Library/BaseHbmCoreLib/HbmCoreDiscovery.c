/** @file
  Interface source file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include "HbmCore.h"
#include "Library/SysHostPointerLib.h"
#include <Library/HbmIoIpLib.h>
#include <MemHostChipCommon.h>

/**

  HBM Calculate stack memory size.

  @param[in] ChannelBitMap           Bit map indicates which channel is available.
  @param[in] DensityCode             HBM memory density, see HBM_DENSITY enum.

  @return  The value of HBM stack size.

**/
UINT16
HbmStackSize (
  IN UINT8       ChannelBitMap,
  IN UINT8       DensityCode
  )
{
  UINT8  BitIndex;
  UINT8  HighestBit;
  UINT8  ChannelNumber;
  UINT16 MemDensity  = 0;

  //
  // HBM stack size is 0 if there is no available HBM channel.
  //
  if (ChannelBitMap == 0) {
    return 0;
  }

  ChannelNumber = 0;
  HighestBit = (UINT8) HighBitSet32 ((UINT32) ChannelBitMap);
  for (BitIndex = 0; BitIndex <= HighestBit; BitIndex++) {
    if ((ChannelBitMap & (BIT0 << BitIndex)) !=0) {
      ChannelNumber++;
    }
  }

  switch (DensityCode) {

  case Hbm1Gb:
    MemDensity = 1 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm2Gb:
    MemDensity = 2 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm4Gb:
    MemDensity = 4 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm6Gb:
    MemDensity = 6 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm8Gb8H:
  case Hbm8Gb:
    MemDensity = 8 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm12Gb8H:
  case Hbm8Gb12H:
    MemDensity = 12 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm16Gb8H:
    MemDensity = 16 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm12Gb12H:
    MemDensity = 18 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  case Hbm16Gb12H:
    MemDensity = 24 * (MEM_1GB_AT_64MB_GRAN / 8);
    break;

  default:
    HbmDebugPrint (HBM_SDBG_MINMAX, "HBM: Invalid HBM density code 0x%x, force HBM density to 0\n", DensityCode);
    MemDensity = 0;
    break;
  }

  return (MemDensity * ChannelNumber);
}

/**
  HBM Discovery

  This function reads the Device ID data from HBM FW and detects the HBM device attributes
  according to the Device ID data. The HBM device attributes will be cached into HBM Host.

  @param[in] SocketId             Socket index.

  @retval  EFI_SUCCESS            This function completed the HBM discovery successfully.
  @retval  !EFI_SUCCESS           This function failed to discover HBM device.
**/
EFI_STATUS
EFIAPI
HbmDiscovery (
  IN UINT8       SocketId
  )
{
  UINT8          HbmIoId;
  UINT8          MaxHbmIo;
  UINT8          HbmChIdPerIo;
  UINT8          MaxHbmChPerHbmIo;
  UINT8          Density;
  UINT8          ChannelBitMap;
  HBM_HOST       *HbmHost;
  EFI_STATUS     Status;
  DEVICE_ID_WDR  *DeviceIdData;

  MaxHbmIo = GetMaxIoHbm ();
  MaxHbmChPerHbmIo = GetNumChannelPerIoHbm ();
  HbmHost = (HBM_HOST *) GetHbmHostPointer ();

  for (HbmIoId = 0; HbmIoId < MaxHbmIo; HbmIoId++) {
    if (!IsHbmMemSsEnabled (SocketId, HbmIoId)) {
      continue;
    }

    //
    // Read and cache HBM Device ID data.
    //
    DeviceIdData = GetHbmDeviceId (SocketId, HbmIoId);
    Status = HbmioMailboxReadDeviceId (SocketId, HbmIoId, DeviceIdData);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    HbmDebugPrint (HBM_SDBG_MIN, "HBM: DeviceIdData = {0x%08x, 0x%08x, 0x%08x}\n", DeviceIdData->Data[0], DeviceIdData->Data[1], DeviceIdData->Data[2]);

    //
    // Calculate HBM stack size.
    //
    Density = (UINT8) DeviceIdData->Bits.density;
    ChannelBitMap = (UINT8) DeviceIdData->Bits.channel_available;
    SetHbmCacheMemSize (SocketId, HbmIoId, HbmStackSize (ChannelBitMap, Density));

    //
    // Update HBM memory subsystem status.
    //
    if (ChannelBitMap == 0) {
      SetHbmMemSsEnable (SocketId, HbmIoId, 0);
    }

    //
    // Update HBM channel status.
    //
    for (HbmChIdPerIo = 0; HbmChIdPerIo < MaxHbmChPerHbmIo; HbmChIdPerIo++) {
      SetHbmChEnable (SocketId, HbmChIdPerIo + (HbmIoId * MaxHbmChPerHbmIo), (ChannelBitMap >> HbmChIdPerIo) & BIT0);
    }
  }

  return EFI_SUCCESS;
}


/**

  Initialize  HBM IO via FSM mechanism.

  @param[in]                Socket              Current Socket (0-Based)

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
UINT32
EFIAPI
HbmBasicTraining (
  IN        UINT8           Socket
  )
{
  // HBM TODO, will trigger functions from HBMIO IP lib to do training
  return 0;

} // HbmBasicTraining


/**
  This function detects which HBM memory subsystems are enabled.

  @param[in] SocketId       Current Socket index (0-Based).

  @retval HbmMemSsBitMap    The bitmap of HBM memory subsystems. Enabled = 1; Disabled = 0.
                            One bit corresponds to one HBM memory subsystem.
**/
UINT8
EFIAPI
HbmDetected (
  IN UINT8    SocketId
  )
{
  UINT8       HbmIoId;
  UINT8       MaxHbmIo;
  UINT8       HbmMemSsBitMap;

  MaxHbmIo = GetMaxIoHbm();
  HbmMemSsBitMap = 0;

  for (HbmIoId = 0; HbmIoId < MaxHbmIo; HbmIoId++) {
    if (IsHbmMemSsEnabled(SocketId, HbmIoId)) {
      HbmMemSsBitMap |= (BIT0 << HbmIoId);
    }
  }

  return HbmMemSsBitMap;
}
