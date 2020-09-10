/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <Library/TimerLib.h>
#include "CrystalRidge.h"
#include "Dsm.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsPatrolScrubber.h"
#include "Ars/ArsFlowsPrivate.h"
#include "Ars/ShortArs.h"

#define MAX_SHORT_ARS_ERRORS ((MAX_SOCKET) * (MAX_MC_CH) * (MAX_IMC) * 8 + 16)
#define GET_LONG_OP_STATUS_MAX_ITER 1000
#define GET_LONG_OP_STATUS_POLLING_INTERVAL_US 1000 // 1 ms interval

STATIC UINT64 mShortArsScrubList[MAX_SHORT_ARS_ERRORS];
STATIC UINT32 mShortArsScrubListLen;

/**
  Adds specified SPA to short ARS scrub list.

  Spa is aligned to ARS block size before addition.

  @param[in]  Spa                   System address.

  @retval     EFI_SUCCESS           Address added OK or already exists.
  @retval     EFI_BUFFER_TOO_SMALL  Short ARS scrub list full. Not added.

**/
EFI_STATUS
ShortArsAddError (
  IN  UINT64  Spa
  )
{
  UINT32 Index;

  if (mShortArsScrubListLen == ARRAY_SIZE (mShortArsScrubList)) {
    return EFI_BUFFER_TOO_SMALL;
  }
  AlignAddressForArs (&Spa, FALSE);
  for (Index = 0; Index < mShortArsScrubListLen; Index++) {
    if (mShortArsScrubList[Index] == Spa) {
      return EFI_SUCCESS;
    }
  }
  mShortArsScrubList[mShortArsScrubListLen++] = Spa;
  return EFI_SUCCESS;
}

/**
  Retrieves media log errors from all DIMMs, and adds all error locations
  that are currently mapped to PMEM to short ARS scrub list.
**/
VOID
ShortArsGatherMediaLogErrors (
  VOID
  )
{
  ERR_LOG_DATA   ErrLog;
  NVDIMM         *Dimm;
  EFI_STATUS     Status;
  MEDIA_ERR_LOG  *MediaLogPtr;
  UINT64         Spa;
  UINT16         LogIndex;
  UINT16         SequenceNumber;
  UINT8          Index;
  UINT8          Priority;

  for (Index = 0; Index < mCrInfo.NvdimmInfo.NumNvdimms; Index++) {

    Dimm = &mCrInfo.NvdimmInfo.Nvdimms[Index];
    if (Dimm->SADSpaBase == SPA_NOT_MAPPED) {
      continue;
    }
    for (Priority = 0; Priority <= 1 ; Priority++) {
      SequenceNumber = 0;
      while (TRUE) {
        Status = NvmCtlrGetErrorLog ((UINT8)Dimm->SocketId, Dimm->Ch, Dimm->Dimm, MEDIA_LOG_REQ, Priority, FALSE, &SequenceNumber, &ErrLog);

        if (Status == EFI_COMPROMISED_DATA) {
          ASSERT(FALSE);
          return;
        }

        if (Status == EFI_NOT_FOUND || ErrLog.MediaLogData.ReturnCount == 0) {
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ShortArsGatherMediaLogErrors: S%d.C%d.D%d: ", Dimm->SocketId, Dimm->Ch, Dimm->Dimm));
          CRDEBUG ((DEBUG_INFO, "no media logs (%d) found, new sequence number: %d\n", Priority, SequenceNumber));
          break;
        }

        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "ShortArsGatherMediaLogErrors: S%d.C%d.D%d: ", Dimm->SocketId, Dimm->Ch, Dimm->Dimm));
          CRDEBUG ((DEBUG_ERROR, "NvmCtlrGetErrorLog (..., MEDIA_LOG_REQ, %d, ..., %d, ...) failed: %r\n", Priority, SequenceNumber, Status));
          break;
        }

        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ShortArsGatherMediaLogErrors: S%d.C%d.D%d: ", Dimm->SocketId, Dimm->Ch, Dimm->Dimm));
        CRDEBUG ((DEBUG_INFO, "found %d media logs (%d), new sequence number: %d\n", ErrLog.MediaLogData.ReturnCount,
                Priority, SequenceNumber));

        if (ErrLog.MediaLogData.ReturnCount > ARRAY_SIZE (ErrLog.MediaLogData.MediaLogs)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "ShortArsGatherMediaLogErrors: S%d.C%d.D%d: ", Dimm->SocketId, Dimm->Ch, Dimm->Dimm));
          CRDEBUG ((DEBUG_ERROR, "media log return count (%d) is %d and exceeds MAX_MEDIA_LOGS\n", Priority, ErrLog.MediaLogData.ReturnCount));
        }

        for (LogIndex = 0; LogIndex < ErrLog.MediaLogData.ReturnCount && LogIndex < MAX_MEDIA_LOGS; LogIndex++) {
          MediaLogPtr = &ErrLog.MediaLogData.MediaLogs[LogIndex];
          CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ShortArsGatherMediaLogErrors: S%d.C%d.D%d: ", Dimm->SocketId, Dimm->Ch, Dimm->Dimm));
          CRDEBUG ((DEBUG_INFO, "media log: seq: %d, timestamp: %lld, error type %d\n",
                     MediaLogPtr->SequenceNumber,
                     MediaLogPtr->SysTimeStamp,
                     MediaLogPtr->ErrorType));
          CRDEBUG ((DEBUG_INFO, "                                                      "));
          CRDEBUG ((DEBUG_INFO, "error flags: 0x%2x, transaction type: %d\n",
                     MediaLogPtr->ErrorFlags,
                     MediaLogPtr->TransactionType));
          CRDEBUG ((DEBUG_INFO, "                                                      "));
          CRDEBUG ((DEBUG_INFO, "dpa: 0x%llx, pda: 0x%llx, range: %d\n",
                     MediaLogPtr->Dpa,
                     MediaLogPtr->Pda,
                     MediaLogPtr->Range));
          if ((MediaLogPtr->ErrorFlags & BIT1) == 0) {
            continue;
          }
          Status = GetPmemSpaFromDpa (Dimm, MediaLogPtr->Dpa, &Spa);
          if (!EFI_ERROR (Status)) {
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ShortArsGatherMediaLogErrors: S%d.C%d.D%d: ", Dimm->SocketId, Dimm->Ch, Dimm->Dimm));
            CRDEBUG ((DEBUG_INFO, "adding SPA 0x%lx to short ARS scrub list\n", Spa));
            ShortArsAddError (Spa);
          } else {
            CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ShortArsGatherMediaLogErrors: S%d.C%d.D%d: ", Dimm->SocketId, Dimm->Ch, Dimm->Dimm));
            CRDEBUG ((DEBUG_INFO, "PMEM SPA for DPA 0x%lx: %r\n", MediaLogPtr->Dpa, Status));
          }
        }
        if (ErrLog.MediaLogData.ReturnCount != ARRAY_SIZE (ErrLog.MediaLogData.MediaLogs)) {
          break;
        }
      }
    }
  }
}

/**
  Runs ARS on single SPA block.
  If uncorrectable/poison is found, it is added to Patrol Scrubber error list.

  @param[in]  Spa                    System address.

  @retval     EFI_INVALID_PARAMETER  Spa not aligned to ARS block.
  @retval     EFI_NO_RESPONSE        ARS did not complelete in time alotted.
  @retval     EFI_SUCCESS            Error found at Spa
  @retval     EFI_NOT_FOUND          Error not found at Spa
  @retval     <EFI error status>     Other error occurred

**/
EFI_STATUS
RunArsOnSpa (
  IN  UINT64  Spa
  )
{
  ARS_OP_STATUS  ArsStatus;
  EFI_STATUS     Status;
  UINT64         Dpa;
  UINT32         Iter;
  UINT8          Socket;
  UINT8          Ch;
  UINT8          Dimm;
  UINT8          Index;

  if (Spa % ARS_ALIGNMENT_SIZE) {
    return EFI_INVALID_PARAMETER;
  }
  Status = SpaToNvmDpa (Spa, &Socket, &Ch, &Dimm, &Dpa);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "RunArsOnSpa: SpaToNvmDpa (0x%lx, ...)  failed: %r\n", Spa, Status));
    return Status;
  }
  Status = NvmCtlrSetAddressRangeScrub (Socket, Ch, Dimm, Dpa, Dpa + 0x100, TRUE);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "RunArsOnSpa: S%d.C%d.D%d: NvmCtlrSetAddressRangeScrub (..., 0x%lx, ...) failed: %r\n",
            Socket, Ch, Dimm, Dpa, Status));
    return Status;
  }
  for (Iter = 0; Iter < GET_LONG_OP_STATUS_MAX_ITER; Iter++) {
    Status = GetLongOperationStatusDataForArs (Socket, Ch, Dimm, &ArsStatus);
    if (!EFI_ERROR (Status) && ArsStatus.InProgress == FALSE) {
      break;
    }
    MicroSecondDelay (GET_LONG_OP_STATUS_POLLING_INTERVAL_US);
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "RunArsOnSpa: S%d.C%d.D%d: get long op status iter: %d\n",
          Socket, Ch, Dimm, Iter));
  if (Iter == GET_LONG_OP_STATUS_MAX_ITER) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "RunArsOnSpa: S%d.C%d.D%d: reached maximal iteration limit\n",
            Socket, Ch, Dimm));
    return EFI_NO_RESPONSE;
  }
  for (Index = 0; Index < ArsStatus.ErrorsFound; Index++) {
    if (ArsStatus.DpaErrorAddress[Index] == Dpa) {
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "RunArsOnSpa: S%d.C%d.D%d: DPA 0x%llx found. Adding SPA 0x%llx\n",
              Socket, Ch, Dimm, Dpa, Spa));
      Status = ArsPatrolScrubberAdd (ConvertSocketChannelDimmToNfitHandle (Socket, Ch, Dimm), Spa);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "RunArsOnSpa: S%d.C%d.D%d: ArsPatrolScrubberAdd (..., 0x%llx) failed: %r\n",
                Socket, Ch, Dimm, Spa, Status));
        return Status;
      }
      return EFI_SUCCESS;
    }
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "RunArsOnSpa: S%d.C%d.D%d: DPA 0x%llx not found. Error cleared?\n",
          Socket, Ch, Dimm, Dpa));

  return EFI_NOT_FOUND;
}

/**
  Runs ARS on short ARS scrub list.
  Confirmed errors are added to Patrol Scrubber list.
**/
VOID
RunArsOnShortArsScrubList (
  VOID
  )
{
  UINT32  Index;

  for (Index = 0; Index < mShortArsScrubListLen; Index++) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "RunArsOnShortArsScrubList: running ARS on SPA 0x%lx\n", mShortArsScrubList[Index]));
    RunArsOnSpa (mShortArsScrubList[Index]);
  }
}
