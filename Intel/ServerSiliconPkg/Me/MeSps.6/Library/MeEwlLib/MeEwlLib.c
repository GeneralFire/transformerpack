/** @file
  MeEwlLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include "Sps.h"
#include "MeAccess.h"
#include <SysHost.h>
#include <Library/MeEwlLib.h>
#include <Library/MeTypeLib.h>

#define ME_EWL_STATE_FAILURES_TYPE           EWL_ENTRY_TYPE16
#define ME_EWL_STATE_FAILURES_ID             EwlType16
#define ME_EWL_COMMUNICATION_FAILURES_TYPE   EWL_ENTRY_TYPE17
#define ME_EWL_COMMUNICATION_FAILURES_ID     EwlType17

/**
  To separate interfaces ME has it's own severity type ME_EWL_SEVERITY.
  Function converts severity from ME_EWL_SEVERITY to EWL_SEVERITY

  @param[in] Severity           ME EWL severity

  @retval EWL severity
**/
UINT32
GetEwlSeverity (
  IN UINT32 Severity
  )
{
  switch (Severity) {
  default:
    ASSERT (FALSE);
  case MeEwlSeverityInfo:
    Severity = EwlSeverityInfo;
    break;
  case MeEwlSeverityWarning:
    Severity = EwlSeverityWarning;
    break;
  case MeEwlSeverityFatal:
    Severity = EwlSeverityFatal;
    break;
  }

  return Severity;
}

/**
  Add to Enhanced Warning Log entry for Me State Failures

  @param[in] Severity           ME EWL severity
  @param[in] MajorCheckpoint    Major Warning code
  @param[in] MinorCheckpoint    Minor Warning code
  @param[in] MajorWarningCode   Major checkpoint progress indicators
                                written to scratchpad CSR
  @param[in] MinorWarningCode   Minor checkpoint progress indicators
                                written to scratchpad CSR
  @retval None
**/
VOID
MeEwlStateFailures (
  IN UINT32 Severity,
  IN UINT8  MajorCheckpoint,
  IN UINT8  MinorCheckpoint,
  IN UINT8  MajorWarningCode,
  IN UINT8  MinorWarningCode
  )
{
  ME_EWL_STATE_FAILURES_TYPE Ewl;

  // Limit EWL from common code (e.g. HECI)
  // to SPS only
  if (MeTypeIsSps () == FALSE) {
    return;
  }

  Severity = GetEwlSeverity (Severity);

  Ewl.Header.Type = ME_EWL_STATE_FAILURES_ID;
  Ewl.Header.Size = sizeof (ME_EWL_STATE_FAILURES_TYPE);
  Ewl.Header.Severity = (UINT8)Severity;
  Ewl.Context.MajorCheckpoint = MajorCheckpoint;
  Ewl.Context.MinorCheckpoint = MinorCheckpoint;
  Ewl.Context.MajorWarningCode = MajorWarningCode;
  Ewl.Context.MinorWarningCode = MinorWarningCode;
  Ewl.Revision = 0;
  Ewl.Mefs1 = HeciPciRead32 (R_ME_HFS);
  Ewl.Mefs2 = HeciPciRead32 (R_ME_HFS_2);
  Ewl.H2fs = Heci2PciRead32 (R_ME_HFS);
  Ewl.H3fs = Heci3PciRead32 (R_ME_HFS);
  Ewl.H4fs = Heci4PciRead32 (R_ME_HFS);

  ReportStatusCodeEx (
    (EFI_ERROR_CODE | EFI_ERROR_MINOR),
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR,
    0,
    NULL,
    NULL,
    &Ewl,
    sizeof (ME_EWL_STATE_FAILURES_TYPE)
  );

}

/**
  Add to Enhanced Warning Log entry for ME communication failures

  @param[in] Severity           ME EWL severity
  @param[in] MajorCheckpoint    Major Warning code
  @param[in] MinorCheckpoint    Minor Warning code
  @param[in] MajorWarningCode   Major checkpoint progress indicators
                                written to scratchpad CSR
  @param[in] MinorWarningCode   Minor checkpoint progress indicators
                                written to scratchpad CSR
  @param[in] HeciDevice         HECI device (1, 2, or 3)
  @param[in] MeAddress          HECI address of ME entity
  @param[in] SendStatus         Status of send operation
  @param[in] ReceiveStatus      Status of receive operation
  @param[in] Request            First 8 bytes of request message
  @param[in] Response           First 4 bytes of response message

  @retval None
**/
VOID
MeEwlCommunicationFailures (
  IN UINT32     Severity,
  IN UINT8      MajorCheckpoint,
  IN UINT8      MinorCheckpoint,
  IN UINT8      MajorWarningCode,
  IN UINT8      MinorWarningCode,
  IN UINT8      HeciDevice,
  IN UINT8      MeAddress,
  IN EFI_STATUS SendStatus,
  IN EFI_STATUS ReceiveStatus,
  IN UINT64     Request,
  IN UINT32     Response
  )
{
  ME_EWL_COMMUNICATION_FAILURES_TYPE Ewl;

  Severity = GetEwlSeverity (Severity);

  Ewl.Header.Type = ME_EWL_COMMUNICATION_FAILURES_ID;
  Ewl.Header.Size = sizeof (ME_EWL_COMMUNICATION_FAILURES_TYPE);
  Ewl.Header.Severity = (UINT8)Severity;
  Ewl.Context.MajorCheckpoint = MajorCheckpoint;
  Ewl.Context.MinorCheckpoint = MinorCheckpoint;
  Ewl.Context.MajorWarningCode = MajorWarningCode;
  Ewl.Context.MinorWarningCode = MinorWarningCode;
  Ewl.Revision = 0;
  Ewl.Mefs1 = HeciPciRead32 (R_ME_HFS);
  Ewl.Mefs2 = HeciPciRead32 (R_ME_HFS_2);
  Ewl.HeciDevice = HeciDevice;
  Ewl.MeAddress = MeAddress;
  Ewl.SendStatus = (UINT8)SendStatus;
  Ewl.ReceiveStatus = (UINT8)ReceiveStatus;
  Ewl.Request = Request;
  Ewl.Response = Response;

  ReportStatusCodeEx (
    (EFI_ERROR_CODE | EFI_ERROR_MINOR),
    EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR,
    0,
    NULL,
    NULL,
    &Ewl,
    sizeof (ME_EWL_COMMUNICATION_FAILURES_TYPE)
  );

}
