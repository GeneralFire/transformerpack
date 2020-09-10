/** @file
  Definitions for the TraceHubStatusCodeHandlerPei

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

#ifndef __TRACE_HUB_STATUS_CODE_WORKER_PEI_H__
#define __TRACE_HUB_STATUS_CODE_WORKER_PEI_H__

#include <PiPei.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseDebugPrintErrorLevelLib.h>
#include <Library/PcdLib.h>
#include <Library/PlatformPostCodeMapLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PrintLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/TraceHubDebugLib.h>
#include <Ppi/ReportStatusCodeHandler.h>

/**
  Convert status code value and extended data to readable ASCII string, send string to TraceHub device.

  @param  PeiServices      An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to TraceHub successfully.

**/
EFI_STATUS
EFIAPI
TraceHubStatusCodeReportWorkerPei (
  IN CONST  EFI_PEI_SERVICES      **PeiServices,
  IN EFI_STATUS_CODE_TYPE         CodeType,
  IN EFI_STATUS_CODE_VALUE        Value,
  IN UINT32                       Instance,
  IN CONST EFI_GUID               *CallerId,
  IN CONST EFI_STATUS_CODE_DATA   *Data OPTIONAL
  );

#endif
