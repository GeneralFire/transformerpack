/** @file
  Header file for ME Error Warning Log Lib

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _ME_EWL_LIB_H_
#define _ME_EWL_LIB_H_

#include <Library/ReportStatusCodeLib.h>

///
/// EWL severities
///
typedef enum {
  MeEwlSeverityInfo,
  MeEwlSeverityWarning,
  MeEwlSeverityFatal,
  MeEwlSeverityMax,
  MeEwlSeverityDelim = MAX_UINT32
} ME_EWL_SEVERITY;

/**
  Add to Enhanced Warning Log entry for ME State Failures

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
  );

/**
  Add to Enhanced Warning Log entry for Me Communication Failures

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
  );


//
// Checkpoint numbers used with type 30 & 31
//
#define ME_EWL_CHKP_MAJOR_NA         0   // Checkpoint unspecified
#define   ME_EWL_CHKP_MINOR_NA         0 // Checkpoint unspecified

#define ME_EWL_CHKP_MAJOR_PREMEM     1   // Before DRAM_INIT_DONE message
#define   ME_EWL_CHKP_PREMEM_HECI      0 // HECI initialization issue
#define   ME_EWL_CHKP_PREMEM_MESTATE   1 // ME state analysis
#define   ME_EWL_CHKP_PREMEM_MESEG     2 // MESEG (UMA) allocation

#define ME_EWL_CHKP_MAJOR_PREINIT    2   // Before MEFS1.InitComplete
#define   ME_EWL_CHKP_PREINIT_MESTATE  0 // ME state analysis
#define   ME_EWL_CHKP_PREINIT_MPHY     1 // MPHY update

#define ME_EWL_CHKP_MAJOR_POSTINIT   3   // Post MEFS1.InitComplete
#define   ME_EWL_CHKP_POSTINIT_MESTATE 0 // ME in recovery mode
#define   ME_EWL_CHKP_POSTINIT_MPHY    1 // MPHY update
#define   ME_EWL_CHKP_POSTINIT_COMPATIB 2 // ME-BIOS compatibility
#define   ME_EWL_CHKP_POSTINIT_ICC     3
#define   ME_EWL_CHKP_POSTINIT_EOP     4

//
// Warning Codes used with type 30 & 31
//
#define ME_EWL_WARN_MAJOR_NA         0   // Warning Code unspecified
#define   ME_EWL_WARN_MINOR_NA         0 // Warning Code unspecified

#define ME_EWL_WARN_MAJOR_HECI       1   // HECI access problem
#define   ME_EWL_WARN_HECI_PCI         0 // HECI PCI device access problem
#define   ME_EWL_WARN_HECI_API         1 // HECI API (protocol) access problem
#define   ME_EWL_WARN_HECI_MESSAGE     2 // HECI protocol message failed

#define ME_EWL_WARN_MAJOR_MEFS1      2   // Issue in ME Firmware Status #1
#define   ME_EWL_WARN_MEFS1_CURSTATE   0 // Unexpected MEFS1.CurrentState
#define   ME_EWL_WARN_MEFS1_INCOMPLETE 1 // Unexpected MEFS1.InitComplete
#define   ME_EWL_WARN_MEFS1_ERROR      0 // Unexpected MEFS1.ErrorCode
#define   ME_EWL_WARN_MEFS1_OPMODE     0 // Unexpected MEFS1.OperatingMode


#endif // _ME_EWL_LIB_H_
