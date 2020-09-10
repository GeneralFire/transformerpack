/** @file NvdimmSmbusAccess.c
  Implementation of NVDIMM SMBus interface.

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

#include "JedecNvdimm.h"
#include "NvdimmSmbusAccess.h"
#include <Library/ProcSmbIpLib.h>
#include <CpuPciAccess.h>
#include <Library/IoAccessLib.h>
#include <Protocol/NvdimmSmbusSmmProtocol.h>

#define NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI  0xFE
#define NVDIMM_SMBUS_ACCESS_MS_TO_US(DelayMs)     ((DelayMs) * 1000)

NVDIMM_SMBUS_ACCESS_DATA mSmbusAccessData[MAX_SOCKET][MAX_IMC];


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
  )
{
  if (NULL == PollState) {
    return EFI_INVALID_PARAMETER;
  }

  if (Socket >= MAX_SOCKET || Imc >= MAX_IMC) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Imc]) {
    DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "Socket (%d) iMC (%d) - iMC not enabled\n", Socket, Imc));
    return EFI_INVALID_PARAMETER;
  }

  *PollState = (UINT8) GetTsodPollEnSmb(Socket, GetProcSmbInstanceFromMcId (Imc));
  DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "Socket (%d) iMC (%d) - TSOD Polling %a\n", Socket, Imc, ((*PollState) ? "enabled" : "disabled")));
  return EFI_SUCCESS;

}

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
  )
{
  UINT32 CurrentPollState;

  if (Socket >= MAX_SOCKET || Imc >= MAX_IMC) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Imc]) {
    DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "Socket (%d) iMC (%d) - iMC not enabled\n", Socket, Imc));
    return EFI_INVALID_PARAMETER;
  }

  if (mInSmm) {
    CurrentPollState = GetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (Imc));

    DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "Socket (%d) iMC (%d) - TSOD Polling state change from '%a' to '%a'\n",
        Socket, Imc, (CurrentPollState ? "enabled" : "disabled"), (PollState ? "enabled" : "disabled")));

    if (SaveState != NULL) {
      *SaveState = (UINT8)CurrentPollState;
    }

    SetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (Imc), PollState);
  } else {
    //
    // Update type/parameters/data in mNvdimmSmbusSmmInterfaceProtocol to access in SMM
    //
    if (mNvdimmSmbusSmmInterface == NULL) {
      //
      // NVDIMM SMBus DXE <-> SMM interface should already allocated at CR DXE driver entry
      //
      ASSERT (FALSE);
      return EFI_LOAD_ERROR;
    }

    mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Socket = Socket;
    mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Imc = Imc;
    mNvdimmSmbusSmmInterface->RequestType = SET_TSOD_POLLING_STATE;
    mNvdimmSmbusSmmInterface->RequestData.SetTsodPollState.PollState = PollState;

    //
    // Generating SW SMI for TSOD register value change
    //
    DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "Socket (%d) iMC (%d) - Generating SW SMI for TSOD polling state change\n", Socket, Imc));
    IoWrite8 (0xB2, (UINT8)NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI);

    //
    // SMI handler executed, retrieve operation status
    //
    if (EFI_ERROR (mNvdimmSmbusSmmInterface->RequestStatus)) {
      DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "Socket (%d) iMC (%d) - TSOD polling state change failed with status %r\n", Socket, Imc, mNvdimmSmbusSmmInterface->RequestStatus));
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

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
  )
{
//   EFI_STATUS Status;

  if (NULL == TimeMs) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check Platform Policy for TSOD polling state
  //
  if (mSystemMemoryMap->MemSetup.thermalThrottlingOptions & CLTT_EN) {
    *TimeMs = mSystemMemoryMap->MemSetup.NvdimmSmbusMaxAccessTime;
  } else {
    //
    // TSOD polling is disabled, no time restrictions when accessing the SMBus
    //
    *TimeMs = 0;
  }

  if (*TimeMs == 0) {
    DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "SMBus max allowed access time: no time restrictions when accessing the SMBus\n"));
  } else {
    DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "SMBus max allowed access time: %d ms\n", *TimeMs));
  }

  return EFI_SUCCESS;

}

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
  )
{
  EFI_STATUS Status;

  if (Socket >= MAX_SOCKET || Imc >= MAX_IMC) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSmbusAccessData[Socket][Imc].AccessAcquired) {
    mSmbusAccessData[Socket][Imc].AccessAcquired = TRUE;
  } else {
    DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "SMBus access in progress (socket: %d, iMC %d)\n", Socket, Imc));
    return EFI_ALREADY_STARTED;
  }

  Status = EFI_SUCCESS;
  mSmbusAccessData[Socket][Imc].OemTsodPollingState = NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN;
  if (mSmbusAccessData[Socket][Imc].OemTsodPollingState == NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN) {
    Status = NvdimmSmbuslGetPollingState (Socket, Imc, &mSmbusAccessData[Socket][Imc].TsodPollingState);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") " SMBus access not acquired (socket: %d, iMC %d) - retrieving TSOD state failed\n", Socket, Imc));
    }
  } else {
    //
    // Do not need to get the TSOD Polling State just use the OEM TSOD Polling state
    //
    mSmbusAccessData[Socket][Imc].TsodPollingState = mSmbusAccessData[Socket][Imc].OemTsodPollingState;
  }

  //
  // Disable TSOD polling (if enabled and no errors so far) and save initial state
  //
  if (!EFI_ERROR (Status) && mSmbusAccessData[Socket][Imc].TsodPollingState == NVDIMM_SMBUS_TSOD_POLLING_ENABLED) {
    Status = NvdimmSmbusSetPollingState (Socket, Imc, NVDIMM_SMBUS_TSOD_POLLING_DISABLED, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "SMBus access not acquired (socket: %d, iMC %d) - TSOD state change failed\n", Socket, Imc));
    }
  }

  //
  // Check for error (we need to release OEM SMBus control in case of non-successful SMBus access acquisition)
  //
  if (EFI_ERROR (Status)) {
    mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
    mSmbusAccessData[Socket][Imc].TsodPollingState = NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN;
    return EFI_DEVICE_ERROR;
  }

  //
  // SMBus access acquisition completed successfully
  //
  DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "SMBus access acquired (socket: %d, iMC %d)\n", Socket, Imc));
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS Status;

  if (Socket >= MAX_SOCKET || Imc >= MAX_IMC) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSmbusAccessData[Socket][Imc].AccessAcquired) {
    DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "SMBus access not acquired - nothing to release (socket: %d, iMC %d)\n", Socket, Imc));
    return EFI_NOT_STARTED;
  }

  //
  // Restore the initial TSOD state if needed
  //
  if (mSmbusAccessData[Socket][Imc].TsodPollingState == NVDIMM_SMBUS_TSOD_POLLING_ENABLED) {
    Status = NvdimmSmbusSetPollingState (Socket, Imc, NVDIMM_SMBUS_TSOD_POLLING_ENABLED, NULL);
    if (EFI_ERROR (Status)) {
      mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
      DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "SMBus access released with errors (socket: %d, iMC %d)\n", Socket, Imc));
      return EFI_DEVICE_ERROR;
    }
    MicroSecondDelay (NVDIMM_SMBUS_ACCESS_MS_TO_US (mSystemMemoryMap->MemSetup.NvdimmSmbusReleaseDelay));
  }

  mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
  DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "SMBus access released (socket: %d, iMC %d)\n", Socket, Imc));
  return EFI_SUCCESS;
}

/**
  @brief SMM handler to handle NVDIMM SMBus access related actions.

  @param[in]  DispatchHandle       - EFI handler
  @param[in]  DispatchContext      - context from the caller
  @param[in]  CommBuffer           - buffer from caller - optional
  @param[out] CommBufferSize       - size of the buffer - optional

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbusAccessSmmActionsHandler (
   IN       EFI_HANDLE                   DispatchHandle,
   IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
   IN OUT   VOID                         *CommBuffer,     OPTIONAL
   IN OUT   UINTN                        *CommBufferSize  OPTIONAL
   )
{
  switch (mNvdimmSmbusSmmInterface->RequestType) {
    case SET_TSOD_POLLING_STATE:
      DEBUG ((DEBUG_INFO, NVDIMM_INFO_STR ("SMB") "SMM TSOD change polling state (socket %d, iMC %d)\n",
          mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Socket, mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Imc));

      mNvdimmSmbusSmmInterface->RequestStatus = NvdimmSmbusSetPollingState (
        mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Socket,
        mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Imc,
        mNvdimmSmbusSmmInterface->RequestData.SetTsodPollState.PollState,
        NULL);
      break;

    default:
      DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "Unknown SMM request type (type %d)\n", mNvdimmSmbusSmmInterface->RequestType));
      mNvdimmSmbusSmmInterface->RequestStatus = EFI_INVALID_PARAMETER;
      break;
  }

  return EFI_SUCCESS;
}

/**
  @brief Initializes NVDIMM SMBus access
  (installs SMM handler for NVDIMM SMBus related actions)

  @param None

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbusAccessSmmInit (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch = 0;
  EFI_SMM_SW_REGISTER_CONTEXT   SwContext;
  EFI_HANDLE                    SwHandle;

  Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "Can't locate SW Dispatch protocol (status %r)\n", Status));
    return EFI_ABORTED;
  }

  SwContext.SwSmiInputValue = NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI;
  Status = SwDispatch->Register (SwDispatch, (EFI_SMM_HANDLER_ENTRY_POINT2)NvdimmSmbusAccessSmmActionsHandler, &SwContext, &SwHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, NVDIMM_ERROR_STR ("SMB") "SMM handler registration failed (status %r)\n", Status));
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
