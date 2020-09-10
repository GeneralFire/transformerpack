/** @file
  Serial IO UART Lib implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include "SerialIoUartPrivateLib.h"

/**
  Polls a serial device to see if there is any data waiting or pending for read.
  If there is data pending, then TRUE is returned.
  If there is no data, then FALSE is returned.

  @param[in]  UartNumber   Selects Serial IO UART device

  @retval TRUE             Data is waiting to read from the serial device.
  @retval FALSE            There is no data waiting to read from the serial device.
**/
BOOLEAN
EFIAPI
SerialIoUartPolling (
  IN  UINT8         UartNumber
  )
{
  UART_ACCESS_MODE     AccessMode;
  UINT64               BaseAddress;
  SERIAL_IO_UART_LSR   Lsr;

  if (!SerialIoUartEnable (UartNumber, &BaseAddress, &AccessMode)) {
    return FALSE;
  }

  //
  // Read the serial port status
  //
  Lsr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_PCH_SERIAL_IO_8BIT_UART_LSR);
  return (BOOLEAN) Lsr.Fields.DR;
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  UartNumber       Selects Serial IO UART device
  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
SerialIoUartWrite (
  IN UINT8            UartNumber,
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  BOOLEAN           UseTimeout;
  UINT32            Index;
  UINTN             Timeout;
  UART_ACCESS_MODE  AccessMode;
  UINT64            BaseAddress;

  if (!SerialIoUartEnable (UartNumber, &BaseAddress, &AccessMode)) {
    return 0;
  }
  if ((NULL == Buffer) || (SerialIoUartReadRegister (AccessMode, BaseAddress, R_PCH_SERIAL_IO_8BIT_UART_USR) == 0xFF)) {
    return 0;
  }

  UseTimeout = TRUE;

  //
  // If Timeout is equal to 0, then timeout is disabled
  //
  Timeout = PcdGet32 (PcdSerialIoUartTimeOut);
  if (Timeout == 0) {
    UseTimeout = FALSE;
  }
  for (Index = 0; Index < NumberOfBytes; Index++) {
    while (!SerialIoUartTransmit (AccessMode, BaseAddress, Buffer[Index])) {
      if (UseTimeout == TRUE) {
        //
        // Unsuccessful write so check if timeout has expired, if not,
        // stall for a bit, increment time elapsed, and try again
        //
        if (Timeout == 0) {
          // If Auto Flow is already disabled, and timeout was reached return
          if (SerialIoUartGetAutoFlow (UartNumber) == FALSE) {
            return 0;
          }
          //
          // Disable AutoFlow Control if data did not come out of the FIFO in given time
          //
          SerialIoUartSetAutoFlow (UartNumber, FALSE);
          Timeout = PcdGet32 (PcdSerialIoUartTimeOut);
        }
        MicroSecondDelay (SERIALIO_UART_TIMEOUT_DELAY_INTERVAL);
        Timeout -= SERIALIO_UART_TIMEOUT_DELAY_INTERVAL;
      }
    }
  }

  return NumberOfBytes;
}

/*
  Read data from serial device and save the data in buffer.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is 0, then return 0.

  @param[in]   UartNumber           Selects Serial IO UART device
  @param[out]  Buffer               Pointer to the buffer that will contain the return data
  @param[in]   NumberOfBytes        Number of input bytes to read
  @param[in]   WaitUntilBufferFull  TRUE  - function waits until whole buffer is filled.
                                    FALSE - function returns as soon as no new characters are available.

  @retval                           Actual number of bytes raed to serial device.
**/
UINTN
EFIAPI
SerialIoUartRead (
  IN  UINT8            UartNumber,
  OUT UINT8            *Buffer,
  IN  UINTN            NumberOfBytes,
  IN  BOOLEAN          WaitUntilBufferFull
  )
{
  UINTN                BytesReceived;
  UINT64               BaseAddress;
  UART_ACCESS_MODE     AccessMode;
  SERIAL_IO_UART_LSR   Lsr;
  UINT8                Byte;

  if (!SerialIoUartEnable (UartNumber, &BaseAddress, &AccessMode)) {
    return 0;
  }
  if ((NULL == Buffer) || (SerialIoUartReadRegister (AccessMode, BaseAddress, R_PCH_SERIAL_IO_8BIT_UART_USR) == 0xFF)) {
    return 0;
  }

  BytesReceived = 0;

  while (BytesReceived != NumberOfBytes) {
    Lsr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_PCH_SERIAL_IO_8BIT_UART_LSR);
    //
    // If there is data in the RX buffer, read it.
    //
    if (Lsr.Fields.DR != 0) {
      Byte = SerialIoUartReadRegister (AccessMode, BaseAddress, R_PCH_SERIAL_IO_8BIT_UART_RXBUF);
      //
      // Check if the break interrupt bit is set. If set, the byte read from the
      // RX buffer is invalid and should be ignored. If not set, copy the byte into
      // the receive buffer.
      //
      if (Lsr.Fields.BI == 0) {
        *Buffer = Byte;
        Buffer++;
        BytesReceived++;
      }
    } else {
      if (!WaitUntilBufferFull) {
        //
        // If there's no data and function shouldn't wait, exit early
        //
        return BytesReceived;
      }
    }
  }
  return BytesReceived;
}

/**
  Sets the baud rate, parity, data bits, flow control and stop bits on a serial device.

  @param[in]  UartNumber   Selects Serial IO UART device
  @param[in]  BaudRate     The requested baud rate. A BaudRate value of 0 will use the the
                           device's default interface speed.
  @param[in]  Parity       The type of parity to use on this serial device. A Parity value of
                           DefaultParity will use the device's default parity value.
  @param[in]  DataBits     The number of data bits to use on the serial device. A DataBits
                           value of 0 will use the device's default data bit setting.
  @param[in]  StopBits     The number of stop bits to use on this serial device. A StopBits
                           value of DefaultStopBits will use the device's default number of
                           stop bits.
  @param[in]  AutoFlow     Enable/Disable Auto Flow Control
**/
VOID
SerialIoUartSetAttributes (
  IN UINT8               UartNumber,
  IN UINT64              BaudRate,
  IN EFI_PARITY_TYPE     Parity,
  IN UINT8               DataBits,
  IN EFI_STOP_BITS_TYPE  StopBits,
  IN BOOLEAN             AutoFlow
  )
{
  SerialIoUartFifoEnable  (UartNumber);
  SerialIoUartSetBaudRate (UartNumber, BaudRate);
  SerialIoUartSetParity   (UartNumber, Parity);
  SerialIoUartSetDataBits (UartNumber, DataBits);
  SerialIoUartSetStopBits (UartNumber, StopBits);
  SerialIoUartSetAutoFlow (UartNumber, AutoFlow);
}

/**
  Dummy Read required after setting/clearing 16550 8bit mode

  @param[in] UartNumber         UART Number
**/
VOID
SerialIoUartDummyRead (
  IN UINT8        UartNumber
  )
{
  UINT64  BaseAddress;
  UINT64  PciCfgBase;

  PciCfgBase  = GetSerialIoUartPciCfg (UartNumber);
  BaseAddress = GetSerialIoBar (PciCfgBase);

  MmioRead8 ((UINTN) BaseAddress + R_PCH_SERIAL_IO_32BIT_UART_CTR);
}

/**
  Gets UART out of reset

  @param[in]  BaseAddress    Base address of UART MMIO space
**/
VOID
SerialIoUartGetOutOfReset (
  IN UINT64            MmioBaseAddress
  )
{
  UINT16    ResetOffset;

  ResetOffset = R_SERIAL_IO_MEM_PPR_RESETS;

  if (MmioRead32 ((UINTN) (MmioBaseAddress + R_PCH_SERIAL_IO_32BIT_UART_CTR)) != UART_COMPONENT_IDENTIFICATION_CODE) {
    ResetOffset = R_SERIAL_IO_MEM_PPR_RESETS_8BIT;
  }
  MmioOr8 ((UINTN) (MmioBaseAddress + ResetOffset),
               B_SERIAL_IO_MEM_PPR_RESETS_FUNC | B_SERIAL_IO_MEM_PPR_RESETS_APB | B_SERIAL_IO_MEM_PPR_RESETS_IDMA);
}

/**
  Return whether UART is in PCI mode or not

  @param[in] UartNumber         UART Number

  @retval                       TRUE  - UART is in PCI mode
  @retval                       FALSE - UART is in Hidden mode or disabled
**/
BOOLEAN
IsSerialIoUartInPciMode (
  IN UINT8        UartNumber
  )
{
  UINT64  PciCfgBase;

  PciCfgBase = SerialIoUartPciCfgBase (UartNumber);
  if (PciSegmentRead16 (PciCfgBase) != MAX_UINT16) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Assign MMIO Resource in PCI Mode.

  @param[in] UartNumber         UART Number
  @param[in] MmioBaseAddress    Base address of UART MMIO space

  @retval EFI_UNSUPPORTED       SerialIo UART isn't in PCI mode.
  @retval EFI_SUCCESS           Finish to set SerialIo UART MMIO in PCI mode.

**/
EFI_STATUS
SerialIoUartSetMmioInPciMode (
  IN UINT8        UartNumber,
  IN UINT64       MmioBaseAddress
  )
{
  UINT64  PciCfgBase;

  if (!IsSerialIoUartInPciMode (UartNumber)) {
    return EFI_UNSUPPORTED;
  }

  PciCfgBase  = GetSerialIoUartPciCfg (UartNumber);

  SerialIoSetD3 (PciCfgBase);
  SerialIoDisableMse (PciCfgBase);

  //
  // Assign MMIO Base Address
  //
  PciSegmentWrite32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET, MmioBaseAddress & MAX_UINT32);
  PciSegmentWrite32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + 4, RShiftU64 (MmioBaseAddress, 32) & MAX_UINT32);

  SerialIoSetD0 (PciCfgBase);
  SerialIoEnableMse (PciCfgBase, FALSE);

  return EFI_SUCCESS;
}