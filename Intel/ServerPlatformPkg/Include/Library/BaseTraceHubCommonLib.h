/** @file
  TraceHubCommonLib Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2017 Intel Corporation. <BR>

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

#ifndef _BASE_TRACE_HUB_COMMON_LIB_H_
#define _BASE_TRACE_HUB_COMMON_LIB_H_

/**
  Calculate TraceHub Trace Address.

  @param [in] Thread          Master is being used.
  @param [in] Channel         Channel is being used.
  @param [out] TraceAddress   TraceHub Address for the Master/Channel
                              is being used.

  @retval                     RETURN_SUCCESS      Retrieve correct Address.
                              RETURN_UNSUPPORTED  Not implemented.
                              Other               Fail to Retrieve correct
                                                  TraceHub Address.

**/
RETURN_STATUS
EFIAPI
TraceHubMmioTraceAddress (
  IN  UINT16    Master,
  IN  UINT16    Channel,
  OUT UINT32    *TraceAddress
  );

#endif // _BASE_TRACE_HUB_COMMON_LIB_H_
