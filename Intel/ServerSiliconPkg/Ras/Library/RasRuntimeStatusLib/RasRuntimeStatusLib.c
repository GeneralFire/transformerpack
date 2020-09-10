/** @file
  Implementation of saving/reading Ras runtime status. Incluing mirror failover status,
  adddc status etc.

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

#include <Uefi/UefiBaseType.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/RasRuntimeStatusLib.h>
#include <Library/DebugLib.h>


RAS_RUNTIME_STATUS *mRasRuntimeStatus;

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
)
{
  mRasRuntimeStatus->Failover.FailedCh[Socket][Mc] = FailedCh;
  DEBUG ((EFI_D_ERROR, "Faillover happend in socket %d, mc %d, Saving failed channel %d\n", Socket, Mc, FailedCh));
}

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
)
{
  return mRasRuntimeStatus->Failover.FailedCh[Socket][Mc];
}

/**
  The constructor function initialize the Ras runtime status.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
UINTN
EFIAPI
InitRasRuntimeStatusLib(
    VOID
)
{
  BOOLEAN    NewAllocation;
  UINTN      i, j;

  mRasRuntimeStatus = GetRasGlobalData (L"RasRuntimeStatus", sizeof (RAS_RUNTIME_STATUS), &NewAllocation);
  ASSERT (mRasRuntimeStatus != NULL);

  if (NewAllocation) {
    for (i = 0; i < MAX_SOCKET; i++) {
      for (j = 0; j < MAX_IMC; j++) {
        mRasRuntimeStatus->Failover.FailedCh[i][j] = 0xff;
      }
    }
  }
  return EFI_SUCCESS;
}
