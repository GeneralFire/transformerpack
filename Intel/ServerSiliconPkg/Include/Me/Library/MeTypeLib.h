/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation.

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

#ifndef _ME_TYPE_LIB_H_
#define _ME_TYPE_LIB_H_

#include <Uefi.h>
#include "MeTypeDefs.h"

///
/// Define all the types of CSME firmware
///
typedef enum {
  MeTypeSps      = METYPE_SPS,
  MeTypeSpsEpo   = METYPE_SPS_EPO,
  MeTypeClient   = METYPE_CLIENT,
  MeTypeDfx      = METYPE_DFX,
  MeTypeUndef    = METYPE_UNDEF,
  MeTypeDisabled = METYPE_DISABLED
} ME_TYPE;

/**
  Get detected Me FW type

  @retval MeType
**/
ME_TYPE
MeTypeGet (
  VOID
  );

/**
  Checks if Me FW is DFX type

  @retval FALSE           No Dfx ME on board
  @retval TRUE            Dfx ME detected
**/
BOOLEAN
MeTypeIsDfx (
  VOID
  );

/**
  Checks if Me FW is DISABLED type

  @retval FALSE           ME is not disabled
  @retval TRUE            ME is disabled
**/
BOOLEAN
MeTypeIsDisabled (
  VOID
  );

/**
  Checks if Me FW is SPS type

  @retval FALSE           No SPS on board
  @retval TRUE            SPS detected
**/
BOOLEAN
MeTypeIsSps (
  VOID
  );

/**
  Checks if Me FW is SPS Ignition type

  @retval FALSE           No SPS Ignition on board
  @retval TRUE            SPS Ignition detected
**/
BOOLEAN
MeTypeIsSpsIgnition (
  VOID
  );

/**
  Checks if Me FW is Client type

  @retval FALSE           No Client ME on board
  @retval TRUE            Client ME detected
**/
BOOLEAN
MeTypeIsClient (
  VOID
  );

/**
  Print the the ME FW Type and Subtype.
**/
VOID
EFIAPI
PrintMeType (
  IN CHAR16 *DispString
  );

/**
  Displays debug information about detected Me FW type

  @param[in] Leading debug string
**/
VOID
MeTypeShowDebug (
  IN     CHAR16 *dispString
  );

/**
  Get ME FW type and subtype from HOB.

  @param[out] FwType     ME FW type
  @param[out] FwSubType  ME FW subtype

  @retval EFI_SUCCESS            Success
  @retval EFI_UNSUPPORTED        Not supported
  @retval EFI_INVALID_PARAMETER  Both parameters are NULL
  @retval EFI_NOT_FOUND          Cannot get MeType HOB
**/
EFI_STATUS
GetMeFwTypeSubTypeFromHob (
     OUT UINT8 *FwType,
     OUT UINT8 *FwSubType
  );

#endif // _ME_TYPE_LIB_H_
