/** @file
  Mirror failover private
 
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

#ifndef __MIRROR_FAILOVER_PRIVATE_H__
#define __MIRROR_FAILOVER_PRIVATE_H__

#define NUM_ATTEMPTS 3

typedef enum {
  FullyMirrored,
  PartiallyMirrored,
  NoMirror
} CHANNEL_MIRROR_STATUS;

/**
 * DoHardwareAssistedMirrorFailover : In this method the silicon has already failed over a bad channel.
 * Bios is responsible to remove the ranks of this channel from Amap. Mark the mirrored TADs with
 * Also log this information!
 * @param[in] UINT8  Socket
 * @param[in] UINT8  Mc
 * @param[out] status : Return EFI_SUCCESS or EFI_FAILURE
 */
EFI_STATUS
DoHardwareAssistedMirrorFailover (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  OUT BOOLEAN *McFailover
  );

/**
 * DoBiosAssistedMirrorFailover : In this method the bios is resposible to determine the channel failed from the shadow registers. Bios
 * algorithm reads the value thrice to ensure itegrity and then programs this value into the mirrorfailover register under quiesce with the
 * assistance of pcode mailbox call
 * @param[in] UINT8  Socket
 * @param[in] UINT8  Mc
 * @param[out] McFailover : TRUE - Successful, FALSE - Failed
 *
 * @retval None
 */
VOID
DoBiosAssistedMirrorFailover (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  OUT BOOLEAN *McFailover
  );

/**
  This function enables  SMI generation of for Mirror failover.

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number
  @param[in]  FailedChBitMap  - One hot encoded value for failed channel

  @retval EFI_SUCCESS  if successful
          EFI_UNSUPPORTED if unsuccessful

**/
EFI_STATUS
SetTadBaseForMirrorFailover (
    UINT8 Socket,
    UINT8 Mc,
    UINT8 FailedChBitMap
    );

/**
  Is this channel fully mirrored or partially mirrored

  Note: corrected error threshold init is done by MRC

  @param[in]  socket    - Socket number
  @param[in]  Mc        - Mc number
  @param[in]  Ch        - Channel ID
  @param[out] MirrorStatus - Mirror Status FullyMirrored, PartiallyMirroed or No Mirror

  @retval None
**/
VOID
GetChannelMirrorStatus (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  OUT CHANNEL_MIRROR_STATUS *MirrorStatus
  );

#endif //__MIRROR_FAILOVER_PRIVATE_H__
