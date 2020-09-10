/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "SetupPlatform.h"
#include "Register/PchRegsPcie.h"
#include "PcieRegs.h"
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/SocketVariable.h>
#include <Guid/SetupVariable.h>
#include <Library/SataSocLib.h>


#define NEWSTRING_SIZE 0x200

extern EFI_GUID gSignalBeforeEnterSetupGuid;
extern EFI_GUID gEfiMfgUUIDHobGuid;

STATIC UINT16 RpSpeedStrings[] = {
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP1,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP2,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP3,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP4,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP5,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP6,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP7,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP8,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP9,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP10,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP11,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP12,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP13,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP14,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP15,
   STR_PCH_PCIE_SPEEDSTATUS_VALUE_RP16

};

STATIC UINT16 RpWidthStrings[] = {
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP1,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP2,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP3,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP4,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP5,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP6,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP7,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP8,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP9,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP10,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP11,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP12,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP13,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP14,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP15,
   STR_PCH_PCIE_WIDTHSTATUS_VALUE_RP16


};

EFI_GUID                      mProcessorProducerGuid;
VOID                          *mRegistration;
UINT64                        mMonotonicCount;
UINT16                        mMemorySpeed = 0xffff;
extern EFI_PLATFORM_TYPE_PROTOCOL    *mPlatformType;

/**

  Register event handler to find setup data records in Data Hub.

  @retval EFI_SUCCESS  -  The data hub filter event handler was registered.

**/
EFI_STATUS
FindDataRecords (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_EVENT                   FilterEvent;
  EFI_EVENT                   DiskInfoEvent;
  VOID                        *DiskInfo;
  VOID                        *FilterDummyRegistration;

  //
  // Register our Setup Data Filter Function.
  // This function is notified at EFI_TPL_CALLBACK.
  //
  Status = gBS->CreateEvent (
    EVT_NOTIFY_SIGNAL,
    TPL_CALLBACK,
    SetupDataFilter,
    NULL,
    &FilterEvent
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
    &gSignalBeforeEnterSetupGuid,
    FilterEvent,
    &FilterDummyRegistration
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->CreateEvent (
    EVT_NOTIFY_SIGNAL,
    TPL_CALLBACK,
    IdeCallback,
    NULL,
    &DiskInfoEvent
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
    &gEfiDiskInfoProtocolGuid,
    DiskInfoEvent,
    &DiskInfo
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**

    UpdateAhciString

    @param[in] ControllerNo
    @param[in] UpdateIndex

    @return String reference or 0 if not found

**/
EFI_STRING_ID
UpdateAhciString (
  IN  UINT8     SataCtrlIndex,
  IN  UINTN     UpdateIndex
  )
{
  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    switch (UpdateIndex) {
      case 0:
        return (STRING_REF) STR_SATA0_NAME;
      case 1:
        return (STRING_REF) STR_SATA1_NAME;
      case 2:
        return (STRING_REF) STR_SATA2_NAME;
      case 3:
        return (STRING_REF) STR_SATA3_NAME;
      case 4:
        return (STRING_REF) STR_SATA4_NAME;
      case 5:
        return (STRING_REF) STR_SATA5_NAME;
      case 6:
        return (STRING_REF) STR_SATA6_NAME;
      case 7:
        return (STRING_REF) STR_SATA7_NAME;
      default:
        return 0;
    }
  }
// APTIOV_SERVER_OVERRIDE_RC_START
/**  
  else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    switch (UpdateIndex) {
      case 0:
        return (STRING_REF) STR_SSATA0_NAME;
      case 1:
        return (STRING_REF) STR_SSATA1_NAME;
      case 2:
        return (STRING_REF) STR_SSATA2_NAME;
      case 3:
        return (STRING_REF) STR_SSATA3_NAME;
      case 4:
        return (STRING_REF) STR_SSATA4_NAME;
      case 5:
        return (STRING_REF) STR_SSATA5_NAME;
      default:
        return 0;
    }
  }
 **/
// APTIOV_SERVER_OVERRIDE_RC_END
  return 0;
}

/**

  IDE data filter function.

  @param[in] Event
  @param[in] Context

**/
VOID
EFIAPI
IdeCallback (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
{
  EFI_STATUS                      Status;
  UINTN                           HandleCount;
  EFI_HANDLE                      *HandleBuffer;
  EFI_DISK_INFO_PROTOCOL          *DiskInfo;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePathNode;
  PCI_DEVICE_PATH                 *PciDevicePath;
  UINTN                           Index;
  UINT8                           Index1;
  UINT32                          BufferSize;
  // Changing the variable datatype as UINT64 since UINT32 is not sufficient to store 4TB Hard Disk size.
// APTIOV_SERVER_OVERRIDE_RC_START : BIOS setup just show 801.7 GB for the HDD which is 4TB.
  UINT64                          DriveSize;       
// APTIOV_SERVER_OVERRIDE_RC_END : BIOS setup just show 801.7 GB for the HDD which is 4TB.
  UINT32                          IdeChannel;
  UINT32                          IdeDevice;
  EFI_IDENTIFY_DATA               *IdentifyDriveInfo;
  CHAR16                          *NewString;
  CHAR16                          SizeString[20];
  EFI_STRING_ID                   NameToUpdate;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  UINT8                           PciBuffer;
  UINT8                           SataMode;
  UINT8                           SataCtrlIndex;
  // APTIOV_SERVER_OVERRIDE_RC_START : Changes to support 4K Sector Size.
  UINT32                          SectorSize;
  // APTIOV_SERVER_OVERRIDE_RC_END : Changes to support 4K Sector Size.

  //
  // Assume no line strings is longer than 256 bytes.
  //
  NewString     = AllocateZeroPool (NEWSTRING_SIZE);
  PciDevicePath = NULL;

  if (NewString == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  NameToUpdate = (STRING_REF) STR_SATA0_NAME;
  //
  // Initialize IDE Combined mode
  //
  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, &PciRootBridgeIo);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = PciRootBridgeIo->Pci.Read (
    PciRootBridgeIo,
    EfiPciWidthUint8,
    0x1F020A,
    1,
    &PciBuffer
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }
  SataMode = PciBuffer;

  //
  // Fill IDE Information for IDE mode
  //
  Status = gBS->LocateHandleBuffer (
    ByProtocol,
    &gEfiDiskInfoProtocolGuid,
    NULL,
    &HandleCount,
    &HandleBuffer
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
      HandleBuffer[Index],
      &gEfiDevicePathProtocolGuid,
      (VOID *) &DevicePath
      );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return;
    }

    DevicePathNode = DevicePath;
    while (!IsDevicePathEnd (DevicePathNode)) {
      if ((DevicePathType (DevicePathNode) == HARDWARE_DEVICE_PATH) &&
          (DevicePathSubType (DevicePathNode) == HW_PCI_DP)
          ) {
        PciDevicePath = (PCI_DEVICE_PATH *) DevicePathNode;
        break;
      }

      DevicePathNode = NextDevicePathNode (DevicePathNode);
    }

    if (PciDevicePath == NULL) {
      continue;
    }
    //
    // Check for onboard SATA
    //
    for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum(); SataCtrlIndex++) {
      if ((SataDevNumber(SataCtrlIndex) == PciDevicePath->Device) && (SataFuncNumber(SataCtrlIndex) == PciDevicePath->Function)) {
        break;
      }
    }
    if (SataCtrlIndex < MaxSataControllerNum()) {
      Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiDiskInfoProtocolGuid, &DiskInfo);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return;
      }

      Status = DiskInfo->WhichIde (
        DiskInfo,
        &IdeChannel,
        &IdeDevice
        );
      // APTIOV_SERVER_OVERRIDE_RC_START : Fix for EIP #119521 (IdeCallback() in SetupInfoRecords.c causes ASSERT when SATA cdrom is 
      // connected and ScsiPassThruAtapi module is present in project.)
      //ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)){
        continue;
      }
      // APTIOV_SERVER_OVERRIDE_RC_END : Fix for EIP #119521 (IdeCallback() in SetupInfoRecords.c causes ASSERT when SATA cdrom is 
      // connected and ScsiPassThruAtapi module is present in project.)      

      BufferSize = sizeof (EFI_IDENTIFY_DATA);
      IdentifyDriveInfo = AllocatePool (BufferSize);

      if (IdentifyDriveInfo == NULL) {
        DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
        ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
        return;
      }

      Status = DiskInfo->Identify (
        DiskInfo,
        IdentifyDriveInfo,
        &BufferSize
        );
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return;
      }

      if (SataMode == PCI_CLASS_MASS_STORAGE_IDE) {   // IDE MODE
        Status = DiskInfo->WhichIde (
          DiskInfo,
          &IdeChannel,
          &IdeDevice
          );
        if (EFI_ERROR (Status)) {
          ASSERT_EFI_ERROR (Status);
          return;
        }

        IdeDevice = (IdeChannel * 2) + IdeDevice;
        // Port 0 = Primary Master
        // Port 2 = Primary Slave
        // Port 1 = Secondary Master
        // Port 3 = Secondary Slave
        if (IdeDevice == 1 || IdeDevice == 2) {   // SINDX Serial ATA Index (D31:F2)
          IdeDevice ^= 3;
        }
        if (PciDevicePath->Function == 5) {  //Port 4, 5 in SATA2
          IdeDevice += 4;
        }
        NameToUpdate = UpdateAhciString (
          SataCtrlIndex,
          IdeDevice
          );

      } else { // SATA or RAID MODE
        Status = DiskInfo->WhichIde (
          DiskInfo,
          &IdeChannel,
          &IdeDevice
          );
        if (EFI_ERROR (Status)) {
          ASSERT_EFI_ERROR (Status);
          return;
        }
        NameToUpdate = UpdateAhciString (
          SataCtrlIndex,
          IdeChannel
          );
      }

      SwapEntries ((CHAR8 *) &IdentifyDriveInfo->AtaData.ModelName, 40);
      AsciiToUnicode ((CHAR8 *) &IdentifyDriveInfo->AtaData.ModelName, NewString);

      // APTIOV_SERVER_OVERRIDE_RC_START : Changes to support 4K Sector Size.
      // Update sector size if necessary
      if ( (IdentifyDriveInfo->AtaData.phy_logic_sector_support & 0x4000) && (!(IdentifyDriveInfo->AtaData.phy_logic_sector_support & 0x8000)) && (IdentifyDriveInfo->AtaData.phy_logic_sector_support & 0x1000) ) {
          SectorSize = (UINT32)(IdentifyDriveInfo->AtaData.logic_sector_size_lo + (IdentifyDriveInfo->AtaData.logic_sector_size_hi << 16)) * 2;
      }
      else {
          SectorSize = 512;
      }
      // APTIOV_SERVER_OVERRIDE_RC_END : Changes to support 4K Sector Size.
      
      //
      // Chop it off after 14 characters
      //
      NewString[14] = 0;

      //
      // For HardDisk append the size. Otherwise display atapi
      //
      if ((IdentifyDriveInfo->AtaData.config & 0x8000) == 00) {
        //
        // 48 bit address feature set is supported, get maximum capacity
        //
        if ((IdentifyDriveInfo->AtaData.command_set_supported_83 & 0x0400) == 0) {
          DriveSize =
            (((((IdentifyDriveInfo->AtaData.user_addressable_sectors_hi << 16) +
                    IdentifyDriveInfo->AtaData.user_addressable_sectors_lo
                  ) / 1000
                // APTIOV_SERVER_OVERRIDE_RC_START : Changes to support 4K Sector Size.  
                ) * SectorSize
                // APTIOV_SERVER_OVERRIDE_RC_END : Changes to support 4K Sector Size.
              ) /
              1000
            );
        } else {
          DriveSize = IdentifyDriveInfo->AtaData.maximum_lba_for_48bit_addressing[0];
          for (Index1 = 1; Index1 < 4; Index1++) {
            DriveSize |= LShiftU64 (
              IdentifyDriveInfo->AtaData.maximum_lba_for_48bit_addressing[Index1],
              16 * Index1
              );
          }
          // APTIOV_SERVER_OVERRIDE_RC_START : Changes to support 4K Sector Size.
          DriveSize = (UINT32) DivU64x64Remainder (MultU64x32 (DivU64x64Remainder (DriveSize, 1000, 0), SectorSize), 1000, 0);
          // APTIOV_SERVER_OVERRIDE_RC_END : Changes to support 4K Sector Size.
        }

        StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), L" - ");
        UnicodeValueToStringS (SizeString, sizeof (SizeString), LEFT_JUSTIFY, DriveSize / 1000, 5);
        StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), SizeString);
        StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), L".");
        UnicodeValueToStringS (SizeString, sizeof (SizeString), LEFT_JUSTIFY, (DriveSize % 1000) / 100, 1);
        StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), SizeString);
        StrCatS (NewString, NEWSTRING_SIZE/sizeof(CHAR16), L" GB");
      } else {
        StrCatS (NewString, NEWSTRING_SIZE/sizeof (CHAR16), L" - ATAPI");
      }

      HiiSetString (mAdvancedHiiHandle, NameToUpdate, NewString, NULL); //Update Sata Port # page with info
      // APTIOV_SERVER_OVERRIDE_RC_START
      //HiiSetString(mMainHiiHandle, NameToUpdate, NewString, NULL);    //Update Main page with info as well
      // APTIOV_SERVER_OVERRIDE_RC_END
      gBS->FreePool (IdentifyDriveInfo);
    }
  }

  if (HandleBuffer != NULL) {
    gBS->FreePool (HandleBuffer);
  }

  gBS->FreePool (NewString);

  return;
}

/**

  Acquire the string associated with the Index from SMBIOS structure and return it.
  The caller is responsible for freeing the string buffer.

  @param[in]    OptionalStrStart  The start position to search the string
  @param[in]    Index             The index of the string to extract
  @param[out]   String            The string that is extracted
  @param[in]    StringSize        Size of the string

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS if successful.
  @retval   EFI_NOT_FOUND     The function returns EFI_NOT_FOUND if unsuccessful.

**/
EFI_STATUS
SmbiosGetOptionalStringByIndex (
  IN      CHAR8                   *OptionalStrStart,
  IN      UINT8                   Index,
  OUT     CHAR16                  **String,
  IN      UINTN                   StringSize
  )
{
  UINTN          StrSize;

  if (Index == 0) {
    return EFI_INVALID_PARAMETER;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart); // size includes null terminator
    //
    // SMBIOS strings are NULL terminated, and end of all strings is indicated with NULL
    // loop until at end of all SMBIOS strings (found NULL terminator at string index past string's NULL), and Index != 0
    //
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  if ((Index != 0) || (StrSize == 1)) {
    //
    // Meet the end of strings set but Index is non-zero
    //
    return EFI_INVALID_PARAMETER;
  } else {
    AsciiStrToUnicodeStrS (OptionalStrStart, *String, StringSize);
  }

  return EFI_SUCCESS;
}

/**

  Setup data filter function. This function is invoked when there is data records
  available in the Data Hub.

  @param[in] Event    -  the event that is signaled.
  @param[in] Context  -  not used here.

**/
VOID
EFIAPI
SetupDataFilter (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{

  EFI_STATUS                            Status;
// APTIOV_SERVER_OVERRIDE_RC_START : Commenting the unwanted Intel Setup Page  
#if 0
  UINT64                                MemSize = 0;
  CHAR16                                *NewString;
  CHAR16                                *NewString2;

  EFI_STRING_ID                         TokenToUpdate;
  VOID                                  *HobList;
  struct SystemMemoryMapHob             *mSystemMemoryMap;
  UINTN                                 DataSize;
  EFI_HOB_GUID_TYPE                     *UuidGuidHob;
  EFI_GUID                              *SystemUuid;
  EFI_SMBIOS_TABLE_HEADER               *SmbiosRecord;
  EFI_SMBIOS_HANDLE                     SmbiosHandle;
  EFI_SMBIOS_PROTOCOL                   *Smbios;
  SMBIOS_TABLE_TYPE0                    *SmbiosType0Record;
  SMBIOS_TABLE_TYPE19                   *SmbiosType19Record;
  UINT8                                 StrIndex;
  RC_VERSION                            *RcVersion;

#ifdef LT_FLAG
  UINT32                                *Ptr32;
  UINT32                                AcmVersion;
#endif
  DataSize    = 0;
  NewString   = AllocateZeroPool (NEWSTRING_SIZE);
  NewString2  = AllocateZeroPool (NEWSTRING_SIZE);

  if (NewString == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  if (NewString2 == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    gBS->FreePool (NewString);
    return;
  }

  Status = gBS->LocateProtocol (
    &gEfiSmbiosProtocolGuid,
    NULL,
    (VOID **) &Smbios
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Display the BIOS Version
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  while (!EFI_ERROR(Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &SmbiosRecord, NULL))){

    if (SmbiosRecord->Type == EFI_SMBIOS_TYPE_BIOS_INFORMATION) {
      SmbiosType0Record = (SMBIOS_TABLE_TYPE0 *) SmbiosRecord;
      StrIndex = SmbiosType0Record->BiosVersion;
      Status = SmbiosGetOptionalStringByIndex ((CHAR8*)((UINT8*)SmbiosType0Record + SmbiosType0Record->Hdr.Length),
                                               StrIndex, &NewString, (NEWSTRING_SIZE / sizeof (CHAR16)));
      if (Status == EFI_SUCCESS) {
        TokenToUpdate = (STRING_REF) STR_BIOS_REVISION_VALUE;
        HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);
      }
    }

    if (SmbiosRecord->Type == EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS) {
      SmbiosType19Record = (SMBIOS_TABLE_TYPE19 *) SmbiosRecord;
      // if mem populated > 4TB in system, SMBios 2.7.0 spec says to use Extended fields instead
      if ((SmbiosType19Record->StartingAddress == 0xFFFFFFFF) && (SmbiosType19Record->EndingAddress == 0xFFFFFFFF)) {
        MemSize += RShiftU64 (SmbiosType19Record->ExtendedEndingAddress, 10) - RShiftU64 (SmbiosType19Record->ExtendedStartingAddress, 10) + 1;
      } else {
        MemSize += SmbiosType19Record->EndingAddress - SmbiosType19Record->StartingAddress + 1;
      }
    }

  }

  //
  // Display the RC Version in Setup
  //
  RcVersion = (RC_VERSION *) PcdGetPtr (PcdRcVersion);
  UnicodeSPrint (
    NewString,
    NEWSTRING_SIZE,
    L"%d.%d.%d.%04x",
    RcVersion->Major,
    RcVersion->Minor,
    RcVersion->Revision,
    RcVersion->BuildNumber
    );
  HiiSetString (
    mMainHiiHandle,
    STRING_TOKEN (STR_RC_REVISION_VALUE),
    NewString,
    NULL
    );

  //
  // Display the Firmware Version in Setup
  //
  UnicodeSPrint (
    NewString,
    NEWSTRING_SIZE,
    L"0x%x",
    gST->FirmwareRevision
    );
  TokenToUpdate = (STRING_REF) STRING_TOKEN (STR_FIRMWARE_VERSION_VALUE);
  HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);

#ifdef LT_FLAG
  //
  // Display Acm Version in Setup
  //
  Ptr32 = (UINT32 *) (UINTN) (FLASH_REGION_BIOS_ACM_BASE + ACM_VERSION_OFFSET);
  AcmVersion = *Ptr32;
  DEBUG ((EFI_D_INFO, "ACMAddress= %08x\n", Ptr32));
  DEBUG ((EFI_D_INFO, "AcmVersion= %08x\n", AcmVersion));

  UnicodeSPrint (NewString, NEWSTRING_SIZE, L"%x.%x.%x",
    (AcmVersion & 0xFF00)    >> 8,
    (AcmVersion & 0xFF0000)  >>16,
    (AcmVersion & 0xFF000000)>>24 );

  TokenToUpdate = (STRING_REF)STRING_TOKEN(STR_ACM_VERSION_VALUE);
  HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);
#endif
  //
  // Display the System UUID in Setup
  //
  UuidGuidHob       = GetFirstGuidHob (&gEfiMfgUUIDHobGuid);
  ASSERT (UuidGuidHob != NULL);
  if (UuidGuidHob != NULL) {
    SystemUuid  = (EFI_GUID *)GET_GUID_HOB_DATA (UuidGuidHob);
    TokenToUpdate = (STRING_REF) STR_SYSTEM_UUID_VALUE;
    UnicodeSPrint (
      NewString,
      NEWSTRING_SIZE,
      L"%g",
      SystemUuid
      );
    HiiSetString  (mMainHiiHandle, TokenToUpdate, NewString, NULL);
  }


  //
  // Update setup variable for memory display
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  mSystemMemoryMap = GetSystemMemoryMapData ();
  if (mSystemMemoryMap == NULL) {
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
    return;
  }

  UnicodeValueToStringS (
    NewString,
    NEWSTRING_SIZE,
    LEFT_JUSTIFY,
    RShiftU64 (MemSize, 10),
    10
    );
  StrCatS (NewString, NEWSTRING_SIZE / sizeof (CHAR16), L" MB");
  TokenToUpdate = (STRING_REF) STR_TOTAL_MEMORY_SIZE_VALUE;
  HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Display Memory Freq
  //
  UnicodeValueToStringS (
    NewString,
    NEWSTRING_SIZE,
    LEFT_JUSTIFY,
    (mSystemMemoryMap->memFreq == 2132) ? 2133 : mSystemMemoryMap->memFreq,
    6
    );
  StrCatS (NewString, NEWSTRING_SIZE / sizeof (CHAR16), L" MT/s");
  TokenToUpdate = (STRING_REF) STR_SYSTEM_MEMORY_SPEED_VALUE;
  HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Display Mem Mode supported
  //
  if (mSystemMemoryMap->memMode == 0) {
    StrCpyS (NewString, NEWSTRING_SIZE / sizeof (CHAR16), L"Independent");
  } else if (mSystemMemoryMap->memMode == 1) {
    StrCpyS (NewString, NEWSTRING_SIZE / sizeof (CHAR16), L"Lock Step");
  } else if (mSystemMemoryMap->memMode == 2) {
    StrCpyS (NewString, NEWSTRING_SIZE / sizeof (CHAR16), L"Mirrored");
  } else {
    StrCpyS (NewString, NEWSTRING_SIZE / sizeof (CHAR16), L"Error");
  }

  TokenToUpdate = (STRING_REF) STR_MEMORY_MODE_VALUE;
  HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Update the overall memory speed
  //
  if (mMemorySpeed != 0xffff) {
    UnicodeValueToStringS (NewString, NEWSTRING_SIZE, LEFT_JUSTIFY, mMemorySpeed * 2, 5);
    StrCatS (NewString, NEWSTRING_SIZE / sizeof (CHAR16), L" MT/s");

    TokenToUpdate = (STRING_REF) STR_SYSTEM_MEMORY_SPEED_VALUE;
    HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);
  }

  //
  // ReserveMem setup
  //
  ReserveMemPage ();

  //
  // Diplay Platform Info
  //
  Status = DisplayPlatformInfo (mPlatformType);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }
#endif
  // APTIOV_SERVER_OVERRIDE_RC_END

  //
  // Display PCH Root port statuses
  // Upon specific EV request s4927269
  //
  Status = DisplayPchPcieRootPortStatus ();
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }
  // APTIOV_SERVER_OVERRIDE_RC_START
  #if 0
  gBS->FreePool (NewString);
  gBS->FreePool (NewString2);

  #endif
  // APTIOV_SERVER_OVERRIDE_RC_END
  return ;
}

/**

  Set Ide's mode.

  @retval EFI_SUCCESS  - Return status.

**/
EFI_STATUS
SetIdeMode (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**

  Setup process in HT mode.

  @param None.

  @retval EFI_SUCCESS  -  Must complete successfully.

**/
EFI_STATUS
ProcessorHtMode (
  VOID
  )
{
  UINT16              State;
  EFI_STATUS          Status;
  VOID                *HobList;
  UINT32              RegEbx, RegEdx;
  EFI_HOB_GUID_TYPE   *GuidHob;
  State = 0;

  //
  // Get Hob list
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  GuidHob = GetFirstGuidHob (&gEfiCpuHtCapableGuid);
  if (GuidHob != NULL) {
    State = *(UINT16*) GET_GUID_HOB_DATA (GuidHob);
  }

  //
  // Check if processor is HT capable
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &RegEbx, NULL, &RegEdx);
  State |= (UINT16) (!((RegEdx & 0x10000000) && (((RegEbx >> 16) & 0xFF) <= 1)));
  return EFI_SUCCESS;
}


/**

  Displays the Platform, NorthBridge and SouthBridge info in the Setup.

  @param[in] PlatformType.

  @retval EFI_SUCCESS  -  Must complete successfully.

**/
// APTIOV_SERVER_OVERRIDE_RC_START : Commenting the unwanted Intel Setup Page
#if 0
EFI_STATUS
DisplayPlatformInfo (
  IN EFI_PLATFORM_TYPE_PROTOCOL            *PlatformType
  )

{
  EFI_STRING_ID  TokenToUpdate;
  CHAR16      *NewString;

  //
  // Platform Type Info
  //
  NewString     = (CHAR16 *)PlatformType->TypeStringPtr;
  TokenToUpdate = (STRING_REF) STR_PLATFORM_REVISION_VALUE;
  HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // South Bridge Info
  //
  NewString     = (CHAR16 *)PlatformType->PchStringPtr;
  TokenToUpdate = (STRING_REF) STR_PCH_REVISION_VALUE;
  HiiSetString (mMainHiiHandle, TokenToUpdate, NewString, NULL);

  return EFI_SUCCESS;
}
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Commenting the unwanted Intel Setup Page

/**

  Displays PCH PCIe root port statuses

  @retval EFI_SUCCESS          - Successfully updated PCH PCIe root port status.
  @retval EFI_OUT_OF_RESOURCES - Cannot allocate resources.

**/
EFI_STATUS
DisplayPchPcieRootPortStatus (
   VOID
  )
{
  EFI_STRING_ID                     TokenToUpdate;
  CHAR16                            *NewString;
  UINT8                             MaxPciePortNum;
  UINTN                             RpBase;
  UINT8                             PortIndex;
  UINT32                            DevNum;
  UINT32                            FuncNum;
  UINT32                            RpLnkSts;

  NewString     = AllocateZeroPool (NEWSTRING_SIZE);
  ASSERT (NewString != NULL);
  if (NewString == NULL) {
    DEBUG ((DEBUG_ERROR, " ***ERROR*** Out of resources.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  MaxPciePortNum  = GetPchMaxPciePortNum ();

  for (PortIndex = 0; PortIndex < MaxPciePortNum; ) {
     DevNum = PchPcieRpDevNumber (PortIndex);
     FuncNum = PchPcieRpFuncNumber (PortIndex);
     RpBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, DevNum, FuncNum);
     if (MmioRead16 (RpBase) != 0xFFFF) {
       //
       // RP is valid
       //
       //
       // Check the Link Speed status and update related string in HII
       //
       RpLnkSts = MmioRead16(RpBase + R_PCH_PCIE_CFG_LINKSTS);
       switch (RpLnkSts & B_PCIE_LSTS_CLS) {
          case V_PCIE_LSTS_CLS_GEN1:
            AsciiToUnicode ((CHAR8 *)"Gen1",NewString);
            break;
          case V_PCIE_LSTS_CLS_GEN2:
            AsciiToUnicode ((CHAR8 *)"Gen2",NewString);
            break;
          case V_PCIE_LSTS_CLS_GEN3:
            AsciiToUnicode ((CHAR8 *)"Gen3",NewString);
            break;
          default:
            AsciiToUnicode ((CHAR8 *)"Can't identify!",NewString);
       }
       TokenToUpdate = (STRING_REF) RpSpeedStrings[PortIndex];
       HiiSetString (mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

       //
       // Check the Link Width and update related string in HII
       //
       switch (RpLnkSts & B_PCIE_LSTS_NLW) {
          case V_PCIE_LSTS_NLW_1:
            AsciiToUnicode ((CHAR8 *)"4 ports x1",NewString);
            break;
          case V_PCIE_LSTS_NLW_2:
            AsciiToUnicode ((CHAR8 *)"2 ports x2",NewString);
            break;
          case V_PCIE_LSTS_NLW_4:
            AsciiToUnicode ((CHAR8 *)"1 ports x4",NewString);
            break;
          default:
            AsciiToUnicode ((CHAR8 *)"Can't identify!",NewString);
       }
       TokenToUpdate = (STRING_REF) RpWidthStrings[PortIndex];
       HiiSetString (mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);
       PortIndex++;
     } else {
       if (FuncNum == 0) {
         //
         // If unable to read FuncNum is 0 then skip the whole device (move to next 8 functions in next root port device)
         //
         PortIndex = ((PortIndex/8) + 1) * 8;
       } else {
         //
         // If unable to read FuncNum other then 0 then just move to the next Function
         PortIndex++;
       }
     }

  }

  gBS->FreePool (NewString);

  return EFI_SUCCESS;
}


/**

  This is the function for initialization information on ReserveMem page.

  @param VOID.

  @return VOID.

**/
// APTIOV_SERVER_OVERRIDE_RC_START : Commenting the unwanted Intel Setup Page
#if 0
VOID
ReserveMemPage (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 VarSize;
  BOOLEAN                               ReserveMemFlag;
  CHAR16                                *NewString;
  EFI_STRING_ID                         TokenToUpdate;

  //
  //  Create buffer to hold the string.
  //
  NewString = AllocateZeroPool (NEWSTRING_SIZE);

  if (NewString == NULL) {
    DEBUG ((EFI_D_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocateZeroPool() returned NULL pointer.\n"));
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }

  //
  //  Get Reserve Memory result to update the Status string.
  //  If GetVariable fails, set ReserveMemFlag to FALSE.
  //
  VarSize = 0;
  Status = gRT->GetVariable (L"ReserveMemFlag", &gReserveMemFlagVariableGuid, NULL, &VarSize, &ReserveMemFlag);

  if (EFI_ERROR (Status)) {
    ReserveMemFlag = FALSE;
  }

  //
  //  Fill the string by the Flag.
  //
  if (ReserveMemFlag) {
    StrCpyS (NewString, NEWSTRING_SIZE/sizeof (CHAR16), L"Memory Reserved ");
  } else {
    StrCpyS (NewString, NEWSTRING_SIZE/sizeof (CHAR16), L"Reserved Fail ");
  }

  //
  //  Update the Status string to display results of page reservation.
  //
  TokenToUpdate = (STRING_REF) STR_RESERVE_MEMORY_RESULT_VALUE;
  HiiSetString (mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  gBS->FreePool (NewString);

  return;
}
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Commenting the unwanted Intel Setup Page
