/** @file
  Contains protocol information for the Error Handling Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2017 Intel Corporation. <BR>

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

#ifndef _ERROR_HANDLING_PROTOCOL_H_
#define _ERROR_HANDLING_PROTOCOL_H_

//
// Reporter Protocol (This should be moved to CpRasPkg)
//

#define REPORTER_PROTOCOL_GUID \
  { \
      0xdb5b875, 0x7c10, 0x4d3c, 0x88, 0xec, 0x74, 0xc8, 0xb7, 0x98, 0xb6, 0xab   \
  }

typedef enum {
  AutoOption, //Let implementation decide if clearing on detect or not
  ClearError, //Clear after handling
  DoNotClearError   //Do not clear after handling
} ClearOption;


typedef
BOOLEAN
(EFIAPI *IS_SUPPORTED) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *ENABLE_REPORTING) (
  VOID                    // RASCM_TODO - add parameters for devicetype, eventtype, scope similar to DisableReporting()
  );

typedef
EFI_STATUS
(EFIAPI *DISABLE_REPORTING) (
  UINT32 DeviceType,
  UINT32 EventType,
  UINT32 *ExtData
  );

typedef
BOOLEAN
(EFIAPI *CHECK_STATUS) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *DETECT_AND_HANDLE) (
  UINT32    *CurrentSev,
  ClearOption      Clear
  );

typedef
EFI_STATUS
(EFIAPI *CLEAR_STATUS) (
  VOID
  );

typedef
struct _REPORTER_PROTOCOL {
  IS_SUPPORTED      IsSupported;
  ENABLE_REPORTING  EnableReporting;
  DISABLE_REPORTING DisableReporting;
  CHECK_STATUS      CheckStatus;
  CLEAR_STATUS      ClearStatus;
  DETECT_AND_HANDLE DetectAndHandle;
} REPORTER_PROTOCOL;


extern EFI_GUID gReporterProtocolGuid;


//
// Forward reference for pure ANSI compatability
//

#define EFI_ERROR_HANDLING_PROTOCOL_GUID \
   { \
      0x3ba7e14b, 0x176d, 0x4b2a, 0x94, 0x8a, 0xc8, 0x6f, 0xb0, 0x01, 0x94, 0x3c \
   }

typedef enum {
  NoPriority,
  LowPriority,
  MediumPriority,
  HighPriority,
  MaxPriority
} HandlerPriority;

// Type definitions

//typedef
//EFI_STATUS
//(EFIAPI *EFI_ERROR_HANDLER_FUNCTION) (
//      OUT   UINT32            *Severity
//  );

typedef struct _HANDLER_LIST_ENTRY {
  REPORTER_PROTOCOL           *Reporter;
  UINT8                       Priority;
  struct _HANDLER_LIST_ENTRY  *NextHandler;
} HANDLER_LIST_ENTRY;

// Protocol Interfaces

typedef
EFI_STATUS
(EFIAPI *EFI_ERROR_HANDLING_REGISTER_HANDLER_FUNCTION) (
  //IN      EFI_ERROR_HANDLER_FUNCTION  ErrorHandler,
  IN      REPORTER_PROTOCOL           *Reporter,
  IN      UINT8                       Priority
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ERROR_HANDLING_HANDLE_BOOT_ERROR) (
  IN      VOID    *ErrorRecord,
  IN      UINT8   Type
  );

typedef struct _EFI_ERROR_HANDLING_PROTOCOL {
  EFI_ERROR_HANDLING_REGISTER_HANDLER_FUNCTION  RegisterHandler;
} EFI_ERROR_HANDLING_PROTOCOL;

extern EFI_GUID gEfiErrorHandlingProtocolGuid;


#endif
