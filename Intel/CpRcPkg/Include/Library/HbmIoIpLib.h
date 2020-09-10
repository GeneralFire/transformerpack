/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _HBM_IO_IP_LIB_H_
#define _HBM_IO_IP_LIB_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Memory/HbmJedec.h>

#define MBOX_PARAMETER  UINT32

//Define opcodes
typedef enum {
  HBM_MBOX_INITIALIZE_OPCODE           = 0x00,
  HBM_MBOX_DLL_CTRL_OPCODE             = 0x01,
  HBM_MBOX_RESET_WRST_N_CTRL_OPCODE    = 0x02,
  HBM_MBOX_HBM_PHY_CONFIG_OPCODE       = 0x03,
  HBM_MBOX_DA28_LOCK_SECURITY_OPCODE   = 0x04,
  HBM_MBOX_PHY_INIT_COMPLETE_OPCODE    = 0x05,
  HBM_MBOX_IEEE_1500_READ_DEVID_OPCODE = 0x09,
  HBM_MBOX_IEEE_1500_GENERIC_OPCODE    = 0x0A,
  HBM_MBOX_LANE_REPAIR_LOAD_OPCODE     = 0x0B,
  HBM_MBOX_TRAIN_XOVER_OPCODE          = 0x0C,
  HBM_MBOX_TRAIN_AWORD_LFSR_OPCODE     = 0x0D,
  HBM_MBOX_TRAIN_DWORD_TX_LFSR_OPCODE  = 0x0E,
  HBM_MBOX_TRAIN_DWORD_RX_LFSR_OPCODE  = 0x0F,
  HBM_MBOX_TRAIN_ACIO_LB_OPCODE        = 0x10,
  HBM_MBOX_TRAIN_DRIVE_STRENTH_OPCODE  = 0x11,
  HBM_MBOX_TRAIN_AWORD_ADVANCE_OPCODE  = 0x12,
  HBM_MBOX_TRAIN_DWORD_ADVANCE_OPCODE  = 0x13,
  HBM_MBOX_FULL_PROD_TRAINING_OPCODE   = 0x14,
  HBM_MBOX_LANE_REPAIR_AWORD_OPCODE    = 0x15,
  HBM_MBOX_LANE_REPAIR_DWORD_OPCODE    = 0x16,
  HBM_MBOX_FULL_HVM_SORT_OPCODE        = 0x1A,
  HBM_MBOX_FULL_HVM_CLASS_OPCODE       = 0x1B,
  HBM_MBOX_TRAIN_PARAM_SET_VAL_OPCODE  = 0x20,

  HBM_MBOX_MAX_OPCODE
} HBMIO_MBX_OPCODES;

// ********************************************************************************************************************
//                                                                                                                    *
// Public HBM IP Lib interfaces                                                                                         *
//                                                                                                                    *
// ********************************************************************************************************************

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
  );

EFI_STATUS
EFIAPI
HbmioExecuteMbox (
  IN UINT8                Socket,
  IN UINT8                HbmIoId,
  IN HBMIO_MBX_OPCODES    HbmMbOpcode,
  IN MBOX_PARAMETER       HbmMbParam,
  IN OUT UINT8            *Data,
  IN OUT UINT32           DataLength
  );

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
  );

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
  );
#endif // _HBM_IO_IP_LIB_H_
