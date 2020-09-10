/** @file
  The Proc SMB Chip Common header file provides common function definitions
  for the PEI, DXE, and SMM phases of the implmentation of ProcSmbIpLib for Spr.

  This common file provides functions which are universal between PEI, DXE, and
  SMM.  The implementations for disabling/restoring TSOD and are common between
  all phases.  The read/write I3C routines are common between DXE and SMM.
  PEI has its own implementation of these functions.  The common code is
  implemented here to mitigate replicated code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _PROC_SMB_CHIP_COMMON_SPR_H_
#define _PROC_SMB_CHIP_COMMON_SPR_H_

/**
  I3C has queues for transactions, and do not need to wait for Host not busy.
  But need Wait for comand queue ready based on target build.
  Command Queue Ready Status field reflects interrupt status
  that is populated upon number of entries in command queue
  is smaller or equal to threshold specified by
  CMD_EMPTY_BUF_THLD in QUEUE_THLD_CTRL
  register. This field is automatically cleared once number of
  entries is greater than the threshold value.

  @param[in] Socket       - Socket number
  @param[in] Instance     - I3C Instance number
  @param[in] I3cStatusReg - Status register to check

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.
**/
EFI_STATUS
EFIAPI
WaitForHostNotBusyTargetCommon (
  IN UINT8   Socket,
  IN UINT8   Instance,
  IN UINT32  I3cStatusReg
  );

/**
  Reads from I3C at a given device and offset.

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - I3C device object
  @param[in] ByteOffset  - Byte offset within device
  @param[out] Data       - Pointer to buffer to store read value

  @retval EFI_SUCCESS       Read completed successfully
  @retval EFI_UNSUPPORTED   Function not supported
  @retval EFI_DEVICE_ERROR  Error on read
**/
EFI_STATUS
SmbReadCommon (
  IN  UINT8              Socket,
  IN  SMB_DEVICE_STRUCT  Dev,
  IN  UINT8              ByteOffset,
  OUT volatile UINT8     *Data
  );

/**
  Performs I3C write operations at a given device and offset.

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - I3C device object
  @param[in] ByteOffset  - Byte offset within device
  @param[in] Data        - Pointer to data to write

  @retval EFI_SUCCESS       - Write completed successfully
  @retval EFI_UNSUPPORTED   - Function not supported
  @retval EFI_DEVICE_ERROR  - Write failure
**/
EFI_STATUS
SmbWriteCommon (
  IN UINT8              Socket,
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  );

/**
  Returns I3C Instance for given McId, by mapping Instance to
  McId

  @param[in] McId  - Memory Controller Id, 0 based

  @retval UINT8  Instance Number
**/
UINT8
GetProcSmbInstanceFromMcIdCommon (
  IN UINT8  McId
  );

/**
  Returns the I3C strap address of the DIMM slot.

  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Dev     - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved I3C address data
**/
EFI_STATUS
GetSmbAddressCommon (
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *Dev
  );

/**
  Get slave device type - i2cni3c

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number

  @rval SlaveType   - slave device type

**/
UINT8
GetSlaveTypeCommon (
  IN UINT8   Socket,
  IN UINT8   Instance
  );

/**
  Get I3C mode and speed - mode_speed

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number

  @rval ModeSpeed   - I3C mode and speed

**/
I3C_BUS_MODE
GetModeSpeedCommon (
  IN UINT8  Socket,
  IN UINT8  Instance
  );

/**
  Set I3C bus operation mode

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number
  @param[in] BusMode    - I3C bus operation mode

  @rval EFI_SUCCESS     - Successfully set I3C bus operation mode
  @rval EFI_NOT_READY   - Cannot retrieve SPD Access data

**/
EFI_STATUS
EFIAPI
SetI3cBusModeCommon (
  IN  UINT8  Socket,
  IN  UINT8  Instance,
  IN  UINT8  BusMode
  );

#endif  // _PROC_SMB_CHIP_COMMON_SPR_H_

