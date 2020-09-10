/** @file
  Definitions for the TraceHubStatusCodeHandlerSmm

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2014 Intel Corporation. <BR>

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

#ifndef __TRACE_HUB_STATUS_CODE_WORKER_SMM_H__
#define __TRACE_HUB_STATUS_CODE_WORKER_SMM_H__

#include <PiSmm.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseDebugPrintErrorLevelLib.h>
#include <Library/PcdLib.h>
#include <Library/PlatformPostCodeMapLib.h>
#include <Library/PrintLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/TraceHubDebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/SmmReportStatusCodeHandler.h>

/**
  Convert status code value and extended data to readable ASCII string, send string to TraceHub device.

  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or software entity.
                           This included information about the class and subclass that is used to
                           classify the entity as well as an operation.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to TraceHub successfully.
  @retval EFI_DEVICE_ERROR EFI TraceHub device cannot work after ExitBootService() is called.
  @retval EFI_DEVICE_ERROR EFI TraceHub device cannot work with TPL higher than TPL_CALLBACK.

**/
EFI_STATUS
EFIAPI
TraceHubStatusCodeReportWorkerSmm (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data OPTIONAL
  );

#endif
