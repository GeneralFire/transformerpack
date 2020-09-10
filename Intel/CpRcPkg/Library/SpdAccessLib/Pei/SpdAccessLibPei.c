/** @file
  The PEI implementation of the SpdAccessLib.

  This library exposes the SPD read/write functions for use during PEI.

  This implementation works as an interface between an agent that needs
  to read/write to a DIMM SPD and the processor SMBus library which handles
  the actual transactions.  The read/write functions accept DIMM location
  information as well as the SPD byte offset and handles the steps necessary
  to initiate the SMBus transaction.  Setup is accomplished with a call to
  InitializeSpd which allocates memory for a structure to store SPD related
  information.

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

#include <SysHost.h>
#include <SysRegs.h>
#include <SpdAccessLibCommon.h>
#include <ScratchpadList.h>
#include <Library/SpdAccessLib.h>
#include "SpdAccessLibPei.h"
#include <Library/SysHostPointerLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SemaphoreLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SpdAccessTargetLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/HardwareSemaphoreTargetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/SystemInfoLib.h>
#include <Memory/I3cDefinitions.h>
#include <ReferenceCodeFatalErrors.h>

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
  BIOS_NON_STICKY_SCRATCHPAD5_STRUCT  CachedPageData;
  UINT32                              CachedPage;
  BOOLEAN                             UpdatePage;
  INT16                               PageOffset;
  UINT8                               Data;
  UINT8                               SmbSocket;

  Status = EFI_SUCCESS;
  PageOffset = 0;
  SpdPageDev = *SpdDev;
  Data = 0;
  UpdatePage = FALSE;
  CachedPage = SPD_PAGE_INIT_DDR4;
  CachedPageData.Data = 0;

  SpdPageDev.address.deviceType = DTI_WP_EEPROM;

  SmbSocket = (UINT8) SpdPageDev.address.SmbSocket;
  // Retrieve cached page data from scratchpad
  CachedPageData.Data = GetNonStickyScratchpad (SmbSocket, BIOS_NON_SCRATCHPAD_5);

  if (SpdDev->address.SmbInstance == 0) {
    CachedPage = CachedPageData.Bits.Seg0CachedPage;
  } else {
    CachedPage = CachedPageData.Bits.Seg1CachedPage;
  }

  // Select appropriate page based on SPD ByteOffset
  if (ByteOffset > 255) {

    // If current page does not match desired page
    if (CachedPage != 1) {
      // Set page
      SpdPageDev.address.strapAddress = SPD_EE_PAGE_SELECT_1;
      CachedPage = SPD_PAGE_ONE;
      UpdatePage = TRUE;

      // Write to SPA
      // Some EEPROM devices while sending PAGE selection command
      // will respond with NACK, which causes the SMBUS error status bit to set
      // This will cause DIMM detection failure. So we will not be checking the return status
      // for the below Write SMBUS function call.
      WriteProcSmb (SpdPageDev, SMB_PAGE_ADDR, &Data);
    }
    PageOffset = -256;

  } else {

    // If current page does not match desired page
    if (CachedPage != 0) {
      // Set page
      SpdPageDev.address.strapAddress = SPD_EE_PAGE_SELECT_0;
      CachedPage = SPD_PAGE_ZERO;
      UpdatePage = TRUE;

      // Write to SPA
      // Some EEPROM devices while sending PAGE selection command
      // will respond with NACK, which causes the SMBUS error status bit to set
      // This will cause DIMM detection failure. So we will not be checking the return status
      // for the below Write SMBUS function call.
      WriteProcSmb (SpdPageDev, SMB_PAGE_ADDR, &Data);
    }
  }

  *SmbOffset = (UINT8) (ByteOffset + PageOffset);

  // Save new cached page value if necessary
  if (UpdatePage) {
    if (SpdDev->address.SmbInstance == 0) {
      CachedPageData.Bits.Seg0CachedPage = CachedPage;
    } else {
      CachedPageData.Bits.Seg1CachedPage = CachedPage;
    }
    SetNonStickyScratchpad (SmbSocket, BIOS_NON_SCRATCHPAD_5, CachedPageData.Data);
  }

  return Status;
}

/**
  Determines the appropriate page based on the SPD offset and initiates
  a SMBus transaction to select the page.  Also converts the SPD byte
  offset to a valid SMBus address offset.  This function is specific to
  DDR5.

  @param[in]  Socket         - Socket ID
  @param[in]  ByteOffset     - SPD Byte Offset to be read/written and converted
  @param[in]  SpdDev         - Pointer to a SMB_DEVICE structure
  @param[out] SmbOffset      - Pointer which will store the SMBus offset
  @param[in]  SpdData        - Pointer to the SPD_ACCESS_DATA structure

  @retval EFI_SUCCESS       Page and offset successfully set
  @retval EFI_DEVICE_ERROR  Error occured during SMBus transaction
**/
EFI_STATUS
SelectPageAndOffsetDdr5 (
  IN UINT8              Socket,
  IN UINT16             ByteOffset,
  IN SMB_DEVICE_STRUCT  *SpdDev,
  OUT UINT8             *SmbOffset,
  IN SPD_ACCESS_DATA    *SpdData
  )
{
  EFI_STATUS                          Status;
  SMB_DEVICE_STRUCT                   SpdPageDev;
  UINT16                              PageOffset;
  SPD_DDR5_MR11_STRUCT                Mr11Data;
  SPD_DDR5_ADDRESS_FIRST_BYTE_STRUCT  SmbAddressData;
  UINT8                               SpdPage;
  UINT32                              CachedSegmentPages;
  UINT8                               CachedPage;
  BOOLEAN                             UpdatePage;

  Status = EFI_SUCCESS;
  PageOffset = 0;
  SpdPageDev = *SpdDev;
  Mr11Data.Data = 0;
  SmbAddressData.Data = 0;
  UpdatePage = TRUE;

  // Calculate page by dividing the offset by the page size
  SpdPage = (UINT8) (ByteOffset / SpdData->PageSize);
  PageOffset = SPD_DDR5_PAGE_SIZE * SpdPage;

  if (SpdData->Ddr5Caching) {
    // If caching is enabled, remote access to another socket's DIMM SPD is not allowed
    if (GetSysSocketId () != Socket) {
      RC_FATAL_ERROR (GetSysSocketId () == Socket, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_78);
    }
    if (SpdDev->address.SmbInstance == 0 ) {
      CachedSegmentPages = SpdData->Seg0Ddr5PageCache;
    } else {
      CachedSegmentPages = SpdData->Seg1Ddr5PageCache;
    }

    // Use DIMM StrapAddress to find relevant DIMM in cached pages
    CachedPage = (UINT8) (CachedSegmentPages >> (SpdDev->address.strapAddress * DDR5_PAGE_BITWIDTH));
    CachedPage &= DDR5_SPD_PAGE_MASK;

    if (CachedPage == SpdPage) {
      UpdatePage = FALSE;
    }
  }

  if (UpdatePage) {
    if (GetSlaveType (Socket, (UINT8) SpdDev->address.SmbInstance) == I2C_DEVICE) {
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

    if (Status == EFI_SUCCESS && SpdData->Ddr5Caching) {
      CachedSegmentPages &= ~(DDR5_SPD_PAGE_MASK << (SpdDev->address.strapAddress * DDR5_PAGE_BITWIDTH));
      CachedSegmentPages |= (SpdPage << (SpdDev->address.strapAddress * DDR5_PAGE_BITWIDTH));

      if (SpdDev->address.SmbInstance == 0 ) {
        SpdData->Seg0Ddr5PageCache = CachedSegmentPages;
      } else {
        SpdData->Seg1Ddr5PageCache = CachedSegmentPages;
      }
    }
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

  @param[in]  Socket         - Socket ID
  @param[in]  ByteOffset     - SPD Byte Offset to be read/written and converted
  @param[in]  SpdDev         - Pointer to a SMB_DEVICE structure
  @param[out] SmbOffset      - Pointer which will store the SMBus offset
  @param[in]  SpdData        - Pointer to the SPD_ACCESS_DATA structure

  @retval EFI_SUCCESS       Page and offset successfully set
  @retval EFI_DEVICE_ERROR  Error occured during SMBus transaction
**/
EFI_STATUS
SelectPageAndOffset (
  IN UINT8              Socket,
  IN UINT16             ByteOffset,
  IN SMB_DEVICE_STRUCT  *SpdDev,
  OUT UINT8             *SmbOffset,
  IN SPD_ACCESS_DATA    *SpdData
  )
{
  EFI_STATUS  Status;

  // Page selection and offset calculation differs for DDR4/DDR5
  if (SpdData->DdrType == Ddr5Type) {
    Status = SelectPageAndOffsetDdr5 (Socket, ByteOffset, SpdDev, SmbOffset, SpdData);
  } else {
    Status = SelectPageAndOffsetDdr4 (ByteOffset, SpdDev, SmbOffset);
  }

  return Status;
}

/**
  Stores DIMM presence information for use within library.

  @param[out] SpdData  - pointer SPD_ACCESS_DATA structure

  @retval VOID
**/
VOID
GetDimmPop (
  OUT SPD_ACCESS_DATA  *SpdData
  )
{
  struct dimmNvram     (*DimmNvList)[MAX_DIMM];
  struct channelNvram  (*ChannelNvList)[MAX_CH];
  UINT8                Socket;
  UINT8                Ch;
  UINT8                Dimm;

  PSYSHOST  Host;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  // Search for dimms
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    ChannelNvList = &Host->nvram.mem.socket[Socket].channelList;

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        // Record available dimms to SPD data structure
        if ((*DimmNvList)[Dimm].dimmPresent == 1) {
          SpdData->DimmPresent[Socket][Ch][Dimm] = TRUE;
        } else {
          SpdData->DimmPresent[Socket][Ch][Dimm] = FALSE;
        }
      }
    }
  }

}

/**
  Initializes data in the SPD_ACCESS_DATA structure and calls into ProcSmbIpLib
  to initialize the SMBus for the given socket.

  @param[in] Socket   - Socket ID
  @param[in] SpdData  - Pointer to a SPD_ACCESS_DATA structure
  @param[in] DdrType  - Type of DDR Technology supported

  @retval EFI_SUCCESS           Initialization data successfully populated
  @retval EFI_OUT_OF_RESOURCES  Unable to allocate space for data structure
  @retval EFI_NOT_READY         Host data not yet available
**/
EFI_STATUS
InitSpdData (
  IN UINT8                Socket,
  IN SPD_ACCESS_DATA      *SpdData,
  IN DDR_TECHNOLOGY_TYPE  DdrType
  )
{
  EFI_STATUS                          Status;
  UINT8                               Channel;
  UINT8                               Instance;
  BIOS_NON_STICKY_SCRATCHPAD5_STRUCT  CachedPageData;
  PSYSHOST                            Host;
  SYS_SETUP                           *Setup;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Status = EFI_SUCCESS;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  // At power on, by default, the devices come up in I2C mode of operation
  for (Instance = 0; Instance < MAX_SMB_INSTANCE; Instance++) {
    Host->var.mem.SmbMode[Socket][Instance] = I2C_MODE;
  }

  // Store memory technology present
  SpdData->DdrType = DdrType;

  // Only initialize the following on the first pass
  if (SpdData->FirstPass[Socket]) {
    // If this is the first call into SPD Access Lib in PEI, then DIMM detection is not yet done
    SpdData->DimmDetectDone = FALSE;

    // If DDR5 present, prep DDR5 data
    if (DdrType == Ddr5Type) {
      // Initialize DDR5 data
      SpdData->PageSize = SPD_DDR5_PAGE_SIZE;
      SpdData->MaxOffset = MAX_SPD_BYTE_DDR5;
      // DDR5 page caching is only supported when multi-threaded mode is enabled and XMP is disabled
      if ((Setup->mem.options & MULTI_THREAD_MRC_EN) && !(PcdGetBool (PcdMrcXmpSupport))) {
        SpdData->Ddr5Caching = TRUE;
        // Initialize each DIMMs cached page to SPD_PAGE_INIT_DDR5 (0xF) on each SMBus Instance
        SpdData->Seg0Ddr5PageCache = SPD_PAGE_INIT_DDR5;
        SpdData->Seg1Ddr5PageCache = SPD_PAGE_INIT_DDR5;
      } else {
        SpdData->Ddr5Caching = FALSE;
      }

    } else {  //Prep DDR4 data
      // Initialize DDR4 data
      SpdData->PageSize = SPD_EE_PAGE_SIZE;
      SpdData->MaxOffset = MAX_SPD_BYTE_DDR4;
      // Initialize cached page to 0x3 indicating no page is currently cached
      CachedPageData.Data = GetNonStickyScratchpad (Socket, BIOS_NON_SCRATCHPAD_5);
      CachedPageData.Bits.Seg0CachedPage = SPD_PAGE_INIT_DDR4;
      CachedPageData.Bits.Seg1CachedPage = SPD_PAGE_INIT_DDR4;
      SetNonStickyScratchpad (Socket, BIOS_NON_SCRATCHPAD_5, CachedPageData.Data);
    }
  }

  Host->nvram.mem.socket[Socket].ddr4SpdPageEn = 1;

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }
    SetFmcAccessMode (Socket, Channel, SMBUS);
  }

  return Status;
}

/**
  Retrieves the SpdData structure pointer from the static pointer table.  If
  it has not yet been initialized, resources are allocated and the pointer is set.

  @param[in, out] SpdData  - pointer to the SPD_ACCESS_DATA structure

  @retval RETURN_SUCCESS  SPD data structure pointer retrieved
  @retval EFI_NOT_READY   Cannot retrieve SPD Access data
**/
EFI_STATUS
GetSpdData (
  IN OUT SPD_ACCESS_DATA  **SpdData
  )
{
  EFI_STATUS       Status;
  SPD_ACCESS_DATA  *InitDataPtr;
  UINT8            Socket;
  UINT8            Chan;
  UINT8            Dimm;
  UINT8            MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Status = EFI_SUCCESS;
  InitDataPtr = NULL;

  *SpdData = (SPD_ACCESS_DATA *) GetSpdDataPointer ();

  // If first time calling into this library, initialize Semaphore Data
  if (*SpdData == NULL) {
    InitDataPtr = RcAllocatePool (sizeof (SPD_ACCESS_DATA));
    if (InitDataPtr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    // Store the pointer to Semaphore Owner data
    SetSpdDataPointer (InitDataPtr);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    // If pointer is still NULL, return error
    *SpdData = (SPD_ACCESS_DATA *) GetSpdDataPointer ();
    if (*SpdData == NULL) {
      return EFI_DEVICE_ERROR;
    }
    // Indicate this is first pass through initialize SPD
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

      (*SpdData)->FirstPass[Socket] = TRUE;

      // Stores the Last Spd Byte Data
      for (Chan = 0; Chan < MaxChDdr; Chan++) {
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          (*SpdData)->CacheLastSpdByte[Socket][Chan][Dimm].Offset = INVALID_SPD_OFFSET;
          (*SpdData)->CacheLastSpdByte[Socket][Chan][Dimm].Data = 0;
        }
      }
    }
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
  EFI_STATUS       Status;
  SPD_ACCESS_DATA  *SpdData;

  Status = EFI_SUCCESS;

  // Reserve mem for SpdData struct if not already done
  Status = GetSpdData (&SpdData);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  InitSpdData (Socket, SpdData, DdrType);

  // Init hardware agents if first pass through
  if (SpdData->FirstPass[Socket]) {
    InitSpdAgents (Socket);

    // Indicate InitializeSpd has already been called for this socket
    SpdData->FirstPass[Socket] = FALSE;
  }

  return Status;
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
  SPD_ACCESS_DATA    *SpdData;
  UINT8              SpdPage;
  UINT8              SmbSocket;

  Status = GetSpdData (&SpdData);

    // Sanity check incoming data
  if (Socket >= MAX_SOCKET || Chan >= MAX_CH || Dimm >= MAX_DIMM || Data == NULL || ByteOffset >= SpdData->MaxOffset) {
    return EFI_INVALID_PARAMETER;
  }

  // If DIMM detection is done, check DIMM population
  if (SpdData->DimmDetectDone) {
    if (!SpdData->DimmPresent[Socket][Chan][Dimm]) {
      return EFI_NOT_FOUND;
    }
  }

  // if this ByteOffset is the same with the CacheLastSpdByte
  if (!EFI_ERROR (Status) && (SpdData->CacheLastSpdByte[Socket][Chan][Dimm].Offset == ByteOffset)) {
    *Data = SpdData->CacheLastSpdByte[Socket][Chan][Dimm].Data;
    return Status;
  }

  SpdDev.address.SmbSocket = Socket;

  Status = PrepSmbData (Socket, Chan, Dimm, &SpdDev);

  SmbSocket = (UINT8) SpdDev.address.SmbSocket;
  AcquireSmbusSemaphoreControl (SmbSocket);

  // Select Page
  Status = SelectPageAndOffset (Socket, ByteOffset, &SpdDev, &SmbOffset, SpdData);

  // If error encountered while selecting page, release semaphore and exit
  if (EFI_ERROR (Status)) {
    ReleaseSmbusSemaphoreControl (SmbSocket);
    return Status;
  }

  // Calculate page by dividing the offset by the page size
  SpdPage = (UINT8) (ByteOffset / SpdData->PageSize);
  SpdDev.SpdPage = SpdPage;

  // Read from SPD
  Status = ReadSpdTarget (Socket, SpdPage, SpdData->DdrType, SpdDev, SmbOffset, Data);

  ReleaseSmbusSemaphoreControl (SmbSocket);

  // only cache when the Status is SUCCESS. if this ByteOffset is not the CacheLastSpdByte, save a new one.
  if (!EFI_ERROR (Status)) {
    SpdData->CacheLastSpdByte[Socket][Chan][Dimm].Offset = ByteOffset;
    SpdData->CacheLastSpdByte[Socket][Chan][Dimm].Data = *Data;
  }

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
**/
EFI_STATUS
EFIAPI
SpdWriteByte (
  IN UINT8   Socket,
  IN UINT8   Chan,
  IN UINT8   Dimm,
  IN UINT16  ByteOffset,
  IN UINT8   *Data
  )
{
  EFI_STATUS         Status;
  SMB_DEVICE_STRUCT  SpdDev;
  UINT8              SmbOffset;
  SPD_ACCESS_DATA    *SpdData;
  UINT8              ReadData;
  UINT8              SpdPage;
  UINT8              SmbSocket;

  Status = GetSpdData (&SpdData);

  // Sanity check incoming data
  if (Socket >= MAX_SOCKET || Chan >= MAX_CH || Dimm >= MAX_DIMM || Data == NULL || ByteOffset >= SpdData->MaxOffset) {
    return EFI_INVALID_PARAMETER;
  }

  // If DIMM detection is done, check DIMM population
  if (SpdData->DimmDetectDone) {
    if (!SpdData->DimmPresent[Socket][Chan][Dimm]) {
      return EFI_NOT_FOUND;
    }
  }

  SpdDev.address.SmbSocket = Socket;
  Status = PrepSmbData (Socket, Chan, Dimm, &SpdDev);

  SmbSocket = (UINT8) SpdDev.address.SmbSocket;
  AcquireSmbusSemaphoreControl (SmbSocket);

  // Select Page
  Status = SelectPageAndOffset (Socket, ByteOffset, &SpdDev, &SmbOffset, SpdData);

  // If error encountered while selecting page, release semaphore and exit
  if (EFI_ERROR (Status)) {
    ReleaseSmbusSemaphoreControl (SmbSocket);
    return Status;
  }

  // Calculate page by dividing the offset by the page size
  SpdPage = (UINT8) (ByteOffset / SpdData->PageSize);
  SpdDev.SpdPage = SpdPage;

  // Write to SPD
  Status = WriteSpdTarget (Socket, SpdPage, SpdData->DdrType, SpdDev, SmbOffset, Data);

  // If write was successful, read back data to ensure write has completed before returning.
  if (!EFI_ERROR (Status)) {
    Status = ReadSpdTarget (Socket, SpdPage, SpdData->DdrType, SpdDev, SmbOffset, &ReadData);
    if ((!EFI_ERROR (Status)) && (*Data != ReadData)) {
      Status = EFI_DEVICE_ERROR;
    } else {
      // Update the Cache incase we got a wrong SPD data from cache
      if (SpdData->CacheLastSpdByte[Socket][Chan][Dimm].Offset == ByteOffset) {
        SpdData->CacheLastSpdByte[Socket][Chan][Dimm].Data = *Data;
      }
    }
  }

  ReleaseSmbusSemaphoreControl (SmbSocket);

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
  EFI_STATUS       Status;
  SPD_ACCESS_DATA  *SpdData;

  Status = GetSpdData (&SpdData);

  // If unable to access SPD Data, return error
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  // Fill out DIMM population Data for use in PEI
  GetDimmPop (SpdData);

  // Indicate DIMM detection is done
  SpdData->DimmDetectDone = TRUE;

  return Status;
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
  EFI_STATUS         Status;
  SMB_DEVICE_STRUCT  SpdDev;
  SPD_ACCESS_DATA    *SpdData;
  UINT8              SmbSocket;
  SPD_DDR5_ADDRESS_FIRST_BYTE_STRUCT  SmbAddressData;

  SmbAddressData.Data = 0;

  Status = GetSpdData (&SpdData);

  // Sanity check incoming data
  if (Socket >= MAX_SOCKET || Channel >= MAX_CH || Dimm >= MAX_DIMM || Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // If DIMM detection is done, check DIMM population
  if (SpdData->DimmDetectDone) {
    if (!SpdData->DimmPresent[Socket][Channel][Dimm]) {
      return EFI_NOT_FOUND;
    }
  }

  if (SpdData->DdrType != Ddr5Type) {
    return EFI_DEVICE_ERROR;
  }

  SpdDev.address.SmbSocket = Socket;

  Status = PrepSmbData (Socket, Channel, Dimm, &SpdDev);

  SmbSocket = (UINT8) SpdDev.address.SmbSocket;
  AcquireSmbusSemaphoreControl (SmbSocket);

  //
  // MemReg: 1 for NVM location(SPD), 0 for internal register
  //
  SmbAddressData.Bits.MemReg = 0;
  SmbAddressData.Bits.Address = Register;
  Status = ReadProcSmb (SpdDev, SmbAddressData.Data, Data);

  ReleaseSmbusSemaphoreControl (SmbSocket);
  return Status;
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
  EFI_STATUS         Status;
  SMB_DEVICE_STRUCT  SpdDev;
  SPD_ACCESS_DATA    *SpdData;
  UINT8              SmbSocket;
  SPD_DDR5_ADDRESS_FIRST_BYTE_STRUCT  SmbAddressData;

  SmbAddressData.Data = 0;

  Status = GetSpdData (&SpdData);

  // Sanity check incoming data
  if (Socket >= MAX_SOCKET || Channel >= MAX_CH || Dimm >= MAX_DIMM || Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // If DIMM detection is done, check DIMM population
  if (SpdData->DimmDetectDone) {
    if (!SpdData->DimmPresent[Socket][Channel][Dimm]) {
      return EFI_NOT_FOUND;
    }
  }

  if (SpdData->DdrType != Ddr5Type) {
    return EFI_DEVICE_ERROR;
  }

  SpdDev.address.SmbSocket = Socket;

  Status = PrepSmbData (Socket, Channel, Dimm, &SpdDev);

  SmbSocket = (UINT8) SpdDev.address.SmbSocket;
  AcquireSmbusSemaphoreControl (SmbSocket);

  //
  // MemReg: 1 for NVM location(SPD), 0 for internal register
  //
  SmbAddressData.Bits.MemReg = 0;
  SmbAddressData.Bits.Address = Register;
  Status = WriteProcSmb (SpdDev, SmbAddressData.Data, Data);

  ReleaseSmbusSemaphoreControl (SmbSocket);
  return Status;
}