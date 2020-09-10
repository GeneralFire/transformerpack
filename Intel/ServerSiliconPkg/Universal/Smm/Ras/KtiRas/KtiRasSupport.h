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

#ifndef _UPI_RAS_SUPPORT_H
#define _UPI_RAS_SUPPORT_H

#include "KtiRas.h"

#ifdef RAS_KTI_UNIT_TEST
extern UINT16                         SmbBase;
extern UINT32                         MmCfgBase;
#endif

RAS_MPLINK_STATUS
SearchQuiesceBuffer (
  IN  UINT64        Address,
  IN  UINT32        *Data32
  );

VOID
RasMpLinkClearKtiInfoBuffer (
  VOID
);

RAS_MPLINK_STATUS
RasMpLinkClearShadowCpuBufferInSteps (
  VOID
);

VOID
RasMpLinkClearBuffer (
  VOID
);

VOID
RasMpLinkClearSocketData (
  VOID
);

VOID
RasMpLinkAllocateBuffer (
  VOID
);

#ifdef RAS_KTI_MEASURE_TIME
VOID
GetExpiredTimeEntry (
  IN UINT32             MsIndex
  );
VOID
GetExpiredTimeEntryPeriod (
  VOID
  );
VOID
GetExpiredTimeExit (
  VOID
  );
#endif

#ifdef RAS_KTI_DEBUG
VOID
CsrWriteCheckPoint (
  VOID
  );

VOID
MarkEndOfCsrWrite (
  VOID
  );

VOID
DumpQuiesceBuffer (
  VOID
  );

#endif

#ifdef RAS_KTI_UNIT_TEST

EFI_STATUS
KtiRasDebugHandler (
  IN  EFI_HANDLE                    DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  );


VOID
RegisterSwSmi (
  VOID
  );

#endif           //RAS_KTI_UNIT_TEST

#if 0
VOID
DelayFn(
  UINT32 dCount
  );

EFI_STATUS
AcquireBus (
  VOID
  );

BOOLEAN
IoDone (
  IN      UINT8           *StsReg
  );

EFI_STATUS
SmbusReadWrite (
  IN      UINT8    SlaveAddress,
  IN      UINT8    Operation,
  IN      UINT8    Command,
  IN OUT  VOID     *Buffer
  );

RAS_MPLINK_STATUS
IssuePowerOnOffCommand (
  IN UINT8          EventType,
  IN INT32          SocketType,
  IN INT32          SocketId
  );
#endif

#endif // _UPI_RAS_SUPPORT_H
