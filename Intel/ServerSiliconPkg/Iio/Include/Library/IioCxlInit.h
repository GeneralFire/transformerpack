/** @file
  IIO CXL.IO initialization header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _IIOCXLIOINIT_H_
#define _IIOCXLIOINIT_H_

/**
  Per CXL.IO port initialization before link training.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO

**/
VOID
CxlIoPortPreTrainInit (
  IN IIO_GLOBALS                 *IioGlobalData,
  IN UINT8                       IioIndex
  );

/**

  Per CXL.IO port initialization.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
CxlIoPortInit (
  IN IIO_GLOBALS                        *IioGlobalData,
  IN UINT8                              IioIndex
  );

/**
  Enable clock gating for ARB/MUX

  @param[in] IioGlobalData    Pointer to IIO_GLOBALS
  @param[in] IioIndex         Index to Socket

  @retval IIO_SUCCESS Initialization completed without problems
--*/
VOID
EFIAPI
ArbMuxClockGatingEn (
  IN OUT IIO_GLOBALS                 *IioGlobalData,
  IN UINT8                           IioIndex
  );

/**

  FBLP Pre Train initialization.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
FblpPreTrainInit (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

/**

  FBLP Post Train initialization.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
FblpPostTrainInit (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  );

#endif /* _IIOCXLIOINIT_H_*/
