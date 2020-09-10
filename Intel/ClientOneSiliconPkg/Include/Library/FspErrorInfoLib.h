/** @file
  Library to provide service for sending FSP error information to bootloader.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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
#ifndef _PEI_DXE_FSP_ERROR_INFO_LIB_H_
#define _PEI_DXE_FSP_ERROR_INFO_LIB_H_

/**
  Function attempts to send FSP error information to bootloader
  by both FSP_ERROR_INFO_HOB and ReportStatusCode service.

  @param[in] CallerId           - GUID indicates which component is executing.
  @param[in] ErrorType          - GUID indicates what error was encountered.
  @param[in] Status             - EFI_STATUS code for the error.

  @retval EFI_SUCCESS           - The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SendFspErrorInfo (
  IN EFI_GUID CallerId,
  IN EFI_GUID ErrorType,
  IN UINT32   Status
  );

/**
  Function attempts to send FSP error information to bootloader
  by ReportStatusCode service.
  This typically is used by DXE drivers inside FSP which cannot
  create hob.

  @param[in] CallerId           - GUID indicates which component is executing.
  @param[in] ErrorType          - GUID indicates what error was encountered.
  @param[in] Status             - EFI_STATUS code for the error.

  @retval EFI_SUCCESS           - The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SendFspErrorInfoStatusCode (
  IN EFI_GUID   CallerId,
  IN EFI_GUID   ErrorType,
  IN EFI_STATUS Status
  );

/**
  Function attempts to dump all FSP error information hobs.

  @param[in] HobList            - Pointer to the HOB data structure.

  @retval EFI_SUCCESS           - No FSP_ERROR_INFO_HOB found.
  @retval EFI_DEVICE_ERROR      - At least one FSP_ERROR_INFO_HOB found.
**/
EFI_STATUS
EFIAPI
DumpFspErrorInfo (
  IN VOID *HobList
  );

/**
  ReportStatusCode worker for FSP Error Information.

  @param  CodeType         Always (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED)
  @param  Value            Always 0
  @param  Instance         Always 0
  @param  CallerId         This optional parameter may be used to identify the caller.
                           It may be used to identify which internal component of the FSP
                           was executing at the time of the error.
  @param  Data             This data contains FSP error type and status code.

  @retval EFI_SUCCESS      Show error status sent by FSP successfully.
  @retval RETURN_ABORTED   Function skipped as unrelated.

**/
EFI_STATUS
EFIAPI
FspErrorStatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE       CodeType,
  IN EFI_STATUS_CODE_VALUE      Value,
  IN UINT32                     Instance,
  IN CONST EFI_GUID             *CallerId,
  IN CONST EFI_STATUS_CODE_DATA *Data OPTIONAL
  );
#endif //_PEI_DXE_FSP_ERROR_INFO_LIB_H_
