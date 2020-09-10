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

//BKL_PORTING - remove this when all macro re-definitions are solved
#ifndef __GNUC__
#pragma warning(disable :4005)
#endif  //__GNUC__
//BKL_PORTING

#include "RasInit.h"

extern  EFI_CPU_RAS_PROTOCOL                        *mCpuRas;
extern  BOOLEAN                                     mIsSoftSmiRequest;
extern  UINT32                                      mSmiParam[4];
extern  UINT8                                       FirstCpuRasEntry;
extern  UINT8                                       mHpRequest[6];
extern  BIOS_ACPI_PARAM                             *mAcpiParameter;
extern  BOOLEAN                                     mExitImmediately;
extern  UINT8                                       mKtiCpuSktHotPlugEn;
extern  CPU_CSR_ACCESS_VAR                          *mCpuCsrAccessVarPtr;


UINT8   CpuOLRequest;
UINT8   TargetSocketIdList;

UINT8
CheckCpuOLRequest (
)
{
  UINT8       Counter;
  UINT8       Counter2;
  UINT8       SocketOccupied;
  UINT8       CurrentCpuStatus;
  UINT8       NumberOfActiveCpus;
  UINT8       CpuJumper;

  DEBUG     ((EFI_D_INFO, "\nCpu Ras Hanlder: CheckCpuOLRequest - Entry\n"));

  CpuOLRequest        = NoRequest;
  CurrentCpuStatus    = 0;
  NumberOfActiveCpus  = 0;

  //
  // Mask off only the CPU0-CPU3 bits for both socket occupied and jumper status.
  // This eliminates the IOH signals.
  //
  SocketOccupied = mHpRequest[1];
  CpuJumper      = mHpRequest[0];

  //
  // Get the current CPU status for all CPUs.  It is assumed that if a CPU gets
  // onlined or offlined during runtime, that the SocketBitMask will always reflect
  // the current state of the CPUs.
  //
  CurrentCpuStatus = (UINT8) (UINT8)mCpuCsrAccessVarPtr->socketPresentBitMap;

  //
  // Sanity check against socket occupied.  That is, no online request from a socket
  // that isn't present.
  //
  CurrentCpuStatus &= SocketOccupied;
  CpuJumper        &= SocketOccupied;

  DEBUG     ((EFI_D_INFO, "\tCheckCpuOLRequest - SocketOccupied = %x\n",SocketOccupied));
  DEBUG     ((EFI_D_INFO, "\tCheckCpuOLRequest - CpuJumper = %x\n",CpuJumper));
  DEBUG     ((EFI_D_INFO, "\tCheckCpuOLRequest - CurrentCpuStatus = %x\n",CurrentCpuStatus));

  //
  // XORing will extract the change in status. Onlined CPU -> Offline request.
  // Offline CPU -> Online request.
  //
  TargetSocketIdList =  CurrentCpuStatus ^ CpuJumper;
  DEBUG     ((EFI_D_INFO, "\tCheckCpuOLRequest - TargetSocketIdList = %x\n",TargetSocketIdList));

  //
  // Loop through all CPUs and look for online/offline requests for each.
  //
  for (Counter = 0; Counter < MAX_SOCKET; Counter++) {
    //
    // If the CPU has a pending online/offline request...
    //
    if (((TargetSocketIdList >> Counter) & 0x01) == 1) {

      //
      // Then if the CPU is currently online, it means the user
      // has requested to offline the CPU.
      //
      if (((CurrentCpuStatus >> Counter) & 0x01) == 1) {
        //
        // If we're requesting an offline, need to make sure that there are at
        // least 2 processors installed in the system before offlining one of them.
        //
        for (Counter2 = 0; Counter2 < MAX_SOCKET; Counter2++) {
          NumberOfActiveCpus += ((((UINT8) mAcpiParameter->SocketBitMask) >> Counter2) & 0x01);
        }
        if (NumberOfActiveCpus > 1) {
          CpuOLRequest = CpuRasOfflineRequest;
          DEBUG     ((EFI_D_INFO, "\tCheckCpuOLRequest - It is OfflineRequest\n"));

        } else {
          CpuOLRequest = NoRequest;
          DEBUG     ((EFI_D_INFO, "\tCheckCpuOLRequest - NoRequest\n"));
        }
      } else {
        CpuOLRequest = CpuRasOnlineRequest;
        DEBUG     ((EFI_D_INFO, "\tCheckCpuOLRequest - It is OnlineRequest\n"));
      }

    }
  }

  return  CpuOLRequest;

}


EFI_STATUS
CpuRasHandler (
    VOID
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       MultipleReqs = 0;
  UINT8       Count = 0;


  //
  // If the user has requested a soft SMI, it means the mHpRequest jumper values have been hard coded for a specific
  // type of RAS event.  We can only proceed with the CpuRasHandler if the user has requested a CPU event.
  //
  if (mIsSoftSmiRequest && ((mSmiParam[0] != SMI_CPU_ONLINE_OFFLINE) && (mSmiParam[0] != SMI_CPU_HOT_ADD_REMOVE))) {
    return NO_REQUEST;
  }

  //
  // Check the setup option for QpiCpuSktHotPlugEn.  If it is not enabled, don't service the Online/offline request
  //
  DEBUG((EFI_D_LOAD|EFI_D_INFO, "\n The value of KtiCpuSktHotPlugEn is %d\n",mKtiCpuSktHotPlugEn));
  if (mKtiCpuSktHotPlugEn != 1) {
    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nCPU RAS HANDLER: PLEASE ENABLE CPU HOTPLUG IN QPI SETUP TO PROCEED FURTHER WITH SKT ONLINE/OFFLINE\n"));
    return NO_REQUEST;
  }

  if (FirstCpuRasEntry) {

    DEBUG((EFI_D_LOAD|EFI_D_INFO, "\nFirst Time Entry to CPU Ras Handler\n"));
    FirstCpuRasEntry  = 0;
    mCpuRas->FirstCpuRasEntry = 1;

    //
    // Check the request corresponds to Online or Offline request
    //
    CpuOLRequest  = CheckCpuOLRequest();

    //
    // Return NO_REQUEST if the request is not for CPU.
    //
    if (CpuOLRequest == NoRequest) {
      mExitImmediately = TRUE;
      return NO_REQUEST;
    }


    //
    // Update the SocketID and Multiple online and Offline Reqs.
    //
    for (Count = 0; Count <= MAX_SOCKET; Count++)  {
      if ((TargetSocketIdList >> Count ) & 01)  {
        MultipleReqs++;
        mCpuRas->SocketToBeOl = Count;
      }
    }


    //
    // Return error if multiple requests come.
    //
    if (MultipleReqs > 01) {
      DEBUG     ((EFI_D_INFO | EFI_D_ERROR, "    Detected more than 1 jumper change. No RAS Request will be processed.\n"));
      mExitImmediately = TRUE;
      return  ERR_CPU_MULTIPLE_REQ;
    }

    //
    // Assert error if there is no request.
    //
    if (MultipleReqs == 00) {
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    }

    mCpuRas->RASEvent = CpuOLRequest;
  }



  if (CpuOLRequest == CpuRasOnlineRequest) {
    Status = mCpuRas->CpuRasOnline();
    if (Status == ERROR_EXIT) {
      mExitImmediately = TRUE;
    }
  } else if (CpuOLRequest == CpuRasOfflineRequest) {
    Status = mCpuRas->CpuRasOffline();
    if (Status == NO_SUPPORT || Status == ERROR_EXIT || Status == STATUS_DONE) {
      mExitImmediately = TRUE;
    }
  }

  return Status;
}
