/** @file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#include "PeiBoardInit.h"

/**

    TypeArcherCityEVBGpioConfigForInterposer: Configure the GPIO for detecting an interposer

    @param GpioId - GPIO to configure

    @retval EFI_SUCCESS     - GPIO configured
**/
EFI_STATUS
TypeArcherCityEVBGpioConfigForInterposer (
  IN GPIO_PAD   GpioId
  )
{
  EFI_STATUS           Status;
  GPIO_CONFIG          PadConfig;

  PadConfig.PadMode          = GpioPadModeGpio;
  PadConfig.HostSoftPadOwn   = GpioHostOwnGpio;
  PadConfig.Direction        = GpioDirIn;
  PadConfig.OutputState      = GpioOutDefault;
  PadConfig.InterruptConfig  = GpioIntDefault;
  PadConfig.PowerConfig      = GpioResetDefault;
  PadConfig.ElectricalConfig = GpioTermDefault;
  PadConfig.LockConfig       = GpioLockDefault;
  PadConfig.OtherSettings    = GpioRxRaw1Default;

  Status = GpioSetPadConfig (GpioId, &PadConfig);
  if (Status == EFI_WRITE_PROTECTED) {
    GpioUnlockPadCfg(GpioId);
    Status = GpioSetPadConfig (GpioId, &PadConfig);
  }
  return Status;
}

/**
  The constructor function for Board Init Libray.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval  EFI_SUCCESS            Table initialization successfully.
  @retval  EFI_OUT_OF_RESOURCES   No enough memory to initialize table.
**/

EFI_STATUS
EFIAPI
TypeArcherCityEVBPeiBoardInitLibConstructor (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UBA_CONFIG_DATABASE_PPI         *UbaConfigPpi;
  EFI_HOB_GUID_TYPE               *GuidHob;
  EFI_PLATFORM_INFO               *PlatformInfo;
  UINT8                           SocketIndex;
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  DEBUG ((EFI_D_ERROR, "UbaInitPeim: PlatformType=0x%X\n", PlatformInfo->BoardId));
  if (PlatformInfo->BoardId == TypeArcherCityEVB) {

    Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = UbaConfigPpi->InitSku (
                       UbaConfigPpi,
                       PlatformInfo->BoardId,
                       NULL,
                       NULL
                       );
    ASSERT_EFI_ERROR (Status);

    Status = TypeArcherCityEVBInstallGpioData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeArcherCityEVBInstallPcdData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeArcherCityEVBPchEarlyUpdate (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeArcherCityEVBPlatformUpdateUsbOcMappings (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeArcherCityEVBInstallSlotTableData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeArcherCityEVBInstallKtiEparamData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    // Initialize InterposerType to InterposerUnknown
    //
    for (SocketIndex = 0; SocketIndex < MAX_SOCKET; ++SocketIndex) {
      PlatformInfo->InterposerType[SocketIndex] = InterposerUnknown;
    }


    BuildGuidDataHob (
      &gEfiPlatformInfoGuid,
      &(PlatformInfo),
      sizeof (EFI_PLATFORM_INFO)
      );

    //
    //  TypeArcherCityEVBIioPortBifurcationInit will use PlatformInfo->InterposerType for PPO.
    //
    Status = TypeArcherCityEVBIioPortBifurcationInit (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = TypeArcherCityEVBPchPcieBifurcation (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO, "UBA PEI: %r\n", Status));
      return Status;
    }

    Status = TypeArcherCityEVBInstallGpioPlatformData (UbaConfigPpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }

      DEBUG ((DEBUG_INFO, "UBA PEI: Install tables success\n"));
  }
  return Status;
}
