/** @file

  Topology degrade implementation for sockets that have a maximum of 3 or 4 UPI links.

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

#include <KtiSetupDefinitions.h>
#include <Library/UncoreLib.h>
#include "EightSocketTopoDegradeSupport.h"

/**
  Reduces the degraded 8S topology to 6S topology if possible. Otherwise it will reduced using
  degraded 4S handling rules.

  This function should be called only when the system has degraded 8S topology.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
HandleDegraded8S (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  KTI_STATUS              Status;
  KTI_DISC_DATA           TmpDiscEngData;

  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE){
    KTIVAR->SysConfig = SYS_CONFIG_8S;             //hard code force to 8s
    return KTI_SUCCESS;
  }

  if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_AS_IS) {
    KTIVAR->SysConfig = SYS_CONFIG_8S;
    return KTI_SUCCESS;
  } else if (KTISETUP->DfxParm.DfxSystemDegradeMode == DEGRADE_TOPOLOGY_1S) {
    DegradeTo1S (SocketData, KtiInternalGlobal);
    KTIVAR->SysConfig = SYS_CONFIG_1S;
    return KTI_SUCCESS;
  }

  Status = CanItBeDegradedTo6S (SocketData, KtiInternalGlobal, &TmpDiscEngData);
  if (Status != KTI_SUCCESS) {
    HandleDegraded4S (SocketData, KtiInternalGlobal, &TmpDiscEngData);
  }

  return KTI_SUCCESS;
}

/**
  Check the topology type for 8S configuration.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS

**/
KTI_STATUS
CheckTopologyType (
  KTI_SOCKET_DATA           *SocketData,
  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  return KTI_SUCCESS;
}
