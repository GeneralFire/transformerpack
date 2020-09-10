/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _KTI_TOPOLOGY_DEGRADE_SUPPORT_H_
#define _KTI_TOPOLOGY_DEGRADE_SUPPORT_H_

#define TOPOLOGY_TYPE_8S_DPW    0
#define TOPOLOGY_TYPE_8S_AFC    1

/**
  Try to discover and degrade to a best 6S topology that include the legacy socket.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval KTI_SUCCESS      - Already degraded to 6S topology.
  @retval KTI_FAILURE      - Can not degrade to 6S topology.

**/
KTI_STATUS
CanItBeDegradedTo6S (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  KTI_DISC_DATA             *DiscEngData
  );

#endif // _KTI_TOPOLOGY_DEGRADE_SUPPORT_H_
