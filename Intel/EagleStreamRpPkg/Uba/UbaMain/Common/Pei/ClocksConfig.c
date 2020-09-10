/** @file
  Install Platform Clocks Config Data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include "PeiCommonBoardInitLib.h"
#include <Library/PlatformClocksLib.h>
#include <Library/UbaClocksConfigLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/SpsPeiLib.h>
#include <Library/PchInfoLib.h>
#include <Ppi/PchPolicy.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>

//
// Table providing details on clocks supported by this library
//
CLOCK_GENERATOR_DETAILS mSupportedClockGenerator = {
  ClockGeneratorCk440,
  CK440_GENERATOR_ID,
  CK440_GENERATOR_SPREAD_SPECTRUM_BYTE,
  CK440_GENERATOR_SPREAD_SPECTRUM_BIT
};

/**

  Configure the clock generator to enable free-running operation.  This keeps
  the clocks from being stopped when the system enters C3 or C4.

  @param None

  @retval EFI_SUCCESS      The function completed successfully.
  @retval EFI_ABORTED      An error occurred.

**/
EFI_STATUS
PlatformClocksConfigCallback (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS                            Status;
  CLOCKING_MODES                        ClockingMode;
  UINT8                                 *ConfigurationTable;
  UINT8                                 ConfigurationTablePlatformSRP[] = CLOCK_GENERATOR_SETTINGS_PLATFORMSRP;
  UINT8                                 ClockGeneratorAddress = 0;
  UINTN                                 Length;
  BOOLEAN                               SecondarySmbus = FALSE;
  BOOLEAN                               EnableSpreadSpectrum;
  CLOCK_GENERATOR_TYPE                  ClockType;
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  DEBUG((DEBUG_INFO,"%a\n",__FUNCTION__));

  ClockGeneratorAddress = PcdGet8 (PcdOemSkuClockGeneratorAddress);
  Status = PeiGetCurrenClockingMode(&ClockingMode);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_ERROR, "PeiGetCurrenClockingMode: Status = %r\n", Status));
    return EFI_ABORTED;
  }

  if(ClockingMode != External) {
    DEBUG ((DEBUG_INFO, "External clock configuration not required\n"));
  } else {
    DEBUG ((DEBUG_INFO, "ConfigurePlatformClocks. Clock Mode: External\n"));
    GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
    if (GuidHob == NULL) {
      DEBUG ((EFI_D_ERROR, "Get Platform External Config Failed\n"));
      return EFI_ABORTED;
    }
    PlatformInfo = GET_GUID_HOB_DATA(GuidHob);

    if (IsSimicsEnvironment()) {
      //
      // Simics External Clock SSC is not required here.
      //
      return EFI_SUCCESS;
    } else {
      //
      // SRP/DVP configuration
      //
      ConfigurationTable = ConfigurationTablePlatformSRP;
      Length = sizeof (ConfigurationTablePlatformSRP);
      ClockType = ClockGeneratorCk440;
    }
    EnableSpreadSpectrum = (BOOLEAN) PlatformInfo->ExternalClock.EnableClockSpreadSpec;

    //
    // For ClockGeneratorCk440, set 0 to enable SSC, set 1 to disable SSC.
    //
    EnableSpreadSpectrum = ~(EnableSpreadSpectrum) & 1;

    if (EnableSpreadSpectrum) {
      ConfigurationTable[mSupportedClockGenerator.SpreadSpectrumByteOffset] |=
                      mSupportedClockGenerator.SpreadSpectrumBitOffset;
    } else {
      ConfigurationTable[mSupportedClockGenerator.SpreadSpectrumByteOffset] &=
                      ~(mSupportedClockGenerator.SpreadSpectrumBitOffset);
    }
    Status = ConfigureClockGenerator (PeiServices,
                                      ClockType,
                                      ClockGeneratorAddress,
                                      Length,
                                      ConfigurationTable,
                                      EnableSpreadSpectrum,
                                      &mSupportedClockGenerator,
                                      SecondarySmbus
                                     );
  }
  return EFI_SUCCESS;
}

PLATFORM_CLOCKS_CONFIG_TABLE    PlatformClocksConfigTable =
{
  PLATFORM_CLOCKS_CONFIG_SIGNATURE,
  PLATFORM_CLOCKS_CONFIG_VERSION,
  PlatformClocksConfigCallback
};

EFI_STATUS
InstallPlatformClocksConfigData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;

   Status = UbaConfigPpi->AddData (
                                  UbaConfigPpi,
                                  &gPlatformClocksConfigDataGuid,
                                  &PlatformClocksConfigTable,
                                  sizeof(PlatformClocksConfigTable)
                                  );

  return Status;
}

