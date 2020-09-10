/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2019 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include "KtiRas.h"
#include "KtiRasSupport.h"
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/TimerLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/CsrAccessLib.h>
#include <Guid/UboxIpInterface.h>

RAS_MPLINK_STATUS
SearchQuiesceBuffer (
  IN  UINT64        Address,
  IN  UINT32        *Data32
  )
{
  QUIESCE_DATA_TABLE_ENTRY      *Ptr;

  Ptr = mNextEntry - 1;
  while (Ptr >= (QUIESCE_DATA_TABLE_ENTRY *)mQuiesceBuf) {
    if (Ptr->CsrAddress == Address) {
      *Data32 = Ptr->DataMask.Data;
      return RAS_MPLINK_SUCCESS;
    }
    Ptr--;
  }

  return RAS_MPLINK_FAILURE_GENERAL;
}

VOID
RasMpLinkClearKtiInfoBuffer (
  VOID
)
{
  ZeroMem ((UINT8 *) mRasSocketData, sizeof (KTI_SOCKET_DATA));
  ZeroMem ((UINT8 *) mRasSocketDataOrig, sizeof (KTI_SOCKET_DATA));
  ZeroMem ((UINT8 *) mKtiInternalGlobal, sizeof (KTI_HOST_INTERNAL_GLOBAL));

  return;
}

VOID
RasMpLinkClearCpuShadowBuffer (
  IN UINT8        StartCpu,
  IN UINT8        EndCpu
)
{
  UINT8   SocId;

  for (SocId = StartCpu; SocId <= EndCpu; ++SocId) {
    ZeroMem ((UINT8 *) (mCpuShadowFlag + SocId * MAX_FLAG_BUF_PER_CPU), MAX_FLAG_BUF_PER_CPU);
  }

  return;
}

RAS_MPLINK_STATUS
RasMpLinkClearShadowCpuBufferInSteps (
  VOID
)
{
  STATIC UINT8    ClearBufType = 0;
  RAS_MPLINK_STATUS   Status = RAS_MPLINK_SUCCESS;

  switch (ClearBufType) {
    case 0:
      RasMpLinkClearCpuShadowBuffer(0, 1); // CPU 0, 1
      ClearBufType++; // Next two CPU sockets
      Status = RAS_MPLINK_PENDING;
      break;

    case 1:
      RasMpLinkClearCpuShadowBuffer(2, 3); // CPU 2, 3
      if (MAX_SOCKET <= 4) {
        ClearBufType = 0; // Done clearing
        Status = RAS_MPLINK_SUCCESS;
      } else {
        ClearBufType++; // Next two CPU sockets
        Status = RAS_MPLINK_PENDING;
      }
      break;

    case 2:
      RasMpLinkClearCpuShadowBuffer(4, 5); // CPU 4, 5
      ClearBufType++; // Next two CPU sockets
      Status = RAS_MPLINK_PENDING;
      break;

    case 3:
      RasMpLinkClearCpuShadowBuffer(6, 7); // CPU 6, 7
      ClearBufType = 0; // Done clearing
      Status = RAS_MPLINK_SUCCESS;
      break;

    default:
      ClearBufType = 0; // Done clearing
      Status = RAS_MPLINK_SUCCESS;
  }

  return Status;
}


VOID
RasMpLinkClearBuffer (
  VOID
)
{
  RasMpLinkClearKtiInfoBuffer ();
  while (RasMpLinkClearShadowCpuBufferInSteps() != RAS_MPLINK_SUCCESS);

  return;
}

VOID
RasMpLinkClearSocketData (
  VOID
)
{
  UINT8             Idx1;
  KTI_SOCKET_DATA  TempRasSocketData;

  ZeroMem ((UINT8 *) &TempRasSocketData, sizeof (TempRasSocketData));

  for (Idx1 = 0; Idx1 < MAX_SOCKET; ++Idx1) {
    TempRasSocketData.Cpu[Idx1].Valid = mRasSocketData->Cpu[Idx1].Valid;
    TempRasSocketData.Cpu[Idx1].SocId = mRasSocketData->Cpu[Idx1].SocId;
    CopyMem ((UINT8*)(UINTN)TempRasSocketData.Cpu[Idx1].LinkData, (UINT8*)(UINTN)(mRasSocketData->Cpu[Idx1].LinkData), sizeof (TempRasSocketData.Cpu[Idx1].LinkData));
  }

  CopyMem ((VOID *)mRasSocketData, (VOID *)&TempRasSocketData, sizeof (KTI_SOCKET_DATA));

  return;
}

VOID
RasMpLinkAllocateBuffer (
  VOID
)
{
  EFI_STATUS                Status;

  // Buffer to store the KTI related socket data gathered when O*L event is in progress
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof(KTI_SOCKET_DATA), &mRasSocketData);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof(KTI_SOCKET_DATA), &mRasSocketDataOrig);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, CPU_SHADOW_BUF_SIZE, &mCpuShadowBuf);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, CPU_SHADOW_FLAG_SIZE, &mCpuShadowFlag);
  ASSERT_EFI_ERROR (Status);

  // Buffer for Host KTI internal global
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof(KTI_HOST_INTERNAL_GLOBAL), &mKtiInternalGlobal);
  ASSERT_EFI_ERROR (Status);

  // Clear all the internal buffers
  RasMpLinkClearBuffer ();

  return;
}

#ifdef RAS_KTI_MEASURE_TIME

UINT32             mMsIndex;
UINT64             mStartTime;

VOID
GetExpiredTimeEntry (
  IN UINT32             MsIndex
  )
{
  mStartTime = GetPerformanceCounter ();
  mMsIndex = MsIndex;

  return;
}

VOID
GetExpiredTimeEntryPeriod (
  VOID
  )
{
  UINT8              Temp;
  UINT32             Latency = 0;

  Latency = TimeDiff (mStartTime, GetPerformanceCounter (), TDIFF_UNIT_US);
  Temp = GetKtiDebugPrintLevel ();
  SetKtiDebugPrintLevel (0xF);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    MileStone: %02u   Time Period from last entry (uSecs):  %u \n",
             mMsIndex, Latency));

  SetKtiDebugPrintLevel (Temp);

  return;
}

VOID
GetExpiredTimeExit (
  VOID
  )
{
  UINT8              Temp;
  UINT32              Latency = 0;

  Latency = TimeDiff (mStartTime, GetPerformanceCounter (), TDIFF_UNIT_US);
  Temp = GetKtiDebugPrintLevel ();
  SetKtiDebugPrintLevel (0xF);

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    MileStone: %02u   Elapsed Time (uSecs): %u \n",
             mMsIndex, Latency));

  SetKtiDebugPrintLevel (Temp);

  return;
}
#endif

#ifdef RAS_KTI_DEBUG
VOID
CsrWriteCheckPoint (
  VOID
  )
{
  UINT8   Size = 4;
  UINT64  CsrAddress = 0;

  mCsrWriteCtr++;
  if (mCsrWriteCtr % 16 == 0) {
    mNextEntry->Operation = EnumQuiesceWrite;
    mNextEntry->AccessWidth = 4;
    UBoxGetBiosNonStickyScratchPad10CsrAddress (mSbspId, 0, &Size, &CsrAddress);
    mNextEntry->CsrAddress = CsrAddress;
    mNextEntry->AndMask =0;
    mNextEntry->DataMask.OrMask = mCsrWriteCtr;
    mNextEntry++;
    mCsrWriteCtr++;
  }
}

VOID
MarkEndOfCsrWrite (
  VOID
  )
{
  UINT8   Size = 4;
  UINT64  CsrAddress = 0;

  mNextEntry->Operation = EnumQuiesceWrite;
  mNextEntry->AccessWidth = 4;
  UBoxGetBiosNonStickyScratchPad10CsrAddress (mSbspId, 0, &Size, &CsrAddress);
  mNextEntry->CsrAddress = CsrAddress;
  mNextEntry->AndMask = 0;
  mNextEntry->DataMask.OrMask = (0xE0F0 << 16) | mCsrWriteCtr;
  mNextEntry++;
  return;
}

VOID
DumpQuiesceBuffer (
  VOID
  )
{
  QUIESCE_DATA_TABLE_ENTRY     *TempPtr;

  if (mQuieseBufStart == NULL || mNextEntry == NULL) {
    return;
  }

  for (TempPtr = mQuieseBufStart; TempPtr < mNextEntry; ++TempPtr) {
    if ((TempPtr->Operation != EnumQuiesceWrite) || (TempPtr->AndMask!= 0) ||
        (TempPtr->AccessWidth != 1 && TempPtr->AccessWidth != 2 && TempPtr->AccessWidth != 4)) {
      ASSERT(FALSE);
    }
#ifdef RAS_KTI_DEBUG_REG_DETAIL
    RcDebugPrint (SDBG_INFO, "\n    WRITE    Addr: 0x%08X    Data:0x%08X    Size:%u    TempPtr/mQuieseBuf:0x%08X",
        (UINT32)TempPtr->CsrAddress, TempPtr->DataMask.OrMask, TempPtr->AccessWidth, TempPtr);
#endif
  }

  return;
}
#endif // RAS_KTI_DEBUG

#ifdef RAS_KTI_UNIT_TEST

EFI_SMM_SW_REGISTER_CONTEXT   KtiRasSwSmiContext;

EFI_STATUS
KtiRasDebugHandler (
  IN  EFI_HANDLE                    DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  )
{
  UINT32                Data32;
  UINT8                 Ctr1;
  INT8                  EventType = RAS_MPLINK_EVNT_RESET_SM;
  INT8                  SocketType = RAS_MPLINK_SOCKET_TYPE_NA;
  INT8                  SocketId = RAS_MPLINK_SOCKET_ID_NA;
  //INT8                  Size =4;
  RAS_MPLINK_STATUS         Status;

  //
  // Bits [7:0] - O*L event for CPU x. Only one bit can be set.
  // Bit [8]: 1 - Offline Event. 0 - Online Event
  //
  mDirectCommit = TRUE;

  Data32 = 0x02;   // online Socket 1 for unit test
  UBoxSetBiosNonStickyScratchPadRegisterValue (mSbspId, 0, 10, Data32);

  UBoxGetBiosNonStickyScratchPadRegisterValue (mSbspId, 0, 10, &Data32);
  Data32 = Data32 & 0x1FF;

  if (Data32 & 0xFF) {
    EventType = (Data32 & 0x100) ? RAS_MPLINK_EVNT_OFFLINE: RAS_MPLINK_EVNT_ONLINE;
    for (Ctr1 = 0; Ctr1 < MAX_SOCKET; ++Ctr1) {
      if (Data32 & (1 << Ctr1)) { // CPU x needs to be on/offlined?
        SocketType = RAS_MPLINK_SOCKET_TYPE_CPU;
        SocketId = Ctr1;
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    CPU %u %s event ", Ctr1, (EventType == RAS_MPLINK_EVNT_OFFLINE) ? "OFFLINE": "ONLINE"));
        break;
      }
    }
  }

  if (EventType != RAS_MPLINK_EVNT_RESET_SM) {
    while ((Status = RasMpLinkCheckEvent (EventType, SocketId)) == RAS_MPLINK_PENDING);
    if (Status != RAS_MPLINK_SUCCESS) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n    UPI RAS can not on/offline!!!\n "));
      CpuDeadLoop();
    }

    Status = RasMpLinkHandlerNonSliced(EventType, SocketId);
    if (Status != RAS_MPLINK_SUCCESS && Status != RAS_MPLINK_INT_DOMAIN_PENDING) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n    UPI RAS handling failed!!!\n "));
      CpuDeadLoop();
      RasMpLinkHandlerNonSliced(RAS_MPLINK_EVNT_RESET_SM, RAS_MPLINK_SOCKET_INFO_NA);
    } else if (Status == RAS_MPLINK_INT_DOMAIN_PENDING) {
      Status = RasMpLinkHandlerNonSliced(EventType, SocketId);
      if (Status != RAS_MPLINK_SUCCESS) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n\n    UPI RAS INT Domain init failed!!!\n "));
        CpuDeadLoop();
        RasMpLinkHandlerNonSliced(RAS_MPLINK_EVNT_RESET_SM, RAS_MPLINK_SOCKET_INFO_NA);
      }
    }
  }

  return EFI_SUCCESS;
}

VOID
RegisterSwSmi (
  VOID
  )
{
  EFI_STATUS        Status;
  EFI_HANDLE         TempHandle;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;

  Status = gBS->LocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
  ASSERT_EFI_ERROR (Status);

  KtiRasSwSmiContext.SwSmiInputValue = 0x76;
  //KtiRasSwSmiContext.Type  = 0x76;
  //KtiRasSwSmiContext.Phase = KtiRasDebugHandler;

  Status = SwDispatch->Register (SwDispatch, KtiRasDebugHandler, &KtiRasSwSmiContext, &TempHandle);
  ASSERT_EFI_ERROR (Status);

  return;
}
#endif  //RAS_KTI_UNIT_TEST



