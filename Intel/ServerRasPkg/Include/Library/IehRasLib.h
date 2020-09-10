/** @file
  Interface of IEH RAS library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef __IEH_RAS_LIB_H__
#define __IEH_RAS_LIB_H__

/**
  This routine returns whether IioRas Device exist.
  For 10nm will check whether Global Ieh exist.
  For CPX just always return true because UBOX device always there.

  @param -- none
  @retval   TRUE  -- Iio device exist; FALSE -- Iio device not exist

**/
BOOLEAN
EFIAPI
IioRasDeviceExist (
  VOID
  );

/**
  This routine purpose is to enable IEH error handling for whole system.

  @param   none
  @retval  none
**/
VOID
EFIAPI
IehErrorEnable (
  VOID
  );

/**
  This routine purpose is to handle IEH error for IOMCA.
  This routine can be used to MCA driver for IOMCA handling.

  @param   NONE

  @retval  none
**/
VOID
EFIAPI
IOMCAErrorhandler (
  VOID
  );

/**
  Iio error enter pointer for non-IOMCA

  @param[in]      DispatchHandle  The unique handle assigned to this handler.
  @param[in]      Context         Standard SMI handler parameter; not used here.
  @param[in, out] CommBuffer      Standard SMI handler parameter; not used here.
  @param[in, out] CommBufferSize  Standard SMI handler parameter; not used here.

  @retval EFI_SUCCESS -- error handled.
**/
EFI_STATUS
EFIAPI
IioErrorDetectAndHandle (
  IN EFI_HANDLE             DispatchHandle,
  IN CONST VOID            *DispatchContext,
  IN OUT VOID              *CommBuffer,
  IN OUT UINTN             *CommBufferSize
  );

/**
  This function will enable Iio error reporting.

  @param[in] Protocol        Protocol unique ID.
  @param[in] Interface            Interface instance.
  @param[in] Handle      The handle on which the interface is installed..

  @retval    Status.
**/
EFI_STATUS
EFIAPI
IioErrorEnableCallBack (
  IN      CONST EFI_GUID   *Protocol,
  IN      VOID             *Interface,
  IN      EFI_HANDLE        Handle
  );
#endif
