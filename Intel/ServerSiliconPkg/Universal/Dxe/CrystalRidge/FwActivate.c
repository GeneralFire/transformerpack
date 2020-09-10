/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include "CrystalRidge.h"
#include "Dsm.h"
#include "Pci.h"
#include "FwActivate.h"
#include <Library/CpuAndRevisionLib.h>

#define CR_FW_ACTIVATE_PROTOCOL_VERSION_USED    CR_FW_ACTIVATE_PROTOCOL_VER_1

CR_FW_ACTIVATE_PROTOCOL *mCrFwActivateProtocol;

UINT8             mFwActivationSupported;
CR_HANDLE         mCrFwActivateProtocolHandle;
CTO_DEVICE_ENTRY *mCtoDeviceArray;
UINT32            mCtoDeviceArraySize;
UINT64            mMaxIoQuiesceTime;


EFI_STATUS
LoadCrFwActivateProtocol (
  VOID
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "LoadCrFwActivateProtocol\n"));
  Status = mSmst->SmmLocateProtocol (&gCrFwActivateProtocolGuid, NULL, (VOID **) &mCrFwActivateProtocol);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "Cannot find gCrFwActivateProtocolGuid.\n"));
  } else {
    Status = mCrFwActivateProtocol->Initialize (CR_FW_ACTIVATE_PROTOCOL_VERSION_USED, &mCrFwActivateProtocolHandle);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Invalid Protocol Handle received.\n"));
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
LoadCrFwActivateProtocolHandler (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  )
{
  return LoadCrFwActivateProtocol ();
}


BOOLEAN
FwActivateInitialize (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT8                         Dimm;
  NVDIMM                        *DimmPtr;
  EFI_EVENT                     RegisterCrFwActivateEvent;
  FIS_GET_SECURITY_OPT_IN_RSP   FisGetSecurityOptInRsp;
  FIS_GET_SECURITY_OPT_IN_REQ   FisGetSecurityOptInReq;

  DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "FwActivateInitialize\n"));

  if (!IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "FW Activation supported on ICX only\n"));
    return FALSE;
  }

  Status = mSmst->SmmLocateProtocol (&gCrFwActivateProtocolGuid, NULL, (VOID **) &mCrFwActivateProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "Did not find gCrFwActivateProtocolGuid, registering a notification.\n"));
    Status = mSmst->SmmRegisterProtocolNotify (&gCrFwActivateProtocolGuid, LoadCrFwActivateProtocolHandler, &RegisterCrFwActivateEvent);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "Cannot register notification for gCrFwActivateProtocolGuid.\n"));
      return FALSE;
    }
  } else {
    LoadCrFwActivateProtocol();
  }

  for (Dimm = 0; Dimm < mCrInfo.NvdimmInfo.NumNvdimms; Dimm++) {
    DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Dimm];
    if (DimmPtr->SADSpaBase == SPA_NOT_MAPPED) {
      continue;
    }
    if (DimmPtr->FisVersion < FIS_2_03 || DimmPtr->FisVersion >= FIS_3_00) {
      DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "FisVersion: %d. FW activate unsupported.\n", DimmPtr->FisVersion));
      continue;
    }
    ZeroMem (&FisGetSecurityOptInReq, sizeof (FisGetSecurityOptInReq));
    FisGetSecurityOptInReq.Bits.OptInCode = FW_ACTIVATE_OPT_IN_CODE;
    Status = FisRequest ((UINT8)DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, (UINT32 *)&FisGetSecurityOptInReq, sizeof(FisGetSecurityOptInReq), (UINT32 *)&FisGetSecurityOptInRsp, GET_SECURITY_OPT_IN, 0, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Unable to get FW activate opt-in. Status: %d\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, Status));
      continue;
    }
    if (FisGetSecurityOptInRsp.Bits.OptInCode != FW_ACTIVATE_OPT_IN_CODE) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Invalid Opt-In code: %d\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, FisGetSecurityOptInRsp.Bits.OptInCode));
      continue;
    }
    DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "S%d.C%d.D%d: FW Activation: %d\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, FisGetSecurityOptInRsp.Bits.OptInValue));
    if (FisGetSecurityOptInRsp.Bits.OptInValue != FW_ACTIVATE_OPTED_IN) {
      continue;
    }
    DimmPtr->ActivationData.ActivationEnabled    = TRUE;
    DimmPtr->ActivationData.ActivationState      = CrActivationStateIdle;
    DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_NONE;
    mFwActivationSupported = 1;
  }
  DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "FW Activation: %d\n", mFwActivationSupported));
  return (mFwActivationSupported != 0);
}

UINT8
GetDimmState (
  IN UINT8 Socket,
  IN UINT8 Imc,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  CR_ACTIVATION_STATE    DimmState;
  EFI_STATUS             Status;

  Status = mCrFwActivateProtocol->GetDimmActivationState (mCrFwActivateProtocolHandle, Socket, Imc, Channel, Dimm, &DimmState, NULL, NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed GetDimmActivationState. EFI_STATUS = 0x%x.\n", Status));
  }

  return DimmState;
}

EFI_STATUS
GetPlatformFwActivationState (
  UINT8 *NumArmedDevicesPtr,
  UINT8 *NumArmedAndStagedDevicesPtr,
  UINT8 *ActivationStatePtr
  )
{
  NVDIMM *DimmPtr;
  UINT8 Index;
  UINT8 ImcCh;
  CR_ACTIVATION_STATE ActivationState;
  CR_PLATFORM_ACTIVATION_STATUS LastActivationStatus;
  UINT8 NumArmedDevices = 0;
  UINT8 NumArmedAndStagedDevices = 0;
  EFI_STATUS Status;
  FIS_GET_FIRMWARE_INFO_RSP FwInfo;

  Status = mCrFwActivateProtocol->GetPlatformActivationState (mCrFwActivateProtocolHandle, &ActivationState, &LastActivationStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed GetPlatformActivationState. EFI_STATUS = 0x%x.\n", Status));
    return Status;
  }

  for (Index = 0; Index < mCrInfo.NvdimmInfo.NumNvdimms; Index++) {
    DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Index];
    if (DimmPtr->SADSpaBase == SPA_NOT_MAPPED) {
      continue;
    }
    DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_SUCCESS;

    ImcCh = ConvertSktChToImcCh (DimmPtr->Ch);
    if (GetDimmState ((UINT8)DimmPtr->SocketId, (UINT8)DimmPtr->ImcId, ImcCh, DimmPtr->Dimm) == CrActivationStateArmed) {
      DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Dimm is armed.\n", (UINT8)DimmPtr->SocketId, (UINT8)DimmPtr->Ch, DimmPtr->Dimm));
      NumArmedDevices++;
    } else {
      DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Dimm is not armed.\n", (UINT8)DimmPtr->SocketId, (UINT8)DimmPtr->Ch, DimmPtr->Dimm));
      continue;
    }

    Status = DsmFisRequest ((UINT8)DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, NULL, 0, &FwInfo.DWord[0], GET_FIRMWARE_IMAGE, 0, NULL);
    if (EFI_ERROR (Status) || FwInfo.Bits.StgFwRev.ProductNo || FwInfo.Bits.StgFwRev.RevisionNo || FwInfo.Bits.StgFwRev.SecurityNo || FwInfo.Bits.StgFwRev.BuildNo) {
      NumArmedAndStagedDevices++;
    } else {
      DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_NO_NEW_STAGED_FIRMWARE;
    }

  }
  DEBUG ((DEBUG_ERROR, "GetPlatformFwActivationState: ActivationState: %d, NumArmedDevices: %d, NumArmedAndStagedDevices: %d\n", ActivationState, NumArmedDevices, NumArmedAndStagedDevices));
  if (NumArmedDevicesPtr) {
    *NumArmedDevicesPtr = NumArmedDevices;
  }
  if (NumArmedAndStagedDevicesPtr) {
    *NumArmedAndStagedDevicesPtr = NumArmedAndStagedDevices;
  }
  *ActivationStatePtr = ActivationState;

  return EFI_SUCCESS;
}


UINT64
GetMemoryQuiesceTimeRequired (
  VOID
  )
{
  UINT64      CurrentDimmQuiesceTime = 0;
  UINT64      MaxDimmQuiesceTime     = 0;
  NVDIMM      *DimmPtr;
  EFI_STATUS  Status;
  UINT8       ImcCh;
  UINT8       Dimm;
  FIS_GET_SECURITY_OPT_IN_RSP   FisGetSecurityOptInRsp;
  FIS_GET_SECURITY_OPT_IN_REQ   FisGetSecurityOptInReq;

  for (Dimm = 0; Dimm < mCrInfo.NvdimmInfo.NumNvdimms; Dimm++) {
    DimmPtr = &mCrInfo.NvdimmInfo.Nvdimms[Dimm];
    if (DimmPtr->SADSpaBase == SPA_NOT_MAPPED) {
      continue;
    }

    // check if dimm armed
    ImcCh = ConvertSktChToImcCh (DimmPtr->Ch);
    if (GetDimmState ((UINT8)DimmPtr->SocketId, (UINT8)DimmPtr->ImcId, ImcCh, DimmPtr->Dimm) == CrActivationStateArmed) {
      DEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Dimm is armed.\n", (UINT8)DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm));
    } else {
      DEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Dimm is not armed.\n", (UINT8)DimmPtr->SocketId,DimmPtr->Ch, DimmPtr->Dimm));
      continue;
    }

    ZeroMem (&FisGetSecurityOptInReq, sizeof (FisGetSecurityOptInReq));
    FisGetSecurityOptInReq.Bits.OptInCode = FW_ACTIVATE_OPT_IN_CODE;
    Status = FisRequest ((UINT8)DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, (UINT32 *)&FisGetSecurityOptInReq, sizeof(FisGetSecurityOptInReq), (UINT32 *)&FisGetSecurityOptInRsp, GET_SECURITY_OPT_IN, 0, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, CR_ERROR_STR ("CRP") "S%d.C%d.D%d: Unable to get FW activate opt-in. Status: %d\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm, Status));
      continue;
    }

    if (!(FisGetSecurityOptInRsp.Bits.OptInSpecificData[0] & 1)){
      DEBUG ((DEBUG_ERROR, CR_INFO_STR ("CRP") "S%d.C%d.D%d: Data/Traffic Quiesce NOT Required\n", DimmPtr->SocketId, DimmPtr->Ch, DimmPtr->Dimm));
      continue;
    }

    CurrentDimmQuiesceTime = (UINT64) ((FisGetSecurityOptInRsp.Bits.OptInSpecificData[0] >> 8) & 0xFFFF) * 1000;
    if (MaxDimmQuiesceTime < CurrentDimmQuiesceTime) {
      MaxDimmQuiesceTime = CurrentDimmQuiesceTime;
    }
  }

  DEBUG ((DEBUG_INFO, CR_INFO_STR ("CRP") "MaxDimmQuiesceTime: %lld\n", MaxDimmQuiesceTime));
  return MaxDimmQuiesceTime;
}

EFI_STATUS
DsmRootIntelGetDevicesFwRuntimeActivationInfo (
  VOID
  )
{
  ROOT_DSM_INTEL_FNOUT_GET_DEVICES_FW_RUNTIME_ACTIVATION_INFO *FnOutPtr;
  CR_ACTIVATION_STATE             PlatformFwActivationState;
  CR_PLATFORM_ACTIVATION_STATUS   LastActivationStatus;
  EFI_STATUS  Status;

  FnOutPtr = (ROOT_DSM_INTEL_FNOUT_GET_DEVICES_FW_RUNTIME_ACTIVATION_INFO*) mNvdimmAcpiSmmInterface->OutputBuffer;
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (ROOT_DSM_INTEL_FNOUT_GET_DEVICES_FW_RUNTIME_ACTIVATION_INFO);
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  DEBUG ((DEBUG_ERROR, "DsmRootIntelGetDevicesFwRuntimeActivationInfo: Begin\n"));

  if (!mFwActivationSupported) {
    FnOutPtr->Status = DSM_STS_ROOT_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "FW Activation not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  if (!mCrFwActivateProtocol) {
    FnOutPtr->Status = DSM_STS_ROOT_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "CrFwActivateProtocol is not installed.\n"));
    return EFI_UNSUPPORTED;
  }

  Status = mCrFwActivateProtocol->GetPlatformActivationState (mCrFwActivateProtocolHandle, &PlatformFwActivationState, &LastActivationStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get Platform Activation State, Status = 0x%x.\n", Status));
    FnOutPtr->Status = DSM_STS_ROOT_HW_ERROR;
    return EFI_UNSUPPORTED;
  }

  if (PlatformFwActivationState == CrActivationStateArmed) {
    // TODO: Use proper formulas to count those times
    FnOutPtr->EstimatedFwActivationTime = GetMemoryQuiesceTimeRequired () + BMC_QUIESCE_DIMM_PENALTY;
    FnOutPtr->EstimatedIoAccessToMemoryQuiesceTime = GetMemoryQuiesceTimeRequired () + BMC_QUIESCE_DIMM_PENALTY;
    FnOutPtr->EstimatedProcessorQuiesceTime = GetMemoryQuiesceTimeRequired () + BMC_QUIESCE_DIMM_PENALTY;
    FnOutPtr->PlatformFwSupportedMaxIoAccessToMemoryQuiesceTime = MIN(MEMORY_QUIESCE_TIME, mMaxIoQuiesceTime);
  }

  FnOutPtr->ActivationCapability |= PLATFORM_FIRMWARE_SUPPORTS_OS_ASSISTED_IO_DEVICE_QUIESCE_BASED_FW_ACTIVATION; // Set when at least one DIMM supports FW Activation
  if (mMaxIoQuiesceTime > 0) {
    FnOutPtr->ActivationCapability |= PLATFORM_FIRMWARE_SUPPORTS_LIVE_FW_ACTIVATION;
  }

  FnOutPtr->ActivationState = PlatformFwActivationState;

  DEBUG ((DEBUG_ERROR, "DsmRootIntelGetDevicesFwRuntimeActivationInfo: End\n"));

  return EFI_SUCCESS;
}


EFI_STATUS
DsmRootIntelActivateFirmware (
  VOID
  )
{
  ROOT_DSM_INTEL_FNOUT_ACTIVATE_FIRMWARE *FnOutPtr;
  ROOT_DSM_INTEL_FNINP_ACTIVATE_FIRMWARE *FnInpPtr;
  UINT64 QuiesceTimeRequired;
  UINT8 PlatformFwActivationState;
  UINT8 NumArmedDevices;
  UINT8 NumArmedAndStagedDevices;
  EFI_STATUS Status;

  DEBUG ((DEBUG_ERROR, "DsmRootIntelActivateFirmware: Begin\n"));

  FnOutPtr = (ROOT_DSM_INTEL_FNOUT_ACTIVATE_FIRMWARE *) mNvdimmAcpiSmmInterface->OutputBuffer;
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (ROOT_DSM_INTEL_FNOUT_ACTIVATE_FIRMWARE);
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (!mFwActivationSupported) {
    FnOutPtr->Status = DSM_STS_ROOT_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "FW Activation not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  if (!mCrFwActivateProtocol) {
    FnOutPtr->Status = DSM_STS_ROOT_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "CrFwActivateProtocol is not installed.\n"));
    return EFI_UNSUPPORTED;
  }

  FnInpPtr = (ROOT_DSM_INTEL_FNINP_ACTIVATE_FIRMWARE *) &mNvdimmAcpiSmmInterfaceInputBuffer[0];
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr)) {
    FnOutPtr->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if Platform allows Activation
  //
  Status = GetPlatformFwActivationState (&NumArmedDevices, &NumArmedAndStagedDevices, &PlatformFwActivationState);
  if (EFI_ERROR (Status)) {
    FnOutPtr->Status = DSM_STS_ROOT_HW_ERROR;
    return EFI_NOT_READY;
  }

  //
  // If Activation State is Busy do not preceed with Activation
  //
  if (PlatformFwActivationState == CrActivationStateBusy) {
    DEBUG ((DEBUG_ERROR, "DsmRootIntelActivateFirmware: Platform is Busy.\n"));
    FnOutPtr->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
    FnOutPtr->ExtendedStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNKNOWN_ERROR;
    return EFI_NOT_READY;
  }

  //
  // Check if any DIMM is armed for FW Activation
  //
  if (NumArmedDevices == 0) {
    DEBUG ((DEBUG_ERROR, "DsmRootIntelActivateFirmware: No Armed Devices found.\n"));
    FnOutPtr->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
    FnOutPtr->ExtendedStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_NO_ARMED_DEVICES;
    return EFI_NOT_READY;
  }
  DEBUG ((DEBUG_INFO, "DsmRootIntelActivateFirmware: Armed devices found.\n"));

  //
  // Check if any armed DIMM has new staged firmware to activate
  //
  if (NumArmedAndStagedDevices == 0) {
    DEBUG ((DEBUG_ERROR, "DsmRootIntelActivateFirmware: No new staged firmware found.\n"));
    FnOutPtr->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
    FnOutPtr->ExtendedStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_NO_NEW_FIRMWARE;
    return EFI_NOT_READY;
  }
  DEBUG ((DEBUG_INFO, "DsmRootIntelActivateFirmware: New staged firmware found.\n"));

  //
  // Check memory timing requirements
  //
  QuiesceTimeRequired = GetMemoryQuiesceTimeRequired ();
  if (FnInpPtr->IoDeviceState == OS_MANAGED_IO_CPU_QUIESCE) {
    if (QuiesceTimeRequired > MEMORY_QUIESCE_TIME) {
      DEBUG ((DEBUG_ERROR, "Quiesce Time required by DIMMs exceeds %dms: %lld\n", MEMORY_QUIESCE_TIME, QuiesceTimeRequired));
      FnOutPtr->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
      FnOutPtr->ExtendedStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNABLE_TO_MEET_MEMORY_TIMING_REQS;
      return EFI_UNSUPPORTED;
    }
  } else {
    if (QuiesceTimeRequired > MIN(MEMORY_QUIESCE_TIME, mMaxIoQuiesceTime)){
      DEBUG ((DEBUG_ERROR, "Quiesce Time required by DIMMs exceeds MIN(%d, %lld): %lld\n", MEMORY_QUIESCE_TIME, mMaxIoQuiesceTime, QuiesceTimeRequired));
      FnOutPtr->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
      FnOutPtr->ExtendedStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNABLE_TO_MEET_MEMORY_TIMING_REQS;
      return EFI_UNSUPPORTED;
    }
  }
  DEBUG ((DEBUG_INFO, "DsmRootIntelActivateFirmware: Memory timing requirements are met.\n"));

  //
  // Begin Activation
  //
  DEBUG ((DEBUG_INFO, "DsmRootIntelActivateFirmware: Activating.\n"));
  AepFwActivateViaBmc (MEMORY_QUIESCE_TIME / 1000, 50, 1, FnInpPtr->IoDeviceState);

  //
  // Collect Activation Status
  //
  FnOutPtr->Status = DSM_STS_ROOT_SUCCESS;
  FnOutPtr->ExtendedStatus = mCrInfo.PlatformActivationStatus;
  if (mCrInfo.PlatformActivationStatus != 0) {
    FnOutPtr->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
  }
  DEBUG ((DEBUG_ERROR, "DsmRootIntelActivateFirmware: End\n"));
  return EFI_SUCCESS;
}


EFI_STATUS
DsmGetDeviceRuntimeFwActivationInfo (
  VOID
  )
{
  UINT8 Socket;
  UINT8 Ch;
  UINT8 Dimm;
  UINT8 ImcCh;
  UINT8 Imc;
  DSM_FNOUT_GET_DEVICE_RUNTIME_FW_ACTIVATION_INFO *FnOutPtr;
  NVDIMM *DimmPtr;
  CR_ACTIVATION_STATE BmcActivationState;
  EFI_STATUS Status;

  FnOutPtr = (DSM_FNOUT_GET_DEVICE_RUNTIME_FW_ACTIVATION_INFO*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (!mFwActivationSupported) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "FW Activation not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  if (!mCrFwActivateProtocol) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "CrFwActivateProtocol is not installed.\n"));
    return EFI_UNSUPPORTED;
  }

  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  DEBUG ((DEBUG_ERROR, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmGetDeviceRuntimeFwActivationInfo: Begin\n", Socket, Ch, Dimm));

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }

  if (mNvdimmAcpiSmmInterface->InputLength != 0) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (DimmPtr->ActivationData.ActivationEnabled == FALSE) {
    DEBUG ((DEBUG_ERROR, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Activation Not Enabled\n", Socket, Ch, Dimm));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
    return EFI_UNSUPPORTED;
  }

  Imc = ConvertSktChToImc (Ch);
  ImcCh = ConvertSktChToImcCh (Ch);
  Status = mCrFwActivateProtocol->GetDimmActivationState (mCrFwActivateProtocolHandle, Socket, Imc, ImcCh, Dimm, &BmcActivationState, NULL, NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "S%d.C%d.D%d: Failed GetDimmActivationState. EFI_STATUS = 0x%x.\n", Socket, Ch, Dimm, Status));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_HW_ERROR;
    return Status;
  }

  FnOutPtr->ActivationState        = BmcActivationState;
  FnOutPtr->LastFwActivationStatus = DimmPtr->ActivationData.LastActivationStatus;

  mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
  DEBUG ((DEBUG_ERROR, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmGetDeviceRuntimeFwActivationInfo: End\n", Socket, Ch, Dimm));
  return EFI_SUCCESS;
}

EFI_STATUS
DsmSetDeviceRuntimeFwActivationArmState (
  VOID
  )
{
  EFI_STATUS Status;
  UINT8 Socket;
  UINT8 Ch;
  UINT8 Dimm;
  UINT8 ImcCh;
  UINT8 Imc;
  DSM_FNINP_SET_DEVICE_RUNTIME_FW_ACTIVATION_ARM_STATE *FnInpPtr;
  NVDIMM *DimmPtr;


  mNvdimmAcpiSmmInterface->OutputLength = 0;

  if (!mFwActivationSupported) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "FW Activation not supported.\n"));
    return EFI_UNSUPPORTED;
  }

  if (!mCrFwActivateProtocol) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
    DEBUG ((DEBUG_ERROR, "CrFwActivateProtocol is not installed.\n"));
    return EFI_UNSUPPORTED;
  }

  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  DEBUG ((DEBUG_ERROR, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmSetDeviceRuntimeFwActivationArmState: Begin\n", Socket, Ch, Dimm));

  FnInpPtr = (DSM_FNINP_SET_DEVICE_RUNTIME_FW_ACTIVATION_ARM_STATE *) &mNvdimmAcpiSmmInterfaceInputBuffer[0];
  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr)) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  if (DimmPtr->ActivationData.ActivationEnabled == FALSE) {
    DEBUG ((DEBUG_ERROR, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Activation Not Enabled\n", Socket, Ch, Dimm));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
    return EFI_UNSUPPORTED;
  }

  Imc = ConvertSktChToImc (Ch);
  ImcCh = ConvertSktChToImcCh (Ch);
  Status = mCrFwActivateProtocol->SetArmState (mCrFwActivateProtocolHandle, Socket, Imc, ImcCh, Dimm, FnInpPtr->ArmState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "S%d.C%d.D%d: Failed SetArmState. EFI_STATUS = 0x%x.\n", Socket, Ch, Dimm, Status));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_HW_ERROR;
    return Status;
  }
  DEBUG ((DEBUG_ERROR, "S%d.C%d.D%d: DsmSetDeviceRuntimeFwActivationArmState: ActivationState: %d\n", Socket, Ch, Dimm, FnInpPtr->ArmState));

  mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
  DEBUG ((DEBUG_ERROR, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmSetDeviceRuntimeFwActivationArmState: End\n", Socket, Ch, Dimm));
  return EFI_SUCCESS;
}

VOID
FwActivateAtEndOfDxe (
  VOID
  )
{
  EFI_STATUS        Status;
  CTO_DEVICE_ENTRY *CtoDeviceArray;
  UINT32            CtoDeviceArraySize;
  UINT32            MaxIoQuiesceTime;

  if (!mFwActivationSupported) {
    return;
  }
  Status = OemGetPciCtoDeviceList (&CtoDeviceArray, &CtoDeviceArraySize, &MaxIoQuiesceTime);
  if (!EFI_ERROR (Status)) {
    mCtoDeviceArray = CtoDeviceArray;
    mCtoDeviceArraySize = CtoDeviceArraySize;
    mMaxIoQuiesceTime = (UINT64) MaxIoQuiesceTime * 1000;
  }
  DEBUG ((DEBUG_ERROR, "FW Activate: %d CTO devices, mMaxIoQuiesceTime[us]: %lld\n", mCtoDeviceArraySize, mMaxIoQuiesceTime));
  if (mMaxIoQuiesceTime < MEMORY_QUIESCE_TIME) {
    mFwActivationSupported = 0;
  }
}
