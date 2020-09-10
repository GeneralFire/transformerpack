/** @file
  Mirror Failover library.

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

#ifndef __MIRRORFAILOVER_LIB_H__
#define __MIRRORFAILOVER_LIB_H__

/**
  Get VMSE error0 log bit.

  Note: corrected error threshold init is done by MRC

  @param[in]  socket    - socket number
  @param[in]  Ch        - Channel ID

  @retval None
**/
BOOLEAN
EFIAPI
GetVmseErr0Sts (
  IN        UINT8   Socket,
  IN        UINT8   Mc
  );


/**
  Clears VMSE error0 log bit.

  Note: corrected error threshold init is done by MRC

  @param[in]  socket    - socket number
  @param[in]  Ch        - Channel ID

  @retval None
**/
VOID
EFIAPI
ClearVmseErr0Sts (
  IN        UINT8   Socket,
  IN        UINT8   Mc
  );


/**
  This function enables the SMI signaling when link goes in link fail state.

  @retval EFI_SUCCESS if the call is succeed.

**/
VOID
EFIAPI
EnableVmseErr0Error (
  UINT8 Socket
  );

/**
  This function enables  SMI generation of for Mirror failover.

  @param[in]  Node      - Node to init

  @retval None

**/
VOID
EnableIntForMirrorFailover (
  IN        UINT8   Socket,
  IN        UINT8   EmcaCsmiEn
  );


/**
 * MirrorFailoverHandler : Handle a mirror scrub fail event on a channel
 * @param[in]   UINT8   Socket
 * @param[in]   UINT8   EmcaCsmiEn
 * @param[out]  EFI_STATUS MirrorFailoverStatus : Status EFI_SUCCESS if successful
 */
EFI_STATUS
MirrorFailoverHandler (
    IN        UINT8   Socket,
    IN        UINT8   EmcaCsmiEn
    );


#endif //__MIRRORFAILOVER_LIB_H__
