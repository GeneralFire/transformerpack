/** @file
  Contains protocol information for the Platform Error Handling Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#ifndef _PLATFORM_ERROR_HANDLING_PROTOCOL_H_
#define _PLATFORM_ERROR_HANDLING_PROTOCOL_H_
#include <Guid/ErrorReportLibListener.h>
#include <Library/SynchronizationLib.h>
#include <Cpu/CpuCoreRegs.h>

//
// Forward reference for pure ANSI compatability
//

#define EFI_PLATFORM_ERROR_HANDLING_PROTOCOL_GUID \
   { \
      0x6820abd4, 0xa292, 0x4817, 0x91, 0x47, 0xd9, 0x1d, 0xc8, 0xc5, 0x35, 0x42 \
   }

typedef enum {
  CorrectableSev0,
  RecoverableSev1,
  UncorrectableSev2,
  MaxSeverity
} ERROR_SEVERITY;

typedef
EFI_STATUS
(EFIAPI *EFI_ERROR_LISTENER_FUNCTION) (
  IN      EFI_COMMON_ERROR_RECORD_HEADER   *ErrorRecordHeader
  );

typedef struct _LISTENER_LIST_ENTRY {
  EFI_ERROR_LISTENER_FUNCTION   Listener;
  struct _LISTENER_LIST_ENTRY   *NextListener;
} LISTENER_LIST_ENTRY;

typedef
EFI_STATUS
(EFIAPI *EFI_NOTIFIER_FUNCTION) (
  IN      UINT8     ErrorSeverity,
     OUT  BOOLEAN   *OsNotified
  );

typedef struct _NOTIFIER_LIST_ENTRY {
  EFI_NOTIFIER_FUNCTION         Notifier;
  struct _NOTIFIER_LIST_ENTRY   *NextNotifier;
  UINT8                         Priority;
} NOTIFIER_LIST_ENTRY;

// Protocol Interfaces

typedef
EFI_STATUS
(EFIAPI *EFI_PLATFORM_ERROR_HANDLING_REGISTER_ERROR_LISTENER) (
  IN      EFI_ERROR_LISTENER_FUNCTION  ErrorListener
);

typedef
EFI_STATUS
(EFIAPI *EFI_PLATFORM_ERROR_HANDLING_LOG_REPORTED_ERROR) (
  IN      EFI_COMMON_ERROR_RECORD_HEADER   *ErrorRecordHeader
);

typedef
EFI_STATUS
(EFIAPI *EFI_PLATFORM_ERROR_HANDLING_REGISTER_NOTIFIER) (
  IN      EFI_NOTIFIER_FUNCTION  FunctionPtr,
  IN      UINT8                  Priority
);

typedef
EFI_STATUS
(EFIAPI *EFI_PLATFORM_ERROR_HANDLING_NOTIFY_OS) (
  IN      UINT8   ErrorSeverity
);

#define MAX_ERROR_EVENTS MaxErrSrc
#define MAX_ERROR_SEVERITY    3
#define SEVERITY_MAP_FATAL 2
#define SEVERITY_MAP_RECOVERABLE 1
#define SEVERITY_MAP_CORRECTED 0


typedef enum {
  MceHdrRestart= 0,
  MceHdrReset,
  MceHdrAssert
} SMM_MCE_ENDING_ACTION;

typedef enum {
  McbankSrc,
  CsrSrc,
  PcieSrc,
  PlatformSrc,
  MaxErrSrc
} SMM_ERROR_SRC;

typedef struct {
  UINT32 BankIdx;
  UINT32 ApicId;
} SMM_ERROR_EVENT;

typedef struct {
  BOOLEAN ErrorFound;
  UINT32 SeverityMap;
  SMM_ERROR_EVENT Log[MAX_ERROR_SEVERITY];
} SMM_ERROR_EVENT_DATA;

typedef struct {
  BOOLEAN ErrorsFound;
  SMM_ERROR_EVENT_DATA Events[MAX_ERROR_EVENTS];
} SMM_ERROR_EVENT_LOG;

typedef struct {
  volatile BOOLEAN Lmce;
  volatile UINT64 CoreProcessingBank;
  volatile UINT64 UncoreProcessingBank;
  volatile UINT64 ThreadProcessingBank;
  volatile UINT64 CoreSmiSrcCp;
  volatile UINT64 UncoreSmiSrcCp;
  volatile UINT64 SmmMceSyncState;
  volatile SMM_ERROR_EVENT_LOG EventLog;
  volatile UINT8 CmciMcBankErrCount[MCBANK_MAX];
} SMM_ERROR_CPU_DATA_BLOCK;

typedef struct {
  SMM_ERROR_CPU_DATA_BLOCK      CpuData[FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber)];
  SPIN_LOCK                     SerialLock; //For Debug Purpose.
  volatile BOOLEAN              Broadcast;
} SMM_ERROR_MP_SYNC_DATA;

typedef struct _EFI_PLATFORM_ERROR_HANDLING_PROTOCOL {
  EFI_PLATFORM_ERROR_HANDLING_REGISTER_ERROR_LISTENER   RegisterErrorListener;
  EFI_PLATFORM_ERROR_HANDLING_LOG_REPORTED_ERROR        LogReportedError;
  EFI_PLATFORM_ERROR_HANDLING_REGISTER_NOTIFIER         RegisterNotifier;
  EFI_PLATFORM_ERROR_HANDLING_NOTIFY_OS                 NotifyOs;
  SMM_ERROR_MP_SYNC_DATA                                *SmmErrorMpSyncData;
} EFI_PLATFORM_ERROR_HANDLING_PROTOCOL;

extern EFI_GUID gEfiPlatformErrorHandlingProtocolGuid;

#endif
