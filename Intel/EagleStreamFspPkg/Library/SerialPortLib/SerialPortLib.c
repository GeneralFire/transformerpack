/** @file
  16550 UART Serial Port library functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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

#include <Base.h>
#include <IndustryStandard/Pci.h>
#include <Library/SerialPortLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>

#include <Library/FspCommonLib.h>
#include <FspmUpd.h>

//
// 16550 UART register offsets and bitfields
//
#define R_UART_RXBUF          0
#define R_UART_TXBUF          0
#define R_UART_BAUD_LOW       0
#define R_UART_BAUD_HIGH      1
#define R_UART_FCR            2
#define   B_UART_FCR_FIFOE    BIT0
#define   B_UART_FCR_FIFO64   BIT5
#define R_UART_LCR            3
#define   B_UART_LCR_DLAB     BIT7
#define R_UART_MCR            4
#define   B_UART_MCR_RTS      BIT1
#define R_UART_LSR            5
#define   B_UART_LSR_RXRDY    BIT0
#define   B_UART_LSR_TXRDY    BIT5
#define   B_UART_LSR_TEMT     BIT6
#define R_UART_MSR            6
#define   B_UART_MSR_CTS      BIT4
#define   B_UART_MSR_DSR      BIT5

//
// 16550 UART Device Number
//
#define HSUART_DEV     0x1A

/**
  Read an 8-bit 16550 register.  If UseMmio is TRUE, then the value is read from
  MMIO space.  If UseMmio is FALSE, then the value is read from I/O space.  The
  parameter Offset is added to the base address of the 16550 registers.

  @param  Base     The base address register of UART device.
  @param  Offset   The offset of the 16550 register to read.
  @param  UseMmio  Indicates serial port registers are in MMIO space, or in I/O space

  @return The value read from the 16550 register.

**/
UINT8
SerialPortReadRegister (
  UINTN      Base,
  UINTN      Offset,
  BOOLEAN    UseMmio
  )
{
  if (UseMmio) {
    return MmioRead8 (Base + Offset);
  } else {
    return IoRead8 (Base + Offset);
  }
}

/**
  Write an 8-bit 16550 register.  If UseMmio is TRUE, then the value is written to
  MMIO space.  If UseMmio is FALSE, then the value is written to I/O space.  The
  parameter Offset is added to the base address of the 16550 registers.

  @param  Base     The base address register of UART device.
  @param  Offset   The offset of the 16550 register to write.
  @param  Value    The value to write to the 16550 register specified by Offset.
  @param  UseMmio  Indicates serial port registers are in MMIO space, or in I/O space

  @return The value written to the 16550 register.

**/
UINT8
SerialPortWriteRegister (
  UINTN      Base,
  UINTN      Offset,
  UINT8      Value,
  BOOLEAN    UseMmio
  )
{
  if (UseMmio) {
    return MmioWrite8 (Base + Offset, Value);
  } else {
    return IoWrite8 (Base + Offset, Value);
  }
}

/**
  Update the value of an 16-bit PCI configuration register in a PCI device.  If the
  PCI Configuration register specified by PciAddress is already programmed with a
  non-zero value, then return the current value.  Otherwise update the PCI configuration
  register specified by PciAddress with the value specified by Value and return the
  value programmed into the PCI configuration register.  All values must be masked
  using the bitmask specified by Mask.

  @param  PciAddress  PCI Library address of the PCI Configuration register to update.
  @param  Value       The value to program into the PCI Configuration Register.
  @param  Mask        Bitmask of the bits to check and update in the PCI configuration register.

**/
UINT16
SerialPortLibUpdatePciRegister16 (
  UINTN   PciAddress,
  UINT16  Value,
  UINT16  Mask
  )
{
  UINT16  CurrentValue;

  CurrentValue = PciRead16 (PciAddress) & Mask;
  if (CurrentValue != 0) {
    return CurrentValue;
  }
  return PciWrite16 (PciAddress, Value & Mask);
}

/**
  Update the value of an 32-bit PCI configuration register in a PCI device.  If the
  PCI Configuration register specified by PciAddress is already programmed with a
  non-zero value, then return the current value.  Otherwise update the PCI configuration
  register specified by PciAddress with the value specified by Value and return the
  value programmed into the PCI configuration register.  All values must be masked
  using the bitmask specified by Mask.

  @param  PciAddress  PCI Library address of the PCI Configuration register to update.
  @param  Value       The value to program into the PCI Configuration Register.
  @param  Mask        Bitmask of the bits to check and update in the PCI configuration register.

  @return  The current value in the register or the value written to the register.

**/
UINT32
SerialPortLibUpdatePciRegister32 (
  UINTN   PciAddress,
  UINT32  Value,
  UINT32  Mask
  )
{
  UINT32  CurrentValue;

  CurrentValue = PciRead32 (PciAddress) & Mask;
  if (CurrentValue != 0) {
    return CurrentValue;
  }
  return PciWrite32 (PciAddress, Value & Mask);
}

/**
  Retrieve the I/O or MMIO base address register for the PCI UART device.

  This function assumes Root Bus Number is Zero and HSUART_DEV.
  The PCI device function number is taken from the UPD value PcdHsuartSerialPort.
  This function will also determine if serial port registers are in MMIO space, or in I/O space,
  and update parameter UseMmio.
  If MMIO or IO BAR is not found this function will return zero.

  @param   UseMmio  Indicates serial port registers are in MMIO space, or in I/O space

  @return  The base address register of the UART device.

**/
UINTN
GetSerialRegisterBase (
  BOOLEAN     *UseMmio
  )
{
  *UseMmio = FALSE;
  // wa: for server onboard serail port. I/O: 3F8.
  // to refine next version.
  return 0x3F8;
}

/**
  Initialize the serial device hardware.

  FSP will not initialize the Serial Device, this must be done by the bootloader.

  @retval RETURN_SUCCESS        The serial device was initialized.

**/
RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  return RETURN_SUCCESS;
}

/**
  Write data from buffer to serial device.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.

  If Buffer is NULL, then return 0.

  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the write operation failed.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
  )
{
  UINTN      SerialRegisterBase;
  UINTN      Result;
  BOOLEAN    UseMmio;

  if (Buffer == NULL) {
    return 0;
  }

  SerialRegisterBase = GetSerialRegisterBase (&UseMmio);
  if (SerialRegisterBase == 0) {
    return 0;
  }

  if (NumberOfBytes == 0) {
    //
    // Wait for both the transmit FIFO and shift register empty.
    //
    while ((SerialPortReadRegister (SerialRegisterBase, R_UART_LSR, UseMmio) & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY));

    return 0;
  }

  Result = NumberOfBytes;
  while (NumberOfBytes--) {
    //
    // Wait for the serial port to be ready, to make sure both the transmit FIFO
    // and shift register empty.
    //
    while ((SerialPortReadRegister (SerialRegisterBase, R_UART_LSR, UseMmio) & B_UART_LSR_TEMT) == 0);
    //
    // Write byte to the transmit buffer.
    //
    SerialPortWriteRegister (SerialRegisterBase, R_UART_TXBUF, *Buffer++, UseMmio);
  }
  return Result;
}

/**
  Reads data from a serial device into a buffer.

  @param  Buffer           Pointer to the data buffer to store the data read from the serial device.
  @param  NumberOfBytes    Number of bytes to read from the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes read from the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.

**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
  )
{
  UINTN      SerialRegisterBase;
  UINTN      Result;
  BOOLEAN    UseMmio;

  if (NULL == Buffer) {
    return 0;
  }

  SerialRegisterBase = GetSerialRegisterBase (&UseMmio);
  if (SerialRegisterBase == 0) {
    return 0;
  }

  for (Result = 0; NumberOfBytes-- != 0; Result++, Buffer++) {
    //
    // Wait for the serial port to have some data.
    //
    while ((SerialPortReadRegister (SerialRegisterBase, R_UART_LSR, UseMmio) & B_UART_LSR_RXRDY) == 0);

    //
    // Read byte from the receive buffer.
    //
    *Buffer = SerialPortReadRegister (SerialRegisterBase, R_UART_RXBUF, UseMmio);
  }

  return Result;
}


/**
  Polls a serial device to see if there is any data waiting to be read.

  Polls a serial device to see if there is any data waiting to be read.
  If there is data waiting to be read from the serial device, then TRUE is returned.
  If there is no data waiting to be read from the serial device, then FALSE is returned.

  @retval TRUE             Data is waiting to be read from the serial device.
  @retval FALSE            There is no data waiting to be read from the serial device.

**/
BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  UINTN      SerialRegisterBase;
  BOOLEAN    UseMmio;

  SerialRegisterBase = GetSerialRegisterBase (&UseMmio);
  if (SerialRegisterBase == 0) {
    return FALSE;
  }

  //
  // Read the serial port status
  //
  if ((SerialPortReadRegister (SerialRegisterBase, R_UART_LSR, UseMmio) & B_UART_LSR_RXRDY) != 0) {
    return TRUE;
  }

  return FALSE;
}
