/** @file
  Platform Type Driver for Harwich.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

#include <PiDxe.h>
#include <Protocol/PlatformType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/VariableWrite.h>
#include <Protocol/Spi.h>
#include <Protocol/IioUds.h>
#include <SystemBoard.h>
#include <Library/MemoryAllocationLib.h>

#include <Guid/HobList.h>
#include <Cpu/CpuCoreRegs.h>
#include <Register/PchRegsSpi.h>
#include <Register/PchRegsLpc.h>
#include <Library/MmPciBaseLib.h>
#include <PchAccess.h>
#include <Platform.h>
#include <Register/Cpuid.h>




#define EFI_PLATFORM_TYPE_DRIVER_PRIVATE_SIGNATURE  SIGNATURE_32 ('T', 'Y', 'P', 'P')
#define EFI_IIO_UDS_DRIVER_PRIVATE_SIGNATURE  SIGNATURE_32 ('S', 'D', 'U', 'I')


typedef unsigned char BYTE;     //!<  8-bit quantities
typedef unsigned short WORD;    //!< 16-bit quantities
typedef unsigned long DWORD;    //!< 32-bit quantities

typedef enum
{
#ifndef SUCCESS
    SUCCESS = 0x00,             //!< Packet it good! .data[] is valid
#endif
    DEFER = 0x01,               //!< Packet is defered. .data[1] = BufID
    W_EARLY_NACK = 0x02,        //!< Packet mastered on the SMBus by the MCU was NACKed earlier than expected
    NOT_RESP = 0x03,            //!< Packet mastered on the SMBus by the MCU was NACKed during the address byte
    BUFFER_OVERRUN = 0x04,      //!< Too many BYTE s were stuffed into the buffer.
    NO_BUFFER = 0x05,           //!< All the buffers are used
    INVALID_BUF = 0x06,         //!< Command passed a buffer id that was not in range
    BUF_NOT_IN_QUEUE = 0x07,    //!< Command passed a buffer id is not being used.
    ARBITRATION_LOST = 0x08,    //!< While the MCU was mastering a packet on the SMBus it lost arbitration.
    TIMEOUT = 0x0B,             //!< SMBus timed out.
    CHECKSUM_ERR = 0x0C,        //!< Operation encountered a checksum mismatch
    DATA_NACK = 0x0D,           //!< Still don't know what these mean?
    BUS_ERR = 0x0E,             //!< ?
    FAIL = 0x0F,                //!< Generic error
    BUSY = 0x10,                //!< ?
    R_EARLY_NACK = 0x11,        //!< ?
    INVALID_LCD_COL_OFF = 0x12, //!< The cursor on the LCD was set to a column that was out of range.
    INVALID_LCD_ROW_OFF = 0x13, //!< The cursor on the LCD was set to a row that was out of range.
    INVALID_CK410_SEL = 0x14,   //!< ?
    CMD_NOT_SUPPORTED = 0x15,   //!< This command is not supported
    MORE_DATA_AVAILABLE = 0x16, //!< Do the command again to get more data
} STATUS;

typedef struct {
  BYTE byte_count;
  STATUS status;
  BYTE data[31];
} BUFFER_RSLT;

typedef struct {
  UINTN                               Signature;
  EFI_HANDLE                          Handle;               // Handle for protocol this driver installs on
  EFI_PLATFORM_TYPE_PROTOCOL          PlatformType;         // Policy protocol this driver installs
} EFI_PLATFORM_DATA_DRIVER_PRIVATE;

typedef struct {
  UINTN                               Signature;
  EFI_HANDLE                          Handle;         // Handle for protocol this driver installs on
  EFI_IIO_UDS_PROTOCOL                IioUds;         // Policy protocol this driver installs
} EFI_IIO_UDS_DRIVER_PRIVATE;

#endif
