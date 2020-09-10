/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2016 Intel Corporation. <BR>

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

#ifndef _IE_EWL_LIB_H_
#define _IE_EWL_LIB_H_

#include <Library/ReportStatusCodeLib.h>
#include <SysHost.h>

/**
  Add to Enhanced Warning Log entry for IE State Failures

  @param[in] Severity           EWL severity
  @param[in] MajorCheckpoint    Major Warning code
  @param[in] MinorCheckpoint    Minor Warning code
  @param[in] MajorWarningCode   Major checkpoint progress indicators
                                written to scratchpad CSR
  @param[in] MinorWarningCode   Minor checkpoint progress indicators
                                written to scratchpad CSR
  @retval None
**/
VOID
IeEwlStateFailures (
  IN UINT32 Severity,
  IN UINT8  MajorCheckpoint,
  IN UINT8  MinorCheckpoint,
  IN UINT8  MajorWarningCode,
  IN UINT8  MinorWarningCode
  );

/**
  Add to Enhanced Warning Log entry for IE Communication Failures

  @param[in] Severity           EWL severity
  @param[in] MajorCheckpoint    Major Warning code
  @param[in] MinorCheckpoint    Minor Warning code
  @param[in] MajorWarningCode   Major checkpoint progress indicators
                                written to scratchpad CSR
  @param[in] MinorWarningCode   Minor checkpoint progress indicators
                                written to scratchpad CSR
  @param[in] HeciDevice         HECI device (1, 2, or 3)
  @param[in] IeAddress          HECI address of IE entity
  @param[in] SendStatus         Status of send operation
  @param[in] ReceiveStatus      Status of receive operation
  @param[in] Request            First 8 bytes of request message
  @param[in] Response           First 4 bytes of response message

  @retval None
**/
VOID
IeEwlCommunicationFailures (
  IN UINT32     Severity,
  IN UINT8      MajorCheckpoint,
  IN UINT8      MinorCheckpoint,
  IN UINT8      MajorWarningCode,
  IN UINT8      MinorWarningCode,
  IN UINT8      HeciDevice,
  IN UINT8      IeAddress,
  IN EFI_STATUS SendStatus,
  IN EFI_STATUS ReceiveStatus,
  IN UINT64     Request,
  IN UINT32     Response
  )
;

//
// Checkpoint numbers used with type 30 & 31
//
#define IE_EWL_CHKP_MAJOR_NA         0   // Checkpoint unspecified
#define   IE_EWL_CHKP_MINOR_NA         0 // Checkpoint unspecified

#define IE_EWL_CHKP_MAJOR_PREMEM     1   // Before DRAM_INIT_DONE message
#define   IE_EWL_CHKP_PREMEM_HECI      0 // HECI initialization issue
#define   IE_EWL_CHKP_PREMEM_IESTATE   1 // IE state analysis
#define   IE_EWL_CHKP_PREMEM_MESEG     2 // MESEG (UMA) allocation

#define IE_EWL_CHKP_MAJOR_PREINIT    2   // Before IEFS1.InitComplete
#define   IE_EWL_CHKP_PREINIT_IESTATE  0 // IE state analysis
#define   IE_EWL_CHKP_PREINIT_MPHY     1 // MPHY update

#define IE_EWL_CHKP_MAJOR_POSTINIT   3   // Post IEFS1.InitComplete
#define   IE_EWL_CHKP_POSTINIT_IESTATE 0 // IE in recovery mode
#define   IE_EWL_CHKP_POSTINIT_MPHY    1 // MPHY update
#define   IE_EWL_CHKP_POSTINIT_COMPATIB 2 // IE-BIOS compatibility
#define   IE_EWL_CHKP_POSTINIT_ICC     3
#define   IE_EWL_CHKP_POSTINIT_EOP     4

//
// Warning Codes used with type 30 & 31
//
#define IE_EWL_WARN_MAJOR_NA         0   // Warning Code unspecified
#define   IE_EWL_WARN_MINOR_NA         0 // Warning Code unspecified

#define IE_EWL_WARN_MAJOR_HECI       1   // HECI access problem
#define   IE_EWL_WARN_HECI_PCI         0 // HECI PCI device access problem
#define   IE_EWL_WARN_HECI_API         1 // HECI API (protocol) access problem
#define   IE_EWL_WARN_HECI_MESSAGE     2 // HECI protocol message failed

#define IE_EWL_WARN_MAJOR_IEFS1      2   // Issue in IE Firmware Status #1
#define   IE_EWL_WARN_IEFS1_CURSTATE   0 // Unexpected MEFS1.CurrentState
#define   IE_EWL_WARN_IEFS1_INCOMPLETE 1 // Unexpected IEFS1.InitComplete
#define   IE_EWL_WARN_IEFS1_ERROR      0 // Unexpected IEFS1.ErrorCode
#define   IE_EWL_WARN_IEFS1_OPMODE     0 // Unexpected IEFS1.OperatingMode

#endif // _IE_EWL_LIB_H_
