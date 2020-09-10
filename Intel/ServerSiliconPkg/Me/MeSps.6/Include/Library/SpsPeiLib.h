/** @file
  Definitions for Sps Pei Library

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

#ifndef _SPS_PEI_LIB_H
#define _SPS_PEI_LIB_H

// Available clocking modes
typedef enum _CLOCKING_MODES {
  InternalStandard        = 0,
  InternalAlternate,
  HybridStandard,
  HybridAlternate,
  External
} CLOCKING_MODES;

/**
  Get current clocking mode via HECI.
  In case non SPS ME on board read it from soft straps

  @param[out] ClockingMode        Clocking mode read

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid pointer
  @retval EFI_NOT_READY           Function called before DID
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         Unsupported Clocking Mode has been returned
**/
EFI_STATUS
PeiGetCurrenClockingMode (
   OUT CLOCKING_MODES *ClockingMode
  );

/**
  Function sets SSC on/off

  @param[in] UseAlternate         Set Alternate SSC
                                  TRUE:  turn off SSC for BCLK PLL
                                  FALSE: turn on SSC 0.5% downspread for BCLK PLL

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid pointer
  @retval EFI_NOT_READY           Function called before DID
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         Unsupported Clocking Mode has been returned
**/
EFI_STATUS
PeiHeciSetSSCAlternate (
   IN BOOLEAN UseAlternate
  );

/**
  Get current clocking mode via HECI.
  In case non SPS ME on board read it from soft straps

  @param[out] ClockingMode        Clocking mode read

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid pointer
  @retval EFI_NOT_READY           Function called before DID
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         Unsupported Clocking Mode has been returned
**/
EFI_STATUS
PeiGetCurrenClockingMode (
   OUT CLOCKING_MODES *ClockingMode
  );

/**
  Return information if SPS supports ICC feature

  The function sends SPS_CMD_GET_MEBIOS_INTERFACE message to SPS FW and basing on
  the received message determines if ICC is supported or not.

  @retval TRUE   ICC is supported by SPS
  @retval FALSE  ICC isn't supported by SPS or a HECI communication error occurred

**/
BOOLEAN
SpsIsIccSupported (
  VOID
  );

/**
  Return information if SPS supports FiaMux feature

  The function sends SPS_CMD_GET_MEBIOS_INTERFACE message to SPS FW and basing on
  the received message determines if FiaMux is supported or not.

  @retval TRUE   FiaMux is supported by SPS
  @retval FALSE  FiaMux isn't supported by SPS or a HECI communication error occurred

**/
BOOLEAN
SpsIsFiaMuxSupported (
  VOID
  );

#endif // _SPS_PEI_LIB_H
