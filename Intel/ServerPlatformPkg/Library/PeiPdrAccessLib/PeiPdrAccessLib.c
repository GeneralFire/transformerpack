/** @file
  A Library to support PlatformDataRegion access in PEI Phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Ppi/Spi.h>
#include <Library/HobLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiPdrAccessLib.h>
#include <Guid/SetupVariable.h>
#include <Library/IoLib.h>
#include <Register/PchRegsSpi.h>
#include <PchAccess.h>
#include <Library/MmPciBaseLib.h>

#define BLOCK_SIZE_64KB            0x10000
#define BLOCK_SIZE_32KB            0x8000
#define PDR_REGION_START_OFFSET    0x0
#define PDR_REGION_START_OFFSET2   0x8000
#define UUID_OFFSET                0
#define PCAT_RTC_ADDRESS_REGISTER  0x70
#define PCAT_RTC_DATA_REGISTER     0x71


/**
  Update PlatformDataRegion information, not only for UUID.
  This routine reads the PDR information - 64KB size to memory buffer,
  and backup the PDR data, update memory buffer and then write back to PDR.

  @param[in] PeiServices   Describes the list of possible PEI Services.
  @param[in] Buffer        Data to update.
  @param[in] DestOffset    Region to update, the offset of PDR.
  @param[in] BufferSize    The size of data to update.

  @return EFI_STATUS

**/
EFI_STATUS
UpdatePlatDataArea (
  IN  CONST EFI_PEI_SERVICES  **PeiServices,
  IN  UINT8                   *Buffer,
  IN  UINT32                  DestOffset,
  IN  UINT32                  BufferSize
)
{
  EFI_STATUS            Status;
  PCH_SPI_PROTOCOL      *SpiPpi;
  UINT8                 *BackUpBlock1, *BackUpBlock2, *TargetAddress;
  UINT8                 *BackUpBlock1Start, *BackUpBlock2Start;
  UINT32                DescSize;
  UINT32                Index;
  UINTN                 SpiBaseAddress;
  UINT8                 BiosControl;

  SpiBaseAddress = MmPciBase(
    DEFAULT_PCI_BUS_NUMBER_PCH,
    PCI_DEVICE_NUMBER_PCH_SPI,
    PCI_FUNCTION_NUMBER_PCH_SPI
    );

  //
  // Locate the SPI PPI Interface
  //
  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gPchSpiPpiGuid,
                    0,
                    NULL,
                    &SpiPpi
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Ensure and Clear EISS before attempting SPI flash update; EISS can be modified only if it is not locked [Bios Control:Bit1-Lock Enable]
  // Platform data area is updated with new UUID.
  //
  BiosControl = MmioRead8(SpiBaseAddress + R_SPI_CFG_BC);
  if (BiosControl & B_SPI_CFG_BC_EISS) {
    MmioAnd8(SpiBaseAddress + R_SPI_CFG_BC, (UINT8)~B_SPI_CFG_BC_EISS);
  }
  //
  // SPI Write operation requires a block erase since even for a single bit the erase operation will clear an
  // entire block so before we attempt a write we must find a way to back up the block that it will be erased.
  //
  DescSize = BLOCK_SIZE_32KB;
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             DescSize,
                             &BackUpBlock1
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AllocatePool BackUpBlock1 Status:%r\n",Status));
    goto PlatDataAreaEnd;
  }

  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             DescSize,
                             &BackUpBlock2
                             );
  if (EFI_ERROR (Status)) {
    goto PlatDataAreaEnd;
  }

  BackUpBlock1Start = BackUpBlock1;
  BackUpBlock2Start = BackUpBlock2;
  //
  // Read the PDR region, copy to buffer
  //
  Status = SpiPpi->FlashRead (
                              SpiPpi,
                              FlashRegionPlatformData,
                              PDR_REGION_START_OFFSET,
                              DescSize,
                              (UINT8 *)BackUpBlock1
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FlashRead the first 32kb Status:%r\n",Status));
    goto PlatDataAreaEnd;
  }
  Status = SpiPpi->FlashRead (
                              SpiPpi,
                              FlashRegionPlatformData,
                              PDR_REGION_START_OFFSET2,
                              DescSize,
                              (UINT8 *)BackUpBlock2
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FlashRead the second 32kb Status:%r\n",Status));
    goto PlatDataAreaEnd;
  }
  //
  // Adjust the BackupBlock pointer to the offset passed in, and copy the data
  // in the buffer passed in to the BackupBlock
  //
  if (DestOffset < DescSize) {
    TargetAddress  = BackUpBlock1 + DestOffset;
  } else {
    TargetAddress  = BackUpBlock2 + (DestOffset - DescSize);
  }
  for (Index = 0; Index < BufferSize; Index++, TargetAddress++) {
    *TargetAddress = Buffer[Index];
  }
  //
  //Erase the PDR
  //
  Status = SpiPpi->FlashErase (
                          SpiPpi,
                          FlashRegionPlatformData,
                          PDR_REGION_START_OFFSET,
                          BLOCK_SIZE_64KB
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FlashErase Status:%r\n",Status));
    goto PlatDataAreaEnd;
  }
  //
  // Write back the PDR with the new data added.
  //
  Status = SpiPpi->FlashWrite(
                          SpiPpi,
                          FlashRegionPlatformData,
                          PDR_REGION_START_OFFSET,
                          DescSize,
                          (UINT8 *)BackUpBlock1
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FlashWrite back the first 32 Status:%r\n",Status));
    goto PlatDataAreaEnd;
  }

  Status = SpiPpi->FlashWrite(
                          SpiPpi,
                          FlashRegionPlatformData,
                          PDR_REGION_START_OFFSET2,
                          DescSize,
                          (UINT8 *)BackUpBlock2
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UUID Update failed !\n"));
  }

PlatDataAreaEnd:
  //
  // Restore EISS value to BC register
  //
  if (BiosControl & B_SPI_CFG_BC_EISS) {
    MmioWrite8(SpiBaseAddress + R_SPI_CFG_BC, BiosControl);
  }
  return Status;
}


/**
  Return a pointer to the PDR (Platform Data Region) on SPI.

  @param  PeiServices      Describes the list of possible PEI Services.
  @param  Pdr              A pointer to PLATFORM_PDR_TABLE, which contains PDR information.

  @return EFI_STATUS

**/
EFI_STATUS
PlatDataRegRead (
  IN  CONST EFI_PEI_SERVICES     **PeiServices,
  OUT PLATFORM_PDR_TABLE         *Pdr
  )
{
  EFI_STATUS              Status;
  PCH_SPI_PROTOCOL        *SpiPpi;

  //
  // Locate the SPI PPI Interface
  //
  Status = (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gPchSpiPpiGuid,
                    0,
                    NULL,
                    &SpiPpi
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Cannot Locate gPchSpiPpiGuid!\n"));
    return Status;
  }
  //
  // Read the PIT (Platform Info Table) from the SPI Flash Platform Data Region
  //
  Status = SpiPpi->FlashRead(
                          SpiPpi,
                          FlashRegionPlatformData,
                          0,
                          sizeof (PLATFORM_PDR_TABLE),
                          (UINT8 *) Pdr
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "PIT from SPI PDR reports Platform ID invalidation. Status:%r!\n", Status));
  }
  return Status;
}


/**
  Read UUID from PDR and copy the value into the address provided.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to UUID info.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
UuidGetByPdr (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  OUT EFI_GUID                *UuidPtr
  )
{
  EFI_STATUS          Status;
  PLATFORM_PDR_TABLE  Pdr;

  Status = PlatDataRegRead (PeiServices, &Pdr);
  if (!EFI_ERROR (Status)) {
    CopyMem (UuidPtr, Pdr.SystemUuid, sizeof (EFI_GUID));
  }

  return Status;
}


/**
  The RP cannot have a UUID value of all 0's or F's for PXE PDK tests.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to UUID.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
RpUuidRefresh (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN OUT EFI_GUID             *UuidPtr
  )
{
  EFI_STATUS        Status;
  PCH_SPI_PROTOCOL  *SpiPpi;
  UINT8             UuidSize;
  UINT8             *pPIT;
  UINT8             i;
  BOOLEAN           UuidValid;

  Status    = EFI_SUCCESS;
  pPIT      = (UINT8 *)UuidPtr;
  UuidSize  = sizeof (EFI_GUID);
  UuidValid = FALSE;

  for (i = 0; i < UuidSize; i++) {
    if ( pPIT[i] != 0xff && pPIT[i] != 0x00 ) {
       UuidValid = TRUE;
       break;
    }
  }

  if (!UuidValid) {
    //
    // Locate the SPI PPI Interface
    //
    Status = (*PeiServices)->LocatePpi (
                      PeiServices,
                      &gPchSpiPpiGuid,
                      0,
                      NULL,
                      &SpiPpi
                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // fill the first six bytes of the UUID with internal GbE controller MAC address
    //
    Status = SpiPpi->FlashRead (
                          SpiPpi,
                          FlashRegionGbE,
                          0,
                          6,
                          pPIT
                          );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "RpUuidRefresh: ERROR -- Can't get GbE MAC address\n"));
      //
      // if error, fill with generic data
      //
      for (i = 0; i < 6; i++) {
        pPIT[i] = 0xa5;
      }
    }
    //
    // fill another six bytes with current time
    //
    pPIT[6]  = RtcReadDec (RTC_YEAR);
    pPIT[7]  = RtcReadDec (RTC_DAY);
    pPIT[8]  = RtcReadDec (RTC_MONTH);
    pPIT[9]  = RtcReadDec (RTC_HOURS);
    pPIT[10] = RtcReadDec (RTC_MINUTES);
    pPIT[11] = RtcReadDec (RTC_SECONDS);

    //
    // fill the rest with some generic filler
    //
    for (i = 12; i < UuidSize; i++) {
      pPIT[i] = 0xa5;
    }

    Status = UpdatePlatDataArea (
      PeiServices,
      (UINT8*)UuidPtr,
      UUID_OFFSET,
      UuidSize
      );
  }
  return Status;
}

/**
  Get variable MfgSystemUuid, which was programmed by tool in MFG Mode.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to variable MfgSystemUuid.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
GetMfgSystemUuidVar (
  IN  EFI_PEI_SERVICES  **PeiServices,
  OUT EFI_GUID           *UuidPtr
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *PeiVariable;
  UINTN                            SizeUUID;

  //
  // Locate the Setup configuration value.
  //
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gEfiPeiReadOnlyVariable2PpiGuid,
                            0,
                            NULL,
                            &PeiVariable
                            );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Check if MFG UUID variable is present
  //
  SizeUUID = sizeof (EFI_GUID);
  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"MfgSystemUuid",
                          &gEfiSetupVariableGuid,
                          NULL,
                          &SizeUUID,
                          UuidPtr
                          );

  return Status;
}

/**
  Check for variable containing the Mfg UUID, if exists, update PDR with new UUID,
  if not found, get UUID from PDR, and then sync to PlatformInfoHob.

  @param[in]  PeiServices   Describes the list of possible PEI Services.
  @param[out] UuidPtr       A pointer to UUID value.

  @retval EFI_SUCCESS   Function completed successfully.
  @retval others        An error occured.

**/
EFI_STATUS
MfgUuidGet (
  IN  EFI_PEI_SERVICES  **PeiServices,
  OUT EFI_GUID           *UuidPtr
  )
{
  EFI_STATUS          Status;

  Status = GetMfgSystemUuidVar (PeiServices, UuidPtr);
  if (!EFI_ERROR (Status)) {
    //
    // Update the PDR in flash with the UUID.
    //
    Status = UpdatePlatDataArea (
      PeiServices,
      (UINT8*)UuidPtr,
      UUID_OFFSET,
      sizeof (EFI_GUID)
      );
  } else {
    //
    // Failed to Get Variable MfgSystemUuid, get UUID from PDR
    //
    Status = UuidGetByPdr (PeiServices, UuidPtr);
  }

  return Status;
}

/**

  Read RTC data register and return contents after converting it to decimal.
  Needed here because it looks like other methods of getting time (ex. gRT->GetTime) tend to fail.

  @param Address RTC Register address to read
  @retval Value of data register contents converted to Decimal

**/
UINT8
RtcReadDec (
  IN RTC_ADDRESS Address
  )
{
  UINT8 High;
  UINT8 Low;
  UINT8 Result;

  IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, (UINT8)(Address | (UINT8)(IoRead8 (PCAT_RTC_ADDRESS_REGISTER) & 0x80)));
  Result = IoRead8 (PCAT_RTC_DATA_REGISTER);

  High = Result >> 4;
  Low = Result - (High << 4);

  return (UINT8) (Low + (High * 10));
}

