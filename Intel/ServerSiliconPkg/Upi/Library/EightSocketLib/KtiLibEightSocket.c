/** @file
  This files contains funcitons which are used for 8S support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#include <KtiMisc.h>
#include <Library/KtiApi.h>
#include "EightSocketInternal.h"
#include <Library/CpuAndRevisionLib.h>

/**
  Reduces the degraded 8S topology to 6S topology if possible. Otherwise it will reduced using
  degraded 4S handling rules.

  This function should be called only when the system has degraded 4S topology.

  @param SocketData        - Socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval 0 - Successful

**/
KTI_STATUS
CheckForDegraded8S (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  UINT8   Idx1, Idx2;
  BOOLEAN Degraded;
  BOOLEAN IsCpx;
  BOOLEAN DegradeDone;

  BOOLEAN IsSpr;
  IsSpr = FALSE;
  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    IsSpr = TRUE;
  }

  IsCpx = FALSE;
  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    IsCpx = TRUE;
  }

  if (IsCpx || IsSpr) {
    DegradeDone = CheckEightSocketTwoClumpTopology (SocketData, KtiInternalGlobal);
    if (DegradeDone) {
      return KTI_SUCCESS;
    }
  }

  CheckTopologyType (SocketData, KtiInternalGlobal);

  //
  // Check if all the links of all the 8 sockets are valid
  //
  Degraded = FALSE;
  for (Idx1 = 0; Idx1 < 8; Idx1++) {
    if ((SocketData->Cpu[Idx1].Valid == TRUE) && (SocketData->Cpu[Idx1].SocType == SOCKET_TYPE_CPU)) {
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid != TRUE) {
          Degraded = TRUE;
          break;
        }
      }
      if (Degraded == TRUE) {
        break;
      }
    }
  }

  if (Degraded == FALSE) {
    KTIVAR->SysConfig = SYS_CONFIG_8S;
    return KTI_SUCCESS;
  }

  //
  // It is a degraded 8S topology; reduce it to one of the supported topology.
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n  Degraded 8S topology is detected."));

  HandleDegraded8S (SocketData, KtiInternalGlobal);

  return KTI_SUCCESS;
}

