/** @file
  Init Platform DeepS5 file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
#include <Ppi/Smbus2.h>
#include <Ppi/SmbusPolicy.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>

#define NCT3012S_SLAVE_ADDRESS          0x6C
#define NCT3012S_VENDOR_ID_OFFSET       0x20
#define NCT3012S_DEEP_SLEEP_OFFSET      0x0
#define NCT3012S_DELAY_TIME_OFFSET      0x15
#define NCT3012S_ENABLE_DEEPS5          0x3D
#define NCT3012S_DISABLE_DEEPS5         0x3C
#define PCA9546A_SLAVE_ADDRESS          0xE2
#define PCA9546A_ENABLE_CHANNEL         0x0F

/**

  Init PCH Smbus Mux

  @param SmbusPpi               Pointer to SMBUs services PPI.

  @return EFI_SUCCESS           Init PCH Smbus Mux completed successfully.
  @return EFI_ABORTED           An error occurred.

**/
EFI_STATUS
InitSmbusMux (
  IN    EFI_PEI_SMBUS2_PPI        *SmbusPpi
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SlaveAddress;
  UINTN                           Length;
  EFI_SMBUS_DEVICE_COMMAND        Command;
  UINT8                           Value = 0;

  //
  // Init Smbus Mux Host Clocks Channel (Channel 1)
  //
  SlaveAddress.SmbusDeviceAddress = PCA9546A_SLAVE_ADDRESS >> 1;
  Length = sizeof (Value);
  Command = PCA9546A_ENABLE_CHANNEL;
  Status = SmbusPpi->Execute (
                        SmbusPpi,
                        SlaveAddress,
                        Command,
                        EfiSmbusWriteByte,
                        FALSE,
                        &Length,
                        &Value
                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Init PCH Smbus Mux Host Clocks Channel Failed\n"));
    return EFI_ABORTED;
  }
  return EFI_SUCCESS;
}

/**

  Set Platform DeepS5 Mode

  @param SmbusPpi            Pointer to SMBUs services PPI.
  @param PlatformMode        The Platform DeepS5 Mode to set.

  @return EFI_SUCCESS        Set Platform DeepS5 Mode completed successfully.
  @return EFI_ABORTED        An error occurred.

**/
EFI_STATUS
SetPlatformDeepS5 (
  IN    EFI_PEI_SMBUS2_PPI        *SmbusPpi,
  IN    BOOLEAN                   PlatformMode
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SlaveAddress;
  UINTN                           Length;
  EFI_SMBUS_DEVICE_COMMAND        Command;
  UINT8                           Value;

  //
  // Read Chip and Version ID Register, default value is 0x88
  //
  SlaveAddress.SmbusDeviceAddress = NCT3012S_SLAVE_ADDRESS >> 1;
  Length = sizeof (Value);
  Command = NCT3012S_VENDOR_ID_OFFSET;
  Status = SmbusPpi->Execute (
                        SmbusPpi,
                        SlaveAddress,
                        Command,
                        EfiSmbusReadByte,
                        FALSE,
                        &Length,
                        &Value
                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Read NCT3012S Chip Version ID Failed\n"));
    return EFI_ABORTED;
  }
  DEBUG ((EFI_D_INFO, "Read NCT3012S Chip Version ID = 0x%x\n", Value));

  //
  // Write DeepSleep Enable Control Register, default is disable DeepSleep mode
  //
  Command = NCT3012S_DEEP_SLEEP_OFFSET;
  if (PlatformMode) {
    Value = NCT3012S_ENABLE_DEEPS5;
    Status = SmbusPpi->Execute (
                          SmbusPpi,
                          SlaveAddress,
                          Command,
                          EfiSmbusWriteByte,
                          FALSE,
                          &Length,
                          &Value
                          );
  } else {
    Value =NCT3012S_DISABLE_DEEPS5;
    Status = SmbusPpi->Execute (
                          SmbusPpi,
                          SlaveAddress,
                          Command,
                          EfiSmbusWriteByte,
                          FALSE,
                          &Length,
                          &Value
                          );
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Set Platform DeepS5 Mode:%d, Failed\n", PlatformMode));
    return EFI_ABORTED;
  }
  return EFI_SUCCESS;
}

/**

  Set Platform DeepS5 Delay Time for entering Deep sleep mode

  @param SmbusPpi         Pointer to SMBUs services PPI
  @param DelayTime        The Delay Time Value to set.

  @return EFI_SUCCESS     Set Platform DeepS5 Delay Time completed successfully.
  @return EFI_ABORTED     An error occurred.

**/
EFI_STATUS
SetDeepS5DelayTime (
  IN    EFI_PEI_SMBUS2_PPI        *SmbusPpi,
  IN    UINT8                     DelayTime
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SlaveAddress;
  UINTN                           Length;
  EFI_SMBUS_DEVICE_COMMAND        Command;
  UINT8                           Value;

  //
  // Write Platform DeepS5 Delay Time Control Register, default is 3s delay
  //
  SlaveAddress.SmbusDeviceAddress = NCT3012S_SLAVE_ADDRESS >> 1;
  Length = sizeof (Value);
  Command = NCT3012S_DELAY_TIME_OFFSET;
  switch (DelayTime) {
    case 0:
      Value = 0x0;
      break;
    case 3:
      Value = 0x01;
      break;
    case 6:
      Value = 0x02;
      break;
    case 10:
      Value = 0x03;
      break;
    default:
      Value = 0x01;
  }
  Status = SmbusPpi->Execute (
                        SmbusPpi,
                        SlaveAddress,
                        Command,
                        EfiSmbusWriteByte,
                        FALSE,
                        &Length,
                        &Value
                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Set DeepS5 Delay Time:%ds, Failed\n", DelayTime));
    return EFI_ABORTED;
  }
  return EFI_SUCCESS;
}

/**

  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    Platform DeepS5 initialization completed successfully.
  @return EFI_ABORTED    An error occurred.

**/
EFI_STATUS
InitPlatformDeepS5 (
  IN       EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_SMBUS2_PPI              *SmbusPpi;
  EFI_PLATFORM_INFO               *PlatformInfo;
  EFI_HOB_GUID_TYPE               *GuidHob;
  //
  // Locate SmBus Ppi
  //
  Status = (**PeiServices).LocatePpi (
                              PeiServices,
                              &gEfiPeiSmbus2PpiGuid,
                              0,
                              NULL,
                              &SmbusPpi
                              );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Locate Smbus Ppi Failed\n"));
    return EFI_ABORTED;
  }

  //
  // Get Platform DeepS5 Config
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  if (GuidHob == NULL) {
    DEBUG ((EFI_D_ERROR, "Get Platform DeepS5 Config Failed\n"));
    return EFI_ABORTED;
  }
  PlatformInfo = GET_GUID_HOB_DATA(GuidHob);

  Status = InitSmbusMux (SmbusPpi);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Init PCH Smbus Mux Status = %r\n", Status));
    return EFI_ABORTED;
  }
  Status = SetPlatformDeepS5 (SmbusPpi, PlatformInfo->DeepS5Data.PlatformDeepS5);
  DEBUG ((EFI_D_INFO, "Set Platform DeepS5 Status = %r\n", Status));
  Status = SetDeepS5DelayTime (SmbusPpi, PlatformInfo->DeepS5Data.DeepS5DelayTime);
  DEBUG ((EFI_D_INFO, "Set Platform DeepS5 DelayTime Status = %r\n", Status));

  return EFI_SUCCESS;
}
