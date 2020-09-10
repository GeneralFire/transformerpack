/** @file
  BaseTraceHubInfoFruLib Library header file

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

@par Specification
**/
#ifndef _TRAC_HUB_INFO_FRU_LIB_H_
#define _TRAC_HUB_INFO_FRU_LIB_H_

/*
  Check if CPU TraceHub support

  @retval TRUE   Cpu TraceHub is supported
  @retval FALSE  Cpu TraceHub is not supported
*/
BOOLEAN
IsCpuTraceHubSupport (
  VOID
  );

/*
  Check if TSCU programming support for CPU TraceHub

  @retval TRUE   TSCU programming is supported
  @retval FALSE  TSCU programming is not supported
*/
BOOLEAN
IsTscuProgramSupport (
  VOID
  );

/*
  Check if PMTCS initialization for CPU TraceHub

  @retval TRUE   PMTCS initialization is supported
  @retval FALSE  PMTCS initialization is not supported
*/
BOOLEAN
IsPmtcsInitSupport (
  VOID
  );

/*
  Check if Secure Boot configuration for trace to memory (short term) is applied

  @retval TRUE   Secure Boot configuration is applied
  @retval FALSE  Secure Boot configuration is not applied
*/
BOOLEAN
IsSecureBootConfigApplied (
  VOID
  );
#endif
