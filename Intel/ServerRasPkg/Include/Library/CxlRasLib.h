/** @file
  Interface of CxlRasLib Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef __CXL_RAS_H__
#define __CXL_RAS_H__

/**
  Dump the Errors of CXL DP and UP
  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number

  @retval none.

**/
VOID
DumpCxlDpUpRegisters (
  IN      UINT8    Socket,
  IN      UINT8    Stack
  );

/**
  This function is to get the secondary and Subordinate bus number of CXL.DP

  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number
  @param[in] SecondaryBus         Secondary Bus Number of CXL.DP
  @param[in] SubordinateBus      Subordinate Bus Number of CXL.DP

  @retval    None
--*/
VOID
EFIAPI
CxlRasGetCxlDevBusRange (
  IN      UINT8    Socket,
  IN      UINT8    Stack,
  OUT     UINT8   *SecondaryBus,
  OUT     UINT8   *SubordinateBus
  );

/**
  This function initializes and enables error reporting in CXL DP and UP

  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number

  @retval    None
--*/
VOID
EFIAPI
CxlRasConfigDpUp (
  IN      UINT8    Socket,
  IN      UINT8    Stack
  );

/**
  Compute eXpress Link error handler for DP and UP

  @param[in] Socket               Socket Number
  @param[in] Stack                Stack  Number
  @param[in] SevPerStack          Error Severity to handle

  @retval    None
--*/
VOID
EFIAPI
CxlDpUpErrorHandler (
  IN      UINT8    Socket,
  IN      UINT8    Stack,
  IN      UINT8    SevPerStack
  );
#endif
