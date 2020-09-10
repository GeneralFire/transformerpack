/** @file
  Platform Clocks Lib file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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
#include <Library/IoLib.h>
#include <Library/PlatformClocksLib.h>
#include <Ppi/Smbus2.h>
#include <Ppi/SmbusPolicy.h>
#include <Library/DebugLib.h>
#include <Ppi/Stall.h>
#include <Library/HobLib.h>
#include <Platform.h>
#include <Library/BaseMemoryLib.h>

#define MAX_SMBU_RETRIES        10
#define PCA9546A_SLAVE_ADDRESS          0xE2
#define PCA9546A_ENABLE_CHANNEL         0x0F

EFI_STATUS
ConfigureClockGeneratorOnSecondarySmbus (
  IN    EFI_PEI_SERVICES          **PeiServices,
  IN    EFI_PEI_SMBUS2_PPI        *SmbusPpi,
  IN    UINT8                     ClockAddress,
  IN    UINTN                     ConfigurationTableLength,
  IN    BOOLEAN                   EnableSpreadSpectrum,
  IN    CLOCK_GENERATOR_DETAILS   *mSupportedClockGeneratorT,
  IN OUT UINT8                    *ConfigurationTable
  );

/**

  Configure the clock generator using the SMBUS PPI services.

  This function performs a block write, and dumps debug information.

  @param PeiServices               - General purpose services available to every PEIM.
  @param ClockType                 - Clock generator's model name.
  @param ClockAddress              - SMBUS address of clock generator.
  @param ConfigurationTableLength  - Length of configuration table.
  @param ConfigurationTable        - Pointer of configuration table.

  @retval EFI_SUCCESS - Operation success.

**/
EFI_STATUS
ConfigureClockGenerator (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN     CLOCK_GENERATOR_TYPE     ClockType,
  IN     UINT8                    ClockAddress,
  IN     UINTN                    ConfigurationTableLength,
  IN OUT UINT8                    *ConfigurationTable,
  IN     BOOLEAN                  EnableSpreadSpectrum,
  IN     CLOCK_GENERATOR_DETAILS  *mSupportedClockGeneratorT,
  IN     UINT8                    SecondarySmbus
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SlaveAddress;
  UINT8                           Buffer[MAX_CLOCK_GENERATOR_BUFFER_LENGTH];
  UINTN                           Length;
  EFI_SMBUS_DEVICE_COMMAND        Command;
  EFI_PEI_SMBUS2_PPI              *SmbusPpi;
  UINT8                           SmbErrorsCounter;
  UINT8                           Value = 0;

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
  ASSERT_EFI_ERROR (Status);

  //
  // Verify input arguments
  //
  ASSERT (ConfigurationTableLength >= 8);
  ASSERT (ConfigurationTableLength <= MAX_CLOCK_GENERATOR_BUFFER_LENGTH);
  ASSERT (ClockType < ClockGeneratorMax);
  ASSERT (ConfigurationTable != NULL);

  //
  // Init Smbus Mux Host Clocks Channel (Channel 1)
  //
  SlaveAddress.SmbusDeviceAddress = PCA9546A_SLAVE_ADDRESS >> 1;
  Length = sizeof (Value);
  Command = PCA9546A_ENABLE_CHANNEL;
  SmbErrorsCounter = 0;
  do
  {
    Status = SmbusPpi->Execute (
                          SmbusPpi,
                          SlaveAddress,
                          Command,
                          EfiSmbusWriteByte,
                          FALSE,
                          &Length,
                          &Value
                          );
    if (EFI_ERROR (Status))
    {
      DEBUG ((EFI_D_ERROR, "PCH SMBUS Enable Host Clocks Channel ERROR\n"));
    }
    SmbErrorsCounter ++;
  } while (EFI_ERROR (Status) && (SmbErrorsCounter < MAX_SMBU_RETRIES));

  //
  // Init some local vars
  //
  SlaveAddress.SmbusDeviceAddress = ClockAddress >> 1;
  Length = sizeof (Buffer);
  Command = 0;

  if (SecondarySmbus == TRUE) {

    return (ConfigureClockGeneratorOnSecondarySmbus(PeiServices,
                                          SmbusPpi,
                                          ClockAddress,
                                          ConfigurationTableLength,
                                          EnableSpreadSpectrum,
                                          mSupportedClockGeneratorT,
                                          ConfigurationTable
                                         ));
  } else {
    //
    // Not LightningRidge
    // Read the clock generator on the primary SMBus
    //

    SmbErrorsCounter = 0;

    do
    {
    Status = SmbusPpi->Execute (
      SmbusPpi,
      SlaveAddress,
      Command,
      EfiSmbusReadBlock,
      FALSE,
      &Length,
      Buffer
      );
      if(Status != EFI_SUCCESS)
      {
        DEBUG ((EFI_D_ERROR, "SMBUS reading error\n"));

      }

      SmbErrorsCounter ++;
    }
    while ((Status != EFI_SUCCESS) && (SmbErrorsCounter < MAX_SMBU_RETRIES));

    //
    // Sanity check that the requested clock type is present in our supported clocks table
    //
    DEBUG ((EFI_D_ERROR, "Expected Clock Generator ID is %x, populated %x\n", mSupportedClockGeneratorT->ClockId, Buffer[7]));

    if (EnableSpreadSpectrum) {
      Buffer[mSupportedClockGeneratorT->SpreadSpectrumByteOffset] |= mSupportedClockGeneratorT->SpreadSpectrumBitOffset;
    } else {
      Buffer[mSupportedClockGeneratorT->SpreadSpectrumByteOffset] &= ~(mSupportedClockGeneratorT->SpreadSpectrumBitOffset);
    }

#ifdef EFI_DEBUG
    {
      UINT8   i;
      DEBUG ((EFI_D_ERROR, "SlaveAddress.SmbusDeviceAddress %x: Size =%x\n", SlaveAddress.SmbusDeviceAddress, Length));
      for (i = 0; i < ConfigurationTableLength; i++) {
        DEBUG ((EFI_D_ERROR, "Default Clock Generator Byte %d: %x\n", i, Buffer[i]));
      }
    }
#endif

    //
    // Program clock generator, using the platform default values
    //

  SmbErrorsCounter = 0;
  do
  {
    Command = 0;
    Status = SmbusPpi->Execute (
      SmbusPpi,
      SlaveAddress,
      Command,
      EfiSmbusWriteBlock,
      FALSE,
      &Length,  // &ConfigurationTableLength,
      Buffer   //ConfigurationTable
      );
   // ASSERT_EFI_ERROR (Status);


     if(Status != EFI_SUCCESS)
     {
       DEBUG ((EFI_D_ERROR, "SMBUS writing error\n"));
     }

     SmbErrorsCounter ++;
   }
   while ((Status != EFI_SUCCESS) && (SmbErrorsCounter < MAX_SMBU_RETRIES));

    //
    // Dump contents after write
    //
#ifdef EFI_DEBUG
    {
      UINT8   i;
    SlaveAddress.SmbusDeviceAddress = ClockAddress >> 1;
    Length = sizeof (Buffer);
    Command = 0;
    Status =  SmbusPpi->Execute (
        SmbusPpi,
        SlaveAddress,
        Command,
        EfiSmbusReadBlock,
        FALSE,
        &Length,
        Buffer
        );

      for (i = 0; i < ConfigurationTableLength; i++) {
        DEBUG ((EFI_D_ERROR, "Clock Generator Byte %d: %x\n", i, Buffer[i]));
      }
    }
#endif

    return EFI_SUCCESS;
  } // else (<not Kahuna>)
}


/**

  Configure clock generator on Kahuna
  Clock gen is on a secondary SMBus

  @param IN EFI_PEI_SERVICES         **PeiServices - Pointer to PEI Services table
  @param IN EFI_PEI_SMBUS2_PPI       *SmbusPpi - Pointer to SMBUs services PPI
  @param IN UINT8                    ClockAddress - SMBus address of clock gen
  @param IN UINT8                    *Buffer - Pointer to buffer containing byte stream to send to clock gen
  @param IN UINTN                    Length - Number of bytes in buffer

  @retval EFI_SUCCESS    The function completed successfully.
  @retval EFI_INVALID_PARAMETER     The function cannot continue due to length is out of bound.

**/
EFI_STATUS
ConfigureClockGeneratorOnSecondarySmbus (
  IN EFI_PEI_SERVICES         **PeiServices,
  IN EFI_PEI_SMBUS2_PPI       *SmbusPpi,
  IN UINT8                    ClockAddress,
  IN UINTN                    ConfigurationTableLength,
  IN BOOLEAN                  EnableSpreadSpectrum,
  IN CLOCK_GENERATOR_DETAILS  *mSupportedClockGeneratorT,
  IN OUT UINT8                *ConfigurationTable
  )
{
  EFI_PEI_STALL_PPI           *StallPpi;
  EFI_STATUS                  Status;
  EFI_SMBUS_DEVICE_ADDRESS    SlaveAddress;
  EFI_SMBUS_DEVICE_COMMAND    SmbusCommand;
  UINTN                       SmbusLength;
  UINT8                       SmbusData[32];
  UINT8                       Buffer[MAX_CLOCK_GENERATOR_BUFFER_LENGTH];
  UINT8                       Length;

  ZeroMem(Buffer, sizeof(Buffer));
  if (ConfigurationTableLength > MAX_CLOCK_GENERATOR_BUFFER_LENGTH) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate Stall PPI
  //
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gEfiPeiStallPpiGuid,
                            0,
                            NULL,
                            &StallPpi
                            );
  ASSERT_EFI_ERROR (Status);

  //
  // Get length of payload to send to clock gen
  //
  Length = (UINT8) ConfigurationTableLength;

  //
  // Copy the default clock generator data into Buffer
  //
  CopyMem((VOID*)Buffer, ConfigurationTable, Length);

  //
  // Set spread spectrum bit in Buffer or clear it?
  //
  if (EnableSpreadSpectrum) {
    Buffer[mSupportedClockGeneratorT->SpreadSpectrumByteOffset] |= mSupportedClockGeneratorT->SpreadSpectrumBitOffset;
  } else {
    Buffer[mSupportedClockGeneratorT->SpreadSpectrumByteOffset] &= ~(mSupportedClockGeneratorT->SpreadSpectrumBitOffset);
  }

  //
  // Now encapsulate the data for a Write Block command to the clock gen
  //  as the payload in a Write Block command to the SMBus bridge
  //
  //
  // Segment address = 0xF2, this becomes slave address
  // Slave address (clock gen) = ClockAddress, this becomes slave command
  //
  SlaveAddress.SmbusDeviceAddress = (0xF2 >> 1);
  SmbusCommand = ClockAddress;

  // Set byte index in clock gen to start with, always 0
  //
  SmbusData[0] = 0;

  // Set byte count clock gen wants to see
  //
  SmbusData[1] = (UINT8) Length;

  // Payload byte count for SMBus bridge
  //
  SmbusLength = Length + 2;
  if (SmbusLength > MAX_CLOCK_GENERATOR_BUFFER_LENGTH) {
    return EFI_INVALID_PARAMETER;
  }

  // Copy the clock gen data to the SMBus buffer
  //
  CopyMem((VOID *)(((UINT8*)SmbusData)+2), (VOID *)Buffer, Length);

  //
  // Use EfiSmbusWriteBlock to communicate with clock gen
  //
  Status = SmbusPpi->Execute( SmbusPpi,
                              SlaveAddress,
                              SmbusCommand,
                              EfiSmbusWriteBlock,
                              FALSE,
                              &SmbusLength,
                              &SmbusData );
  return Status;
}
