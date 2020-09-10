/** @file
  The SMM implementation of the SpdAccessLib.

  This library exposes the SPD read/write functions for use during SMM.

  This implementation works as an interface between an agent that needs
  to read/write to a DIMM SPD and the processor SMBus library which handles
  the actual transactions.  The read/write functions accept DIMM location
  information as well as the SPD byte offset and handles the steps necessary
  to initiate the SMBus transaction.  Setup is accomplished via a constructor
  which retrieves DIMM population information and initializes the cached
  page data area.

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


#include <SysRegs.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <SpdAccessLibCommon.h>
#include <Library/MemRcLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/HobLib.h>
#include <Library/SemaphoreLib.h>
#include <Library/SpdAccessLib.h>


//
// Module variable to keep track of system DIMM population
//
BOOLEAN mDimmPop[MAX_SOCKET][MAX_CH][MAX_DIMM];

//
// Module variable to keep track of DDR technology present
//
UINT16 DramType;

//
// Module variable to keep track of maximum offset based on DDR technology present
//
UINT16 MaxOffset;

//
// Module variable to keep track of maximum offset based on DDR technology present
//
UINT16 MinProgOffset;

BOOLEAN mInitialized = FALSE;

/**
  The constructor function retrieves and saves the DIMM population config and
  initializes the SPD data.

  The constructor function locates the System Memory Map HOB.
  It will ASSERT() if that operation fails and it will always return EFI_SUCCESS.

  @param[in] ImageHandle  - The firmware allocated handle for the EFI image.
  @param[in] SystemTable  - A pointer to the EFI System Table.

  @retval EFI_SUCCESS   - DIMM population successfully retrieved.
  @retval EFI_NOT_FOUND - Error locating DIMM population info HOB.
**/

VOID
InitializeSpdAccessLib (
  VOID
  )
{
  struct SystemMemoryMapHob  *SystemMemoryMap;
  UINT8  Socket;
  UINT8  Ch;
  UINT8  Dimm;
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  if (MaxChDdr > MAX_CH) {
     MaxChDdr = MAX_CH;
  }
  SystemMemoryMap = NULL;

  // Retrieve system memory map data which contains dimm population info
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return;
  }

  DramType = SystemMemoryMap->DramType;
  if (DramType == SPD_TYPE_DDR5) {
    MaxOffset = MAX_SPD_BYTE_DDR5;
    MinProgOffset = SPD_DDR5_EUP_RANGE;
  } else {
    MaxOffset = MAX_SPD_BYTE_DDR4;
    MinProgOffset = SPD_DDR4_EUP_RANGE;
  }

  // Populate DIMM population info for use in SMM
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (SystemMemoryMap->Socket[Socket].ChannelInfo[Ch].DimmInfo[Dimm].Enabled) {
          mDimmPop[Socket][Ch][Dimm] = TRUE;
        } else {
          mDimmPop[Socket][Ch][Dimm] = FALSE;
        }
      }
    }
  }

  mInitialized = TRUE;

}

/**
  Determines the appropriate page based on the SPD offset and initiates
  a SMBus transaction to select the page.  Also converts the SPD byte
  offset to a valid SMBus address offset.  This function is specific to
  DDR4.

  @param[in]  ByteOffset     - SPD Byte Offset to be read/written and converted
  @param[in]  SpdDev         - Pointer to a SMB_DEVICE structure
  @param[out] SmbOffset      - Pointer which will store the SMBus offset

  @retval EFI_SUCCESS       Page and offset successfully set
  @retval EFI_DEVICE_ERROR  Error occured during SMBus transaction
**/
EFI_STATUS
SelectPageAndOffsetDdr4 (
  IN UINT16             ByteOffset,
  IN SMB_DEVICE_STRUCT  *SpdDev,
  OUT UINT8             *SmbOffset
  )
{
  EFI_STATUS                          Status;
  SMB_DEVICE_STRUCT                   SpdPageDev;
  INT16                               PageOffset;
  UINT8                               Data;

  Status = EFI_SUCCESS;
  PageOffset = 0;
  SpdPageDev = *SpdDev;
  Data = 0;

  SpdPageDev.address.deviceType = DTI_WP_EEPROM;

  // Select appropriate page based on SPD ByteOffset
  if (ByteOffset > 255) {

    // Set page
    SpdPageDev.address.strapAddress = SPD_EE_PAGE_SELECT_1;

    // Write to SPA
    // Some EEPROM devices while sending PAGE selection command
    // will respond with NACK, which causes the SMBUS error status bit to set
    // This will cause DIMM detection failure. So we will not be checking the return status
    // for the below Write SMBUS function call.
    WriteProcSmb (SpdPageDev, SMB_PAGE_ADDR, &Data);
    PageOffset = -256;

  } else {

    // Set page
    SpdPageDev.address.strapAddress = SPD_EE_PAGE_SELECT_0;

    // Write to SPA
    // Some EEPROM devices while sending PAGE selection command
    // will respond with NACK, which causes the SMBUS error status bit to set
    // This will cause DIMM detection failure. So we will not be checking the return status
    // for the below Write SMBUS function call.
    WriteProcSmb (SpdPageDev, SMB_PAGE_ADDR, &Data);
  }
  *SmbOffset = (UINT8) (ByteOffset + PageOffset);

  return Status;
}

/**
  Determines the appropriate page based on the SPD offset and initiates
  a SMBus transaction to select the page.  Also converts the SPD byte
  offset to a valid SMBus address offset.  This function is specific to
  DDR5.

  @param[in]  ByteOffset     - SPD Byte Offset to be read/written and converted
  @param[in]  SpdDev         - Pointer to a SMB_DEVICE structure
  @param[out] SmbOffset      - Pointer which will store the SMBus offset

  @retval EFI_SUCCESS       Page and offset successfully set
  @retval EFI_DEVICE_ERROR  Error occured during SMBus transaction
**/
EFI_STATUS
SelectPageAndOffsetDdr5 (
  IN UINT16             ByteOffset,
  IN SMB_DEVICE_STRUCT  *SpdDev,
  OUT UINT8             *SmbOffset
  )
{
  EFI_STATUS                          Status;
  SMB_DEVICE_STRUCT                   SpdPageDev;
  UINT16                              PageOffset;
  SPD_DDR5_MR11_STRUCT                Mr11Data;
  SPD_DDR5_ADDRESS_FIRST_BYTE_STRUCT  SmbAddressData;
  UINT8                               SpdPage;

  Status = EFI_SUCCESS;
  PageOffset = 0;
  SpdPageDev = *SpdDev;
  Mr11Data.Data = 0;
  SmbAddressData.Data = 0;

  // DDR5 pages are broken up into 128 bit chunks
  SpdPage = (UINT8) (ByteOffset / SPD_DDR5_PAGE_SIZE);
  PageOffset = SPD_DDR5_PAGE_SIZE * SpdPage;

  if (GetSlaveType ((UINT8) SpdDev->address.SmbSocket, (UINT8) SpdDev->address.SmbInstance) == I2C_DEVICE) {
    // MemReg should be set to 0 for selecting page
    SmbAddressData.Bits.MemReg = 0;
    SmbAddressData.Bits.Address = SMB_MR11_ADDR;

    // Always use 1 Byte addressing
    Mr11Data.Bits.I2cLegacyMode = 0;
    Mr11Data.Bits.Page = SpdPage;

    // Write MR11 to set page
    Status = WriteProcSmb (SpdPageDev, SmbAddressData.Data, &Mr11Data.Data);
    SmbAddressData.Data = 0;
  }

  // MemReg should be set to 1 for NVM addressing
  SmbAddressData.Bits.MemReg = 1;
  SmbAddressData.Bits.Address = (UINT8) (ByteOffset - PageOffset);

  *SmbOffset = SmbAddressData.Data;

  return Status;
}

/**
  Determines the appropriate page based on the SPD offset and initiates
  a SMBus transaction to select the page.  Also converts the SPD byte
  offset to a valid SMBus address offset.

  @param[in]  ByteOffset     - SPD Byte Offset to be read/written and converted
  @param[in]  SpdDev         - Pointer to a SMB_DEVICE structure
  @param[out] SmbOffset      - Pointer which will store the SMBus offset

  @retval EFI_SUCCESS       Page and offset successfully set
  @retval EFI_DEVICE_ERROR  Error occured during SMBus transaction
**/
EFI_STATUS
SelectPageAndOffset (
  IN UINT16             ByteOffset,
  IN SMB_DEVICE_STRUCT  *SpdDev,
  OUT UINT8             *SmbOffset
  )
{
  EFI_STATUS                          Status;

  if (mInitialized == FALSE) {
    InitializeSpdAccessLib ();
  }

  if (DramType == SPD_TYPE_DDR5) {
    Status = SelectPageAndOffsetDdr5 (ByteOffset, SpdDev, SmbOffset);
  } else {
    Status = SelectPageAndOffsetDdr4 (ByteOffset, SpdDev, SmbOffset);
  }

  return Status;
}

/**
  Initializes any SPD related data structures and prepares the system for
  initiating SPD reads via the SMBus if necessary.  This must be called
  before attempting to read/write to SPD.

  @param[in] Socket   - Socket ID
  @param[in] DdrType  - Type of DDR Technology supported

  @retval EFI_SUCCESS       Successfully initialized SPD related functionality
  @retval EFI_DEVICE_ERROR  Error initializing devices
**/
EFI_STATUS
EFIAPI
InitializeSpd (
  IN UINT8                Socket,
  IN DDR_TECHNOLOGY_TYPE  DdrType
  )
{
  return EFI_SUCCESS;
}

/**
  Performs SPD byte read operations.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  ByteOffset  - SPD Byte Offset to be read
  @param[out] Data        - Pointer to buffer to store read value

  @retval RETURN_SUCCESS         The read completed successfully
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_NOT_READY          Unable to acquire semaphore
**/
EFI_STATUS
EFIAPI
SpdReadByte (
  IN UINT8      Socket,
  IN UINT8      Chan,
  IN UINT8      Dimm,
  IN UINT16     ByteOffset,
  OUT UINT8     *Data
  )
{
  EFI_STATUS         Status;
  SMB_DEVICE_STRUCT  SpdDev;
  UINT8              SmbOffset;
  BOOLEAN            SemAcquired;
  UINT32             QueueNumber;
  UINT8              SmbSocket;

  if (mInitialized == FALSE) {
    InitializeSpdAccessLib ();
  }

  // Sanity check incoming data
  if (Socket >= MAX_SOCKET || Chan >= MAX_CH || Dimm >= MAX_DIMM || ByteOffset >= MaxOffset || Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // If specified DIMM is not present, return error
  if (!mDimmPop[Socket][Chan][Dimm]) {
    return EFI_NOT_FOUND;
  }

  SpdDev.address.SmbSocket = Socket;
  Status = PrepSmbData (Socket, Chan, Dimm, &SpdDev);

  SmbSocket = (UINT8) SpdDev.address.SmbSocket;
  SemAcquired = AcquireSocketSemaphore (SmbSocket, SystemSemaphore0, DO_NOT_ADD_TO_QUEUE, &QueueNumber);

  // If Semaphore not acquired, do not attempt read.  Return not ready.
  if (!SemAcquired) {
    return EFI_NOT_READY;
  }

  // Select Page
  Status = SelectPageAndOffset (ByteOffset, &SpdDev, &SmbOffset);

  if (EFI_ERROR (Status)) {
    ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);
    return Status;
  }

  SpdDev.SpdPage = CalculateSpdPageNum (DramType, ByteOffset);

  // Read from SPD
  Status = ReadProcSmb (SpdDev, SmbOffset, Data);

  ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);

  return Status;
}

/**
  Performs SPD byte write operations.

  @param[in] Socket      - Socket ID
  @param[in] Chan        - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[in] ByteOffset  - SPD Byte Offset to be written
  @param[in] Data        - Pointer to data to write

  @retval RETURN_SUCCESS         The write completed successfully.
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_NOT_READY          Unable to acquire semaphore
**/
EFI_STATUS
EFIAPI
SpdWriteByte (
  IN  UINT8   Socket,
  IN  UINT8   Chan,
  IN  UINT8   Dimm,
  IN  UINT16  ByteOffset,
  IN  UINT8   *Data
  )
{
  EFI_STATUS         Status;
  SMB_DEVICE_STRUCT  SpdDev;
  UINT8              SmbOffset;
  BOOLEAN            SemAcquired;
  UINT32             QueueNumber;
  UINT8              SmbSocket;

  if (mInitialized == FALSE) {
    InitializeSpdAccessLib ();
  }

  // Sanity check incoming data
  if (Socket >= MAX_SOCKET || Chan >= MAX_CH || Dimm >= MAX_DIMM || ByteOffset < MinProgOffset || ByteOffset >= MaxOffset || Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  // If specified DIMM is not present, return error
  if (!mDimmPop[Socket][Chan][Dimm]) {
    return EFI_NOT_FOUND;
  }

  SpdDev.address.SmbSocket = Socket;
  Status = PrepSmbData (Socket, Chan, Dimm, &SpdDev);

  SmbSocket = (UINT8) SpdDev.address.SmbSocket;
  SemAcquired = AcquireSocketSemaphore (SmbSocket, SystemSemaphore0, DO_NOT_ADD_TO_QUEUE, &QueueNumber);

  // If Semaphore not acquired, do not attempt write.  Return not ready.
  if (!SemAcquired) {
    return EFI_NOT_READY;
  }

  Status = SelectPageAndOffset (ByteOffset, &SpdDev, &SmbOffset);

  if (EFI_ERROR (Status)) {
    ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);
    return Status;
  }

  SpdDev.SpdPage = CalculateSpdPageNum (DramType, ByteOffset);

  // Write to SPD, Do not attempt to read back in SMM
  Status = WriteProcSmb (SpdDev, SmbOffset, Data);

  ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);

  return Status;
}

/**
  Indicates to the Library that DIMM detection is complete.

  @param  None

  @retval RETURN_SUCCESS  DIMM detection done successfully communicated.
  @retval EFI_NOT_READY   Cannot store DIMM detection done indicator.
**/
EFI_STATUS
EFIAPI
SpdDimmDetectDone (
  VOID
  )
{
  // DIMM Detection is complete by SMM.
  return EFI_SUCCESS;
}

/**
  Performs read operations to SPD Hub internal register (Integrated Thermal Sensor)

  @param[in]  Socket      - Socket ID
  @param[in]  Channel     - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  Register    - Internal register to be read
  @param[out] Data        - Pointer to buffer to store read value

  @retval RETURN_SUCCESS         The read completed successfully
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
**/
EFI_STATUS
EFIAPI
SpdHubReadByte (
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Register,
  OUT UINT8     *Data
  )
{
  // SPD Hub access is complete by SMM.
  return EFI_SUCCESS;
}

/**
  Performs write operations to SPD Hub internal register (Integrated Thermal Sensor)

  @param[in] Socket      - Socket ID
  @param[in] Channel     - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[in] Register    - Internal register to be written
  @param[in] Data        - Pointer to data to write

  @retval RETURN_SUCCESS         The write completed successfully.
  @retval EFI_DEVICE_ERROR       Error configuring or during SMBus transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
**/
EFI_STATUS
EFIAPI
SpdHubWriteByte (
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Register,
  IN UINT8     *Data
  )
{
  // SPD Hub access is complete by SMM.
  return EFI_SUCCESS;
}
