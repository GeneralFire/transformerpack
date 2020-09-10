/** @file NvdimmSmbusAccess.h
  Implementation of NVDIMM SMBus Interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
**/

#ifndef _JEDEC_SMB_ACCESS_H_
#define _JEDEC_SMB_ACCESS_H_

typedef enum _NVDIMM_SMBUS_TSOD_POLLING {
  NVDIMM_SMBUS_TSOD_POLLING_DISABLED = 0x00,
  NVDIMM_SMBUS_TSOD_POLLING_ENABLED,
  NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN = 0xFF
} NVDIMM_SMBUS_TSOD_POLLING;

typedef struct _NVDIMM_SMBUS_ACCESS_DATA {
  BOOLEAN AccessAcquired;
  UINT8   TsodPollingState;
  UINT8   OemTsodPollingState;
} NVDIMM_SMBUS_ACCESS_DATA;

/**
  TSOD Polling settings

  @param[in]     Socket          - Socket index at system level
  @param[in]     Imc             - Imc index at socket level
  @param[in]     PollState      - Enable TSOD polling.(0x00-disable, 0x01-Enable)
  @param[out]    SaveState      - Preserve polling state if provided.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbusSetPollingState (
  IN     UINT8           Socket,
  IN     UINT8           Imc,
  IN     UINT8           PollState,
  OUT    UINT8  * CONST  SaveState
  );


/**
  TSOD polling state retrieving.

  @param[in]     Socket          - Socket index at system level
  @param[in]     Imc             - Imc index at socket level
  @param[in]     PollingState    - TSOD polling state (0x00-disable, 0x01-Enable)

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbuslGetPollingState (
  IN     UINT8  Socket,
  IN     UINT8  Imc,
  OUT    UINT8  *PollState
  );

/**
  @brief Return maximum amount of time allowed to use the SMBus

  This function returns maximum amount of time allowed for UEFI MGMT SW to use the SMBus
  and can be overwritten by OEM hook. Note, that zero is special value and means no limit.

  @param[out] TimeMs           Maximum amount of time in ms allowed to use the SMBus.

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER TimeMs is NULL.
**/
EFI_STATUS
NvdimmSmbusGetMaxAccessTime (
  OUT UINT32 *TimeMs
  );


/**
  @brief Provides interface to acquire SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
NvdimmSmbusAccessAcquire (
  IN UINT8 Socket,
  IN UINT8 Imc
  );

/**
  @brief Provides interface to release SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
NvdimmSmbusAccessRelease (
  IN UINT8 Socket,
  IN UINT8 Imc
  );

/**
  @brief Initializes NVDIMM SMBus access
  (installs SMM handler for NVDIMM SMBus related actions)

  @param None

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbusAccessSmmInit (
  VOID
  );

#endif // _JEDEC_SMB_ACCESS_H_
