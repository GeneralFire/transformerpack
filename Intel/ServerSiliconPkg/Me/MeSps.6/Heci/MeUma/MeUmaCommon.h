/** @file
  MeUma internal definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _ME_UMA_COMMON_H_
#define _ME_UMA_COMMON_H_

#include <Ppi/MeUmaPpi.h>

/**
  This checks if ME UMA is enabled

  @param[in] MeUmaPrefix  Pointer to prefix

  @retval TRUE            ME UMA is enabled
  @retval FALSE           ME UMA is disabled
**/
BOOLEAN
EFIAPI
MeUmaInternalIsEnabled (
  IN CONST CHAR8  *MeUmaPrefix
  );

/**
  Updates the ME UMA Size Info in the ME UMA PPI

  @param[in] MeUmaPrefix    Pointer to prefix
  @param[in] MeUmaSize      ME UMA Size requested by the ME FW
  @param[in] MeUmaSizeError ME UMA Size request error (non 0 value indicates error)

  @retval EFI_SUCCESS       The ME UMA size info is updated.
  @retval EFI_DEVICE_ERROR  ME UMA Info data pointer invalid
**/
EFI_STATUS
EFIAPI
MeUmaUpdatePpiMeUmaSizeInfo (
  IN CONST CHAR8  *MeUmaPrefix,
  IN UINT32       MeUmaSize,
  IN UINT8        MeUmaSizeError
  );

/**
  This procedure will enforce the BIOS Action that was requested by ME FW
  as part of the DRAM Init Done message.

  @param[in] MeUmaPrefix          Pointer to prefix
  @param[in] BiosAction           Me requests BIOS to act

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MeUmaInternalHandleBiosAction (
  IN CONST CHAR8      *MeUmaPrefix,
  IN UINT8            BiosAction
  );

/**
  Get pointer to ME_UMA_INFO stored in a HOB

  @retval ME_UMA_INFO*    Pointer to ME_UMA_INFO
  @retval NULL            Error, HOB cant be located
**/
ME_UMA_INFO*
EFIAPI
MeUmaGetInfo (
  VOID
  );

/**
  This procedure will enforce the BIOS Action that was requested by ME FW
  as part of the DRAM Init Done message.

  @param[in] BiosAction           Me requests BIOS to act

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
MeUmaHandleBiosAction  (
  UINT8               BiosAction
  );

/**
  Entry point for ME UMA Pre-Mem.

  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] PeiServices          Pointer to the PEI services table

  @retval Return Status based on errors that occurred while waiting for time to expire.
**/
EFI_STATUS
EFIAPI
MeUmaPreMemEntry (
  IN EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  );

/**
  Entry point for ME UMA Post-Mem.

  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] PeiServices          Pointer to the PEI services table

  @retval Return Status based on errors that occurred while waiting for time to expire.
**/
EFI_STATUS
EFIAPI
MeUmaPostMemEntry (
  IN EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  );

#endif // _ME_UMA_COMMON_H_
