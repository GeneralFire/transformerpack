/** @file
  UbaGpioInitLib implementation.

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

#include <PiPei.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UbaGpioInitLib.h>
#include <Library/GpioLib.h>
#include <Library/PeiDxeSmmGpioLib/GpioLibrary.h>
#include <Library/HobLib.h>
#include <Register/PchRegsSpi.h>
#include <Library/PchPcrLib.h>
#include <Register/PchRegsPcr.h>
#include <Library/EspiLib.h>
#include <Library/SetupLib.h>
#include <Pins/GpioPinsVer3.h>
#include <Memory/MemDefaults.h>

/**
  Configure GPIO

  @param[in]  PlatformInfo
**/
static GPIO_INIT_CONFIG mLpcConfig[] =
{
  0,0,0,0,0
};

/*++

Routine Description:
BIOS to set value on two EBG GPIOs according the ADR flow selected on the platform set up. (e.g. ADR, fADR, DDR5 BBU, or C2F)
1. Platform ADR is disabled, then GPPC_C_18 = 0, GPPC_S_3 = 0
2. Enabled platform Legacy/Fast ADR flow, then GPPC_C_18 = 0, GPPC_S_3 = 1
3. Enabled platform emulated DDR5 BBU ADR flow, then GPPC_C_18 = 1, GPPC_S_3 = 0
4. Enabled platform emulated copy to flash ADR flow, then GPPC_C_18 = 1, GPPC_S_3 = 1

--*/
VOID
UpdateADRGpios (
)
{
  EFI_STATUS                            Status;
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  INTEL_SETUP_DATA                            SetupData;
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.

  //
  // Get Setup Data
  //
  Status = GetEntireConfig (&SetupData);
  ASSERT_EFI_ERROR (Status);

  if (SetupData.SocketConfig.MemoryConfig.ADREn == ADR_DISABLE) {                                                                         // ADR disabled
    Status = GpioSetOutputValue (GPIO_VER3_GPP_C18, 0);
    Status = GpioSetOutputValue (GPIO_VER3_GPPC_S3, 0);
  } else if ((SetupData.SocketConfig.MemoryConfig.ADREn == ADR_ENABLE) && (SetupData.SocketConfig.MemoryConfig.ADRDataSaveMode == ADR_BBU)) {  // "Batterybacked DIMMs", DDR5 BBU
    Status = GpioSetOutputValue (GPIO_VER3_GPP_C18, 1);
    Status = GpioSetOutputValue (GPIO_VER3_GPPC_S3, 0);
  } else if ((SetupData.SocketConfig.MemoryConfig.ADREn == ADR_ENABLE) && (SetupData.SocketConfig.MemoryConfig.ADRDataSaveMode == ADR_NVDIMM)) {  // "NVDIMMs", Copy to Flash(C2F mode)
    Status = GpioSetOutputValue (GPIO_VER3_GPP_C18, 1);
    Status = GpioSetOutputValue (GPIO_VER3_GPPC_S3, 1);
  } else {                                                                                                                                 // Legacy or Fast ADR enabled
    Status = GpioSetOutputValue (GPIO_VER3_GPP_C18, 0);
    Status = GpioSetOutputValue (GPIO_VER3_GPPC_S3, 1);
  }

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "UpdateADRGpios fail, Status = %r.\n", Status));
  }

}

VOID
UpdateLpcConfiguration (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
)
{
  UINT32 Index1;
  UINT32 Index2;

  for(Index1 = 0; Index1 < sizeof (mLpcConfig)/sizeof (GPIO_INIT_CONFIG); Index1++) {
    for (Index2 = 0; Index2 < NumberOfItems; Index2++) {
      if (mLpcConfig[Index1].GpioPad == (GpioInitTableAddress+Index2)->GpioPad){
        (GpioInitTableAddress + Index2)->GpioConfig = mLpcConfig[Index1].GpioConfig;
      }
    }
  }
}

EFI_STATUS
PlatformInitGpios (
  VOID
)
{
  EFI_STATUS                            Status;
  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  GPIO_INIT_CONFIG                      *GpioTable;
  UINTN                                 TableSize;
  GPIO_INIT_CONFIG                      *mPchGpioInitData = NULL;
  VOID                                  *HobPtr;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = PcdGet32(PcdOemSku_GPIO_TABLE_SIZE);
  DEBUG ((DEBUG_ERROR, "UBA:Size of GpioTable 0x%X, blocks: 0x%X.\n", TableSize, (TableSize/sizeof (GPIO_INIT_CONFIG)) ));
  Status = PeiServicesAllocatePool (TableSize, &GpioTable);
  if (EFI_ERROR(Status)) {
   return Status;
  }
  Status = UbaConfigPpi->GetData (
                                UbaConfigPpi,
                                &gPlatformGpioInitDataGuid,
                                GpioTable,
                                &TableSize
                                );

  if (EFI_ERROR(Status)) {
    return Status;
  }
  // eSPI is a default mode for EBG and ADL. Default GPIO configuration table contains proper configuration.
  // If mode is switched to LPC we have to override GPIO configuration related to LPC interface pins.
  if (IsEspiEnabled()) {
    DEBUG((DEBUG_INFO, "eSPI Mode\n"));
    mPchGpioInitData = GpioTable;
  } else {
    DEBUG((DEBUG_INFO, "LPC Mode\n"));
    // Point to LPC config values
    mPchGpioInitData = AllocateZeroPool((UINTN)TableSize);
    if (mPchGpioInitData != NULL) {
      CopyMem(mPchGpioInitData, GpioTable, TableSize);
      UpdateLpcConfiguration((UINT32)(TableSize / sizeof(GPIO_INIT_CONFIG)), mPchGpioInitData);
    }
    else {
      DEBUG((DEBUG_ERROR, "\n*** ERROR!!!! AllocateZeroPool returned NULL pointer when trying to allocate buffer for 'mPchGpioInitData'!!!! ***\n"));
      ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
      return Status;
    }
  }

  DEBUG ((DEBUG_ERROR, "UBA: Start ConfigureGpio().\n"));
  Status = GpioConfigurePads (TableSize/sizeof (GPIO_INIT_CONFIG), mPchGpioInitData);
  DEBUG ((DEBUG_ERROR, "UBA: ConfigureGpio() end.\n"));

  UpdateADRGpios ();

  HobPtr = BuildGuidDataHob (
             &gPlatformGpioConfigGuid,
             mPchGpioInitData,
             TableSize
             );
  if (HobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "PlatformInitGpios(): ERROR: BuildGuidDataHob failed!\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
