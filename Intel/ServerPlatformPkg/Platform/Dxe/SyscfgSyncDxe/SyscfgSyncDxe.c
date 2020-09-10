/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2017 Intel Corporation. <BR>

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

#include "SyscfgSyncDxe.h"


VOID
PcieSlotOpromBitMapInit(
  VOID
  )
{
  EFI_STATUS                 Status;
  UINT32 PcieSlotOpromBitMap;
  SOCKET_IIO_CONFIGURATION   mSocketIioConfiguration;
  Status = GetSpecificConfigGuid (&gEfiSocketIioVariableGuid, (VOID *) &mSocketIioConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Can't get PcieSlotOpromBitMap from SETUP!\n" ));
    mSocketIioConfiguration.PcieSlotOprom1 = 1;
    mSocketIioConfiguration.PcieSlotOprom2 = 1;
    mSocketIioConfiguration.PcieSlotOprom3 = 1;
    mSocketIioConfiguration.PcieSlotOprom4 = 1;
    mSocketIioConfiguration.PcieSlotOprom5 = 1;
    mSocketIioConfiguration.PcieSlotOprom6 = 1;
    mSocketIioConfiguration.PcieSlotOprom7 = 1;
    mSocketIioConfiguration.PcieSlotOprom8 = 1;
  }

  // Initalize Pcie Slot Oprom BitMap and store in PCD.
  // The data source is from mSocketIioConfiguration
  // This part could be customized.
  PcieSlotOpromBitMap = 0;
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom1) << 0);
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom2) << 1);
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom3) << 2);
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom4) << 3);
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom5) << 4);
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom6) << 5);
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom7) << 6);
  PcieSlotOpromBitMap |= ((mSocketIioConfiguration.PcieSlotOprom7) << 7);

  DEBUG((DEBUG_INFO, "PcieSlotOpromBitMap : 0x%X\n", PcieSlotOpromBitMap ));
  Status = PcdSet32S(PcdOemSkuPcieSlotOpromBitMap, PcieSlotOpromBitMap);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return;
  }
  DEBUG((DEBUG_INFO, "PcdOemSkuPcieSlotOpromBitMap : 0x%X\n", PcdGet32(PcdOemSkuPcieSlotOpromBitMap)));
  return;
}


/**

  Main Entry point of the SyscfgSyncDxe Driver.

  @param ImageHandle  -  Handle to the image.
  @param SystemTable  -  Handle to System Table.

  @retval EFI_STATUS  -  Status of the function calling.

**/
EFI_STATUS
EFIAPI
SyscfgSyncDxeDriverEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{

  PcieSlotOpromBitMapInit();
  return EFI_SUCCESS;
}
