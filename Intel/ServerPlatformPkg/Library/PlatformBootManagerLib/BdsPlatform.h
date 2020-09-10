/** @file
  Head file for BDS Platform specific code

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

#ifndef _BDS_PLATFORM_H
#define _BDS_PLATFORM_H

#include <PiDxe.h>

#include <Protocol/FirmwareVolume2.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/LoadFile.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/PciIo.h>
#include <Protocol/UserManager.h>
#include <Protocol/DeferredImageLoad.h>
#include <Protocol/GenericMemoryTest.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/PciPlatform.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/CpuCsrAccess.h>

#include <Guid/CapsuleVendor.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/GlobalVariable.h>
#include <Guid/SioInfo.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/PlatformBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/PrintLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/LogoLib.h>
#include <Library/HiiLib.h>

#include <Library/TcgPhysicalPresenceLib.h>
#include <Library/Tcg2PhysicalPresenceLib.h>

#include <Library/PlatformHooksLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/SetupLib.h>

#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Atapi.h>
#include <Platform.h>
#include <PchAccess.h>
#include <Library/PciLib.h>

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     ConnectType;
} BDS_CONSOLE_CONNECT_ENTRY;

#define CONSOLE_OUT 0x00000001
#define STD_ERROR   0x00000002
#define CONSOLE_IN  0x00000004
#define CONSOLE_ALL (CONSOLE_OUT | CONSOLE_IN | STD_ERROR)

#define VIDEO_DEV_NUM                   0x00
#define VIDEO_FUNC_NUM                  0x00

#define VIDEO_SELECT_AUTO               0
#define VIDEO_SELECT_ONBOARD            1
#define VIDEO_SELECT_ADDIN              2

#define ONBOARD_SERIAL_HSUART0_DEVICE      FixedPcdGet8(PcdOnBoardSerialHsuart0Device)
#define ONBOARD_SERIAL_HSUART0_FUNCTION    FixedPcdGet8(PcdOnBoardSerialHsuart0Function)

extern EFI_DEVICE_PATH_PROTOCOL        *gPlatformRootBridges [];
extern BDS_CONSOLE_CONNECT_ENTRY        gPlatformConsole [];
extern BDS_CONSOLE_CONNECT_ENTRY        gPilotPlatformConsole [];
extern BDS_CONSOLE_CONNECT_ENTRY        gAst2500PlatformConsole [];
extern EFI_DEVICE_PATH_PROTOCOL        *gPlatformAllPossibleAgpConsole [];
extern EFI_DEVICE_PATH_PROTOCOL        *gPlatformConnectSequence [];
extern EFI_DEVICE_PATH_PROTOCOL        *gPlatformDriverOption [];
extern EFI_DEVICE_PATH_PROTOCOL        *gPlatformBootOption [];
extern EFI_DEVICE_PATH_PROTOCOL        *gUserAuthenticationDevice[];

#define gPciRootBridge \
  { \
    ACPI_DEVICE_PATH, ACPI_DP, (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), (UINT8) \
      ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8), EISA_PNP_ID (0x0A03), 0 \
  }

#define gEndEntire \
  { \
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, END_DEVICE_PATH_LENGTH, 0 \
  }

//
// Platform Root Bridge
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ROOT_BRIDGE_DEVICE_PATH;

//
// Below is the platform console device path
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      Keyboard;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_KEYBOARD_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           OnboardVga;
  ACPI_ADR_DEVICE_PATH      DisplayDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ONBOARD_VGA_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           PcieBridge;
  PCI_DEVICE_PATH           PcieDevice;
  ACPI_ADR_DEVICE_PATH      DisplayDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_PCIE_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      IsaSerial;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ISA_SERIAL_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      IsaHostController;
  SIO_DEVICE_PATH           Sio;
  ACPI_HID_DEVICE_PATH      IsaSerial;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PILOT_PLATFORM_ISA_SERIAL_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      IsaHostController;
  SIO_DEVICE_PATH           Sio;
  ACPI_HID_DEVICE_PATH      IsaSerial;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} AST2500_PLATFORM_ISA_SERIAL_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           PciDevicde;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_PCI_SERIAL_DEVICE_PATH;


//
// Below is the boot option device path
//
typedef struct {
  BBS_BBS_DEVICE_PATH       LegacyHD;
  EFI_DEVICE_PATH_PROTOCOL  End;
} LEGACY_HD_DEVICE_PATH;

//
// Below is the platform IDE device path
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ATAPI_DEVICE_PATH         Ide;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_IDE_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           PcieBridge;
  PCI_DEVICE_PATH           PcieBridge2;
  PCI_DEVICE_PATH           PcieSwitch;
  PCI_DEVICE_PATH           PcieDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ONBOARD_SCU_DEVICE_PATH;


//
// Below is the platform USB controller device path for
// USB disk as user authentication device.
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           PciDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_USB_DEVICE_PATH;

//
// Floppy device path definition
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      Floppy;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_FLOPPY_DEVICE_PATH;
#define CLASS_HID           3
#define SUBCLASS_BOOT       1
#define PROTOCOL_KEYBOARD   1

typedef struct {
  USB_CLASS_DEVICE_PATH           UsbClass;
  EFI_DEVICE_PATH_PROTOCOL        End;
} USB_CLASS_FORMAT_DEVICE_PATH;

#pragma pack(1)
typedef struct {
  UINT32  dSignature;         // Signature, ASCII string
  UINT8   bLength;            // Structure length in bytes
  UINT8   bChecksum;          // Checksum
  UINT8   bVersion;           // Version Number
  UINT8   bReserved;          // Reserved for future use
  UINT32  dPtrSataController; // SATA_CONTROLLER_STRUC address
  UINT32  dReserved;          // Reserved for future use
} S2A_INSTALLATION_STRUC;

typedef struct {
  UINT32  dPtrSataController;   // Next SATA_CONTROLLER_STRUC address
  UINT8   bAttribute;           // Attribute
  UINT8   bBus;                 // Bus# of SATA controller
  UINT8   bDevice;              // Device# of SATA controller
  UINT8   bFunction;            // Function# of SATA controller
  UINT32  dImplementedPort;     // Implemented SATA Ports
  UINT32  dDevicePresentOnPort; // Device present on SATA Ports
  UINT32  dDeviceTypeOnPort;    // Device Type on SATA Ports
  UINT32  dPtrSataDevice;       // SATA_DEVICE_STRUC address
} SATA_CONTROLLER_STRUC;

typedef struct  {
  UINT32  dPtrSataDevice;               // Next SATA_DEVICE_STRUC address
  UINT8   bAttribute;                   // Attribute
  UINT8   bPort;                        // SATA Port# (0-based) where device is present
  UINT8   bIdentifyDeviceData[0x200];   // 512-bytes of identify device data
} SATA_DEVICE_STRUC;
#pragma pack()

extern PLATFORM_PCIE_DEVICE_PATH        gPlatformOnboardConsole;
extern PLATFORM_ONBOARD_SCU_DEVICE_PATH gPlatformScu;

//
// Platform BDS Functions
//

/**
  Perform the memory test base on the memory test intensive level,
  and update the memory resource.

  @param  Level         The memory test intensive level.

  @retval EFI_STATUS    Success test all the system memory and update
                        the memory resource

**/
EFI_STATUS
MemoryTest (
  IN EXTENDMEM_COVERAGE_LEVEL Level
  );

VOID
RegisterLoadOptions (
  VOID
  );

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
;

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
  );

/**
  This function is to identify a user, and return whether deferred images exist.

  @param[out]  User               Point to user profile handle.
  @param[out]  DeferredImageExist On return, points to TRUE if the deferred image
                                  exist or FALSE if it did not exist.

**/
VOID
CheckDeferredImage (
  OUT EFI_USER_PROFILE_HANDLE        *User,
  OUT BOOLEAN                        *DeferredImage
  );

VOID
InitializeStringSupport (
  VOID
  );
VOID
PrintBootPrompt (
  VOID
  );
VOID
PrintMfgModePrompt (
  VOID
  );

EFI_HANDLE
GetBootableDeviceHandle (
  IN  EFI_DEVICE_PATH_PROTOCOL        *DevicePath
  );
#endif
