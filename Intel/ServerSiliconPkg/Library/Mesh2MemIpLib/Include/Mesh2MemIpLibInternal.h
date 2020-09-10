/** @file
  Mesh2Mem IP implementation.

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

#ifndef _MESH_2_MEM_IP_LIB_INTERNAL_
#define _MESH_2_MEM_IP_LIB_INTERNAL_

/**

  Initialize topology information in M2Mem.

  @param[in] Socket         - Socket ID
  @param[in] McId           - Mc Controller Id
  @param[in] SncCluster     - SncCluster
  @param[in] BaseCluster1   - BaseCluster1

**/
VOID
InitM2mTopology (
  IN UINT8               Socket,
  IN UINT8               McId,
  IN UINT8               SncCluster,
  IN UINT32              BaseCluster1
  );

/**

  Set max number of DDRT writes allowed per channel store tracker.

  @param[in] Socket       - Socket ID
  @param[in] McId         - Mc Controller Id
  @param[in] DdrtThresh   - Max number of DDRT writes allowed per channel store tracker

**/
VOID
SetM2mWrTrKrAllocDdrtThresh (
  IN UINT8                         Socket,
  IN UINT8                         McId,
  IN UINT8                         DdrtThresh
  );

#endif //  _MESH_2_MEM_IP_LIB_INTERNAL_
