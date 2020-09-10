/** @file
  Source code file for Gfsp HBMIO Training for SPR HBM

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials, and
  your use of them is governed by the express license under which they were provided
  to you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related documents
  without Intel's prior written permission.

  This software and the related documents are provided as is, with no express or implied
  warranties, other than those that are expressly stated in the License.

  @par Specification
**/
#include "Library/HbmIoIpLib.h"

/**

  HbmIo Mailbox Initialize API

  @param[In]               Socket              Init the HbmIo instances for a specific socket
  @param[In]               DisableuC           Whether to disable the uC of HBM after init

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
HbmioMboxInit (
  UINT8   Socket,
  BOOLEAN DisableuC
  )

{
  return EFI_UNSUPPORTED;
}

/**
  This function reads the Device ID data by issuing ieee1500_read_device_id HBMIO mailbox command.

  @param[in]  SocketId                Socket index.
  @param[in]  HbmIoId                 HBM IO index on socket.
  @param[out] DeviceIdData            Pointer to the returned Device ID data

  @retval EFI_SUCCESS                 This function read the the Device ID data successfully.
  @retval EFI_NOT_READY               There is no Device ID data pending upon return.
  @retval EFI_INVALID_PARAMETER       Some of input parameters are invalid.
  @retval EFI_UNSUPPORTED             This operation is not supported.
**/
EFI_STATUS
EFIAPI
HbmioMailboxReadDeviceId (
  IN  UINT8           SocketId,
  IN  UINT8           HbmIoId,
  OUT DEVICE_ID_WDR   *DeviceIdData
  )
{
  return EFI_UNSUPPORTED;
}

/**
  HBMIO Mailbox Physical Initialization Start.

  @param[in] Socket       Socket Index

  @retval EFI_SUCCESS     This function was executed successfully.
  @retval !EFI_SUCCESS    This function failed to be executed.
**/
EFI_STATUS
EFIAPI
HbmioMboxPhyInitStart (
  UINT8 Socket
  )
{
  return EFI_UNSUPPORTED;
}
