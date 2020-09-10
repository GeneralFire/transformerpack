/** @file
  The System Topology Library provides functionality needed to interpret SoC
  topology bytestreams.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __SYSTEM_TOPOLOGY_LIB_H__
#define __SYSTEM_TOPOLOGY_LIB_H__

/**
  Parses SoC data blob and populates topology structure.
  This is the starting point for RAT based register access,
  as without topology data no addresses can be resolved.

  @param[in] SocketCount          Count of populated sockets in platform
  @param[in] SocData              Pointer to map file bytestream
  @param[in] IndexLevel           Index compression level of the map file
  @param[in] Chunks               Pointer to the map file chunks

  @retval EFI_SUCCESS           - SoC topology data populated
  @retval EFI_OUT_OF_RESOURCES  - Could not allocate HOB
**/
EFI_STATUS
EFIAPI
InstallTopologyData (
  IN UINT8  SocketCount,
  IN UINT8  *SocData,
  IN UINT8  IndexLevel,
  IN UINT8  *Chunks
  );

#endif // #ifndef __SYSTEM_TOPOLOGY_LIB_H__
