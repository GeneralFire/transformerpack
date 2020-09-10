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

#include <Protocol/CrystalRidge.h>
#include <Library/TimerLib.h>
#include <Protocol/SmmCpuService.h>

#include <Library/UsraAccessApi.h>

#include <Cpu/Msr/ArchMsr.h>
#include <Library/PciSegmentLib.h>

#include "RcRegs.h"
#include "CrystalRidge.h"
#include "Dsm.h"
#include "Fis.h"
#include "FwActivate.h"
#include "NvdimmSmbusAccess.h"

#define ACTIVATION_ENABLE_FLAG  0x1
#define QUIESCE_DISABLE_FLAG    0x2

#ifdef _MSC_VER
#define LocalAsmReadTsc() __rdtsc ()
#endif // _MSC_VER
#ifdef __GNUC__
/**
  Reads the current value of Time Stamp Counter (TSC).

  Reads and returns the current value of TSC. This function is only available
  on IA-32 and X64.

  @return The current value of TSC

**/
__attribute__((always_inline)) inline
UINT64
LocalAsmReadTsc (
  VOID
  )
{
  UINT32  LowData;
  UINT32  HiData;

  __asm__ __volatile__ (
    "rdtsc"
    : "=a" (LowData),
      "=d" (HiData)
    );

  return (((UINT64)HiData) << 32) | LowData;
}

#endif // __GNUC__

extern EFI_SMM_SYSTEM_TABLE2* mSmst;

EFI_SMM_CPU_SERVICE_PROTOCOL    *pSmmCpuServiceProtocol = NULL;

UINT8 mPollState[MAX_SOCKET][MAX_IMC];

VOID
GatherDevices (
  UINT8 DimmActiveTable[MAX_SOCKET][MAX_CH][MAX_DIMM],
  BOOLEAN EnableActivationFlow
  )
{
  UINT8       Imc, ImcCh, MaxChDdr;
  UINT8       Socket, Ch, Dimm;
  NVDIMM      *DimmPtr;
  BOOLEAN     IsDimmArmed;

  MaxChDdr = GetMaxChDdr ();
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        ImcCh = ConvertSktChToImcCh (Ch);
        Imc   = ConvertSktChToImc (Ch);

        DimmActiveTable[Socket][Ch][Dimm] = 0;
        // Don't detect Aep if you don't need to activate
        if (EnableActivationFlow == 0) {
          continue;
        }
        DimmPtr = GetDimm(&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
        if (!DimmPtr) {
          continue;
        }
        IsDimmArmed = GetDimmState (Socket, Imc, ImcCh, Dimm) == CrActivationStateArmed;

        if (IsDimmArmed) {
          DEBUG ((DEBUG_ERROR, "Armed DCPMM DIMM on Socket:%d, Mc:%d, Ch:%d, Dimm:%d\n", Socket, Imc, ImcCh, Dimm));
          DimmActiveTable[Socket][Ch][Dimm] = 1;
        }
      }
    }
  }
}

EFI_STATUS
UpdateLastActivationStatusForDimms (
  IN     UINT8    DimmActiveTable[MAX_SOCKET][MAX_CH][MAX_DIMM],
     OUT BOOLEAN  *IsSystemOkToContinuePtr
)
{
  UINT8                     Socket, Imc, ImcCh, Ch, Dimm, MaxChDdr;
  NVDIMM                    *DimmPtr;
  BOOLEAN                   IsSystemOkToContinue;
  BOOLEAN                   IsDimmOkToContinue;
  EFI_STATUS                Status;
  CR_ACTIVATION_STATE       BmcActivationState;
  CR_ACTIVATION_STATUS      BmcLastActivationStatus;
  UINT64                    BmcLastActivationBsr;
  UINT64                    BmcLastActivationMailboxStatus;
  FIS_BOOT_STATUS_REGISTER  Bsr;
  FIS_STATUS                MbStatus;

  IsSystemOkToContinue = TRUE;
  MaxChDdr = GetMaxChDdr ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        ImcCh = ConvertSktChToImcCh (Ch);
        Imc   = ConvertSktChToImc (Ch);

        DimmPtr = GetDimm(&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
        if (!DimmPtr) {
          continue;
        }

        if (DimmActiveTable[Socket][Ch][Dimm]) {
          Status = mCrFwActivateProtocol->GetDimmActivationState (mCrFwActivateProtocolHandle, Socket, Imc, ImcCh, Dimm, &BmcActivationState, &BmcLastActivationStatus, &BmcLastActivationBsr, &BmcLastActivationMailboxStatus);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "S%d.C%d.D%d: Error when trying to get Dimm Specific status from BMC\n", Socket, Ch, Dimm));
            return Status;
          }

          //
          // No-staged status is set before activation
          //
          if (DimmPtr->ActivationData.LastActivationStatus == DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_NO_NEW_STAGED_FIRMWARE) {
            continue;
          }
          Bsr.Bits = ((FIS_BOOT_STATUS_REGISTER*)&BmcLastActivationBsr)->Bits;
          MbStatus = (FIS_STATUS)BmcLastActivationMailboxStatus;

          IsDimmOkToContinue = FALSE;
          if (BmcActivationState == CrActivationStateIdle && Bsr.QWord != CR_INVALID_BSR_CONTENT){

            if (Bsr.Bits.MailboxInterfaceReady && Bsr.Bits.FwActivateComplete) {
              DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_SUCCESS;
              IsDimmOkToContinue = TRUE;
            } else if (Bsr.Bits.MailboxInterfaceReady && !Bsr.Bits.FwActivateComplete && MbStatus != CR_INVALID_MAILBOX_STATUS_CONTENT) {
              IsDimmOkToContinue = TRUE;
              if (MbStatus == FIS_STS_INVALID_PARAM || MbStatus == FIS_STS_INVALID_ACTIVATION_VERSION) {
                DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_FW_DOES_NOT_MEET_ACTIVATION_REQS;
              }
            } else if (Bsr.Bits.RebootRequired) {
              DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_NVDIMM_RESET_REQUIRED;
            } else if (Bsr.Bits.MediaDisabled) {
              DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_MEDIA_DISABLED;
            } else {
              DimmPtr->ActivationData.LastActivationStatus = DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_UNCLASSIFIED_FW_ACTIVATION_ERROR;
            }
          }

          if (!IsDimmOkToContinue) {
            DEBUG ((DEBUG_ERROR, "S%d.C%d.D%d: Activation Error.\n", Socket, Ch, Dimm));
            DEBUG ((DEBUG_ERROR, "BmcActivationState: 0x%x BmcLastActivationStatus: 0x%x BSR: 0x%llx MbStatus: 0x%llx\n", BmcActivationState, BmcLastActivationStatus, Bsr.QWord, MbStatus));
            IsSystemOkToContinue = FALSE;
          }
        }
      }
    }
  }

  if (IsSystemOkToContinuePtr) {
    *IsSystemOkToContinuePtr = IsSystemOkToContinue;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
AepFwActivateViaBmc (UINT32 QuiesceTimeout, UINT32 DMATimeoutValue, UINT32 ActivationFlowEn, UINT8 IoDeviceQuiesceSource)
{
  BOOLEAN       EnableActivationFlow = ActivationFlowEn & ACTIVATION_ENABLE_FLAG;
  UINT8         DimmActiveTable[MAX_SOCKET][MAX_CH][MAX_DIMM];
  UINT8         ActivationTimedout = TRUE;
  BOOLEAN       NeedToCheckPerDimmStatus;
  BOOLEAN       IsSystemOkToContinue;
  EFI_STATUS    Status = EFI_SUCCESS;
  UINT64        TicksPerMillisecond;
  UINT64        NumOfMilliseconds;
  UINT64        EndOfWaitInTicks;
  UINT32        TotalQMs;
  UINT8         Socket;
  UINT8         MaxImc;
  UINT64        StartQ;
  UINT64        Count;
  UINT64        EndQ;
  UINT8         Mc;
  CR_ACTIVATION_STATE  BmcActivationState;
  CR_ACTIVATION_STATUS LastActivationStatus;

  DEBUG ((DEBUG_ERROR, "AepFwActivate: QuiesceTimeout[ms]: %d, DMATimeoutValue[ms]: %d, ActivationFlowEn: %x, IoDevQuiesce: %d\n",
                      QuiesceTimeout, DMATimeoutValue, ActivationFlowEn, IoDeviceQuiesceSource));

  // ----------------------------------------
  // Calculate number of ticks per millisecond
  // ----------------------------------------
  TicksPerMillisecond = LocalAsmReadTsc();
  MicroSecondDelay(1000); //delay 1ms
  TicksPerMillisecond = LocalAsmReadTsc() - TicksPerMillisecond;

  // ----------------------------------------
  // Harvest DCPMM DIMM presence information
  // ----------------------------------------
  GatherDevices (DimmActiveTable, EnableActivationFlow);

  // ----------------------------------------
  // Disable TSOD Polling during activation
  // ----------------------------------------
  MaxImc = GetMaxImc ();
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!mSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    for (Mc = 0; Mc < MaxImc; Mc++) {
      if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Mc]) {
        continue;
      }
      NvdimmSmbuslGetPollingState (Socket, Mc, &mPollState[Socket][Mc]);
    }
  }
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!mSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    for (Mc = 0; Mc < MaxImc; Mc++) {
      if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Mc]) {
        continue;
      }
      NvdimmSmbusSetPollingState (Socket, Mc, 0, NULL);
    }
  }

  // -----------------------------------------------------------------------------
  // Harvest PCIe CTO information & change PCIe CTO to be more than 1sec
  // -------------------------------------------------------------------------------

  if (IoDeviceQuiesceSource == PLATFORM_FW_MANAGED_IO_CPU_QUIESCE) {
    for (Count = 0; Count < mCtoDeviceArraySize; Count++) {
      mCtoDeviceArray[Count].DeviceControl2Previous = PciSegmentRead32 (mCtoDeviceArray[Count].DeviceControl2Address);
      DEBUG ((DEBUG_ERROR, "CTO: 0x%llx devctl2: 0x%x\n", mCtoDeviceArray[Count].DeviceControl2Address, mCtoDeviceArray[Count].DeviceControl2Previous));
      PciSegmentWrite32 (mCtoDeviceArray[Count].DeviceControl2Address, (mCtoDeviceArray[Count].DeviceControl2Previous & (~0x1FU)) | mCtoDeviceArray[Count].DeviceControl2CtoValue);
    }
    MicroSecondDelay (DMATimeoutValue * 1000);
  }

  StartQ = LocalAsmReadTsc();

  // ----------------------------------
  // Execute Activate DCPMM FW Command
  // ----------------------------------
  Status = mCrFwActivateProtocol->ActivateFw (mCrFwActivateProtocolHandle, (UINT16)QuiesceTimeout, (UINT8)EnableActivationFlow);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error when sending IPMI Command to BMC.\n"));
    return Status;
  }

  // ----------------------------------------------------------------
  // For 1s after sending activation command check activation status
  // ----------------------------------------------------------------
  NeedToCheckPerDimmStatus = FALSE;
  IsSystemOkToContinue = FALSE;
  NumOfMilliseconds = 1000;
  EndOfWaitInTicks = LocalAsmReadTsc() + NumOfMilliseconds * TicksPerMillisecond;
  do {
    Status = mCrFwActivateProtocol->GetPlatformActivationState (mCrFwActivateProtocolHandle, &BmcActivationState, &LastActivationStatus);

    if (BmcActivationState == CrActivationStateIdle){
      ActivationTimedout = FALSE;
      switch (LastActivationStatus)
      {
        case CrPlatformActivationStatusSuccess:
          mCrInfo.PlatformActivationStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_SUCCESS;
          NeedToCheckPerDimmStatus         = TRUE; // Per-DIMM Status will decide if system is OK to continue
          break;
        case CrPlatformActivationStatusQuiesceFailure:
          mCrInfo.PlatformActivationStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNKNOWN_ERROR;
          NeedToCheckPerDimmStatus         = FALSE;
          IsSystemOkToContinue             = TRUE;
          break;
        case CrPlatformActivationStatusTimeout:
          mCrInfo.PlatformActivationStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNKNOWN_ERROR;
          NeedToCheckPerDimmStatus         = TRUE; // Per-DIMM Status will decide if system is OK to continue
          break;
        case CrPlatformActivationStatusFailed:
          mCrInfo.PlatformActivationStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNKNOWN_ERROR;
          NeedToCheckPerDimmStatus         = TRUE; // Per-DIMM Status will decide if system is OK to continue
          break;
        default:
          mCrInfo.PlatformActivationStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNKNOWN_ERROR;
          NeedToCheckPerDimmStatus         = TRUE; // Per-DIMM Status will decide if system is OK to continue
          break;
      }
      break;
    }
    MicroSecondDelay(5000);
  } while (LocalAsmReadTsc() < EndOfWaitInTicks);

  EndQ = LocalAsmReadTsc();
  TotalQMs = (UINT32)((EndQ - StartQ) / TicksPerMillisecond);

  DEBUG ((DEBUG_ERROR, "Platform Activation Status: 0x%x\n", LastActivationStatus));
  DEBUG ((DEBUG_ERROR, "Total Activation Time: %dms\n", TotalQMs));

  if (ActivationTimedout) {
    DEBUG ((DEBUG_ERROR, "Activation timed out.\n"));
    mCrInfo.PlatformActivationStatus = DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNKNOWN_ERROR;
    NeedToCheckPerDimmStatus         = TRUE;
  }

  // ----------------------------------------
  // Udpdate Dimms state
  // ----------------------------------------

  if (NeedToCheckPerDimmStatus) {
    Status = UpdateLastActivationStatusForDimms (DimmActiveTable, &IsSystemOkToContinue);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if (!IsSystemOkToContinue) {
    DEBUG ((DEBUG_ERROR, "Error during FW Activation. Cannot continue. Find the root-cause above.\n"));
    CpuDeadLoop ();
  }

  // ----------------------------------------
  // Restore TSOD Polling states for MCs
  // ----------------------------------------

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!mSystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    for (Mc = 0; Mc < MaxImc; Mc++) {
      if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Mc]) {
        continue;
      }
      NvdimmSmbusSetPollingState (Socket, Mc, mPollState[Socket][Mc], NULL);
    }
  }

  return EFI_SUCCESS;
}
