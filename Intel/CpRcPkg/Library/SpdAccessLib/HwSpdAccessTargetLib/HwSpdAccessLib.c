/** @file
  HwSpdAccessLib.c

  Hw Target Lib implementation for SpdAccessLib.  This hardware specific implementation
  of the SpdAccessTargetLib provides functions for use in a hardware environment for 10nm.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#include <Ppi/MemoryPolicyPpi.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/SpdAccessLib.h>

/**
  Initializes the agents which are required before SPD transactions can be
  carried out.

  @param[in]  - Socket ID

  @retval EFI_SUCCESS       - successfully initialized SPD agents
  @retval EFI_DEVICE_ERROR  - error during initialization
**/
EFI_STATUS
EFIAPI
InitSpdAgents (
  IN UINT8  Socket
  )
{
  EFI_STATUS  Status;

  // Initialize the Process SMBus
  Status = InitProcSmb (Socket);

  return Status;
}

/**
  Read from a DIMM SPD at the specified location.

  @param[in] Socket   - Processor socket ID
  @param[in] SpdPage  - Page offset of SPD to be used
  @param[in] DdrType  - Type of DDR Technology supported
  @param[in] SpdDev   - SMBUS Device with SPD data
  @param[in] Offset   - byte offset within Device
  @param[out] Data    - pointer to store value

  @retval EFI_SUCCESS       - Read was successful
  @retval EFI_DEVICE_ERROR  - Error reading from device
**/
EFI_STATUS
EFIAPI
ReadSpdTarget (
  IN UINT8                Socket,
  IN UINT8                SpdPage,
  IN DDR_TECHNOLOGY_TYPE  DdrType,
  IN SMB_DEVICE_STRUCT    SpdDev,
  IN UINT8                Offset,
  OUT UINT8               *Data
  )
{
  EFI_STATUS  Status;

  Status = ReadProcSmb (SpdDev, Offset, Data);

  return Status;
}

/**
  Write a DIMM SPD at the specified location.

  @param[in] Socket   - Processor socket ID
  @param[in] SpdPage  - Page offset of SPD to be used
  @param[in] DdrType  - Type of DDR Technology supported
  @param[in] SpdDev   - SMBUS Device with SPD data
  @param[in] Offset   - byte offset within Device
  @param[in] Data     - pointer to data to write

  @retval EFI_SUCCESS       - Read was successful
  @retval EFI_DEVICE_ERROR  - Error writing into device
**/
EFI_STATUS
EFIAPI
WriteSpdTarget (
  IN UINT8                Socket,
  IN UINT8                SpdPage,
  IN DDR_TECHNOLOGY_TYPE  DdrType,
  IN SMB_DEVICE_STRUCT    SpdDev,
  IN UINT8                Offset,
  IN UINT8                *Data
  )
{
  EFI_STATUS  Status;

  Status = WriteProcSmb (SpdDev, Offset, Data);

  return Status;
}
