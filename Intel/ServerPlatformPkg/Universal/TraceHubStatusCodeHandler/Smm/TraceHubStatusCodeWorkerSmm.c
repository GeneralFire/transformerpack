/** @file
  TraceHubStatusCodeHandlerSmm implementation.

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

#include <TraceHubStatusCodeWorkerSmm.h>

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
  )
{
  CHAR8                     *Filename;
  CHAR8                     *Description;
  CHAR8                     *Format;
  CHAR8                     Buffer[EFI_STATUS_CODE_DATA_MAX_SIZE];
  UINT32                    ErrorLevel;
  UINT32                    LineNumber;
  UINTN                     CharCount;
  BASE_LIST                 Marker;
  UINT32                    DebugPrintErrorLevel;
  TRACE_HUB_SEVERITY_TYPE   TraceHubSeverityLevel;
  EFI_STATUS    StatusTmp;

  Buffer[0] = '\0';

  DebugPrintErrorLevel = GetDebugPrintErrorLevel ();

  if (Data != NULL &&
      ReportStatusCodeExtractAssertInfo (CodeType, Value, Data, &Filename, &Description, &LineNumber)) {
    if ((DEBUG_ERROR& DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Print ASSERT() information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "\n\rSMM_ASSERT!: %a (%d): %a\n\r",
                  Filename,
                  LineNumber,
                  Description
                  );

    TraceHubSeverityLevel = SeverityFatal;
  } else if (Data != NULL &&
             ReportStatusCodeExtractDebugInfo (Data, &ErrorLevel, &Marker, &Format)) {
    if ((ErrorLevel & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Print DEBUG() information into output buffer.
    //
    CharCount = AsciiBSPrint (
                  Buffer,
                  sizeof (Buffer),
                  Format,
                  Marker
                  );

    if (ErrorLevel == DEBUG_WARN) {
      TraceHubSeverityLevel = SeverityNormal;
    } else if (ErrorLevel == DEBUG_ERROR) {
      TraceHubSeverityLevel = SeverityError;
    } else {
      TraceHubSeverityLevel = SeverityNormal;
    }
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE) {
    if ((DEBUG_ERROR& DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Print ERROR information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "ERROR: C%08x:V%08x I%x",
                  CodeType,
                  Value,
                  Instance
                  );
    ASSERT (CharCount > 0);

    if (CallerId != NULL) {
      CharCount += AsciiSPrint (
                     &Buffer[CharCount],
                     (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                     " %g",
                     CallerId
                     );
    }

    if (Data != NULL) {
      CharCount += AsciiSPrint (
                     &Buffer[CharCount],
                     (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                     " %x",
                     Data
                     );
    }

    CharCount += AsciiSPrint (
                   &Buffer[CharCount],
                   (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                   "\n\r"
                   );

    TraceHubSeverityLevel = SeverityError;
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) {
    //
    // Print PROGRESS information into output buffer.
    //
    // Print in PlatformPort80HandlerSmm.efi for synchronizing all output channel
    Buffer[0] = '\0';
    CharCount = 0;

    TraceHubSeverityLevel = SeverityNormal;
  } else if (Data != NULL &&
             CompareGuid (&Data->Type, &gEfiStatusCodeDataTypeStringGuid) &&
             ((EFI_STATUS_CODE_STRING_DATA *) Data)->StringType == EfiStringAscii) {
    if ((DEBUG_INFO & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // EFI_STATUS_CODE_STRING_DATA
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "%a\n\r",
                  ((EFI_STATUS_CODE_STRING_DATA *) Data)->String.Ascii
                  );

    TraceHubSeverityLevel = SeverityNormal;
  } else {
    if ((DEBUG_INFO & DebugPrintErrorLevel) == 0) {
      return EFI_SUCCESS;
    }
    //
    // Code type is not defined.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "Undefined: C%x:V%x I%x\n\r",
                  CodeType,
                  Value,
                  Instance
                  );

    TraceHubSeverityLevel = SeverityNone;
  }

  //
  // Call TraceHub Lib function to do print.
  //
  StatusTmp = TraceHubDebugWrite (TraceHubSeverityLevel, (UINT8 *) Buffer, CharCount);
  ASSERT (StatusTmp != EFI_UNSUPPORTED);
  return EFI_SUCCESS;
}
