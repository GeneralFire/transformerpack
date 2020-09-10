/** @file
  Interface of Ras runtime status library.

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

#ifndef __RAS_RUNTIME_STATUS_LIB_H__
#define __RAS_RUNTIME_STATUS_LIB_H__

typedef struct {
  UINT8 FailedCh[MAX_SOCKET][MAX_IMC];
} MIRROR_FAILOVER_STATUS;

typedef struct {
  MIRROR_FAILOVER_STATUS  Failover;
} RAS_RUNTIME_STATUS;

/**

  Save Mirror failover status

  @param Socket         socket id
  @param Mc             memory controller id
  @param FailedCh       failed channel in Mc

  @return NONE

**/
VOID
EFIAPI
SaveMirrorFailoverFailedChannel (
  IN  UINT8          Socket,
  IN  UINT8          Mc,
  IN  UINT8          FailedCh
  );

/**

  Get Mirror failover status

  @param Socket         socket id
  @param Mc             memory controller id

  @return the failed channel in Mc, 0xff if no failed channel

**/
UINT8
EFIAPI
GetMirrorFailoverFailedChannel (
  IN  UINT8          Socket,
  IN  UINT8          Mc
  );
#endif
