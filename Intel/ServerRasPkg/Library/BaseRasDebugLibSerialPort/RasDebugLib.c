/** @file
  Implementation of RAS MP safe debug print routine.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/SerialPortLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/RasDebugLib.h>

#define RAS_MAX_DEBUG_MESSAGE_LENGTH  0x200

typedef struct {
  SPIN_LOCK                    MpLoggingLock;
  SPIN_LOCK                    MceLoggingLock;
} RAS_LOGGING_LOCK;

RAS_LOGGING_LOCK  *mLoggingLock = NULL;

/**
  The constructor function initialize the Serial Port Library

  @retval EFI_SUCCESS   The constructor always returns RETURN_SUCCESS.

**/
RETURN_STATUS
EFIAPI
BaseRasDebugLibInit (
  VOID
  )
{
  BOOLEAN      NewAllocation;

  mLoggingLock = GetRasGlobalData (L"LoggingLock", sizeof (*mLoggingLock), &NewAllocation);
  ASSERT (mLoggingLock != NULL);
  if (NewAllocation) {
    InitializeSpinLock (&mLoggingLock->MpLoggingLock);
    InitializeSpinLock (&mLoggingLock->MceLoggingLock);
  }

  return SerialPortInitialize ();
}


/*
  Retrieves the global RAS lock data structure and initialize if not yet initialized before.

  @return  global RAS lock data structure.

*/
RAS_LOGGING_LOCK *
GetRasLock (
  VOID
  )
{
  if (mLoggingLock == NULL) {
    BaseRasDebugLibInit ();
  }

  return mLoggingLock;
}

VOID
EFIAPI
RasDebugPrint (
  IN  UINTN        ErrorLevel,
  IN  CHAR8       *Format,
  ...
  )
{
  CHAR8             Buffer[RAS_MAX_DEBUG_MESSAGE_LENGTH];
  VA_LIST           Marker;
  MP_CPU_INFO       MpInfo;
  UINTN             Start;
  RAS_LOGGING_LOCK  *RasDebugLock;
  BOOLEAN           MpLockAcquired;
  BOOLEAN           MceLockAcquired;

  if ((ErrorLevel & GetDebugPrintErrorLevel ()) == 0) {
    return;
  }

  Start = 0;
  if ((ErrorLevel & (MP_PRINT | MCE_PRINT)) != 0) {
    GetProcessorInfo (&MpInfo);
    Start = AsciiSPrint (Buffer, sizeof (Buffer), "S%d C%d T%d ", MpInfo.Package, MpInfo.Core, MpInfo.Thread);
  }

  VA_START (Marker, Format);
  AsciiVSPrint (&Buffer[Start], sizeof (Buffer) - Start, Format, Marker);
  VA_END (Marker);

  RasDebugLock = GetRasLock ();
  if (RasDebugLock != NULL) {
    if ((ErrorLevel & MCE_PRINT) != 0) {
      AcquireSpinLock (&RasDebugLock->MceLoggingLock);
      MceLockAcquired = TRUE;
      MpLockAcquired = AcquireSpinLockOrFail (&RasDebugLock->MpLoggingLock);
    } else {
      AcquireSpinLock (&RasDebugLock->MpLoggingLock);
      MceLockAcquired = FALSE;
      MpLockAcquired = TRUE;
    }
  }

  SerialPortWrite ((UINT8 *) Buffer, AsciiStrLen (Buffer));

  if (RasDebugLock != NULL) {
    if (MpLockAcquired) {
      ReleaseSpinLock (&RasDebugLock->MpLoggingLock);
    }
    if (MceLockAcquired) {
      ReleaseSpinLock (&RasDebugLock->MceLoggingLock);
    }
  }
}
