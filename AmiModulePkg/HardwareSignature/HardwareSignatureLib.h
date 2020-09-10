//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file  HardwareSignatureLib.h

  The header file for Hardware Signature Library. 

**/

#ifndef _HARDWARE_SIGNATURE_LIB_H_
#define _HARDWARE_SIGNATURE_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif
///----------------------------------------------------------------------
/// Include Files
///----------------------------------------------------------------------
#include <Token.h>
#include <Protocol/PciIo.h>
#include <Protocol/UsbIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EdidDiscovered.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/BlockIo.h>
#include <IndustryStandard/AmiAtaAtapi.h>

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>

#define HARDWARE_CONFIG_DATA_VARIABLE L"HardwareConfigData"

#define FIRMWARE_UPDATE_COUNT_VARIABLE L"FirmwareUpdateCountVar"

#define IsRestoreMemoryType(MemoryType) \
    ((MemoryType) == EfiACPIReclaimMemory || \
    (MemoryType) == EfiACPIMemoryNVS || \
    (MemoryType) == EfiRuntimeServicesCode || \
    (MemoryType) == EfiRuntimeServicesData || \
    (MemoryType) == EfiReservedMemoryType )

#define VESA_BIOS_EXTENSIONS_EDID_BLOCK_SIZE 128

/**
 *  Status Register contains various flags that indicate the state of the PS/2 controller.
 */ 
typedef enum {
    PS2_STATUS_OUTPUT_BUFFER       = BIT0, ///< Output buffer status (0 = empty, 1 = full) 
    PS2_STATUS_INPUT_BUFFER        = BIT1, ///< Input buffer status (0 = empty, 1 = full) 
    PS2_STATUS_SYSTEM_FLAG         = BIT2, ///< System Flag
    PS2_STATUS_COMMAND_DATA        = BIT3, ///< 0 = data written to input buffer is data for PS/2 device, 1 = data written to input buffer is data for PS/2 controller command
    PS2_STATUS_INHIBIT_SWITCH      = BIT4, ///< Inhibit Switch 
    PS2_STATUS_TRANSMIT_TIME_OUT   = BIT5, ///< Transmit Time Out (0 = no error, 1 = time-out error) 
    PS2_STATUS_RECEIVE_TIME_OUT    = BIT6, ///< Receive Time Out (0 = no error, 1 = time-out error) 
    PS2_STATUS_PARITY_ERROR        = BIT7, ///< Parity error (0 = no error, 1 = parity error)
} PS2_STATUS_REGISTER;

/**
 *  The PS/2 Controller IO ports 
 */ 
#define PS2_COMMAND_STATUS_PORT 0x64  ///< PS2_COMMAND_STATUS_PORT  
#define PS2_DATA_PORT           0x60  ///< PS2_DATA_PORT

/**
 *  PS2 Controller Commands 
 */ 
typedef enum {
    PS2_KEYBOARD_WRITE_MODE_COMMAND     = 0x60, ///< Keyboard Write Mode 
    PS2_WRITE_MOUSE_COMMAND             = 0xD4, ///< Write the following byte to the mouse
    PS2_DISABLE_PS2_PORT                = 0xA7, ///< Disable second PS/2 port (only if 2 PS/2 ports supported)
    PS2_DISABLE_KEYBOARD_INTERFACE      = 0xAD, ///< Disable Keyboard Feature
    PS2_ENABLE_KEYBOARD_INTERFACE       = 0xAE, ///< Enable keyboard interface
    PS2_SET_RESOLUTION_COMMAND          = 0xE8, ///< Set resolution
    PS2_DIAGNOSTIC_FAILURE_COMMAND      = 0xFD, ///< 
    PS2_GET_DEVICE_ID_COMMAND           = 0xF2, ///< Get Device ID
    PS2_SET_SAMPLE_RATE                 = 0xF3, ///< Set Sample Rate
    PS2_ENABLE_SCANNING_COMMAND         = 0xF4, ///< Enable keyboard scanning
    PS2_DISABLE_SCANNING_COMMAND        = 0xF5, ///< Disable keyboard scanning
    PS2_RESET_COMMAND                   = 0xFF, ///< Reset command
    PS2_OVER_RUN_COMMAND                = 0xFF, ///< OVERRUN command
} PS2_COMMAND;

/**
 *  PS2 Replies
 */ 
typedef enum {
    PS2_MOUSE_ID                        = 0x00,
    PS2_RESPONSE_RESET_SUCCESS          = 0xAA, ///< Power on reset Successfully
    PS2_RESPONSE_RESET_FAIL             = 0xFC,
    PS2_RESPONSE_ACK                    = 0xFA, ///< ACKNOWLEDGE command  
    PS2_RESPONSE_RESEND                 = 0xFE, ///< Re-send command
} PS2_RESPONSE;

#define PS2_MOUSE_CLEAR_OBF_COUNTDOWN               200 ///< ClearObfCountdown
#define MAX_RETRY_TIMES_OF_DETECTING_PS2_MOUSE      3   ///< max retry time of detecting ps2 mouse

/**
 *  PS2 Mouse Samples Rate
 */ 
typedef enum {
    PS2_MOUSE_SAMPLE_RATE_200           = 0xC8, ///< 200 samples/sec
    PS2_MOUSE_SAMPLE_RATE_100           = 0x64, ///< 100 samples/sec
    PS2_MOUSE_SAMPLE_RATE_80            = 0x50, ///< 80 samples/sec
    PS2_MOUSE_SAMPLE_RATE_20            = 0x14, ///< 20 samples/sec
}PS2_MOUSE_SAMPLE_RATE; 

/**
 *  PS2 Mouse Resolution
 */ 
typedef enum {
    PS2_MOUSE_1_COUNT_MM                = 0x00, ///< 1 count/mm
    PS2_MOUSE_2_COUNT_MM                = 0x01, ///< 2 count/mm
    PS2_MOUSE_4_COUNT_MM                = 0x02, ///< 4 count/mm
    PS2_MOUSE_8_COUNT_MM                = 0x03, ///< 8 count/mm
}PS2_MOUSE_RESOLUTION; 

/**
 *  PS2 Mouse Type
 */ 
typedef enum {
    PS2_3_BUTTON_MOUSE                  = 0x00, ///< 3 Button Mouse
    PS2_SCROLLING_MOUSE                 = 0x03, ///< Scrolling mouse
    PS2_SCROLLING_WHEEL_5_BUTTON_MOUSE  = 0x04, ///< Scrolling Wheel + 5 Button Mouse
}PS2_MOUSE_TYPE;

/**
 *  PS2 8042 Command Byte Bit
 */ 
typedef enum {
    PS2_ENABLE_OUTPUT_BUFFER_INTERRUPT_BIT  = BIT0, ///< Enable Output Buffer Full Interrupt
    PS2_SET_STATUS_REGISTER_SYSTEM_BIT      = BIT2, ///< 1=set status register system, 0=clear
    PS2_OVERRIDE_KEYBOARD_INHIBIT_BIT       = BIT3, ///< 1=override keyboard inhibit, 0=allow inhibit
    PS2_DISABLE_KEYBOARD_IO_BIT             = BIT4, ///< Disable keyboard I/O by driving clock line low
    PS2_DISABLE_AUX_DEVICE_BIT              = BIT5, ///< Disable auxiliary device, drives clock line low
    PS2_SCANCODE_TRANSLATION_BIT            = BIT6, ///< Scancode translation 0=AT, 1=PC/XT
}PS2_8042_COMMAND_BYTE; 

#define PS2_OUTPUT_BUFFER_READ_TIMEOUT      40
#define ONE_MILLISECOND                     1000
#define MAXIMUM_TIMEOUT_FOR_INPUT_BUFFER    0x1388

#define PS2_KEYBOARD_COMMEND_RETRY_TIMEOUT  3    ///< 3 times retry on keyboard reset

/**
 *  Identify LBA Format Data Structure, NVM Command Set Specific
 *  Figure 93 NVM Express 1.2 Spec
 */ 
typedef struct {
    
    UINT16      Ms;                             ///< Bits 15:0      Metadata Size
    UINT8       Lbads;                          ///< Bits 23:16     LBA Data Size
    UINT8       Rp : 2;                         ///< Bits 25:24     Relative Performance
    UINT8       Reserved1 :6;                   ///< Bits 31:26     Reserved
    
} NVME_LBA_FORMAT_DATA;

/**
 *  Identify Namespace Data Structure, NVM Command Set Specific
 *  Figure 92 NVM Express 1.2 Spec
 */ 
typedef struct {
    
    UINT64                  Nsize;              ///< Offset 7:0      Namespace Size
    UINT64                  Ncap;               ///< Offset 15:8     Namespace Capacity
    UINT64                  Nuse;               ///< Offset 23:16    Namespace Utilization
    UINT8                   Nsfeat;             ///< Offset 24       Namespace Features
    UINT8                   Nlbaf;              ///< Offset 25       Number of LBA Formats
    UINT8                   Flbas;              ///< Offset 26       Formatted LBA Size
    UINT8                   Mc;                 ///< Offset 27       Metadata Capabilities
    UINT8                   Dpc;                ///< Offset 28       End-to-end Data Protection Capabilities
    UINT8                   Dps;                ///< Offset 29       End-to-end Data Protection Type Settings
    UINT8                   Nmic;               ///< Offset 30       Namespace Multi-path I/O and Namespace Sharing Capabilities
    UINT8                   Rescap;             ///< Offset 31       Reservation Capabilities
    UINT8                   Fpi;                ///< Offset 32       Format Progress Indicator
    UINT8                   Reserved1;          ///< Offset 33       Reserved
    UINT16                  Nawun;              ///< Offset 35:34    Namespace Atomic Write Unit Normal
    UINT16                  Nawupf;             ///< Offset 37:36    Namespace Atomic Write Unit Power Fail
    UINT16                  Nacwu;              ///< Offset 39:38    Namespace Atomic Compare & Write Unit
    UINT16                  Nabsn;              ///< Offset 41:40    Namespace Atomic Boundary Size Normal
    UINT16                  Nabo;               ///< Offset 43:42    Namespace Atomic Boundary Offset
    UINT16                  Nabspf;             ///< Offset 45:44    Namespace Atomic Boundary Size Power Fail
    UINT8                   Reserved2[2];       ///< Offset 46:47    Reserved
    UINT8                   Nvmcap[16];         ///< Offset 63:48    NVM Capacity
    UINT8                   Reserved3[40];      ///< Offset 103:64   Reserved
    UINT8                   Nguid[16];          ///< Offset 119:104  Namespace Globally Unique Identifier
    UINT64                  Eui64;              ///< Offset 127:120  IEEE Extended Unique Identifier
    NVME_LBA_FORMAT_DATA    Lbaf[16];           ///< Offset 191:128  LBA Format 
    UINT8                   Reserved4[192];     ///< Offset 383:192  Reserved
    UINT8                   Vs[3712];           ///< Offset 4095:384 Vendor Specific   
} NVME_IDENTIFY_NAMESPACE_DATA;

/**
 *  ATA/ATAPI-6
 * 
 */ 
#define ATA_SERIAL_NUMBER_LENGTH  20 ///< Serial number (20 ASCII characters)

#define HARDWARE_SIGNATURE_FACS_ADDRESS_NUMBER 3

/**
 *  ---------------------------------------------------------------------------
 *  
 *  Function Definitions
 *  
 *  ---------------------------------------------------------------------------
 */ 
EFI_STATUS 
SetHardwareConfigDataToNvram (
    IN AMI_HARDWARE_SIGNATURE_CONFIG_DATA *HardwareConfigData );

BOOLEAN 
IsDifferentMemorySize(
    IN UINT32 CurMemorySizeMb, 
    IN UINT32 PreMemorySizeMb);

EFI_STATUS 
GetMemMapData(
    IN OUT AMI_HARDWARE_SIGNATURE_MEMORY_MAP_DATA *MemMapConfigData);

EFI_STATUS 
GetPciDeviceData(
    IN OUT AMI_HARDWARE_SIGNATURE_PCI_DATA *HardwarePciData);

EFI_STATUS 
GetUsbDeviceData(
    IN OUT AMI_HARDWARE_SIGNATURE_USB_DATA *HardwareUsbData);
        
EFI_STATUS 
GetDiskInfoData(
    OUT AMI_HARDWARE_SIGNATURE_DISK_DATA *HardwareDiskData);

EFI_STATUS 
GetVideoData(
    OUT AMI_HARDWARE_SIGNATURE_VIDEO_DATA *HardwareVideoData);
/**
 *  ---------------------------------------------------------------------------
 *  
 *  Functions to detect and set Hardware configurations
 *  
 *  ---------------------------------------------------------------------------
 */ 
#if HARDWARE_SIGNATURE_DETECT_PS2_KEYBOARD || HARDWARE_SIGNATURE_DETECT_PS2_MOUSE
EFI_STATUS 
DetectPs2KeyboardMouseState(
    IN AMI_HARDWARE_SIGNATURE_CONFIG_TYPE ConfigType, 
    IN OUT BOOLEAN *BitPresent);
#endif

#if HARDWARE_SIGNATURE_DETECT_PS2_KEYBOARD
EFI_STATUS 
DetectAndSetPs2KeyboardConfig(
    IN UINTN BufferSize, 
    IN VOID *Buffer);
#endif

#if HARDWARE_SIGNATURE_DETECT_PS2_MOUSE
EFI_STATUS 
DetectAndSetPs2MouseConfig(
    IN UINTN BufferSize, 
    IN VOID *Buffer);
#endif

EFI_STATUS 
DetectAndSetMemorySizeConfig(
    IN UINTN BufferSize, 
    IN VOID *Buffer);

EFI_STATUS 
DetectAndSetMemoryMapConfig(
    IN UINTN BufferSize, 
    IN VOID *Buffer);

EFI_STATUS 
DetectAndSetPciConfig(
    IN UINTN BufferSize, 
    IN VOID *Buffer);

#if HARDWARE_SIGNATURE_USB_CHANGE
EFI_STATUS 
DetectAndSetUsbConfig(
    IN UINTN BufferSize, 
    IN VOID *Buffer);
#endif

EFI_STATUS 
DetectAndSetVideoConfig (
    IN UINTN BufferSize,
    IN VOID  *Buffer );

EFI_STATUS 
DetectAndSetDiskConfig (
    IN UINTN BufferSize,
    IN VOID  *Buffer );

EFI_STATUS 
Ps2DetectKeyboard (
  IN AMI_HARDWARE_SIGNATURE_CONFIG_TYPE ConfigType, 
  IN OUT BOOLEAN *Present );

EFI_STATUS
Ps2DetectMouse  (
    IN AMI_HARDWARE_SIGNATURE_CONFIG_TYPE ConfigType, 
    IN OUT BOOLEAN *Present 
);

EFI_STATUS 
EFIAPI
UpdateHardwareSignatureData(
    IN EFI_EVENT Event, 
    IN VOID *Context);

VOID 
QuickSort(
    IN OUT UINT32 *ChecksumData, 
    IN     UINT32 LChecksumPosition, 
    IN     UINT32 RChecksumPosition);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif

//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


