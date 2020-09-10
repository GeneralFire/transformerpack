/** @file
  Data format for Socket Data Structure

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#ifndef   __SOCKET_CONFIG_DATA_H__
#define   __SOCKET_CONFIG_DATA_H__

#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"
#include <Guid/SocketIioVariable.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Guid/SocketPowermanagementVariable.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Guid/SocketMpLinkVariable.h>
#include <Guid/SocketMemoryVariable.h>

#pragma pack(1)

typedef struct {
  SOCKET_IIO_CONFIGURATION       IioConfig;
  SOCKET_COMMONRC_CONFIGURATION  CommonRcConfig;
  SOCKET_MP_LINK_CONFIGURATION   UpiConfig;
  SOCKET_MEMORY_CONFIGURATION    MemoryConfig;
  SOCKET_POWERMANAGEMENT_CONFIGURATION PowerManagementConfig;
  SOCKET_PROCESSORCORE_CONFIGURATION   SocketProcessorCoreConfiguration;
} SOCKET_CONFIGURATION;



#pragma pack()
#endif

