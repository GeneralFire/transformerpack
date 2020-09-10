/** @file
  SPS dynamic update library definitions.
  This library provides dymanic update to various SPS structures.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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
#ifndef _DXE_SPS_LIB_H_
#define _DXE_SPS_LIB_H_

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>

typedef
VOID
(EFIAPI *SPS_DXE_START_ACTION_CALLBACK) (
  IN EFI_EVENT      Event,
  IN VOID           *Context
  );

/**
  Function verifies dependency rules

  @param[in] Callback          StartAction callback function pointer.
                               If dependency is not satisfied event callback is set
                               to verify it again.
                               Context of the callback is EFI_STATUS pointer

  @retval TRUE                 All dependency elements has been found
  @retval FALSE                Not all dependency elements has been found
**/
BOOLEAN
VerifyDependencyRules (
  IN SPS_DXE_START_ACTION_CALLBACK  Callback
  );

/**
  Get Turbo information

  @retval TRUE    Turbo bit is enabled
  @retval FALSE   Turbo bit is disabled
**/
BOOLEAN
SpsGetTurboBit (
  VOID
  );

/**
  Get TStates information
  When TStates information is not available input data is returned

  @param[in] TStatesNumber    TStates number to return in error case

  @retval TStatesNumber       Number of TStatesNumber configured in the system
**/
UINT8
SpsGetTStatesNumber (
  IN UINT8 TStatesNumber
  );

/**
  Fill PStates information in the message table

  @param[out] PStatesNumber      Pointer to PStatesNumber
  @param[out] NPStatesTable      Pointer to NPStatesTable table
  @param[out] GPSstatesNumber    Pointer to GPSstatesNumber
  @param[out] GPStatesTable      Pointer to GPStatesTable table
  @param[in] Turbo               Turbo information

  @retval EFI_SUCCESS            PStates Tables are prepared
  @retval EFI_INVALID_PARAMETER  Passed pointers are invalid
  @retval EFI_NOT_FOUND          Information has been not found in the system
**/
EFI_STATUS
SpsFillStatesTable (
  OUT UINT8   *PStatesNumber,
  OUT UINT8   *NPStatesTable,
  OUT UINT8   *GPSstatesNumber,
  OUT UINT8   *GPStatesTable,
  IN UINT8    Turbo
  );

/**
  Get Cores and threads information

  @param[out] ProcCoresEnabled   Pointer to return cores information
  @param[out] ProcThreadsEnabled Pointer to return threads information

  @retval EFI_SUCCESS            Cores and threads information has been prepared
  @retval EFI_INVALID_PARAMETER  Passed pointers are invalid
  @retval EFI_NOT_FOUND          Information has been not found in the system
**/
EFI_STATUS
SpsGetCoresAndTreads (
  OUT UINT16    *ProcCoresEnabled,
  OUT UINT16    *ProcThreadsEnabled
  );

/**
  Check if Hardware Power Management may be configured
  in OOB or Native w/o Legacy

  @param[out] ProcNumber   Pointer to processorNumber information

  @retval TRUE             HWP is configured in OOB or Native w/o Legacy
  @retval FALSE            HWP is not configured in OOB or Native w/o Legacy
**/
BOOLEAN
SpsIsLegacyOob (
  OUT UINT16    *ProcNumber
  );

#endif // _DXE_SPS_LIB_H_
