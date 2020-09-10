/** @file
  Defined the platform specific device path which will be used by
  platform Bbd to perform the platform policy connect.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2018 Intel Corporation. <BR>

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

//
// Predefined platform default time out value
//
UINT16                            gPlatformBootTimeOutDefault = 10;

//
// Predefined platform root bridge
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ROOT_BRIDGE_DEVICE_PATH  gPlatformRootBridge0 = {
  gPciRootBridge,
  gEndEntire
};

EFI_DEVICE_PATH_PROTOCOL          *gPlatformRootBridges[] = {
  (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformRootBridge0,
  NULL
};

//
// Platform specific keyboard device path
//
PLATFORM_KEYBOARD_DEVICE_PATH     gKeyboardDevicePath = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    0,
    0x1f
  },
  {
    ACPI_DEVICE_PATH,
    ACPI_DP,
    (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
    (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8),
    EISA_PNP_ID(0x0303),
    0
  },
  gEndEntire
};

PLATFORM_PCI_SERIAL_DEVICE_PATH   gSerialDevicePathHsuart0 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    ONBOARD_SERIAL_HSUART0_FUNCTION,
    ONBOARD_SERIAL_HSUART0_DEVICE
  },
  {
    MESSAGING_DEVICE_PATH,
    MSG_UART_DP,
    (UINT8) (sizeof (UART_DEVICE_PATH)),
    (UINT8) ((sizeof (UART_DEVICE_PATH)) >> 8),
    0,
    115200,
    8,
    1,
    1
  },
  {
    MESSAGING_DEVICE_PATH,
    MSG_VENDOR_DP,
    (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
    (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8),
    DEVICE_PATH_MESSAGING_VT_100_PLUS
  },
  gEndEntire
};

//
// Platform specific serial device path
//
PLATFORM_ISA_SERIAL_DEVICE_PATH   gSerialDevicePath = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    0,
    0x1f
  },
  {
    ACPI_DEVICE_PATH,
    ACPI_DP,
    (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
    (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8),
    EISA_PNP_ID(0x0501),
    0
  },
  {
    MESSAGING_DEVICE_PATH,
    MSG_UART_DP,
    (UINT8) (sizeof (UART_DEVICE_PATH)),
    (UINT8) ((sizeof (UART_DEVICE_PATH)) >> 8),
    0,
    115200,
    8,
    1,
    1
  },
  {
    MESSAGING_DEVICE_PATH,
    MSG_VENDOR_DP,
    (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
    (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8),
    DEVICE_PATH_MESSAGING_VT_100_PLUS
  },
  gEndEntire
};

USB_CLASS_FORMAT_DEVICE_PATH gUsbClassKeyboardDevicePath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      (UINT8) (sizeof (USB_CLASS_DEVICE_PATH)),
      (UINT8) ((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
    },
    0xffff,           // VendorId
    0xffff,           // ProductId
    CLASS_HID,        // DeviceClass
    SUBCLASS_BOOT,    // DeviceSubClass
    PROTOCOL_KEYBOARD // DeviceProtocol
  },

  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    END_DEVICE_PATH_LENGTH,
    0
  }
};

//
// Predefined platform default console device path
//
BDS_CONSOLE_CONNECT_ENTRY         gPlatformConsole[] = {
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gSerialDevicePath,
    (CONSOLE_OUT | CONSOLE_IN)
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath,
    CONSOLE_IN
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gKeyboardDevicePath,
    CONSOLE_IN
  },
  {
    NULL,
    0
  }
};

//
// PILOT SIO Platform specific serial device path
//
PILOT_PLATFORM_ISA_SERIAL_DEVICE_PATH   gPilotSerialDevicePath = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    PCI_FUNCTION_NUMBER_PCH_LPC,
    PCI_DEVICE_NUMBER_PCH_LPC
  },
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0A06),
    0
  },
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {
          (UINT8) (sizeof (SIO_DEVICE_PATH)),
          (UINT8) ((sizeof (SIO_DEVICE_PATH)) >> 8)
        }
      },
      SIO_PILOTIII_GUID
    },
    { 0x2E, 0x2E, 0x2F }
  },
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0501),
    0
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_UART_DP,
      {
        (UINT8) (sizeof (UART_DEVICE_PATH)),
        (UINT8) ((sizeof (UART_DEVICE_PATH)) >> 8)
      }
    },
    0,
    115200,
    8,
    1,
    1
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    DEVICE_PATH_MESSAGING_VT_100_PLUS
  },
  gEndEntire
};

//
// AST2500 SIO Platform specific serial device path
//
AST2500_PLATFORM_ISA_SERIAL_DEVICE_PATH   gAst2500SerialDevicePath = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    PCI_FUNCTION_NUMBER_PCH_LPC,
    PCI_DEVICE_NUMBER_PCH_LPC
  },
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0A06),
    0
  },
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {
          (UINT8) (sizeof (SIO_DEVICE_PATH)),
          (UINT8) ((sizeof (SIO_DEVICE_PATH)) >> 8)
        }
      },
      SIO_AST2500_GUID
    },
    { 0x2E, 0x2E, 0x2F }
  },
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0501),
    0
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_UART_DP,
      {
        (UINT8) (sizeof (UART_DEVICE_PATH)),
        (UINT8) ((sizeof (UART_DEVICE_PATH)) >> 8)
      }
    },
    0,
    115200,
    8,
    1,
    1
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    DEVICE_PATH_MESSAGING_VT_100_PLUS
  },
  gEndEntire
};

//
// Predefined platform default console device path
//
BDS_CONSOLE_CONNECT_ENTRY         gPilotPlatformConsole[] = {
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gPilotSerialDevicePath,
    (CONSOLE_OUT | CONSOLE_IN)
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath,
    CONSOLE_IN
  },
  {
    NULL,
    0
  }
};

//
// Predefined platform default console device path
//
BDS_CONSOLE_CONNECT_ENTRY         gAst2500PlatformConsole[] = {
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gAst2500SerialDevicePath,
    (CONSOLE_OUT | CONSOLE_IN)
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath,
    CONSOLE_IN
  },
  {
    NULL,
    0
  }
};

//
// All the possible platform Agp device path
//
PLATFORM_PCIE_DEVICE_PATH         gPlatformOnboardConsole = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    0x07,
    0x1c
  },
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    VIDEO_FUNC_NUM,
    VIDEO_DEV_NUM
  },
  gEndEntire
};

//
// Legacy hard disk boot option
//
LEGACY_HD_DEVICE_PATH             gLegacyHd = {
  {
    BBS_DEVICE_PATH,
    BBS_BBS_DP,
    (UINT8) (sizeof (BBS_BBS_DEVICE_PATH)),
    (UINT8) ((sizeof (BBS_BBS_DEVICE_PATH)) >> 8),
    BBS_TYPE_HARDDRIVE,
    0,
    0
  },
  gEndEntire
};
//
// SCU device path
//
PLATFORM_ONBOARD_SCU_DEVICE_PATH         gPlatformScu = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    0x00,
    0x01
  },
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    0,
    0
  },
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    0,
    8
  },
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8) (sizeof (PCI_DEVICE_PATH)),
    (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8),
    0,
    0
  },
  gEndEntire
};


//
// Legacy cdrom boot option
//
LEGACY_HD_DEVICE_PATH             gLegacyCdrom = {
  {
    BBS_DEVICE_PATH,
    BBS_BBS_DP,
    (UINT8) (sizeof (BBS_BBS_DEVICE_PATH)),
    (UINT8) ((sizeof (BBS_BBS_DEVICE_PATH)) >> 8),
    BBS_TYPE_CDROM,
    0,
    0
  },
  gEndEntire
};

//
// Predefined platform specific perdict boot option
//
EFI_DEVICE_PATH_PROTOCOL          *gPlatformBootOption[] = {
  (EFI_DEVICE_PATH_PROTOCOL *) &gLegacyHd,
  (EFI_DEVICE_PATH_PROTOCOL *) &gLegacyCdrom,
  NULL
};

//
// Predefined platform specific driver option
//
EFI_DEVICE_PATH_PROTOCOL          *gPlatformDriverOption[] = { NULL };

//
// Predefined platform connect sequence
//
EFI_DEVICE_PATH_PROTOCOL          *gPlatformConnectSequence[] = { NULL };

//
// Platform specific USB controller device path
//
PLATFORM_USB_DEVICE_PATH gUsbDevicePath0 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8)(sizeof(PCI_DEVICE_PATH)),
    (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
    0x0,
    0x1D
  },
  gEndEntire
};

//
// Platform specific USB controller device path
//
PLATFORM_USB_DEVICE_PATH gUsbDevicePath1 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8)(sizeof(PCI_DEVICE_PATH)),
    (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
    0x1,
    0x1D
  },
  gEndEntire
};

//
// Platform specific USB controller device path
//
PLATFORM_USB_DEVICE_PATH gUsbDevicePath2 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8)(sizeof(PCI_DEVICE_PATH)),
    (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
    0x2,
    0x1D
  },
  gEndEntire
};

//
// Platform specific USB controller device path
//
PLATFORM_USB_DEVICE_PATH gUsbDevicePath3 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8)(sizeof(PCI_DEVICE_PATH)),
    (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
    0x7,
    0x1D
  },
  gEndEntire
};

//
// Platform specific USB controller device path
//
PLATFORM_USB_DEVICE_PATH gUsbDevicePath4 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8)(sizeof(PCI_DEVICE_PATH)),
    (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
    0x0,
    0x1A
  },
  gEndEntire
};

//
// Platform specific USB controller device path
//
PLATFORM_USB_DEVICE_PATH gUsbDevicePath5 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8)(sizeof(PCI_DEVICE_PATH)),
    (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
    0x1,
    0x1A
  },
  gEndEntire
};

//
// Platform specific USB controller device path
//
PLATFORM_USB_DEVICE_PATH gUsbDevicePath6 = {
  gPciRootBridge,
  {
    HARDWARE_DEVICE_PATH,
    HW_PCI_DP,
    (UINT8)(sizeof(PCI_DEVICE_PATH)),
    (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
    0x7,
    0x1A
  },
  gEndEntire
};

//
// Predefined platform device path for user authtication
//
EFI_DEVICE_PATH_PROTOCOL* gUserAuthenticationDevice[] = {
  //
  // Predefined device path for secure card (USB disk).
  //
  (EFI_DEVICE_PATH_PROTOCOL*)&gUsbDevicePath0,
  (EFI_DEVICE_PATH_PROTOCOL*)&gUsbDevicePath1,
  (EFI_DEVICE_PATH_PROTOCOL*)&gUsbDevicePath2,
  (EFI_DEVICE_PATH_PROTOCOL*)&gUsbDevicePath3,
  (EFI_DEVICE_PATH_PROTOCOL*)&gUsbDevicePath4,
  (EFI_DEVICE_PATH_PROTOCOL*)&gUsbDevicePath5,
  (EFI_DEVICE_PATH_PROTOCOL*)&gUsbDevicePath6,
  NULL
};

