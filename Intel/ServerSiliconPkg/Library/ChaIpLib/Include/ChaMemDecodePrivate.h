/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _CHA_MEM_ADDR_MAP_PRIVATE_H_
#define _CHA_MEM_ADDR_MAP_PRIVATE_H_

#include <Library/ChaLib.h>


/**

  For SPR, programs the durable_mem_en bit of the CHA DRAM_RULE_CFG2 registers for each DRAM rule.
  For ICX, it is an empty function.

  @param[in] SocketId                The socket number ID and 0 should always be the SBSP.
  @param[in] SadIndex                The index of SAD entry.
  @param[in] SadData                 Logic information for a SAD entry.

  @retval None

**/
VOID
ChaSetSadDataEx (
  IN UINT8     ScktId,
  IN UINT8     SadId,
  IN SAD_DATA  *SadData
  );

/**

  Programs the lock bit to unlock cluster Id.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.
  @param[in] ClusterId               The index of cluster to unlock.

  @retval None

**/
VOID
ChaUnlockCluster (
  IN UINT8     ScktId,
  IN UINT8     ClusterId
  );

/**

  Programs the lock bit to lock cluster Id.

  @param[in] SocketId                The socket ID and 0 should always be the SBSP.
  @param[in] ClusterId               The index of cluster to lock.

  @retval None

**/
VOID
ChaLockCluster (
  IN UINT8     ScktId,
  IN UINT8     ClusterId
  );

#endif  // #ifndef _CHA_MEM_ADDR_MAP_PRIVATE_H_
