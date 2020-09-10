/** @file

  SerialIo Uart Debug Fsp Library

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation.

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

#ifndef _FSP_SERIAL_IO_UART_DEBUG_PROPERTY_H_
#define _FSP_SERIAL_IO_UART_DEBUG_PROPERTY_H_

#include <SerialIoDevices.h>

/**
  Returns configuration data of Serial Io UART in Debug mode

  @param[in] FspInfoHeaderPointer  FSP Info Header
  @param[in/out] Uart              UART Device Configuration
  @param[in/out] UartNumber        UART Device Number
**/
VOID
FsptSerialIoUartDebugGetDeviceConfig (
  IN  UINT32                      FspInfoHeaderPointer,
  IN OUT  SERIAL_IO_UART_CONFIG   *Uart,
  IN OUT  UINT8                   *UartNumber
  );

/**
  Returns Serial Io UART Debug Enable

  @param[in] FspInfoHeader         FSP Info Header

  @retval  0 - Disabled
  @retval  1 - Enabled
  @retval  2 - Enabled without initializing
**/
UINT8
FsptSerialIoUartDebugGetDebugEnable (
  IN UINT32  FspInfoHeaderPointer
  );

/**
  Returns UART attributes

  @param[in/out] Attributes          UART Attributes
**/
VOID
FspmSerialIoUartDebugGetAttributes (
  IN OUT SERIAL_IO_UART_ATTRIBUTES  *Attributes
  );

/**
  Returns Serial Io UART Controller Number used in Debug mode

  @retval  ControllerNumber   UART Controller Number
**/
UINT8
FspmSerialIoUartDebugGetControllerNumber (
  VOID
  );

#endif// _FSP_SERIAL_IO_UART_DEBUG_PROPERTY_H_
