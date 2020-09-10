/**@file
  BAE Pet Alert Implementation

@copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation.

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

#include "AmtPetAlert.h"

// Example 1: 16 10 0F 6F 02 68 08 FF FF 22 00 40 13 XX XX XX
PET_ALERT mPetAlertWithoutChange = {
  {
    ASF_MESSAGE_COMMAND_MESSAGE +
    (ASF_MESSAGE_BYTE_COUNT_MAP (PET_ALERT) << 8) +
    (ASF_MESSAGE_SUBCOMMAND_NORETRANSMIT << 16) +
    (ASF_VERSION << 24)
  },
  ASF_EVENT_SENSOR_TYPE_SYS_FW_ERR_PROG,
  ASF_EVENT_TYPE_SENSOR_SPECIFIC,
  ASF_EVENT_OFFSET_SYS_FW_PROGRESS,
  ASF_EVENT_SOURCE_TYPE_ASF10,
  ASF_EVENT_SEVERITY_CODE_NONCRITICAL,
  ASF_SENSOR_DEVICE,
  ASF_SENSOR_NUMBER,
  ASF_ENTITY_BIOS,
  ASF_ENTITY_INSTANCE_UNSPECIFIED,
  ASF_BAE_EVENT_DATA1,
  ASF_FP_BOOT_OS,
  0x00,
  0x00,
  0x00
};

// Example 2: 15 10 0F 6F 02 68 10 FF FF 22 00 AA 13 03 03 02
PET_ALERT mPetAlertWithChange = {
  {
    ASF_MESSAGE_COMMAND_MESSAGE +
    (ASF_MESSAGE_BYTE_COUNT_MAP (PET_ALERT) << 8) +
    (ASF_MESSAGE_SUBCOMMAND_RETRANSMIT << 16) +
    (ASF_VERSION << 24)
  },
  ASF_EVENT_SENSOR_TYPE_SYS_FW_ERR_PROG,
  ASF_EVENT_TYPE_SENSOR_SPECIFIC,
  ASF_EVENT_OFFSET_SYS_FW_PROGRESS,
  ASF_EVENT_SOURCE_TYPE_ASF10,
  ASF_EVENT_SEVERITY_CODE_CRITICAL,
  ASF_SENSOR_DEVICE,
  ASF_SENSOR_NUMBER,
  ASF_ENTITY_BIOS,
  ASF_ENTITY_INSTANCE_UNSPECIFIED,
  ASF_BAE_EVENT_DATA1_EVENT_DATA_SET_BY_OEM,
  ASF_FP_BOOT_OS,
  0x00,
  0x00,
  0x00
};


#define PLATFORM_GLOBAL_VARIABLE \
  { \
    0xa5dabcf0, 0xc7a1, 0x4370, { 0x8c, 0x21, 0xfa, 0x50, 0xd3, 0x22, 0xa4, 0x3e } \
  }
EFI_GUID gPlatformGlobalVariableGuid = PLATFORM_GLOBAL_VARIABLE;

/**
  This function converts an input device structure to a Unicode string.

  @param DevPath                  A pointer to the device path structure.

  @return A new allocated Unicode string that represents the device path.
**/
CHAR16 *
DevicePathToStr (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevPath
  )
{
  EFI_STATUS                       Status;
  CHAR16                           *ToText;
  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DevPathToText;

  if (DevPath == NULL) {
    return NULL;
  }

  Status = gBS->LocateProtocol (
                  &gEfiDevicePathToTextProtocolGuid,
                  NULL,
                  (VOID **) &DevPathToText
                  );
  ASSERT_EFI_ERROR (Status);
  ToText = DevPathToText->ConvertDevicePathToText (
                            DevPath,
                            FALSE,
                            TRUE
                            );
  ASSERT (ToText != NULL);
  return ToText;
}

/**
  Checks whenever a given load option starts with a partition device path

  @param[in]       LoadOption          Pointer to load option to test

  @retval          TRUE
  @retval          FALSE
**/
BOOLEAN
IsItPartitionsDevicePath (
  IN EFI_BOOT_MANAGER_LOAD_OPTION     *LoadOption
  )
{
  return  DevicePathType (LoadOption->FilePath) == MEDIA_DEVICE_PATH &&
          (DevicePathSubType (LoadOption->FilePath) == MEDIA_HARDDRIVE_DP);
}

/**
  Check whether there is a instance in BlockIoDevicePath, which contain multi device path
  instances, has the same partition node with HardDriveDevicePath device path

  @param  BlockIoDevicePath      Multi device path instances which need to check
  @param  HardDriveDevicePath    A device path which starts with a hard drive media
                                 device path.

  @retval TRUE                   There is a matched device path instance.
  @retval FALSE                  There is no matched device path instance.

**/
BOOLEAN
DevPathContainsPartition (
  IN  HARDDRIVE_DEVICE_PATH           *ParitionDevPath,
  IN  EFI_DEVICE_PATH_PROTOCOL        *FullDevPath
  )
{
  HARDDRIVE_DEVICE_PATH               *PartitionToCompare;
  EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
  BOOLEAN                              Match;

  DEBUG ((DEBUG_INFO , "Device Path Contains Partition\n"));

  if ((FullDevPath == NULL) || (ParitionDevPath == NULL)) {
    return FALSE;
  }

  //
  // find the partition device path node
  //
  DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)FullDevPath;
  PartitionToCompare = NULL;
  while (!IsDevicePathEnd (DevicePath)) {
    if ((DevicePathType (DevicePath) == MEDIA_DEVICE_PATH) &&
        (DevicePathSubType (DevicePath) == MEDIA_HARDDRIVE_DP)) {

      PartitionToCompare = (HARDDRIVE_DEVICE_PATH *) DevicePath;
      break;
    }
    DevicePath = NextDevicePathNode (DevicePath);
  }

  if (PartitionToCompare == NULL) {
    return FALSE;
  }

  DEBUG_CODE(
      CHAR16                          *PartitionToCompareStr;
      CHAR16                          *PartitionStr;
      PartitionToCompareStr = DevicePathToStr ((EFI_DEVICE_PATH_PROTOCOL*)PartitionToCompare);
      PartitionStr = DevicePathToStr ((EFI_DEVICE_PATH_PROTOCOL*)ParitionDevPath);
      DEBUG ((DEBUG_INFO, "\t\tpartition         : \"%s\"\n\t\tpartitionToCompare: \"%s\"\n", PartitionStr, PartitionToCompareStr));
      FreePool (PartitionStr);
      FreePool (PartitionToCompareStr);
  );

  //
  // Check for the match
  //
  Match = FALSE;

  if ((PartitionToCompare->MBRType == ParitionDevPath->MBRType) &&
      (PartitionToCompare->SignatureType == ParitionDevPath->SignatureType)) {

    switch (PartitionToCompare->SignatureType) {
    case SIGNATURE_TYPE_GUID:
      Match = CompareGuid (
                (EFI_GUID *)PartitionToCompare->Signature,
                (EFI_GUID *)ParitionDevPath->Signature
              );

      DEBUG ((
          DEBUG_INFO, "GUID Signature\n\t\tpartition          %016X%016X\n"
              "\t\tpartitionToCompare: %016X%016X\n",
          (UINT64*) ((VOID*)&ParitionDevPath->Signature[0]),
          (UINT64*) ((VOID*)&ParitionDevPath->Signature[1]),
          (UINT64*) ((VOID*)&PartitionToCompare->Signature[0]),
          (UINT64*) ((VOID*)&PartitionToCompare->Signature[1])
      ));
      break;

    case SIGNATURE_TYPE_MBR:
      Match = (
                (BOOLEAN)(*((UINT32 *)(&(PartitionToCompare->Signature[0]))) ==
                ReadUnaligned32((UINT32 *)(&(ParitionDevPath->Signature[0]))))
              );

      DEBUG ((
          DEBUG_INFO,
          "MBR signature type:\n\tpartition          %08X\n\tpartitionToCompare %08X\n",
          ReadUnaligned32((UINT32 *)(&(ParitionDevPath->Signature[0]))),
          *((UINT32 *)(&(PartitionToCompare->Signature[0])))
      ));
      break;

    default:
      Match = FALSE;
      break;
    }
  }

  if (Match) {
    DEBUG ((DEBUG_INFO, "Partition is contained inside dev path.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Partition is not contained inside dev path.\n"));
  }

  return Match;
}

/**
  Tries to expand the given partitions device path to start with physical HDD
  device path
  @param[in]       LoadOption          pointer to an Efi Boot Manager boot option
  @param[out]      BootDevicePath      indirect pointer to retrieved device path

  @retval          EFI_SUCCESS         Function found
**/
EFI_STATUS
ExpandPartitionsDevPath (
  IN EFI_BOOT_MANAGER_LOAD_OPTION     *LoadOption,
  OUT EFI_DEVICE_PATH_PROTOCOL       **BootDevicePath
  )
{
  EFI_STATUS                           Status;
  UINTN                                BlockIoHandleCount;
  EFI_HANDLE                          *BlockIoBuffer;
  EFI_DEVICE_PATH_PROTOCOL            *PartitionDevPath;
  EFI_DEVICE_PATH                     *BlockIoDevicePath;
  UINT32                               Index;

  DEBUG ((DEBUG_INFO, "Expand partitions device path\n"));

  *BootDevicePath = NULL;
  PartitionDevPath = LoadOption->FilePath;

  DEBUG_CODE (
    CHAR16                            *DevPathStr;
    DevPathStr = DevicePathToStr (PartitionDevPath);
    DEBUG ((DEBUG_INFO, "Partition dev path : %s\n", DevPathStr));
    FreePool (DevPathStr);
  );

  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiBlockIoProtocolGuid, NULL, &BlockIoHandleCount, &BlockIoBuffer);
  if (EFI_ERROR (Status) || BlockIoHandleCount == 0 || BlockIoBuffer == NULL) {
    return EFI_NOT_FOUND;
  }

  for (Index=0; Index < BlockIoHandleCount; Index++) {

    Status = gBS->HandleProtocol (BlockIoBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID *) &BlockIoDevicePath);
    if (EFI_ERROR (Status) || BlockIoDevicePath == NULL) {
      continue;
    }

    DEBUG_CODE (
      CHAR16                          *DevPathStr;
      DevPathStr = DevicePathToStr (BlockIoDevicePath);
      DEBUG ((DEBUG_INFO, "Checking dev path : %s\n", DevPathStr));
      FreePool (DevPathStr);
    );

    if (DevPathContainsPartition ((HARDDRIVE_DEVICE_PATH*) PartitionDevPath, BlockIoDevicePath)) {
      *BootDevicePath = DuplicateDevicePath (BlockIoDevicePath);
      break;
    }
  }

  if (*BootDevicePath == NULL) {
    Status = EFI_NOT_FOUND;
  }
  if (BlockIoBuffer != NULL) {
    FreePool (BlockIoBuffer);
  }
  return Status;
}


/**
  Entry point for AMT PET Alert initialization.

  @param[in] ImageHandle   Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   Pointer to the EFI System Table

  @retval EFI_SUCCESS      PET Alert driver initialized successfully.
  @retval EFI_UNSUPPORTED  Functionality not supported by onboard ME FW
**/
EFI_STATUS
EFIAPI
InitializeAmtPetAlert (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBootEvent;

  DEBUG ((DEBUG_INFO, "%a() - start\n", __FUNCTION__));

  if (!MeTypeIsClient ()) {
    return EFI_UNSUPPORTED;
  }

  //
  // Register Ready to Boot Event for AMT Wrapper
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             AmtPetAlertReadyToBoot,
             (VOID *) &ImageHandle,
             &ReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Switches the endianess of every 16-bit integer.

  @param[in, out] Data                 The address of data
  @param[in]      Size                 Size of data

**/
VOID
SwapEntries (
  IN OUT CHAR8    *Data,
  IN UINT16       Size
  )
{
  UINT16  Index;
  CHAR8   Temp8;

  for (Index = 0; (Index + 1) < Size; Index += 2) {
    Temp8           = Data[Index];
    Data[Index]     = Data[Index + 1];
    Data[Index + 1] = Temp8;
  }
}

/**
  Send BAE PET message

  @param[in] BaePetAlert          BAE PET message

  @retval EFI_SUCCESS             Message packet sent.
  @retval EFI_NOT_READY           HECI is not ready for communication
**/
EFI_STATUS
SendBaeMessage (
  IN PET_ALERT                    BaePetAlert
  )
{
  EFI_STATUS                      Status;
  HECI_PROTOCOL                   *Heci;
  UINT32                          MeStatus;

  Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Heci->GetMeStatus (&MeStatus);
  ASSERT_EFI_ERROR (Status);

  //
  // Only send ASF Push Progress code when ME is ready. Ignore FW Init Status.
  //
  if (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY) {
    Status = Heci->SendMsg (
                     HECI1_DEVICE,
                     (UINT32 *) &BaePetAlert,
                     sizeof (PET_ALERT),
                     BIOS_ASF_HOST_ADDR,
                     HECI_ASF_MESSAGE_ADDR
                     );
  }

  return Status;
}

/**
  Send BAE PET message

  @param[in] DeviceType           PET Alert device type
  @param[in] HarddriveTag         Hard drive nanme

  @retval EFI_SUCCESS             The function completed successfully.
  @return EFI_NOT_FOUND           PetAlertCfg variable not found.
**/
EFI_STATUS
PetAlert (
  IN UINT8                   DeviceType,
  IN CHAR8                   *HarddriveTag
  )
{
  PET_ALERT_CFG           PetAlertCfgVar;
  UINTN                   VarSize;
  UINTN                   UpdateFlag;
  EFI_STATUS              Status;
  PET_ALERT               PetAlertMsg;

  UpdateFlag  = 0;
  PetAlertMsg = mPetAlertWithoutChange;

  if (HarddriveTag == NULL) {
    HarddriveTag = "";
  }

  VarSize = sizeof (PetAlertCfgVar);
  Status = gRT->GetVariable (
                  L"PetAlertCfg",
                  &gPlatformGlobalVariableGuid,
                  NULL,
                  &VarSize,
                  &PetAlertCfgVar
                 );
  if (EFI_ERROR (Status)) {
    UpdateFlag = 1;
    PetAlertCfgVar.BootQueue[0] = DeviceType;
    PetAlertCfgVar.BootQueue[1] = DeviceType;
    PetAlertCfgVar.BootQueue[2] = DeviceType;
    AsciiStrCpyS ((CHAR8 *) PetAlertCfgVar.BootHarddriveTag, sizeof (PetAlertCfgVar.BootHarddriveTag) / sizeof (CHAR8), HarddriveTag);
  }

  DEBUG ((DEBUG_INFO, "PetAlert HDD ModelSerialNumber = %a\n", PetAlertCfgVar.BootHarddriveTag));

  if ((DeviceType != BAE_HDD_DEVICE) ||
      ((DeviceType == BAE_HDD_DEVICE) && AsciiStrCmp (HarddriveTag, (CHAR8 *) PetAlertCfgVar.BootHarddriveTag))) {
    //
    // If booting from a network/removable device or from a different HDD send expanded BAE message
    //
    UpdateFlag               = 1;
    PetAlertMsg              = mPetAlertWithChange;
    PetAlertMsg.EventData5   = PetAlertCfgVar.BootQueue[2] = PetAlertCfgVar.BootQueue[1];
    PetAlertMsg.EventData4   = PetAlertCfgVar.BootQueue[1] = PetAlertCfgVar.BootQueue[0];
    PetAlertMsg.EventData3   = PetAlertCfgVar.BootQueue[0] = DeviceType;
  }

  SendBaeMessage (PetAlertMsg);

  if (UpdateFlag) {
    AsciiStrCpyS ((CHAR8 *) PetAlertCfgVar.BootHarddriveTag, sizeof (PetAlertCfgVar.BootHarddriveTag) / sizeof (CHAR8), HarddriveTag);
    VarSize = sizeof (PetAlertCfgVar);
    Status = gRT->SetVariable (
                    L"PetAlertCfg",
                    &gPlatformGlobalVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    VarSize,
                    &PetAlertCfgVar
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Compare two device paths up to a size of Boot Option's Device Path

  @param[in] BootOptionDp     Device path acquired from BootXXXX EFI variable
  @param[in] FileSysDp        Device path acquired through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer

  @retval TRUE                Both device paths point to the same device
  @retval FALSE               Device paths point to different devices
**/
BOOLEAN
CompareDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL *BootOptionDp,
  IN  EFI_DEVICE_PATH_PROTOCOL *FileSysDp
  )
{
  UINTN BootOptionDpSize;
  UINTN FileSysDpSize;

  if (BootOptionDp == NULL || FileSysDp == NULL) {
    return FALSE;
  }

  BootOptionDpSize = GetDevicePathSize (BootOptionDp) - END_DEVICE_PATH_LENGTH;
  FileSysDpSize    = GetDevicePathSize (FileSysDp) - END_DEVICE_PATH_LENGTH;

  if ((BootOptionDpSize <= FileSysDpSize) && (CompareMem (FileSysDp, BootOptionDp, BootOptionDpSize) == 0)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get EFI device path through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer. Acquired path must
  point to the same device as argument DevicePath passed to the function.

  @param[in] DevicePath              Device path acquired from BootXXXX EFI variable

  @retval EFI_DEVICE_PATH_PROTOCOL   Device path for booting
**/
EFI_DEVICE_PATH_PROTOCOL *
GetFullBootDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH_PROTOCOL *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL *ReturnDevicePath;
  UINTN                    HandleNum;
  EFI_HANDLE               *HandleBuf;
  UINTN                    Index;

  ReturnDevicePath = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleNum,
                  &HandleBuf
                  );
  if ((EFI_ERROR (Status)) || (HandleBuf == NULL)) {
    return NULL;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    TempDevicePath     = DevicePathFromHandle (HandleBuf[Index]);

    if (CompareDevicePaths (DevicePath, TempDevicePath)) {
      ReturnDevicePath = DuplicateDevicePath (TempDevicePath);
      break;
    }
  }

  return ReturnDevicePath;
}

UINT8
GetDeviceType (
  IN EFI_DEVICE_PATH_PROTOCOL *DevPath
  )
{
  UINT8                    DeviceType;

  DeviceType    = BAE_EMPTY_QUEUE;

  while (!IsDevicePathEnd (DevPath)) {
    if ((DevicePathType (DevPath) == MESSAGING_DEVICE_PATH) &&
        (DevicePathSubType (DevPath) == MSG_USB_DP)) {
      //
      // If device connected through USB, we automatically treat it as removable
      //
      DeviceType = BAE_REMOVABLE_DEVICE;
      break;
    } else if (DevicePathType (DevPath) == MEDIA_DEVICE_PATH) {
      if (DevicePathSubType (DevPath) == MEDIA_HARDDRIVE_DP) {
        DeviceType = BAE_HDD_DEVICE;
        break;
      } else if (DevicePathSubType (DevPath) == MEDIA_CDROM_DP) {
        DeviceType = BAE_REMOVABLE_DEVICE;
        break;
      } else if (DevicePathSubType (DevPath) == MEDIA_FILEPATH_DP) {
        DeviceType = BAE_NETWORK_DEVICE;
        break;
      }
    }

    DevPath = NextDevicePathNode(DevPath);
  }


  return DeviceType;
}

/**
  Cleans off leading and trailing spaces.

  @param[in, out] String     Pointer to the string to trim them off.
**/
VOID
TrimSpace (
  IN OUT CHAR8 *String
  )
{
  UINTN  Index;

  if ((String == NULL) || (AsciiStrLen (String) == 0)) {
    return;
  }

  //
  // Remove any spaces at the beginning of the String.
  //
  Index = 0;
  while ((String [Index] == ' ') && (AsciiStrLen (String) > Index)) {
    Index++;
  }
  if (Index > 0) {
    CopyMem (String, String + Index, AsciiStrLen (String) - Index);
    String [AsciiStrLen (String) - Index] = '\0';
  }

  //
  // Remove any spaces at the end of the String.
  //
  for (Index = AsciiStrLen (String) - 1; (String [Index] == ' ') && (Index > 0); Index--);
  if (Index != AsciiStrLen (String) - 1) {
    String [Index + 1] = '\0';
  }

  return;
}

///
///@todo: File a request to add NVME related command to core
///
/**
  Get Nvme device identify data.

  @param[in]   NvmeDevice        The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]   NamespaceId       NamespaceId for an NVM Express namespace present on the NVM Express controller
  @param[in]   IdentifyStructure Specifies the information to be returned to host.
  @param[out]  Buffer            The buffer used to store the identify controller data.

  @return EFI_SUCCESS            Successfully get the identify controller data.
  @return EFI_DEVICE_ERROR       Fail to get the identify controller data.
**/
EFI_STATUS
NvmeIdentifyCommand (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT32                                NamespaceId,
  IN  UINT32                                IdentifyStructure,
  OUT VOID                                  *Buffer
  )
{
  EFI_STATUS                                Status;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));

  DEBUG ((DEBUG_INFO, "Sending Identify Command with Cns = %d\n", IdentifyStructure));

  Command.Cdw0.Opcode          = NVME_ADMIN_IDENTIFY_CMD;
  Command.Nsid                 = NamespaceId;
  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = Buffer;
  CommandPacket.TransferLength = sizeof (NVME_ADMIN_CONTROLLER_DATA);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;
  //
  // Set bit 0 (Cns bit) to 0 to identify a namespace / 1 to identify a controller
  //
  Command.Cdw10                = IdentifyStructure;
  Command.Flags                = CDW10_VALID;

  Status = NvmeDevice->PassThru (NvmeDevice, NamespaceId, &CommandPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Identify Command Status=%r\n", Status));
  }

  return Status;
}

EFI_STATUS
GetModelSerialNumberFromNVMePassThru (
  IN EFI_HANDLE Handle,
  OUT CHAR8 *ModelSerialNumber,
  IN  UINTN ModelSerialNumberSize
  )
{
  EFI_STATUS                           Status;
  CHAR8                                ModelNumber[42];
  CHAR8                                SerialNumber[22];
  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL   *NvmePassthruProtocol;
  UINT32                               NamespaceId;
  NVME_ADMIN_CONTROLLER_DATA           ControllerData;

  Status               = EFI_NOT_FOUND;
  DevicePath           = NULL;
  NvmePassthruProtocol = NULL;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateDevicePath (
                  &gEfiNvmExpressPassThruProtocolGuid,
                  &DevicePath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  (VOID**)&NvmePassthruProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  NamespaceId = NVME_ALL_NAMESPACES;
  Status = NvmePassthruProtocol->GetNextNamespace (NvmePassthruProtocol, &NamespaceId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Due to the fact that RAID Driver expects other value of NamespaceId parameter than AHCI driver,
  // need to send the Identify Controller command twice - once with the NamespaceId retrieved
  // using GetNextNamespace. If it fails we need to set NamespaceId = 0.
  //
  ZeroMem (&ControllerData, sizeof (NVME_ADMIN_CONTROLLER_DATA));
  Status = NvmeIdentifyCommand (NvmePassthruProtocol, NamespaceId, NVME_IDENTIFY_CONTROLLER_STRUCT, (VOID *) &ControllerData);
  if (EFI_ERROR (Status)) {
    Status = NvmeIdentifyCommand (NvmePassthruProtocol, NVME_CONTROLLER_ID, NVME_IDENTIFY_CONTROLLER_STRUCT, (VOID *) &ControllerData);
     if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  CopyMem (SerialNumber, ControllerData.Sn, 20);
  SerialNumber[20] = '\0';
  TrimSpace (SerialNumber);

  CopyMem (ModelNumber, ControllerData.Mn, 40);
  ModelNumber[40] = '\0';
  TrimSpace (ModelNumber);

  AsciiStrCpyS (ModelSerialNumber, ModelSerialNumberSize, ModelNumber);
  AsciiStrCatS (ModelSerialNumber, ModelSerialNumberSize, "_");
  AsciiStrCatS (ModelSerialNumber, ModelSerialNumberSize, SerialNumber);

  return Status;
}

EFI_STATUS
GetModelSerialNumber (
  IN EFI_HANDLE Handle,
  OUT CHAR8     *ModelSerialNumber,
  IN  UINTN     ModelSerialNumberSize
  )
{
  EFI_STATUS                      Status;
  EFI_DISK_INFO_PROTOCOL          *DiskInfo;
  EFI_ATAPI_IDENTIFY_DATA         *IdentifyDriveInfo;
  UINT32                          BufferSize;
  UINT16                          ModelNameLength;
  CHAR8                           *ModelNumber;
  UINT16                          SerialNumberLength;
  CHAR8                           *SerialNumber;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;

  IdentifyDriveInfo  = NULL;
  ModelNameLength    = 0;
  ModelNumber        = NULL;
  SerialNumberLength = 0;
  SerialNumber       = NULL;
  DevicePath         = NULL;
  Status             = EFI_NOT_FOUND;
  AsciiStrCpyS (ModelSerialNumber, ModelSerialNumberSize, "");

  Status = GetModelSerialNumberFromNVMePassThru (Handle, ModelSerialNumber, ModelSerialNumberSize);
  if (!EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateDevicePath (
                  &gEfiDiskInfoProtocolGuid,
                  &DevicePath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDiskInfoProtocolGuid,
                  (VOID **) &DiskInfo
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  IdentifyDriveInfo = AllocatePool (sizeof(EFI_ATAPI_IDENTIFY_DATA));
  if (IdentifyDriveInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SetMem (IdentifyDriveInfo, sizeof (EFI_ATAPI_IDENTIFY_DATA), 0);
  BufferSize = sizeof (EFI_ATAPI_IDENTIFY_DATA);
  Status = DiskInfo->Identify (
                       DiskInfo,
                       IdentifyDriveInfo,
                       &BufferSize
                       );
  if (EFI_ERROR (Status)) {
    FreePool (IdentifyDriveInfo);
    return Status;
  }

  SerialNumberLength = sizeof (IdentifyDriveInfo->SerialNo) / sizeof (CHAR8);
  SerialNumber = AllocateZeroPool ((SerialNumberLength + 1) * sizeof (CHAR8));
  if (SerialNumber == NULL) {
    FreePool (IdentifyDriveInfo);
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (SerialNumber, IdentifyDriveInfo->SerialNo, SerialNumberLength);
  SwapEntries (SerialNumber, SerialNumberLength);
  SerialNumber [SerialNumberLength] = '\0';
  TrimSpace (SerialNumber);

  ModelNameLength = sizeof (IdentifyDriveInfo->ModelName) / sizeof (CHAR8);
  ModelNumber = AllocateZeroPool ((ModelNameLength + 1) * sizeof (CHAR8));
  if (ModelNumber == NULL) {
    FreePool (IdentifyDriveInfo);
    FreePool (SerialNumber);
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (ModelNumber, IdentifyDriveInfo->ModelName, ModelNameLength);
  SwapEntries (ModelNumber, ModelNameLength);
  ModelNumber [ModelNameLength] = '\0';
  TrimSpace (ModelNumber);

  AsciiStrCpyS (ModelSerialNumber, ModelSerialNumberSize, ModelNumber);
  AsciiStrCatS (ModelSerialNumber, ModelSerialNumberSize, "_");
  AsciiStrCatS (ModelSerialNumber, ModelSerialNumberSize, SerialNumber);

  FreePool (IdentifyDriveInfo);
  FreePool (SerialNumber);
  FreePool (ModelNumber);

  return EFI_SUCCESS;
}

EFI_STATUS
GetModelSerialNumberFromBbs (
  IN  UINT8   DeviceType,
  OUT CHAR8   *ModelSerialNumber,
  IN  UINTN   ModelSerialNumberSize
  )
{
  return EFI_NOT_FOUND;
}

/**
  Ready to Boot Event notification handler.

  @param[in] Event      Event whose notification function is being invoked.
  @param[in] Context    Pointer to the notification function's context.

**/
VOID
EFIAPI
AmtPetAlertReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                      Status;
  UINT16                          BootCurrent;
  UINTN                           VarSize;
  CHAR16                          BootXXXX[16];
  EFI_DEVICE_PATH_PROTOCOL        *BootDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *TempDevicePath;
  UINT8                           DeviceType;
  CHAR8                           ModelSerialNumber[64];
  EFI_HANDLE                      BootDeviceHandle;
  EFI_BOOT_MANAGER_LOAD_OPTION    BootOption;
  UINTN                           ModelSerialNumberSize;
  EFI_HOB_GUID_TYPE               *GuidHob;
  MEBX_DATA_HOB                   *MebxData;

  gBS->CloseEvent (Event);

  GuidHob  = GetFirstGuidHob (&gAmtMebxDataGuid);
  if (GuidHob == NULL) {
    return;
  }
  MebxData = (MEBX_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  if (MebxData->PlatformMngSel == 0) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Send BAE message in ReadyToBoot event\n"));
  ModelSerialNumberSize = sizeof (ModelSerialNumber) / sizeof (CHAR8);
  AsciiStrCpyS (ModelSerialNumber, ModelSerialNumberSize, "");
  BootDeviceHandle            = 0;
  DeviceType                  = 0;
  BootDevicePath              = NULL;

  //
  // Get current boot option from NVRAM variables
  //
  VarSize = sizeof (UINT16);
  Status = gRT->GetVariable (
                  L"BootCurrent",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &VarSize,
                  &BootCurrent
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  UnicodeSPrint (BootXXXX, sizeof(BootXXXX), L"Boot%04X", BootCurrent);

  Status = EfiBootManagerVariableToLoadOption (BootXXXX, &BootOption);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (IsItPartitionsDevicePath (&BootOption)) {
    Status = ExpandPartitionsDevPath (&BootOption, &BootDevicePath);
  } else {
    //
    // For EFI boot get full device path from EFI_SIMPLE_FILE_SYSTEM
    //
    BootDevicePath = GetFullBootDevicePath (BootOption.FilePath);
  }

  EfiBootManagerFreeLoadOption (&BootOption);

  if (BootDevicePath == NULL) {
    return;
  }

  DeviceType = GetDeviceType (BootDevicePath);

  //
  // Get model serial number if device is HDD
  //
  if (DeviceType == BAE_HDD_DEVICE) {
    TempDevicePath = BootDevicePath;
    Status = gBS->LocateDevicePath (
                    &gEfiSimpleFileSystemProtocolGuid,
                    &TempDevicePath,
                    &BootDeviceHandle
                    );
    if (EFI_ERROR (Status)) {
      FreePool (BootDevicePath);
      return;
    }

    GetModelSerialNumber (BootDeviceHandle, ModelSerialNumber, ModelSerialNumberSize);
  }

  FreePool (BootDevicePath);

  Status = PetAlert (DeviceType, ModelSerialNumber);
  return;
}
