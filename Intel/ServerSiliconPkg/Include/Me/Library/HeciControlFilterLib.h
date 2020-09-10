/** @file
 Definitions for HECI Control Filter functionality driver

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/
#ifndef _HECI_CONTROL_FILTER_LIB_H_
#define _HECI_CONTROL_FILTER_LIB_H_

/**
  HECI Control filter procedure specific to SPS provided by User

  This function returns HECI function number that message has been provided to

  @param[in]     This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 1..N
                               0 Id is default and
                               HeciControl decide where to route communication
  @param[in,out] HeciIn        Pointer to default HECI provided by HeciControl that will be filtered and returned
  @param[out]    HostAddress   Host address
  @param[out]    EngineAddress Engine (ME/IE) address
  @param[out]    MsgBody       Body of the message to send
  @param[in,out] MsgLen        On input buffer size, on exit message body size in bytes

  @retval EFI_SUCCESS            Success
  @retval EFI_INVALID_PARAMETERS Bad input parameters
  @retval EFI_DEVICE_ERROR       Unknown ME Type
**/
EFI_STATUS
FilterHeciMessage (
  IN     UINT32               HeciPchId,
  IN OUT UINTN                *HeciIn,
  IN     UINT8                HostAddress,
  IN     UINT8                EngineAddress,
  IN     UINT32               *MessageBody,
  IN     UINT32               MessageLength
  );

/**
  HECI Control Initialization filter procedure

  This function returns if HECI interface is allowed for initialization

  @param[in]    HeciTransport  HECI device handle identifies device function will work on

  @retval TRUE                 Initialization allowed
  @retval FALSE           `    Initialization disallowed
**/
BOOLEAN
FilterHeciInitialization (
  IN     HECI_TRANSPORT       *HeciTransport
  );

#endif // _HECI_CONTROL_FILTER_LIB_H_
