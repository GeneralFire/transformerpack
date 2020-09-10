/** @file
  Library for Dual BIOS Watchdog for Dxe modules.

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

#ifndef _ME_WATCHDOG_CONTROL_LIB_H
#define _ME_WATCHDOG_CONTROL_LIB_H

#include "DualBiosMsgs.h"
#include <Protocol/HeciControlProtocol.h>


#define SMM_SPS_WATCHDOG_COMMUNICATE_HEADER_SIZE OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)

#pragma pack (1)
typedef struct {
  UINT8           ReturnStatus;
  EFI_STATUS      Status;
  UINT8           ActionFlag;
} SPS_SMM_WATCHDOG_HEADER;
#pragma pack ()

/**
  Initialize the Communicate Buffer and send data to SMM

  @param[in, out]  pHeaderBuffer The pointer to the header structure

  @retval EFI_SUCCESS            The constructor always returns EFI_SUCCESS.
  @retval EFI_INVALID_PARAMETER  Some parameter is NULL

**/
EFI_STATUS
MeWatchdogControlNotifySmm (
  IN OUT VOID  *pHeaderBuffer
  );

/**
  Checks whether ME Watchdog Control is enabled or not.

  @returns True is returned if ME is functional and supports Watchdog
           functionality, otherwise false.
 **/
BOOLEAN
EFIAPI
MeWatchdogControlIsEnabed (
  VOID
  );

/**
  Procedure to send BiosUpdateGetInventory HECI Request

  @param[in]      pHeci           pointer to HECI driver
  @param[in]      ImageId         identifier of region to check
  @param[in,out]  ImageInventory  buffer for Image Inventory from HECI response

  @return EFI_SUCCESS            Returned if success.
  @return EFI_INVALID_PARAMETER  Some parameter is wrong.
  @return EFI_UNSUPPORTED        If not SPS or unsuccess response from SPS.
*/
EFI_STATUS
HeciReq_GetInventory (
  IN     HECI_CONTROL     *pHeci,
  IN     UINT8            ImageId,
  IN OUT IMAGE_INVENTORY  *ImageInventory
  );

/**
  Function returns number of the current used bios region.

  @param[in]   pHeci             pointer to HECI driver
  @param[out]  Region            Buffer for the response - number of the current used region.
                                 Proper region number can be equal only to 0 or 1.
  @return EFI_SUCCESS            Returned if success.
  @return EFI_INVALID_PARAMETER  Some parameter is wrong.
  @return EFI_DEVICE_ERROR       Can't recognize which bios region is running and active.
*/
EFI_STATUS
GetActiveRegionNum (
  IN  HECI_CONTROL   *pHeci,
  OUT UINT8          *Region
  );

#endif // _ME_WATCHDOG_CONTROL_LIB_H
