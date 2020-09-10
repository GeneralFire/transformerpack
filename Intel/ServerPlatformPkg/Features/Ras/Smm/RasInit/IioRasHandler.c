/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2019 Intel Corporation. <BR>

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

#include "RasInit.h"

extern BOOLEAN                 mExitImmediately;
EFI_CPU_RAS_PROTOCOL           *mCpuRas;
EFI_IIO_RAS_PROTOCOL           *mIioRasProtocol;
extern  UINT8                  mKtiCpuSktHotPlugEn;
extern  BOOLEAN                mIsSoftSmiRequest;
extern  UINT32                 mSmiParam[4];

EFI_STATUS
IioRasHandler(
  VOID
  )
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;

  if (mIsSoftSmiRequest && ((mSmiParam[0] != SMI_CPU_ONLINE_OFFLINE) && (mSmiParam[0] != SMI_CPU_HOT_ADD_REMOVE))) {
    return NO_REQUEST;
  }
  //
  // Check the setup option for QpiCpuSktHotPlugEn.  If it is not enabled, don't servie the Online/offline request
  //
  if (mKtiCpuSktHotPlugEn != 1) {
    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nIIO RAS HANDLER: PLEASE ENABLE CPU HOTPLUG IN QPI SETUP TO PROCEED FURTHER WITH SKT ONLINE/OFFLINE\n"));
    return NO_REQUEST;
  }

  //Process the request
  if (mCpuRas->RASEvent == CpuRasOnlineRequest) {
    DEBUG     ((EFI_D_INFO | EFI_D_ERROR, "\n\n  IIO %d Online Detected.\n", mCpuRas->SocketToBeOl));
  Status = mIioRasProtocol->IioOnline(mCpuRas->SocketToBeOl);

    if (Status == STATUS_DONE) {
      DEBUG     ((EFI_D_INFO | EFI_D_ERROR, "    IIO Online is DONE.\n"));
    }

  } else if (mCpuRas->RASEvent == CpuRasOfflineRequest) {
    DEBUG     ((EFI_D_INFO | EFI_D_ERROR, "    IIO %d Offline Detected.\n", mCpuRas->SocketToBeOl));
  Status = mIioRasProtocol->IioOffline(mCpuRas->SocketToBeOl);

    if (Status == STATUS_DONE) {
      DEBUG     ((EFI_D_INFO | EFI_D_ERROR, "    IIO Offline is DONE.\n"));
    }
  }
  return Status;
}
