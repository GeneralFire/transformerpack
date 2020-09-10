/** @file
  Interface header file for the PPR CMOS library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _PPR_CMOS_LIB_H_
#define _PPR_CMOS_LIB_H_

/**
  Function to check if we already consumed the PPR Data
  Mapping of Bits as follows, Bit0 -> Socket0..Bit7 -> Socket7
  Accesses CMOS with IO operation on Multithreaded environment, so
  added semaphore to avoid race condtion

  @param[in] Socket - 0 Based Socket number

  @retval TRUE   -  PPR Data has been consumed
  @retval FALSE  -  PPR DATA has not been consumed
**/
BOOLEAN
EFIAPI
IsPprDataConsumed (
  IN UINT8  Socket
  );

/**
  Function to indicate PPR data is consumed
  Mapping of Bits as follows, Bit0 -> Socket0..Bit7 -> Socket7
  Accesses CMOS with IO operation on Multithreaded environment, so
  added semaphore to avoid race condtion

  @param[in] Socket - 0 Based Socket number

  @retval None
**/
VOID
EFIAPI
SetPprDataConsumed (
  IN UINT8  Socket
  );

/**
  Clear PPR CMOS Byte to 0, for first boot
**/
VOID
EFIAPI
InitalizePprCmosStatus (
  VOID
  );

/**
  Function to indicate fresh PPR data may be available
**/
VOID
EFIAPI
ResetPprDataConsumed (
  VOID
  );

/**
  Function to Get PPR CMOS Status byte

  @retval UINT8  - CMOS Value of PPR Status
**/
UINT8
EFIAPI
GetPprCmosStatus (
  VOID
  );

#endif  // _PPR_CMOS_LIB_H_
