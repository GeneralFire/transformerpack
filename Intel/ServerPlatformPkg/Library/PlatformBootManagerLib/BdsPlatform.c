/** @file
  This file include all platform action which can be customized
  by IBV/OEM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include "BdsPlatform.h"
#include <Guid/SetupVariable.h>
#include <Protocol/IioUds.h>
#include <RcRegs.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/Tcg2Protocol.h>
#include <Protocol/TcgService.h>
#include <Protocol/VariableLock.h>
#include <Guid/SocketMpLinkVariable.h>
#include <Guid/FileInfo.h>
#include <Guid/Tcg2PhysicalPresenceData.h>
#include <IndustryStandard/Pci.h>
#include <Guid/PhysicalPresenceData.h>
#include <Library/TcgPpVendorLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootManagerLib.h>
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#include "Library/AmtSupportLib.h"
#include "Library/MeTypeLib.h"
#include <Library/MeUtilsLib.h>
#endif // AMT_SUPPORT

#include <Library/PlatformStatusCodes.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UbaSmbiosUpdateLib.h>
#include <Library/PlatformBootManagerHookLib.h>

#include <Library/OtaDxeLib.h>
#include <ScratchpadList.h>
#include <Library/EmulationConfigurationLib.h>
#include <SioRegs.h>
#include <Protocol/PrintBootPrompt.h>
#include <Library/TpmPhysicalPresenceHookLib.h>
#include <Library/PlatformBootTimeOutLib.h>


#undef MAX_LENGTH_CAPSULEVARNAME
#define MAX_LENGTH_CAPSULEVARNAME       sizeof(EFI_CAPSULE_VARIABLE_NAME)+10 // 10 extra bytes for appended update counter
#define FAST_BOOT_MODE 0x13

extern EFI_GUID mBmAutoCreateBootOptionGuid;
UINTN    mBootMenuOptionNumber;

INTN
EFIAPI
EfiBootManagerFindLoadOption (
  IN CONST EFI_BOOT_MANAGER_LOAD_OPTION *Key,
  IN CONST EFI_BOOT_MANAGER_LOAD_OPTION *Array,
  IN UINTN                              Count
  );

/**
  An empty function to pass error checking of CreateEventEx ().

  This empty function ensures that EVT_NOTIFY_SIGNAL_ALL is error
  checked correctly since it is now mapped into CreateEventEx() in UEFI 2.0.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
InternalBdsEmptyCallbackFunction (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}

typedef VOID (*GEN_GBT_XML)(VOID *XmlCliProto);

typedef struct {
  GEN_GBT_XML   GenerateGbtXml;
} XML_CLI_PROTO;

VOID
GenGbtXml (
  VOID
  )
{
  EFI_STATUS     Status;
  XML_CLI_PROTO  *XmlCliProto;

  Status = gBS->LocateProtocol (&gXmlCliProtocolGuid, NULL, (VOID **)&XmlCliProto);
  if (!EFI_ERROR (Status)) {
    XmlCliProto->GenerateGbtXml ((VOID*)XmlCliProto);
  }
}


extern USB_CLASS_FORMAT_DEVICE_PATH gUsbClassKeyboardDevicePath;
extern PLATFORM_PCI_SERIAL_DEVICE_PATH   gSerialDevicePathHsuart0;
EFI_USER_PROFILE_HANDLE             mCurrentUser = NULL;

#define EFI_1G_UNDI_DRIVER_IMAGE_GUID \
    {0xA1f436EA, 0xA127, 0x4EF8, 0x95, 0x7C, 0x80, 0x48, 0x60, 0x6F, 0xF6, 0x70}

#define EFI_NAC_UNDI_DRIVER_IMAGE_GUID \
    {0xB496912D, 0xB461, 0x4A20, 0x87, 0x4B, 0xEE, 0xE9, 0x67, 0x69, 0xE2, 0x98}



EFI_GUID gEfi1gUndiDriverImageGuid = EFI_1G_UNDI_DRIVER_IMAGE_GUID;
EFI_GUID gEfiNacUndiDriverImageGuid = EFI_NAC_UNDI_DRIVER_IMAGE_GUID;

UINTN       mDiskInfoHandleCount   = 0;
EFI_HANDLE  *mDiskInfoHandleBuffer = NULL;
BOOLEAN     mSetShellFirst = FALSE;

typedef enum {
  FORCE_TO_SATA_HD = 1,
  FORCE_TO_SATA_CD,
  FORCE_TO_SETUP,
  FORCE_TO_BOOT_DEVICE_LIST,
  FORCE_TO_UEFI_HD,
  FORCE_TO_UEFI_CD,
  FORCE_TO_SHELL,
  FORCE_TO_USB,
  FORCE_TO_UEFI_USB
} BOOTDEVICESCRATCHPAD;
  EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess;
  BOOTDEVICESCRATCHPAD        mForceValue;
#define FORCE_BITS            0xF0000
#define FORCE_SHIFT           16

  /**
    Get the headers (dos, image, optional header) from an image

    @param  Device                SimpleFileSystem device handle
    @param  FileName              File name for the image
    @param  DosHeader             Pointer to dos header
    @param  Hdr                   The buffer in which to return the PE32, PE32+, or TE header.

    @retval EFI_SUCCESS           Successfully get the machine type.
    @retval EFI_NOT_FOUND         The file is not found.
    @retval EFI_LOAD_ERROR        File is not a valid image file.

  **/
  EFI_STATUS
  GetImageHeader (
    IN  EFI_HANDLE                  Device,
    IN  CHAR16                      *FileName,
    OUT EFI_IMAGE_DOS_HEADER        *DosHeader,
    OUT EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION   Hdr
    )
  {
    EFI_STATUS                       Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Volume;
    EFI_FILE_HANDLE                  Root;
    EFI_FILE_HANDLE                  ThisFile;
    UINTN                            BufferSize;
    UINT64                           FileSize;
    EFI_FILE_INFO                    *Info;

    Root     = NULL;
    ThisFile = NULL;
    //
    // Handle the file system interface to the device
    //
    Status = gBS->HandleProtocol (
                    Device,
                    &gEfiSimpleFileSystemProtocolGuid,
                    (VOID *) &Volume
                    );
    if (EFI_ERROR (Status)) {
      goto Done;
    }

    Status = Volume->OpenVolume (
                       Volume,
                       &Root
                       );
    if (EFI_ERROR (Status)) {
      Root = NULL;
      goto Done;
    }
    Status = Root->Open (Root, &ThisFile, FileName, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR (Status)) {
      goto Done;
    }

    //
    // Get file size
    //
    BufferSize  = SIZE_OF_EFI_FILE_INFO + 200;
    do {
      Info   = NULL;
      Status = gBS->AllocatePool (EfiBootServicesData, BufferSize, (VOID **) &Info);
      if (EFI_ERROR (Status)) {
        goto Done;
      }
      Status = ThisFile->GetInfo (
                           ThisFile,
                           &gEfiFileInfoGuid,
                           &BufferSize,
                           Info
                           );
      if (!EFI_ERROR (Status)) {
        break;
      }
      if (Status != EFI_BUFFER_TOO_SMALL) {
        FreePool (Info);
        goto Done;
      }
      FreePool (Info);
    } while (TRUE);

    FileSize = Info->FileSize;
    FreePool (Info);

    //
    // Read dos header
    //
    BufferSize = sizeof (EFI_IMAGE_DOS_HEADER);
    Status = ThisFile->Read (ThisFile, &BufferSize, DosHeader);
    if (EFI_ERROR (Status) ||
        BufferSize < sizeof (EFI_IMAGE_DOS_HEADER) ||
        FileSize <= DosHeader->e_lfanew ||
        DosHeader->e_magic != EFI_IMAGE_DOS_SIGNATURE) {
      Status = EFI_LOAD_ERROR;
      goto Done;
    }

    //
    // Move to PE signature
    //
    Status = ThisFile->SetPosition (ThisFile, DosHeader->e_lfanew);
    if (EFI_ERROR (Status)) {
      Status = EFI_LOAD_ERROR;
      goto Done;
    }

    //
    // Read and check PE signature
    //
    BufferSize = sizeof (EFI_IMAGE_OPTIONAL_HEADER_UNION);
    Status = ThisFile->Read (ThisFile, &BufferSize, Hdr.Pe32);
    if (EFI_ERROR (Status) ||
        BufferSize < sizeof (EFI_IMAGE_OPTIONAL_HEADER_UNION) ||
        Hdr.Pe32->Signature != EFI_IMAGE_NT_SIGNATURE) {
      Status = EFI_LOAD_ERROR;
      goto Done;
    }

    //
    // Check PE32 or PE32+ magic
    //
    if (Hdr.Pe32->OptionalHeader.Magic != EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC &&
        Hdr.Pe32->OptionalHeader.Magic != EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
      Status = EFI_LOAD_ERROR;
      goto Done;
    }

   Done:
    if (ThisFile != NULL) {
      ThisFile->Close (ThisFile);
    }
    if (Root != NULL) {
      Root->Close (Root);
    }
    return Status;
  }

/**
  Identify a user and, if authenticated, returns the current user profile handle.

  @param[out]  User           Point to user profile handle.

  @retval EFI_SUCCESS         User is successfully identified, or user identification
                              is not supported.
  @retval EFI_ACCESS_DENIED   User is not successfully identified

**/
EFI_STATUS
UserIdentify (
  OUT EFI_USER_PROFILE_HANDLE         *User
  )
{
  EFI_STATUS                          Status;
  EFI_USER_MANAGER_PROTOCOL           *Manager;

  Status = gBS->LocateProtocol (
                  &gEfiUserManagerProtocolGuid,
                  NULL,
                  (VOID **) &Manager
                  );
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  return Manager->Identify (Manager, User);
}

/**
  Search out the video controller.

  @return  PCI device path of the video controller.
**/
EFI_HANDLE
PlatformGetVideoController (
  EFI_HANDLE                *OnboardVideoController,
  EFI_HANDLE                *AddinVideoController
  )
{
  EFI_STATUS                Status;
  UINTN                     RootBridgeHandleCount;
  EFI_HANDLE                *RootBridgeHandleBuffer;
  UINTN                     HandleCount;
  EFI_HANDLE                *HandleBuffer;
  UINTN                     RootBridgeIndex;
  UINTN                     Index;
  EFI_HANDLE                VideoController = NULL;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_TYPE00                Pci;

  //
  // Make all the PCI_IO protocols show up
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &RootBridgeHandleCount,
                  &RootBridgeHandleBuffer
                  );
  if (EFI_ERROR (Status) || (RootBridgeHandleCount == 0)) {
    return NULL;
  }

  for (RootBridgeIndex = 0; RootBridgeIndex < RootBridgeHandleCount; RootBridgeIndex++) {
    gBS->ConnectController (RootBridgeHandleBuffer[RootBridgeIndex], NULL, NULL, FALSE);

    //
    // Start to check all the pci io to find the first video controller
    //
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiPciIoProtocolGuid,
                    NULL,
                    &HandleCount,
                    &HandleBuffer
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    for (Index = 0; Index < HandleCount; Index++) {
      Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiPciIoProtocolGuid, &PciIo);
      if (!EFI_ERROR (Status)) {
        //
        // Check for all video controller
        //
        Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0,
                          sizeof (Pci) / sizeof (UINT32),
                          &Pci
                          );
        if (!EFI_ERROR (Status) && IS_PCI_VGA (&Pci)) {
          // Set VideoController to default to first video controller.
          // This will be the device used if Video Select is set to Auto.
          if(VideoController == NULL) {
            VideoController = HandleBuffer[Index];
          }

          if ((Pci.Hdr.VendorId == PcdGet16 (PcdOnboardVideoPciVendorId)) &&
              (Pci.Hdr.DeviceId == PcdGet16 (PcdOnboardVideoPciDeviceId))) {
            *OnboardVideoController = HandleBuffer[Index];
          } else {
            *AddinVideoController = HandleBuffer[Index];
          }
        }
      }
    }
    FreePool (HandleBuffer);

    if (VideoController != NULL) {
      break;
    }
  }
  FreePool (RootBridgeHandleBuffer);

  return VideoController;
}

/**

    GC_TODO: add routine description

    @param ForceOnboardVideo: TRUE - use onboard video ONLY; FALSE - use Setup controlled video

    @retval BOOLEAN: TRUE - onboard video used; FALSE - addon video used

**/
BOOLEAN
UpdateConOut (
  BOOLEAN ForceOnboardVideo
  )
{
  EFI_HANDLE                 VideoController;
  EFI_DEVICE_PATH_PROTOCOL   *Gop;
  EFI_HANDLE                 OnboardVideoController = NULL;
  EFI_HANDLE                 AddinVideoController = NULL;
  EFI_STATUS                 Status;
  UINT8                      VideoSelect;
  EFI_BOOT_MODE              BootMode;
  BOOLEAN                    OnboardVideoUsed = FALSE;

  // Platform may have a different driver for Dual Video that will take control of the ConOut selection,
  // and will override any value set here, so in that case, just return to aviod conflict
  if (PcdGetBool(PcdOemDualVideo)) {
    return TRUE;
  }

  BootMode = GetBootModeHob ();

  //
  // Get the platform vga device
  //
  if (FeaturePcdGet(PcdLocalVideoEnable)) {
    VideoController = PlatformGetVideoController (&OnboardVideoController, &AddinVideoController);
  } else {
    VideoController = NULL; // Avoid using local head - save lots of time!
  }

  if (VideoController == NULL) {
    return FALSE;
  }

  Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, VideoSelect), &VideoSelect, sizeof(VideoSelect));
  if(EFI_ERROR(Status)) {
    VideoSelect = 0;
  }

  // If device selected by SystemConfig is not present, force to Auto
  if (VideoSelect == 1 && OnboardVideoController == NULL) {
    VideoSelect = 0;
  }
  if (VideoSelect == 2 && AddinVideoController == NULL) {
    VideoSelect = 0;
  }
  //
  // If we are booting in MFG Mode, set Onboard video as default if present
  //
  if (BootMode == BOOT_WITH_MFG_MODE_SETTINGS && OnboardVideoController != NULL){
    VideoSelect = 1;
  }

  //
  // If ForceOnboardVideo is true, modify VideoSelect here
  //
  if (ForceOnboardVideo) {
    VideoSelect = 1;
    if (OnboardVideoController == NULL) {
      //If onboard video is not detected, just return
      return FALSE;
    }
  }

  switch(VideoSelect) {
  case 1:
    // Onboard selected
    VideoController = OnboardVideoController;
    DEBUG((EFI_D_INFO, "Video select: Onboard\n"));
    OnboardVideoUsed = TRUE;
    break;
  case 2:
    // Add-in selected
    VideoController = AddinVideoController;
    DEBUG((EFI_D_INFO, "Video select: Add-in\n"));
    break;
  case 0:
    // Auto selected
    if (OnboardVideoController != NULL) {
      DEBUG((EFI_D_INFO, "Video select: Auto: Onboard\n"));
      VideoController = OnboardVideoController;
      OnboardVideoUsed = TRUE;
    } else if (AddinVideoController != NULL) {
      DEBUG((EFI_D_INFO, "Video select: Auto: Add-in\n"));
      VideoController = AddinVideoController;
    } else {
      // Use first Video controller found. Take no other action.
      DEBUG((EFI_D_INFO, "Video select: Auto: First-found\n"));
    }
    break;
  default:
    // Use first VideoController found, which is what GetVideoController returns as VideoController
    DEBUG((EFI_D_INFO, "Video select: First found\n"));
    break;
  }

  //
  // Try to connect the PCI device path, so that GOP dirver could start on this
  // device and create child handles with GraphicsOutput Protocol installed
  // on them, then we get device paths of these child handles and select
  // them as possible console device.
  //

  if (BootMode == FAST_BOOT_MODE) {
    // return after pci enumerate finished
    return FALSE;
  }

  gBS->ConnectController (VideoController, NULL, NULL, FALSE);

  Gop = EfiBootManagerGetGopDevicePath (VideoController);
  if (Gop == NULL) {
    return FALSE;
  }

  //
  // Update ConOut variable to remove device path of on-board/add-in video controller
  // and add the device path of the specified video controller
  //
  EfiBootManagerUpdateConsoleVariable (ConOut, NULL, DevicePathFromHandle (OnboardVideoController));
  EfiBootManagerUpdateConsoleVariable (ConOut, Gop, DevicePathFromHandle (AddinVideoController));
  FreePool (Gop);

  return OnboardVideoUsed;
}

/**
  Check if the device path is terminal type or not.

  @param Instance         Pointer to the checked device path.

  @retval TRUE            It is a terminal type.
  @retval FALSE           It is not a terminal type.

**/
static
BOOLEAN
IsTerminalDevice (
  IN  EFI_DEVICE_PATH_PROTOCOL    *Instance
  )
{
  EFI_DEVICE_PATH_PROTOCOL    *Node;
  BOOLEAN                     IsTerminal;
  VENDOR_DEVICE_PATH          *Vendor;
  UART_DEVICE_PATH            *Uart;

  IsTerminal = FALSE;
  Vendor = NULL;
  Uart = NULL;

  for (Node = Instance; !IsDevicePathEnd (Node); Node = NextDevicePathNode (Node)) {
    //
    // Vendor points to the node before the End node
    //
    Vendor = (VENDOR_DEVICE_PATH *)Node;

    if ((DevicePathType (Node) == MESSAGING_DEVICE_PATH) &&
        (DevicePathSubType (Node) == MSG_UART_DP)) {
      Uart = (UART_DEVICE_PATH *)Node;
    }
  }

  if (Vendor != NULL &&
      DevicePathType (Vendor) == MESSAGING_DEVICE_PATH &&
      DevicePathSubType (Vendor) == MSG_VENDOR_DP &&
      Uart != NULL) {
    IsTerminal = TRUE;
  }

  return IsTerminal;
}

/**
  Check if the input device path is a terminal device path. If it is, check if
  it is already appeared in the related console variable

  @param ConsoleType        Console type. only support ConIn, ConOut and ErrOut.
  @param DevicePath         Point to the checked device path.

  @retval TRUE              The DevicePath is terminal device path and it already
                            exist in the related console variable.
  @retval FALSE             The DevicePath is not a terminal device path or it is
                            but doesn't in the related console variable.

**/
static
BOOLEAN
IsTeminalConAlreadyExist (
  IN  CONSOLE_TYPE                ConsoleType,
  IN  EFI_DEVICE_PATH_PROTOCOL    *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL        *ConVar;
  EFI_DEVICE_PATH_PROTOCOL        *Instance;
  EFI_DEVICE_PATH_PROTOCOL        *TempDevicePath;
  CHAR16                          *VarName;
  UINTN                           DevicePathLen;
  EFI_STATUS                      Status;
  BOOLEAN                         IsDuplicateTerminal;

  if (!IsTerminalDevice (DevicePath)) {
    return FALSE;
  }

  VarName = NULL;
  switch (ConsoleType) {
    case ConIn:
      VarName = L"ConIn";
      break;
    case ConOut:
      VarName = L"ConOut";
      break;
    case ErrOut:
      VarName = L"ErrOut";
      break;
    default:
      break;
  }

  if (VarName == NULL) {
    return FALSE;
  }

  Status = GetEfiGlobalVariable2 (VarName, &ConVar, NULL);
  TempDevicePath = ConVar;

  Instance = GetNextDevicePathInstance (&ConVar, &DevicePathLen);

  IsDuplicateTerminal = FALSE;
  while (Instance != NULL) {
    if (IsTerminalDevice (Instance)) {
      //
      // Skip the uart, vendor and end device path because
      // uart and vendor would be updated.
      //
      DevicePathLen = DevicePathLen - sizeof (UART_DEVICE_PATH) -
                      sizeof (VENDOR_DEVICE_PATH) - sizeof (EFI_DEVICE_PATH_PROTOCOL);
      if (!CompareMem (Instance, DevicePath, DevicePathLen)) {
        //
        // it is a duplicate terminal device, skip it
        //
        FreePool (Instance);
        IsDuplicateTerminal = TRUE;
        break;
      }
    }
    FreePool (Instance);
    Instance = GetNextDevicePathInstance (&ConVar, &DevicePathLen);
  }

  if (TempDevicePath != NULL) {
    FreePool (TempDevicePath);
  }

  return IsDuplicateTerminal;
}

/**
  Add the console variable to the related console varialbe

  @param PlatformConsole  Predfined platform default console device array.

**/
VOID
UpdateConsoleVariables (
  IN BDS_CONSOLE_CONNECT_ENTRY   *PlatformConsole
  )
{
  UINTN                     Index;

  for (Index = 0; PlatformConsole[Index].DevicePath != NULL; Index++) {
    //
    // Update the console variable with the connect type
    //
    if ((PlatformConsole[Index].ConnectType & CONSOLE_IN) == CONSOLE_IN) {
      if (!IsTeminalConAlreadyExist (ConIn, PlatformConsole[Index].DevicePath)) {
        EfiBootManagerUpdateConsoleVariable (ConIn, PlatformConsole[Index].DevicePath, NULL);
      }
    }
    if ((PlatformConsole[Index].ConnectType & CONSOLE_OUT) == CONSOLE_OUT) {
      if (!IsTeminalConAlreadyExist (ConOut, PlatformConsole[Index].DevicePath)) {
        EfiBootManagerUpdateConsoleVariable (ConOut, PlatformConsole[Index].DevicePath, NULL);
      }
    }
    if ((PlatformConsole[Index].ConnectType & STD_ERROR) == STD_ERROR) {
      if (!IsTeminalConAlreadyExist (ErrOut, PlatformConsole[Index].DevicePath)) {
        EfiBootManagerUpdateConsoleVariable (ErrOut, PlatformConsole[Index].DevicePath, NULL);
      }
    }
  }
}

/**
  Initialize the platform default console variables.

  @param PlatformConsole  Predfined platform default console device array.
**/
VOID
InitializeConsoleVariables (
  IN BDS_CONSOLE_CONNECT_ENTRY   *PlatformConsole
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *VarConOut;
  EFI_DEVICE_PATH_PROTOCOL  *VarConIn;
  EFI_BOOT_MODE             BootMode;
  EFI_STATUS                Status;

  BootMode = GetBootModeHob ();

  if (BootMode == BOOT_WITH_FULL_CONFIGURATION ||
      BootMode == BOOT_WITH_DEFAULT_SETTINGS ||
      BootMode == BOOT_WITH_MFG_MODE_SETTINGS ||
      BootMode == BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS) {

    Status = GetEfiGlobalVariable2 (L"ConOut", &VarConOut, NULL);  if (VarConOut != NULL) { FreePool (VarConOut); }
    Status = GetEfiGlobalVariable2 (L"ConIn", &VarConIn, NULL);    if (VarConIn  != NULL) { FreePool (VarConIn);  }
    if (VarConOut == NULL || VarConIn == NULL) {
      //
      // Only fill ConIn/ConOut when ConIn/ConOut is empty because we may drop to Full Configuration boot mode in non-first boot
      //
      UpdateConsoleVariables (PlatformConsole);
    }
  }
}

/**

  This routine sets the Setup boot option to active so it will automatically
  launch without the need for an F2 key press during the hotkey window.

  @param VOID

  @retval Nothing

**/
VOID
ForceSetup (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION    BootOption;
  EFI_STATUS                      Status;

  // If ForceBoot set to Setup, or if we are coming from above boot to shell, boot to Setup.
  // Setup boot option must be overridden by MFG mode, even if we change the above logic to boot to Shell
  Status = EfiBootManagerGetBootManagerMenu (&BootOption);
  ASSERT_EFI_ERROR (Status);
  EfiBootManagerBoot(&BootOption);
  EfiBootManagerFreeLoadOption (&BootOption);
}
/**
  Algorithm follows the UEFI Spec chapter 3.4 Boot Mechanisms.

  @param  DevicePath  Device Path to a  bootable device

  @return  The bootable media handle. If the media on the DevicePath is not bootable, NULL will return.

**/
EFI_HANDLE
GetBootableDeviceHandle (
  IN  EFI_DEVICE_PATH_PROTOCOL        *DevicePath
  )
{
  EFI_STATUS                          Status;
  EFI_DEVICE_PATH_PROTOCOL            *UpdatedDevicePath;
  EFI_HANDLE                          Handle;
  EFI_BLOCK_IO_PROTOCOL               *BlockIo;
  VOID                                *Buffer;
  EFI_DEVICE_PATH_PROTOCOL            *TempDevicePath;
  UINTN                               Size;
  UINTN                               TempSize;
  EFI_HANDLE                          ReturnHandle;
  EFI_HANDLE                          *SimpleFileSystemHandles;
  UINTN                               NumberSimpleFileSystemHandles;
  UINTN                               Index;
  EFI_IMAGE_DOS_HEADER                DosHeader;
  EFI_IMAGE_OPTIONAL_HEADER_UNION     HdrData;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION Hdr;

  ReturnHandle      = NULL;
  UpdatedDevicePath = DevicePath;

  //
  // Check whether the device is connected
  //
  Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &UpdatedDevicePath, &Handle);
  if (EFI_ERROR (Status)) {
    //
    // Skip the case that the boot option point to a simple file protocol which does not consume block Io protocol,
    //
    Status = gBS->LocateDevicePath (&gEfiSimpleFileSystemProtocolGuid, &UpdatedDevicePath, &Handle);
    if (EFI_ERROR (Status)) {
      //
      // Fail to find the proper BlockIo and simple file protocol, maybe because device not present,  we need to connect it firstly
      //
      UpdatedDevicePath = DevicePath;
      Status            = gBS->LocateDevicePath (&gEfiDevicePathProtocolGuid, &UpdatedDevicePath, &Handle);
      gBS->ConnectController (Handle, NULL, NULL, TRUE);
    }
  } else {
    //
    // For removable device boot option, its contained device path only point to the removable device handle,
    // should make sure all its children handles (its child partion or media handles) are created and connected.
    //
    gBS->ConnectController (Handle, NULL, NULL, TRUE);
    //
    // Get BlockIo protocol and check removable attribute
    //
    Status = gBS->HandleProtocol (Handle, &gEfiBlockIoProtocolGuid, (VOID **)&BlockIo);
    //
    // Issue a dummy read to the device to check for media change.
    // When the removable media is changed, any Block IO read/write will
    // cause the BlockIo protocol be reinstalled and EFI_MEDIA_CHANGED is
    // returned. After the Block IO protocol is reinstalled, subsequent
    // Block IO read/write will success.
    //
    Buffer = AllocatePool (BlockIo->Media->BlockSize);
    if (Buffer != NULL) {
      BlockIo->ReadBlocks (
               BlockIo,
               BlockIo->Media->MediaId,
               0,
               BlockIo->Media->BlockSize,
               Buffer
               );
      FreePool(Buffer);
    }
  }

  //
  // Detect the the default boot file from removable Media
  //
  Size = GetDevicePathSize(DevicePath) - END_DEVICE_PATH_LENGTH;
  gBS->LocateHandleBuffer (
         ByProtocol,
         &gEfiSimpleFileSystemProtocolGuid,
         NULL,
         &NumberSimpleFileSystemHandles,
         &SimpleFileSystemHandles
         );
  for (Index = 0; Index < NumberSimpleFileSystemHandles; Index++) {
    //
    // Get the device path size of SimpleFileSystem handle
    //
    TempDevicePath = DevicePathFromHandle (SimpleFileSystemHandles[Index]);
    TempSize       = GetDevicePathSize (TempDevicePath)- END_DEVICE_PATH_LENGTH;
    //
    // Check whether the device path of boot option is part of the SimpleFileSystem handle's device path
    //
    if ((Size <= TempSize) && (CompareMem (TempDevicePath, DevicePath, Size) == 0)) {
      //
      // Load the default boot file \EFI\BOOT\boot{machinename}.EFI from removable Media
      //  machinename is ia32, ia64, x64, ...
      //
      Hdr.Union = &HdrData;
      Status = GetImageHeader (
                 SimpleFileSystemHandles[Index],
                 EFI_REMOVABLE_MEDIA_FILE_NAME,
                 &DosHeader,
                 Hdr
                 );
      if (!EFI_ERROR (Status) && EFI_IMAGE_MACHINE_TYPE_SUPPORTED (Hdr.Pe32->FileHeader.Machine) &&
          (Hdr.Pe32->OptionalHeader.Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION)
         ) {
        ReturnHandle = SimpleFileSystemHandles[Index];
        break;
      }
    }
  }

  if (SimpleFileSystemHandles != NULL) {
    FreePool(SimpleFileSystemHandles);
  }

  return ReturnHandle;
}

/**
  Functional placeholder for TPM library.  Initializes and locks PPI flags without executing a PP request.

  @param  None

  @return  None

**/
VOID
InitializeAndLockPPIVariables(
    VOID
    )
{
  EFI_STATUS                        Status;
  UINTN                             DataSize;
  EFI_TCG2_PHYSICAL_PRESENCE        TcgPpData;
  EFI_TCG2_PROTOCOL                 *Tcg2Protocol;
  EDKII_VARIABLE_LOCK_PROTOCOL      *VariableLockProtocol;
  EFI_TCG2_PHYSICAL_PRESENCE_FLAGS  PpiFlags;

  Status = gBS->LocateProtocol(&gEfiTcg2ProtocolGuid, NULL, (VOID **)&Tcg2Protocol);
  if (EFI_ERROR(Status)) {
    return;
  }

  //
  // Initialize physical presence flags.
  //
  DataSize = sizeof(EFI_TCG2_PHYSICAL_PRESENCE_FLAGS);
  Status = gRT->GetVariable(
      TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE,
      &gEfiTcg2PhysicalPresenceGuid,
      NULL,
      &DataSize,
      &PpiFlags
      );
  if (EFI_ERROR(Status)) {
    PpiFlags.PPFlags = TCG2_BIOS_TPM_MANAGEMENT_FLAG_PP_REQUIRED_FOR_TURN_OFF   |
                       TCG2_BIOS_TPM_MANAGEMENT_FLAG_PP_REQUIRED_FOR_CLEAR      |
                       TCG2_BIOS_TPM_MANAGEMENT_FLAG_PP_REQUIRED_FOR_CHANGE_EPS |
                       TCG2_BIOS_TPM_MANAGEMENT_FLAG_PP_REQUIRED_FOR_CHANGE_PCRS;
    Status   = gRT->SetVariable(
        TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE,
        &gEfiTcg2PhysicalPresenceGuid,
        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        sizeof(EFI_TCG2_PHYSICAL_PRESENCE_FLAGS),
        &PpiFlags
        );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "[TPM2] Set physical presence flag failed, Status = %r\n", Status));
      return;
    }
  }
  DEBUG((EFI_D_INFO, "[TPM2] PpiFlags = %x\n", PpiFlags.PPFlags));

  //
  // This flags variable controls whether physical presence is required for TPM command.
  // It should be protected from malicious software. We set it as read-only variable here.
  //
  Status = gBS->LocateProtocol(&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLockProtocol);
  if (!EFI_ERROR(Status)) {
    Status = VariableLockProtocol->RequestToLock(
        VariableLockProtocol,
        TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE,
        &gEfiTcg2PhysicalPresenceGuid
        );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "[TPM2] Error when lock variable %s, Status = %r\n", TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE, Status));
      ASSERT_EFI_ERROR(Status);
    }
  }

  //
  // Initialize physical presence variable.
  //
  DataSize = sizeof(EFI_TCG2_PHYSICAL_PRESENCE);
  Status = gRT->GetVariable(
      TCG2_PHYSICAL_PRESENCE_VARIABLE,
      &gEfiTcg2PhysicalPresenceGuid,
      NULL,
      &DataSize,
      &TcgPpData
      );
  if (EFI_ERROR(Status)) {
    ZeroMem((VOID *)&TcgPpData, sizeof(TcgPpData));
    DataSize = sizeof(EFI_TCG2_PHYSICAL_PRESENCE);
    Status   = gRT->SetVariable(
        TCG2_PHYSICAL_PRESENCE_VARIABLE,
        &gEfiTcg2PhysicalPresenceGuid,
        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        DataSize,
        &TcgPpData
        );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "[TPM2] Set physical presence variable failed, Status = %r\n", Status));
      return;
    }
  }

  DEBUG((EFI_D_INFO, "[TPM2] Flags=%x, PPRequest=%x (LastPPRequest=%x)\n", PpiFlags.PPFlags, TcgPpData.PPRequest, TcgPpData.LastPPRequest));
  return;
}

/**
Functional placeholder for TPM library.  Initializes and locks PPI flags without executing a PP request.

@param  None

@return  None

**/
VOID
InitializeAndLockPPIVariablesTpm12(
VOID
)
{
  EFI_STATUS                        Status;
  UINTN                             DataSize;
  EFI_PHYSICAL_PRESENCE             TcgPpData;
  EFI_TCG_PROTOCOL                  *TcgProtocol;
  EDKII_VARIABLE_LOCK_PROTOCOL      *VariableLockProtocol;
  EFI_PHYSICAL_PRESENCE_FLAGS       PpiFlags;

  Status = gBS->LocateProtocol(&gEfiTcgProtocolGuid, NULL, (VOID **)&TcgProtocol);
  if (EFI_ERROR(Status)) {
    return;
  }

  //
  // Initialize physical presence flags.
  //
  DataSize = sizeof(EFI_PHYSICAL_PRESENCE_FLAGS);
  Status = gRT->GetVariable(
    PHYSICAL_PRESENCE_FLAGS_VARIABLE,
    &gEfiPhysicalPresenceGuid,
    NULL,
    &DataSize,
    &PpiFlags
    );
  if (EFI_ERROR(Status)) {
    PpiFlags.PPFlags = TCG_BIOS_TPM_MANAGEMENT_FLAG_NO_PPI_PROVISION;  // Default Flag to allow PPI requests without User Confirmation


    Status = gRT->SetVariable(
      PHYSICAL_PRESENCE_FLAGS_VARIABLE,
      &gEfiPhysicalPresenceGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      sizeof(EFI_PHYSICAL_PRESENCE_FLAGS),
      &PpiFlags
      );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "[TPM] Set physical presence flag failed, Status = %r\n", Status));
      return;
    }
  }
  DEBUG((EFI_D_INFO, "[TPM] PpiFlags = %x\n", PpiFlags.PPFlags));

  //
  // This flags variable controls whether physical presence is required for TPM command.
  // It should be protected from malicious software. We set it as read-only variable here.
  //
  Status = gBS->LocateProtocol(&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLockProtocol);
  if (!EFI_ERROR(Status)) {
    Status = VariableLockProtocol->RequestToLock(
      VariableLockProtocol,
      PHYSICAL_PRESENCE_FLAGS_VARIABLE,
      &gEfiPhysicalPresenceGuid
      );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "[TPM] Error when lock variable %s, Status = %r\n", PHYSICAL_PRESENCE_FLAGS_VARIABLE, Status));
      ASSERT_EFI_ERROR(Status);
    }
  }

  //
  // Initialize physical presence variable.
  //
  DataSize = sizeof(EFI_PHYSICAL_PRESENCE);
  Status = gRT->GetVariable(
  PHYSICAL_PRESENCE_VARIABLE,
    &gEfiPhysicalPresenceGuid,
    NULL,
    &DataSize,
    &TcgPpData
    );
  if (EFI_ERROR(Status)) {
    ZeroMem((VOID*)&TcgPpData, sizeof(TcgPpData));
    DataSize = sizeof(EFI_PHYSICAL_PRESENCE);
    Status = gRT->SetVariable(
      PHYSICAL_PRESENCE_VARIABLE,
      &gEfiPhysicalPresenceGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      DataSize,
      &TcgPpData
      );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "[TPM] Set physical presence variable failed, Status = %r\n", Status));
      return;
    }
  }

  DEBUG((EFI_D_INFO, "[TPM] Flags=%x, PPRequest=%x\n", PpiFlags.PPFlags, TcgPpData.PPRequest));
}

//
// BDS Platform Functions
//
/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
EFIAPI
PlatformBootManagerBeforeConsole (
  VOID
  )
{
  EFI_HANDLE                Handle;
  EFI_STATUS                Status;
  UINTN                     DriverImageHandleCount;
  EFI_HANDLE                *DriverImageHandleBuffer;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
  EFI_GUID                  *NameGuid;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *LastDeviceNode;
  UINTN                     Index;
  UINT8                     EfiNetworkSupportOption;
  UINT8                     SetShellFirstOption;
  EFI_EVENT                 EndOfDxeEvent;
  EFI_EVENT                 AfterPlatformLocksEvent;
  BOOLEAN                   OnboardVideoActive, PpiNeedUserConfirmation, ProcessPpiRequest, Tpm12, Tpm20;
  UINT32                    DetectedSio;
  UINT8                     DfxPpvEnabled;
  UINT16                    PlatformBootTimeOut;
  EFI_HANDLE                DeviceHandle;
  UINT8                     ClassCode;
  UINT8                     Delay;
  UINT16                    Data16;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  EFI_BOOT_MODE             BootMode;
  EFI_GUID                  *TpmInstanceGuid;


  DEBUG ((EFI_D_INFO, "PlatformBootManagerBeforeConsole()  {\n"));

  Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, EfiNetworkSupport), &EfiNetworkSupportOption, sizeof(EfiNetworkSupportOption));
  Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, SetShellFirst), &SetShellFirstOption, sizeof(SetShellFirstOption));

  DEBUG ((EFI_D_INFO, "EfiNetworkSupportOption = 0x%x\n",EfiNetworkSupportOption));

  if (HybridSystemLevelEmulationEnabled ()) {
    PcdSet16S (PcdPlatformBootTimeOut, 0);
  }

  //
  // Add platform string package
  //
  InitializeStringSupport ();
  BootMode = GetBootModeHob ();
  if (BootMode != FAST_BOOT_MODE) {
    //
    // Check SIO and initialize applicable platform console
    //
    DetectedSio = IsSioExist();
    if (DetectedSio & ASPEED_EXIST) {
      DEBUG ((EFI_D_INFO, "PlatformBootManagerBeforeConsole(): Init Ast2500 Platform Console\n"));
      UpdateConsoleVariables (gAst2500PlatformConsole);
    } else if (DetectedSio & PILOTIV_EXIST) {
      DEBUG ((EFI_D_INFO, "PlatformBootManagerBeforeConsole(): Init Pilot Platform Console\n"));
      UpdateConsoleVariables (gPilotPlatformConsole);
    } else {
      DEBUG ((EFI_D_INFO, "PlatformBootManagerBeforeConsole(): Init gPlatformConsole\n"));
      UpdateConsoleVariables (gPlatformConsole);
    }

    if (PcdGetBool (PcdOnBoardSerialHsuart0Enable)) {
      EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *)&gSerialDevicePathHsuart0, NULL);
      EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL *)&gSerialDevicePathHsuart0, NULL);
    }

    //------------------- Prepare to handle PPI request ------------------------------------
    // Determine TPM type and check if PPI request need user confirmation
    DEBUG ((EFI_D_INFO, "  Prepare to handle PPI request: "));
    Tpm20 = FALSE; Tpm12 = FALSE;                // Indicate TPM2.0 absent, TPM1.2 absent
    PpiNeedUserConfirmation = FALSE;             // Indicate PPI request does not need user confirmation
    TpmInstanceGuid = (EFI_GUID *)PcdGetPtr(PcdTpmInstanceGuid);
    if (CompareGuid(TpmInstanceGuid, &gEfiTpmDeviceInstanceTpm20DtpmGuid)) {// If TPM2.0
      DEBUG ((EFI_D_INFO, "TPM2.0\n"));
      Tpm20 = TRUE;                  //   Indicate TPM2.0 present
      PpiNeedUserConfirmation = Tcg2PhysicalPresenceLibNeedUserConfirm();    //   TRUE/FALSE: PPI request yes/not need user confirmation
    } else {
      if (CompareGuid(TpmInstanceGuid, &gEfiTpmDeviceInstanceTpm12Guid)) {  // If TPM1.2
        DEBUG ((EFI_D_INFO, "TPM1.2\n"));
        Tpm12 = TRUE;                  //   Indicate TPM1.2 present
        PpiNeedUserConfirmation = TcgPhysicalPresenceLibNeedUserConfirm();    //   TRUE/FALSE: PPI request yes/not need user confirmation
      }
    }

    // Determine whether to allow On-Board/Off-Board video for PPI request
    OnboardVideoActive = FALSE;       // FALSE: Use Setup selection for video
    ProcessPpiRequest = FALSE;        // FALSE: Do not process PPI request
    if (PpiNeedUserConfirmation) {      // If PPI request need user confirmation
      DEBUG ((EFI_D_INFO, "  PPI request need user confirmation, "));
      OnboardVideoActive = TRUE;          //   TRUE: Force On-board video

      Status = PcdSetBoolS (PcdTpmPendingPpiRequestFlag, TRUE);
      ASSERT_EFI_ERROR(Status);
    } else {                            // If PPI request does not need user confirmation
      DEBUG ((EFI_D_INFO, "  PPI request does not need user confirmation, "));
      ProcessPpiRequest = TRUE;         //   TRUE: Process PPI request
    }

    // Select proper video for conout
    if (OnboardVideoActive) { DEBUG ((EFI_D_INFO, "Use On-Board Video\n")); }
    else { DEBUG ((EFI_D_INFO, "Use Setup Selection for Video\n")); }

    OnboardVideoActive = UpdateConOut (OnboardVideoActive);   // Select proper video for ConOut
                                                            // OUTPUT: OnboardVideoActive = TRUE/FALSE: On/Off board video active
    if (OnboardVideoActive) {                                 // If on-board video active
      DEBUG ((EFI_D_INFO, "On-Board Video Active, "));
      ProcessPpiRequest = TRUE;                               //   TRUE: Process PPI request
    } else {
      DEBUG ((EFI_D_INFO, "Off-Board Video Active, "));
    }

    // OnboardVideoActive              TRUE/FALSE: OnBoard/OffBoard video is active
    // PpiRequestNeedUserConfirmation  TRUE/FALSE: PPI request yes/not need user confirmation
    // ProcessPpiRequest               TRUE/FALSE: Yes/Not process PPI request
    DEBUG ((EFI_D_INFO, "Process PPI Request? "));
    if (ProcessPpiRequest) { DEBUG ((EFI_D_INFO, "Yes\n")); }
    else { DEBUG ((EFI_D_INFO, "No\n")); }

    //
    // Signal AfterTrustConsoleEvent event after trust console
    //
    SignalEventAfterTrustConsole();

#if defined(AMT_SUPPORT) && AMT_SUPPORT
  if (MeClientIsCorporate () && MeClientIsReady ()) {
    AmtRaiseBootException ();
    AmtUpdateConsoleVariable ();
  }
#endif // AMT_SUPPORT
  } else {
    UpdateConOut (FALSE);
  }

  if (!EfiNetworkSupportOption) {
    DriverImageHandleCount  = 0;
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiLoadedImageProtocolGuid,
                    NULL,
                    &DriverImageHandleCount,
                    &DriverImageHandleBuffer
                    );
    ASSERT_EFI_ERROR (Status);

    for (Index = 0; Index < DriverImageHandleCount; Index++) {
      Status = gBS->HandleProtocol (
                        DriverImageHandleBuffer[Index],
                        &gEfiLoadedImageProtocolGuid,
                        (VOID *) &LoadedImage
                        );
      if (LoadedImage->FilePath == NULL) {
        continue;
      }

      TempDevicePath = LoadedImage->FilePath;
      LastDeviceNode = TempDevicePath;
      while (!IsDevicePathEnd (TempDevicePath)) {
        LastDeviceNode = TempDevicePath;
        TempDevicePath = NextDevicePathNode (TempDevicePath);
      }
      NameGuid = EfiGetNameGuidFromFwVolDevicePathNode (
                    (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) LastDeviceNode
                    );


      if ((NameGuid != NULL) && (CompareGuid (NameGuid, &gEfi1gUndiDriverImageGuid))) {
          DEBUG ((EFI_D_INFO, "Unload %g\n",NameGuid));
          Status = gBS->UnloadImage (DriverImageHandleBuffer[Index]);
          ASSERT_EFI_ERROR (Status);
      }

      if ((NameGuid != NULL) && (CompareGuid (NameGuid, &gEfiNacUndiDriverImageGuid))) {
        DEBUG ((EFI_D_INFO, "Unload %g\n",NameGuid));
        Status = gBS->UnloadImage (DriverImageHandleBuffer[Index]);
        ASSERT_EFI_ERROR (Status);
      }
    }
  }

  if ((!PcdGetBool(PcdCustomerHotkeySupport)) && (BootMode != FAST_BOOT_MODE)) {
    RegisterLoadOptions ();
  }

  if ((GetBootModeHob () == BOOT_WITH_MFG_MODE_SETTINGS) || (BootMode == FAST_BOOT_MODE)) {
    Status = PcdSet16S (PcdPlatformBootTimeOut, 0);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
      return;
    }
  }

  DfxPpvEnabled = DFX_PPV_AUTO;
  Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, DfxPpvEnabled), &DfxPpvEnabled, sizeof(DfxPpvEnabled));
  if ( EFI_ERROR(Status) ) DfxPpvEnabled = DFX_PPV_AUTO;

  PlatformBootTimeOut = PcdGet16 (PcdPlatformBootTimeOut);
  if (DfxPpvEnabled == DFX_PPV_ENABLED) {
   if (PlatformBootTimeOut > 0) PcdSet16S (PcdPlatformBootTimeOut, 0);

   Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, ShellEntryTime), &Delay, sizeof(Delay));
   if (EFI_ERROR (Status)) {
      Delay = 5;
   }
   if (Delay > 1) {
    Delay = 1;
    Status = SetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ShellEntryTime), &Delay, sizeof(Delay));
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_WARN, "Unable to set ShellEntryTime to %d\n", Delay));
    }
   }
  }

#ifndef CDF_SC_FLAG // ICXD_TODO
  //-----------------------------------------------------------------------------------------------------
  // Handle Out-Of-Band One Touch Request before handling In-Band Request
  ExecuteOobOneTouchRequest (NULL);        // OOB One-Touch Request
  //-----------------------------------------------------------------------------------------------------
#endif //CDF_SC_FLAG

  //-----------------------------------------------------------------------------------------------------
  // Handle PPI request
  // OnboardVideoActive              TRUE/FALSE: OnBoard/OffBoard video is active
  // PpiRequestNeedUserConfirmation  TRUE/FALSE: PPI request yes/not need user confirmation
  // ProcessPpiRequest               TRUE/FALSE: Yes/Not process PPI request (Depending on execution
  //                                 environment, it can be FALSE even if PPI request is pending
  if (BootMode != FAST_BOOT_MODE) {
    if (ProcessPpiRequest) {          // If PPI request to be processed
      if (PpiNeedUserConfirmation) {    //   If PPI request need user confirmation
        DEBUG ((EFI_D_INFO, "  Connect Default Console for PPI request\n"));
        Status = EfiBootManagerConnectAllDefaultConsoles();    //   Connect consoles to service PPI request
        ASSERT_EFI_ERROR(Status);
      }
      DEBUG ((EFI_D_INFO, "\n  Handle PPI request: "));
      if (Tpm20) {            //     If TPM2.0
        DEBUG ((EFI_D_INFO, "TPM2.0\n"));
        Tcg2PhysicalPresenceLibProcessRequest(NULL);    //       Process TPM2.0 PPI Request
      } else {              //     If not TPM2.0
        if (Tpm12) {            //       If TPM1.2
          DEBUG ((EFI_D_INFO, "TPM1.2\n"));
          TcgPhysicalPresenceLibProcessRequest();      //         Process TPM1.2 PPI Request
        }
      }
    }

    // Lock PPI_FLAGS variable
    DEBUG ((EFI_D_INFO, "  Lock PPI Variable and TPM (if necessary): "));
    if (Tpm20) {
      DEBUG ((EFI_D_INFO, "TPM2.0\n"));
      InitializeAndLockPPIVariables();        // TPM2.0
    } else {
      if (Tpm12) {
        DEBUG ((EFI_D_INFO, "TPM1.2\n"));
        InitializeAndLockPPIVariablesTpm12();      // TPM1.2
      }
    }
  //-----------------------------------------------------------------------------------------------------
  }

  //
  // Signal EndOfDxe event to set platform locks
  //
  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      InternalBdsEmptyCallbackFunction,
      NULL,
      &gEfiEndOfDxeEventGroupGuid,
      &EndOfDxeEvent
      );
  ASSERT_EFI_ERROR(Status);
  gBS->SignalEvent(EndOfDxeEvent);
  gBS->CloseEvent(EndOfDxeEvent);
  DEBUG((EFI_D_INFO,"All EndOfDxe callbacks have returned successfully\n"));

  //
  // NOTE: We need install DxeSmmReadyToLock directly here because many boot script is added via EndOfDxe callback.
  // If we install them at same callback, these boot script will be rejected because BootScript Driver runs first to lock them done.
  // So we seperate them to be 2 different events, EndOfDxe is last chance to let platform add boot script. DxeSmmReadyToLock will
  // make boot script save driver lock down the interface.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Dispatch the deferred 3rd party images.
  //
  EfiBootManagerDispatchDeferredImages ();

  //
  // Connect 3rd party images only for video
  //
  UpdateConOut(FALSE);

  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      InternalBdsEmptyCallbackFunction,
      NULL,
      &gEfiAfterPlatformLocksEventGuid,
      &AfterPlatformLocksEvent
      );
  ASSERT_EFI_ERROR(Status);
  DEBUG((EFI_D_INFO, "Signalling AfterPlatformLocks...\n"));
  gBS->SignalEvent(AfterPlatformLocksEvent);
  gBS->CloseEvent(AfterPlatformLocksEvent);

  if (BootMode != FAST_BOOT_MODE) {
    // If control comes here AND PpiNeedUserConfirmation = TRUE
    // This means that EITHER    PPI request was rejected otherwise system would have been reset after perfmorming PPI request
    //  OR  PPI request was not processed due to the setting of execution environment
    if (PpiNeedUserConfirmation) {
      DEBUG ((EFI_D_WARN, "  PPI request rejected or not processed: Force Setup Selection for Video\n"));
      UpdateConOut(FALSE);          // Use Setup Selection for Video
    }

    //
    // Append Usb Keyboard short form DevicePath into "ConInDev"
    //
    EfiBootManagerUpdateConsoleVariable (
      ConInDev,
      (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath,
      NULL
      );

    //
    // Before user authentication, the user identification devices need be connected
    // from the platform customized device paths
    //
    ConnectAuthDevice ();

    //
    // As console is not ready, the auto logon user will be identified.
    //
    UserIdentify (&mCurrentUser);

#if defined(AMT_SUPPORT) && AMT_SUPPORT
    if (MeClientIsCorporate () && MeClientIsReady ()) {
      //
      // Register ME hotkeys here to avoid signalling ReadyToBoot before ExitPmAuth
      //
      RegisterMeHotKey (NULL);
    }
#endif // AMT_SUPPORT

    // Unload EFI PCI Option ROM image for Network device if EfiNetworkSupportOption is disabled
    if(!EfiNetworkSupportOption) {
      DriverImageHandleCount  = 0;
      Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiLoadedImageProtocolGuid,
                      NULL,
                      &DriverImageHandleCount,
                      &DriverImageHandleBuffer
                      );
      ASSERT_EFI_ERROR (Status);

      for (Index = 0; Index < DriverImageHandleCount; Index++) {
        Status = gBS->HandleProtocol (
                          DriverImageHandleBuffer[Index],
                          &gEfiLoadedImageProtocolGuid,
                          (VOID *) &LoadedImage
                          );
        DeviceHandle = LoadedImage->DeviceHandle;
        if (LoadedImage->DeviceHandle == NULL) {
          continue;
        }

        Status = gBS->HandleProtocol(
                        DeviceHandle,
                        &gEfiPciIoProtocolGuid,
                        &PciIo
                        );


        if(EFI_ERROR(Status)) {
          continue;
        } else {
          PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0x0B, 1, &ClassCode);

          if(ClassCode == PCI_CLASS_NETWORK) {
            PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &Data16);
            DEBUG ((EFI_D_INFO, "Unload image for Vid/Did: %x/", Data16));
            PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &Data16);
            DEBUG ((EFI_D_INFO, "%x\n", Data16));

            Status = gBS->UnloadImage(DriverImageHandleBuffer[Index]);
          }
        }
      }
    }
  }
  else {
    //
    // clear console variable in fast boot mode
    //
    Status = gRT->SetVariable(
                     L"ConIn",
                     &gEfiGlobalVariableGuid,
                     EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS| EFI_VARIABLE_NON_VOLATILE,
                     0,
                     NULL
                     );

    Status = gRT->SetVariable(
                    L"ConOut",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    0,
                    NULL
                    );

    Status = gRT->SetVariable(
                    L"ErrOut",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS| EFI_VARIABLE_NON_VOLATILE,
                    0,
                    NULL
                    );
  }

  //
  // Indicate EndOfDxe
  //
  REPORT_STATUS_CODE (
    EFI_PROGRESS_CODE,
    (EFI_SOFTWARE_UNSPECIFIED | EFI_SW_OEM_END_OF_DXE)
    );
  DEBUG ((EFI_D_INFO, "PlatformBootManagerBeforeConsole()  }\n"));
}

VOID
EFIAPI
BootManagerConnectControllersBySetup (
  VOID
  )
{
  EFI_STATUS               Status;
  UINTN                    HandleCount;
  EFI_HANDLE               *HandleBuffer;
  UINTN                    Index;
  EFI_PCI_IO_PROTOCOL      *PciIo;
  BOOLEAN                  NeedConnect;
  UINT16                   VendorId, DeviceId;
  UINT8                    Class;
  BOOLEAN                  onboardDevice = FALSE;
  UINT8                    StorageOpROMSuppression = 0;
  //
  // Connect the platform console first
  //
  EfiBootManagerConnectAllDefaultConsoles ();

 do {
    //
    // Connect All EFI 1.10 drivers following EFI 1.10 algorithm
    //
    gBS->LocateHandleBuffer (
           AllHandles,
           NULL,
           NULL,
           &HandleCount,
           &HandleBuffer
           );

    for (Index = 0; Index < HandleCount; Index++) {
      NeedConnect = TRUE; // set true first
      Status = gBS->OpenProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo,
                    NULL,
                    HandleBuffer[Index],
                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
                    );
      if (!EFI_ERROR (Status)) {
          //
          // In manufacturing mode, if an OpROM is found for a device of PCI Device Class "storage", BIOS should not load the OpROM.
          //
          PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0x0B, 1, &Class);
          if (Class == PCI_CLASS_MASS_STORAGE) {
            //skip onboard SATA controller
            PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &VendorId);
            PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &DeviceId);
            if ((VendorId == V_PCH_INTEL_VENDOR_ID) && (IS_PCH_SATA_AHCI_DEVICE_ID (DeviceId))) {
               onboardDevice = TRUE;
            } else {
               onboardDevice = FALSE;
            }

            if(!onboardDevice){
              Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, StorageOpROMSuppression), &StorageOpROMSuppression, sizeof(StorageOpROMSuppression));
              ASSERT_EFI_ERROR (Status);
              if  (StorageOpROMSuppression == 1) {
                NeedConnect = FALSE;//skip to connect non-onboard MAss storage device
              }
            }
          }
      }

      if(NeedConnect) {
        SignalEventBootManagerPause ();
        gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
        SignalEventBootManagerResume ();
      }
    }

    if (HandleBuffer != NULL) {
      FreePool (HandleBuffer);
    }

    //
    // Check to see if it's possible to dispatch an more DXE drivers.
    // The above code may have made new DXE drivers show up.
    // If any new driver is dispatched (Status == EFI_SUCCESS) and we will try
    // the connect again.
    //
    Status = gDS->Dispatch ();

  } while (!EFI_ERROR (Status));

  //
  // Here we have the assumption that we have already had
  // platform default console
  //
  EfiBootManagerConnectAllDefaultConsoles ();
}

/**

  Connect with predeined platform connect sequence,
  the OEM/IBV can customize with their own connect sequence.

  @param None.

  @retval None.

**/
VOID
ConnectSequence (
  VOID
  )
{
  UINTN Index;

  //
  // Here we can get the customized platform connect sequence
  // Notes: we can connect with new variable which record the
  // last time boots connect device path sequence
  //
  for (Index = 0; gPlatformConnectSequence[Index] != NULL; Index++) {
    //
    // Build the platform boot option
    //
    EfiBootManagerConnectDevicePath (gPlatformConnectSequence[Index], NULL);
  }
  //
  // For the debug tip, just use the simple policy to connect all devices
  //
//  EfiBootManagerConnectAll ();
  BootManagerConnectControllersBySetup();

}


EFI_DEVICE_PATH *
FvFilePath (
  EFI_GUID                     *FileGuid
  )
{

  EFI_STATUS                         Status;
  EFI_LOADED_IMAGE_PROTOCOL          *LoadedImage;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;

  EfiInitializeFwVolDevicepathNode (&FileNode, FileGuid);

  Status = gBS->HandleProtocol (
                  gImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **) &LoadedImage
                  );
  ASSERT_EFI_ERROR (Status);
  return AppendDevicePathNode (
           DevicePathFromHandle (LoadedImage->DeviceHandle),
           (EFI_DEVICE_PATH_PROTOCOL *) &FileNode
           );
}

EFI_GUID mUiFile = {
  0x462CAA21, 0x7614, 0x4503, { 0x83, 0x6E, 0x8A, 0xB6, 0xF4, 0x66, 0x23, 0x31 }
};
EFI_GUID mBootMenuFile = {
  0xEEC25BDC, 0x67F2, 0x4D95, { 0xB1, 0xD5, 0xF8, 0x1B, 0x20, 0x39, 0xD1, 0x1D }
};

/**

    Registers Firmware volume with passed options

    @param FileGuid       - GC_TODO: add arg description
    @param Description    - GC_TODO: add arg description
    @param Position       - GC_TODO: add arg description
    @param IsBootCategory - GC_TODO: add arg description
    @param OptionalData     Optional data of the load option, else NULL
    @param OptionalDataSize Size of the optional data of the load option, else 0
    @param OptionNumber     Entry in BootXXXX if replacing existing, else LoadOptionNumberUnassigned

    @retval (UINT16) NewOption.OptionNumber - GC_TODO: add retval description

**/
UINT16
RegisterFvBootWithOptions (
  EFI_GUID                         *FileGuid,
  CHAR16                           *Description,
  UINTN                            Position,
  BOOLEAN                          IsBootCategory,
  IN  UINT8                        *OptionalData,   OPTIONAL
  IN  UINT32                       OptionalDataSize,
  IN  UINTN                        OptionNumber
  )
{
  EFI_STATUS                       Status;
  UINTN                            OptionIndex;
  EFI_BOOT_MANAGER_LOAD_OPTION     NewOption;
  EFI_BOOT_MANAGER_LOAD_OPTION     *BootOptions;
  UINTN                            BootOptionCount;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;

  DEBUG ((EFI_D_VARIABLE, "in RegisterFvBootWithOptions(Description='%s',Position=%d,IsBootCategory=%d,",
           Description, Position, IsBootCategory));
  DEBUG ((EFI_D_VARIABLE, "OptionalData='%S',OptionalDataSize=%d,OptionNumber=%d )\n",
           OptionalData, OptionalDataSize, OptionNumber));

  BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

  DevicePath = FvFilePath (FileGuid);
  Status = EfiBootManagerInitializeLoadOption (
             &NewOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             IsBootCategory ? LOAD_OPTION_ACTIVE : LOAD_OPTION_CATEGORY_APP,
             Description,
             DevicePath,
             OptionalData,
             OptionalDataSize
             );
  ASSERT_EFI_ERROR (Status);
  FreePool (DevicePath);

  OptionIndex = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);

  if (OptionIndex == -1) {
    if (OptionNumber != LoadOptionNumberUnassigned) {
     NewOption.OptionNumber = OptionNumber;
     DEBUG ((EFI_D_VARIABLE, "setting NewOption.OptionNumber=%d\n", NewOption.OptionNumber));
    }
    Status = EfiBootManagerAddLoadOptionVariable (&NewOption, Position);
    ASSERT_EFI_ERROR (Status);
  } else {
    NewOption.OptionNumber = BootOptions[OptionIndex].OptionNumber;
  }
  EfiBootManagerFreeLoadOption (&NewOption);
  EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);

  return (UINT16) NewOption.OptionNumber;
}

/**

    GC_TODO: add routine description

    @param FileGuid       - GC_TODO: add arg description
    @param Description    - GC_TODO: add arg description
    @param Position       - GC_TODO: add arg description
    @param IsBootCategory - GC_TODO: add arg description

    @retval (UINT16) NewOption.OptionNumber - GC_TODO: add retval description

**/
UINT16
RegisterFvBootOption (
  EFI_GUID                         *FileGuid,
  CHAR16                           *Description,
  UINTN                            Position,
  BOOLEAN                          IsBootCategory
  )
{
    return RegisterFvBootWithOptions( FileGuid, Description, Position, IsBootCategory, NULL, 0, LoadOptionNumberUnassigned );
}



/**

  Load the predefined driver option, OEM/IBV can customize this
  to load their own drivers.

  @param BdsDriverLists  -  The header of the driver option link list.

  @retval None.

**/
VOID
RegisterLoadOptions (
  VOID
  )
{
  EFI_STATUS                     Status;
  UINTN                          Index;
  EFI_BOOT_MANAGER_LOAD_OPTION   BootOption;
  EFI_BOOT_MANAGER_LOAD_OPTION   DriverOption;
  EFI_BOOT_MANAGER_LOAD_OPTION   *DriverOptions;
  UINTN                          DriverOptionCount;
  UINTN                          ExistingOptionNum;
  UINT16                         OptionNumber;
  UINT16                         NewOptionNum;
  EFI_INPUT_KEY                  SetupMenuScanCode;
  EFI_INPUT_KEY                  BootDeviceListScanCode;
  EFI_INPUT_KEY                  Enter;
  UINT32                         ShellDataSize;
  CHAR16                         ShellData[15];
  CHAR16                         ShellDescription[30];
  UINT8                          Delay;
  EFI_GUID                       *BootMenuFileGuid;

  ShellData[0] = '\0';
  ShellDataSize = 0;
  Delay = 5;
  BootMenuFileGuid = (EFI_GUID *) PcdGetPtr(PcdBootMenuFile);

  //
  // 0. Continue key
  //
  Enter.ScanCode    = SCAN_NULL;
  Enter.UnicodeChar = CHAR_CARRIAGE_RETURN;
  EfiBootManagerRegisterContinueKeyOption (0, &Enter, NULL);

    Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, ShellEntryTime), &Delay, sizeof(Delay));
    if ( EFI_ERROR(Status) ) {
      Delay = 5;
      DEBUG ((EFI_D_WARN, "ERROR: Status=%x, default Shell Delay will be %d.\n", Status, Delay ));
    } else if (Delay) {
      UnicodeSPrint ( ShellData, sizeof (ShellData), L"-delay %d", Delay);
      ShellDataSize = (UINT32) StrSize (ShellData);
      DEBUG ((EFI_D_VARIABLE, "Status=%x, setting Shell Delay to %d. ShellData=%s\n", Status, Delay, (CHAR8 *)&ShellData[0] ));
    }

    if (Delay != 5) {
     UnicodeSPrint ( ShellDescription, sizeof ( ShellDescription ), L"%S (%ds)",
                     PcdGetPtr(PcdShellDescription), Delay);
    } else {
     UnicodeSPrint ( ShellDescription, sizeof ( ShellDescription ), L"%S",
                     PcdGetPtr(PcdShellDescription));
    }

    //
    // 1. Internal Shell
    //
    if (ShellDataSize > 0) {
     ExistingOptionNum = LoadOptionNumberUnassigned;

     DriverOptions = EfiBootManagerGetLoadOptions (&DriverOptionCount, LoadOptionTypeBoot);

     //custom search to find Internal UEFI Shell entry if it exists so can be reused
     // (prevents multiple Shell entries in bootlist)
     for (Index = 0; Index < DriverOptionCount; Index++) {
      if ( ( DriverOptions[Index].OptionType == LoadOptionTypeBoot ) &&
           ( DriverOptions[Index].Attributes == LOAD_OPTION_ACTIVE ) &&
           ( StrStr (DriverOptions[Index].Description, PcdGetPtr(PcdShellDescription) ) != NULL) ) {

       ExistingOptionNum = DriverOptions[Index].OptionNumber;

       //Only re-create if payload is different
       if ( (DriverOptions[Index].OptionalDataSize != ShellDataSize) ||
            (CompareMem (DriverOptions[Index].OptionalData, ShellData,
                         ShellDataSize ) != 0 ) ) {
        Status = EfiBootManagerDeleteLoadOptionVariable ( ExistingOptionNum, LoadOptionTypeBoot );
        if (EFI_ERROR(Status)) {
         DEBUG ((EFI_D_VARIABLE, "Status=%x returned from EfiBootManagerDeleteLoadOptionVariable(%d,%d)\n",
                 Status, ExistingOptionNum, LoadOptionTypeBoot ));
        }
       }
       break; //entry found; exit for loop
      }
     }

     NewOptionNum=RegisterFvBootWithOptions (PcdGetPtr (PcdShellFile), (CHAR16 *) ShellDescription, (UINTN) -1, TRUE, (UINT8 *)ShellData, ShellDataSize, ExistingOptionNum);
     DEBUG ((EFI_D_VARIABLE, "RegisterFvBootWithOptions(%d) returned %d\n", ExistingOptionNum, NewOptionNum));
    } else {
     RegisterFvBootOption (PcdGetPtr (PcdShellFile), (CHAR16 *) PcdGetPtr(PcdShellDescription), (UINTN) -1, TRUE);
    }
    //
    // 2. Setup menu
    //
    SetupMenuScanCode.UnicodeChar  = CHAR_NULL;
    SetupMenuScanCode.ScanCode     = PcdGet16(PcdSetupMenuScanCode);
  Status          = EfiBootManagerGetBootManagerMenu (&BootOption);
  ASSERT_EFI_ERROR (Status);
  Status          = EfiBootManagerAddKeyOptionVariable (NULL, (UINT16) BootOption.OptionNumber, 0, &SetupMenuScanCode, NULL);
    ASSERT (Status == EFI_ALREADY_STARTED || Status == EFI_SUCCESS);
  EfiBootManagerFreeLoadOption (&BootOption);

    //
    // 3. Boot Device List menu
    //
    BootDeviceListScanCode.UnicodeChar  = CHAR_NULL;
    BootDeviceListScanCode.ScanCode     = PcdGet16(PcdBootDeviceListScanCode);
    OptionNumber    = RegisterFvBootOption (BootMenuFileGuid, L"Boot Device List", (UINTN) -1, PcdGetBool (PcdIsBootCategory));
    Status = EfiBootManagerAddKeyOptionVariable (NULL, OptionNumber, 0, &BootDeviceListScanCode, NULL);
    ASSERT (Status == EFI_ALREADY_STARTED || Status == EFI_SUCCESS);
    mBootMenuOptionNumber = OptionNumber;

    //
    // 4. Create platform specified Driver####
    //
    DriverOptions = EfiBootManagerGetLoadOptions (&DriverOptionCount, LoadOptionTypeDriver);

    for (Index = 0; gPlatformDriverOption[Index] != NULL; Index++) {
      Status = EfiBootManagerInitializeLoadOption (
                 &DriverOption,
                 LoadOptionNumberUnassigned,
                 LoadOptionTypeDriver,
                 LOAD_OPTION_ACTIVE,
                 L"",
                 gPlatformDriverOption[Index],
                 NULL,
                 0
                 );
      ASSERT_EFI_ERROR (Status);

//      if (EfiBootManagerFindLoadOption (&DriverOption, DriverOptions, DriverOptionCount) == -1) {
        Status = EfiBootManagerAddLoadOptionVariable (&DriverOption, (UINTN) -1);
        ASSERT_EFI_ERROR (Status);
//      }

      EfiBootManagerFreeLoadOption (&DriverOption);
    }

    EfiBootManagerFreeLoadOptions (DriverOptions, DriverOptionCount);
  }

/**

  Perform the platform diagnostic, such like test memory. OEM/IBV also
  can customize this fuction to support specific platform diagnostic.

  @param MemoryTestLevel  - The memory test intensive level

  QuietBoot        - Indicate if need to enable the quiet boot

  BaseMemoryTest   - A pointer to BdsMemoryTest()

  @retval None.

**/
VOID
Diagnostics (
  IN EXTENDMEM_COVERAGE_LEVEL    MemoryTestLevel,
  IN BOOLEAN                     QuietBoot
  )
{
  EFI_STATUS  Status;
  UINT8       DfxPpvEnabled = DFX_PPV_AUTO;

  if(!PcdGetBool(PcdCustomerMemoryTest)){
    Status = GetOptionData (&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, DfxPpvEnabled), &DfxPpvEnabled, sizeof(DfxPpvEnabled));
    if (EFI_ERROR(Status)) {
      DfxPpvEnabled = DFX_PPV_AUTO;
    }

    //
    // Here we can decide if we need to show
    // the diagnostics screen
    // Notes: this quiet boot code should be remove
    // from the graphic lib
    //

    if (QuietBoot && (DfxPpvEnabled != DFX_PPV_ENABLED)) {
      EnableQuietBoot (PcdGetPtr(PcdLogoFile));

      //
      // Perform system diagnostic
      //
      Status = MemoryTest (MemoryTestLevel);
      if (EFI_ERROR (Status)) {
        DisableQuietBoot ();
      }
      return ;
    }
    //
    // Perform system diagnostic
    //
    Status = MemoryTest (MemoryTestLevel);
  }else{
      //
      // Perform Platform system diagnostic
      //
      Status = PlatformMemoryTest (MemoryTestLevel);
      return ;
  }
}

/**
  Returns a priority number based on the device type of the boot option
  and where it should, relatively, appear in the boot order

  @param BootOption
**/
UINTN
BootOptionPriority (
  CONST EFI_BOOT_MANAGER_LOAD_OPTION *BootOption
  )
{
  EFI_HANDLE                  BootableDeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL    *DevicePath;
  if (PcdGetBool(PcdDfxAdvDebugJumper)) {
    if ( BootOption->FilePath != NULL ) {
      //
      // Check this device can boot from EFI file.
      //
      BootableDeviceHandle = GetBootableDeviceHandle (BootOption->FilePath);
      if (BootableDeviceHandle) {
        DevicePath = DevicePathFromHandle (BootableDeviceHandle);
        ASSERT (DevicePath != NULL);
        if (DevicePath != NULL) {
          while (!IsDevicePathEndType(NextDevicePathNode(DevicePath))) {
            DevicePath = NextDevicePathNode(DevicePath);
          }
          switch (DevicePathSubType(DevicePath)) {
            case MEDIA_HARDDRIVE_DP:
              if (mForceValue == FORCE_TO_UEFI_HD) {
                return 5;
              } else {
                return 10;
              }
              break;
            case MEDIA_CDROM_DP:
              if (mForceValue == FORCE_TO_UEFI_CD) {
                return 6;
              } else {
                return 11;
              }
              break;
            case MSG_USB_CLASS_DP:
            case MSG_USB_WWID_DP:
              if (mForceValue == FORCE_TO_UEFI_USB) {
                return 9;
              } else {
                return 12;
              }
              break;
            default:
              DEBUG ((EFI_D_WARN, "default case.\n"));  //Auto added. Please review.
              break;
          }
        }
      }
    }
  if ( StrStr (BootOption->Description, (CHAR16 *)PcdGetPtr(PcdShellDescription) ) != NULL) {
      if (mSetShellFirst == TRUE || (mForceValue == FORCE_TO_SHELL)) {
        return 7;
      } else {
        return 100;
      }
    }
    if (StrCmp (BootOption->Description, L"Boot Device List") == 0) {
      if (mForceValue == FORCE_TO_BOOT_DEVICE_LIST) {
        return 4;
      }
    }
  } else {
    if ( BootOption->FilePath != NULL ) {
      if (DevicePathType (BootOption->FilePath) == ACPI_DEVICE_PATH) {
        //
        // Check this device can boot from EFI file.
        //
        if (GetBootableDeviceHandle (BootOption->FilePath)) {
          return 5;
        }
      }
    }
    //
    // Make sure Shell is last
    //
  if ( StrStr (BootOption->Description, (CHAR16 *)PcdGetPtr(PcdShellDescription) ) != NULL) {
      //
      // If Set Shell First enabled, make sure Shell is first.
      //
      if (mSetShellFirst == TRUE) {
        return 0;
      } else {
        return 100;
      }
    }
    #if defined(CLUSTER_ENABLE) && CLUSTER_ENABLE
      if ( StrStr (BootOption->Description, L"PXE" ) != NULL) {
          return 1;
      }
    #endif
  }
  return 30;
}

/**

    GC_TODO: add routine description

    @param Left  - GC_TODO: add arg description
    @param Right - GC_TODO: add arg description

    @retval (BOOLEAN) (BootOptionPriority (Left) < BootOptionPriority (Right)) - GC_TODO: add retval description

**/

INTN
EFIAPI
CompareBootOption (
  IN CONST VOID                 *Buffer1,
  IN CONST VOID                 *Buffer2
  )

{
  return (BOOLEAN) (BootOptionPriority ((EFI_BOOT_MANAGER_LOAD_OPTION  *) Buffer1) > BootOptionPriority ((EFI_BOOT_MANAGER_LOAD_OPTION  *) Buffer2));
}

extern EFI_GUID  gSignalBeforeEnterSetupGuid;

/**
  This function Installs a guid before entering the Setup.

**/
VOID
SignalProtocolEvent(IN EFI_GUID *ProtocolGuid)
{
  EFI_HANDLE  Handle = NULL;

  gBS->InstallProtocolInterface (
      &Handle, ProtocolGuid, EFI_NATIVE_INTERFACE,NULL
  );

  gBS->UninstallProtocolInterface (
      Handle, ProtocolGuid, NULL
  );
}


/**
  Compare two device pathes to check if they are exactly same.

  @param DevicePath1    A pointer to the first device path data structure.
  @param DevicePath2    A pointer to the second device path data structure.

  @retval TRUE    They are same.
  @retval FALSE   They are not same.

**/
BOOLEAN
EfiCompareDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath1,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath2
  )
{
  UINTN Size1;
  UINTN Size2;

  Size1 = GetDevicePathSize (DevicePath1);
  Size2 = GetDevicePathSize (DevicePath2);

  if (Size1 != Size2) {
    return FALSE;
  }

  if (CompareMem (DevicePath1, DevicePath2, Size1) != 0) {
    return FALSE;
  }

  return TRUE;
}


/**
  Remove duplicate Boot entires
  Touches BootOrder and BootXXXX variables
  @param None

  @retval None

**/

VOID
EfiBootManagerUniqueBootOptions (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION    *BootOptions, *BootOptionsOrg;
  UINTN                           BootOptionCount, BootOrderCount;
  UINTN                           Index1, Index2, tempIndex;
  UINT16                          *BootOrderList;
  UINTN                           BootOrderListSize;
  BOOLEAN                         BootOrderChanged = FALSE;
  EFI_STATUS                      Status;


  EfiBootManagerRefreshAllBootOption();

  Status = GetVariable2 (
                    L"BootOrder",
                    &gEfiGlobalVariableGuid,
                    &BootOrderList,
                    &BootOrderListSize
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }
  if (BootOrderList == NULL) {
    return;
  }
  if (BootOrderListSize >= 2) { //Needs to be at least 2 bytes in length; if 1 byte in length, the "for Index1" acts like an effective infinite loop
    BootOptionsOrg = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);
    ASSERT (BootOptionsOrg != NULL);
    if (BootOptionsOrg == NULL) {
      return;
    }
    BootOptions = BootOptionsOrg;

    BootOrderCount = BootOrderListSize / sizeof(UINT16);
    //
    // Traverse the existing Boot Order to find if there are duplicate entries
    //
    for(Index1 = 0; Index1 < (BootOrderCount-1); Index1++) {
      //
      // Index1 Points "Key_Item" to be searched all over the BootOrder list
      //
      for( Index2 = Index1 + 1; Index2 < BootOrderCount; Index2++) {
        ASSERT(BootOrderList[Index2] == BootOptions[Index2].OptionNumber);
        if( BootOrderList [Index1] == BootOrderList[Index2] ){
          //
          // Redundancy Found
          //
          BootOrderChanged = TRUE;
          DEBUG ((EFI_D_INFO, "[Bds]Duplicate BootOrder Entry Boot%04x: Deleted\n",BootOrderList[Index2]));
          CopyMem((UINT8 *)&BootOrderList[Index2], (UINT8 *)&BootOrderList[Index2+1], ((BootOrderCount-(Index2+1))* sizeof(UINT16)));
          EfiBootManagerFreeLoadOption(&BootOptions[Index2]);
          CopyMem((UINT8 *)&BootOptions[Index2], (UINT8*)&BootOptions[Index2+1],  ((BootOrderCount-(Index2+1))* sizeof(EFI_BOOT_MANAGER_LOAD_OPTION)));
          BootOrderCount--;
          Index2--;  // Keep Travering the BootOrder from the same position with new data
        }
        if (!((BootOptions[Index1].OptionalDataSize == sizeof (EFI_GUID)) &&
              (CompareGuid ((EFI_GUID *) BootOptions[Index1].OptionalData, &mBmAutoCreateBootOptionGuid)))) {
          if (!((BootOptions[Index2].OptionalDataSize == sizeof (EFI_GUID)) &&
                (CompareGuid ((EFI_GUID *) BootOptions[Index2].OptionalData, &mBmAutoCreateBootOptionGuid)))) {
            if ((BootOptions[Index1].OptionNumber != BootOptions[Index2].OptionNumber) &&
                 EfiCompareDevicePath(BootOptions[Index1].FilePath, BootOptions[Index2].FilePath)) {
              //
              // Found redundant Boot Variable for same boot device path
              //
              BootOrderChanged = TRUE;
              if(BootOptions[Index1].OptionNumber > BootOptions[Index2].OptionNumber) {
                tempIndex = Index1;
              } else {
                tempIndex = Index2;
              }
              DEBUG ((EFI_D_INFO, "[Bds]Redundant BootVariable Boot%04x => %s: Deleted\n",BootOptions[tempIndex].OptionNumber, BootOptions[tempIndex].Description));
              CopyMem((UINT8 *)&BootOrderList[tempIndex], (UINT8 *)&BootOrderList[tempIndex+1], ((BootOrderCount-(tempIndex+1))* sizeof(UINT16)));
              EfiBootManagerFreeLoadOption(&BootOptions[tempIndex]);
              CopyMem((UINT8 *)&BootOptions[tempIndex], (UINT8*)&BootOptions[tempIndex+1],  ((BootOrderCount-(tempIndex+1))* sizeof(EFI_BOOT_MANAGER_LOAD_OPTION)));
              BootOrderCount--;
              Index2--;          // Keep Traversing the BootOrder from the same position with new data
            }
          }
        }
      }// For Index 2
    }// For index1
    if (BootOrderChanged) {
      BootOrderListSize = BootOrderCount * sizeof(UINT16);

      Status = gRT->SetVariable (
               L"BootOrder",
                   &gEfiGlobalVariableGuid,
                   EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           BootOrderListSize,
           BootOrderList
               );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_WARN, "[Bds]Redundant BootVariable Save failed, Status is ->%r\n",Status));
      }

    }
    EfiBootManagerFreeLoadOptions(BootOptionsOrg, BootOrderCount);
    FreePool(BootOrderList);
  }
}

/**

  The function will execute with as the platform policy, current policy
  is driven by boot mode. IBV/OEM can customize this code for their specific
  policy action.

  @param DriverOptionList - The header of the driver option link list

  BootOptionList   - The header of the boot option link list

  ProcessCapsules  - A pointer to ProcessCapsules()

  BaseMemoryTest   - A pointer to BaseMemoryTest()

  @retval None.

**/
VOID
EFIAPI
PlatformBootManagerAfterConsole (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_BOOT_MODE             BootMode;
  BOOLEAN                   DeferredImageExist;
  UINTN                     Index;
  CHAR16                    CapsuleVarName[MAX_LENGTH_CAPSULEVARNAME];
  CHAR16                    *TempVarName;
  BOOLEAN                   IsFirstBoot;
  UINT8                     SetShellFirstOption;
  UINT8                     ForceSetupOption;
  UINT8                     SocketId = 0;
  BIOS_SCRATCHPAD5_STRUCT   Sp5;
  BOOLEAN          QuietBoot = TRUE;
  PRINT_BOOT_PROMPT_PROTOCOL *PrintBootPrompt;

  if (PcdGetBool(PcdDfxAdvDebugJumper)) {
    Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
    if (Status == EFI_SUCCESS) {
      //
      // Use BIT16:BIT19 in BIOSSCRATCHPAD5_UBOX_MISC_REG for Boot Device ScratchPad
      //
      Sp5.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG);
      DEBUG ((EFI_D_INFO, "Scratchpad_Debug PlatformBootManagerAfterConsole, Scratchpad5 = %d\n", Sp5.Data));
      mForceValue = Sp5.Bits.Bds;
    }
  }

  Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, SetShellFirst), &SetShellFirstOption, sizeof(SetShellFirstOption));
  Status = GetOptionData(&gEfiSetupVariableGuid, OFFSET_OF (SYSTEM_CONFIGURATION, ForceSetup), &ForceSetupOption, sizeof(ForceSetupOption));

  //
  // Uba smbios lib update SMBios
  //
  PlatformInitSmbiosUpdate();

  //
  // Check BootState PCD, TRUE means it's the first boot after reflashing
  //
  IsFirstBoot = PcdGetBool(PcdBootState);

  BootMode = GetBootModeHob ();
  DEBUG ((EFI_D_INFO, "[PlatformBds]BootMode = %d\n", (UINTN) BootMode));

  QuietBoot = PcdGetBool(PcdQuietBootFlag);

  //
  // Clear all the capsule variables CapsuleUpdateData, CapsuleUpdateData1, CapsuleUpdateData2...
  // as early as possible which will avoid the next time boot after the capsule update
  // will still into the capsule loop
  //
  Status = StrnCpyS (CapsuleVarName, MAX_LENGTH_CAPSULEVARNAME, EFI_CAPSULE_VARIABLE_NAME, (sizeof(EFI_CAPSULE_VARIABLE_NAME)/sizeof(CHAR16)) - 1);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }
  TempVarName = CapsuleVarName + StrLen (CapsuleVarName);
  Index = 0;
  while (TRUE) {
    if (Index > 0) {
      UnicodeValueToStringS (
        TempVarName,
        sizeof (CapsuleVarName) - ((UINTN)TempVarName - (UINTN)CapsuleVarName),
        0,
        Index,
        0
        );
    }
    Status = gRT->SetVariable (
                 CapsuleVarName,
                 &gEfiCapsuleVendorGuid,
                 EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                 0,
                 (VOID *)NULL
                 );
    if (EFI_ERROR (Status)) {
      //
      // There is no capsule variables, quit
      //
      break;
    }
    Index++;
  }

  //
  // No deferred image exists by default
  //
  DeferredImageExist = FALSE;

  //
  // After Console is available, run WS specific action
  //
#if defined(AMT_SUPPORT) && AMT_SUPPORT
   if (MeClientIsCorporate () && MeClientIsReady ()) {
    AmtConsoleReady ();
    DEBUG((EFI_D_INFO,"Local bootmode before Amt update -%x\n", BootMode));
    AmtUpdateBootMode (&BootMode);
    DEBUG ((EFI_D_INFO, "Current local bootmode - %x\n", BootMode));
  }
#endif // AMT_SUPPORT
  //
  // Go the different platform policy with different boot mode
  // Notes: this part code can be change with the table policy
  //
  switch (BootMode) {
  case BOOT_ON_FLASH_UPDATE:
    //
    // Boot with the specific configuration
    //
    Diagnostics (EXTENSIVE, FALSE);
    EfiBootManagerConnectAll ();

    //
    // Perform user identification
    //
    if (mCurrentUser == NULL) {
      CheckDeferredImage (&mCurrentUser, &DeferredImageExist);
      if (DeferredImageExist) {
        //
        // After user authentication, the deferred drivers was loaded again.
        // Here, need to ensure the deferred images are connected.
        //
        EfiBootManagerConnectAll ();
      }
    }

    break;

  case BOOT_IN_RECOVERY_MODE:
    //
    // In recovery mode, just connect platform console
    // and show up the front page
    //
    Diagnostics (EXTENSIVE, FALSE);
    EfiBootManagerConnectAll ();

    //
    // Perform user identification
    //
    if (mCurrentUser == NULL) {
      CheckDeferredImage (&mCurrentUser, &DeferredImageExist);
      if (DeferredImageExist) {
        //
        // After user authentication, the deferred drivers was loaded again.
        // Here, need to ensure the deferred drivers are connected.
        //
        EfiBootManagerConnectAll ();
      }
    }

    break;

  case BOOT_WITH_FULL_CONFIGURATION:
  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
  case BOOT_WITH_DEFAULT_SETTINGS:
    //
    // Memory test and Logo show
    //
    if (HybridSystemLevelEmulationEnabled ()) {
      Diagnostics (IGNORE, FALSE);
    } else {
      Diagnostics (IGNORE, QuietBoot);
    }
    //
    // Perform some platform specific connect sequence
    //
    ConnectSequence ();

    //
    // Perform user identification
    //
    if (mCurrentUser == NULL) {
      CheckDeferredImage (&mCurrentUser, &DeferredImageExist);
      if (DeferredImageExist) {
        //
        // After user authentication, the deferred drivers was loaded again.
        // Here, need to ensure the deferred drivers are connected.
        //
        EfiBootManagerConnectAllDefaultConsoles ();
        ConnectSequence ();
      }
    }

    //
    // Here we have enough time to do the enumeration of boot device
    //
   // EfiBootManagerRefreshAllBootOption ();
   // if (IsFirstBoot) {
   //   EfiBootManagerSortLoadOptionVariable (LoadOptionTypeBoot, CompareBootOption);
   // }

    break;
  case FAST_BOOT_MODE:
    //
    // Do nothing for FastBoot mode.
    // Will not connect any device here, and assume the boot option created by
    // previous OS boot is not changed.
    //
    break;
  case BOOT_WITH_MFG_MODE_SETTINGS:
    //
    // For manufacturing mode, UEFI Shell needs to be the first boot option
    //
    if (SetShellFirstOption) {
      mSetShellFirst = TRUE;
      if (IsFirstBoot) {
        EfiBootManagerSortLoadOptionVariable (LoadOptionTypeBoot, CompareBootOption);
      }
    }
    // No break statement needed. Continue to default.
  case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
  case BOOT_WITH_MINIMAL_CONFIGURATION:
  default:
    //
    // Memory test and Logo show
    //
      Diagnostics (IGNORE, QuietBoot);

    //
    // Perform some platform specific connect sequence
    //
    ConnectSequence ();

    //
    // As console is ready, perform user identification again.
    //
    if (mCurrentUser == NULL) {
      CheckDeferredImage (&mCurrentUser, &DeferredImageExist);
      if (DeferredImageExist) {
        //
        // After user authentication, the deferred drivers was loaded again.
        // Here, need to ensure the deferred images are connected.
        //
        EfiBootManagerConnectAllDefaultConsoles ();
        ConnectSequence ();
      }
    }
    break;
  }
  //
  // Here we have enough time to do the enumeration of boot device
  //
  EfiBootManagerRefreshAllBootOption ();
  if (IsFirstBoot) {
    EfiBootManagerSortLoadOptionVariable (LoadOptionTypeBoot, CompareBootOption);
  }

  if (PcdGetBool(PcdDfxAdvDebugJumper)) {
    DEBUG ((EFI_D_INFO, "PcdBootDeviceScratchPad5Changed: %d\n", PcdGetBool(PcdBootDeviceScratchPad5Changed)));
    if (PcdGetBool(PcdBootDeviceScratchPad5Changed) || mForceValue > 0 || SetShellFirstOption) {
      if (SetShellFirstOption) {
        mSetShellFirst = TRUE;
        Status = PcdSetBoolS(PcdBootDeviceScratchPad5Changed, TRUE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return;
        }
      }
      //
      // Update the Boot order here if the scrathcpad changed and it is not the first boot.
      //
      if (IsFirstBoot == FALSE) {
        EfiBootManagerSortLoadOptionVariable (LoadOptionTypeBoot, CompareBootOption);
      }
      if (PcdGetBool(PcdBootDeviceScratchPad5Changed) && mForceValue == 0 && mSetShellFirst == FALSE) {
        Status = PcdSetBoolS(PcdBootDeviceScratchPad5Changed, FALSE);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)) {
          return;
        }
      }
    }
  } else {
    //
    // When SetShellFirst question enabled, UEFI SHELL will be boot first
    //
    if (SetShellFirstOption) {
      mSetShellFirst = TRUE;
      EfiBootManagerSortLoadOptionVariable (LoadOptionTypeBoot, CompareBootOption);
    }
  }


#ifndef SILENT_MODE
  GenGbtXml();
#endif

  //
  // Not needed for most customers
  //
  if (FeaturePcdGet(PcdPreUefiLegacyEnable)) {
    Status = gBS->LocateProtocol (&gPrintBootPromptProtocolGuid, NULL, (VOID **)&PrintBootPrompt);
    if (!EFI_ERROR (Status)) {
      PrintBootPrompt->PrintPrompt (PrintBootPrompt, BootMode);
    }
  }

#if defined(AMT_SUPPORT) && AMT_SUPPORT
  if (MeClientIsCorporate () && MeClientIsReady ()) {
    AmtBdsBoot ();
  }
#endif // AMT_SUPPORT

  //
  // Signal a event before entering Setup
  //
  DEBUG ((EFI_D_INFO, "Before Entering Setup...\n"));
  if (BootMode != FAST_BOOT_MODE) {
    SignalProtocolEvent(&gSignalBeforeEnterSetupGuid);
  }
  //
  // Signal a event in platform boot manager after console for platform hook
  //
  SignalEventAfterConsole();

  //
  // 4988321: PROMOTE from haswell_server: HSX BIOS Asserts after so many reboots
  // RHEL 7.0 Grub corrupts the "BootOrder" variable and duplicates the boot order entries
  // WA to ensure the UNIQUE of boot Order entires
  //
  EfiBootManagerUniqueBootOptions();

  if (PcdGetBool(PcdDfxAdvDebugJumper)) {
    if (mForceValue > 0) {
      if (mForceValue == FORCE_TO_SETUP) {
        ForceSetupOption = 1;
      }
      //
      // Clear BIT16:BI19 in BIOSSCRATCHPAD5_UBOX_MISC_REG
      //

      Sp5.Bits.Bds = 0;
      mCpuCsrAccess->WriteCpuCsr(SocketId, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG, Sp5.Data);
      DEBUG ((EFI_D_INFO, "Scratchpad_Debug PlatformBootManagerAfterConsole, Write Scratchpad5 %d\n", Sp5.Data));

      //
      // ScratchPad is changed, set PCD ture here.
      //
      Status = PcdSetBoolS(PcdBootDeviceScratchPad5Changed, TRUE);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR(Status)) {
        return;
      }
    }
  }

  if ((ForceSetupOption == 1) && (BootMode != BOOT_WITH_MFG_MODE_SETTINGS)) {
    ForceSetup ();
  }

  if (IsFirstBoot) {
    Status = PcdSetBoolS(PcdBootState, FALSE);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
      return;
    }
  }
}

/**
  Connect the predefined platform default authentication devices.

  This function connects the predefined device path for authentication device,
  and if the predefined device path has child device path, the child handle will
  be connected too. But the child handle of the child will not be connected.

**/
VOID
EFIAPI
ConnectAuthDevice (
  VOID
  )
{
  EFI_STATUS                   Status;
  UINTN                        Index;
  UINTN                        HandleIndex;
  UINTN                        HandleCount;
  EFI_HANDLE                   *HandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL     *ChildDevicePath;
  EFI_USER_MANAGER_PROTOCOL    *Manager;

  Status = gBS->LocateProtocol (
                  &gEfiUserManagerProtocolGuid,
                  NULL,
                  (VOID **) &Manager
                  );
  if (EFI_ERROR (Status)) {
    //
    // As user manager protocol is not installed, the authentication devices
    // should not be connected.
    //
    return ;
  }

  Index = 0;
  while (gUserAuthenticationDevice[Index] != NULL) {
    //
    // Connect the platform customized device paths
    //
    EfiBootManagerConnectDevicePath (gUserAuthenticationDevice[Index], NULL);
    Index++;
  }

  //
  // Find and connect the child device paths of the platform customized device paths
  //
  HandleBuffer = NULL;
  for (Index = 0; gUserAuthenticationDevice[Index] != NULL; Index++) {
    HandleCount = 0;
    Status = gBS->LocateHandleBuffer (
                    AllHandles,
                    NULL,
                    NULL,
                    &HandleCount,
                    &HandleBuffer
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Find and connect the child device paths of gUserIdentificationDevice[Index]
    //
    for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
      ChildDevicePath = NULL;
      Status = gBS->HandleProtocol (
                      HandleBuffer[HandleIndex],
                      &gEfiDevicePathProtocolGuid,
                      (VOID **) &ChildDevicePath
                      );
      if (EFI_ERROR (Status) || ChildDevicePath == NULL) {
        continue;
      }

      if (CompareMem (
            ChildDevicePath,
            gUserAuthenticationDevice[Index],
            (GetDevicePathSize (gUserAuthenticationDevice[Index]) - sizeof (EFI_DEVICE_PATH_PROTOCOL))
            ) != 0) {
        continue;
      }
      gBS->ConnectController (HandleBuffer[HandleIndex], NULL, NULL, TRUE);
    }
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }
}

/**
  This function is to identify a user, and return whether deferred images exist.

  @param[out]  User               Point to user profile handle.
  @param[out]  DeferredImageExist On return, points to TRUE if the deferred image
                                  exist or FALSE if it did not exist.

**/
VOID
CheckDeferredImage (
  OUT EFI_USER_PROFILE_HANDLE        *User,
  OUT BOOLEAN                        *DeferredImageExist
  )
{
  EFI_STATUS                         Status;
  EFI_DEFERRED_IMAGE_LOAD_PROTOCOL   *DeferredImage;
  UINTN                              HandleCount;
  EFI_HANDLE                         *HandleBuf;
  UINTN                              Index;
  UINTN                              DriverIndex;
  EFI_DEVICE_PATH_PROTOCOL           *ImageDevicePath;
  VOID                               *DriverImage;
  UINTN                              ImageSize;
  BOOLEAN                            BootOption;

  //
  // Perform user identification
  //
  do {
    Status = UserIdentify (User);
  } while (EFI_ERROR (Status));

  //
  // After user authentication now, try to find whether deferred images exists
  //
  HandleCount = 0;
  HandleBuf   = NULL;
  *DeferredImageExist = FALSE;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDeferredImageLoadProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuf
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuf[Index],
                    &gEfiDeferredImageLoadProtocolGuid,
                    (VOID **) &DeferredImage
                    );
    if (!EFI_ERROR (Status)) {
      //
      // Find whether deferred image exists in this instance.
      //
      DriverIndex = 0;
      Status = DeferredImage->GetImageInfo(
                                DeferredImage,
                                DriverIndex,
                                &ImageDevicePath,
                                (VOID **) &DriverImage,
                                &ImageSize,
                                &BootOption
                                );
      if (!EFI_ERROR (Status)) {
        //
        // The deferred image is found.
        //
        FreePool (HandleBuf);
        *DeferredImageExist = TRUE;
        return ;
      }
    }
  }

  FreePool (HandleBuf);
}

/**
  The function is called when no boot option could be launched,
  including platform recovery options and options pointing to applications
  built into firmware volumes.

  If this function returns, BDS attempts to enter an infinite loop.
**/
VOID
EFIAPI
PlatformBootManagerUnableToBoot (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION BootDeviceList;
  CHAR16                       OptionName[sizeof ("Boot####")];

  if (mBootMenuOptionNumber == LoadOptionNumberUnassigned) {
    return;
  }
  UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", mBootMenuOptionNumber);
  Status = EfiBootManagerVariableToLoadOption (OptionName, &BootDeviceList);
  ASSERT_EFI_ERROR (Status);

  for (;;) {
    EfiBootManagerBoot (&BootDeviceList);
  }
}
