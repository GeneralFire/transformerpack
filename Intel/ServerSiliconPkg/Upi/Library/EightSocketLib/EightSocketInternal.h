/** @file
  This is internal header file used by EightSocketLib. Interfaces in this file are only used in this library

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

#ifndef _KTI_EIGHT_SOCKET_INTERNAL_H_
#define _KTI_EIGHT_SOCKET_INTERNAL_H_

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
  );

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
  );

/*
  Routine to check and handle 8S2Clump topology. (Clump means 4S Fully Connect)

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval                  - TRUE:  8S2Clump Topology degradation is done.
                           - FALSE: Need to further check and handle for this topology.
*/
BOOLEAN
EFIAPI
CheckEightSocketTwoClumpTopology (
  IN  KTI_SOCKET_DATA           *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  );

#endif // _KTI_EIGHT_SOCKET_INTERNAL_H_
