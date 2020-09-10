/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <UncoreCommonIncludes.h>
#include <Upi/KtiHost.h>

/**
  Reduces the degraded 8S topology to 6S topology if possible. Otherwise it will reduced using
  degraded 4S handling rules.

  This function should be called only when the system has degraded 4S topology.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval 0 - Successful

**/
KTI_STATUS
CheckForDegraded8S (
  IN KTI_SOCKET_DATA           *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
ComputeTopologyTree8S (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  return KTI_SUCCESS;
}

KTI_STATUS
ComputeTopologyTree6S (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  return KTI_SUCCESS;
}

/**
  Computes the topology tree for 8S 2Clumps topology.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param DiscEngData       - Pointer to socket specific data

  @retval KTI_SUCCESS      - Success
  @retval KTI_FAILURE      - Failure
**/
KTI_STATUS
ComputeTopologyTree8S2Clumps (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  KTI_DISC_DATA            *DiscEngData
  )
{
  return KTI_SUCCESS;
}
