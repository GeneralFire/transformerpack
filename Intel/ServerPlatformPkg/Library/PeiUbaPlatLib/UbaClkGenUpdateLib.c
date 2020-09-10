/** @file
  UbaClkGenUpdateLib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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
#include <Uefi/UefiSpec.h>

#include <Ppi/UbaCfgDb.h>
#include <Ppi/Smbus2.h>

#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>

#include <Library/UbaClkGenUpdateLib.h>

EFI_STATUS
PlatformUpdateClockgen (
  IN  BOOLEAN                           EnableSpreadSpectrum
)
{
  EFI_STATUS                            Status;

  UBA_CONFIG_DATABASE_PPI               *UbaConfigPpi = NULL;
  PLATFORM_CLOCKGEN_UPDATE_TABLE        ClockgenTable;
  UINTN                                 TableSize = 0;

  EFI_SMBUS_DEVICE_ADDRESS              SlaveAddress;
  UINT8                                 Buffer[PLATFORM_NUMBER_OF_CLOCKGEN_DATA];
  UINTN                                 Length = 0;
  EFI_SMBUS_DEVICE_COMMAND              Command;
  EFI_PEI_SMBUS2_PPI                    *SmbusPpi = NULL;
  UINT8                                 ClockGeneratorAddress = 0;

  ClockGeneratorAddress = PcdGet8 (PcdOemSkuClockGeneratorAddress);

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = PeiServicesLocatePpi (
              &gEfiPeiSmbus2PpiGuid,
              0,
              NULL,
              &SmbusPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Read the clock generator
  //
  SlaveAddress.SmbusDeviceAddress = ClockGeneratorAddress >> 1;
  Length      = sizeof (Buffer);
  Command     = 0;
  Status = SmbusPpi->Execute (
                      SmbusPpi,
                      SlaveAddress,
                      Command,
                      EfiSmbusReadBlock,
                      FALSE,
                      &Length,
                      Buffer
                      );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  do {

    TableSize = sizeof(ClockgenTable);
    Status = UbaConfigPpi->GetData (
                                  UbaConfigPpi,
                                  &gPlatformClockgenConfigDataGuid,
                                  &ClockgenTable,
                                  &TableSize
                                  );

    if (EFI_ERROR(Status)) {
      return Status;
    }

    ASSERT (ClockgenTable.Signature == PLATFORM_CLOCKGEN_UPDATE_SIGNATURE);
    ASSERT (ClockgenTable.Version == PLATFORM_CLOCKGEN_UPDATE_VERSION);

    if (ClockgenTable.Id != PLATFORM_CLOCKGEN_NO_ID) {
      if (ClockgenTable.Id != Buffer[ClockgenTable.IdOffset]) {
        continue;
      }
    }

    if (EnableSpreadSpectrum) {
      ClockgenTable.Data[ClockgenTable.SpreadSpectrumByteOffset] = ClockgenTable.SpreadSpectrumValue;
    }

    //
    // Program clock generator
    //
    Command = 0;
    Length  = ClockgenTable.DataLength;

    Status = SmbusPpi->Execute (
                        SmbusPpi,
                        SlaveAddress,
                        Command,
                        EfiSmbusWriteBlock,
                        FALSE,
                        &Length,              //&ConfigurationTableLength,
                        &ClockgenTable.Data   //ConfigurationTable
                        );
    if (EFI_ERROR(Status)) {
      return Status;
    } else {
      break;
    }
  }while (!EFI_ERROR(Status));

  return Status;
}


