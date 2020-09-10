/** @file

  MemVrSvidMapLib.h

  API Header for VrSvid Mapping

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

#ifndef _MEM_VR_SVID_MAP_LIB_H_
#define _MEM_VR_SVID_MAP_LIB_H_

#define MAX_SVID_MC     16
#define MAX_SVID_SOCKET 16

typedef struct {
    UINT8 Mc[MAX_SVID_MC];  // Store SVID for Socket\MC Pair
} SVID_SOCKET;

typedef struct {
  SVID_SOCKET Socket[MAX_SVID_SOCKET];
} MEM_SVID_MAP;


/**
  Get SVID Mapping from Socket and MCID

  @param[in] Socket    - Socket Id - 0 based
  @param[in] McId      - Memory controller 0 based
  @param[in] SvidValue - SVID Value

  @retval EFI_SUCCESS   - Value found
  @retval EFI_NOT_FOUND - Value not found

**/
EFI_STATUS
EFIAPI
GetSvidMap (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 *SvidValue
  );

/**
  Set SVID Mapping for given Socket and MCID

  @param[in] Socket    - Socket Id - 0 based
  @param[in] McId      - Memory controller 0 based
  @param[in] SvidValue - SVID Value

  @retval EFI_SUCCESS   - Value set successfully
  @retval EFI_NOT_FOUND - Value not set

**/
EFI_STATUS
EFIAPI
SetSvidMap (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 SvidValue
  );

#endif //_MEM_VR_SVID_MAP_LIB_H_
